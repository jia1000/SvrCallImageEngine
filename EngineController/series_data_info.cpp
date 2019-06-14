
#include "series_data_info.h"

#include "common_utils.h"
#include "global_define.h"

#include <string.h>

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

        // printf("cur_len : 0x%04x\n", cur_len);
        // printf("hello buffer:\n");
        // int count = 0;
        // for (size_t i = 0; i < cur_len; i++)
        // {
        //     unsigned char c = *(cur_buf + i);
        //     if (c != 0)
        //     {
        //         printf(" %02x ", c);
        //         count++;
        //     }            
            
        //     if (count % 16 == 15)
        //     {
        //         printf("\n");
        //     }            
        // }

        // printf("count : %d\n", count);
        

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
        
        // calculate position
        char c = 0;
        std::string tag("");
        if(dicom_info.base.getTag(GKDCM_ImagePositionPatient, tag))
        {
            bool status = true;
            std::stringstream istr(tag);
            for (size_t i = 0; i < POSITION_LENGHT; i++)
            {
                if (status && !istr.eof())
                {
                    istr >> dicom_info.position[i];
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
                    dicom_info.position[i] = 0.0f;
                }
            }  
            
        
        }

        //
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
    int ret = 0;
    auto iter = m_bases.begin();
    if (iter != m_bases.end())
    {
        ret = iter->second.base.tags.size();
    }
    return ret;
}
void SeriesDataInfo::GetDicomDataSet(GIL::DICOM::DicomDataset& base)
{
    auto iter = m_bases.begin();
    if (iter != m_bases.end())
    {
        base = iter->second.base;
    }
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