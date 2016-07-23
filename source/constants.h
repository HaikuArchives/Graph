//constants.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

const uint32 FUNC_CHG = 'FUNC';
const uint32 ADD_FUNC = 'ADDF';
const uint32 SUB_FUNC = 'SUBF';
const uint32 MENU_FILE_ABOUT = 'MFAB';
const uint32 MENU_FILE_QUIT	 = 'MFQU';
const uint32 MENU_FILE_SAVE  = 'MFSA';
const uint32 MENU_EDIT_COPY	 = 'MECO';
const uint32 MENU_WINDOW_SETTINGS = 'MWST';
const uint32 MENU_WINDOW_FUNCTION = 'MWFU';
const uint32 INVAL_GRAPH ='IVGP';
const uint32 SET_DRAW = 'STDR';


const double ixMax = 10;  //inital values for the graph.
const double ixMin = -10;
const double ixStep = 1;
const double iyMax = 10;
const double iyMin = -10;
const double iyStep = 1;
const double iThetaStep = .31415926535898;
const double iThetaStart = 0;
const double iThetaStop = 14;
const double iTStep = .31415926535898;
const double iTStart = 0;
const double iTStop = 6.2831853071796;
const char FLOAT_FORMAT[] = "%.10g";

const char INITIAL_FUNC[] = "e^sin(x)";
const rgb_color FUNCTION_COLOR = {0,0,255,0};
const rgb_color DEFAULT_COLOR = {0,0,255,0};

#endif