#include "bbbpch.h"
#include "GraphVertex.h"

namespace BlackBirdBox {

GraphVertex::GraphVertex()
    : color_(-1)
{
    PROFILE_FUNCTION();
}

void GraphVertex::SetConnections(const std::vector<Ref<GraphVertex>>& connections)
{
    PROFILE_FUNCTION();

    connections_ = connections;
}

void GraphVertex::SelectColor()
{
    PROFILE_FUNCTION();

    for (int color = 0; color < connections_.size() + 1; ++color) {
        bool color_already_used = false;
        for (int i = 0; i < connections_.size(); ++i) {
            if (color == connections_[i]->color_) {
                color_already_used = true;
                break;
            }
        }
        if (!color_already_used) {
            color_ = color;
            return;
        }
    }

    color_ = connections_.size() + 1;
}

}
