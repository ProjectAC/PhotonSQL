#pragma once

#include <string>
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
            std::string col1, col2;
            Relation relation;
            Attribute value;
            uint cn1, cn2;
        };

        bool satisfy(const std::vector<Column> &columns, const Row &row);
        const std::vector<ConditionItem> &getItems();

    private:

        std::vector<ConditionItem> conditions;
    };
}