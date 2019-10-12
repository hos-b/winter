#include "tests/test.h"

namespace winter
{
namespace test
{
    
TestMenu::TestMenu(Test*& current_test_pointer) : current_test_(current_test_pointer)
{

}
void TestMenu::OnImGuiRender()
{
    for( auto& test : tests) 
    {
        if(ImGui::Button(test.first.c_str()))
            current_test_ = test.second();
    }
}

} // namespace test
} // namespace winter