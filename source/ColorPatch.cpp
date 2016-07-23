//ColorPatch.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include "ColorPatch.h"
#include <ColorControl.h>
#include <Button.h>
#include <Messenger.h>

ColorPatch::ColorPatch(BRect frame, char* name, rgb_color newColor,
						uint32 resizingMode =B_FOLLOW_LEFT|B_FOLLOW_TOP,
						uint32 flags = B_WILL_DRAW | B_NAVIGABLE):
				BView(frame,name,resizingMode,flags),color(newColor),colorWindow(NULL)

{

}

ColorPatch::~ColorPatch(void)
{
	if 	(colorWindow != NULL)
		colorWindow->Quit();
}

void ColorPatch::AttachedToWindow(void)
{
	rgb_color background;
	background.red = 255;
	background.green = 255;
	background.blue = 255;
	background.alpha = 255;
	SetViewColor(background);

	BPicture* on;
	BPicture* off;
	BRect rect=Bounds();
	rgb_color black,grey;
	black.red=black.blue=black.green=0;
	black.alpha=255;
	
	grey.red=grey.green=grey.blue=128;
	grey.alpha=255;
		
	rect.InsetBy(1,1);
	
	BeginPicture(new BPicture);
	SetDrawingMode(B_OP_OVER);
	SetHighColor( black );
	StrokeRect(Bounds());
	SetHighColor(color);;
	FillRect(rect);
	on = EndPicture();
	
	BeginPicture(new BPicture);
	SetDrawingMode(B_OP_OVER);
	SetHighColor(black);
	StrokeRect(Bounds());
	SetHighColor(color);
	FillRect(rect);
	SetDrawingMode(B_OP_BLEND);
	SetHighColor(grey);
	FillRect(rect);
	off = EndPicture();
	
	picButton=new BPictureButton(Bounds(),"picture button",on,off,new BMessage(COLOR_REQ));
	picButton->SetTarget(this);
	AddChild(picButton);
	delete on;
	delete off;
}

void ColorPatch::FixButton(void)
{
	BPicture* on;
	BPicture* off;
	BRect rect=Bounds();
	rgb_color black,grey;
	black.red=black.blue=black.green=0;
	black.alpha=255;
	
	grey.red=grey.green=grey.blue=128;
	grey.alpha=255;
		
	rect.InsetBy(1,1);
	
	BeginPicture(new BPicture);
	SetDrawingMode(B_OP_OVER);
	SetHighColor( black );
	StrokeRect(Bounds());
	SetHighColor(color);;
	FillRect(rect);
	on = EndPicture();
	
	BeginPicture(new BPicture);
	SetDrawingMode(B_OP_OVER);
	SetHighColor(black);
	StrokeRect(Bounds());
	SetHighColor(color);
	FillRect(rect);
	SetDrawingMode(B_OP_BLEND);
	SetHighColor(grey);
	FillRect(rect);
	off = EndPicture();
	
	picButton->SetEnabledOn(off);
	picButton->SetEnabledOff(on);
	picButton->Invalidate();
}

void ColorPatch::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case COLOR_REQ:
		{
			 if (colorWindow!=NULL)
			 {
			 	colorWindow->Activate(true);
			 }else
			 {
			 	BRect rect=Bounds();
			 	rect.OffsetBy(Bounds().Width(),0.0f);
			 	colorWindow = new class ColorWindow(ConvertToScreen(rect),"Function Color",color,this);
			 }
		}
		break;
		case COLOR_WINDOW_DESTROYED:
			colorWindow=NULL;
		break;
		case COLOR_CHG:
		{
			const void* data;
			ssize_t size = sizeof(rgb_color);
			if (message->FindData("color",B_RGB_COLOR_TYPE,&data,&size)== B_OK)
			{
				color = *(const rgb_color*)(data);
				FixButton();
				//BMessenger(Parent()).SendMessage(message);
				BView::MessageReceived(message);
			}
		}
		break;
		default:
			BView::MessageReceived(message);
		break;
	}
}

ColorWindow::ColorWindow(BRect frame, char* title, rgb_color iColor, BHandler* newHandler):
				BWindow(frame,title, B_FLOATING_WINDOW,B_NOT_ZOOMABLE|B_NOT_RESIZABLE),
				handler(newHandler)
{
	control=new BColorControl(BPoint(5,5),B_CELLS_32x8,5.0f,"color",new BMessage(COLOR_CHG),false);
	control->ResizeToPreferred();
	control->SetValue(iColor);
	AddChild(control);
	
	BRect rect;
	rect.right=control->Frame().right;
	rect.top = control->Frame().bottom+10.0f;
	rect.bottom = rect.top+19;
	rect.left = rect.right-30;
	
	BButton* button = new BButton(rect,"Close","Close",new BMessage(BUTTON_CLOSE));
	button->ResizeToPreferred();
	button->MakeDefault(true);
	AddChild(button);
	
	ResizeTo(control->Bounds().Width()+10.0f,control->Bounds().Height()+10.0f+button->Bounds().Height()+10.0f);
	
	button->MoveTo(Bounds().Width()-(button->Bounds().Width()+5),Bounds().Height()-(button->Bounds().Height()+5.0f));

	Show();
}

void ColorWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case BUTTON_CLOSE:
		{
			BMessenger(handler).SendMessage(COLOR_WINDOW_DESTROYED);
			Quit();
		}
		break;
		case COLOR_CHG:
		{
			ssize_t size = sizeof(rgb_color);
			rgb_color color = control->ValueAsColor();
			msg->AddData("color", B_RGB_COLOR_TYPE, &color,size);
			BMessenger(handler).SendMessage(msg);
		}
		break;
		default:
			BWindow::MessageReceived(msg);
		break;
	}
}

bool ColorWindow::QuitRequested(void)
{
	BMessenger(handler).SendMessage(COLOR_WINDOW_DESTROYED);
	return true;
}
				