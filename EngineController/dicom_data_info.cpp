
#include "dicom_data_info.h"

#include <string.h>

SeriesDataInfo::SeriesDataInfo(const std::string path, bool is_folder)
: m_src_path(path)
, m_pixel_data_buffer(nullptr)
, m_pixel_data_length(0)
{
    m_bases.clear();
    
    if (is_folder)
    {
        ReadFolder();
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
        printf("memory too large, can't asigned space.\n");
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
        total_len += iter->second.tagBinary.GetSize();
    }
    
    return total_len;
}

int SeriesDataInfo::ReadFolder()
{
    // traversal the whole folder
    std::vector<std::string> vFiles;
    vFiles.push_back("1.dcm");
    vFiles.push_back("2.dcm");
    vFiles.push_back("3.dcm");

    for (size_t i = 0; i < vFiles.size(); i++)
    {
        ReadFile(vFiles.at(i));
    }
    
    return 1;
}

int SeriesDataInfo::ReadFile(const std::string& file_name) 
{
    GIL::DICOM::IDICOMManager*	pDICOMManager = new GIL::DICOM::DICOMManager();
	if(pDICOMManager) 
	{
        DicomInfo dicom_info;        
        std::string file_path = m_src_path + file_name;
		
        bool ret = pDICOMManager->CargarFichero(file_path, dicom_info.base);
        if (ret)
        {
            pDICOMManager->GetTag(0x7fe0, 0x0010, dicom_info.tagBinary); 
            
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
                    //printf("dicom_info.position[%d] : %f\n", i, dicom_info.position[i]);                 
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
        }
        
        delete pDICOMManager;
		pDICOMManager = NULL;
    }

    return 1;
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
            s = tag;
        }
    }
}
