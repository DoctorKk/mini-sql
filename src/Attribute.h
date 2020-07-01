/*
 * @Author: 小文
 * @Date: 2020-06-24 16:13:35
 * @LastEditTime: 2020-06-24 17:42:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \新建文件夹\Attribute.h
 */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <string>
#include <iostream>
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
         
    Attribute(string n, int t, bool i){
        name = n;
        type = t; 
        ifUnique = i;
        index = "";
    }
    ~Attribute(){};
};

#endif
