// stdafx.cpp : 只包括标准包含文件的源文件
// weijian.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <InitGuid.h>
// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用


#include "..\..\include\ACCef\ACCef.c"
//#include "..\baseclass\sha1.c"


string MakeBucketUrl()
{
// 	string strUrl = COS_BucketUrlProd;
// 	wstring wstrHost = TestModeHelper()->GetString(L"TestMode", L"BucketUrl", L"");
// 
// 	if (!wstrHost.empty())
// 	{
// 		CUTFString utf(wstrHost.c_str());
// 		strUrl = utf.ANSI();
// 	}

//	return strUrl;
	return "";
}


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdlib.h>


using namespace std;


//从文件读入到string里
string readFileIntoString(const char * filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}
