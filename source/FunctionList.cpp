//FunctionList.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/


#include <OS.h>

#include "FunctionList.h"
#include "CartesianItemView.h"
#include "ParametricItemView.h"
#include "PolarItemView.h"

FunctionList::FunctionList(BMessage* msg):UpdateLock(5,msg)
{}

FunctionItem::FunctionItem(rgb_color newColor, float newThickness, double 
							newStep, enum FuncType newType):
							color(newColor),step(newStep),
							thickness(newThickness),type(newType)
{
	draw=true;
	switch (type)
	{
		case cart:
		case polar:
			text = new BString;
			break;
		case para:
			text = new BString[2];
			break;
		default:
			text=NULL;
	}
}

class ListItemView* FunctionItem::CreateListItemView(BRect rect, const char *title)
{
	switch (type)
	{
		case cart:
			return new class CartesianItemView( rect, title, this);
		case para:
			return new class ParametricItemView(rect,title,this);
		case polar:
			return new class PolarItemView(rect,title,this);
		default:
			return NULL;
	}
	return NULL;
}

void FunctionItem::SetType(enum FuncType newType)
{
	if (type==newType)
		return;
	
	switch (newType)
	{
		case cart:
		{
			type=newType;
			//delete text;
			text = new BString;
		}
		break;
		case para:
		{
			type=newType;
			//delete text;
			text = new BString[2];
		}
		break;
		case polar:
		{
			type=newType;
			text = new BString;
		}
		default:
			break;
	}
}