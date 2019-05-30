


#include "Controller.h"

#include "./DicomEngine/api/studycontextmy.h"
#include "./DicomEngine/main/controllers/dicommanager.h"
#include "./DicomEngine/api/dicom/dicomdataset.h"
#include "./DicomEngine/api/dicom/dcmdictionary.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diargimg.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcdict.h"

int Test_Controller(const std::string src_path_file, const std::string dst_path_file) 
{
	printf("Test_Controller()\n");

	//const std::string src_path_file("./10.dcm");
	//const std::string dst_path_file("./20.dcm");

	GIL::DICOM::DicomDataset base;
	GIL::DICOM::IDICOMManager*	pDICOMManager = new GIL::DICOM::DICOMManager();
	if(pDICOMManager) 
	{
		pDICOMManager->CargarFichero(src_path_file, base);
		std::string str_tag("");
		base.getTag(GKDCM_PatientName , str_tag);
		printf("patient name : %s(use DicomManager)\n", str_tag.c_str());

		// modify one tag
		GIL::DICOM::DicomDataset modify_base;		
		modify_base.tags["0010|0010"] = "test 555";
		pDICOMManager->ActualizarJerarquia(modify_base);
		
		// save dicom file
		pDICOMManager->AlmacenarFichero(dst_path_file);
	
		delete pDICOMManager;
		pDICOMManager = NULL;
	}
	return 0;
}
