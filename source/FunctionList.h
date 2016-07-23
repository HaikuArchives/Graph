//FunctionList.h
//A vector with with some locking functions
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef FUNTIONLIST_H
#define FUNTIONLIST_H

#include <vector.h>
#include <View.h>
#include <String.h>
#include <OS.h>
#include <Rect.h>
#include <Locker.h>

#include "UpdateLock.h"

class ListItemView;

typedef enum FuncType
{
	invalid,
	cart,
	polar,
	para
} FuncType;

typedef class FunctionItem
{
	public:
		FunctionItem(rgb_color newColor, float newThickness, double newStep, enum FuncType newType);
		inline const rgb_color& Color(void){return color;};
		//virtual operator()(const double){return 0;};
		inline const float& Thickness(void){return thickness;};
		inline const double& Step(void){return step;};
		inline const enum FuncType& Type(void){return type;};
		inline bool ShouldDraw(void){return draw;};
		inline void SetDraw(bool newDraw){draw=newDraw;};
		inline void SetColor(rgb_color newColor){color = newColor;};
		inline void SetThickness(float newThickness){thickness=newThickness;};
		inline void SetStep(double newStep){step=newStep;};
		void SetType(enum FuncType newType);
		
		class ListItemView* CreateListItemView(BRect rect, const char *title);
		BString* text;
	private:
		rgb_color color;
		double step;
		float thickness;
		enum FuncType type;
		bool draw;
} FunctionItem;


typedef vector<class FunctionItem*> func_data_t;

class FunctionList: public vector<FunctionItem*>,public UpdateLock
{
	public:
		FunctionList(BMessage* msg);
};			


#endif

