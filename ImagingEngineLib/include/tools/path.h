/*=========================================================================

  Program:   ImagingEngine
  Module:    path.h
  author: 	 zhangjian
  Brief:	 提供文件目录操作方法

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
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
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
	str2char(dir, des_dir); // 将string 写入到字符数组中
	int state = access(des_dir, R_OK|W_OK); // 头文件 #include <unistd.h>
	if (state==0) {
		//cout<<"file exist"<<endl;
		return true;
	}
	else if (mkdir_flag){
		dir = "mkdir " + dir;
		str2char(dir, des_dir);
		//cout<<des_dir<<endl;
		system(des_dir); // 调用linux系统命令创建文件

		return true;
	}
	else{
		return false;
	}
}

#endif
