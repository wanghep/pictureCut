
// pictureCut.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpictureCutApp:
// �йش����ʵ�֣������ pictureCut.cpp
//

class CpictureCutApp : public CWinApp
{
public:
	CpictureCutApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpictureCutApp theApp;