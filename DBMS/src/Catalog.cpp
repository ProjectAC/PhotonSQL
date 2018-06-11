#include "../include/Catalog.h"
#include "../include/Exception.h"

using namespace std;

namespace Photon
{
    /////////////////// Attibute ///////////////////

    bool operator <(const Attribute &a, const Attribute &b)
    {
        if (a.index() != b.index())
            throw TypeMismatchException();
        
        if (a.index == INTEGER)
            return get<Integer>(a) < get<Integer>(b);
        else if (a.index == REAL)
            return get<Real>(a) < get<Real>(b);
        else if (a.index == STRING)
            return get<String>(a) < get<String>(b);
        else
            throw UnknownTypeException();
    }

    /////////////////// Table ///////////////////

    Column & Table::getColumn(uint id)
    {
        return columns[id];
    }

    vector<Column> & Table::getColumns()
    {
        return columns;
    }

    uint Table::hasColumn(const string &name)
    {
        int cnt = 0;
        for (auto &c : columns)
            if (c.name == name)
                return cnt;
            else
                cnt++;
        return -1;
    }

    uint Table::rowSize()
    {
        return width;
    }

    vector<std::string> & Table::getIndicies()
    {
        return indicies;
    }

    uint Table::getIncrement()
    {
        return autoIncrement;
    }

    void Table::count()
    {
        autoIncrement++;
    }

    Table::Table(const vector<Column> & columns, const vector<std::string> & indicies, uint autoIncrement) :std
        columns(columns),
        indicies(indicies),
        autoIncrement(autoIncrement)
    {
        width = 0;
        for (auto &c : columns)
            width += c.length;
    }
}
