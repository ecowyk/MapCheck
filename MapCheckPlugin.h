#ifndef __MapCheckPlugin_h__
#define __MapCheckPlugin_h__

#include "Plugin.hpp"
#include "MapCheckID.h"
#include "SDKAboutPluginsHelper.h"
#include "SDKDef.h"
#include "windows.h"
#include "Windowsx.h"
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

	//��Ӳ˵�
	ASErr AddMenus(SPInterfaceMessage* message);
	//������ѡ�˵���Ĺ���
	ASErr GoMenuItem(AIMenuMessage* message);

	ASErr ChooseMap();
	ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer);
	ASErr CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum]);
	ASErr GenerateReport(CollectError collectError);

	AIPanelRef fPanel;
	AIPanelPlatformWindow hDlg;
	ASErr AddWidgets();
	static LRESULT CALLBACK NewWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool PanelWindowProc(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CallDefaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WNDPROC fDefaultWindProc;

	AIPanelRef fPanel2;//���ִ����б������õ���
	AIPanelPlatformWindow hDlg2;
	ASErr CreateErrorListDlg(CollectError& collectError);
	ASErr UpdateErrorList(CollectError& collectError);
	HWND errorDescription[ErrorNumMax];
	HWND errorHighlight[ErrorNumMax];
	HWND errorCorrected[ErrorNumMax];
	HWND previousPageButton,nextPageButton;
	static LRESULT CALLBACK NewWindowProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool PanelWindowProc2(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CallDefaultWindowProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WNDPROC fDefaultWindProc2;


	HWND button1,button2,button3;
	HWND checkBoxs[ErrorTypeNum];

	AIMenuItemHandle fChooseMapMenu;//�û�ѡ����Ҫ���ĵ�ͼ�������������ļ�ѡ�񴰿�
	AIMenuItemHandle fAnalyseMapMenu;//��ȡ��ͼ���ݣ���Ҫ��ӽ�����
	AIMenuItemHandle fCheckErrorMenu;//����ͼ����Ҫ���û����棬�縴ѡ��ѡ����Ҫ���Ĵ���
	AIMenuItemHandle fGenerateReportMenu;//���ɴ��󱨸�

	CollectError collectError;//�����ռ���
	GreenLayer greenLayer;
	BlueLayer blueLayer;
	BrownLayer brownLayer;
	BlackLayer blackLayer;
	AIBoolean errorSelectedByUser[ErrorTypeNum];//��ѡ��������û�ѡ��Ҫ���еĴ�����
	AIBoolean errorCheckedByUser[ErrorTypeNum];//�û��Ѿ������Ĵ���
};

#endif
