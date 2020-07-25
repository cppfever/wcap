#pragma once

#include <string>
#include <sstream>
#include <exception>
#include <windows.h>


namespace wcap
{

class SharedMemory
{
public:

    SharedMemory(const char* sharedname, int size)
    {
        /*
        const char* filename;
        DWORD access = GENERIC_READ | GENERIC_WRITE;
        DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE;
        HANDLE hfile = ::CreateFileA(filename, access, share, NULL, 0, 0, NULL);

        if(hfile == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Can't create file.");*/

        m_mapping = ::CreateFileMappingA(
                              INVALID_HANDLE_VALUE,    // use paging file
                              NULL,                    // default security
                              PAGE_READWRITE,          // read/write access
                              0,                       // maximum object size (high-order DWORD)
                              size,                // maximum object size (low-order DWORD)
                              sharedname);

        if(!m_mapping)
            throw std::runtime_error("Can't create file mapping.");

        m_view = ::MapViewOfFile(m_mapping, FILE_MAP_WRITE, 0, 0 ,0);

        if(!m_view)
            throw std::runtime_error("Can't create view file mapping.");

        m_size = size;
    }

    ~SharedMemory()
    {
        if(m_view)
            ::UnmapViewOfFile(m_view);

        if(m_mapping)
            ::CloseHandle(m_mapping);
    }

    char* begin()
    {
        return static_cast<char*>(m_view);
    }

    char* end()
    {
        return static_cast<char*>((char*)m_view + m_size);
    }

    size_t size()
    {
        return m_size;
    }

    operator bool()
    {
        return m_mapping;
    }

private:

    HANDLE m_file = NULL;
    HANDLE m_mapping = NULL;
    LPVOID m_view = NULL;
    int m_size = 0;
};

}//namespace wcap
