//GraphWindow.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include <TextControl.h>
#include <stdio.h>
#include <stdlib.h>
#include <TextControl.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuBar.h>
#include <FilePanel.h>
#include <Messenger.h>
#include <String.h>

#include "GraphApp.h"
#include "GraphWindow.h"
#include "constants.h"
#include "FunctionListWindow.h"
//#include "SettingsManager.h"

GraphWindow::GraphWindow(BRect frame):BWindow(frame, "Graph", B_TITLED_WINDOW,
								B_NOT_ZOOMABLE|B_ASYNCHRONOUS_CONTROLS|B_WILL_ACCEPT_FIRST_CLICK)
{	
	BRect the_rect;
	BMenu* tempMenu;
	
	the_rect = Bounds();
	
	menuBar = new BMenuBar(the_rect, "GraphMenuBar");
	AddChild(menuBar);
	
	tempMenu = new BMenu("File");
	tempMenu->AddItem(new BMenuItem("About", new BMessage(MENU_FILE_ABOUT) ));
	tempMenu->AddSeparatorItem();
	tempMenu->AddItem(new BMenuItem("Save", new BMessage(MENU_FILE_SAVE), 'S'));
	tempMenu->AddSeparatorItem();
	tempMenu->AddItem(new BMenuItem("Quit", new BMessage(MENU_FILE_QUIT) , 'Q') );
	
	
	menuBar->AddItem(tempMenu);
	
	tempMenu = new BMenu("Edit");
	tempMenu->AddItem(new BMenuItem("Copy", new BMessage(MENU_EDIT_COPY), 'C' ) );
	
	menuBar->AddItem(tempMenu);
	
	tempMenu = new BMenu("Window");
	settingsWindowItem = new BMenuItem("Window Settings", new BMessage(MENU_WINDOW_SETTINGS), '1');
	settingsWindowItem->SetMarked(true);
	tempMenu->AddItem(settingsWindowItem);
	
	functionWindowItem = new BMenuItem("Function List", new BMessage(MENU_WINDOW_FUNCTION), '2');
	functionWindowItem->SetMarked(true);
	tempMenu->AddItem(functionWindowItem);
	
	menuBar->AddItem(tempMenu);
	
	menuBar->ResizeToPreferred();
	
	the_rect.top = menuBar->Frame().bottom +1;
	AddChild(the_graph = new GraphView( the_rect ));
	savePanel = new BFilePanel(B_SAVE_PANEL, new BMessenger(this) ,NULL,B_FILE_NODE,false);

	BRect windowRect;
	windowRect.Set(frame.right,frame.top, frame.right+250, frame.top+110);
	windowRect.OffsetBy(10,0);
	settingsWindow = new SettingsWindow(windowRect);
	windowRect.OffsetBy(0,settingsWindow->Frame().Height()+20);
	windowRect.right=windowRect.left+250;
	windowRect.bottom=windowRect.top+300; 
	functionWindow = new FunctionListWindow(windowRect);
	settingsWindow->AddToSubset(this);
	functionWindow->AddToSubset(this);


	
	
}
GraphWindow::~GraphWindow(void)
{	
//	class SettingsManager settings;
	
//	settings.SaveSettings();
	
	if (settingsWindow->Lock())
		settingsWindow->Quit();
		
	if (functionWindow->Lock())
		functionWindow->Quit();
}

bool GraphWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void GraphWindow::MessageReceived(BMessage *message)
{
			
	switch (message->what)
	{
		case MENU_FILE_ABOUT:
		{
			//	DA@Y2K:
			//	This is a subtle memory leak.
			//	PostMessage will copy your message, and your pointer will not be deleted.
			//	Use a stack allocated object instead.
			
			BMessage msg(B_ABOUT_REQUESTED);
			be_app->PostMessage(&msg);
		}
		break;
		case MENU_FILE_QUIT:
			QuitRequested();
			break;
		case MENU_EDIT_COPY:
			the_graph->CopyGraph();
			break;
		case MENU_FILE_SAVE:
		{
			savePanel->Show();
			break;
		}
		case B_SAVE_REQUESTED:
		{
			entry_ref saveDir;
			BString fileName;
			message->FindRef("directory",&saveDir);
			message->FindString("name", &fileName);
			
			the_graph->SaveGraph(saveDir,fileName);
			break;
		}
		case MENU_WINDOW_SETTINGS:
		{
			settingsWindowItem->SetMarked(!settingsWindowItem->IsMarked());
			if (settingsWindowItem->IsMarked() && settingsWindow->IsHidden())
				settingsWindow->Show();
			else if ((!settingsWindowItem->IsMarked()) && !(settingsWindow->IsHidden())) 
				settingsWindow->Hide();
			break;
		}
		case MENU_WINDOW_FUNCTION:
		{
			functionWindowItem->SetMarked(!functionWindowItem->IsMarked());
			if (functionWindowItem->IsMarked() && functionWindow->IsHidden())
				functionWindow->Show();
			else if ((!functionWindowItem->IsMarked()) && !(functionWindow->IsHidden())) 
				functionWindow->Hide();
			break;
		}
		case INVAL_GRAPH:
		{
			the_graph->UpdateGraph();
		}
		break;
		default:
			BWindow::MessageReceived(message);
			break;
	}
	return;
}
