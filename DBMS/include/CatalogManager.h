#pragma once

#include <map>
#include "Catalog.h"

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

    private:

        static CatalogManager *instance;
        std::map<std::string, Table> tables;
        std::map<std::string, Index> indicies;
    };
}
