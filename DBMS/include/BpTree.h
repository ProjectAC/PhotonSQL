#pragma once

#include <vector>
#include <stack>
#include "Definitions.h"
#include "Catalog.h"
#include "BufferManager.h"
#include "Exception.h"

namespace Photon
{
    class BpTree
    {
    public:
        typedef Attribute Key;
        typedef uint Value;

        struct Node;
        friend class BpIterator;

        void insert(const Key &key, const Value &value);
        void erase(const Key &key);
        std::pair<Node*, int> find(const Key &key);
        Node *load(uint nodeId);

        void debug();

        BpTree(std::string name);
        BpTree(std::string name, AttributeType type, uint keyLength);
        ~BpTree();

    private:

        std::stack<std::pair<Node*, int> > finds(const Key &key);

        void save();

        std::string name;
        uint root;
		AttributeType type;
        uint keyLength;
        uint degree;
        uint nodeCount;
    };

    struct BpTree::Node
    {
        BpTree *tree;
        uint id;
        uint size;
        Key *keys;
        Value *pointers;
        bool leaf;

        std::pair<Node *, Key> insert(int pos, const Key &key, const uint &val, uint &id)
        {
            // Move
            for (int i = size; i > pos; i--)
                pointers[i + 1] = pointers[i],
                keys[i] = keys[i - 1];

            // Insert
            keys[pos] = key;
            pointers[pos + 1] = val;
            size++;

            // Split
            if (size > tree->degree)
            {
                int nsize = (size + 1) / 2;
                int tot = size;
                auto nnode = new Node(tree, ++id, leaf);

                int i = nsize, j = 0;
                if (leaf == true) // Is leaf, construct linked list
                {
                    nnode->pointers[0] = pointers[0];
                    pointers[0] = nnode->id;
                    size = nsize;
                }
                else  // Not leaf, split point
                {
                    nnode->pointers[0] = pointers[nsize];
                    size = nsize - 1;
                }

                // Copy
                for (nnode->size = tot - i; i < tot; i++, j++)
                    nnode->pointers[j + 1] = pointers[i + 1],
                    nnode->keys[j] = keys[i];

                return { nnode, keys[size] };
            }
            else
            {
                return { nullptr, Attribute() };
            }
        }

        Node::Node(BpTree *tree, uint id, bool leaf) :
            tree(tree),
            id(id),
            size(0),
            keys(new Key[tree->degree + 1]),
            pointers(new Value[tree->degree + 2]),
            leaf(leaf)
        {
            memset(pointers, 0, sizeof(pointers));
        }

        Node::~Node()
        {
            delete[] keys;
            delete[] pointers;
        }

        void load(char *p, AttributeType type)
		{
			size = *(uint*)p;
            leaf = *(bool*)(p + 4);

            char *t = p;
            int i;

            for (i = 0, t = p + 5; i < (int)size; i++)
            {
                if (tree->type == INTEGER)
                    keys[i] = Attribute(*(Integer*)(t));
                else if (tree->type == REAL)
                    keys[i] = Attribute(*(Real*)(t));
                else if (tree->type == STRING)
                    keys[i] = Attribute(String(t));
                t += tree->keyLength;
            }

            memcpy(pointers, t, sizeof(uint) * (size + 1));
		}
		
        void save(char *p)
		{
            char *t = p;
            int i;

            memcpy(t, &size, sizeof(uint));
            t += sizeof(uint);
            memcpy(t, &leaf, sizeof(bool));
            t += sizeof(bool);

            for (i = 0; i < (int)size; i++)
            {
                if (tree->type == INTEGER)
                {
                    Integer tmp = std::get<Integer>(keys[i]);
                    memcpy(t, &tmp, sizeof(Integer));
                }
                else if (tree->type == REAL)
                {
                    Real tmp = std::get<Real>(keys[i]);
                    memcpy(t, &keys[i], sizeof(Real));
                }
                else if (tree->type == STRING)
                {
                    memset(t, 0, sizeof(char) * tree->keyLength);
                    const std::string &tmp = std::get<String>(keys[i]);
                    memcpy(t, tmp.c_str(), sizeof(char) * tmp.length());
                }
                t += tree->keyLength;
            }
            memcpy(t, pointers, sizeof(uint) * (size + 1));
		}
    };

    class BpIterator
    {
    public:

        BpIterator(const BpIterator &x) :
            tree(x.tree),
            p(x.p ? tree->load(x.p->id) : nullptr),
            pos(x.pos)
        {
        }

        BpIterator(BpTree *tree, const BpTree::Key &key) :
            tree(tree)
        {
            auto tmp = tree->find(key);
            p = tmp.first;
            pos = tmp.second;

            while (p && p->pointers[pos + 1] == (uint)-1)
                step();
        }

        explicit BpIterator(BpTree *tree, bool head = false):
            tree(tree),
            p(head ? tree->load(1) : nullptr),
            pos(head ? 0 : -1)
        {
            while (p && p->pointers[pos + 1] == (uint)-1)
                step();
        }

        ~BpIterator()
        {
            if (p != nullptr)
                delete p;
        }

        bool operator != (const BpIterator &i) const
        {
            if (p == nullptr)
                return i.p != nullptr;
            else if (i.p == nullptr)
                return true;
            else
                return p->id != i.p->id || pos != i.pos;
        }

        std::pair<Attribute, uint> operator *() const
        {
            if (p == nullptr)
                throw ObjectNotFountException();
            return { p->keys[pos], p->pointers[pos + 1] };
        }

        const BpIterator& operator ++()
        {
            do step();
            while (p && p->pointers[pos + 1] == (uint)-1);
            return *this;
        }

    private:
        
        void step()
        {
            pos++;
            if (pos == p->size)
            {
                uint nid = p->pointers[0];   // Linked listed pointer
                delete p;
                if (nid == 0)
                    p = nullptr;
                else
                {
                    p = tree->load(nid);
                    pos = 0;
                }
            }
        }

        BpTree *tree;
        BpTree::Node *p;
        int pos;
    };
}