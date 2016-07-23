//CartesianItemView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include "CartesianItemView.h"
#include "constants.h"
#include "ColorPatch.h"
#include "GraphView.h"
#include <Box.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <PopUpMenu.h>
#include <MenuItem.h>


CartesianItemView::CartesianItemView(BRect frame, const char* name, FunctionItem* func):
			ListItemView(frame, name, func)
{
	//gFuncList had better already be locked!!
	
	BRect rect;
	
	rect = Bounds();
	
	rect.InsetBy(5,5);
	
	tc = new BTextControl(rect, "Function", "y=", func->text->String(), 
							new BMessage(FUNC_CHG));
	tc->SetDivider(17);
	box->AddChild(tc);
	
	menuField->MenuBar()->FindItem("Cartesian")->SetMarked(true);
	
}

void CartesianItemView::AttachedToWindow(void)
{
 	float width,height;
	tc->GetPreferredSize(&width, &height);
	ResizeTo(Frame().Width(), Frame().Height()+height); 

	tc->SetTarget(this);
	ListItemView::AttachedToWindow();
}


void CartesianItemView::MessageReceived(BMessage* msg)
{

	switch (msg->what)
	{
		case FUNC_CHG:
		{
			if (gFuncList->LockAsWriter())
			{
				*(function->text) = tc->Text();
				gFuncList->UnlockAsWriter();
				
			}
		}
		break;
		default:
			ListItemView::MessageReceived(msg);
			break;
	}
}