//SettingsWindow.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <Window.h>
#include <Button.h>
#include <TextControl.h>
#include "GraphView.h"

class SettingsWindow:public BWindow
{
	public:
		SettingsWindow(BRect frame);
		bool QuitRequested(void);
		void MessageReceived(BMessage *);
	private:
		BView *view;
		BTextControl* XMaxBox;
		BTextControl* XMinBox;
		BTextControl* YMaxBox;
		BTextControl* YMinBox;
		BTextControl* XStepBox;
		BTextControl* YStepBox;
		BTextControl* ThetaStepBox;
		BTextControl* ThetaStartBox;
		BTextControl* ThetaStopBox;
		BTextControl* TStepBox;
		BTextControl* TStartBox;
		BTextControl* TStopBox;
};

const uint32 XMAX_CHG = 'XMAX';
const uint32 XMIN_CHG = 'XMIN';
const uint32 YMAX_CHG = 'YMAX';
const uint32 YMIN_CHG = 'YMIN';
const uint32 XSTEP_CHG = 'XSCG';
const uint32 YSTEP_CHG = 'YSCG';
const uint32 THETA_STEP_CHG = 'THSP';
const uint32 THETA_START_CHG = 'THSR';
const uint32 THETA_STOP_CHG = 'THSO';
const uint32 T_STEP_CHG = 'TSPC';
const uint32 T_START_CHG = 'TSRC';
const uint32 T_STOP_CHG = 'TSOC';


#endif