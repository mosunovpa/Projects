/**
 * \file NoteModuleImpl.cpp
 *
 * \since 2006/07/28
 * \author pavel
 */

#include "StdAfx.h"
#include "resource.h"
#include "assistres.h"
#include "resutils.h"
#include "NotesModuleImpl.h"
#include "NoteWnd.h"

CNotesModuleImpl::CNotesModuleImpl(Assist::IApplication* pApplication)
:	CNotesModuleImplBase(pApplication)
{
}

CNotesModuleImpl::CNotesModuleImpl( CNotesModuleImpl const& right )
:	CNotesModuleImplBase(NULL)
{
	assert(false);
}

CNotesModuleImpl::~CNotesModuleImpl()
{
}

void CNotesModuleImpl::CheckStorage(Assist::IDocStorage* pStorageDef)
{
	try
	{
		CNotesModuleImplBase::CheckStorage(pStorageDef);
// 		switch (pStorageDef->GetStorageVer())
// 		{
// 		case 0: // storage is not exist
// 			_tstring script = resutils::reshtml(IDR_UPDATE_SQL_0);
// 			pStorageDef->Update(script.c_str(), 0x100);
//		}
	}
	RAISE()
}

void CNotesModuleImpl::NewItem(int nParentId)
{
	CNoteWnd* pNoteWnd = new CNoteWnd(nParentId);
	pNoteWnd->Create(NULL, CWindow::rcDefault, _T("NoteWnd"), 
		WS_POPUPWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		WS_THICKFRAME | WS_MINIMIZEBOX, WS_EX_NOPARENTNOTIFY/*WS_CAPTION | , WS_EX_TOOLWINDOW | WS_EX_APPWINDOW*/
		//  		WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | 
		//  		WS_THICKFRAME | WS_OVERLAPPED /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/, 
		//  		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE | WS_EX_APPWINDOW
		);
}

void CNotesModuleImpl::OpenItem(int nId)
{

}
