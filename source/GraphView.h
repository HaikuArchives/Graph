//GraphView.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <View.h>
#include <Rect.h>
#include <list.h>
#include <Bitmap.h>
#include "constants.h"
#include "FunctionList.h"
#include "ViewState.h"

//a global  (yes, I know; globals are bad, m'kay)
extern class FunctionList* gFuncList;
extern class ViewState* gState;

class GraphView : public BView
{
	public:
		GraphView(BRect frame);
		virtual void Draw(BRect updateRect);
		virtual void FrameResized(float width, float height);
		virtual void MouseDown(BPoint);
		virtual void MouseMoved(BPoint,uint32,const BMessage*);
		virtual void MouseUp(BPoint);
		virtual void AttachedToWindow(void);
		virtual void MessageReceived(BMessage* msg);
		inline void UpdateGraph(void){listChangedSinceLastDraw=true;Invalidate();};
		void CopyGraph(void);
		void SaveGraph(const entry_ref&, const BString&);
	private:
		bool IsTracking(void);
		void SetTracking(bool);
		bool IsZooming(void);
		void SetZooming(bool);
		bool tracking;
		bool zooming;
		int32 clicks;
		int32 modifiers;
		double zoomFactor;
		BPoint MouseDStart;
		BBitmap* Offscreen;
		color_space bitmapColorSpace;
		class GraphOffscreenView* OffscreenView;
		bool listChangedSinceLastDraw;
		bigtime_t lastSingleClickTime;  //a variable to check for double clicks


};
inline void GraphView::SetTracking(bool value)
{	
	tracking=value;
	return;
}
inline bool GraphView::IsTracking(void)
{
	return tracking;
}

inline void GraphView::SetZooming(bool value)
{
	zooming=value;
	return;
}

inline bool GraphView::IsZooming(void)
{
	return zooming;
}

class GraphOffscreenView : public BView
{
	public:
		GraphOffscreenView(BRect frame);
		void DrawGraph(BRect updateRect);
};


#endif