/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_image_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_image_source.h"

using namespace DW::IMAGE;
using namespace DW::Batch;


BufferResult *BatchImageSource::GetImage(int index)
{
	if (index < buffer_list_.size())
		return buffer_list_[index];

	return NULL;
}

void BatchImageSource::AddImage(BufferResult *buffer)
{
	buffer_list_.push_back(buffer);
}

int BatchImageSource::GetImageNumber()
{
	return buffer_list_.size();
}

void BatchImageSource::OnBatchOneGenerated(BufferResult* results, BYTE status)
{
	buffer_list_.push_back(results);

	//TODO ֪ͨǰ�����ɽ���
}

void BatchImageSource::OnBatchAllGenerated(BYTE status)
{
	//TODO ֪ͨǰ���������
}