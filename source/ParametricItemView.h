//ParametricItemView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef PARAMETRIC_ITEM_VIEW_H
#define PARAMETRIC_ITEM_VIEW_H

#include "FunctionList.h"
#include "ListItemView.h"
#include <Locker.h>
#include <Rect.h>
#include <TextControl.h>
#include <Box.h>

class ParametricItemView: public ListItemView
{
	public:
		ParametricItemView(BRect frame, const char* name, FunctionItem* func);
		virtual void MessageReceived(BMessage* message);
		virtual void AttachedToWindow(void);
	protected:
		BTextControl* xT;
		BTextControl* yT;
};

const uint32 XT_CHANGED='XTCG';
const uint32 YT_CHANGED='YTCG';

#endif