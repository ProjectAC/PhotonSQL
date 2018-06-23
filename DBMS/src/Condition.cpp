#include "../include/Condition.h"
#include "../include/Exception.h"
#include "../include/CatalogManager.h"
#include <ctype.h>

using namespace std;

namespace Photon
{
	void Condition::clear()
	{
		conditions.clear();
	}
    
    void Condition::add(const ConditionItem &item)
	{
		conditions.push_back(item);
	}

    bool Condition::satisfy(const std::vector<Column> &columns, const Row &row)
    {
		for (auto &c : conditions)
        {
            const Attribute &a = c.i1 < 0 ? c.a1 : row[c.i1];
            const Attribute &b = c.i2 < 0 ? c.a2 : row[c.i2];

            if (c.relation == LESS)
            {
                if (!(a < b)) return false;
            }
            else if (c.relation == NOT_GREATER)
            {
                if (b < a) return false;
            }
            else if (c.relation == GREATER)
            {
                if (!(b < a)) return false;
            }
            else if (c.relation == NOT_LESS)
            {
                if (a < b) return false;
            }
            else if (c.relation == EQUAL)
            {
                if (a != b) return false;
            }
            else if (c.relation == NOT_EQUAL)
            {
                if (a == b) return false;
            }
            else
                throw SQLException();
		}
        return true;
    }

    void Condition::setTable(std::string name)
    {
        tableName = name;
    }

    SQL::SQL(std::string sql)
    {
        std::string cur = "";
        bool in = false;
        for (auto &c : sql)
        {
            if (c == '\'')
            {
                in = !in;
                cur += c;
            }
            else if (in)
            {
                cur += c;
            }
            else
            {
                if (isspace(c))
                {
                    if (cur.length())
                        split.push_back(cur);
                    cur = "";
                }
                else if (!isalnum(c))
                {
                    if (cur.length())
                        split.push_back(cur);
                    cur = c;
                }
                else
                {
                    cur += c;
                }
            }
        }

        if (cur.length())
            split.push_back(cur);
    }

    Condition SQL::getCondition()
    {
        Condition cond;
        Condition::ConditionItem ci;

        std::string tname = tableName();
        cond.setTable(tname);
        Table &t = CatalogManager::getInstance().getTable(tname);
        auto &cols = t.getColumns();

        uint i = 0, n = split.size();
        for (i = 0; i < n && split[i] == "where"; i++);
        i++;

        for (uint t = 0; i < n - 1; i++)
        {
            std::string &s = split[i];
            if (t == 0)  // op1
            {
                if (s[0] == '\"')
                {
                    if (s[s.length() - 1] == '\"')
                        ci.a1 = s.substr(1, s.length() - 2);
                }
                else if (isdigit(split[i][0]))
                t = 1;
            }
        }
    }
}