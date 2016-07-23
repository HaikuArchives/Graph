//FunctionClasses.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef FUNCTION_CLASSES_H
#define FUNCTION_CLASSES_H

#include "FunctionList.h"

class CartesianFunction
{
	public:
		CartesianFunction(BString* newText, rgb_color newColor, float newThickness, double newStep);
		operator()(const double);
		BString* text;

#endif