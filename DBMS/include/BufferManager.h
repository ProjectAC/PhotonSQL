#pragma once

#include <string>
#include "Definitions.h"

namespace Photon
{
    class BufferManager
    {
    public:

        static BufferManager &getInstance();

        void *get(const std::string &fileName, uint id);

    private:

        static BufferManager *instance;
        
        class Buffer
        {
        public:
            void load(const std::string &fileName, uint id);
            void *content();
            Buffer();
            ~Buffer();

        private:
            std::string fileName;
            uint id;
            void *buffer;
        };

    };
}