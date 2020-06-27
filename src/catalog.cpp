#include "catalog.h"
#define SIZE 1024
#define BUF_SIZE 256

Catalog::Catalog(){}
Catalog:: ~Catalog(){}

//table implementation
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

int Catalog::dropTable(string tableName) {
    string post = ".txt";
    tableName += post;
    if (remove(tableName.c_str()))
    {
        return -2;
    }
    return 2;
}

int Catalog::createTable(string tableName, vector<Attribute>* attributeVector, string primaryKeyName = "") {
    Catalog tem;
    int find = tem.findTable(tableName);
    if (find == 1) {
        return -3;
    }
    // create file
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
        // attribute number
        int size = (*attributeVector).size();
        fprintf(fp,"%d|",size);
  
        // record number
        int record = 0;
        fprintf(fp, "%d|", record);

        // primary key
        if (primaryKeyName != "")
            fprintf(fp,"%s||\n",primaryKeyName.c_str());
     
        for (int i = 0; i < (*attributeVector).size(); i++) {
            string name = (*attributeVector)[i].getName();
            int type = (*attributeVector)[i].getType();
            bool unique = (*attributeVector)[i].getUnique();
            string index = (*attributeVector)[i].getIndex();
            fprintf(fp,"%s|%d|%d|%s||\n",name.c_str(),type,unique,index.c_str());
        }
        fclose(fp);
        return 3;
    }
}

//record
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
    int a_n,r_n; 
    fscanf(fp,"%d|%d", &a_n,&r_n);
    fclose(fp);
    return r_n;
}

int Catalog::insertRecordNum(string tableName, int recordNum) {  // increment the number of record
    Catalog tem;
    int find = tem.findTable(tableName);
    if (find == -1) {
        return -1;
    }
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(),"r");
    if (fp == NULL)
        return -1;
    int i = 0,j = 0,a,b;
    char p[SIZE];
    char ch;
    fscanf(fp, "%d|%d", &a, &b);
    
    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  break;
        p[i++] = ch;
    }
    fclose(fp);

    fp = fopen(tableName.c_str(), "w");
    b = b + recordNum;
    fprintf(fp, "%d|%d", a, b);
    while (j < i) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);
    return 4;
}

int Catalog::deleteRecordNum(string tableName, int deleteNum) {// delete the number of record
    Catalog tem;
    int find = tem.findTable(tableName);
    if (find == -1) {
        return -1;
    }
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;
    int i = 0, j = 0, a, b;
    char p[SIZE] = { 0 }, ch;
    fscanf(fp, "%d|%d", &a, &b);

    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  break;
        p[i++] = ch;
    }
    fclose(fp);

    fp = fopen(tableName.c_str(), "w");
    b = b - deleteNum;
    fprintf(fp, "%d|%d", a, b);
    while (j < i) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);
    return 5;
}

//attribute
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

    int a_num;	//attribute number
    fscanf(fp, "%d", &a_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {

        for (int k = 0; k < a_num; k++) {
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

            //cout << tem_name << " " << tem_type << " " << tem_unique << " " << tem_index << endl;

            int type = char2int(tem_type, len_type);
            int unique = (int)tem_unique - '0';

            //cout << type << endl;
            Attribute tem(tem_name,type,unique,tem_index);
            (*attributeVector).push_back(tem);
        }
    }

    fclose(fp);
    return 12;
}

//index
int Catalog::findIndex(string indexName) {
    int flag = 0;
    FILE* fp;
    fp = fopen("Index.txt", "r");
    if (fp == NULL)
        return 0;
    char line[256] = { 0 };
    while (!feof(fp) && !ferror(fp)) {
        strcpy(line, "\n");
        fgets(line, sizeof(line), fp);
      
        const char delim[2] = "|";
        char* tem = strtok(line, delim);
       
        if (!strcmp(indexName.c_str(), tem)) {
            flag = 1;
            break;
        }
    }
    fclose(fp);
    if (flag)
        return 6;
    else
        return -6;
}

int Catalog::addIndex(string indexName, string tableName, string attributeName, int type) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    int i = 0, j = 0;
    char p[SIZE] = { 0 };
    char ch;

    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == EOF)  break;
        p[i++] = ch;
    }
    fclose(fp);

    string index;
    if (type >= 0)
        index = indexName + "|" + tableName + "|" + attributeName + "|" + (char)(type + '0') + "||\n";
    else
        index = indexName + "|" + tableName + "|" + attributeName + "|-" + (char)(-type + '0') + "||\n";

    strcat(p, index.c_str());
    fp = fopen(table.c_str(), "w");

    while (j < strlen(p)) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);
    return 7;
}

int Catalog::dropIndex(string index) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char p[SIZE] = { 0 };
    int len, l, j = 0;

    while (fgets(buff, SIZE, fp) != NULL)
    {
        len = strlen(buff);
        l = strlen(index.c_str());
        if (strncmp(buff, index.c_str(), l))
            strcat(p, buff);
   
    }

    fclose(fp);

    fp = fopen(table.c_str(), "w");

    while (j < strlen(p)) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);

    return 8;
}

int Catalog::setIndexOnAttribute(string tableName, string AttributeName, string indexName) {
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char p[SIZE] = { 0 };
    int len_buff, len_attribute, j = 0;
    int a_num;	//attribute number
    fscanf(fp, "%d", &a_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {
        strcat(p, buff);
        for (int k = 0; k < a_num; k++) {
            fgets(buff, BUF_SIZE, fp);
            len_buff = strlen(buff);
            len_attribute = strlen(AttributeName.c_str());
            if (strncmp(buff, AttributeName.c_str(), len_attribute) || buff[len_attribute] != '|')	//find attribute
                strcat(p, buff);
            else {
                char tem[BUF_SIZE] = { 0 };
                int find = 4;
                while (buff[len_buff - find] != '|') { find++; }
                strncpy(tem, buff, len_buff - find + 1);
                strcat(tem, indexName.c_str());
                char tail[4] = "||\n";
                strcat(tem, tail);
                cout << tem;
                strcat(p, tem);
            }
        }
    }

    fclose(fp);

    fp = fopen(tableName.c_str(), "w");

    fprintf(fp, "%d", a_num);
    while (j < strlen(p)) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);
    return 11;
}

int Catalog::revokeIndexOnAttribute(string tableName, string AttributeName, string indexName) {
    FILE* fp;
    tableName += ".txt";
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char p[SIZE] = { 0 };
    int len_buff, len_attribute, j = 0;
    int a_num;	//attribute number
    fscanf(fp, "%d", &a_num);

    if (fgets(buff, BUF_SIZE, fp) != NULL) {
        strcat(p, buff);
        for (int k = 0; k < a_num; k++) {
            fgets(buff, BUF_SIZE, fp);
            len_buff = strlen(buff);
            len_attribute = strlen(AttributeName.c_str());
            if (strncmp(buff, AttributeName.c_str(), len_attribute) || buff[len_attribute] != '|')	//find attribute
                strcat(p, buff);
            else {
                char tem[BUF_SIZE] = { 0 };
                int len_index = strlen(indexName.c_str());
                int new_len = len_buff - len_index - 3;
                strncpy(tem, buff, new_len);
                char tail[4] = "||\n";
                strcat(tem, tail);
                strcat(p, tem);
            }
        }
    }

    fclose(fp);

    fp = fopen(tableName.c_str(), "w");

    fprintf(fp, "%d", a_num);
    while (j < strlen(p)) {
        fprintf(fp, "%c", p[j++]);
    }
    fclose(fp);
    return 9;
}

int Catalog::indexNameListGet(string tableName, vector<string>* indexNameVector) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    char tem_index[BUF_SIZE] = { 0 };
    char tem_table[BUF_SIZE] = { 0 };
    char p[SIZE] = { 0 };
    int len_buff, len_index, i = 0, j = 0;

    while (fgets(buff, BUF_SIZE, fp) != NULL)
    {
        strcpy(tem_index, buff);
        strcat(p, buff);

        const char delim[2] = "|";
        strtok(tem_index, delim);
        len_buff = strlen(buff);
        len_index = strlen(tem_index);
        strncpy(tem_table, buff + len_index + 1, len_buff - len_index);

        strtok(tem_table, delim);

        if (!strcmp(tem_table, tableName.c_str())) {
            //tem_index[len_index] = '\0';
            string str_index(tem_index);
            (*indexNameVector).push_back(str_index);
        }
    }

    fclose(fp);
    return 10;
}

int Catalog::getIndexType(string indexName) {
    FILE* fp;
    string table = "Index.txt";
    fp = fopen(table.c_str(), "r");
    if (fp == NULL)
        return -1;

    char buff[BUF_SIZE] = { 0 };
    int len_buff, len_index, type = 0;

    while (fgets(buff, BUF_SIZE, fp) != NULL)
    {
        len_buff = strlen(buff);
        len_index = strlen(indexName.c_str());
        int tail = 4;
        if (buff[len_buff - 1] != '\n')
            tail = 3;
        if (!strncmp(buff, indexName.c_str(), len_index) && buff[len_index] == '|') {
            type = buff[len_buff - tail] - '0';
            if (buff[len_buff - tail - 1] == '-')
                type = -type;
        }

    }

    fclose(fp);

    return type;
}

//length 
int Catalog::calculateLength(string tableName) {
    Catalog tem;
    int res = tem.getRecordNum(tableName);
    vector<Attribute>* attributeVector = new vector<Attribute>;
    tem.attributeGet(tableName, attributeVector);

    int size = 0,type;
    for (auto i = (*attributeVector).begin(); i != (*attributeVector).end(); i++) {
        type = (*i).getType();
        size += tem.calculateLength2(type);
    }
    
    size *= res;

    return size;
}

int Catalog::calculateLength2(int type) {
    if (type == 0) {
        return sizeof(int);
    }
    else if (type == -1)
    {
        return sizeof(float);
    }
    else {
        return type*(int)sizeof(char); // Note that the type stores in Attribute.h
    }
}

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