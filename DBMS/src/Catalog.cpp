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
        
        if (a.index() == INTEGER)
            return get<Integer>(a) < get<Integer>(b);
        else if (a.index() == REAL)
            return get<Real>(a) < get<Real>(b);
        else if (a.index() == STRING)
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

    set<string> & Table::getIndicies()
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

    Table::Table(const vector<Column> & columns, const set<std::string> & indicies, uint autoIncrement) :
        columns(columns),
        indicies(indicies),
        autoIncrement(autoIncrement)
    {
        width = 0;
        for (auto &c : columns)
            width += c.length;
    }

	//////////
	void Photon::Table::addRow(const std::vector<Row>& res)
	{	
		string content;
		string temp;
		for (auto &i : res) {
			content = "";
			for (auto &j : i) {
				temp = std::get<String>(j);
				content = content + temp;
			}
			indicies.insert(content);
		}
	}


	std::vector<Row> Photon::Table::select(const Condition & c)
	{
		Attribute tempattr;
		uint start;
		Row temprow;
		std::vector<Row> result;
		for (auto &index : indicies) {
			start = 0;
			temprow.clear();
			for (auto &col : columns) {
				tempattr = index.substr(start, col.length);
				start += col.length;
				temprow.push_back(tempattr);
			}
			if (c.satisfy(columns, temprow)) 
				result.push_back(temprow);
		}
		return result;
	}


	uint Photon::Table::Delete(const Condition & c)
	{
		Attribute tempattr;
		uint start, numdelete = 0;
		Row temprow;
		for (auto &index : indicies) {
			start = 0;
			temprow.clear();
			for (auto &col : columns) {
				tempattr = index.substr(start, col.length);
				start += col.length;
				temprow.push_back(tempattr);
			}
			if (c.satisfy(columns, temprow)) {
				indicies.erase(index);
				numdelete++;
			}	
		}

		return numdelete;
	}
}
