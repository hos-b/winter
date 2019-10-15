#ifndef __GL_F_TEST__
#define __GL_F_TEST__


#include <vector>
#include <string>
#include <functional>
#include <imgui/imgui.h>
#include "framework/util/window.h"

namespace winter
{
namespace test
{

class Test
{
public:
    Test() {};
    virtual ~Test() {};

    virtual void OnUpdate(float delta_time) {(void)delta_time;}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
    // window management
    virtual void SetWindowReference(util::Window *window) { window_ = window; }
    util::Window* window() { return window_; }
private:
    // window reference
    util::Window *window_ = nullptr;
};
class TestMenu : public Test
{
public:
    TestMenu(Test*& current_test_pointer);
    void OnImGuiRender() override;

    template<typename T>
    void RegisterTest(const std::string& name)
    {
        tests.emplace_back(std::make_pair(name, []() {return new T();} ));
    }

private:
    // reference to pointer
    Test*& current_test_;
    std::vector<std::pair<std::string, std::function<Test*()>>> tests;
};

} // namespace test
} // namespace winter
 
#endif