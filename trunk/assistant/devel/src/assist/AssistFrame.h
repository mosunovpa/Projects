/**
 * @file AssistFrame.h
 *
 * CAssistFrame base class.
 *
 * @author Pavel Mosunov
 * @since 2004-9-14
 */

#ifndef __ASSIST_FRAME_H__
#define __ASSIST_FRAME_H__

#include "AssistViewImpl.h"

namespace Module
{
	class IModuleInfo;
}
class CAssistFrame : public Views::CAssistViewImpl
{
public:
	CAssistFrame();
	virtual ~CAssistFrame(void); 

	// interface

	BEGIN_MSG_MAP(CAssistFrame)
		CHAIN_MSG_MAP(Views::CAssistViewImpl)
	END_MSG_MAP()

 	virtual Module::IModuleInfo* GetCurModule() = 0;
 	virtual DWORD GetCurFolder() = 0;
	virtual DWORD GetRootFolder(DWORD nModuleId) = 0;
protected:
private:
};

#endif // __ASSIST_FRAME_H__
