#ifndef ATTRIBUTE_H 
#define ATTRIBUTE_H
#include<iostream>
using namespace std;

class Attribute
{
public:
    int static const TYPE_FLOAT = -1;
    int static const TYPE_INT = 0;
    string name;
    int type, size;              //-1为float，0为int，其余整数对应varchar(type)    
    bool ifUnique;          //1为unique, 0不unique
    string index;

    Attribute(string n, int t, bool i, string in = "") {
        name = n;
        type = t;
        ifUnique = i;
        index = in;
    }
    string getName() {
        return name;
    }
    int getType() {
        return type;
    }
    bool getUnique() {
        return ifUnique;
    }
    string getIndex() {
        return index;
    }
    void print()
    {
        cout << "name:" << name << ";type: " << type << ";ifUnique: " << ifUnique << endl;
    }
    ~Attribute() {};
};
#endif
