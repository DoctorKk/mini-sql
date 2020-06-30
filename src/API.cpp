#include"API.h"

void API::tableDrop(string tableName) {
    if (!tableExist(tableName)) return;

    vector<string> indexNameVector;

    //get all index in the table, and drop them all
    indexNameListGet(tableName, &indexNameVector);
    for (int i = 0; i < indexNameVector.size(); i++)
    {
        printf("%s", indexNameVector[i].c_str());

        indexDrop(indexNameVector[i]);
    }

    //delete a table file
    if (cm->dropTable(tableName) == 2)
    {
        cout << "Drop table " << tableName << " successfully." << endl;
    }
}

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
