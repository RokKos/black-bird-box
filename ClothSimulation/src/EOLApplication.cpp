#include "EOLApplication.h"


namespace EOL {

	EOLApplication::EOLApplication()
	{
		general_setting_ = Core::CreateRef<GeneralSettings>();

		PushLayer(new EOLLayer(general_setting_));
	}
	

}


namespace Core {
	Application* CreateApplication() {
		return new EOL::EOLApplication();
	}
}