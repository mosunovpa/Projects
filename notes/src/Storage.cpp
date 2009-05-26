#include "StdAfx.h"
#include "Storage.h"
#include "resource.h"
#include "fileutils.h"
#include "Application.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////////
//

#define CHECK_HR_MSG(x, s) { HRESULT hr = x; if (FAILED(hr)) ThrowError(s); }
#define CHECK_HR(x) CHECK_HR_MSG(x, _T("Xml operation error"))

static CComPtr<IXMLDOMDocument> s_spDoc = NULL;

static CComPtr<IXMLDOMDocument> _GetDocument()
{
	if (s_spDoc == NULL)
	{
		CHECK_HR_MSG(s_spDoc.CoCreateInstance(__uuidof(DOMDocument)), _T("Create document error"));
		LPCTSTR sFileName = CApplication::Get().GetDataFileName();
		VARIANT_BOOL bSuccess = false;
		if (::PathFileExists(sFileName))
		{
			CHECK_HR_MSG(s_spDoc->load(CComVariant(sFileName), &bSuccess), _T("Load file error"));
		}
		else
		{

			CComBSTR sXml(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><notes><options></options></notes>"));
			CHECK_HR_MSG(s_spDoc->loadXML(sXml, &bSuccess), _T("Load xml error"));
		}
	}
	return s_spDoc;
}

static CComPtr<IXMLDOMNode> _GetRootNode(CComPtr<IXMLDOMDocument>& spDoc)
{
	CComPtr<IXMLDOMNode> spRoot;
	CHECK_HR(spDoc->selectSingleNode(CComBSTR(_T("notes")),&spRoot));
	if (!spRoot)
	{
		ThrowError(_T("Root node not found"));
	}
	return spRoot;
}

static CComPtr<IXMLDOMNode> _FindNote(CComPtr<IXMLDOMDocument>& spDoc, int nNoteId)
{
	CComPtr<IXMLDOMNode> spNode;
	_tstring sPath = strutils::format(_T("notes/note[@id=%d]"), nNoteId);
	CHECK_HR(spDoc->selectSingleNode(CComBSTR(sPath.c_str()), &spNode));
	if (!spNode)
	{
		ThrowError(strutils::format(_T("Note <%s> not found"), sPath).c_str());
	}
	return spNode;
}

static CNote _GetNote(CComPtr<IXMLDOMNode> spNode, UINT nMask)
{
	CComPtr<IXMLDOMElement> spElement;
	CHECK_HR(spNode.QueryInterface(&spElement));
	CNote note;
	CComVariant val;

	if ((nMask & CApplication::NM_ID) == CApplication::NM_ID)
	{
		CHECK_HR(spElement->getAttribute(L"id", &val));
		if (val.vt != VT_BSTR)
		{
			ThrowError(_T("Attribute id not found"));
		}
		note.SetId(_ttoi(val.bstrVal));
	}
	if ((nMask & CApplication::NM_POS) == CApplication::NM_POS)
	{
		CRect rc;
		CHECK_HR(spElement->getAttribute(L"left", &val));
		if (val.vt != VT_BSTR)
		{
			rc.left = 0;
		}
		else
		{
			rc.left = _ttoi(val.bstrVal);
		}
		CHECK_HR(spElement->getAttribute(L"top", &val));
		if (val.vt != VT_BSTR)
		{
			rc.top = 0;
		}
		else
		{
			rc.top = _ttoi(val.bstrVal);
		}
		CHECK_HR(spElement->getAttribute(L"right", &val));
		if (val.vt != VT_BSTR)
		{
			rc.right = rc.left + 200;
		}
		else
		{
			rc.right = _ttoi(val.bstrVal);
		}
		CHECK_HR(spElement->getAttribute(L"bottom", &val));
		if (val.vt != VT_BSTR)
		{
			rc.bottom = rc.top + 165;
		}
		else
		{
			rc.bottom = _ttoi(val.bstrVal);
		}
		note.SetPos(rc);
	}

	if ((nMask & CApplication::NM_TEXT) == CApplication::NM_TEXT)
	{
		CComBSTR bstr;
		CHECK_HR(spElement->get_text(&bstr));
		note.SetText((LPCTSTR)bstr);
	}
	if ((nMask & CApplication::NM_CREATED) == CApplication::NM_CREATED)
	{
		CHECK_HR(spElement->getAttribute(L"created", &val));
		time_t t;
		if (val.vt == VT_NULL) // attribute not found
		{
			t = 0;
		}
		else if (val.vt == VT_BSTR)
		{
			t =_ttoi(val.bstrVal);
		}
		else
		{
			ThrowError(_T("Attribute created not found"));
		}
		note.SetCreatedDate(t);
	}
	if ((nMask & CApplication::NM_MODIFIED) == CApplication::NM_MODIFIED)
	{
		CHECK_HR(spElement->getAttribute(L"modified", &val));
		time_t t;
		if (val.vt == VT_NULL) // attribute not found
		{
			t = 0;
		}
		else if (val.vt == VT_BSTR)
		{
			t =_ttoi(val.bstrVal);
		}
		else
		{
			ThrowError(_T("Attribute modified not found"));
		}
		note.SetModifiedDate(t);
	}
	if ((nMask & CApplication::NM_DELETED) == CApplication::NM_DELETED)
	{
		CHECK_HR(spElement->getAttribute(L"deleted", &val));
		time_t t;
		if (val.vt == VT_NULL) // attribute not found
		{
			t = 0;
		}
		else if (val.vt == VT_BSTR)
		{
			t =_ttoi(val.bstrVal);
		}
		else
		{
			ThrowError(_T("Attribute deleted not found"));
		}
		note.SetDeletedDate(t);
	}
	if ((nMask & CApplication::NM_LABEL) == CApplication::NM_LABEL)
	{
		CHECK_HR(spElement->getAttribute(L"label", &val));
		if (val.vt != VT_NULL && val.vt != VT_BSTR)
		{
			ThrowError(_T("Attribute label not found"));
		}
		note.SetLabel(val.bstrVal);
	}
	return note;
}

/*
@param UINT nMask see CApplication::NoteMask flags
*/
static void _GetAllNotes(CNote::List& list, UINT nMask)
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	long len = 0;
	CComPtr<IXMLDOMNodeList> spNotes;
	CHECK_HR(spDoc->getElementsByTagName(L"note", &spNotes));
	CHECK_HR(spNotes->get_length(&len));
	for (int i = 0; i < len; ++i)
	{
		CComPtr<IXMLDOMNode> spNode;
		CHECK_HR(spNotes->get_item(i, &spNode));
		CNote note = _GetNote(spNode, nMask);
		list.push_back(note);
	}
}

static int _GetNextId(CComPtr<IXMLDOMDocument>& spDoc)
{
	CNote::List notes;
	_GetAllNotes(notes, CApplication::NM_ID);
	std::vector<int> ids;
	for (int j = 0; j < notes.size(); ++j)
	{
		ids.push_back(notes[j].GetId());
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

static void _SetNoteContent(CComPtr<IXMLDOMElement>& spElement, CNote const& note, UINT nMask)
{
	if ((nMask & CApplication::NM_ID) == CApplication::NM_ID)
	{
		CHECK_HR(spElement->setAttribute(L"id", CComVariant(note.GetId())));
	}
	if ((nMask & CApplication::NM_POS) == CApplication::NM_POS)
	{
		CRect pos = note.GetPos();
		CHECK_HR(spElement->setAttribute(L"left", CComVariant(pos.left)));
		CHECK_HR(spElement->setAttribute(L"top", CComVariant(pos.top)));
		CHECK_HR(spElement->setAttribute(L"right", CComVariant(pos.right)));
		CHECK_HR(spElement->setAttribute(L"bottom", CComVariant(pos.bottom)));
	}
	if ((nMask & CApplication::NM_TEXT) == CApplication::NM_TEXT)
	{
		CHECK_HR(spElement->put_text(CComBSTR(note.GetText())));
	}
	if ((nMask & CApplication::NM_CREATED) == CApplication::NM_CREATED)
	{
		CHECK_HR(spElement->setAttribute(L"created", CComVariant(note.GetCreatedDate())));
	}
	if ((nMask & CApplication::NM_MODIFIED) == CApplication::NM_MODIFIED)
	{
		CHECK_HR(spElement->setAttribute(L"modified", CComVariant(note.GetModifiedDate())));
	}
	if ((nMask & CApplication::NM_DELETED) == CApplication::NM_DELETED)
	{
		CHECK_HR(spElement->setAttribute(L"deleted", CComVariant(note.GetDeletedDate())));
	}
	if ((nMask & CApplication::NM_LABEL) == CApplication::NM_LABEL)
	{
		CHECK_HR(spElement->setAttribute(L"label", CComVariant(note.GetLabel())));
	}
}

static void _NewNote(CNote& note)
{
	if (note.GetId() > 0)
	{
		ThrowError(_T("Can not add note with not null id"));
	}
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMElement> spElement;
	note.SetId(_GetNextId(spDoc));
	CHECK_HR(spDoc->createElement(L"note", &spElement));
	_SetNoteContent(spElement, note, CApplication::NM_ALL);
	CComPtr<IXMLDOMNode> spRoot = _GetRootNode(spDoc);
	CComPtr<IXMLDOMNode> spChild;
	CHECK_HR(spRoot->appendChild(spElement, &spChild));
	if (!spChild)
	{
		ThrowError(_T("Append note error"));
	}
	CHECK_HR(spDoc->save(CComVariant(CApplication::Get().GetDataFileName())));
}

static void _UpdateNote(CNote const& note, UINT nMask)
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMNode> spNode = _FindNote(spDoc, note.GetId());
	CComPtr<IXMLDOMElement> spElement;
	CHECK_HR(spNode.QueryInterface(&spElement));
	_SetNoteContent(spElement, note, nMask);
	CHECK_HR(spDoc->save(CComVariant(CApplication::Get().GetDataFileName())));
}

static CComPtr<IXMLDOMNode> _GetOptionsNode(CComPtr<IXMLDOMDocument>& spDoc)
{
	CComPtr<IXMLDOMNode> spOptions;
	CHECK_HR(spDoc->selectSingleNode(CComBSTR(_T("notes/options")),&spOptions));
	if (!spOptions)
	{
		ThrowError(_T("Options node not found"));
	}
	return spOptions;
}
//////////////////////////////////////////////////////////////////////////
//

CStorage::CStorage(void)
{
}

CStorage::~CStorage(void)
{
}

/**/
void CStorage::SaveNote(CNote& note, UINT nMask)
{
	if (note.GetId() == 0) // new note
	{
		_NewNote(note);
	}
	else
	{
		_UpdateNote(note, nMask);
	}
}

/*
see CApplication::NoteMask flags
*/
void CStorage::GetAllNotes(CNote::List& list, UINT nMask) const
{
	_GetAllNotes(list, nMask);
}

/**/
void CStorage::DeleteNote(int nNoteId)
{
	if (nNoteId > 0)
	{
		CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
		CComPtr<IXMLDOMNode> spRoot = _GetRootNode(spDoc);
		CComPtr<IXMLDOMNode> spNode = _FindNote(spDoc, nNoteId);
		CComPtr<IXMLDOMNode> spChild;
		CHECK_HR(spRoot->removeChild(spNode, &spChild));
		if (!spChild)
		{
			ThrowError(_T("Remove note error"));
		}
		CHECK_HR(spDoc->save(CComVariant(CApplication::Get().GetDataFileName())));
	}
}

void CStorage::ReadOptions( COptions& opt ) const
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMNode> spOptions = _GetOptionsNode(spDoc);
	CComPtr<IXMLDOMNode> spNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("always-on-top")),&spNode));
	if (spNode)
	{
		CComBSTR s;
		spNode->get_text(&s);
		opt.SetAlwaysOnTop(_ttoi(s));
	}
	CComPtr<IXMLDOMNode> spFontNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("font-size")),&spFontNode));
	if (spFontNode)
	{
		CComBSTR s;
		spFontNode->get_text(&s);
		opt.SetFontSize((COptions::FontSize)_ttoi(s));
	}
}

void CStorage::WriteOptions( COptions const& opt ) const
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMNode> spOptions = _GetOptionsNode(spDoc);
	CComPtr<IXMLDOMNode> spNode;
	CComPtr<IXMLDOMNode> spChild;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("always-on-top")), &spNode));
	if (!spNode)
	{
		CComPtr<IXMLDOMElement> spElem;
		CHECK_HR(spDoc->createElement(L"always-on-top", &spElem));
		CHECK_HR(spOptions->appendChild(spElem, &spNode));
	}
	spNode->put_text(CComBSTR(strutils::to_string(opt.GetAlwaysOnTop()).c_str()));

	CComPtr<IXMLDOMNode> spFontNode;

	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("font-size")), &spFontNode));
	if (!spFontNode)
	{
		CComPtr<IXMLDOMElement> spElem;
		CHECK_HR(spDoc->createElement(L"font-size", &spElem));
		CHECK_HR(spOptions->appendChild(spElem, &spFontNode));
	}
	spFontNode->put_text(CComBSTR(strutils::to_string(opt.GetFontSize()).c_str()));

	CHECK_HR(spDoc->save(CComVariant(CApplication::Get().GetDataFileName())));
}

CNote CStorage::GetNote(int nNoteId) const
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	return _GetNote(_FindNote(spDoc, nNoteId), CApplication::NM_ALL);
}

void CStorage::Release()
{
	s_spDoc.Release();
}