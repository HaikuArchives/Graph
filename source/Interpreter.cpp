//Interpreter.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include <stdlib.h>

#include "Interpreter.h"

Interpreter::Interpreter(const char *input)
{
	inputLength = 1+strlen(input);
	symbolTree=NULL;
	
	inputLine = new char[inputLength];
	strcpy(inputLine, input);
	
	
	errorState = no_error;
	RemoveWhiteSpace();
	if (strlen(inputLine) == 0)
		errorState = no_function;
		
	if (!ParensOK())
	{
		errorState=unmatched_parens;
		return;
	}
	else
		symbolTree = Parse(inputLine);

	
	return;
}

Interpreter::~Interpreter(void)
{
	delete[] inputLine; 
	delete symbolTree;
	return;
}

class Symbol* Interpreter::Parse(const char* inputString)
{
	class Symbol* rootSymbol;
	class Symbol* symbol, *negator=NULL;
	
	int position=0;
	
	symbol = GetSymbol(position, inputString);
	
	if (symbol == NULL)
		return NULL;
	
	//This checks for negation
	if (symbol->Order() == addsub)
	{
		class Symbol* newSymbol = GetSymbol(position,inputString);
		if (newSymbol == NULL)
		{
			delete symbol;
			errorState = syntax_error;
			return NULL;
		}
		if (newSymbol->Catergory() == INFIX)
		{
			delete symbol;
			delete newSymbol;
			errorState=syntax_error;  //if we are negating, then the next symbol
			return NULL;				//should be anything but infix and NULL
		}
		negator=symbol;
		symbol=newSymbol;
		//symbol->AddFirst(new Number(double(0)));
		//symbol->AddLast(newSymbol);
		
	}else if (symbol->Catergory() == INFIX)
	{
		errorState = syntax_error; //we have a problem if we start with an infix
									//that isn't a plus or minus
		delete symbol;
		return NULL;
	}

	class Symbol* infixSymbol = GetSymbol(position, inputString);
	
	if (infixSymbol == NULL) //not another infix symbol? hmm, just return a negated symbol
							//or the symbol its self if there is no negator
	{
		if (negator != NULL)
		{
			negator->AddFirst(new Number(double(0)));
			negator->AddLast(symbol);
			return negator;
		} else
			return symbol;
	}
	
	if (infixSymbol->Catergory() != INFIX)  //we have 2 non infix symbols in a row.
	{
		errorState=syntax_error;
		delete symbol;
		delete infixSymbol;
		return NULL;
	}
	
	if (negator != NULL)  //if negator is not null, then I need to check to see if 
							// the infixSymbol should be evaluated before or after
							//the negation.
	{
		if (*infixSymbol <= *negator)
		{
			negator->AddFirst(new Number(double(0)));
			negator->AddLast(symbol);
			infixSymbol->AddFirst(negator);
			rootSymbol = infixSymbol;
		} else
		{
			negator->AddFirst(new Number(double(0)));
			negator->AddLast(infixSymbol);
			infixSymbol->AddFirst(symbol);
			rootSymbol = negator;
		}
	}else  //no negator, so just apply operate with infixSymbol on symbol and infixSybol
			//will now for the root of the tree
	{
		infixSymbol->AddFirst(symbol);
		rootSymbol = infixSymbol;
	}
	
	
	bool done = false;
	while ( ! done ) //the code in this loop basicly repeats what was done above,
					//except I don't have to set rootSymbol again and I can use infix symbol
					//to keep track of where we are in the tree.
	{
		negator = NULL;
		symbol = GetSymbol(position, inputString);
		
		if (symbol == NULL)
		{
		errorState = syntax_error;
			done = true;
			delete rootSymbol;
			rootSymbol=NULL;
			break;
		}
		if (symbol->Order() == addsub) 
		{
			class Symbol* newSymbol = GetSymbol(position,inputString);
			if (newSymbol == NULL)
			{
				delete symbol;
				errorState = syntax_error;
				return NULL;
			}
			if (newSymbol->Catergory() == INFIX)
			{
				delete symbol;
				delete newSymbol;
				errorState=syntax_error;
				return NULL;
			}
			negator=symbol;
			symbol=newSymbol;

			//symbol->AddFirst(new Number(double(0)));
			//symbol->AddLast(newSymbol);
		}
		else if (symbol->Catergory() == INFIX)
		{
			errorState = syntax_error;
			done = true;
			delete rootSymbol;
			rootSymbol=NULL;
			break;
		}
		class Symbol* nextInfixSymbol = GetSymbol(position, inputString);
		
		if (nextInfixSymbol == NULL)
		{
			if (negator !=NULL)
			{
				negator->AddFirst(new Number(double(0)));
				negator->AddLast(symbol);
				infixSymbol->AddLast(negator);
				done = true;
				break;
			} else
			{
				infixSymbol->AddLast(symbol);
				done = true;
				break;
			}

		}
		if (nextInfixSymbol->Catergory() != INFIX)  //we have 2 non infix symbols in a row.
		{
			errorState=syntax_error;
			delete symbol;
			delete rootSymbol;
			delete nextInfixSymbol;
			return NULL;
		}
		
		if (negator != NULL)  //if negator is not null, then I need to check to see if 
							// the infixSymbol should be evaluated before or after
							//the negation.
		{
			if (*nextInfixSymbol <= *negator)
			{
				negator->AddFirst(new Number(double(0)));
				negator->AddLast(symbol);
				nextInfixSymbol->AddFirst(negator);
				infixSymbol->AddLast(nextInfixSymbol);
				infixSymbol = nextInfixSymbol;
			} else
			{
				negator->AddFirst(new Number(double(0)));
				negator->AddLast(nextInfixSymbol);
				nextInfixSymbol->AddFirst(symbol);
				infixSymbol->AddLast(negator);
				infixSymbol = nextInfixSymbol;
			}
		}else
		{
			if (*infixSymbol >= *nextInfixSymbol)
			{
				infixSymbol->AddLast(symbol);
				infixSymbol->AddNext(nextInfixSymbol);
				infixSymbol = nextInfixSymbol;
			}
			else
			{
				nextInfixSymbol->AddFirst(symbol);
				infixSymbol->AddLast(nextInfixSymbol);
				infixSymbol = nextInfixSymbol;
			}
		}
		
	}
	return rootSymbol;
}	

class Symbol* Interpreter::GetSymbol(int& position, const char* inputString)
{
	if (IsNumber(inputString[position]))
	{
		char buffer[strlen(inputString) +1 ];
		int i=0;
		
		while (IsNumber(inputString[position]) )
		{
			buffer[i++] = inputString[position++];
		}
		buffer[i] = '\0';
		
		return new Number(strtod(buffer, NULL));
	}
	int i=0, numParens=0;
	bool done=false;

	char buffer[ strlen(inputString) + 1 ];
	
	int commandStartIndex=position;
		
	if ( inputString[position] == '\0' )
		return NULL;

	while ( ! done)
	{	 

		buffer[i]=inputString[position];
		i++;    /*error checking?  < kMaxInputLength */
		

		if ( IsSeperator(inputString[position]) )
		{
			buffer[i]='\0';
			done=true;
		}
		else
		{
			if ( inputString[position] == '(' ) 
			{
				buffer[ i ] = '\0';
				done=true;
				numParens++;
				while (numParens != 0)
				{
					position++;
					if (inputString[position] == '(' )
						numParens++;
					if (inputString[position] == ')' )
			 			numParens--;
				}
			 	
			}
			if ( IsSeperator(inputString[position+1]) )
			{
				buffer[i] = '\0';
				done=true;
			}
		}
		position++;
		if  ( inputString[position] =='\0' )
		{
			buffer[i] = '\0';
			done=true; 
		}
	}
	
	
	if  ( strcmp( buffer, "+") == 0 ) 
		return new Add;
	if  ( strcmp( buffer, "-") == 0 ) 
		return new Subtract;
	if  ( strcmp( buffer, "*") == 0 ) 
		return new Multiply;
	if  ( strcmp( buffer, "/") == 0 ) 
		return new Divide;
	if  ( strcmp( buffer, "^") == 0 ) 
		return new Exponent;
	if  ( strcmp( buffer, "(") == 0 )
	{
		class Paren* symbol= new Paren;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if  ( strcmp( buffer, "sqrt(" ) == 0 )
	{
		class SquareRoot* symbol = new SquareRoot;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "cos(" ) == 0 )
	{
		class Cosine* symbol= new Cosine;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "sin(" ) == 0 )
	{
		class Sine* symbol= new Sine;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "tan(" ) == 0 )
	{
		class Tangent* symbol= new Tangent;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "asin(" ) == 0 )
	{
		class Arcsine* symbol= new Arcsine;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "acos(" ) == 0 )
	{
		class Arccosine* symbol= new Arccosine;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "atan(" ) == 0 )
	{
		class Arctangent* symbol= new Arctangent;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "ln(" ) == 0 )
	{
		class NaturalLog* symbol= new NaturalLog;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "log(" ) == 0 )
	{
		class Log* symbol= new Log;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "sinh(" ) == 0 )
	{
		class SineH* symbol= new SineH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "cosh(" ) == 0 )
	{
		class CosineH* symbol= new CosineH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "tanh(" ) == 0 )
	{
		class TangentH* symbol= new TangentH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "asinh(" ) == 0 )
	{
		class ArcsineH* symbol= new ArcsineH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "acosh(" ) == 0 )
	{
		class ArccosineH* symbol= new ArccosineH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "atanh(" ) == 0 )
	{
		class ArctangentH* symbol= new ArctangentH;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "abs(" ) == 0)
	{
		class AbsoluteValue* symbol= new AbsoluteValue;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "erf(" ) == 0)
	{
		class Erf* symbol= new Erf;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "erfc(" ) == 0)
	{
		class Erfc* symbol= new Erfc;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "gamma(" ) == 0)
	{
		class Gamma* symbol= new Gamma;
		ParseExpressionArg(symbol,inputString,commandStartIndex);
		return symbol;
	}
	if ( strcmp( buffer, "j(" ) == 0)
	{
		class BesselOne* symbol = new BesselOne;
		ParseIntArg(symbol,inputString,commandStartIndex,1);
		ParseExpressionArg(symbol, inputString, commandStartIndex,2);
		return symbol;
	}
	if ( strcmp( buffer, "y(" ) == 0)
	{
		class BesselTwo* symbol = new BesselTwo;
		ParseIntArg(symbol,inputString,commandStartIndex,1);
		ParseExpressionArg(symbol, inputString, commandStartIndex,2);
		return symbol;
	}
	if ( strcmp( buffer, "pi" ) == 0 )
		return new Pi;
	if ( strcmp( buffer, "e" ) == 0 )
		return new ETheNumber;
	if ( strcmp( buffer, "x" ) == 0 )
		return new XTheVariable(&x);
	if ( strcmp( buffer, "t" ) == 0 )
		return new TTheVariable(&x);
	if ( strcmp( buffer, "theta") == 0 )
		return new ThetaTheVariable(&x);
	
	errorState=syntax_error;
	
	return NULL;
}

void Interpreter::ParseExpressionArg(class Symbol* symbol,const char* inputString,int position, int argNumber = 1)
{
	bool done=false;
	
	while (inputString[position] != '(')
		position++;
	position++;
	
	int argIndex = 1;
	int numParens = 1;
	
	while (argIndex != argNumber)
	{
		position++;
		
		if (inputString[position] == '(' )
			numParens++;
		if (inputString[position] == ')' )
			numParens--;
		
		if ((inputString[position] == ',' ) && (numParens == 1))
		{
			position++;
			argIndex++;
		}
		if  (numParens == 0)
			return;
	}

		
	char  *buffer = new char[strlen(inputString)+1];
	int i=0;
	
	class Symbol* argSymbol=NULL;
	
	while ( ! done )
	{
		buffer[i] = inputString[position];
		
		if (inputString[position] == '(' )
			numParens++;
		if (inputString[position] == ')' )
			numParens--;
		
		if (((inputString[position] == ',' ) && (numParens == 1)) || (numParens == 0) )
		{
			buffer[i] = '\0';
			done = true;
			argSymbol = Parse(buffer);
		}
		position++;
		i++;
	}

	symbol->AddExpressionArg(argSymbol,argNumber);
		
	return;
}

void Interpreter::ParseIntArg(class Symbol* symbol,const char* inputString, int position, int argNumber = 1)
{
	bool done=false;
	
	while (inputString[position] != '(')
		position++;
	position++;
	
	int argIndex = 1;
	int numParens = 1;
	
	while (argIndex != argNumber)
	{
		position++;
		
		if (inputString[position] == '(' )
			numParens++;
		if (inputString[position] == ')' )
			numParens--;
		
		if ((inputString[position] == ',' ) && (numParens == 1))
		{
			position++;
			argIndex++;
		}
		if  (numParens == 0)
			return;
	}

	int integer=0;
		
	char  *buffer = new char[strlen(inputString)+1];
	int i=0;
	
	
	while ( ! done )
	{
		buffer[i] = inputString[position];
		
		if (inputString[position] == '(' )
			numParens++;
		if (inputString[position] == ')' )
			numParens--;
		
		if (((inputString[position] == ',' ) && (numParens == 1)) || (numParens == 0) )
		{
			buffer[i] = '\0';
			done = true;
			integer = atoi(buffer);
		}
		position++;
		i++;
	}

	symbol->AddIntArg(integer,argNumber);
		
	return;
}


void Interpreter::RemoveWhiteSpace(void)
{
	char buffer[inputLength];

	int i=0, j=0;

	while ( inputLine[i] != '\0' )
	{
		if (! IsWhiteSpace( inputLine[i] ) )
		{
			buffer[j] = inputLine[i];
			j++;
		}
		i++;
	}

	buffer[j] = '\0';

	strcpy(inputLine, buffer);

	return;
}

/********** IsValid **********/
/*Takes a character as an argument and returns whether or not that character
 *is in the string.*/
bool Interpreter::IsValid(char c, const char *s)
{
	static int i=0;
	if (s[i] == '\0')
	{
		i=0;
		return false;
	}
	if (s[i] == c)
	{
		i=0;
		return true;
	}

	i++;
	return IsValid(c, s);
}
bool Interpreter::IsNumber( char c )
{
	return IsValid( c , NUMBERS_STRING);
}
bool Interpreter::IsSeperator( char c)
{
	return IsValid(c , SEPERATORS_STRING);
}
bool Interpreter::IsWhiteSpace( char c)
{
	return IsValid(c, WHITESPACE_STRING);
}

bool Interpreter::ParensOK( void )
{
	int i=0,numParens=0;
	while (inputLine[i] != '\0' )
	{
		if (inputLine[i] == '(')
			numParens++;
		if (inputLine[i] == ')' )
			numParens--;
		i++;
	}
	if (numParens!=0)
		return false;

	return true;
}
