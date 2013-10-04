#pragma once

#define COOL_CONTEXT_MENU
#define SCINTILLA 

#define NOTE_WND_CLASS_NAME _T("MyNoteWnd")

#define NOTE_CMD_FIRST 50000
#define NOTE_CMD_RANGE 9999
#define NOTE_CMD_LAST (NOTE_CMD_FIRST + NOTE_CMD_RANGE)
#define CREATE_NOTE_CMD(id) ((id) + NOTE_CMD_FIRST)
#define GET_NOTE_ID_FROM_CMD(cmd) ((cmd) - NOTE_CMD_FIRST)
#define IS_NOTE_CMD(cmd) (cmd > NOTE_CMD_FIRST && cmd <= NOTE_CMD_LAST)

#define LABEL_CMD_FIRST 60000
#define LABEL_CMD_RANGE 999
#define LABEL_CMD_LAST (LABEL_CMD_FIRST + LABEL_CMD_RANGE)
#define CREATE_LABEL_CMD(id) ((id) + LABEL_CMD_FIRST)
#define GET_LABEL_ID_FROM_CMD(cmd) ((cmd) - LABEL_CMD_FIRST)
#define IS_LABEL_CMD(cmd) (cmd > LABEL_CMD_FIRST && cmd <= LABEL_CMD_LAST)


// GetAllNotes mask
enum NoteMask
{
	NM_NONE = 0,
	NM_ID = 1,
	NM_TEXT = 2,
	NM_POS = 4,
	NM_CREATED = 8,
	NM_MODIFIED = 16,
	NM_DELETED = 32,
	NM_LABEL = 64,
	NM_ALL = NM_ID 
	| NM_TEXT 
	| NM_POS 
	| NM_CREATED 
	| NM_MODIFIED 
	| NM_DELETED 
	| NM_LABEL
};

enum NoteFlags
{
	NF_NONE = 0,
	NF_ROLLUP = 1,
	NF_NOACTIVATE = 2
};

#define CHECK_HR_MSG(x, s) { HRESULT hr = x; if (FAILED(hr)) ThrowError(s); }
#define CHECK_HR(x) CHECK_HR_MSG(x, _T("Xml operation error"))
