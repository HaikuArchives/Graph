//ColorPatch.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef COLOR_PATCH_H
#define COLOR_PATCH_H

#include <PictureButton.h>
#include <Window.h>

class ColorWindow: public BWindow
{
	public:
		ColorWindow(BRect frame, char* title,rgb_color iColor,BHandler* handler);
		void MessageReceived(BMessage* msg);
		bool QuitRequested(void);
	private:
		BColorControl* control;
		BHandler* handler;
};

class ColorPatch : public BView
{
	public:
		ColorPatch(BRect frame, char* name, rgb_color color, 
					uint32 resizingMode=B_FOLLOW_LEFT|B_FOLLOW_TOP,
					uint32 flags=B_WILL_DRAW|B_NAVIGABLE);
		~ColorPatch(void);
		void MessageReceived(BMessage* message);
		void AttachedToWindow(void);
	private:
		rgb_color color;
		BPictureButton* picButton;
		void FixButton(void);
		class ColorWindow* colorWindow;
};

		
const uint32 COLOR_WINDOW_DESTROYED = 'WNDS';
const uint32 BUTTON_CLOSE = 'BTCL';
const uint32 COLOR_REQ = 'CLRQ';
const uint32 COLOR_CHG = 'CLCG';

#endif