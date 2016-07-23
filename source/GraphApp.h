//GraphApp.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef GRAPHAPP_H
#define GRAPHAPP_H

#include <Application.h>
#include "GraphWindow.h"


class GraphApp:public BApplication
{
	public:
		GraphApp(void);
		GraphWindow *the_window;
		virtual void AboutRequested();
	private:

};

#endif