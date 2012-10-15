#include "StdAfx.h"
#include "Storage.h"
#include "resource.h"
#include "fileutils.h"
#include "Application.h"
#include "Options.h"
#include "dateutils.h"

using namespace dateutils;

//////////////////////////////////////////////////////////////////////////
//

#define CHECK_HR_MSG(x, s) { HRESULT hr = x; if (FAILED(hr)) ThrowError(s); }
#define CHECK_HR(x) CHECK_HR_MSG(x, _T("Xml operation error"))

//////////////////////////////////////////////////////////////////////////
//

CStorage::CStorage(void) : m_archtime(0)
{
}

CStorage::~CStorage(void)
{
}

/**/
void CStorage::SetDataFile(LPCTSTR fileName)
{
	m_fileName = fileName;
	if (s_spDoc == NULL)
	{
		CHECK_HR_MSG(s_spDoc.CoCreateInstance(__uuidof(DOMDocument)), _T("Create document error"));
	}
	LPCTSTR sFileName = m_fileName.c_str();
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

/**/
CComPtr<IXMLDOMDocument> CStorage::_GetDocument()
{
	return s_spDoc;
}

/**/
CComPtr<IXMLDOMNode> CStorage::_GetRootNode(CComPtr<IXMLDOMDocument>& spDoc)
{
	CComPtr<IXMLDOMNode> spRoot;
	CHECK_HR(spDoc->selectSingleNode(CComBSTR(_T("notes")),&spRoot));
	if (!spRoot)
	{
		ThrowError(_T("Root node not found"));
	}
	return spRoot;
}

/**/
CComPtr<IXMLDOMNode> CStorage::_FindNote(CComPtr<IXMLDOMDocument>& spDoc, int nNoteId)
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

/**/
CNote CStorage::_GetNote(CComPtr<IXMLDOMNode> spNode, UINT nMask)
{
	CComPtr<IXMLDOMElement> spElement;
	CHECK_HR(spNode.QueryInterface(&spElement));
	CNote note;
	CComVariant val;

	if ((nMask & NM_ID) == NM_ID)
	{
		CHECK_HR(spElement->getAttribute(L"id", &val));
		if (val.vt != VT_BSTR)
		{
			ThrowError(_T("Attribute id not found"));
		}
		note.SetId(_ttoi(val.bstrVal));
	}
	if ((nMask & NM_POS) == NM_POS)
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

	if ((nMask & NM_TEXT) == NM_TEXT)
	{
		CComBSTR bstr;
		CHECK_HR(spElement->get_text(&bstr));
		_tstring s((LPCTSTR)bstr);
		int len = s.size();
		if (len >= 2 && s[0] == _T('"') && s[len - 1] == _T('"'))
		{
			s.erase(len - 1);
			s.erase(0, 1);
		}
		note.SetText(s);
	}
	if ((nMask & NM_CREATED) == NM_CREATED)
	{
		CHECK_HR(spElement->getAttribute(L"created", &val));
		_timeb t = timebn::getempty();
		if (val.vt == VT_BSTR)
		{
			t.time =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL)
		{
			ThrowError(_T("Attribute created not found"));
		}
		CHECK_HR(spElement->getAttribute(L"created_ms", &val));
		if (val.vt == VT_BSTR)
		{
			t.millitm =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL) 
		{
			ThrowError(_T("Attribute created_ms not found"));
		}
		note.SetCreatedDate(t);
	}
	if ((nMask & NM_MODIFIED) == NM_MODIFIED)
	{
		CHECK_HR(spElement->getAttribute(L"modified", &val));
		_timeb t = timebn::getempty();
		if (val.vt == VT_BSTR)
		{
			t.time =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL)
		{
			ThrowError(_T("Attribute modified not found"));
		}
		CHECK_HR(spElement->getAttribute(L"modified_ms", &val));
		if (val.vt == VT_BSTR)
		{
			t.millitm =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL)
		{
			ThrowError(_T("Attribute modified_ms not found"));
		}
		note.SetModifiedDate(t);
	}
	if ((nMask & NM_DELETED) == NM_DELETED)
	{
		CHECK_HR(spElement->getAttribute(L"deleted", &val));
		_timeb t = timebn::getempty();
		if (val.vt == VT_BSTR)
		{
			t.time =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL)
		{
			ThrowError(_T("Attribute deleted not found"));
		}
		CHECK_HR(spElement->getAttribute(L"deleted_ms", &val));
		if (val.vt == VT_BSTR)
		{
			t.millitm =_ttoi(val.bstrVal);
		}
		else if (val.vt != VT_NULL)
		{
			ThrowError(_T("Attribute deleted_ms not found"));
		}
		note.SetDeletedDate(t);
	}
	if ((nMask & NM_LABEL) == NM_LABEL)
	{
		CHECK_HR(spElement->getAttribute(L"label", &val));
		if (val.vt != VT_NULL && val.vt != VT_BSTR)
		{
			ThrowError(_T("Attribute label not found"));
		}
		note.SetLabel(val.bstrVal == 0 ? _T("") : val.bstrVal);
	}
	return note;
}

/*
@param UINT nMask see CApplication::NoteMask flags
*/
void CStorage::_GetAllNotes(CNote::List& list, UINT nMask)
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

int CStorage::_GetNextId(CComPtr<IXMLDOMDocument>& spDoc)
{
	CNote::List notes;
	_GetAllNotes(notes, NM_ID);
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

void CStorage::_SetNoteContent(CComPtr<IXMLDOMElement>& spElement, CNote const& note, UINT nMask)
{
	if ((nMask & NM_ID) == NM_ID)
	{
		CHECK_HR(spElement->setAttribute(L"id", CComVariant(note.GetId())));
	}
	if ((nMask & NM_POS) == NM_POS)
	{
		CRect pos = note.GetPos();
		CHECK_HR(spElement->setAttribute(L"left", CComVariant(pos.left)));
		CHECK_HR(spElement->setAttribute(L"top", CComVariant(pos.top)));
		CHECK_HR(spElement->setAttribute(L"right", CComVariant(pos.right)));
		CHECK_HR(spElement->setAttribute(L"bottom", CComVariant(pos.bottom)));
	}
	if ((nMask & NM_TEXT) == NM_TEXT)
	{
		_tstring s(note.GetText());
		s.insert(0, 1, _T('"'));
		s.append(1, _T('"'));
		CHECK_HR(spElement->put_text(CComBSTR(s.c_str())));
	}
	if ((nMask & NM_CREATED) == NM_CREATED)
	{
		_timeb tb = note.GetCreatedDate();
		CHECK_HR(spElement->setAttribute(L"created", CComVariant(tb.time)));
		CHECK_HR(spElement->setAttribute(L"created_ms", CComVariant(tb.millitm)));
	}
	if ((nMask & NM_MODIFIED) == NM_MODIFIED)
	{
		_timeb tb = note.GetModifiedDate();
		CHECK_HR(spElement->setAttribute(L"modified", CComVariant(tb.time)));
		CHECK_HR(spElement->setAttribute(L"modified_ms", CComVariant(tb.millitm)));
	}
	if ((nMask & NM_DELETED) == NM_DELETED)
	{
		_timeb tb = note.GetDeletedDate();
		CHECK_HR(spElement->setAttribute(L"deleted", CComVariant(tb.time)));
		CHECK_HR(spElement->setAttribute(L"deleted_ms", CComVariant(tb.millitm)));
	}
	if ((nMask & NM_LABEL) == NM_LABEL)
	{
		CHECK_HR(spElement->setAttribute(L"label", CComVariant(note.GetLabel().c_str())));
	}
}

int CStorage::_NewNote(CNote const& nt)
{
	CNote note = nt;
	if (note.GetId() > 0)
	{
		ThrowError(_T("Can not add note with not null id"));
	}
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMElement> spElement;
	int id = _GetNextId(spDoc);
	note.SetId(id);
	CHECK_HR(spDoc->createElement(L"note", &spElement));
	_SetNoteContent(spElement, note, NM_ALL);
	CComPtr<IXMLDOMNode> spRoot = _GetRootNode(spDoc);
	CComPtr<IXMLDOMNode> spChild;
	CHECK_HR(spRoot->appendChild(spElement, &spChild));
	if (!spChild)
	{
		ThrowError(_T("Append note error"));
	}
	CHECK_HR(spDoc->save(CComVariant(m_fileName.c_str())));
	return id;
}

void CStorage::_UpdateNote(CNote const& note, UINT nMask)
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMNode> spNode = _FindNote(spDoc, note.GetId());
	CComPtr<IXMLDOMElement> spElement;
	CHECK_HR(spNode.QueryInterface(&spElement));
	_SetNoteContent(spElement, note, nMask);
	CHECK_HR(spDoc->save(CComVariant(m_fileName.c_str())));
}

CComPtr<IXMLDOMNode> CStorage::_GetOptionsNode(CComPtr<IXMLDOMDocument>& spDoc)
{
	CComPtr<IXMLDOMNode> spOptions;
	CHECK_HR(spDoc->selectSingleNode(CComBSTR(_T("notes/options")),&spOptions));
	if (!spOptions)
	{
		ThrowError(_T("Options node not found"));
	}
	return spOptions;
}

/**/
int CStorage::SaveNote(CNote const& note, UINT nMask)
{
	int id = 0;
	if (note.GetId() == 0) // new note
	{
		id = _NewNote(note);
	}
	else
	{
		_UpdateNote(note, nMask);
		id = note.GetId();
	}
	if ((nMask & NM_MODIFIED) == NM_MODIFIED)
	{
		time_t t = GetDate(GetCurrentDateTime().time);
		if (t != m_archtime)
		{
			if (ArchiveFile())
			{
				m_archtime = t;
			}
		}
	}
	return id;
}

/*
see CApplication::NoteMask flags
*/
void CStorage::GetAllNotes(CNote::List& list, UINT nMask) 
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
		CHECK_HR(spDoc->save(CComVariant(m_fileName.c_str())));
	}
}

void CStorage::ReadOptions( COptions& opt ) 
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

	// font-size
	CComPtr<IXMLDOMNode> spFontNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("font-size")),&spFontNode));
	if (spFontNode)
	{
		CComBSTR s;
		spFontNode->get_text(&s);
		opt.SetFontSize((COptions::FontSize)_ttoi(s));
	}

	// new-note
	CComPtr<IXMLDOMNode> spNewNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("new-note")), &spNewNode));
	if (spNewNode)
	{
		CComPtr<IXMLDOMElement> spElement;
		CHECK_HR(spNewNode.QueryInterface(&spElement));
		
		CComVariant val;
		SIZE sz;
		CHECK_HR(spElement->getAttribute(L"width", &val));
		if (val.vt != VT_BSTR)
		{
			sz.cx = 200;
		}
		else
		{
			sz.cx = _ttoi(val.bstrVal);
		}
		CHECK_HR(spElement->getAttribute(L"height", &val));
		if (val.vt != VT_BSTR)
		{
			sz.cy = 160;
		}
		else
		{
			sz.cy = _ttoi(val.bstrVal);
		}
		opt.SetNewNoteSize(sz);
	}
}

void CStorage::WriteOptions( COptions const& opt ) 
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	CComPtr<IXMLDOMNode> spOptions = _GetOptionsNode(spDoc);
	CComPtr<IXMLDOMNode> spNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("always-on-top")), &spNode));
	if (!spNode)
	{
		CComPtr<IXMLDOMElement> spElem;
		CHECK_HR(spDoc->createElement(L"always-on-top", &spElem));
		CHECK_HR(spOptions->appendChild(spElem, &spNode));
	}
	spNode->put_text(CComBSTR(strutils::to_string(opt.GetAlwaysOnTop()).c_str()));

	// font-size
	CComPtr<IXMLDOMNode> spFontNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("font-size")), &spFontNode));
	if (!spFontNode)
	{
		CComPtr<IXMLDOMElement> spElem;
		CHECK_HR(spDoc->createElement(L"font-size", &spElem));
		CHECK_HR(spOptions->appendChild(spElem, &spFontNode));
	}
	spFontNode->put_text(CComBSTR(strutils::to_string(opt.GetFontSize()).c_str()));

	// new-note
	CComPtr<IXMLDOMNode> spNewNode;
	CHECK_HR(spOptions->selectSingleNode(CComBSTR(_T("new-note")), &spNewNode));
	if (!spNewNode)
	{
		CComPtr<IXMLDOMElement> spElem;
		CHECK_HR(spDoc->createElement(L"new-note", &spElem));
		CHECK_HR(spOptions->appendChild(spElem, &spNewNode));
	}
	CComPtr<IXMLDOMElement> spElement;
	CHECK_HR(spNewNode.QueryInterface(&spElement));
	CHECK_HR(spElement->setAttribute(L"width", CComVariant(strutils::to_string(opt.GetNewNoteSize().cx).c_str())));
	CHECK_HR(spElement->setAttribute(L"height", CComVariant(strutils::to_string(opt.GetNewNoteSize().cy).c_str())));

	CHECK_HR(spDoc->save(CComVariant(m_fileName.c_str())));
}

CNote CStorage::GetNote(int nNoteId) 
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	return _GetNote(_FindNote(spDoc, nNoteId), NM_ALL);
}

void CStorage::Release()
{
	s_spDoc.Release();
}

void CStorage::GetLabels(std::list<_tstring>& list) 
{
	CComPtr<IXMLDOMDocument> spDoc = _GetDocument();
	long len = 0;
	CComPtr<IXMLDOMNodeList> spNotes;

	CHECK_HR(spDoc->selectNodes(L"notes/note[@label!=\"\"]", &spNotes));
	CHECK_HR(spNotes->get_length(&len));
	for (int i = 0; i < len; ++i)
	{
		CComPtr<IXMLDOMNode> spNode;
		CHECK_HR(spNotes->get_item(i, &spNode));
		CNote note = _GetNote(spNode, NM_DELETED | NM_LABEL);
		if (!note.GetLabel().empty() && timebn::isempty(note.GetDeletedDate()))
		{
			list.push_back(note.GetLabel());
		}
	}
	list.sort();
	list.unique();
}

bool CStorage::ArchiveFile()
{
	// формируем имя архива

	_tstring arch_path(m_fileName.c_str(), ::PathFindFileName(m_fileName.c_str()) - m_fileName.c_str());
	const _tstring extension = _T(".dar");
	const int arch_files_count = 7;
	
	LPTSTR filename_ptr = ::PathFindFileName(m_fileName.c_str());
	_tstring filename(filename_ptr, ::PathFindExtension(filename_ptr) - filename_ptr);

	time_t t; time(&t);
	_tstring curdate_s = ToString(t, _T("%Y%m%d"));

	_tstring dest_filename(filename + curdate_s + extension);

	if (!::PathFileExists((arch_path + dest_filename).c_str()))
	{
		// берем существующие файлы архивов

		std::list<_tstring> arch_files;
		fileutils::GetFileList(arch_path, filename + _T("*") + extension, arch_files);

		// проверяем изменен ли файл после последнего архивирования

		bool file_changed = true;
		if (!arch_files.empty())
		{
			ULONGLONG fs1 = fileutils::GetFileSize(arch_files.rbegin()->c_str());
			ULONGLONG fs2 = fileutils::GetFileSize(m_fileName.c_str());
			file_changed = (fs1 != fs2);
		}

		// копируем

		if (file_changed && ::CopyFile(m_fileName.c_str(), (arch_path + dest_filename).c_str(), FALSE))
		{
			// удаляем старые архивы
			
			int recent_files_cnt = 0;
			for (std::list<_tstring>::reverse_iterator it = arch_files.rbegin();
				it != arch_files.rend(); ++it)
			{
				++recent_files_cnt;
				if (recent_files_cnt >= arch_files_count)
				{
					::DeleteFile(it->c_str());
				}
			}

			return true;
		}
	}
	return false;
}