
// StuManageSystemServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStuManageSystemServerApp: 
// �йش����ʵ�֣������ StuManageSystemServer.cpp
//

class CStuManageSystemServerApp : public CWinApp
{
public:
	CStuManageSystemServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStuManageSystemServerApp theApp;