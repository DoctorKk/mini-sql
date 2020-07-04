/*
 * @Author: Holly
 * @Version:2.0
 * @Date: 2020-06-25
 * @LastEditTime: 2020-06-29
 * @LastEditors: Please set LastEditors
 * @Description: verison 2.0
 */

#include "CatalogManager.h"
#define SIZE 1024
#define BUF_SIZE 256

Catalog::Catalog() {}
Catalog:: ~Catalog() {}

/* table operations */
/* 1.find table, input: table name */
/* if table exist, return 1*/
/* else return -1*/
int Catalog::findTable(string tableName) {
    FILE* fp;
    string post = ".txt";
    tableName += post;
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
    {
        return -1;
    }
    else
    {
        fclose(fp);
        return 1;
    }
}

/* 2.drop table,input: table name */
/* if table doesn't exist, return -2 */
/* else remove it and return 2 */
int Catalog::dropTable(string tableName) {
    string post = ".txt";
    tableName += post;
    /* remove() return 0, success */
    /* return 1,fail to remove the file */
    if (remove(tableName.c_str()))
    {
        return -2;
    }
    return 2;
}

/* 3.create table, input: table name, attributes and primary key */
/* if table already exists, return -3 */
/* else, create a new file, write the information and return 3 */
int Catalog::createTable(string tableName, vector<Attribute>* attributeVector, string primaryKeyName ) {
    Catalog tem;
    int find = tem.findTable(tableName);
    if (find == 1) {
        return -3;
    }
    /* create file */
    FILE* fp;
    string post = ".txt";
    tableName += post;
    fp = fopen(tableName.c_str(), "w");
    if (fp == NULL)
    {
        return -3;
    }
    else
    {
        /* attribute number */
        int size = (*attributeVector).size();
        fprintf(fp, "%d|", size);

        /* record number */
        int record = 0;
        fprintf(fp, "%d|", record);

        //cout << "check " << primaryKeyName << endl;

        /* primary key */
        if (primaryKeyName != "")
            fprintf(fp, "%s||\n", primaryKeyName.c_str());
        else
            fprintf(fp, "||\n");

        for (int i = 0; i < (*attributeVector).size(); i++) {
            string name = (*attributeVector)[i].getName();
            int type = (*attributeVector)[i].getType();
            bool unique = (*attributeVector)[i].getUnique();
            string index = (*attributeVector)[i].getIndex();
            //cout << "hhh" << name << " " << type << " " << unique << " " << index << endl;
            fprintf(fp, "%s|%d|%d|%s||\n", name.c_str(), type, unique, index.c_str());
        }
        fclose(fp);
        return 3;
    }
}

/* record operation */
/* 1.get record number of a table, input: table name */
/* if table doesn't exist, return -1 */
/* else return record number (>=0) */
int Catalog::getRecordNum(string tableName) {
    Catalog tem;
    int find = tem.findTable(tableName);
    if (find == -1) {
        return -1;
    }
    string post = ".txt";
    tableName += post;
    FILE* fp;
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;
    int a_n, r_n;
    fscanf(fp, "%d|%d", &a_n, &r_n);
    fclose(fp);
    return r_n;
}

/* 2.increase record number,input: table name and increment */
/* if table doesn't exist, return -1 */
/* else, rewrite the file and return 4 */
int Catalog::insertRecordNum(string tableName, int increment_num) {  // increment the number of record
  
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    int attribute_num, record_num;
    fscanf(fp, "%d|%d", &attribute_num, &record_num);

    char text[SIZE] = { 0 };
    char ch;
    
    int text_size = 0;
    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  
            break;
        text[text_size++] = ch;
    }
    fclose(fp);

    /* rewrite the file */
    fp = fopen(tableName.c_str(), "w");
    record_num += increment_num;
    fprintf(fp, "%d|%d", attribute_num, record_num);

    int i = 0;
    while (i < text_size) {
        fprintf(fp, "%c", text[i++]);
    }
    fclose(fp);
    return 4;
}

/* 3.derease record number, input: table name,decrement number */
/* if table doesn't exist, return -1 */
/* else,rewrite the file and return 5 */
int Catalog::deleteRecordNum(string tableName, int decrement_num) {// delete the number of record
 
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    int attribute_num, record_num;
    fscanf(fp, "%d|%d", &attribute_num, &record_num);

    char text[SIZE] = { 0 }, ch;
    
    int text_size = 0;
    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  break;
        text[text_size++] = ch;
    }
    fclose(fp);

    /* rewrite the file */
    fp = fopen(tableName.c_str(), "w");
    record_num -= decrement_num;
    fprintf(fp, "%d|%d", attribute_num, decrement_num);
    int i = 0;
    while (i < text_size) {
        fprintf(fp, "%c", text[i++]);
    }
    fclose(fp);
    return 5;
}

/* attribute operations */
/* 1.get all attributes of table,input: table name, attribute vector */
/* if table doesn't exist, return -1 */
/* else obtain the attributes and return 12*/
int Catalog::attributeGet(string tableName, vector<Attribute>* attributeVector) {
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char tem_name[BUF_SIZE] = { 0 };
    char tem_type[BUF_SIZE] = { 0 };
    char tem_unique;
    char tem_index[BUF_SIZE] = { 0 };

    int len_buff, len_name, len_type, len_index;

    int attribute_num;	//attribute number
    fscanf(fp, "%d", &attribute_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {

        for (int k = 0; k < attribute_num; k++) {
            /* fgets() read line by line */
            fgets(buff, BUF_SIZE, fp);
            len_buff = strlen(buff);

            const char delim[2] = "|";
            strcpy(tem_name, buff);
            strtok(tem_name, delim);
            len_name = strlen(tem_name);

            strncpy(tem_type, buff + len_name + 1, len_buff - len_name - 1);
            strtok(tem_type, delim);
            len_type = strlen(tem_type);

            tem_unique = buff[len_name + len_type + 2];

            if (buff[len_name + len_type + 4] == '|')
                strcpy(tem_index, "");
            else {
                strncpy(tem_index, buff + len_name + len_type + 4, len_buff - len_name - len_type - 4);
                strtok(tem_index, delim);
            }

            /* data type transfer */
            int type = char2int(tem_type, len_type);
            int unique = (int)tem_unique - '0';

            Attribute tem(tem_name, type, unique, tem_index);
            (*attributeVector).push_back(tem);
        }
    }

    fclose(fp);
    return 12;
}

/* index operation */
/* 1.find index, input: table name */
/* if index exist, return table name */
/* else return "" */
string Catalog::findIndex(string indexName) {
    int flag = 0;
    FILE* fp;
    fp = fopen("Index.txt", "r");
    if (fp == NULL)
        return 0;

    int len_index,len_buff;
    char buff[BUF_SIZE] = { 0 };
    char tem_index[BUF_SIZE] = { 0 };
    char tem_table[BUF_SIZE] = { 0 };
    string table_name;

    while (!feof(fp) && !ferror(fp)) {
        
        fgets(buff, sizeof(buff), fp);
        strcpy(tem_index, buff);
        len_buff = strlen(buff);

        const char delim[2] = "|";
        strtok(tem_index, delim);

        if (!strcmp(indexName.c_str(), tem_index) ) {
            
            len_index = strlen(tem_index);
            strncpy(tem_table, buff + len_index + 1, len_buff - len_index);
            strtok(tem_table, delim);

            flag = 1;
            break;
        }
    }
    fclose(fp);

    
    if (flag) {
        string str_table(tem_table);
        return str_table;
    }
    else
        return "";
}

/* 2.add index, input: index name,table name,attribute name,attribute type */
/* if index name already exist on the same table, return -7 */
/* else if index exist but on the different table,it's ok */ 
/* note that it only revise the "Index" file */
int Catalog::addIndex(string indexName, string tableName, string attributeName, int type) {
    Catalog tem;
    string check_table = tem.findIndex(indexName);
    if (check_table == tableName )
        return -7;

    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    int text_size = 0;
    char text[SIZE] = { 0 };
    char ch;

    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  break;
        text[text_size++] = ch;
    }
    fclose(fp);

    /* index data type transfer */
    string index;
    if (type >= 0)
        index = indexName + "|" + tableName + "|" + attributeName + "|" + (char)(type + '0') + "||\n";
    else
        index = indexName + "|" + tableName + "|" + attributeName + "|-" + (char)(-type + '0') + "||\n";

    strcat(text, index.c_str());

    /* rewrite the file */
    fp = fopen(table.c_str(), "w");

    int i = 0;
    while (i < strlen(text)) {
        fprintf(fp, "%c", text[i++]);
    }
    fclose(fp);
    return 7;
}

/* 3. drop index, input: index name */
/* delete the index line and return 8 */
/* note that it only revise the "Index" file */
int Catalog::dropIndex(string index) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char text[SIZE] = { 0 };
    int len_line, len_index = strlen(index.c_str());

    while (fgets(buff, SIZE, fp) != NULL)
    {
        len_line = strlen(buff);
        if (strncmp(buff, index.c_str(), len_index) || buff[len_index]!='|')
            strcat(text, buff);
    }

    fclose(fp);

    /* rewrite the file */
    fp = fopen(table.c_str(), "w");

    int i = 0;
    int len_text = strlen(text);
    while (i < len_text) {
        fprintf(fp, "%c", text[i++]);
    }
    fclose(fp);

    return 8;
}

/* 4.set index on attribute,input:table name, attribute name,index name */
/* note that,it doesn't check whether there already is an index */
/* set index and return 11 */
int Catalog::setIndexOnAttribute(string tableName, string AttributeName, string indexName) {
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char text[SIZE] = { 0 };
    int len_buff, len_attribute = strlen(AttributeName.c_str());

    int attribute_num;	
    fscanf(fp, "%d", &attribute_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {
        strcat(text, buff);

        for (int k = 0; k < attribute_num; k++) {
            fgets(buff, BUF_SIZE, fp);
            len_buff = strlen(buff);
     
            if (strncmp(buff, AttributeName.c_str(), len_attribute) || buff[len_attribute] != '|')	//find attribute
                strcat(text, buff);
            else {
                char tem[BUF_SIZE] = { 0 };
                int find = 4;
                while (buff[len_buff - find] != '|') { find++; }
                strncpy(tem, buff, len_buff - find + 1);
                strcat(tem, indexName.c_str());
                char tail[4] = "||\n";
                strcat(tem, tail);
                cout << tem;
                strcat(text, tem);
            }
        }
    }

    fclose(fp);

    /* rewrite the file */
    fp = fopen(tableName.c_str(), "w");

    int j = 0;
    fprintf(fp, "%d", attribute_num);
    while (j < strlen(text)) {
        fprintf(fp, "%c", text[j++]);
    }
    fclose(fp);
    return 11;
}

/* 5.delete index on attribute */
/* note that,it doesn't check whether there already is an index */
/* set index and return 9 */
int Catalog::revokeIndexOnAttribute(string tableName, string AttributeName, string indexName) {
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char text[SIZE] = { 0 };
    int len_buff;
    int len_attribute = strlen(AttributeName.c_str());
    int len_index = strlen(indexName.c_str());
    int attribute_num;	//attribute number
    fscanf(fp, "%d", &attribute_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {
        strcat(text, buff);
        for (int k = 0; k < attribute_num; k++) {
            fgets(buff, BUF_SIZE, fp);
            len_buff = strlen(buff);
           
            if (strncmp(buff, AttributeName.c_str(), len_attribute) || buff[len_attribute] != '|')	//find attribute
                strcat(text, buff);
            else {
                char tem[BUF_SIZE] = { 0 };
                
                int new_len = len_buff - len_index - 3;
                strncpy(tem, buff, new_len);
                char tail[4] = "||\n";
                strcat(tem, tail);
                strcat(text, tem);
            }
        }
    }

    fclose(fp);

    /* rewrite the file */
    fp = fopen(tableName.c_str(), "w");

    int j = 0,len_text = strlen(text);

    fprintf(fp, "%d", attribute_num);
    while (j < len_text) {
        fprintf(fp, "%c", text[j++]);
    }
    fclose(fp);
    return 9;
}

/* 6.obtain all indexes on a table,input: table name, index vector pointer */
/* return 10*/
int Catalog::indexNameListGet(string tableName, vector<string>* indexNameVector) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char tem_index[BUF_SIZE] = { 0 };
    char tem_table[BUF_SIZE] = { 0 };
    char text[SIZE] = { 0 };
    int len_buff, len_index;

    while (fgets(buff, BUF_SIZE, fp) != NULL)
    {
        strcpy(tem_index, buff);
        strcat(text, buff);

        const char delim[2] = "|";
        strtok(tem_index, delim);
        len_buff = strlen(buff);
        len_index = strlen(tem_index);
        strncpy(tem_table, buff + len_index + 1, len_buff - len_index);

        strtok(tem_table, delim);

        if (!strcmp(tem_table, tableName.c_str())) {
          
            string str_index(tem_index);
            (*indexNameVector).push_back(str_index);
        }
    }

    fclose(fp);
    return 10;
}

/* 7.index type is one-bit positive integers */
/* if exist, return index type (>= 0 )*/
/* else return -1 */
int Catalog::getIndexType(string indexName) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    int len_buff, len_index = strlen(indexName.c_str());
    int type = -1;

    while (fgets(buff, BUF_SIZE, fp) != NULL)
    {
        len_buff = strlen(buff);
       
        int tail = 4;
        if (buff[len_buff - 1] != '\n')
            tail = 3;
        if (!strncmp(buff, indexName.c_str(), len_index) && buff[len_index] == '|') {
            type = buff[len_buff - tail] - '0';
            //if (buff[len_buff - tail - 1] == '-')
            //    type = -type;
        }

    }

    fclose(fp);

    return type;
}

/* get length operation */
/* 1.get length of the table */
/* return size (>=0) */
int Catalog::calculateLength_table(string tableName) {
    Catalog tem;
    int res = tem.getRecordNum(tableName);
    vector<Attribute>* attributeVector = new vector<Attribute>;
    tem.attributeGet(tableName, attributeVector);

    int size = 0, type;
    for (auto i = (*attributeVector).begin(); i != (*attributeVector).end(); i++) {
        type = (*i).getType();
        size += tem.calculateLength_type(type);
    }

    size *= res;

    return size;
}

/* 2.get length of attribute type */
/* return size (>=0) */
int Catalog::calculateLength_type(int type) {
    if (type == 0) {
        return sizeof(int);
    }
    else if (type == -1)
    {
        return sizeof(float);
    }
    else {
        return type * (int)sizeof(char); // Note that the type stores in Attribute.h
    }
}

/* 3.get length of the all attributes */
/* return size (>=0) */
int Catalog::calculateLength_attribute(string tableName) {
    Catalog tem;
    int res = tem.getRecordNum(tableName);
    vector<Attribute>* attributeVector = new vector<Attribute>;
    tem.attributeGet(tableName, attributeVector);

    int size = 0, type;
    for (auto i = (*attributeVector).begin(); i != (*attributeVector).end(); i++) {
        type = (*i).getType();
        size += tem.calculateLength_type(type);
    }

    return size;
}


/* 4.get offset */
/* if attributeName exist,return size */
/* else return 0 */
int Catalog::getoffset(string tableName, string attributeName) {
    
    vector<Attribute> attributeVector;

    attributeGet(tableName, &attributeVector);

    int size = 0,i;
    for (i = 0; i < attributeVector.size(); i++) {
        cout << attributeVector[i].getName() << endl;
        if (attributeVector[i].getName() == attributeName) {
            break;
        }
        int type = attributeVector[i].getType();
        size += calculateLength_type(type);
    }
    
    if (i == attributeVector.size())
        return 0;
    else
        return size;
}

/* print the operation result */
void Catalog::print(int i) {
    switch (i) {
    case(0):cout << "implementation fail" << endl; break;
    case(1):cout << "find table successfully" << endl; break;
    case(-1):cout << "find table fail, table doesn't exist" << endl; break;
    case(2):cout << "drop table successfully" << endl; break;
    case(-2):cout << "drop table fail, table doesn't exist" << endl; break;
    case(3):cout << "add table successfully" << endl; break;
    case(-3):cout << "add table fail, table has already existed" << endl; break;
    case(4):cout << "increase record number successfuully" << endl; break;
    case(5):cout << "decrease number successfully" << endl; break;
    case(6):cout << "find index successfully" << endl; break;
    case(-6):cout << "find index fail, index doesn't exist" << endl; break;
    case(7):cout << "add index successfully" << endl; break;
    case(-7):cout << "index already exist on the same table" << endl; break;
    case(8):cout << "drop index successfully" << endl; break;
    case(9):cout << "revoke index on attribute successfully" << endl; break;
    case(10):cout << "get index on table successfully" << endl; break;
    case(11):cout << "set index on attribute successfully" << endl; break;
    case(12):cout << "get attribute successfully" << endl; break;
    }
}

int char2int(char type[], int len) {
    int num = 0, flag = 1, i = 0;
    if (type[0] == '-') {
        flag = -1;
        i = 1;
    }

    while (i < len) {
        num += (int)type[i] - '0';
        i++;
        if (i < len)
            num *= 10;
    }
    num *= flag;
    return num;
}

/* if table doesn't exist */
/* return "Error" */
string Catalog::getPrimaryKeyName(string tableName) {
    FILE* fp;
    string post = ".txt";
    tableName += post;

    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return "Error";

    char buff[BUF_SIZE] = { 0 };
    char tem_name[BUF_SIZE] = { 0 };
    int len_buff,count;
    int type = -1;

    if (fgets(buff, BUF_SIZE, fp) != NULL) {
        len_buff = strlen(buff);
        int k = 4,j;
        if (buff[len_buff-1] != '\n') {
            k = 3;
        }
        for (j = len_buff - k; buff[j] != '|';j--) { }
        strncpy(tem_name, buff + j+1,len_buff-j-k);
    }
    string primaryKey(tem_name);
    return primaryKey;
}
