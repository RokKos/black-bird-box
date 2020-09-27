#include "EOLApplication.h"

namespace EOL {

EOLApplication::EOLApplication() { PushLayer(new EOLLayer()); }

}

namespace BlackBirdBox {
Application* CreateApplication() { return new EOL::EOLApplication(); }
}
