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

CollectError collectError;//�����ռ���
GreenLayer greenLayer;
BlueLayer blueLayer;
BrownLayer brownLayer;
BlackLayer blackLayer;

ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer);
ASErr CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum]);
ASErr GenerateReport(CollectError collectError);

AIBoolean flag;//��ǰ���ĸ�����,0��ʾѡ����󴰿ڣ�1��ʾ�����б���
AIBoolean flagPB;//���ĸ�������,0��ʾ������ͼ��1��ʾ������
HWND progressBarAnalyse;//������ͼ������
ai::int32 progressAnalyse;//������ͼ�Ľ���
HWND progressBarCheck;//�����������
ai::int32 progressCheck;//������Ľ���

HINSTANCE hInstance;//ʵ�����
HWND errList;//�����б���
HWND staticText[ErrorNumMax];//��������
//HWND highLightButton[ErrorNumMax];//����������ʾ��ť
HWND correctedCheckBox[ErrorNumMax];//�����Ƿ����
AIBoolean haveCorrected[ErrorNumMax];//��¼�û��Ѹ����Ĵ���
ai::int32 saveVscrollPos;//��¼������λ��

HWND checkErrWnd;//�����󴰿�
HWND errTypeCheckBox[ErrorTypeNum];//���������͸�ѡ��
HWND selectAll,deselectAll,check;//ȫѡ��ȡ��ȫѡ����ʼ���
AIBoolean errorSelectedByUser[ErrorTypeNum];//��ѡ��������û�ѡ��Ҫ���еĴ�����
AIBoolean errorCheckedByUser[ErrorTypeNum];//�û��Ѿ������Ĵ���
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

	//��Ӳ˵�
	ASErr AddMenus(SPInterfaceMessage* message);
	//������ѡ�˵���Ĺ���
	ASErr GoMenuItem(AIMenuMessage* message);

	//AIMenuItemHandle fChooseMapMenu;//�û�ѡ����Ҫ���ĵ�ͼ�������������ļ�ѡ�񴰿�
	AIMenuItemHandle fAnalyseMapMenu;//��ȡ��ͼ���ݣ���Ҫ��ӽ�����
	AIMenuItemHandle fCheckErrorMenu;//����ͼ����Ҫ���û����棬�縴ѡ��ѡ����Ҫ���Ĵ���
	AIMenuItemHandle fDisplayErrorMenu;//���ִ�����Ϣ��������������ʾ����
	AIMenuItemHandle fGenerateReportMenu;//���ɴ��󱨸�
};

#endif
