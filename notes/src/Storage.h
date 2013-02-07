#pragma once

#include "note.h"
#include "defines.h"

class COptions;

class CStorage
{
public:
	CStorage();
	~CStorage(void);
	void SetDataFile(LPCTSTR fileName);
	_tstring GetFileName() { return m_fileName; }
	void Release();
	int SaveNote(CNote const& note, UINT nMask);
	void DeleteNote(int nNoteId);
	void GetAllNotes(CNote::List& list, UINT nMask);
	void GetLabels(std::list<_tstring>& list);
	CNote GetNote(int nNoteId);
	void ReadOptions(COptions& opt);
	void WriteOptions(COptions const& opt);

private:
	bool ArchiveFile();

	CComPtr<IXMLDOMDocument> s_spDoc;

	CComPtr<IXMLDOMDocument> _GetDocument();
	CComPtr<IXMLDOMNode> _GetRootNode(CComPtr<IXMLDOMDocument>& spDoc);
	CComPtr<IXMLDOMNode> _FindNote(CComPtr<IXMLDOMDocument>& spDoc, int nNoteId);
	CNote _GetNote(CComPtr<IXMLDOMNode> spNode, UINT nMask);
	void _GetAllNotes(CNote::List& list, UINT nMask);
	int _GetNextId(CComPtr<IXMLDOMDocument>& spDoc);
	void _SetNoteContent(CComPtr<IXMLDOMElement>& spElement, CNote const& note, UINT nMask);
	int _NewNote(CNote const& nt);
	void _UpdateNote(CNote const& note, UINT nMask);
	CComPtr<IXMLDOMNode> _GetOptionsNode(CComPtr<IXMLDOMDocument>& spDoc);
	_tstring m_fileName;
	time_t m_archtime;
};
