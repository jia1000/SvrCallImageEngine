
#include "series_data_info.h"
#include "data_transfer_control.h"

#include "common_utils.h"
#include "global_define.h"

#include <string.h>
#include <limits>

SeriesDataInfo::SeriesDataInfo(const std::string path)
: m_src_path(path)
, m_pixel_data_buffer(nullptr)
, m_pixel_data_length(0)
{        
}

SeriesDataInfo::~SeriesDataInfo()
{      
}

int SeriesDataInfo::ReadDicomFilesFromDir(bool is_folder)
{
    m_bases.clear();
    if (is_folder)
    {
        ReadFolder(m_src_path);
    }
    else 
    {
        ReadFile(m_src_path);
    }
}

unsigned char* SeriesDataInfo::GetPixelDataBuffer()
{    
    m_pixel_data_length = GetPixelDataLength();

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
            // 如果读取dicom文件失败，需要从成员vFiles，删除。
            iter = vFiles.erase(iter);            
        }
    }
    
    // 输出Dicom文件信息
    int index = 0;
    for (auto iter = m_bases.begin(); iter != m_bases.end();  ++iter, ++index)
    {
        //printf("%03d : %.5f, %s\n", index, iter->first, iter->second.file_name.c_str());
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
		dicom_info.file_name = file_name;

        // query base_set
        bool ret = pDICOMManager->CargarFichero(file_path, dicom_info.base);
        
        if (!ret)
        {
            delete pDICOMManager;
		    pDICOMManager = NULL;
            printf("parse dicom error : %s\n", file_name.c_str());
            return RET_STATUS_DICOM_NOT_FILE;
        }
        // query pixel_data
        bool target = pDICOMManager->GetTag(0x7fe0, 0x0010, dicom_info.tagBinary); 

        if (!target)
        {
            delete pDICOMManager;
		    pDICOMManager = NULL;
            printf("parse fixel data error : %s\n", file_name.c_str());
            return RET_STATUS_DICOM_NOT_FIND_PIXELDATA;
        }       
        
        // calc patient_position
        double position[POSITION_LENGHT] = {0.0f};
        GetPosition(dicom_info.base, position);

        m_bases.insert(std::make_pair(position[2], dicom_info));
                
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
    std::string tag("");
    
    // calculate position
    GetPosition(data_set, paras.position);    
    
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
    // 
    if(data_set.getTag(GKDCM_PatientID, tag))
    {
        std::string id = DataTransferController::GetInstance()->GetPatientid();
        paras.patient_id = id;
    }    
    if(data_set.getTag(GKDCM_StudyID, tag))
    {
        std::string id = DataTransferController::GetInstance()->GetStudyuid();
        paras.study_id = id;
    }
    if(data_set.getTag(GKDCM_SeriesInstanceUID, tag))
    {
        std::string id = DataTransferController::GetInstance()->GetSeriesuid();
        paras.series_iuid = id;
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

int SeriesDataInfo::GetPosition(GIL::DICOM::DicomDataset& base, double position[POSITION_LENGHT])
{
    char c = 0;
    std::string tag("");
    if(base.getTag(GKDCM_ImagePositionPatient, tag))
    {
        bool status = true;
        std::stringstream istr(tag);
        for (size_t i = 0; i < POSITION_LENGHT; i++)
        {
            if (status && !istr.eof())
            {
                istr >> position[i];
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
                position[i] = 0.0f;
            }
        } 
    }
    return 1;
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
