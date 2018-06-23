#include "../include/Catalog.h"
#include "../include/Exception.h"
#include <iostream>

using namespace std;

namespace Photon
{
    /////////////////// Attibute ///////////////////

    bool operator <(const Attribute &a, const Attribute &b)
    {
        /*
        if (a.index() != b.index())
            throw TypeMismatchException();
        */
        
        if (a.index() == INTEGER)
        {
            if (b.index() == INTEGER)
                return get<Integer>(a) < get<Integer>(b);
            else if (b.index() == REAL)
                return get<Integer>(a) < get<Real>(b);
            else
                throw TypeMismatchException();
        }
        else if (a.index() == REAL)
        {
            if (b.index() == INTEGER)
                return get<Real>(a) < get<Integer>(b);
            else if (b.index() == REAL)
                return get<Real>(a) < get<Real>(b);
            else
                throw TypeMismatchException();
        }
        else if (a.index() == STRING)
        {
            if (b.index() == STRING)
            {
                // cout << get<String>(a).c_str() << " " << get<String>(b).c_str() << " " << strcmp(get<String>(a).c_str(), get<String>(b).c_str()) << endl;
                return strcmp(get<String>(a).c_str(), get<String>(b).c_str()) < 0;
            }
            else
                throw TypeMismatchException();
        }
        else
        {
            throw UnknownTypeException();
        }
    }

    std::ostream & operator<<(std::ostream &out, const Attribute &a)
    {
        if (a.index() == INTEGER)
        {
            out << get<Integer>(a);
        }
        else if (a.index() == REAL)
        {
            out << get<Real>(a);
        }
        else if (a.index() == STRING)
        {
            out << get<String>(a).c_str();
        }
        else
        {
            out << "Null";
        }
        return out;
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

    set<string> & Table::getIndicies()
    {
        return indicies;
    }

    void Table::addIndex(string s)
    {
        indicies.insert(s);
    }

    void Table::removeIndex(string s)
    {
        indicies.erase(s);
    }

    uint Table::getIncrement()
    {
        return autoIncrement;
    }

    void Table::count()
    {
        autoIncrement++;
    }

    Table::Table(const vector<Column> & columns, const set<std::string> & indicies, uint autoIncrement) :
        columns(columns),
        indicies(indicies),
        autoIncrement(autoIncrement)
    {
        width = 0;
        for (auto &c : columns)
            width += c.length;
        width += columns.size() / 8 + 1;
    }
}
