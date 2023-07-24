#include "IllustratorSDK.h"
#include "MapCheckPlugin.h"
#include "MapCheckSuites.h"


Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new MapCheckPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	MapCheckPlugin::FixupVTable((MapCheckPlugin*) plugin);
}

MapCheckPlugin::MapCheckPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef)
{
	strncpy(fPluginName, kMapCheckPluginName, kMaxStringLength);
}

MapCheckPlugin::~MapCheckPlugin()
{
}

ASErr MapCheckPlugin::StartupPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
	error = Plugin::StartupPlugin(message);
	error = this->AddMenus(message);
	return error;
}

ASErr MapCheckPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;

	error = Plugin::ShutdownPlugin(message);
	return error;
}

//添加菜单
ASErr MapCheckPlugin::AddMenus(SPInterfaceMessage* message)
{
	ASErr error = kNoErr;

	//首先添加一个菜单项
	AIPlatformAddMenuItemDataUS throwAwayMenuData;
	throwAwayMenuData.groupName = kWindowUtilsMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("MapCheck");
	AIMenuItemHandle throwAwayAIMenu;
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&throwAwayAIMenu);
	if(error)
		goto error;

	//添加子菜单组
	AIMenuGroup throwAwayMenuGroup;
	sAIMenu->AddMenuGroupAsSubMenu(kSecondMenuGroup,0,throwAwayAIMenu,&throwAwayMenuGroup);

	//向子菜单组内添加菜单项
	throwAwayMenuData.groupName = kSecondMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("StartCheck");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fGetAreaMenu);
	if(error)
		goto error;
error:
	return error;
}

//启动所选菜单项的功能
ASErr MapCheckPlugin::GoMenuItem(AIMenuMessage* message)
{
	
	ASErr error = kNoErr;

	ai::int32 layerListCount;//图层列表数
	ai::int32 layerCount;//一个图层列表中的图层数

	AILayerList list;//图层列表
	AILayerHandle layer;//图层

	AIArtHandle path;//路径

	
	CollectError collectError;//错误收集类
	
	//获取路径
	error = sAILayerList->Count(&layerListCount);//获取
	error = sAILayerList->GetFirst(&list);
	error = sAILayerList->CountLayers(list,&layerCount);
	error = sAILayerList->GetFirstLayer(list,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);//获取第一条路径

	//收集绿版信息
	GreenLayer greenLayer;
	greenLayer.GetPathOfLayer(path);

	//进入蓝版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集蓝版信息
	BlueLayer blueLayer;
	blueLayer.GetPathOfLayer(path);

	//进入棕版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集棕版信息
	BrownLayer brownLayer;
	brownLayer.GetPathOfLayer(path);

	//进入黑版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集黑版信息
	BlackLayer blackLayer;
	blackLayer.GetPathOfLayer(path);


	//检测错误
	greenLayer.CheckError1(collectError);

	greenLayer.CheckError2(collectError);
	
	greenLayer.CheckError3(blackLayer,collectError);

	blackLayer.CheckError4(greenLayer,collectError);

	blackLayer.CheckError5(collectError);

	blackLayer.CheckError6(blueLayer,collectError);

	brownLayer.CheckError7(blueLayer,collectError);

	brownLayer.CheckError8(blueLayer,collectError);

	blackLayer.CheckError9(collectError);

	blackLayer.CheckError10(collectError);

	blueLayer.CheckError11(blackLayer,brownLayer,collectError);

	blueLayer.CheckError12(blackLayer,collectError);

	blackLayer.CheckError13(collectError);



	collectError.PrintErrorInfo();
	collectError.SetErrorRed();
	
	
	return error;


}



