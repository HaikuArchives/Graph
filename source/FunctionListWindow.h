//FunctionListWindow.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef FUNCTION_LIST_WINDOW_H
#define FUNCTION_LIST_WINDOW_H

#include <Window.h>
#include <Message.h>
#include <Button.h>

#include "FunctionList.h"
#include "GraphView.h"

class FunctionListWindow : public BWindow
{
	public:
		FunctionListWindow(BRect frame);
		bool QuitRequested(void);
		virtual void FrameResized(float width, float height);
		//	void MessageReceived(BMessage*);
	private:
		BScrollBar* scrollBar; //a pointer to the scrollbar, for easy fixin'
		class FunctionListView* functionListView;
		BButton* lessButton;
		void FixScrollBar(void);
		void FixZoomSize(void);
	friend class FunctionListView;

};

#endif