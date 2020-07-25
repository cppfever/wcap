#include "wcap.h"
#include "dxoffsets.h"
#include "inject.h"
//#include "sharedmemory.h"


int main(int argc, char **argv)
{
 /*   uint64_t present = 0;
    uint64_t presentex = 0;
    wcap::GetDX9Offsets(present, presentex);

  /*  wcap::SharedMemory offsets_map("offsets", 64);
    char* presentstr = offsets_map.begin();
    char* presentexstr = presentstr + 32;
*/
 /*   char presentstr[32];
    char presentexstr[32];

    std::stringstream ss1;
    ss1 << present;
    ss1 >> presentstr;

    std::stringstream ss2;
    ss2 << presentex;
    ss2 >> presentexstr;

    std::cout << "Present offset: " << presentstr << std::endl;
    std::cout << "PresentEx offset: " << presentexstr << std::endl;

    std::ofstream file("offsets.txt");
    file << present << std::endl << presentex << std::endl;
*/
    int pid = wcap::GetProcessID(_T("KMPlayer.exe"));
    if(!wcap::InjectDll(pid, _T("libwcapdll.dll")))
        throw std::runtime_error("Can't inject dll.");
    else
        std::cout << "dll injected" << std::endl;

    getchar();

    return 0;
}
