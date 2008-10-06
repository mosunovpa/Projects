/**
 * @file AssistView.h
 *
 * CAssistViewImpl base class.
 *
 * @author Pavel Mosunov
 * @since 2004-9-10
 */

#ifndef __ASSISTVIEW_H__
#define __ASSISTVIEW_H__

namespace Views
{

/**
 * 
 */
class IAssistView : public CMessageMap
{
public:
	virtual BOOL ActivateChildView(IAssistView* pView) = 0;
	virtual void AddChildView(IAssistView* pView) = 0;
	virtual IAssistView* GetCurrentChildView() = 0;
	virtual HWND SetFocus() = 0;
	virtual void SetParentView(IAssistView* pParentView) = 0;
	virtual IAssistView* GetParentView() = 0;
	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0) = 0;

	virtual HWND GetHwnd() const = 0;
	virtual HWND CreateView(HWND parent) = 0;
	virtual void Delete() = 0;

	// events
	virtual void OnChildViewActivated(BOOL bActive, IAssistView* pView) = 0;
	virtual BOOL OnActivate(BOOL bActive) = 0;
	virtual void OnRefreshed(IAssistView* pView, int code, WPARAM wParam = 0, LPARAM lParam = 0) = 0;

protected:
	virtual ~IAssistView() {}

};

} //namespace Views

#endif //__ASSISTVIEW_H__
