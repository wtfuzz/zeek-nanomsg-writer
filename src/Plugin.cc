
#include "Plugin.h"
#include "Nanomsg.h"

namespace plugin { namespace Thermite_Nanomsg { Plugin plugin; } }

using namespace plugin::Thermite_Nanomsg;

plugin::Configuration Plugin::Configure()
	{
  AddComponent(new ::logging::Component("Nanomsg", ::logging::writer::Nanomsg::Instantiate));

	plugin::Configuration config;
	config.name = "Thermite::Nanomsg";
	config.description = "Nanomsg Log Writer";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
	}
