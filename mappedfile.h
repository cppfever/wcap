#pragma once

#include <exception>
#include <windows.h>

namespace wcap
{

class SharedMemory
{
public:

    MappedFile(const char* filename, DWORD access = GENERIC_READ | GENERIC_WRITE, DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE)
    {
        m_mapping = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0, )
    }

    ~MappedFile()
    {

    }

    operator bool()
    {
        return m_mapping;
    }

private:

    HANDLE m_file = NULL;
    HANDLE m_mapping = NULL;
};

}//namespace wcap
