#ifndef __MapCheckPlugin_h__
#define __MapCheckPlugin_h__

#include "Plugin.hpp"
#include "MapCheckID.h"
#include "SDKAboutPluginsHelper.h"
#include "SDKDef.h"
#include "AIMenuGroups.h"
#include "Layer.h"

#include "windows.h"
#include "commctrl.h"

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

CollectError collectError;//错误收集类
GreenLayer greenLayer;
BlueLayer blueLayer;
BrownLayer brownLayer;
BlackLayer blackLayer;

ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer);
ASErr CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum]);
ASErr GenerateReport(CollectError collectError);

AIBoolean flag;//当前是哪个窗口,0表示选择错误窗口，1表示错误列表窗口
AIBoolean flagPB;//用哪个进度条,0表示分析地图，1表示检查错误
HWND progressBarAnalyse;//分析地图进度条
ai::int32 progressAnalyse;//分析地图的进度
HWND progressBarCheck;//检查错误进度条
ai::int32 progressCheck;//检查错误的进度

HINSTANCE hInstance;//实例句柄
HWND errList;//错误列表窗口
HWND staticText[ErrorNumMax];//错误描述
//HWND highLightButton[ErrorNumMax];//高亮居中显示按钮
HWND correctedCheckBox[ErrorNumMax];//错误是否改正
AIBoolean haveCorrected[ErrorNumMax];//记录用户已改正的错误
ai::int32 saveVscrollPos;//记录滚动条位置

HWND checkErrWnd;//检测错误窗口
HWND errTypeCheckBox[ErrorTypeNum];//检测错误类型复选框
HWND selectAll,deselectAll,check;//全选，取消全选，开始检测
AIBoolean errorSelectedByUser[ErrorTypeNum];//复选框面板中用户选择要进行的错误检查
AIBoolean errorCheckedByUser[ErrorTypeNum];//用户已经检查过的错误
LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);
int RegisterErrList(HINSTANCE hInstance, HWND hwnd);
int RegisterCheckErrWnd(HINSTANCE hInstance, HWND hwnd);
void CreateProgressBar(HINSTANCE hInstance, HWND hwnd);
DWORD WINAPI PBThreadProc(LPVOID lpParameter);
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

	//AIMenuItemHandle fChooseMapMenu;//用户选择需要检测的地图，点击该项，唤起文件选择窗口
	AIMenuItemHandle fAnalyseMapMenu;//获取地图数据，需要添加进度条
	AIMenuItemHandle fCheckErrorMenu;//检查地图错误，要有用户界面，如复选框，选择需要检测的错误
	AIMenuItemHandle fDisplayErrorMenu;//呈现错误信息，高亮并居中显示错误
	AIMenuItemHandle fGenerateReportMenu;//生成错误报告
};

#endif
