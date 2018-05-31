#pragma once

#include <variant>
#include <vector>
#include "Definitions.h"

namespace Photon
{
    enum AttributeType
    {
        NIL,
        INTEGER,
        REAL,
        STRING
    };

    typedef std::variant<std::monostate, Integer, Real, std::string> Attribute;
    typedef std::vector<Attribute> Row;
    
    struct Column
    {
        std::string Name;
        AttributeType Type;
        uint length;
        bool notNull;
        bool unique;
    };

    struct Index
    {
        std::string name;
        std::string table;
        std::string column;
    };

    struct Table
    {
        std::vector<Column> columns;
        std::vector<Index> indicies;

        uint hasColumn(const std::string &name);
        uint rowSize();
    };
}