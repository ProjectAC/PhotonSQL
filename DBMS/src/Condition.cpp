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

    bool Condition::satisfy(const std::vector<Column> &columns, const Row &row) const
    {
        if (!row.size())
            return false;
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
    
    void Condition::setIndex(std::string index, Attribute left, Attribute right)
    {
        this->index = index;
        this->left = left;
        this->right = right;
    }

    std::string Condition::getIndex() const
    {
        return this->index;
    }

    Attribute Condition::getLeft() const
    {
        return this->left;
    }

    Attribute Condition::getRight() const
    {
        return this->right;
    }

    SQL::SQL()
    {

    }

    SQL::SQL(std::string sql)
    {
        splitSQL(sql);
    }

    SQL & SQL::operator= (std::string sql)
    {
        splitSQL(sql);
        return *this;
    }

    void SQL::splitSQL(std::string sql)
    {
        split.clear();

        std::string cur = "";
        bool in = false;
        for (auto &c : sql)
        {
            if (c == '\'')
            {
                if (in)
                {
                    cur += '\'';
                    if (cur.length())
                        split.push_back(cur);
                    cur = "";
                }
                else
                {
                    if (cur.length())
                        split.push_back(cur);
                    cur = '\'';
                }
                in = !in;
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
                else if (!isalnum(c) && (c != '=' || isalnum(cur[0])))
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

    std::string SQL::getTable()
    {
        uint i = 0, n = split.size();
        for (i = 0; i < n - 2 && split[i] != "from" && split[i] != "into" && split[i] != "table"; i++);
        if (i == n - 2) throw SQLException();
        return split[i + 1];
    }

    SQL::Verb SQL::getVerb()
    {
        if (split[0] == "select")
            return SELECT;
        else if (split[0] == "insert")
            return SELECT;
        else if (split[0] == "delete")
            return SELECT;
        else if (split[0] == "create")
            if (split[1] == "table")
                return CREATE_TABLE;
            else if (split[1] == "table")
                return CREATE_INDEX;
            else
                throw SQLException();
        else if (split[0] == "drop")
            if (split[1] == "table")
                return DROP_TABLE;
            else if (split[1] == "table")
                return DROP_INDEX;
            else
                throw SQLException();
    }

    vector<Row> SQL::getRows()
    {
        std::string tname = getTable();
        Table &table = CatalogManager::getInstance().getTable(tname);
        auto &cols = table.getColumns();

        uint i = 0, n = split.size();
        for (i = 0; i < n - 1 && split[i] != "values"; i++);
        if (i == n - 1)
            throw SQLException();

        int cnt = -1;
        vector<Row> res;
        Row r;
        for (i++; i < n - 1; i++)
        {
            std::string &s = split[i];

            if (s == "(")
            {
                cnt = 0;
                r.clear();
            }
            else if (s == ")")
            {
                if (cnt != cols.size())
                    throw SQLException();
                res.push_back(r);
                cnt = -1;
            }
            else if (s == ",")
            {

            }
            else if (cnt == -1)
                throw SQLException();
            else if (s == "null")
            {
                if (cols[cnt].notNull)
                    throw SQLException();
                r.push_back(Attribute());
                cnt++;
            }
            else
            {
                if (cols[cnt].type == INTEGER)
                {
                    long long x;
                    sscanf(s.c_str(), "%lld", &x);
                    r.push_back(x);
                }
                else if (cols[cnt].type == REAL)
                {
                    double x;
                    sscanf(s.c_str(), "%lf", &x);
                    r.push_back(x);
                }
                else if (cols[cnt].type == STRING)
                {
                    r.push_back(s.substr(1, s.length() - 2));
                }
                cnt++;
            }
        }

        if (cnt != -1)
            throw SQLException();
        return res;
    }

    Condition SQL::getCondition()
    {
        Condition cond;
        Condition::ConditionItem ci;

        std::string tname = getTable();
        CatalogManager &cm = (CatalogManager::getInstance());
        Table &table = cm.getTable(tname);
        auto &indicies = table.getIndicies();

        uint i = 0, n = split.size(), t, col;
        for (i = 0; i < n - 1 && split[i] != "where"; i++);
        if (i == n - 1)
            return cond;

        for (i++, t = 0; i < n - 1; i++)
        {
            std::string &s = split[i];

            if (t == 0)  // op1
            {
                if (s[0] == '\'')
                {
                    ci.i1 = -1;
                    if (s[s.length() - 1] == '\'')
                        ci.a1 = s.substr(1, s.length() - 2);
                    else
                        throw SQLException();
                }
                else if (isdigit(s[0]))
                {
                    ci.i1 = -1;
                    if (s.find('.') != string::npos)
                    {
                        double x;
                        sscanf(s.c_str(), "%lf", &x);
                        ci.a1 = x;
                    }
                    else
                    {
                        long long x;
                        sscanf(s.c_str(), "%lld", &x);
                        ci.a1 = x;
                    }
                }
                else if ((col = table.hasColumn(s)) != (uint)-1)
                {
                    ci.i1 = col;
                }
                else
                {
                    throw SQLException();
                }
                t = 1;
            }
            else if (t == 1)  // relation
            {
                if (s == "<")
                    ci.relation = Condition::LESS;
                else if (s == ">")
                    ci.relation = Condition::GREATER;
                else if (s == "<=")
                    ci.relation = Condition::NOT_GREATER;
                else if (s == ">=")
                    ci.relation = Condition::NOT_LESS;
                else if (s == "=")
                    ci.relation = Condition::EQUAL;
                else if (s == "!=")
                    ci.relation = Condition::NOT_EQUAL;
                else
                    throw SQLException();
                t = 2;
            }
            else if (t == 2)  // op2
            {
                if (s[0] == '\'')
                {
                    ci.i2 = -1;
                    if (s[s.length() - 1] == '\'')
                        ci.a2 = s.substr(1, s.length() - 2);
                    else
                        throw SQLException();
                }
                else if (isdigit(s[0]))
                {
                    ci.i2 = -1;
                    if (s.find('.') != string::npos)
                    {
                        double x;
                        sscanf(s.c_str(), "%lf", &x);
                        ci.a2 = x;
                    }
                    else
                    {
                        long long x;
                        sscanf(s.c_str(), "%lld", &x);
                        ci.a2 = x;
                    }
                }
                else if ((col = table.hasColumn(s)) != (uint)-1)
                {
                    ci.i2 = col;
                }
                else
                {
                    throw SQLException();
                }

                if (cond.getIndex() == "")
                {
                    if (ci.i1 != -1 && ci.i2 == -1)
                    {
                        for (auto &t : indicies)
                        {
                            if (table.hasColumn((cm.getIndex(t).column)) == ci.i1)
                            {
                                //cond.setIndex(s, ci.a2);
                                break;
                            }
                        }
                    }
                    else if (ci.i2 != -1 && ci.i1 == -1)
                    {
                        for (auto &t : indicies)
                        {
                            if (table.hasColumn((cm.getIndex(t).column)) == ci.i1)
                            {
                                //cond.setIndex(s);
                                break;
                            }
                        }
                    }
                }

                cond.add(ci);
                t = 3;
            }
            else if (t == 3)
            {
                if (s == "and")
                    t = 0;
                else
                    throw SQLException();
            }
        }

        if (t != 3)
            throw SQLException();
        return cond;
    }
}