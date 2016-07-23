//Symbol.h
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <math.h>
#include <stddef.h>

enum SymbolOrder
{
	NO_ORDER = 0,
	number=1,
	addsub=5,
	multdiv = 10,
	exponent = 20,

	function = 50,
	constant = 55
};

enum SymbolCatergory
{
	NONE,
	NUMBER,
	INFIX,
	CONSTANT,
	SINGLE_ARGUMENT,
	BESSEL,
	OTHER
};

enum ArgType
{
	EXPRESSION,
	INTERGER
};

class Symbol
{
	public:
		Symbol(void) {first=last=next=NULL;}
		virtual double Execute(void)=0;
		virtual inline void AddExpressionArg(Symbol* argSymbol, int argNumber){delete argSymbol;};
		virtual inline void AddIntArg(int integer, int argNumber){return;};
		inline enum SymbolCatergory Catergory(void) const {return catergory;};
		inline enum SymbolOrder Order(void) const {return order;};
		inline void AddFirst(class Symbol* newFirst) {if (first!=NULL) delete first; first = newFirst; return;}
		inline void AddLast(class Symbol* newLast) {if (last!=NULL) delete last; last = newLast; return;}
		inline void AddNext(class Symbol* newNext) {if (next!=NULL) delete next; next = newNext; return;}
	protected:
		enum SymbolOrder order;
		enum SymbolCatergory catergory;
		class Symbol* first;
		class Symbol* last;
		class Symbol* next;
};

class SingleExpressionArg: public Symbol
{
	public:
		virtual inline void AddExpressionArg(Symbol* argSymbol, int argNumber)
		{
			if (argNumber == 1)
				first = argSymbol;
			else
				delete argSymbol;
		};
};

inline bool operator< (const class Symbol& a,const class Symbol& b)
{
	return a.Order() < b.Order();
}
 
inline bool operator== (const class Symbol& a,const class Symbol& b)
{
	return a.Order() ==  b.Order();
}

inline bool operator> (const class Symbol& a,const class Symbol& b)
{
	return b<a;
}

inline bool operator<= (const class Symbol& a,const class Symbol& b)
{
	return !(b<a);
}

inline bool operator>= (const class Symbol& a,const class Symbol& b)
{
	return !(a<b);
}

inline bool operator!= (const class Symbol& a,const class Symbol& b)
{
	return !(b==a);
}

//some basic class definitions

class Number:public Symbol
{
	public:
		inline Number(double num):Symbol() {theNumber=num; order=number; catergory=NUMBER;}
		double Execute(void) {return theNumber;}
	private:
		double theNumber;
};

class ETheNumber:public Symbol
{
	public:
		inline ETheNumber(void):Symbol() {order=constant; catergory = CONSTANT;}
		double Execute(void) {return 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166;}		
};

class Pi:public Symbol
{
	public:
		inline Pi(void):Symbol() {order=constant; catergory = CONSTANT;}
		double Execute(void) {return 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117;}		
};

class XTheVariable:public Symbol
{
	public:
		inline XTheVariable(double* newX):Symbol() {x=newX; order=constant; catergory = CONSTANT;}
		double Execute(void) {return *x;}
	private:
		double* x;
};

class TTheVariable:public Symbol
{
	public:
		inline TTheVariable(double* newX):Symbol() {x=newX; order=constant; catergory = CONSTANT;}
		double Execute(void) {return *x;}
	private:
		double* x;
};

class ThetaTheVariable:public Symbol
{
	public:
		inline ThetaTheVariable(double* newX):Symbol() {x=newX; order=constant; catergory = CONSTANT;}
		double Execute(void) {return *x;}
	private:
		double* x;
};

class Add : public Symbol
{
	public:
		inline Add(void):Symbol(){order=addsub;catergory=INFIX;}
		inline virtual ~Add(void)
		{
			if (first !=NULL)
				delete first;
			if (last !=NULL)
				delete last;
			if (next != NULL)
				delete next;
			return;
		}
				
		double Execute(void)
		{
			double num=0;
			if ((first != NULL) && (last !=NULL))
				num=first->Execute() + last->Execute();
			if (next != NULL)
			{
				class Symbol* symbol = new Number(num);
				next->AddFirst( symbol );
				return next->Execute();
			}
			else
				return num;
		}
};

class Subtract : public Symbol
{
	public:
		inline Subtract(void):Symbol(){order=addsub;catergory=INFIX;}
		inline virtual ~Subtract(void)
		{
			if (first !=NULL)
				delete first;
			if (last !=NULL)
				delete last;
			if (next != NULL)
				delete next;
			return;
		}
		double Execute(void)
		{
			double num=0;
			if ((first != NULL) && (last !=NULL))
				num=first->Execute() - last->Execute();
			if (next != NULL)
			{
				class Symbol* symbol = new Number(num);
				next->AddFirst( symbol );
				return next->Execute();
			}
			else
				return num;
		}
};

class Multiply : public Symbol
{
	public:
		inline Multiply(void):Symbol(){order=multdiv;catergory=INFIX;}
		inline virtual ~Multiply(void)
		{
			if (first !=NULL)
				delete first;
			if (last !=NULL)
				delete last;
			if (next != NULL)
				delete next;
			return;
		}
		double Execute(void)
		{
			double num=0;
			if ((first != NULL) && (last != NULL))
				num = first->Execute() * last->Execute();
			if (next != NULL)
			{
				class Symbol * symbol = new Number(num);
				next->AddFirst( symbol );
				return next->Execute();
			}
			else
				return num;
		}
};

class Divide : public Symbol
{
	public:
		inline Divide(void):Symbol(){order=multdiv;catergory=INFIX;}
		inline virtual ~Divide(void)
		{
			if (first !=NULL)
				delete first;
			if (last !=NULL)
				delete last;
			if (next != NULL)
				delete next;
			return;
		}
		double Execute(void)
		{
			double num=0;
			if ((first != NULL) && (last !=NULL))
				num=first->Execute() / last->Execute();
			if (next != NULL)
			{
				class Symbol* symbol = new Number(num);
				next->AddFirst( symbol );
				return next->Execute();
			}
			else
				return num;
		}
};

class Exponent : public Symbol
{
	public:
		inline Exponent(void):Symbol(){order=exponent;catergory=INFIX;}
		inline virtual ~Exponent(void)
		{
			if (first !=NULL)
				delete first;
			if (last !=NULL)
				delete last;
			if (next != NULL)
				delete next;
			return;
		}
		double Execute(void)
		{
			double num=0;
			if ((first != NULL) && (last !=NULL))
				num=pow(first->Execute(), last->Execute());
			if (next != NULL)
			{
				class Symbol* symbol = new Number(num);
				next->AddFirst( symbol );
				return next->Execute();
			}
			else
				return num;
		}
};

class Paren : public SingleExpressionArg
{
	public:
		inline Paren(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Paren(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return first->Execute();
			
			return 0.0;
		}
};

class SquareRoot : public SingleExpressionArg
{
	public:
		inline SquareRoot(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~SquareRoot(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return sqrt(first->Execute());
			
			return 0.0;
		}
};


class Sine : public SingleExpressionArg
{
	public:
		inline Sine(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Sine(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return sin(first->Execute());
			
			return 0.0;
		}
};

class Cosine : public SingleExpressionArg
{
	public:
		inline Cosine(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Cosine(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return cos(first->Execute());
			
			return 0.0;
		}
};

class Tangent : public SingleExpressionArg
{
	public:
		inline Tangent(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Tangent(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return tan(first->Execute());
			
			return 0.0;
		}
};

class Arcsine : public SingleExpressionArg
{
	public:
		inline Arcsine(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Arcsine(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return asin(first->Execute());
			
			return 0.0;
		}
};

class Arccosine : public SingleExpressionArg
{
	public:
		inline Arccosine(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Arccosine(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return acos(first->Execute());
			
			return 0.0;
		}
};

class Arctangent : public SingleExpressionArg
{
	public:
		inline Arctangent(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Arctangent(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return atan(first->Execute());
			
			return 0.0;
		}
};

class NaturalLog : public SingleExpressionArg
{
	public:
		inline NaturalLog(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~NaturalLog(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return log(first->Execute());
			
			return 0.0;
		}
};

class Log : public SingleExpressionArg
{
	public:
		inline Log(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Log(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return log10(first->Execute());
			
			return 0.0;
		}
};

class SineH : public SingleExpressionArg
{
	public:
		inline SineH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~SineH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return sinh(first->Execute());
			
			return 0.0;
		}
};

class CosineH : public SingleExpressionArg
{
	public:
		inline CosineH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~CosineH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return cosh(first->Execute());
			
			return 0.0;
		}
};

class TangentH : public SingleExpressionArg
{
	public:
		inline TangentH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~TangentH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return tanh(first->Execute());
			
			return 0.0;
		}
};

class ArcsineH : public SingleExpressionArg
{
	public:
		inline ArcsineH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~ArcsineH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return asinh(first->Execute());
			
			return 0.0;
		}
};

class ArccosineH : public SingleExpressionArg
{
	public:
		inline ArccosineH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~ArccosineH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return acosh(first->Execute());
			
			return 0.0;
		}
};

class ArctangentH : public SingleExpressionArg
{
	public:
		inline ArctangentH(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~ArctangentH(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return atanh(first->Execute());
			
			return 0.0;
		}
};

class AbsoluteValue : public SingleExpressionArg
{
	public:
		inline AbsoluteValue(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~AbsoluteValue(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return fabs(first->Execute());
			
			return 0.0;
		}
};

class Erf : public SingleExpressionArg
{
	public:
		inline Erf(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Erf(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return erf(first->Execute());
			
			return 0.0;
		}
};

class Erfc : public SingleExpressionArg
{
	public:
		inline Erfc(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Erfc(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return erfc(first->Execute());
			
			return 0.0;
		}
};

class Gamma : public SingleExpressionArg
{
	public:
		inline Gamma(void):SingleExpressionArg(){order=function;catergory=SINGLE_ARGUMENT;}
		inline virtual ~Gamma(void)
		{
			if (first !=NULL)
				delete first;
			return;
		}
		double Execute(void)
		{
			if (first != NULL)
				return gamma(first->Execute());
			
			return 0.0;
		}
};

class BesselOne : public Symbol
{
	public:
		inline BesselOne(void):Symbol(){order=function;catergory=BESSEL;}
		inline virtual ~BesselOne(void)
		{
			if (first != NULL)
				delete first;
			return;
		}
		inline virtual void AddExpressionArg(class Symbol* argSymbol, int argNumber)
		{
			if (argNumber==2)
				first=argSymbol;
		}
		inline virtual void AddIntArg(int newInteger, int argNumber)
		{
			if (argNumber == 1)
				integer=newInteger;
		}
		inline virtual double Execute(void)
		{
			if (first!=NULL)
				return jn(integer, first->Execute());
			return 0.0;
		}
	private:
		int integer;
};

class BesselTwo : public Symbol
{
	public:
		inline BesselTwo(void):Symbol(){order=function;catergory=BESSEL;}
		inline virtual ~BesselTwo(void)
		{
			if (first != NULL)
				delete first;
			return;
		}
		inline virtual void AddExpressionArg(class Symbol* argSymbol, int argNumber)
		{
			if (argNumber==2)
				first=argSymbol;
		}
		inline virtual void AddIntArg(int newInteger, int argNumber)
		{
			if (argNumber == 1)
				integer=newInteger;
		}
		inline virtual double Execute(void)
		{
			if (first!=NULL)
				return yn(integer, first->Execute());
			return 0.0;
		}
	private:
		int integer;
};


#endif