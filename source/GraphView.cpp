//GraphView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#include <Rect.h>
#include <View.h>
#include <math.h>
#include <Clipboard.h>
#include <Message.h>
#include <StopWatch.h>
#include <Path.h>
#include <File.h>
#include <Entry.h>
#include <TranslatorRoster.h>
#include <BitmapStream.h>
#include <String.h>

//	DA@Y2K:
//	Needed in GraphView::Draw().
#include <Screen.h>


#include "GraphApp.h"
#include "Interpreter.h"
#include "GraphView.h"
#include "constants.h"
#include "GraphWindow.h"
#include "ViewState.h"

//Globals
class FunctionList* gFuncList=new class FunctionList(new BMessage(INVAL_GRAPH));
class ViewState* gState=new ViewState(ixMin,ixMax,ixStep,iyMin,iyMax,iyStep,
										iThetaStep,iThetaStart,iThetaStop,
										iTStep,iTStart,iTStop);

GraphView::GraphView(BRect frame):
	BView(frame, "GraphView", B_FOLLOW_ALL_SIDES,B_WILL_DRAW/*|B_FULL_UPDATE_ON_RESIZE*/|B_FRAME_EVENTS|B_SUBPIXEL_PRECISE),
	lastSingleClickTime(0)
{
	
	
	if (gFuncList->LockAsWriter());
	{
		gFuncList->push_back(new class FunctionItem(DEFAULT_COLOR, 1, 1, cart));
		*(gFuncList->back()->text)="10*j(1,5*x)";
		rgb_color tempColor={255,0,0,255};
		gFuncList->push_back(new class FunctionItem(tempColor,1,1,para));
		gFuncList->back()->text[0].SetTo("5*sin(t)");
		gFuncList->back()->text[1].SetTo("5*cos(t)");
		rgb_color tempColor2={0,255,0,255};
		gFuncList->push_back(new class FunctionItem(tempColor2 ,1,1,polar));
		*(gFuncList->back()->text)="theta";
		
		//gFuncList->push_back(new class FunctionItem(new BString("sin(x)"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString("cos(x)"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString("x"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString("-x"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString("2x"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString("x/10"), DEFAULT_COLOR,1,1,cart));
		//gFuncList->push_back(new class FunctionItem(new BString(".5"), DEFAULT_COLOR,1,1,cart));
	
		gFuncList->UnlockAsWriter();
	}
	
	Offscreen=NULL;
	OffscreenView=NULL;
	
	SetViewColor(B_TRANSPARENT_COLOR);
	
	
	//	DA@Y2K:
	//	Make sure to create the offscreen bitmap in the same color space as the current screen.
	//	Although this might sound expensive memory-wise, it makes the call to DrawBitmap() faster.
	//	Note the brackets around the block -- you want to keep around a BScreen object as shortly
	//	as possible since it locks up screen access while it is alive.
	
	{
		BScreen screen(Window());
		if (screen.IsValid())
			bitmapColorSpace= screen.ColorSpace();
		else
			bitmapColorSpace= B_RGB16;
	}
	
	zoomFactor=2;
	
	listChangedSinceLastDraw = true;
	SetTracking(false);
	SetZooming(false);

	
	return;
}

void GraphView::AttachedToWindow(void)
{
	gState->Register(this);
	gFuncList->Register(this);
}

void GraphView::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case INVAL_GRAPH:
		case UPDATE_VIEW_STATE:
		{
			UpdateGraph();
		}
		break;
		default:
			BView::MessageReceived(msg);
	}
}

void GraphView::Draw(BRect updateRect)
{
	//BStopWatch watch("Draw()");
	
	//If a portion of the screen is invalidated, but no drawing needs
	//to be done, just draw the pixmap out of memory.
	if ((listChangedSinceLastDraw == false) && (Offscreen != NULL) )
	{
		DrawBitmap(Offscreen, updateRect, updateRect);
		return;
	}

	
	//delete Offscreen;
	if (Offscreen == NULL)
	{
		Offscreen = new BBitmap(Bounds(), bitmapColorSpace, true);
		OffscreenView = new GraphOffscreenView(Bounds());
		Offscreen->AddChild(OffscreenView);
	}

	memset(Offscreen->Bits(), -1 , Offscreen->BitsLength());
	Offscreen->Lock();
	OffscreenView->DrawGraph(updateRect);
	OffscreenView->Sync();
	listChangedSinceLastDraw = false;
	Offscreen->Unlock();
	
	DrawBitmap(Offscreen);
	
	return;

}

void GraphView::FrameResized(float width, float height)
{
	delete Offscreen;
	Offscreen=NULL;
	Invalidate();
	return;
}	


void GraphView::MouseDown(BPoint point)
{
	//	DA@Y2K:
	//	Extract information about the current event.
	
	BMessage *clickMessage = Window()->CurrentMessage();
	clicks = clickMessage->FindInt32("clicks");
	modifiers = clickMessage->FindInt32("modifiers");
	int32 buttons = clickMessage->FindInt32("buttons");
	
	bigtime_t clickSpeed;
	clicks = clicks%2;
	if (clicks==0)
		clicks=2;
	
	if ((get_click_speed(&clickSpeed)==B_OK)&&(clicks == 2))
	{
		if ((real_time_clock_usecs()-lastSingleClickTime) > clickSpeed)
			clicks=1;
	}
	
	if (clicks == 1)
	{
		lastSingleClickTime = real_time_clock_usecs();
	}
	
	
	
	if ((modifiers & B_SHIFT_KEY) && (clicks == 1) && (buttons & B_PRIMARY_MOUSE_BUTTON))
	{
		SetZooming(true);
	}
	else if ((clicks == 1) && (buttons & B_PRIMARY_MOUSE_BUTTON))
	{
		SetTracking(true);
	}
	MouseDStart = point;
	SetMouseEventMask(B_POINTER_EVENTS,B_LOCK_WINDOW_FOCUS|B_NO_POINTER_HISTORY);
}

void GraphView::MouseMoved(BPoint point,uint32 flags,const BMessage* a_message)
{
	static BPoint prevPoint=MouseDStart;
	if (IsTracking() && (point != MouseDStart))
	{
		if (gState->LockAsWriter())
		{
			float delx = (point.x - MouseDStart.x)*(gState->TellxMax() - gState->TellxMin())
																			/Bounds().Width();
			float dely = (point.y - MouseDStart.y)*(gState->TellyMax() - gState->TellyMin())
																			/Bounds().Height();
			
			gState->SetxMax(gState->TellxMax() - delx);
			gState->SetyMin(gState->TellyMin() + dely);
			gState->SetxMin(gState->TellxMin() - delx);
			gState->SetyMax(gState->TellyMax() + dely);
			MouseDStart=point;
			
			gState->UnlockAsWriter();
		}
	}
	else if (IsZooming() && (point != prevPoint))
	{
		prevPoint=point;
		BRect selectRect(MouseDStart, point);
		Invalidate();
		Window()->UpdateIfNeeded();
		StrokeRect(selectRect);

	}
	return;
}

void GraphView::MouseUp(BPoint point)	
{

	SetTracking(false);
	if (IsZooming())
	{
		SetZooming(false);
		
		if (point == MouseDStart)
			return;
		
		double width = Bounds().Width();
		double height = Bounds().Height();
		
		if (gState->LockAsWriter())
		{		
			double deltaX = (gState->TellxMax() - gState->TellxMin())/width;
			double deltaY = (gState->TellyMax() - gState->TellyMin())/height;
			
				
			double xMax = point.x*deltaX +gState->TellxMin(); //new xMax
			double xMin = MouseDStart.x*deltaX + gState->TellxMin(); //new xMin
			double yMax = (height-point.y)*deltaY + gState->TellyMin(); //new yMax;
			double yMin = (height-MouseDStart.y)*deltaY + gState->TellyMin(); // new yMin
			
			//now to fix the values in case point.x < MouseDStart.x (i.e. the user drew a rectangle
			//from right to left)
			if (xMax < xMin)
			{
				double num = xMax;
				xMax=xMin;
				xMin=num;
			}
			if (yMax < yMin)
			{
				double num = yMax;
				yMax=yMin;
				yMin=num;
			}
			
			gState->SetxMax(xMax);
			gState->SetxMin(xMin);
			gState->SetyMax(yMax);
			gState->SetyMin(yMin);
			
			gState->UnlockAsWriter();
		}
	
	}else if (clicks == 2 )
	{
		double zoom=zoomFactor;
		if (modifiers & B_SHIFT_KEY)
			zoom = 1/zoomFactor;
		
		if (gState->LockAsWriter())
		{	
			gState->SetxMax(gState->TellxMax() * zoom);
			gState->SetyMin(gState->TellyMin() * zoom);
			gState->SetxMin(gState->TellxMin() * zoom);
			gState->SetyMax(gState->TellyMax() * zoom);
	
			gState->UnlockAsWriter();
		}
	}
	
}

void GraphView::CopyGraph(void)
{
	//	DA@Y2K:
	//	The following code works with Gobe Productive. It should also work with any application
	//	that understands native bitmaps, pretty much everything out there.
	
	if (be_clipboard->Lock()) 
	{ 
		be_clipboard->Clear(); 
		if (BMessage* clip = be_clipboard->Data())
		{
			BMessage msg(B_MIME_TYPE);
			if (Offscreen->Lock())
			{
				Offscreen->Archive(&msg);
				Offscreen->Unlock();
			}
			clip->AddMessage("image/x-vnd.Be-bitmap", &msg);
			be_clipboard->Commit();
		} 
		be_clipboard->Unlock(); 
	}
	return;
}

void GraphView::SaveGraph(const entry_ref& saveDir, const BString& fileName)
{
	BEntry entry;
	BPath path;
	
	entry.SetTo(&saveDir);
	entry.GetPath(&path);
	path.Append(fileName.String());
	
	BFile saveFile(path.Path(), B_CREATE_FILE|B_WRITE_ONLY);
	
	BTranslatorRoster *roster = BTranslatorRoster::Default(); 
	BBitmapStream stream(Offscreen); // init with contents of bitmap
	Offscreen=NULL;
	Invalidate();
	roster->Translate(&stream, NULL, NULL, &saveFile, B_TRANSLATOR_BITMAP);
	
}

GraphOffscreenView::GraphOffscreenView(BRect frame):
						 BView(frame, "GraphOffscreenView", 0,B_WILL_DRAW)
{
	return;
}

void GraphOffscreenView::DrawGraph(BRect updateRect)
{
	//BStopWatch watch("DrawGraph");
	int i=0;
	BPoint start,end;

	float ynot; //xnot and ynot are the pixel location of the x and y axies.
	float xnot;
	
	double width = Bounds().Width();
	double height = Bounds().Height();
	if (gState->LockAsReader())
	{
		double deltaX = (gState->TellxMax() - gState->TellxMin())/width; //range in the graph that each pixel represents
		double deltaY = (gState->TellyMax() - gState->TellyMin())/height;
		
		xnot = gState->TellxMin() /deltaX; //calculate where the x and y axis should go.
		ynot = gState->TellyMax() /deltaY;
		
		xnot = -1 * xnot;
		
		if (!isnan(ynot))
			StrokeLine(BPoint(0,ynot),BPoint(width,ynot));
		if (!isnan(xnot))
			StrokeLine(BPoint(xnot,0),BPoint(xnot,height));
		
		int32 nStart = static_cast<int32>(gState->TellxMin()/gState->TellxStep());
		int32 nStop = static_cast<int32>(ceil(gState->TellxMax()/gState->TellxStep()));
		
		int32 j;
		
		BeginLineArray(nStop-nStart+1);
		for (j=nStart; j<=nStop; j++)
			AddLine(BPoint(xnot+j*(gState->TellxStep()/deltaX),ynot+2),BPoint(xnot+j*(gState->TellxStep()/deltaX),ynot-2),HighColor());
		EndLineArray();
		
		nStart = static_cast<int32>(gState->TellyMin()/gState->TellyStep());
		nStop = static_cast<int32>(gState->TellyMax()/gState->TellyStep());
		BeginLineArray(nStop-nStart+1);
		for (j=nStart; j<=nStop; j++)
			AddLine(BPoint(xnot-2,ynot-j*(gState->TellyStep()/deltaY)),BPoint(xnot+2,ynot-j*(gState->TellyStep()/deltaY)),HighColor());
		EndLineArray();
		
		
		if (gFuncList->LockAsReader())
		{
			func_data_t::iterator functionIter = gFuncList->begin();
			
			
			while (functionIter != gFuncList->end())
			{ 
				i=0;
				if ((*functionIter)->text->String() == 0)
					continue;
					
				while (!(*functionIter)->ShouldDraw()) 
				{
					functionIter++;
					if (functionIter == gFuncList->end())
						break;
				}
				if (functionIter == gFuncList->end())
					break;

				
				switch ((*functionIter)->Type())
				{
					case cart:
						{
							class Interpreter function((*functionIter)->text->String());
							
							start.Set(i,height - ( function.Evaluate(i*deltaX + gState->TellxMin() ) - gState->TellyMin())/deltaY );
							//All these complicated BPoint calls all convert the output of a fuction (i.e. Y()) to a pixel value on the screen
							//This involves, first calculating what the give x value is for a give pixel value (i.e. (pixel number)*(length of scale)/(# of pixals across) )
							//then converting the resultant y value to an approaite height from the bottom of the view.  (i.e.
										//(pixel height) - (Y)*(pixel height) / (lenght of scale)
					
							if ( function.errorState == no_error)
							{
								BeginLineArray(width);
								
								do
								{	
									
									//watch = new BStopWatch("core loop");
									if(!isnan(start.y))
									{
										end.Set((i+1),height - (function.Evaluate((i+1)*deltaX + gState->TellxMin() ) - gState->TellyMin())/deltaY );
										if (!isnan(end.y))
											AddLine(start, end ,  (*functionIter)->Color());
										start=end;
									}else
									{
										start = BPoint(i+1,height - (function.Evaluate((i+1)*deltaX + gState->TellxMin() ) - gState->TellyMin())/deltaY );
									}
									i++;
									//delete watch;
								}while (i < width);
								EndLineArray();
							}
						}
						break;
					case para:
						{
							class Interpreter xOfT((*functionIter)->text[0].String());
							class Interpreter yOfT((*functionIter)->text[1].String());
							//All these complicated BPoint calls all convert the output of a fuction (i.e. Y()) to a pixel value on the screen
							//This involves, first calculating what the give x value is for a give pixel value (i.e. (pixel number)*(length of scale)/(# of pixals across) )
							//then converting the resultant y value to an approaite height from the bottom of the view.  (i.e.
										//(pixel height) - (Y)*(pixel height) / (lenght of scale)
							int steps;
							double tStep=gState->TellTStep();
							double tStart=gState->TellTStart();
							double tStop=gState->TellTStop();
							start.Set((xOfT.Evaluate(tStart)-gState->TellxMin())/deltaX ,
													height-( yOfT.Evaluate(tStart)-gState->TellyMin() )/ deltaY);

							if (( xOfT.errorState == no_error)&&(yOfT.errorState == no_error))
							{
								steps=int(floor((tStop-tStart)/tStep));
								if ((tStep*steps+tStart)<tStop)
									BeginLineArray(steps+1);
								else
									BeginLineArray(steps+1);
								
								do
								{	
									tStart+=tStep;							
									//watch = new BStopWatch("core loop");
									if ((!isnan(start.y))&&(!isnan(start.x)))
									{
										end.Set((xOfT.Evaluate(tStart)-gState->TellxMin())/deltaX ,
														height-( yOfT.Evaluate(tStart)-gState->TellyMin() )/ deltaY);
										if ((!isnan(end.y))&&(!isnan(end.x)))
											AddLine(start, end ,  (*functionIter)->Color());
										start=end;
									}else
									{
										start = BPoint((xOfT.Evaluate(tStart)-gState->TellxMin())/deltaX ,
														height-( yOfT.Evaluate(tStart)-gState->TellyMin() )/ deltaY);
									}
									i++;
									//delete watch;
								}while (i < steps);
								if (tStart<tStop)
								{
						
									end.Set((xOfT.Evaluate(tStop)-gState->TellxMin())/deltaX ,
											height-( yOfT.Evaluate(tStop)-gState->TellyMin() )/ deltaY);
									if ((!isnan(end.y))&&(!isnan(end.x))&&(!isnan(start.y))&&(!isnan(start.x)))
										AddLine(start, end ,  (*functionIter)->Color());
								}
								EndLineArray();
							}

						}
						break;
					case polar:
						{
							class Interpreter rOfTheta((*functionIter)->text->String());
							//All these complicated BPoint calls all convert the output of a fuction (i.e. Y()) to a pixel value on the screen
							//This involves, first calculating what the give x value is for a give pixel value (i.e. (pixel number)*(length of scale)/(# of pixals across) )
							//then converting the resultant y value to an approaite height from the bottom of the view.  (i.e.
										//(pixel height) - (Y)*(pixel height) / (lenght of scale)
							int steps;
							double thetaStep=gState->TellThetaStep();
							double thetaStart=gState->TellThetaStart();
							double thetaStop=gState->TellThetaStop();
							start.Set((rOfTheta.Evaluate(thetaStart)*cos(thetaStart)-gState->TellxMin())/deltaX ,
													height-( rOfTheta.Evaluate(thetaStart)*sin(thetaStart)-gState->TellyMin() )/ deltaY);

							if (rOfTheta.errorState == no_error)
							{
								steps=int(floor((thetaStop-thetaStart)/thetaStep));
								if ((thetaStep*steps+thetaStart)<thetaStop)
									BeginLineArray(steps+1);
								else
									BeginLineArray(steps+1);
								
								do
								{	
									thetaStart+=thetaStep;							
									//watch = new BStopWatch("core loop");
									if ((!isnan(start.y))&&(!isnan(start.x)))
									{
										end.Set((rOfTheta.Evaluate(thetaStart)*cos(thetaStart)-gState->TellxMin())/deltaX ,
													height-( rOfTheta.Evaluate(thetaStart)*sin(thetaStart)-gState->TellyMin() )/ deltaY);
										if ((!isnan(end.y))&&(!isnan(end.x)))
											AddLine(start, end ,  (*functionIter)->Color());
										start=end;
									}else
									{
										start = BPoint((rOfTheta.Evaluate(thetaStart)*cos(thetaStart)-gState->TellxMin())/deltaX ,
													height-( rOfTheta.Evaluate(thetaStart)*sin(thetaStart)-gState->TellyMin() )/ deltaY);
									}
									i++;
									//delete watch;
								}while (i < steps);
								if (thetaStart<thetaStop)
								{
						
									end.Set((rOfTheta.Evaluate(thetaStop)*cos(thetaStop)-gState->TellxMin())/deltaX ,
													height-( rOfTheta.Evaluate(thetaStop)*sin(thetaStop)-gState->TellyMin() )/ deltaY);
									if ((!isnan(end.y))&&(!isnan(end.x))&&(!isnan(start.y))&&(!isnan(start.x)))
										AddLine(start, end ,  (*functionIter)->Color());
								}
								EndLineArray();
							}

						}
						break;							
					default:
						break;
				}
				functionIter++;
			}  //end while
			
			gFuncList->UnlockAsReader();
		}
	
		gState->UnlockAsReader();		
	}
	
	return;
}
