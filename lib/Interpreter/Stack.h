//
//  Stack.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Stack__
#define __Interpreter__Stack__

#include "lexems.h"

template <int max_size>
class Stack {
	T s[max_size];
	int top;
public:
	Stack() { top = 0; }
	void reset() { top = 0; }
	void push(int);
	void push(double);
	void push(const char *);
	void push(bool);
	T pop();
	bool is_empty() { return top == 0; }
	bool is_full() { return top == max_size; }
};

template <int max_size>
void Stack <max_size> :: push(int i)
{
	if (!is_full()) {
		s[top].tl = LEX_INT;
		s[top++].i = i;
	} else
		throw "Stack_is_full";
}

template <int max_size>
void Stack <max_size> :: push(bool i)
{
	if (!is_full()) {
		s[top].tl = LEX_BOOL;
		s[top++].i = (int) i;
	} else
		throw "Stack_is_full";
}

template <int max_size>
void Stack <max_size> :: push(double d)
{
	if (!is_full()) {
		s[top].tl = LEX_DOUBLE;
		s[top++].d = d;
	} else
		throw "Stack_is_full";
}

template <int max_size>
void Stack <max_size> :: push(const char *_s)
{
	if(!is_full()) {
		s[top].tl = LEX_STR;
		s[top].s = new char [strlen(_s)+1];
		strcpy(s[top++].s, _s);
	} else
		throw "Stack_is_full";
}

template <int max_size>
T Stack <max_size> :: pop()
{
	if (!is_empty())
		return s[--top];
	else
		throw "stack_is_empty";
}

#endif /* defined(__Interpreter__Stack__) */
