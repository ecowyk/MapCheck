#ifndef __MapCheckPlugin_h__
#define __MapCheckPlugin_h__

#include "Plugin.hpp"
#include "MapCheckID.h"
#include "SDKAboutPluginsHelper.h"
#include "SDKDef.h"
#include "windows.h"
#include "AIMenuGroups.h"
#include "Layer.h"


const ai::int32 ErrorTypeNum = 13;

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

	ASErr ChooseMap();
	ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer);
	ASErr CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,
		AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum]);
	ASErr GenerateReport(CollectError collectError);

	static LRESULT CALLBACK NewWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool PanelWindowProc(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CallDefaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WNDPROC fDefaultWindProc;
	ASErr AddWidgets();

	AIPanelRef fPanel;
	AIPanelPlatformWindow hDlg;

	HWND button1,button2,checkBox1,checkBox2,checkBox3,checkBox4,checkBox5,checkBox6,
		checkBox7,checkBox8,checkBox9,checkBox10,checkBox11,checkBox12,checkBox13;
	

	AIMenuItemHandle fChooseMapMenu;//用户选择需要检测的地图，点击该项，唤起文件选择窗口
	AIMenuItemHandle fAnalyseMapMenu;//获取地图数据，需要添加进度条
	AIMenuItemHandle fCheckErrorMenu;//检查地图错误，要有用户界面，如复选框，选择需要检测的错误
	AIMenuItemHandle fGenerateReportMenu;//生成错误报告

	CollectError collectError;//错误收集类
	GreenLayer greenLayer;
	BlueLayer blueLayer;
	BrownLayer brownLayer;
	BlackLayer blackLayer;
	AIBoolean errorSelectedByUser[ErrorTypeNum];//复选框面板中用户选择要进行的错误检查
	AIBoolean errorCheckedByUser[ErrorTypeNum];//用户已经检查过的错误
};

#endif
