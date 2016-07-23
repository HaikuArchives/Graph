//ViewState.h  //A threadsafe data structure
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef VIEW_STATE_H
#define VIEW_STATE_H

#include "UpdateLock.h"

class ViewState:public UpdateLock
{
	public:
		ViewState(double newxMin,double newxMax, double newxStep, 
					double newyMin, double newyMax, double newyStep,
					double thetaStep,double thetaStart,double thetaStop,
					double tStep,double tStart,double tStop);
		~ViewState(void);
		void SetxMax(double new_x);
		void SetxMin(double new_x);
		void SetxStep(double newXStep);
		void SetyMax(double new_y);
		void SetyMin(double new_y);
		void SetyStep(double newYStep);
		void SetThetaStep(double value);
		void SetThetaStart(double value);
		void SetThetaStop(double value);
		void SetTStep(double value);
		void SetTStart(double value);
		void SetTStop(double value);
		double TellxMax(void);
		double TellxMin(void);
		double TellxStep(void);
		double TellyMax(void);
		double TellyMin(void);
		double TellyStep(void);
		double TellThetaStep(void);
		double TellThetaStart(void);
		double TellThetaStop(void);
		double TellTStep(void);
		double TellTStart(void);
		double TellTStop(void);
	private:
		double xMin;
		double xMax;
		double xStep;
		double yMin;
		double yMax;
		double yStep;
		double thetaStep;
		double thetaStart;
		double thetaStop;
		double tStep;
		double tStart;
		double tStop;
};

//BMessage constants
const uint32 UPDATE_VIEW_STATE='UDVS';

#endif