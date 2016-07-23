//ListItemView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef LIST_ITEM_VIEW_H
#define LIST_ITEM_VIEW_H

#include "FunctionList.h"
#include <Locker.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Rect.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Box.h>

class ListItemView: public BView
{
	public:
		ListItemView(BRect frame, const char* name, FunctionItem* func);
		virtual void MessageReceived(BMessage* message);
		virtual void AttachedToWindow(void);
		FunctionItem* function;
	protected:
		class ColorPatch* patch;
		BMenuField* menuField;
		BCheckBox* checkBox;
		BBox* box;
		class FunctionTypePopup;
};

class ListItemView::FunctionTypePopup : public BPopUpMenu
{
	public:
		FunctionTypePopup();
};

const uint32 SET_TYPE_CART='STYP';
const uint32 SET_TYPE_PARA='STPR';
const uint32 SET_TYPE_POLAR='STPO';
const uint32 ADD_ITEM = 'ADTM';

#endif