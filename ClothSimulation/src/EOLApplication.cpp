#include "EOLApplication.h"


namespace EOL {

	EOLApplication::EOLApplication()
	{
		PushLayer(new EOLLayer());
	}
	

}


namespace Core {
	Application* CreateApplication() {
		return new EOL::EOLApplication();
	}
}