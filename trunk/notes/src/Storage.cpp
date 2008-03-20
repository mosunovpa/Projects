#include "StdAfx.h"
#include "Storage.h"
#include "resource.h"
#include "fileutils.h"
#include "resutils.h"

//////////////////////////////////////////////////////////////////////////
//

#define CHECK_HR_MSG(x, s) { HRESULT hr = x; if (FAILED(hr)) ThrowError(s); }
#define CHECK_HR(x) CHECK_HR_MSG(x, _T("Xml operation error"))

CString GetDataFileName()
{
	TCHAR szFile[MAX_PATH];
	::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, szFile);
	::PathCombine(szFile, szFile, RESSTR(IDS_APP_NAME));
	if (!::PathFileExists(szFile))
	{
		fileutils::CreateDirectoryRecursive(szFile);
	}
	::PathCombine(szFile, szFile, _T("notes.dat"));
	return szFile;
}

CComPtr<IXMLDOMDocument> GetDocument()
{
	CComPtr<IXMLDOMDocument> spDoc;
	CHECK_HR_MSG(spDoc.CoCreateInstance(__uuidof(DOMDocument)), _T("Create document error"));
	CString sFileName = GetDataFileName();
	VARIANT_BOOL bSuccess = false;
	if (::PathFileExists(sFileName))
	{
		CHECK_HR_MSG(spDoc->load(CComVariant(sFileName), &bSuccess), _T("Load file error"));
	}
	else
	{

		CComBSTR sXml(_T("<?xml version=\"1.0\"?><notes><options></options></notes>"));
		CHECK_HR_MSG(spDoc->loadXML(sXml, &bSuccess), _T("Load xml error"));
	}
	return spDoc;
}

CComPtr<IXMLDOMNode> GetRootNode(CComPtr<IXMLDOMDocument>& spDoc)
{
	CComPtr<IXMLDOMNode> spRoot;
	CHECK_HR_MSG(spDoc->selectSingleNode(CComBSTR(_T("notes")),&spRoot), _T("Root node not found"));
	return spRoot;
}

int GetNextId(CComPtr<IXMLDOMDocument>& spDoc)
{
	long len = 0;
	std::vector<int> ids;
	CComPtr<IXMLDOMNodeList> spNotes;
	CHECK_HR(spDoc->getElementsByTagName(L"note", &spNotes));
	CHECK_HR(spNotes->get_length(&len));
	for (int i = 0; i < len; ++i)
	{
		CComPtr<IXMLDOMNode> spNode;
		CHECK_HR(spNotes->get_item(i, &spNode));
		CComPtr<IXMLDOMElement> spElement;
		CHECK_HR(spNode.QueryInterface(&spElement));
		CComVariant val;
		CHECK_HR(spElement->getAttribute(L"id", &val));
		ids.push_back(val.intVal);
	}
	std::sort(ids.begin(), ids.end());
	int nNextId = 1;
	for (int i = 0; i < ids.size(); ++i)
	{
		if (nNextId != ids[i])
		{
			return nNextId;
		}
		++nNextId;
	}
	return nNextId;
}

void SetNote(CComPtr<IXMLDOMElement>& spElement, CNote const& note)
{
	CHECK_HR(spElement->setAttribute(L"id", CComVariant(note.GetId())));
	CHECK_HR(spElement->put_text(CComBSTR(note.GetText())));
}

void NewNote(CNote& note)
{
	CComPtr<IXMLDOMDocument> spDoc = GetDocument();
	CComPtr<IXMLDOMElement> spElement;
	note.SetId(GetNextId(spDoc));
	CHECK_HR(spDoc->createElement(L"note", &spElement));
	SetNote(spElement, note);
	CHECK_HR(spDoc->save(CComVariant(GetDataFileName())));
}

void UpdateNote(CNote const& note)
{

}

//////////////////////////////////////////////////////////////////////////
//

CStorage::CStorage(void)
{
}

CStorage::~CStorage(void)
{
}

void CStorage::SaveNote(CNote& note)
{
	if (note.GetId() == 0) // new note
	{
		NewNote(note);
	}
	else
	{
		UpdateNote(note);
	}
}

int CStorage::GetNotesCount() const
{
	long len = 0;
	CComPtr<IXMLDOMDocument> spDoc = GetDocument();
	CComPtr<IXMLDOMNodeList> spNotes;
	CHECK_HR(spDoc->getElementsByTagName(L"note", &spNotes));
	CHECK_HR(spNotes->get_length(&len));
	return len;
}

void CStorage::GetAllNotes(CStorage::NotesList& list)
{

}

void CStorage::DeleteNote(int nNoteId)
{
	if (nNoteId > 0)
	{

	}
}