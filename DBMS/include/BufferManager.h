#pragma once

#include <string>
#include <unordered_map>
#include <array>
#include <fstream>
#include "Definitions.h"

namespace Photon
{
    class BufferManager
    {
    public:
        static const uint SIZE = 4096;

        static BufferManager &getInstance();
        BufferManager();

        byte *get(const std::string &fileName, uint id);

    private:

        static BufferManager *instance;
        
        class FileBuffer
        {
        public:

            class BufferUnit
            {
            public:
                void load(uint id);
                void save();

                byte *content();
                uint ID();

#if defined LRUBUFFER
                void count();
                uint getCount();
#endif

                BufferUnit(std::fstream &stream);
                ~BufferUnit();

            private:
                std::fstream &file;
                uint id;
                byte *buffer;

#if defined LRUBUFFER
                uint counter;
#endif
            };
            
            FileBuffer(std::string fileName);
            ~FileBuffer();

            byte *get(uint id);

        private:

            std::fstream file;
            std::array<BufferUnit*, 16> buffers;
        };
        
        std::unordered_map<std::string, FileBuffer> files;
    };
}