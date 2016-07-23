//PolarItemView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef POLAR_ITEM_VIEW_H
#define POLAR_ITEM_VIEW_H

#include "FunctionList.h"
#include "ListItemView.h"
#include <Locker.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Rect.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Box.h>

class PolarItemView: public ListItemView
{
	public:
		PolarItemView(BRect frame, const char* name, FunctionItem* func);
		virtual void MessageReceived(BMessage* message);
		virtual void AttachedToWindow(void);
	protected:
		BTextControl* tc;
};

#endif