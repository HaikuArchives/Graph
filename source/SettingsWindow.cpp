//SettingsWindow.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include <View.h>
#include <Window.h>
#include <Box.h>
#include <Application.h>
#include <TextControl.h>
#include <stdio.h>
#include <Button.h>

#include "GraphView.h"
#include "GraphApp.h"
#include "SettingsWindow.h"
#include "constants.h"
#include "Interpreter.h"

SettingsWindow::SettingsWindow(BRect frame): BWindow(frame, "Settings",
									 B_FLOATING_WINDOW_LOOK,B_FLOATING_SUBSET_WINDOW_FEEL,
									 B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_NOT_RESIZABLE)
{
	view = new BView(Bounds(), "Settings View", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	BRect the_rect = Bounds();
	the_rect.InsetBy(5.0f, 5.0f);
	float rectHOffset = frame.Width() / 2.0f - 2.0f;
	float rectVOffset = 18.0f;
	char buffer[10];

	the_rect.right = the_rect.left + rectHOffset;
	the_rect.bottom = the_rect.top + rectVOffset;
	
	gState->Register(this);
	 
	
	if (gState->LockAsReader())
	{
		sprintf(buffer,FLOAT_FORMAT, ixMin);	
		XMinBox = new BTextControl( the_rect, "xMin", "x-Min", buffer, new BMessage(XMIN_CHG));
		XMinBox->SetTarget(this); 
		XMinBox->SetDivider(30);
		view->AddChild(XMinBox);
		
		the_rect.OffsetBy(rectHOffset + 4,0);
		sprintf(buffer,FLOAT_FORMAT, ixMax);
		XMaxBox = new BTextControl( the_rect, "xMax", "x-Max", buffer, new BMessage(XMAX_CHG));
		XMaxBox->SetTarget(this);
		XMaxBox->SetDivider(30);
		view->AddChild(XMaxBox);
		
		the_rect.OffsetBy(-(rectHOffset + 4.0f), XMinBox->Bounds().Height() + 4.0f);
		sprintf(buffer,FLOAT_FORMAT, iyMin);	
		YMinBox = new BTextControl( the_rect, "yMin", "y-Min", buffer, new BMessage(YMIN_CHG));
		YMinBox->SetTarget(this);
		YMinBox->SetDivider(30);
		view->AddChild(YMinBox);
		
		the_rect.OffsetBy(rectHOffset + 4.0f, 0.0f);
		sprintf(buffer,FLOAT_FORMAT, iyMax);	
		YMaxBox = new BTextControl( the_rect, "yMax", "y-Max", buffer, new BMessage(YMAX_CHG));
		YMaxBox->SetTarget(this);
		YMaxBox->SetDivider(30);
		view->AddChild(YMaxBox);
				
		the_rect.OffsetBy(-(rectHOffset + 4.0f), YMaxBox->Bounds().Height()+4.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellxStep());
		XStepBox = new BTextControl(the_rect,"x step", "x step", buffer, new BMessage(XSTEP_CHG));
		XStepBox->SetDivider(35);
		XStepBox->SetTarget(this);
//		XStepBox->ResizeToPreferred();
		view->AddChild(XStepBox);
		
		the_rect.OffsetBy(rectHOffset +4.0f,0.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellyStep());
		YStepBox = new BTextControl(the_rect,"y step", "y step", buffer, new BMessage(YSTEP_CHG));
		YStepBox->SetDivider(35);
		YStepBox->SetTarget(this);
//		YStepBox->ResizeToPreferred();
		view->AddChild(YStepBox);
		
		the_rect.OffsetBy(-(rectHOffset+4.0f),XStepBox->Bounds().Height()+4.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStep());
		ThetaStepBox = new BTextControl(the_rect,"theta step", "theta step", buffer, new BMessage(THETA_STEP_CHG));
		ThetaStepBox->SetDivider(50);
		ThetaStepBox->SetTarget(this);
//		ThetaStepBox->ResizeToPreferred();
		view->AddChild(ThetaStepBox);
		
		the_rect.OffsetBy(0,ThetaStepBox->Bounds().Height()+4.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStart());
		ThetaStartBox = new BTextControl(the_rect,"theta start", "theta start", buffer, new BMessage(THETA_START_CHG));
		ThetaStartBox->SetDivider(53);
		ThetaStartBox->SetTarget(this);
//		ThetaStartBox->ResizeToPreferred();
		view->AddChild(ThetaStartBox);

		the_rect.OffsetBy(rectHOffset+4.0f,0.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStop());
		ThetaStopBox = new BTextControl(the_rect,"theta stop", "theta stop", buffer, new BMessage(THETA_STOP_CHG));
		ThetaStopBox->SetDivider(50);
		ThetaStopBox->SetTarget(this);
//		ThetaStopBox->ResizeToPreferred();
		view->AddChild(ThetaStopBox);

		the_rect.OffsetBy(-(rectHOffset+4.0f),ThetaStartBox->Bounds().Height()+4.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellTStep());
		TStepBox = new BTextControl(the_rect,"t step", "t step", buffer, new BMessage(T_STEP_CHG));
		TStepBox->SetDivider(35);
		TStepBox->SetTarget(this);
//		TStepBox->ResizeToPreferred();
		view->AddChild(TStepBox);
		
		the_rect.OffsetBy(0,TStepBox->Bounds().Height()+4.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellTStart());
		TStartBox = new BTextControl(the_rect,"t start", "t start", buffer, new BMessage(T_START_CHG));
		TStartBox->SetDivider(35);
		TStartBox->SetTarget(this);
//		TStartBox->ResizeToPreferred();
		view->AddChild(TStartBox);

		the_rect.OffsetBy(rectHOffset+4.0f,0.0f);
		sprintf(buffer,FLOAT_FORMAT, gState->TellTStop());
		TStopBox = new BTextControl(the_rect,"t stop", "t stop", buffer, new BMessage(T_STOP_CHG));
		TStopBox->SetDivider(35);
		TStopBox->SetTarget(this);
//		TStopBox->ResizeToPreferred();
		view->AddChild(TStopBox);
	
		gState->UnlockAsReader();
	}
	
	
	
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	AddChild(view);
	
	ResizeTo(XMinBox->Bounds().Width() + XMaxBox->Bounds().Width() + 4.0f + 10.0f, XMinBox->Bounds().Height()*7+4.0f+ 4.0f +4.0f+4.0f+4.0f+4.0f+ 10.0f);

	Show();
	
	return;
}

bool SettingsWindow::QuitRequested(void)
{

	BMessenger(static_cast<GraphApp*>(be_app)->the_window).SendMessage(MENU_WINDOW_SETTINGS);
	return false;
}
void SettingsWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case UPDATE_VIEW_STATE:
		{
			if (gState->LockAsReader())
			{
				char buffer[20];
				sprintf(buffer,FLOAT_FORMAT, gState->TellxMax());
				XMaxBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellxMin());
				XMinBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellyMax());
				YMaxBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellyMin());
				YMinBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellxStep());
				XStepBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellyStep());
				YStepBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStep());
				ThetaStepBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStart());
				ThetaStartBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellThetaStop());
				ThetaStopBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellTStep());
				TStepBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellTStart());
				TStartBox->SetText(buffer);
				
				sprintf(buffer,FLOAT_FORMAT, gState->TellTStop());
				TStopBox->SetText(buffer);
				
				
				gState->UnlockAsReader();
			}
		}					
		break;
		case XMAX_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(XMaxBox->Text());
				if ((interpreter.Evaluate(0) > gState->TellxMin())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetxMax(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case XMIN_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(XMinBox->Text());
				if ((interpreter.Evaluate(0) < gState->TellxMax())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetxMin(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case YMAX_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(YMaxBox->Text());
				if ((interpreter.Evaluate(0) > gState->TellyMin())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetyMax(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case YMIN_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(YMinBox->Text());
				if ((interpreter.Evaluate(0) < gState->TellyMax())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetyMin(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case XSTEP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(XStepBox->Text());
				if ((interpreter.Evaluate(0) > 0.0f)&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetxStep(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case YSTEP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(YStepBox->Text());
				if ((interpreter.Evaluate(0) > 0.0f)&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetyStep(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case THETA_STEP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(ThetaStepBox->Text());
				if ((interpreter.Evaluate(0) > 0.0f)&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetThetaStep(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case THETA_START_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(ThetaStartBox->Text());
				if ((interpreter.Evaluate(0) < gState->TellThetaStop())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetThetaStart(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case THETA_STOP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(ThetaStopBox->Text());
				if ((interpreter.Evaluate(0) > gState->TellThetaStart())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetThetaStop(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case T_STEP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(TStepBox->Text());
				if ((interpreter.Evaluate(0) > 0.0f)&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetTStep(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case T_START_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(TStartBox->Text());
				if ((interpreter.Evaluate(0) < gState->TellTStop())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetTStart(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		case T_STOP_CHG:
		{
			if (gState->LockAsWriter())
			{
				class Interpreter interpreter(TStopBox->Text());
				if ((interpreter.Evaluate(0) > gState->TellTStart())&&(interpreter.errorState==no_error)&&(!isnan(interpreter.Evaluate(0))))
				{
					gState->SetTStop(interpreter.Evaluate(0));
				}
				gState->UnlockAsWriter();
			}
		}
		break;
		default:
			BWindow::MessageReceived(message);
	}
	return;
}
			
