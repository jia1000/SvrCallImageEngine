
#include "series_data_info.h"

#include "common_utils.h"
#include "global_define.h"

#include <string.h>
#include <limits>

SeriesDataInfo::SeriesDataInfo(const std::string path, bool is_folder)
: m_src_path(path)
, m_pixel_data_buffer(nullptr)
, m_pixel_data_length(0)
{
    m_bases.clear();
    
    if (is_folder)
    {
        ReadFolder(path);
    }
    else 
    {
        ReadFile(path);
    }    
}

SeriesDataInfo::~SeriesDataInfo()
{
    if (m_pixel_data_buffer)
    {
        delete m_pixel_data_buffer;  
        m_pixel_data_buffer = nullptr;      
    }    
}
unsigned char* SeriesDataInfo::GetPixelDataBuffer()
{    
    m_pixel_data_length = GetPixelDataLength();

    if (m_pixel_data_buffer)
    {
        delete m_pixel_data_buffer;  
        m_pixel_data_buffer = nullptr;      
    }

    m_pixel_data_buffer = new unsigned char[m_pixel_data_length];
    if (!m_pixel_data_buffer)
    {
        printf("memory too large, can't assigne space.\n");
        return nullptr;
    }
    
    int pos = 0;
    for (auto iter = m_bases.begin(); iter != m_bases.end(); iter++)
    {
        int cur_len = iter->second.tagBinary.GetSize();
        unsigned char* cur_buf = iter->second.tagBinary.GetValor();
        memcpy(m_pixel_data_buffer + pos, 
            cur_buf, 
            cur_len * sizeof(unsigned char));
        pos += cur_len;
    }

    return m_pixel_data_buffer;
}

unsigned int SeriesDataInfo::GetPixelDataLength()
{
    int total_len = 0;
	for (auto iter = m_bases.begin(); iter != m_bases.end(); iter++)
    {
        // int cur_len = iter->second.tagBinary.GetSize();
        // printf("cur _len : %d\n", cur_len);
        total_len += iter->second.tagBinary.GetSize();
    }
    
    return total_len;
}

int SeriesDataInfo::ReadFolder(const std::string& path)
{
    // traversal the whole folder
    std::vector<std::string> vFiles;
    ListDir(path, vFiles);

    for (auto iter = vFiles.begin(); iter != vFiles.end();)
    {
        int ret = ReadFile(*iter);
        if (ret >= RET_STATUS_SUCCESS)
        {
            ++iter;
        }
        else {
            iter = vFiles.erase(iter);            
        }
    }
    
    return RET_STATUS_SUCCESS;
}

int SeriesDataInfo::ReadFile(const std::string& file_name) 
{
    GIL::DICOM::IDICOMManager*	pDICOMManager = new GIL::DICOM::DICOMManager();
	if(pDICOMManager) 
	{
        DicomInfo dicom_info;        
        std::string file_path = m_src_path + file_name;
		
        bool ret = pDICOMManager->CargarFichero(file_path, dicom_info.base);
        
        if (!ret)
        {
            printf("parse dicom error : %s\n", file_name.c_str());
            return RET_STATUS_DICOM_NOT_FILE;
        }
        bool target = pDICOMManager->GetTag(0x7fe0, 0x0010, dicom_info.tagBinary); 

        if (!target)
        {
            printf("parse fixel data error : %s\n", file_name.c_str());
            return RET_STATUS_DICOM_NOT_FIND_PIXELDATA;
        }       
        
        m_bases.insert(make_pair(file_name, dicom_info));
                
        delete pDICOMManager;
		pDICOMManager = NULL;
    }

    return RET_STATUS_SUCCESS;
}

void SeriesDataInfo::GetTag(const std::string& tag, int& i)
{
    std::string value("");
    auto iter = m_bases.begin();
    if (iter != m_bases.end())
    {
        GIL::DICOM::DicomDataset base = iter->second.base;
        if(base.getTag(tag, value))
        {
            std::stringstream ss(value);
            ss >> i;
        } 
    }
    return;
}
void SeriesDataInfo::GetTag(const std::string& tag, double& d)
{
    std::string value("");
    auto iter = m_bases.begin();
    if (iter != m_bases.end())
    {
        GIL::DICOM::DicomDataset base = iter->second.base;
        if(base.getTag(tag , value))
        {
            std::stringstream ss(value);
            ss >> d;
        }
    }
}

void SeriesDataInfo::GetTag(const std::string& tag, std::string& s)
{
    std::string value("");
    auto iter = m_bases.begin();
    if (iter != m_bases.end())
    {
        GIL::DICOM::DicomDataset base = iter->second.base;
        if(base.getTag(tag , value))
        {
            s = value;
        }
    }
}

int SeriesDataInfo::GetSeriesDicomFileCount()
{
    int ret = m_bases.size();
    return ret;
}
int SeriesDataInfo::GetDicomDataSet(GIL::DICOM::DicomDataset& base, const int slice_index = 0)
{
    int size = m_bases.size();
    if (slice_index < 0 || slice_index >= size)
    {
        return RET_STATUS_DICOM_NOT_SLICE;
    }
    
    auto iter = m_bases.begin();
    int index = 0;
    for (; iter != m_bases.end(); ++iter, ++index)
    {
        if (index == slice_index)
        {
            base = iter->second.base;
            return RET_STATUS_SUCCESS;
        }        
    }
    
    return RET_STATUS_DICOM_NOT_TAGS;
}

int SeriesDataInfo::GetDicomDicomParas(DicomParas& paras, const int slice_index)
{
    GIL::DICOM::DicomDataset data_set;
    int ret = GetDicomDataSet(data_set, slice_index);

    if (ret != RET_STATUS_SUCCESS)
    {
        return ret;
    }
    
    // calculate position
    char c = 0;
    std::string tag("");
    if(data_set.getTag(GKDCM_ImagePositionPatient, tag))
    {
        bool status = true;
        std::stringstream istr(tag);
        for (size_t i = 0; i < POSITION_LENGHT; i++)
        {
            if (status && !istr.eof())
            {
                istr >> paras.position[i];
                if (!istr.eof())
                {
                    istr >> c;
                }                        
            }
            else 
            {
                status = false;
            }   
            // printf("dicom_info.position[%d] : %f\n", (int)i, dicom_info.position[i]);                 
        }
        if (!status)
        {
            for (size_t i = 0; i < POSITION_LENGHT; i++)
            {
                paras.position[i] = 0.0f;
            }
        } 
    }
    
    if(data_set.getTag(GKDCM_WindowWidth, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.window_width;
    }
    if(data_set.getTag(GKDCM_WindowCenter, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.window_level;
    }
    if(data_set.getTag(GKDCM_BitsAllocated, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.bits_per_pixel;
    }
    if(data_set.getTag(GKDCM_BitsStored, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.bits_stored;
    }
    if(data_set.getTag(GKDCM_Rows, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.height;
    }
    if(data_set.getTag(GKDCM_Columns, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.width;
    }
    if(data_set.getTag(GKDCM_BitsAllocated, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.bits_allocated;
    }
    if(data_set.getTag(GKDCM_BitsStored, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.bits_stored;
    }

    if(data_set.getTag(GKDCM_StudyID, tag))
    {
        paras.study_id = tag;
    }
    if(data_set.getTag(GKDCM_SeriesInstanceUID, tag))
    {
        paras.series_iuid = tag;
    }
    // if(data_set.getTag(GKDCM_ImageOrientationPatient, tag))
    // {
    //     paras.orientation = tag;
    // }
    if(data_set.getTag(GKDCM_SliceLocation, tag))
    {
        std::stringstream ss(tag);
        ss >> paras.slice_location;
    }

    GetSpacing(slice_index, paras.spacing[0], paras.spacing[1], paras.spacing[2]);
    
    paras.slice_count = GetSeriesDicomFileCount();

    return RET_STATUS_SUCCESS;
}

bool SeriesDataInfo::SaveDicomFile(
	const std::string src_path_file, const std::string dst_path_file)
{
	SeriesDataInfo data_info(src_path_file, false);

	double thickeness = 1.1f;
	data_info.GetTag(GKDCM_SliceThickness, thickeness);
	//printf("thickness : %.5f\n", thickeness);

	GIL::DICOM::DicomDataset base;
	GIL::DICOM::IDICOMManager*	pDICOMManager = new GIL::DICOM::DICOMManager();
	if(pDICOMManager) 
	{
		pDICOMManager->CargarFichero(src_path_file, base);
		std::string str_tag("");
		base.getTag(GKDCM_PatientName , str_tag);
		//printf("patient name : %s(use DicomManager)\n", str_tag.c_str());

		// modify one tag
		GIL::DICOM::DicomDataset modify_base;		
		modify_base.tags["0010|0010"] = "test 555";
		pDICOMManager->ActualizarJerarquia(modify_base);
		
		// save dicom file
		pDICOMManager->AlmacenarFichero(dst_path_file);
	
		delete pDICOMManager;
		pDICOMManager = NULL;
		return true;
	}
	return false;
}

bool SeriesDataInfo::GetSpacing(const int indice, 
    double& x, double& y, double& z, 
    bool isGetZAsThinkness)
{
    bool hasSpacing = false;
    std::string spacing;
    x=0.0f;
    y=0.0f;
    z=0.0f;
    GIL::DICOM::DicomDataset tagsImage;
    int ret =GetDicomDataSet(tagsImage, indice);
    if (ret != RET_STATUS_SUCCESS)
    {
        return ret;
    }

    
    if(tagsImage.getTag(GKDCM_PixelSpacing,spacing)) {
        char c;
        std::istringstream issl(spacing);
        issl >> x;
        if(!issl.eof()){
            issl>>c;//la barra
            issl>>y;
        }
        if(!issl.eof()){
            issl>>c;//la barra
            issl>>z;
        }
        hasSpacing = true;
    }
    else if(tagsImage.getTag(GKDCM_ImagerPixelSpacing,spacing)) {
        char c;
        std::istringstream issl(spacing);
        issl >> x;
        if(!issl.eof()){
            issl>>c;//la barra
            issl>>y;
        }
        if(!issl.eof()){
            issl>>c;//la barra
            issl>>z;
        }
        hasSpacing = true;
    }
    
    if (x < std::numeric_limits<double>::epsilon()) {
        x = 1.0f;
        hasSpacing = false;
    }
    if (y < std::numeric_limits<double>::epsilon()) {
        y = 1.0f;
        hasSpacing = false;
    }
    if (z < std::numeric_limits<double>::epsilon()) {
        if (isGetZAsThinkness) {
            if(tagsImage.getTag(GKDCM_SliceThickness,spacing)) { //slice thickness
                std::istringstream issl(spacing);
                issl >> z;
            }
        }
        else {
            z = 1.0f;
        }
    }
    return hasSpacing;
}
