#pragma once

#include "wcap.h"


namespace wcap
{

class TDummyWindow {
public:
    TDummyWindow(const wcap::tstring& caption);
    ~TDummyWindow();
    operator HWND();
    operator bool();
private:
    wcap::tstring Caption;
    HWND Hwnd;
};

}//namespace wcap
