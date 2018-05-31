#pragma once

#include <map>
#include "Catalog.h"

namespace Photon
{
    class CatalogManager
    {
    public:

        static CatalogManager &getInstance();
        Table &getTable(const std::string &name);
        Index &getIndex(const std::string &index);

    private:

        std::map<std::string, Table> tables;
        std::map<std::string, Index> indicies;
    };
}
