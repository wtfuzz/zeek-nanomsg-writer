
#ifndef BRO_PLUGIN_THERMITE_NANOMSG
#define BRO_PLUGIN_THERMITE_NANOMSG

#include <plugin/Plugin.h>

namespace plugin {
namespace Thermite_Nanomsg {

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}

#endif
