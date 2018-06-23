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

        bool satisfy(const std::vector<Column> &columns, const Row &row) const;
		void clear();
        void add(const ConditionItem &item);

        void setIndex(std::string index, Attribute left, Attribute right);
        std::string getIndex() const;
        Attribute getLeft() const;
        Attribute getRight() const;

    private:

        std::vector<ConditionItem> conditions;
        std::string index;
        Attribute left;
        Attribute right;
    };
    
    class SQL
    {
        std::vector<std::string> split;

    public:

        enum Verb
        {
            NOTHING,
            SELECT,
            INSERT,
            DELETE,
            CREATE_TABLE,
            CREATE_INDEX,
            DROP_TABLE,
            DROP_INDEX
        };

        SQL(std::string sql);
        SQL();

        SQL & operator= (std::string sql);
        std::string getTable();
        Verb getVerb();
        Condition getCondition();
        std::vector<Row> SQL::getRows();

    private:
        void splitSQL(std::string sql);
    };
}