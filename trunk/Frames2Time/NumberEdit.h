#pragma once


// CNumberEdit

class CNumberEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumberEdit)

public:
	CNumberEdit();
	virtual ~CNumberEdit();

	void SetRange(int nMin, int nMax);

protected:
	BOOL IsInRange(int nValue);
	void ProcessTextChangingMessage();

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPaste(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	int m_nMin;
	int m_nMax;
};


