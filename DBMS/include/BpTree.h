#pragma once

#include <vector>

namespace Photon
{
    template<class Key, class Value>
    class BpTree
    {
    public:

        void insert(Key key, Value value);
        void erase(Key key);

    private:

    };
}