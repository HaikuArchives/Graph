//ListItemView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include "ListItemView.h"
#include "constants.h"
#include "ColorPatch.h"
#include "GraphView.h"
#include <Box.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <PopUpMenu.h>
#include <MenuItem.h>


ListItemView::ListItemView(BRect frame, const char* name, FunctionItem* func):
			BView(frame, name, B_FOLLOW_LEFT|B_FOLLOW_TOP,0)
{
	//gFuncList had better already be locked!!
	rgb_color background;
	background.red = 216;
	background.green = 216;
	background.blue = 216;
	background.alpha = 255;
	SetViewColor(background);

	function=func;
	
	BRect rect;

	
	rect = Bounds();
	
	box = new BBox(rect, "box", B_FOLLOW_ALL_SIDES,
				   B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,
				   B_PLAIN_BORDER);
	AddChild(box);
	
	rect.InsetBy(5,5);
	
	checkBox = new BCheckBox(rect, "Draw", "Draw", new BMessage(SET_DRAW),
														B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	checkBox->ResizeToPreferred();
	checkBox->SetValue(func->ShouldDraw());
	box->AddChild(checkBox);
	
	rect.left = checkBox->Frame().right+4;
	rect.top = checkBox->Frame().top;
	rect.bottom = checkBox->Frame().bottom;
	rect.right = checkBox->Frame().Height() + rect.left;
	
	rect.InsetBy(4,4);
	patch = new class ColorPatch(rect,"patch",func->Color(),B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	box->AddChild(patch);
	
	rect.left = patch->Frame().right+4;
	rect.top = checkBox->Frame().top;
	rect.bottom = rect.top+1;
	rect.right = rect.left+1;
	
	menuField=	new BMenuField(rect, "Type Menu","Type:",new FunctionTypePopup,B_FOLLOW_LEFT|
																B_FOLLOW_BOTTOM);
	box->AddChild(menuField);
	
	float width;
	float cbheight;
	float mfheight;
	
	checkBox->GetPreferredSize(&width, &cbheight);
	menuField->MenuBar()->GetPreferredSize(&width,&mfheight);
	//This is a correction for the extra pixals that the menuField draws
	mfheight+=4;
	
	ResizeTo(Frame().Width(), max(mfheight,cbheight)+5+5); //calculates the height, including a 5 pixel border on the top and bottom
	box->ResizeTo(Frame().Width(), Frame().Height());
	
}

void ListItemView::AttachedToWindow(void)
{
	checkBox->SetTarget(this);
	menuField->Menu()->SetTargetForItems(this);
	
	BView::AttachedToWindow();

}

void ListItemView::MessageReceived(BMessage* msg)
{

	switch (msg->what)
	{
		case SET_DRAW:
		{
			if (gFuncList->LockAsWriter())
			{
				function->SetDraw(checkBox->Value());
				gFuncList->UnlockAsWriter();
			}
		}
		break;
		case COLOR_CHG:
		{	
			const void* data;
			ssize_t size = sizeof(rgb_color);
			if (msg->FindData("color",B_RGB_COLOR_TYPE,&data,&size)== B_OK)
			{
				if (gFuncList->LockAsWriter())
				{
					function->SetColor(*(const rgb_color*)(data));
					gFuncList->UnlockAsWriter();
				}
			}
		}
		break;
		case SET_TYPE_CART:
		{
			if (gFuncList->LockAsWriter())
			{
				if (function->Type()==cart)
				{
					gFuncList->UnlockAsWriter(false);
					break;
				}
				function->SetType(cart);
				ListItemView* newItem = function->CreateListItemView(Frame(),"List Item");
				BMessenger messanger(Parent());
				BMessage viewMessage(ADD_ITEM);
				viewMessage.AddPointer("ListItemPtr",newItem);
				viewMessage.AddPointer("OldItemPtr",this);
				messanger.SendMessage(&viewMessage);
				gFuncList->UnlockAsWriter();
			}
		}
		break;		
		case SET_TYPE_PARA:
		{
			if (gFuncList->LockAsWriter())
			{
				if (function->Type()==para)
				{
					gFuncList->UnlockAsWriter(false);
					break;
				}
				function->SetType(para);
				ListItemView* newItem = function->CreateListItemView(Frame(),"List Item");
				BMessenger messanger(Parent());
				BMessage* viewMessage= new BMessage(ADD_ITEM);
				viewMessage->AddPointer("ListItemPtr",newItem);
				viewMessage->AddPointer("OldItemPtr",this);
				messanger.SendMessage(viewMessage);
				gFuncList->UnlockAsWriter();
			}
		}
		break;
		case SET_TYPE_POLAR:
		{
			if (gFuncList->LockAsWriter())
			{
				if (function->Type()==polar)
				{
					gFuncList->UnlockAsWriter(false);
					break;
				}
				function->SetType(polar);
				ListItemView* newItem = function->CreateListItemView(Frame(),"List Item");
				BMessenger messanger(Parent());
				BMessage* viewMessage= new BMessage(ADD_ITEM);
				viewMessage->AddPointer("ListItemPtr",newItem);
				viewMessage->AddPointer("OldItemPtr",this);
				messanger.SendMessage(viewMessage);
				gFuncList->UnlockAsWriter();
			}
		}
		break;		
		default:
			BView::MessageReceived(msg);
			break;
	}
}

ListItemView::FunctionTypePopup::FunctionTypePopup():
									BPopUpMenu("Function Type")
{
	AddItem(new BMenuItem("Cartesian", new BMessage(SET_TYPE_CART)));
	AddItem(new BMenuItem("Parametric", new BMessage(SET_TYPE_PARA)));
	AddItem(new BMenuItem("Polar", new BMessage(SET_TYPE_POLAR)));
};