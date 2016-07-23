//FunctionListWindow.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include "FunctionListWindow.h"
#include "FunctionListView.h"
#include "constants.h"
#include "GraphApp.h"

#include <ListView.h>
#include <ScrollView.h>
#include <Button.h>



FunctionListWindow::FunctionListWindow(BRect frame):
	BWindow(frame, "Function List", B_FLOATING_WINDOW_LOOK,B_FLOATING_SUBSET_WINDOW_FEEL,
		B_ASYNCHRONOUS_CONTROLS|B_FRAME_EVENTS)
{
	BRect rect;

	SetSizeLimits(frame.Width(),frame.Width(), 95, 10000);
	
	BView* view = new BView(Bounds(), "Background View", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	
	AddChild(view);
	
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	
	BButton* moreButton = new BButton(Bounds(), "AddButton", "More", new BMessage(ADD_FUNC),
									B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	moreButton->ResizeToPreferred();
	//moreButton->ResizeTo(50,moreButton->Bounds().Height());

	moreButton->MoveTo(Bounds().Width()-(moreButton->Bounds().Width()+10),
					Bounds().Height()-(moreButton->Bounds().Height()+10));
	view->AddChild(moreButton);
	
	lessButton = new BButton(Bounds(), "AddButton", "Less", new BMessage(SUB_FUNC),
								B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	lessButton->ResizeToPreferred();
	lessButton->MoveTo(moreButton->Frame().left-lessButton->Bounds().Width()-5,
						Bounds().Height()-(lessButton->Bounds().Height()+10));
	view->AddChild(lessButton);
	
	rect = Bounds();
	
	rect.right -= B_V_SCROLL_BAR_WIDTH;
	
	rect.InsetBy(10,10);
	rect.bottom -= moreButton->Frame().Height()+5;
	
	
	functionListView=new class FunctionListView(rect, "FunctionListView");
	BScrollView* scrollView=new BScrollView("Scroll View", functionListView, 
						B_FOLLOW_ALL_SIDES, 0,false, true, B_FANCY_BORDER);
	
	view->AddChild( scrollView );
	
	scrollBar = scrollView->ScrollBar(B_VERTICAL);

	FixScrollBar();
	FixZoomSize();

	moreButton->SetTarget(functionListView);
	lessButton->SetTarget(functionListView);					
		
	Show();
}

void FunctionListWindow::FixScrollBar(void)
{
	float max;
	if ((max=functionListView->dataHeight-functionListView->Bounds().Height())<0)
		max=0;
		
	scrollBar->SetRange(0,max);
	float ratio = functionListView->Bounds().Height()/functionListView->dataHeight;
	if (ratio>1)
		ratio=1;
	scrollBar->SetProportion(ratio);
}

void FunctionListWindow::FrameResized(float width, float height)
{
	FixScrollBar();
}

bool FunctionListWindow::QuitRequested(void)
{
	BMessenger(static_cast<GraphApp*>(be_app)->the_window).SendMessage(MENU_WINDOW_FUNCTION);
	return false;
}

void FunctionListWindow::FixZoomSize(void)
{
	float delH = functionListView->dataHeight-functionListView->Bounds().Height();
	SetZoomLimits(Frame().Width(), Frame().Height() + delH);
}