#include"API.h"

/*** table operation ***/
/* 1.drop table */
/* @param:table name */
void API::tableDrop(string tableName) {
    if (!tableExist(tableName)) return;

    vector<string> indexNameVector;

    //get all index in the table, and drop them all
    indexNameListGet(tableName, &indexNameVector);

    for (int i = 0; i < indexNameVector.size(); i++)
    {
        indexDrop(indexNameVector[i]);
    }

    //delete a table file
    if (cm->dropTable(tableName) == 2)
    {
        cout << "Drop table " << tableName << " successfully." << endl;
    }
}

/* 2.create table */
/* @param: table name,attributes,primary key & location */
void API::tableCreate(string tableName, vector<Attribute>* attributeVector, string primaryKeyName, int primaryKeyLocation) {
    if (cm->findTable(tableName) == 1)
    {
        cout << "There is a table " << tableName << " already" << endl;
        return;
    }

    //CatalogManager to create a table information
    if (cm->createTable(tableName, attributeVector, primaryKeyName) == 3)
    {
       cout << "Create table " << tableName << " successfully" << endl;
    }

    if (primaryKeyName != "")
    {
        //get a primary key
        string indexName = primaryIndexNameGet(tableName);
        indexCreate(indexName, tableName, primaryKeyName);
    }
}


/* drop index */
/* @param: indexName */
void API::indexDrop(string indexName) {
    if (cm->findIndex(indexName) != "") {
        cout << "There is no index " << indexName << endl;
        return;
    }

    //delete a index file
    if (rm->index_drop(indexName))
    {

        //get type of index
        int indexType = cm->getIndexType(indexName);
        if (indexType == -1)
        {
            cout << "error" << endl;
            return;
        }

        //delete a index information
        cm->dropIndex(indexName);

        //delete a index tree
        im->drop_index(indexName, indexType);            // 需要"INDEX_FILE_"+indexName吗？ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        cout << "Drop index " << indexName << " successfully" << endl;
    }

}


/* create index */
void API::indexCreate(string indexName, string tableName, string attributeName)
{
    if (cm->findIndex(indexName) != "")
    {
        cout << "There is index " << indexName << " already" << endl;
        return;
    }

    if (!tableExist(tableName)) return;

    vector<Attribute> attributeVector;
    cm->attributeGet(tableName, &attributeVector);
    int i;
    int type = 0;
    for (i = 0; i < attributeVector.size(); i++)
    {
        if (attributeName == attributeVector[i].name)
        {
            if (!attributeVector[i].ifUnique)
            {
                cout << "the attribute is not unique" << endl;
                return;
            }
            type = attributeVector[i].type;
            break;
        }
    }

    if (i == attributeVector.size())
    {
        cout << "there is not this attribute in the table" << endl;
        return;
    }

    //RecordManager to create a index file
    if (rm->index_create(indexName))
    {
        //CatalogManager to add a index information
        cm->addIndex(indexName, tableName, attributeName, type);

        //get type of index
        int indexType = cm->getIndexType(indexName);
        if (indexType == -1)
        {
            cout << "error" << endl;
            return;
        }

        //indexManager to create a index tress
        im->create_index(indexName, indexType);

        //recordManager insert already record to index
        //rm->indexRecordAllAlreadyInsert(tableName, indexName);          insert the index of a record of a table in a block？？ <<<<<<<<<<<<<<<<<<<<<<<<

        cout << "Create index " << indexName << " successfully" << endl;
    }
    else
    {
        cout << "Create index " << indexName << " fail" << endl;
    }
}

/* check if table exist,input: table name */
/* if exist, return 1 */
/* else return 0 */
int API::tableExist(string tableName)
{
    if (cm->findTable(tableName) != 1)
    {
        cout << "There is no table " << tableName << endl;
        return 0;
    }
    else
    {
        return 1;
    }
}

/* get all indexes of the table,input:table name,index vector */
/* store index names in vector and return 10 */
int API::indexNameListGet(string tableName, vector<string>* indexNameVector) {
    if (!tableExist(tableName)) return 0;

    return cm->indexNameListGet(tableName, indexNameVector);
}

string API::primaryIndexNameGet(string tableName) {

}
