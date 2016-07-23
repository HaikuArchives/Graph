//Interpreter.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Symbol.h"

enum ErrorState//An enum for holding error states
{
	no_error = 0,
	unrecognized_command = 1,
	unmatched_parens,
	syntax_error,
	no_function
};

class Interpreter
{
	public:
		Interpreter(const char *input);
		~Interpreter(void);
		inline double Evaluate(double newX)
		{
			x = newX;
			if (symbolTree != NULL)
				return symbolTree->Execute();
			return (double)0;
		}
		enum ErrorState errorState;
	private:
		char *inputLine;
		int inputLength;
		double x;
		class Symbol* symbolTree;
		void RemoveWhiteSpace(void);
		class Symbol* GetSymbol(int&, const char*);
		class Symbol* Parse(const char *);
		void ParseExpressionArg(class Symbol* symbol,const char* inputString,int position, int argNumber = 1);
		void ParseIntArg(class Symbol* symbol,const char* inputString, int position, int argNumber = 1);
		
		static const char NUMBERS_STRING[] = "1234567890.";
		static const char SEPERATORS_STRING[] = "+-*/^";
		static const char WHITESPACE_STRING[] = " \t\n";
		bool IsValid(char c, const char *s);
		bool IsNumber( char c );
		bool IsSeperator( char c);
		bool IsWhiteSpace( char c);
		bool ParensOK(void);
};

#endif