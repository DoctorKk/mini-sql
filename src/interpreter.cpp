//  Version 2.1
//  Interpreter.h 
//  MiniSQL
//  Created by ly on 27/6/2020.

#include "interpreter.h"
#include "Condition.h"
#include "Attribute.h"
#include <string>
#include <string.h>
#include <iostream>
#include<fstream>
using namespace std;

//get requests in;
void Interpreter::mainFunction()
{
	cout << "WELCOME TO OUR MINISQL! ENJOY YOURSELF!" << endl;
	string text = "";
	string temp = "";
	string const_empty = " ";
	int fact = 1;
	while (fact != 587) {
		cout << "PLEASE ENTER YOUR SETENCE!WILL LOOP BEFORE YOU QUIT" << endl;
		text = " ";
		do{
			getline(cin, temp);
			text = text + const_empty;
			text = text + temp;
		}while (text[text.size() - 1] != ';');

		fact = interpreter(text);
	}
	cout << "THANK YOU!" << endl;
}
//Interpreter
int Interpreter::interpreter(string s)
{
	int shit = 0;
	int *tmp = &shit;
	string word = getWord(s, tmp);
	//create
	if (strcmp(word.c_str(), "create") == 0)
		return EXEC_CREATE(s, tmp, word);
	//drop
	else if (strcmp(word.c_str(), "drop") == 0)
		return EXEC_DROP(s, tmp, word);
	//select
	else if (strcmp(word.c_str(), "select") == 0)
	    return EXEC_SELECT(s,tmp,word);
//insert
	else if (strcmp(word.c_str(), "insert") == 0)
	return EXEC_INSERT(s,tmp,word);
//quit
	else if (strcmp(word.c_str(), "quit") == 0)
		return EXEC_QUIT(s, tmp, word);
	//execfile
	else if (strcmp(word.c_str(), "execfile") == 0)
	    return EXEC_EXECFILE(s, tmp, word);
    else if (strcmp(word.c_str(), "delete") == 0)
        return EXEC_DELETE(s, tmp, word);
//illegal command
	else
	{
		if (word != "")
			cout << "Error, command " << word << " not found" << endl;
		return 0;
	}
	return 0;
}


//int main()
//{
//	string text;
//  scanf("%s",text);
//	interpreter(text);
//}

//Create
int Interpreter::EXEC_CREATE(string s, int *tmp, string word)
{
	word = getWord(s, tmp);
	if (strcmp(word.c_str(), "table") == 0)
		return EXEC_CREATE_TABLE(s, tmp, word);
	else if (strcmp(word.c_str(), "index") == 0)
		return EXEC_CREATE_INDEX(s, tmp, word);
	else
		cout << "Syntax Error for " << word << endl;
	return 0;
}
//Create table
int Interpreter::EXEC_CREATE_TABLE(string s, int *tmp, string word)
{
	string primaryKey = "";
	string tableName = "";
	word = getWord(s, tmp);
	//无名则退回 
	if (!word.empty())
		tableName = word;
	else
	{
		cout << "Syntax Error for no table name" << endl;
		return 0;
	}
	word = getWord(s, tmp);
	if (word.empty() || strcmp(word.c_str(), "(") != 0)
	{
		cout << "Error in syntax!" << endl;
		return 0;
	}
	else				// deal with attribute list
	{
		word = getWord(s, tmp);
		std::vector<Attribute> attributeVector;
		while (!word.empty() && strcmp(word.c_str(), "primary") != 0 && strcmp(word.c_str(), ")") != 0)
		{
			string attributeName = word;
			int type = 0;
			bool ifUnique = false;
			// deal with the data type
			word = getWord(s, tmp);
			if (strcmp(word.c_str(), "int") == 0)
				type = 0;
			else if (strcmp(word.c_str(), "float") == 0)
				type = -1;
			else if (strcmp(word.c_str(), "char") == 0)
			{
				word = getWord(s, tmp);
				if (strcmp(word.c_str(), "("))
				{
					cout << "Syntax Error: unknown data type" << endl;
					return 0;
				}
				word = getWord(s, tmp);
				istringstream convert(word);
				if (!(convert >> type))
				{
					cout << "Syntax error : illegal number in char()" << endl;
					return 0;
					//cout<<"char type:"<<type<<endl; 
				}
				word = getWord(s, tmp);
				if (strcmp(word.c_str(), ")"))
				{
					cout << "Syntax Error: unknown data type: "<<word << endl;
					return 0;
				}
			}
			else
			{
				cout << "Syntax Error: unknown or missing data type!" << endl;
				return 0;
			}
			word = getWord(s, tmp);
			if (strcmp(word.c_str(), "unique") == 0)
			{
				ifUnique = true;
				word = getWord(s, tmp);
			}
			Attribute attr(attributeName, type, ifUnique);
			attributeVector.push_back(attr);
			if (strcmp(word.c_str(), ",") != 0)
			{
				if (strcmp(word.c_str(), ")") != 0)
				{
					cout << "Syntax Error for ,!" << endl;
					return 0;
				}
				else break;
			}

			word = getWord(s, tmp);
		}
		int primaryKeyLocation = 0;
		if (strcmp(word.c_str(), "primary") == 0)	// deal with primary key
		{
			word = getWord(s, tmp);
			if (strcmp(word.c_str(), "key") != 0)
			{
				cout << "Error in syntax!" << endl;
				return 0;
			}
			else
			{
				word = getWord(s, tmp);
				if (strcmp(word.c_str(), "(") == 0)
				{
					word = getWord(s, tmp);
					primaryKey = word;
					int i = 0;
					for (i = 0; i < attributeVector.size(); i++)
					{
						if (primaryKey == attributeVector[i].name)
						{
							attributeVector[i].ifUnique = true;
							break;
						}
					}
					if (i == attributeVector.size())
					{
						cout << "Syntax Error: primaryKey does not exist in attributes " << endl;
						return 0;
					}
					primaryKeyLocation = i;
					word = getWord(s, tmp);
					if (strcmp(word.c_str(), ")") != 0)
					{
						cout << "Error in syntax!" << endl;
						return 0;
					}
				}
				else
				{
					cout << "Error in syntax!" << endl;
					return 0;
				}
				word = getWord(s, tmp);
				if (strcmp(word.c_str(), ")") != 0)
				{
					cout << "Error in syntax!" << endl;
					return 0;
				}
			}
		}
		else if (word.empty())
		{
			cout << "Syntax Error: ')' absent!" << endl;
			return 0;
		}

		ap->tableCreate(tableName, &attributeVector, primaryKey, primaryKeyLocation);
		return 1;
	}
}
//Create index
int Interpreter::EXEC_CREATE_INDEX(string s, int *tmp, string word)
{
	string indexName = "";
	string tableName = "";
	string attributeName = "";
	word = getWord(s, tmp);
	if (!word.empty())			//create index indexname
		indexName = word;
	else
	{
		cout << "Error in syntax!" << endl;
		return 0;
	}
	word = getWord(s, tmp);
	try {
		if (strcmp(word.c_str(), "on") != 0)
			throw SyntaxException();
		word = getWord(s, tmp);
		if (word.empty())
			throw SyntaxException();
		tableName = word;
		word = getWord(s, tmp);
		if (strcmp(word.c_str(), "(") != 0)
			throw SyntaxException();
		word = getWord(s, tmp);
		if (word.empty())
			throw SyntaxException();
		attributeName = word;
		word = getWord(s, tmp);
		if (strcmp(word.c_str(), ")") != 0)
			throw SyntaxException();
		ap->indexCreate(indexName, tableName, attributeName);
		return 1;
	}
	catch (SyntaxException&) {
		cout << "Syntax Error!" << endl;
		return 0;
	}
}
//Drop
int Interpreter::EXEC_DROP(string s, int *tmp, string word)
{
	word = getWord(s, tmp);
	if (strcmp(word.c_str(), "table") == 0)
		return EXEC_DROP_TABLE(s, tmp, word);
	else if (strcmp(word.c_str(), "index") == 0)
		return EXEC_DROP_INDEX(s, tmp, word);
	else
		cout << "ERROR! CAN ONLY DROP TABLE/INDEX!" << endl;
	return 0;
}
//Drop table
int Interpreter::EXEC_DROP_TABLE(string s, int *tmp, string word)
{
	string tableName = "";
	word = getWord(s, tmp);
	if (!word.empty())
	{
		tableName = word;
		ap->tableDrop(tableName);
	}
	else
	{
		cout << "ERROR!CAN NOT DROP TABLE WITHOUT NAME!" << endl;
		return 0;
	}

	return 1;
}
//Drop Index
int Interpreter::EXEC_DROP_INDEX(string s, int *tmp, string word)
{
	string indexName = "";
	word = getWord(s, tmp);
	if (!word.empty())
	{
		indexName = word;
		ap->indexDrop(indexName);
	}
	else
	{
		cout << "ERROR!! CAN NOT DROP INDEX WITHOUT NAME!" << endl;
		return 0;
	}
	return 1;
}

int Interpreter::EXEC_DELETE(string s, int *tmp, string word) {
    vector<string> attrSelected;
    string tableName = "";
    word = getWord(s, tmp);
    if (strcmp(word.c_str(), "*") != 0)	// only accept select *
    {
        while (strcmp(word.c_str(), "from") != 0)
        {
            attrSelected.push_back(word);
            word = getWord(s, tmp);
        }
    }
    else
    {
        word = getWord(s, tmp);
    }
    if (strcmp(word.c_str(), "from") != 0)
    {
        cout << "Error in syntax!" << endl;
        return 0;
    }
    word = getWord(s, tmp);
    if (!word.empty())
        tableName = word;
    else
    {
        cout << "Error in syntax!" << endl;
        return 0;
    }
    // condition extricate
    word = getWord(s, tmp);
    if (word.empty())	// without condition
    {
        if (attrSelected.size() == 0) {
            ap->recordShow(tableName, nullptr, nullptr);
        }
        else
            ap->recordShow(tableName, &attrSelected, nullptr);
        return 1;
    }
    else if (strcmp(word.c_str(), "where") == 0)
    {
        string attributeName = "";
        string value = "";
        int operate = Condition::OPERATOR_EQUAL;
        std::vector<Condition> conditionVector;
        conditionVector.reserve(10);
        word = getWord(s, tmp);		//col1
        while (1) {
            try {
                if (word.empty())
                    throw SyntaxException();
                attributeName = word;
                word = getWord(s, tmp);
                if (strcmp(word.c_str(), "<=") == 0)
                    operate = Condition::OPERATOR_LESS_EQUAL;
                else if (strcmp(word.c_str(), ">=") == 0)
                    operate = Condition::OPERATOR_MORE_EQUAL;
                else if (strcmp(word.c_str(), "<") == 0)
                    operate = Condition::OPERATOR_LESS;
                else if (strcmp(word.c_str(), ">") == 0)
                    operate = Condition::OPERATOR_MORE;
                else if (strcmp(word.c_str(), "=") == 0)
                    operate = Condition::OPERATOR_EQUAL;
                else if (strcmp(word.c_str(), "<>") == 0)
                    operate = Condition::OPERATOR_NOT_EQUAL;
                else
                    throw SyntaxException();
                word = getWord(s, tmp);
                if (word.empty()) // no condition
                    throw SyntaxException();
                value = word;
                Condition c(attributeName, value, operate);
                conditionVector.push_back(c);
                word = getWord(s, tmp);
                if (word.empty()) // no condition
                    break;
                if (strcmp(word.c_str(), "and") != 0)
                    throw SyntaxException();
                word = getWord(s, tmp);
            }
            catch (SyntaxException&) {
                cout << "Syntax Error!" << endl;
                return 0;
            }
        }
        if (attrSelected.size() == 0) {
            cout << tableName << endl;
            ap->recordDelete(tableName,  &conditionVector);

        }
        else {
            cout << tableName << endl;
            ap->recordDelete(tableName,  &conditionVector);
        }
        return 1;
    }

}
//select
int  Interpreter::EXEC_SELECT(string s, int *tmp, string word)
{
	vector<string> attrSelected;
	string tableName = "";
	word = getWord(s, tmp);
	if (strcmp(word.c_str(), "*") != 0)	// only accept select *
	{
		while (strcmp(word.c_str(), "from") != 0)
		{
			attrSelected.push_back(word);
			word = getWord(s, tmp);
		}
	}
	else
	{
		word = getWord(s, tmp);
	}
	if (strcmp(word.c_str(), "from") != 0)
	{
		cout << "Error in syntax!" << endl;
		return 0;
	}
	word = getWord(s, tmp);
	if (!word.empty())
		tableName = word;
	else
	{
		cout << "Error in syntax!" << endl;
		return 0;
	}
	// condition extricate
	word = getWord(s, tmp);
	if (word.empty())	// without condition
	{
		if (attrSelected.size() == 0) {
			ap->recordShow(tableName, nullptr, nullptr);
		}
		else
			ap->recordShow(tableName, &attrSelected, nullptr);
		return 1;
	}
	else if (strcmp(word.c_str(), "where") == 0)
	{
		string attributeName = "";
		string value = "";
		int operate = Condition::OPERATOR_EQUAL;
		std::vector<Condition> conditionVector;
		conditionVector.reserve(10);
		word = getWord(s, tmp);		//col1
		while (1) {
			try {
				if (word.empty())
					throw SyntaxException();
				attributeName = word;
				word = getWord(s, tmp);
				if (strcmp(word.c_str(), "<=") == 0)
					operate = Condition::OPERATOR_LESS_EQUAL;
				else if (strcmp(word.c_str(), ">=") == 0)
					operate = Condition::OPERATOR_MORE_EQUAL;
				else if (strcmp(word.c_str(), "<") == 0)
					operate = Condition::OPERATOR_LESS;
				else if (strcmp(word.c_str(), ">") == 0)
					operate = Condition::OPERATOR_MORE;
				else if (strcmp(word.c_str(), "=") == 0)
					operate = Condition::OPERATOR_EQUAL;
				else if (strcmp(word.c_str(), "<>") == 0)
					operate = Condition::OPERATOR_NOT_EQUAL;
				else
					throw SyntaxException();
				word = getWord(s, tmp);
				if (word.empty()) // no condition
					throw SyntaxException();
				value = word;
				Condition c(attributeName, value, operate);
				conditionVector.push_back(c);
				word = getWord(s, tmp);
				if (word.empty()) // no condition
					break;
				if (strcmp(word.c_str(), "and") != 0)
					throw SyntaxException();
				word = getWord(s, tmp);
			}
			catch (SyntaxException&) {
				cout << "Syntax Error!" << endl;
				return 0;
			}
		}
		if (attrSelected.size() == 0) {
			cout << tableName << endl;
			ap->recordShow(tableName, nullptr, &conditionVector);

		}
		else {
			cout << tableName << endl;
			ap->recordShow(tableName, &attrSelected, &conditionVector);
		}
		return 1;
	}
}
//insert
int Interpreter::EXEC_INSERT(string s, int *tmp, string word)
{
	string tableName = "";
	std::vector<string> valueVector;
	word = getWord(s, tmp);
	try {
		if (strcmp(word.c_str(), "into") != 0)
			throw SyntaxException();
		word = getWord(s, tmp);
		if (word.empty())
			throw SyntaxException();
		tableName = word;
		word = getWord(s, tmp);
		if (strcmp(word.c_str(), "values") != 0)
			throw SyntaxException();
		word = getWord(s, tmp);
		if (strcmp(word.c_str(), "(") != 0)
			throw SyntaxException();
		word = getWord(s, tmp);
		while (!word.empty() && strcmp(word.c_str(), ")") != 0)
		{
			valueVector.push_back(word);
			valueVector.push_back("/"); // changed here >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			word = getWord(s,tmp);
			if (strcmp(word.c_str(),",") == 0)  // bug here
    			word = getWord(s,tmp);
		}
		if (strcmp(word.c_str(), ")") != 0)
			throw SyntaxException();
	}
	catch (SyntaxException&) {
		cout << "Syntax Error!" << endl;
		return 0;
	}
	ap->recordInsert(tableName, valueVector);
	return 1;
}
//Quit
int Interpreter::EXEC_QUIT(string s, int *tmp, string word)
{
	return 587;
}
//Execfile 
int Interpreter::EXEC_EXECFILE(string s, int *tmp, string word)
{
    string Path = "../exec/";
    word = getWord(s, tmp);
    Path += word;
    cout << "try to open " << word << "..." << endl;
    ifstream in(Path);
    if (!in) {
        cerr << "executable file not found" << endl;
        return 2;
    }

    string text = "";
    string temp = "";
    string const_empty = " ";
    int fact = 1;
    while (fact != 587 && !in.eof()) {
        text = "";
        do {
            getline(in, temp, '\r');
            if (in.eof())
                break;
            //temp.erase(temp.size()-1);
            text = text + const_empty;
            text = text + temp;
        } while (text[text.size() - 1] != ';');
        fact = interpreter(text);
    }
    return 2;


}

string Interpreter::getWord(string s, int *tmp)
{
	string word;
	int idx1, idx2;
	while ((s[*tmp] == ' '|| s[*tmp] == '\'' || s[*tmp] == 10 || s[*tmp] == '\t') && s[*tmp] != 0 && s[*tmp] != ';')
	{
		(*tmp)++;
	}
	idx1 = *tmp;
	if (s[*tmp] == ';') 
	{
		(*tmp)++;
		word = "";
		return word;
	}
	else if (s[*tmp] == '(' || s[*tmp] == ',' || s[*tmp] == ')')
	{
		(*tmp)++;
		idx2 = *tmp;
		word = s.substr(idx1, idx2 - idx1);
		return word;
	}
	//处理单引号 '（ASCII 39）
	else if (s[*tmp] == 39)
	{
		(*tmp)++;
		while (s[*tmp] != 39 && s[*tmp] != 0)
			(*tmp)++;
		if (s[*tmp] == 39)
		{
			idx1++;
			idx2 = *tmp;
			(*tmp)++;
			word = s.substr(idx1, idx2 - idx1);
			return word;
		}
		else
		{
			word = "";
			return word;
		}
	}
	else
	{
		while (s[*tmp] != ' '&&s[*tmp] != '\''&&s[*tmp] != '(' && s[*tmp] != 10 && s[*tmp] != 0 && s[*tmp] != ')' && s[*tmp] != ',' && s[*tmp]!=';')
			(*tmp)++;
		idx2 = *tmp;
		if (idx1 != idx2)
			word = s.substr(idx1, idx2 - idx1);
		else
			word = "";

		return word;
	}
}
