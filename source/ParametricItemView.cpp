//ParametricItemView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include "ParametricItemView.h"
#include "constants.h"
#include "ColorPatch.h"
#include "GraphView.h"
#include <Box.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <PopUpMenu.h>
#include <MenuItem.h>


ParametricItemView::ParametricItemView(BRect frame, const char* name, FunctionItem* func):
			ListItemView(frame, name, func)
{
	//gFuncList had better already be locked!!
	
	BRect rect;
	float width,height;
	
	rect = Bounds();
	
	rect.InsetBy(5,5);
	
	xT = new BTextControl(rect, "x(t)", "x(t)=", func->text[0].String(), 
							new BMessage(XT_CHANGED));
	xT->GetPreferredSize(&width,&height);
	rect.OffsetBy(0.0f,height+4.0f);
	yT = new BTextControl(rect, "y(t)", "y(t)=", func->text[1].String(), 
							new BMessage(YT_CHANGED));

	xT->SetDivider(25);
	yT->SetDivider(25);
	
	box->AddChild(xT);
	box->AddChild(yT);
	
	menuField->MenuBar()->FindItem("Parametric")->SetMarked(true);
	
}

void ParametricItemView::AttachedToWindow(void)
{
 	float width,height;
	xT->GetPreferredSize(&width, &height);
	ResizeTo(Frame().Width(), Frame().Height()+height*2+4.0f); 

	xT->SetTarget(this);
	yT->SetTarget(this);
	ListItemView::AttachedToWindow();
}


void ParametricItemView::MessageReceived(BMessage* msg)
{

	switch (msg->what)
	{
		case XT_CHANGED:
		{
			if (gFuncList->LockAsWriter())
			{
				function->text[0] = xT->Text();
				gFuncList->UnlockAsWriter();
				
			}
		}
		break;
		case YT_CHANGED:
		{
			if (gFuncList->LockAsWriter())
			{
				function->text[1] = yT->Text();
				gFuncList->UnlockAsWriter();
				
			}
		}
		break;
		default:
			ListItemView::MessageReceived(msg);
			break;
	}
}