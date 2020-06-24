/*
 * @Author: 小文
 * @Date: 2020-06-24 16:13:35
 * @LastEditTime: 2020-06-24 17:42:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \新建文件夹\Attribute.h
 */ 

#include <string>
#include <iostream>
using namespace std;

class Attribute
{
public:
    string name;
    int type;              //-1为float，0为int，其余整数对应varchar(type)    
    bool ifUnique;          //是否唯一
    string index;    
         
    Attribute(string n, int t, bool i){
        name = n;
        type = t; 
        ifUnique = i;
        index = "";
    }
    ~Attribute(){};
};

