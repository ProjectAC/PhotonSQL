#include "../include/BufferManager.h"

using namespace std;

namespace Photon
{
    ///////////// BufferManager /////////////
    BufferManager *BufferManager::instance = nullptr;

    BufferManager & BufferManager::getInstance()
    {
        return *BufferManager::instance;
    }

    BufferManager::BufferManager()
    {
        if (instance == nullptr)
            instance = this;
    }

    BufferManager::~BufferManager()
    {
        for (auto &f : files)
            delete f.second;
    }

    byte *BufferManager::get(const std::string &fileName, uint id)
    {
        if (files.find(fileName) == files.end())
            files[fileName] = new FileBuffer(fileName);
        return files[fileName]->get(id);
    }

    ///////////// FileBuffer /////////////

    BufferManager::FileBuffer::FileBuffer(std::string fileName)
    {
        file.open(fileName, ios::app);
        file.close();
        file.open(fileName, ios::in | ios::out | ios::ate | ios::binary);
        for (auto &b : buffers)
            b = new BufferUnit(file);
    }

    BufferManager::FileBuffer::~FileBuffer()
    {
        for (auto &b : buffers)
            delete b;
        file.close();
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
		file.seekg(0, ios::end);
		size_t filesize = file.tellg();
        
        if (filesize == (size_t)-1)
            filesize = 0;

		if (filesize >= id * SIZE + SIZE) {
			file.seekp(id * SIZE, ios::beg);
			file.read(buffer, SIZE);
		}
		else if (filesize < id * SIZE + SIZE && filesize > id * SIZE) {
            memset(buffer, 0, SIZE);
			file.seekp(id * SIZE, ios::beg);
			file.read(buffer, (filesize - id * SIZE));
		}
		else {
            memset(buffer, 0, SIZE);
		}
		
        this->id = id;
#if defined LRUBUFFER
        counter = 0;
#endif
    }

    void BufferManager::FileBuffer::BufferUnit::save()
    {
		file.seekg(0, ios::end);
		size_t filesize = file.tellg();
		
		if (filesize >= id * SIZE) 
			file.seekg(id * SIZE, ios::beg);
        else
        {
            uint length = id * SIZE - filesize;
            char *p = new char[length];
            file.seekg(0, ios::end);
            file.write(p, length);
            file.seekg(0, ios::end);
            delete[] p;
        }
		
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

#if defined LRUBUFFER
    void BufferManager::FileBuffer::BufferUnit::count()
    {
        counter = counter + 1;
    }

    uint BufferManager::FileBuffer::BufferUnit::getCount()
    {
        return counter;
    }
#endif

    BufferManager::FileBuffer::BufferUnit::BufferUnit(std::fstream &stream) :
        file(stream)
    {
        buffer = new byte[SIZE];
        id = -1;
    }

    BufferManager::FileBuffer::BufferUnit::~BufferUnit()
    {
        if (id != uint(-1))
            save();
        delete[] buffer;
    }

}
