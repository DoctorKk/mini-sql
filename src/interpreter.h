//  Version 2.1
//  Interpreter.h 
//  MiniSQL
//  Created by ly on 27/6/2020.

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include "API.h"
#include "BufferManager.h"
using namespace std;
class SyntaxException
{
public:
    SyntaxException() {};
    ~SyntaxException() {};
    const char *ShowReason() const { return "SYNTAX ERROR!"; }
};
class Interpreter{
public:
    API* ap;
    string fileName;
    void  mainFunction();
    int interpreter(string s);	
    int EXEC_CREATE(string s,int *tmp,string word);
    //Create Table
    int EXEC_CREATE_TABLE(string s,int *tmp,string word);
    //Create Index
    int EXEC_CREATE_INDEX(string s,int *tmp,string word);
    //Drop 
    int EXEC_DROP(string s,int *tmp,string word);
    //Drop Table
    int EXEC_DROP_TABLE(string s,int *tmp,string word); 
    //Drop Index
    int EXEC_DROP_INDEX(string s,int *tmp,string word);
    //Select
    int EXEC_SELECT(string s,int *tmp,string word);
    //Insert
    int EXEC_INSERT(string s,int *tmp,string word);
    //Quit
    int EXEC_QUIT(string s,int *tmp,string word);
    //Execfile 
    int EXEC_EXECFILE(string s, int *tmp, string word);
    int EXEC_DELETE(string s, int *tmp, string word);
    string getWord(string s, int *tmp);
	
	Interpreter(){
	    ap = new API;
	}
	~Interpreter(){
	    delete ap;
	}
};

#endif
