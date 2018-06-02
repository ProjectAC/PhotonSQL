#include "../include/Catalog.h"
#include "../include/Exception.h"

using namespace std;

namespace Photon
{
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

    Column Table::getColumn(uint id)
    {
        return columns[id];
    }

    vector<Column> Table::getColumns()
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

    vector<Index> Table::getIndicies()
    {
        return indicies;
    }

    Table::Table(vector<Column> columns, vector<Index> indicies) :
        columns(columns),
        indicies(indicies)
    {
        width = 0;
        for (auto &c : columns)
            width += c.length;
    }
}