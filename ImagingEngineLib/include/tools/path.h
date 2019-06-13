/*=========================================================================

  Program:   ImagingEngine
  Module:    path.h
  author: 	 zhangjian
  Brief:	 �ṩ�ļ�Ŀ¼��������

=========================================================================*/
#pragma once
#include <iostream>
#include <cstdio>

using namespace std;

#ifdef WIN32
#include  <io.h>
#include <atlstr.h>
#include <windows.h>

inline bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return true;
}

inline bool DeleteFile(const CString& szPath)
{
	return DeleteFile(szPath);
}
#else
#include <unistd.h>
#include <sys/stat.h>


int str2char(string s, char c[])
{
	size_t l = s.length();
	int i;
	for(i = 0; i < l; i++)
		c[i] = s[i];

	c[i] = '\0';

	return i;
}


int CreateDirectoryEx(const char *sPathName )  
{
	char DirName[256];
	strcpy(DirName,sPathName);
	int i,len = strlen(DirName);
	if(DirName[len-1]!='/')
		strcat(DirName,"/");            

	len = strlen(DirName);
	for(i=1;i<len;i++){
		if(DirName[i]=='/'){
			DirName[i] = 0;
			int a = access(DirName, F_OK);
			if(a ==-1){
				mkdir(DirName,0755);
			}
			DirName[i] = '/';
		} 
	}
	return 0;
}  

bool PathFileExists(string dir, bool mkdir_flag)
{
	char des_dir[255];
	str2char(dir, des_dir); // ��string д�뵽�ַ�������
	int state = access(des_dir, R_OK|W_OK); // ͷ�ļ� #include <unistd.h>
	if (state==0) {
		//cout<<"file exist"<<endl;
		return true;
	}
	else if (mkdir_flag){
		dir = "mkdir " + dir;
		str2char(dir, des_dir);
		//cout<<des_dir<<endl;
		system(des_dir); // ����linuxϵͳ������ļ�

		return true;
	}
	else{
		return false;
	}
}

#endif
