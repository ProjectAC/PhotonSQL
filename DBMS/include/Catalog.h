#pragma once

#include <variant>
#include <vector>
#include <set>
#include "Definitions.h"
#include "Condition.h"

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
        std::set<std::string> &getIndicies();
        
        uint getIncrement();
        void count();

        Table(const std::vector<Column> &columns, const std::set<std::string> &indicies, uint autoIncrement);
        Table() = default;
		//////////
		void addRow(const std::vector<Row>& res);
		std::vector<Row> select(const Condition & c);
		uint Delete(const Condition & c);
    private:

        std::vector<Column> columns;
        std::set<std::string> indicies;
        uint width;
        uint autoIncrement;
    };
}
