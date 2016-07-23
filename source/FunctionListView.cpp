//FunctionListView.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include <TextControl.h>
#include <stdio.h>
#include <Box.h>

#include "GraphApp.h"
#include "GraphView.h"
#include "FunctionListWindow.h"
#include "FunctionListView.h"
#include "constants.h"
#include "ColorPatch.h"

FunctionListView::FunctionListView(BRect frame, const char* name):
			BView(frame, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	rgb_color background;
	background.red = 245;
	background.green = 245;
	background.blue = 245;
	background.alpha = 255;
	SetViewColor(background);

	dataHeight=0;
	

	return;
}

void FunctionListView::AttachedToWindow(void)
{
		if (listItems.size() <= 1)
		{
			dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(false);
		}
}

void FunctionListView::AllAttached(void)
{
	FunctionList::iterator functionIter;
	BRect rect=Bounds();
	
	if (gFuncList->LockAsReader())
	{

		functionIter = gFuncList->begin();
		while (functionIter != gFuncList->end())
		{
			ListItemView* tempItem=(*functionIter)->CreateListItemView(rect, "List Item");
			AddChild(tempItem);
			rect.bottom=(rect.top=tempItem->Frame().bottom+1)+1;
			listItems.push_back(tempItem);
			functionIter++;
		}
		gFuncList->UnlockAsReader();
	}

	dataHeight=listItems.back()->Frame().bottom;
		
	if (dataHeight <0)
		dataHeight=0;
	
	if (listItems.size() > 1)
	{
		dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(true);
	}
}

void FunctionListView::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case ADD_ITEM:
		{
			void *data;
			void *oldData;
			message->FindPointer("ListItemPtr",&data);
			message->FindPointer("OldItemPtr",&oldData);
			class ListItemView* item=static_cast<class ListItemView*>(data);
			class ListItemView* oldItem=static_cast<class ListItemView*>(oldData);
			
			vector<class ListItemView*>::iterator iter;
			
			iter=listItems.begin();
			while ( iter!=listItems.end())
			{
				if (*iter == oldItem)
				{
					*iter=item;
					AddChild(item);
					oldItem->RemoveSelf();
					break;
				}
				else
				iter++;
			}
			while ( iter!=listItems.end())
			{
				class ListItemView* prev=*iter;
				iter++;
				if (iter!=listItems.end())
					(*iter)->MoveTo(0,prev->Frame().bottom+1);
			}
			dataHeight=listItems.back()->Frame().bottom;
		
			if (dataHeight <0)
				dataHeight=0;
			
			if (listItems.size() > 1)
			{
				dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(true);
			}
			dynamic_cast<class FunctionListWindow*>(Window())->FixScrollBar();
			dynamic_cast<class FunctionListWindow*>(Window())->FixZoomSize();
		}
		break;
		case ADD_FUNC:
		{
			if (gFuncList->LockAsWriter())
			{
				BRect rect=Bounds();
				rect.bottom=(rect.top=dataHeight+1)+1;
				FuncType type=listItems.back()->function->Type();
				gFuncList->push_back(new class FunctionItem(DEFAULT_COLOR,1,1,type));
				ListItemView* tempItem=gFuncList->back()->CreateListItemView( rect, "List Item" );
				AddChild(tempItem);
				dataHeight += tempItem->Frame().Height()+1;
				listItems.push_back(tempItem);
				if (listItems.size() > 1)
				{
					dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(true);
				}
				gFuncList->UnlockAsWriter();
				dynamic_cast<class FunctionListWindow*>(Window())->FixScrollBar();
				dynamic_cast<class FunctionListWindow*>(Window())->FixZoomSize();
			}
		}
		break;
		case SUB_FUNC:
		{
			if (gFuncList->LockAsWriter())
			{
				if (listItems.size() == 1)
				{
					dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(false);
					gFuncList->UnlockAsWriter();
					break;
				}
				ListItemView* view = listItems.back();
				listItems.pop_back();
				view->RemoveSelf();
				dataHeight -= view->Bounds().Height()+1;
				delete view;
				gFuncList->pop_back();
				gFuncList->UnlockAsWriter();
				if (listItems.size() == 1)
				{
					dynamic_cast<class FunctionListWindow*>(Window())->lessButton->SetEnabled(false);
				}
				dynamic_cast<class FunctionListWindow*>(Window())->FixScrollBar();
				dynamic_cast<class FunctionListWindow*>(Window())->FixZoomSize();


			}
		}
		break;
		default:
			BView::MessageReceived(message);
			break;
	}
}

