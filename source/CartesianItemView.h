//CartesianItemView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef CARTESIAN_ITEM_VIEW_H
#define CARTESIAN_ITEM_VIEW_H

#include "FunctionList.h"
#include "ListItemView.h"
#include <Locker.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Rect.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Box.h>

class CartesianItemView: public ListItemView
{
	public:
		CartesianItemView(BRect frame, const char* name, FunctionItem* func);
		virtual void MessageReceived(BMessage* message);
		virtual void AttachedToWindow(void);
	protected:
		BTextControl* tc;
};

#endif