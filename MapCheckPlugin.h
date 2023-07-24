#ifndef __MapCheckPlugin_h__
#define __MapCheckPlugin_h__

#include "Plugin.hpp"
#include "MapCheckID.h"
#include "SDKAboutPluginsHelper.h"
#include "SDKDef.h"
#include "AIMenuGroups.h"
#include "Layer.h"




/**	Creates a new MapCheckPlugin.
	@param pluginRef IN unique reference to this plugin.
	@return pointer to new MapCheckPlugin.
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**	Reloads the MapCheckPlugin class state when the plugin is 
	reloaded by the application.
	@param plugin IN pointer to plugin being reloaded.
*/
void FixupReload(Plugin* plugin);

/**	Hooks MapCheck up as an Illustrator plug-in.
	@ingroup MapCheck
*/
class MapCheckPlugin : public Plugin
{
public:
	/** Constructor.
		@param pluginRef IN reference to this plugin.
	*/
	MapCheckPlugin(SPPluginRef pluginRef);

	/** Destructor.
	*/
	virtual ~MapCheckPlugin();
  
  /**	Restores state of MapCheckPlugin during reload.
	*/
	FIXUP_VTABLE_EX(MapCheckPlugin, Plugin);
  
  /**	Initializes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr StartupPlugin( SPInterfaceMessage * message );

	/**	Removes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr ShutdownPlugin( SPInterfaceMessage * message );

	//添加菜单
	ASErr AddMenus(SPInterfaceMessage* message);
	//启动所选菜单项的功能
	ASErr GoMenuItem(AIMenuMessage* message);

	AIMenuItemHandle fGetAreaMenu;

	

};

#endif
