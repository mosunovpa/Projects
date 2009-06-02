#pragma once


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

const UINT WM_INITNOTE = WM_USER + 100;

