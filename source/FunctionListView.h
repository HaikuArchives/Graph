//FunctionListView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef FUNCTION_LIST_VIEW_H
#define FUNCITON_LIST_VIEW_H

#include <View.h>
#include <Message.h>
#include <TextControl.h>
#include <CheckBox.h>


#include "FunctionList.h"
#include "ListItemView.h"


class FunctionListView : public BView
{
	public:
		FunctionListView(BRect frame,const char* name);
		void MessageReceived(BMessage* message);
		float dataHeight;
		void AttachedToWindow(void);
		void AllAttached(void);
	private:
		vector<class ListItemView*> listItems;
	
};

#endif //FUNCITON_LIST_VIEW_H