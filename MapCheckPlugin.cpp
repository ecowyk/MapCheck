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

//��Ӳ˵�
ASErr MapCheckPlugin::AddMenus(SPInterfaceMessage* message)
{
	ASErr error = kNoErr;

	//�������һ���˵���
	AIPlatformAddMenuItemDataUS throwAwayMenuData;
	throwAwayMenuData.groupName = kWindowUtilsMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("MapCheck");
	AIMenuItemHandle throwAwayAIMenu;
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&throwAwayAIMenu);
	if(error)
		goto error;

	//����Ӳ˵���
	AIMenuGroup throwAwayMenuGroup;
	sAIMenu->AddMenuGroupAsSubMenu(kSecondMenuGroup,0,throwAwayAIMenu,&throwAwayMenuGroup);

	//���Ӳ˵�������Ӳ˵���
	throwAwayMenuData.groupName = kSecondMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("StartCheck");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fGetAreaMenu);
	if(error)
		goto error;
error:
	return error;
}

//������ѡ�˵���Ĺ���
ASErr MapCheckPlugin::GoMenuItem(AIMenuMessage* message)
{
	
	ASErr error = kNoErr;

	ai::int32 layerListCount;//ͼ���б���
	ai::int32 layerCount;//һ��ͼ���б��е�ͼ����

	AILayerList list;//ͼ���б�
	AILayerHandle layer;//ͼ��

	AIArtHandle path;//·��

	
	CollectError collectError;//�����ռ���
	
	//��ȡ·��
	error = sAILayerList->Count(&layerListCount);//��ȡ
	error = sAILayerList->GetFirst(&list);
	error = sAILayerList->CountLayers(list,&layerCount);
	error = sAILayerList->GetFirstLayer(list,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);//��ȡ��һ��·��

	//�ռ��̰���Ϣ
	GreenLayer greenLayer;
	greenLayer.GetPathOfLayer(path);

	//��������ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ�������Ϣ
	BlueLayer blueLayer;
	blueLayer.GetPathOfLayer(path);

	//�����ذ�ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ��ذ���Ϣ
	BrownLayer brownLayer;
	brownLayer.GetPathOfLayer(path);

	//����ڰ�ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ��ڰ���Ϣ
	BlackLayer blackLayer;
	blackLayer.GetPathOfLayer(path);


	//������
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



