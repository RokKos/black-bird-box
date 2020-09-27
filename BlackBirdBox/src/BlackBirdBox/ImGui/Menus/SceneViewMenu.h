#pragma once

#include "BaseMenu.h"
#include "BlackBirdBox/Primitives/LightSource.h"
#include "BlackBirdBox/Primitives/Shape.h"

namespace BlackBirdBox {

class SceneViewMenu : public BaseMenu {
public:
    SceneViewMenu(const std::string& menu_name, const std::vector<Ref<Shape>>& shapes, const std::vector<Ref<LightSource>>& lights);

    virtual void OnImGuiRender() override;

private:
    const std::vector<Ref<Shape>>& shapes_;
    const std::vector<Ref<LightSource>>& lights_;
};
}
