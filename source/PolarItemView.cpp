//PolarItemView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include "PolarItemView.h"
#include "constants.h"
#include "ColorPatch.h"
#include "GraphView.h"
#include <Box.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <PopUpMenu.h>
#include <MenuItem.h>


PolarItemView::PolarItemView(BRect frame, const char* name, FunctionItem* func):
			ListItemView(frame, name, func)
{
	//gFuncList had better already be locked!!
	
	BRect rect;
	
	rect = Bounds();
	
	rect.InsetBy(5,5);
	
	tc = new BTextControl(rect, "Function", "r(theta)=", func->text->String(), 
							new BMessage(FUNC_CHG));
	tc->SetDivider(44);
	box->AddChild(tc);
	
	menuField->MenuBar()->FindItem("Polar")->SetMarked(true);
	
}

void PolarItemView::AttachedToWindow(void)
{
 	float width,height;
	tc->GetPreferredSize(&width, &height);
	ResizeTo(Frame().Width(), Frame().Height()+height); 

	tc->SetTarget(this);
	ListItemView::AttachedToWindow();
}


void PolarItemView::MessageReceived(BMessage* msg)
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