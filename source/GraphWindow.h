//GraphWindow.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <MenuBar.h>
#include <Window.h>
#include <Rect.h>
#include <FilePanel.h>

#include "GraphView.h"
#include "SettingsWindow.h"

class GraphWindow:public BWindow
{
	public:
		GraphWindow(BRect frame);
		~GraphWindow();
		virtual bool QuitRequested(void);
		virtual void MessageReceived(BMessage * message);
	private:
		BMenuItem* functionWindowItem;
		BMenuItem* settingsWindowItem;
		BFilePanel* savePanel;
		BMenuBar *menuBar;
		GraphView *the_graph;
		friend class GraphView;
		class FunctionListWindow* functionWindow;
		class SettingsWindow* settingsWindow;
		int saveFunc;
};

#endif