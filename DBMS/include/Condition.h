#pragma once

#include <string>
#include <sstream>
#include "Catalog.h"

namespace Photon
{
    class Condition
    {
    public:
        
        enum Relation
        {
            LESS,
            NOT_GREATER,
            GREATER,
            NOT_LESS,
            EQUAL,
            NOT_EQUAL
        };

        struct ConditionItem
        {
            Attribute a1, a2;
            int i1, i2;
            Relation relation;
        };

        bool satisfy(const std::vector<Column> &columns, const Row &row);
		void clear();
        void add(const ConditionItem &item);
        //const std::vector<ConditionItem> &getItems();
        const std::string getAvailableIndex();
        void setTable(std::string name);

    private:

        std::string tableName;
        std::vector<ConditionItem> conditions;
    };
    
    class SQL
    {
    public:
        std::vector<std::string> split;
        SQL(std::string sql);

        Condition getCondition();
    };
}