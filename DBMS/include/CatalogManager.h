#pragma once

#include <map>
#include "rapidjson/rapidjson.h"  
#include "rapidjson/document.h"  
#include "rapidjson/writer.h"  
#include "rapidjson/stringbuffer.h"  
#include "rapidjson/prettywriter.h"  
#include "Condition.h"
#include "Catalog.h"
using namespace rapidjson;

namespace Photon
{
    class CatalogManager
    {
    public:

        static CatalogManager &getInstance();
        CatalogManager();

        Table &getTable(const std::string &name);
        Index &getIndex(const std::string &index);

        void createTable(const std::string &tableName, const std::vector<Column> columns);
        void createIndex(const std::string &indexName, const std::string &tableName, const std::string columnName);
        void dropTable(const std::string &tableName);
        void dropIndex(const std::string &indexName);

        // JSON!
        void loadCatalog(const std::string &fileName);
        void saveCatalog(const std::string &fileName);
		/////
		std::vector<Row> select(const Condition & c);
		uint Delete(const Condition & c);
    private:

        static CatalogManager *instance;
        std::map<std::string, Table> tables;
        std::map<std::string, Index> indicies;
    };
}
