#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Catalog.h"
#include "Condition.h"

namespace Photon
{
    /* API Provider */
    class DBMS
    {
    public:

        std::vector<Row> Select(const Condition &c);
        uint Insert(const std::string &table, const std::vector<Row> &res);
        uint Delete(const Condition &c);
        bool CreateTable(const std::string &table, const std::vector<Column> columns);
        bool DropTable(const std::string &table);
        bool CreateIndex(const std::string &index, const std::string &table, const std::string &column);
        bool DropIndex(const std::string &index);

    private:
        


    };
}