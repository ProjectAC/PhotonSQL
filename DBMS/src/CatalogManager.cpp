#include "../include/CatalogManager.h"
#include "../include/IndexManager.h"
#include "../include/Exception.h"

#include "rapidjson/rapidjson.h"  
#include "rapidjson/document.h"  
#include "rapidjson/writer.h"  
#include "rapidjson/stringbuffer.h"  
#include "rapidjson/prettywriter.h"  

#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
using namespace rapidjson;

namespace Photon
{
    CatalogManager *CatalogManager::instance = nullptr;

    CatalogManager &CatalogManager::getInstance()
    {
        return *CatalogManager::instance;
    }

    CatalogManager::CatalogManager()
    {
        if (instance == nullptr)
            instance = this;
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
        tables.insert({tableName, Table(columns, set<string>(), 0)});
    }

    void CatalogManager::createIndex(const std::string &indexName, const std::string &tableName, const std::string columnName)
    {
        indicies.insert({ indexName, {tableName, columnName} });
        getTable(tableName).addIndex(indexName);
    }

    void CatalogManager::dropTable(const std::string &tableName)
    {
        auto &ix = getTable(tableName).getIndicies();
        for (auto &i : ix)
        {
            indicies.erase(i);
            IndexManager::getInstance().drop(i);
        }

        tables.erase(tableName);
    }

    void CatalogManager::dropIndex(const std::string &indexName)
    {
        getTable(indicies[indexName].table).removeIndex(indexName);
        indicies.erase(indexName);
    }

    /* JSON structure
     * {
     *   indicies: [
     *     {
     *       name: string,
     *       table: string,
     *       column: string
     *     }
     *   ],
     *   tables: [
     *     {
     *       name: string,
     *       increment: uint,
     *       columns: [
     *         {
     *           name: string,
     *           type: uint,
     *           length: uint,
     *           notNull: bool,
     *           unique: bool
     *         }
     *       ]
     *     }
     *   ]
     * }
     */

    void CatalogManager::loadCatalog(const std::string &fileName)
    {
        ifstream file(fileName);
        string json, tmp;

        if (file.fail() || file.bad() || file.eof())
            return;

        while (file.good())
        {
            getline(file, tmp);
            json += tmp + '\n';
        }
        if (json.length() == 1)
            return;

        Document d;
        d.Parse(json.c_str());

        // Build indicies
        unordered_map<string, set<string>> indicies;
        const Value &indiciesInfo = d["indicies"];
        for (SizeType i = 0; i < indiciesInfo.Size(); i++)
        {
            const Value &t = indiciesInfo[i];
            string name = t["name"].GetString();
            string table = t["table"].GetString();
            string column = t["column"].GetString();

            if (indicies.find(name) == indicies.end())
                indicies[name] = set<string>();
            indicies[table].insert(name);
            this->indicies[name] = Index({ table, column });
        }

        // Build tables
        const Value &tablesInfo = d["tables"];
        for (SizeType i = 0; i < tablesInfo.Size(); i++)
        {
            const Value &t = tablesInfo[i];
            string name = t["name"].GetString();
            uint inc = t["increment"].GetUint();
            
            // Columns
            const Value &columnsInfo = t["columns"];
            vector<Column> columns;
            for (SizeType j = 0; j < columnsInfo.Size(); j++)
            {
                const Value &c = columnsInfo[j];
                columns.push_back(Column( { c["name"].GetString(), (AttributeType)c["type"].GetUint(), c["length"].GetUint(), c["notNull"].GetBool(), c["unique"].GetBool() } ));
            }

            this->tables[name] = { columns, indicies[name], inc };
        }

        file.close();
    }

    void CatalogManager::saveCatalog(const std::string &fileName)
    {
        ofstream file(fileName);

        Document d;
        rapidjson::Document::AllocatorType &allocator = d.GetAllocator();
        d.SetObject();

        Value indicies(kArrayType);
        for (auto &i : this->indicies)
        {
            Value index(kObjectType);
            
            index.AddMember("name"  , GenericStringRef<char>(i.first        .c_str()), allocator);
            index.AddMember("table" , GenericStringRef<char>(i.second.table .c_str()), allocator);
            index.AddMember("column", GenericStringRef<char>(i.second.column.c_str()), allocator);
            indicies.PushBack(index, allocator);
        }
        d.AddMember("indicies", indicies, allocator);

        Value tables(kArrayType);
        for (auto &t : this->tables)
        {
            Value table(kObjectType);
            table.AddMember("name", GenericStringRef<char>(t.first.c_str()), allocator);
            table.AddMember("increment", t.second.getIncrement(), allocator);

            Value columns(kArrayType);
            for (auto &c : t.second.getColumns())
            {
                Value column(kObjectType);
                column.AddMember("name", GenericStringRef<char>(c.name.c_str()), allocator);
                column.AddMember("type", c.type, allocator);
                column.AddMember("length", c.length, allocator);
                column.AddMember("notNull", c.notNull, allocator);
                column.AddMember("unique", c.unique, allocator);
                columns.PushBack(column, allocator);
            }
            table.AddMember("columns", columns, allocator);
            tables.PushBack(table, allocator);
        }
        d.AddMember("tables", tables, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        file << buffer.GetString();
        file.close();
    }
}