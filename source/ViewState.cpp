//ViewState.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include <Messenger.h>
#include "ViewState.h"

ViewState::ViewState(double newxMin,double newxMax, double newxStep, 
			double newyMin, double newyMax, double newyStep,
			double newThetaStep, double newThetaStart, double newThetaStop,
			double newTStep, double newTStart, double newTStop):
			UpdateLock(5,new BMessage(UPDATE_VIEW_STATE)),xMin(newxMin),xMax(newxMax),xStep(newxStep),
			yMin(newyMin),yMax(newyMax),yStep(newyStep),
			thetaStep(newThetaStep),thetaStart(newThetaStart),thetaStop(newThetaStop),
			tStep(newTStep),tStart(newTStart),tStop(newTStop)
{}

void ViewState::SetxMax(double new_x)
{
	xMax = new_x;
	return;
}

void ViewState::SetyMax(double new_y)
{
	yMax = new_y;
	return;
}

void ViewState::SetxMin(double new_x)
{
	xMin = new_x;
	return;
}

void ViewState::SetyMin(double new_y)
{
	yMin = new_y;
	return;
}

void ViewState::SetxStep(double newXStep)
{
	xStep=newXStep;
	return;
}

void ViewState::SetyStep(double newYStep)
{
	yStep = newYStep;
	return;
}
void ViewState::SetThetaStep(double value)
{
	thetaStep=value;
	return;
}

void ViewState::SetThetaStart(double value)
{
	thetaStart=value;
	return;
}

void ViewState::SetThetaStop(double value)
{
	thetaStop=value;
	return;
}

void ViewState::SetTStep(double value)
{
	tStep=value;
	return;
}

void ViewState::SetTStart(double value)
{
	tStart=value;
	return;
}

void ViewState::SetTStop(double value)
{
	tStop=value;
	return;
}

double ViewState::TellxMax(void)
{
	return xMax;
}

double ViewState::TellxMin(void)
{
	return xMin;
}

double ViewState::TellyMax(void)
{
	return yMax;
}

double ViewState::TellyMin(void)
{
	return yMin;
}

double ViewState::TellxStep(void)
{
	return xStep;
}

double ViewState::TellyStep(void)
{
	return yStep;
}

double ViewState::TellThetaStep(void)
{
	return thetaStep;
}

double ViewState::TellThetaStart(void)
{
	return thetaStart;
}

double ViewState::TellThetaStop(void)
{
	return thetaStop;
}

double ViewState::TellTStep(void)
{
	return tStep;
}

double ViewState::TellTStart(void)
{
	return tStart;
}

double ViewState::TellTStop(void)
{
	return tStop;
}

