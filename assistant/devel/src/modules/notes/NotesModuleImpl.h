/**
 * \file NotesModuleImpl.h
 *
 * \since 2006/07/28
 * \author pavel
 */

#ifndef NotesModuleImpl_h__
#define NotesModuleImpl_h__

#include "resource.h"
#include "ModuleImpl.h"
#include "NoteListPane.h"
#include "NotePropsPane.h"
#include "Application.h"
#include "simplesinglton.h"

typedef CModuleImplT
	<
	3,
	IDS_NOTES, 
	IDS_ITEM_NOTE,
	IDB_NOTES, 
	IDB_NOTES_SM,
	Views::CNoteListPane,
	Views::CNotePropsPane
	>
	CNotesModuleImplBase;

class CNotesModuleImpl : public CNotesModuleImplBase,
						public CSimpleSinglton2<CNotesModuleImpl, Assist::IApplication>
{
	friend class CSimpleSinglton2<CNotesModuleImpl, Assist::IApplication>;
public:
	virtual void NewItem(int nParentId) override;
	virtual void OpenItem(int nId) override;

protected:
	CNotesModuleImpl(Assist::IApplication* pApplication);
	CNotesModuleImpl(CNotesModuleImpl const& right);
	virtual ~CNotesModuleImpl();
	virtual void CheckStorage(Assist::IDocStorage* pStorageDef);
};

#endif // NotesModuleImpl_h__
