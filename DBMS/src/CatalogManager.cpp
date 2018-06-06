#include "../include/CatalogManager.h"
#include "../include/Exception.h"
#include <rapidjson/document.h>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
using namespace rapidjson;

namespace Photon
{
    CatalogManager &CatalogManager::getInstance()
    {
        return *CatalogManager::instance;
    }

    CatalogManager::CatalogManager()
    {
        CatalogManager::instance = this;
    }

    Table &CatalogManager::getTable(const std::string &name)
    {
        auto t = tables.find(name);
        if (t == tables.end())
            throw ObjectNotFountException();
        return t->second;
    }

    Index &CatalogManager::getIndex(const std::string &index)
    {
        auto i = indicies.find(index);
        if (i == indicies.end())
            throw ObjectNotFountException();
        return i->second;
    }

    void CatalogManager::createTable(const std::string &tableName, const std::vector<Column> columns)
    {
        tables.insert({tableName, Table(columns, vector<std::string>(), 0)});
    }

    void CatalogManager::createIndex(const std::string &indexName, const std::string &tableName, const std::string columnName)
    {
        indicies.insert({ tableName, {indexName, tableName, columnName} });
    }

    void CatalogManager::dropTable(const std::string &tableName)
    {
        tables.erase(tableName);
    }

    void CatalogManager::dropIndex(const std::string &indexName)
    {
        indicies.erase(indexName);
    }

    // TODO

    void CatalogManager::loadCatalog(const std::string &fileName)
    {
        ifstream file(fileName);
        string json, tmp;

        while (file.good())
        {
            getline(file, tmp);
            json += tmp + '\n';
        }

        Document d;
        d.Parse(json.c_str());

        // Build indicies
        unordered_map<string, vector<string>> indicies;
        const Value &indiciesInfo = d["indicies"];
        for (SizeType i = 0; i < indiciesInfo.Size(); i++)
        {
            const Value &t = indiciesInfo[i];
            string name = t["name"].GetString();
            string table = t["table"].GetString();
            string column = t["column"].GetString();

            if (indicies.find(name) == indicies.end())
                indicies[name] = vector<string>();
            indicies[table].push_back(name);
            this->indicies[name] = Index({ name, table, column });
        }

        // Build tables
        const Value &tablesInfo = d["tables"];
        for (SizeType i = 0; i < tablesInfo.Size(); i++)
        {
            const Value &t = tablesInfo[i];
            string name = t["name"].GetString();
            uint inc = t["increment"].GetInt();
            
            // Columns
            const Value &columnsInfo = t["columns"];
            vector<Column> columns;
            for (SizeType j = 0; j < columnsInfo.Size(); j++)
            {
                const Value &c = columnsInfo[j];
                columns.push_back(Column( { c["name"].GetString(), (AttributeType)c["type"].GetInt(), c["length"].GetInt(), c["notNull"].GetBool(), c["unique"].GetBool() } ));
            }

            this->tables[name] = Table(columns, indicies[name], inc);
        }
    }

    void CatalogManager::saveCatalog(const std::string &fileName)
    {

    }
}