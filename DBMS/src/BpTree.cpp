#include "../include/BpTree.h"
#include <stack>

using namespace std;

namespace Photon
{
    void BpTree::debug()
    {
        printf("\nNode Count %u, Degree %u, Root %d\n", nodeCount, degree, root);
        printf("\n----------------------------------\n");
        for (int i = 1; i <= nodeCount; i++)
        {
            Node *p = load(i);

            printf("Node #%d:\n", i);
            printf("Keys    : ");
            for (int j = 0; j < p->size; j++)
                printf("%s ", get<string>(p->keys[j]).c_str());
            printf("\nPointers: ");
            for (int j = 0; j <= p->size; j++)
                printf("%u ", p->pointers[j]);
            printf("\n----------------------------------\n");
        }
        printf("\n");
    }

    BpTree::BpTree(std::string name) :
        name(name)
    {
        char *p = BufferManager::getInstance().get(name, 0);
        memcpy(&root, p, sizeof(root)); p += sizeof(root);
        memcpy(&type, p, sizeof(type)); p += sizeof(type);
        memcpy(&keyLength, p, sizeof(keyLength)); p += sizeof(keyLength);
        memcpy(&nodeCount, p, sizeof(nodeCount));

        degree = (BufferManager::SIZE - 9) / (keyLength + sizeof(uint));
    }

    BpTree::BpTree(std::string name, AttributeType type, uint keyLength):
        name(name),
        type(type),
        keyLength(keyLength)
    {
        nodeCount = 1;
        degree = (BufferManager::SIZE - 9) / (keyLength + sizeof(uint));
        root = 1;

        save();
        auto node = new Node(this, 1, true);
        node->save(BufferManager::getInstance().get(name, 1));
        delete node;
    }

    BpTree::~BpTree()
    {
        save();
    }

    void BpTree::save()
    {
        char *p = BufferManager::getInstance().get(name, 0);
        memcpy(p, &root, sizeof(root)); p += sizeof(root);
        memcpy(p, &type, sizeof(type)); p += sizeof(type);
        memcpy(p, &keyLength, sizeof(keyLength)); p += sizeof(keyLength);
        memcpy(p, &nodeCount, sizeof(nodeCount));
    }

    void BpTree::insert(const Key &key, const Value &value)
    {
        auto s = finds(key);
        Key k = key;
        uint v = value;
        int pos;
        Node *cur = nullptr, *p = nullptr;
        
        while (!s.empty())
        {
            cur = s.top().first;
            pos = s.top().second;
            s.pop();

            auto tmp = cur->insert(pos + 1, k, v, nodeCount);
            p = tmp.first;

            cur->save(BufferManager::getInstance().get(name, cur->id));
            delete cur;

            if (p == nullptr)
                break;
            k = tmp.second;
            v = p->id;

            p->save(BufferManager::getInstance().get(name, p->id));
            delete p;
        }

        // New root
        if (p != nullptr)
        {
            Node *r = new Node(this, ++nodeCount, false);
            r->size = 1;
            r->keys[0] = k;
            r->pointers[0] = root;
            r->pointers[1] = v;
            root = r->id;
            r->save(BufferManager::getInstance().get(name, r->id));
            delete r;
        }
    }

    void BpTree::erase(const Key &key)
    {
        auto tmp = find(key);
        Node *p = tmp.first;
        int pos = tmp.second;
        if (0 <= pos && pos < p->size && p->keys[pos] == key)
        {
            p->pointers[pos + 1] = (uint)-1;
            p->save(BufferManager::getInstance().get(name, p->id));
        }
        delete p;
    }

    stack<pair<BpTree::Node*, int> > BpTree::finds(const BpTree::Key &key)
    {
        uint cur = root;
        int pos;
        stack<pair<Node*, int> > res;
        Node *p;

        while (true)
        {
            p = load(cur);

            for (pos = 0; pos < (int)p->size && p->keys[pos] <= key; pos++);
            pos--;

            res.push({p, pos});
            if (p->leaf == true)
                break;
            else
                cur = p->pointers[pos + 1];
        }

        return res;
    }

    pair<BpTree::Node*, int> BpTree::find(const Key &key)
    {
        uint cur = root;
        int pos;
        Node *p;

        while (true)
        {
            p = load(cur);

            for (pos = 0; pos < (int)p->size && !(key < p->keys[pos]); pos++)
                ;
            pos--;

            if (p->leaf == true)
                break;
            else
                cur = p->pointers[pos + 1];

            delete p;
        }

        return { p, pos };
    }

    BpTree::Node *BpTree::load(uint nodeId)
    {
        auto node = new Node(this, nodeId, false);
		node->load(BufferManager::getInstance().get(name, nodeId), type);
        return node;
    }
}