/*!
 * \file ViewFactory.h
 * 
 * \author Pavel Mosunov
 * \since 2005-5-10
 */

#pragma once

#include "Singleton.h"
#include "Factory.h"
#include "AssistView.h"

namespace Views
{
typedef Loki::SingletonHolder<Loki::Factory<Views::IAssistView, DWORD> > CViewFactory;
}