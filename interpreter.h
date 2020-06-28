//  Version 2.1
//  Interpreter.h 
//  MiniSQL
//  Created by ly on 27/6/2020.

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include "API.h"
using namespace std;
class Interpreter{
public:
    API* ap;
	string fileName ;
	int interpreter(string s);	
    int EXEC_CREATE(string s,int *tmp,string word)
    int EXEC_CREATE_TABLE(string s,int *tmp,string word)
    int EXEC_CREATE_INDEX(string s,int *tmp,string word)
    int EXEC_SELECT(string s,int *tmp,string word)
    int EXEC_INSERT(string s,int *tmp,string word)
	string getWord(string s, int *st);
	
	Interpreter(){}
	~Interpreter(){}
};

#endif
