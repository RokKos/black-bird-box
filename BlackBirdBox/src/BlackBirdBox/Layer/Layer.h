#pragma once

#include "BlackBirdBox/Core/TimeStep.h"
#include "BlackBirdBox/Events/Event.h"

namespace BlackBirdBox {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(TimeStep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

}
