#include "../include/BufferManager.h"

using namespace std;

namespace Photon
{
    ///////////// BufferManager /////////////

    BufferManager & BufferManager::getInstance()
    {
        return *BufferManager::instance;
    }

    BufferManager::BufferManager()
    {
        BufferManager::instance = this;
    }

    byte *BufferManager::get(const std::string &fileName, uint id)
    {
        return files[fileName].get(id);
    }

    ///////////// FileBuffer /////////////

    BufferManager::FileBuffer::FileBuffer(std::string fileName) :
        file(fileName)
    {
        for (auto &b : buffers)
            b = new BufferUnit(file);
    }

    BufferManager::FileBuffer::~FileBuffer()
    {
        for (auto &b : buffers)
            delete b;
    }

#if defined LRUBUFFER
#else
    byte *BufferManager::FileBuffer::get(uint id)
    {
        auto &buffer = buffers[id & 0xF];

        if (buffer->ID() != id)
            buffer->load(id);

        return buffer->content();
    }
#endif

    ///////////// BufferUnit /////////////

    void BufferManager::FileBuffer::BufferUnit::load(uint id)
    {
        file.read(buffer, SIZE);
    }

    void BufferManager::FileBuffer::BufferUnit::save()
    {
        file.write(buffer, SIZE);
    }

    byte *BufferManager::FileBuffer::BufferUnit::content()
    {
        return buffer;
    }

    uint BufferManager::FileBuffer::BufferUnit::ID()
    {
        return id;
    }

    void BufferManager::FileBuffer::BufferUnit::count()
    {
        counter = counter + 1;
    }

    uint BufferManager::FileBuffer::BufferUnit::getCount()
    {
        return counter;
    }

    BufferManager::FileBuffer::BufferUnit::BufferUnit(std::fstream &stream) :
        file(stream)
    {
        buffer = new byte[SIZE];
    }

    BufferManager::FileBuffer::BufferUnit::~BufferUnit()
    {
        delete[] buffer;
    }

}