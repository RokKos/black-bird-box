#pragma once

namespace BlackBirdBox {

class BaseMenu {
public:
    BaseMenu(const std::string& menu_name = "Menu");
    virtual ~BaseMenu() = default;

    virtual void OnImGuiRender();

protected:
    std::string menu_name_;
};
}
