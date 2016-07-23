/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include <Alert.h>
#include "GraphApp.h"

GraphApp::GraphApp(void) : BApplication("application/x-vnd.DSR-Graph")
{
	BRect windowRect;
	
	windowRect.Set(50,50, 349, 349);
	the_window = new GraphWindow(windowRect);
	the_window->Show();

	
}
void GraphApp::AboutRequested(void)
{
	(new BAlert("About Graph", "Graph\n\nWritten By Daniel S. Rogers copyright 2000.\n"
							"This program is GPL'd software, enjoy", "Yay GPL!"))->Go();
	return;
}