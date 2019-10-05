#ifndef __TEST_CLEARCOLOR_H__
#define __TEST_CLEARCOLOR_H__

#include "test.h"

namespace test
{
    class ClearColorTest : public Test
    {
    public:
        ClearColorTest();
        void OnRender();
        void OnImGuiRender();
    private:
        float color_[4];
    };
}

#endif