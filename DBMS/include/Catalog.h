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

    typedef std::variant<std::monostate, Integer, Real, String> Attribute;
    typedef std::vector<Attribute> Row;
    bool operator <(const Attribute &a, const Attribute &b);
    
    struct Column
    {
        std::string name;
        AttributeType type;
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

    class Table
    {
    public:
        Column &getColumn(uint id);
        std::vector<Column> &getColumns();
        uint hasColumn(const std::string &name);
        uint rowSize();
        std::vector<Index> &getIndicies();
        
        uint getIncrement();
        void count();

        Table(const std::vector<Column> &columns, const std::vector<std::string> &indicies, uint autoIncrement);

    private:

        std::vector<Column> columns;
        std::vector<std::string> indicies;
        uint width;
        uint autoIncrement;
    };
}