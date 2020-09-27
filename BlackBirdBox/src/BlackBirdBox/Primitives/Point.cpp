#include "bbbpch.h"
#include "Point.h"

namespace BlackBirdBox {

Point::Point(float size, glm::vec3 position, glm::vec3 color)
    : size_(size)
    , position_(position)
    , color_(color)
{
    PROFILE_FUNCTION();
}

}
