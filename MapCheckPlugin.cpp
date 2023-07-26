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
	Plugin(pluginRef),
	fPanel(NULL),
	hDlg(NULL)
{
	strncpy(fPluginName, kMapCheckPluginName, kMaxStringLength);
	fDefaultWindProc = NULL;
}

MapCheckPlugin::~MapCheckPlugin()
{
}

ASErr MapCheckPlugin::StartupPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
	error = Plugin::StartupPlugin(message);
	error = this->AddMenus(message);

	for(ai::int32 i = 0 ;i<ErrorTypeNum;i++)
	{
		errorSelectedByUser[i] = 0;
		errorCheckedByUser[i] = 0;
	}
	

	AddWidgets();
	return error;
}

ASErr MapCheckPlugin::AddWidgets()
{
	//创建面板
	ASErr error = kNoErr;
	AISize pnSize = {240, 320};
	error = sAIPanel->Create(fPluginRef, ai::UnicodeString("MapCheck"),ai::UnicodeString("MapCheck"),3,pnSize,true,NULL,this,fPanel);
	if(error)
		return error;

	AISize size = {600,600};
	error = sAIPanel->SetSize(fPanel,size);
	error = sAIPanel->Show(fPanel,true);

	//创建复选框
	AIPanelPlatformWindow hDlg = NULL;
	error = sAIPanel->GetPlatformWindow(fPanel, hDlg);

	HWND text = CreateWindowEx(0,"Static","请选择需要检测的错误",WS_VISIBLE|WS_CHILD|WS_BORDER,200,10,200,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	button1 = CreateWindowEx(0,"BUTTON","确定",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,50,500,100,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	button2 = CreateWindowEx(0,"BUTTON","取消",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,450,500,100,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox1 = CreateWindowEx(0,"BUTTON","Error1",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,50,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox2 = CreateWindowEx(0,"BUTTON","Error2",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,100,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox3 = CreateWindowEx(0,"BUTTON","Error3",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,150,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox4 = CreateWindowEx(0,"BUTTON","Error4",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,200,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox5 = CreateWindowEx(0,"BUTTON","Error5",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,250,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox6 = CreateWindowEx(0,"BUTTON","Error6",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,300,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox7 = CreateWindowEx(0,"BUTTON","Error7",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,350,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox8 = CreateWindowEx(0,"BUTTON","Error8",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,50,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox9 = CreateWindowEx(0,"BUTTON","Error9",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,100,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox10 = CreateWindowEx(0,"BUTTON","Error10",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,150,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox11 = CreateWindowEx(0,"BUTTON","Error11",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,200,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox12 = CreateWindowEx(0,"BUTTON","Error12",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,250,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBox13 = CreateWindowEx(0,"BUTTON","Error13",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,300,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);


	::SetPropA(hDlg, "TPNL", this);
	fDefaultWindProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hDlg, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MapCheckPlugin::NewWindowProc)));

	return error;
}

LRESULT CALLBACK MapCheckPlugin::NewWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MapCheckPlugin* panel = (MapCheckPlugin*)GetPropA(hWnd, "TPNL");
	LRESULT result = 0;
	bool handled = panel->PanelWindowProc(result, hWnd, msg, wParam, lParam);
	if(!handled)
		result =  panel->CallDefaultWindowProc(hWnd, msg, wParam, lParam);
	return result;
}

bool MapCheckPlugin::PanelWindowProc(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	switch (msg) 
	{
	case WM_COMMAND: 
		{
			if((HWND)lParam == button1)
			{
				/*
				MessageBox(NULL,"button1","提示",MB_OK);
				ai::UnicodeString temp;
				sAIUser->AIRealToString(errorSelectedByUser[0],0,temp);
				sAIUser->MessageAlert(temp);
				sAIUser->AIRealToString(errorSelectedByUser[1],0,temp);
				sAIUser->MessageAlert(temp);*/
				CheckError(greenLayer,blueLayer,brownLayer,blackLayer,collectError,errorSelectedByUser,errorCheckedByUser);
				for(ai::int32 i = 0;i<ErrorTypeNum;i++)
				{
					if(errorCheckedByUser[i] == 0&&errorSelectedByUser[i] == 1)
					{
						errorCheckedByUser[i] = 1;
					}
				}
			}
			if((HWND)lParam == button2)
			{
				
			}
			if((HWND)lParam == checkBox1)
			{
				errorSelectedByUser[0] = (int)SendMessage(checkBox1,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox2)
			{
				errorSelectedByUser[1] = (int)SendMessage(checkBox2,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox3)
			{
				errorSelectedByUser[2] = (int)SendMessage(checkBox3,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox4)
			{
				errorSelectedByUser[3] = (int)SendMessage(checkBox4,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox5)
			{
				errorSelectedByUser[4] = (int)SendMessage(checkBox5,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox6)
			{
				errorSelectedByUser[5] = (int)SendMessage(checkBox6,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox7)
			{
				errorSelectedByUser[6] = (int)SendMessage(checkBox7,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox8)
			{
				errorSelectedByUser[7] = (int)SendMessage(checkBox8,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox9)
			{
				errorSelectedByUser[8] = (int)SendMessage(checkBox9,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox10)
			{
				errorSelectedByUser[9] = (int)SendMessage(checkBox10,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox11)
			{
				errorSelectedByUser[10] = (int)SendMessage(checkBox11,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox12)
			{
				errorSelectedByUser[11] = (int)SendMessage(checkBox12,BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox13)
			{
				errorSelectedByUser[12] = (int)SendMessage(checkBox13,BM_GETCHECK,0,0);
			}
		}
	}
	return handled;
}

LRESULT CALLBACK MapCheckPlugin::CallDefaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(fDefaultWindProc)
		return ::CallWindowProc(fDefaultWindProc, hWnd, msg, wParam, lParam);
	else
	{
		return 0; 
	}
}

ASErr MapCheckPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;

	if(fPanel)
	{
		error = sAIPanel->Destroy(fPanel);
		fPanel = NULL;
	}

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
	throwAwayMenuData.itemText = ai::UnicodeString("ChooseMap");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fChooseMapMenu);
	if(error)
		goto error;

	throwAwayMenuData.groupName = kSecondMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("AnalyseMap");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fAnalyseMapMenu);
	if(error)
		goto error;

	throwAwayMenuData.groupName = kSecondMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("CheckError");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fCheckErrorMenu);
	if(error)
		goto error;

	throwAwayMenuData.groupName = kSecondMenuGroup;
	throwAwayMenuData.itemText = ai::UnicodeString("GenerateReoprt");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fGenerateReportMenu);
	if(error)
		goto error;
error:
	return error;
}

//启动所选菜单项的功能
ASErr MapCheckPlugin::GoMenuItem(AIMenuMessage* message)
{
	
	ASErr error = kNoErr;

    if(message->menuItem == fChooseMapMenu)
	{
		ChooseMap();
	}
	else if(message->menuItem == fAnalyseMapMenu)
	{
		AnalyseMap(greenLayer,blueLayer,brownLayer,blackLayer);
	}
	else if(message->menuItem == fCheckErrorMenu)
	{
		/*CheckError(greenLayer,blueLayer,brownLayer,blackLayer,collectError);*/
		if(fPanel)
		{
			AIBoolean isShown = false;
			error = sAIPanel->IsShown(fPanel, isShown);
			error = sAIPanel->Show(fPanel, !isShown);
		}
	}
	else if(message->menuItem == fGenerateReportMenu)
	{
		GenerateReport(collectError);
    }
	
	return error;


}

ASErr MapCheckPlugin::ChooseMap()
{
	ASErr error = kNoErr;

	return error;
}

ASErr MapCheckPlugin::AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer)
{
	ASErr error = kNoErr;

	ai::int32 layerListCount;//图层列表数
	ai::int32 layerCount;//一个图层列表中的图层数

	AILayerList list;//图层列表
	AILayerHandle layer;//图层

	AIArtHandle path;//路径
	
	//获取路径
	error = sAILayerList->Count(&layerListCount);//获取
	error = sAILayerList->GetFirst(&list);
	error = sAILayerList->CountLayers(list,&layerCount);
	error = sAILayerList->GetFirstLayer(list,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);//获取第一条路径

	//收集绿版信息
	greenLayer.GetPathOfLayer(path);

	//进入蓝版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集蓝版信息
	blueLayer.GetPathOfLayer(path);

	//进入棕版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集棕版信息
	brownLayer.GetPathOfLayer(path);

	//进入黑版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集黑版信息
	blackLayer.GetPathOfLayer(path);

	return error;
}

ASErr MapCheckPlugin::CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum])
{
	ASErr error = kNoErr;

	if(errorSelectedByUser[0]==1&&errorCheckedByUser[0]==0)
	{
		greenLayer.CheckError1(collectError);
	}
	if(errorSelectedByUser[1]==1&&errorCheckedByUser[1]==0)
	{
		greenLayer.CheckError2(collectError);
	}
	if(errorSelectedByUser[2]==1&&errorCheckedByUser[2]==0)
	{
		greenLayer.CheckError3(blackLayer,collectError);
	}
	if(errorSelectedByUser[3]==1&&errorCheckedByUser[3]==0)
	{
		blackLayer.CheckError4(greenLayer,collectError);
	}
	if(errorSelectedByUser[4]==1&&errorCheckedByUser[4]==0)
	{
		blackLayer.CheckError5(collectError);
	}
	if(errorSelectedByUser[5]==1&&errorCheckedByUser[5]==0)
	{
		blackLayer.CheckError6(blueLayer,collectError);
	}
	if(errorSelectedByUser[6]==1&&errorCheckedByUser[6]==0)
	{
		brownLayer.CheckError7(blueLayer,collectError);
	}
	if(errorSelectedByUser[7]==1&&errorCheckedByUser[7]==0)
	{
		brownLayer.CheckError8(blueLayer,collectError);
	}
	if(errorSelectedByUser[8]==1&&errorCheckedByUser[8]==0)
	{
		blackLayer.CheckError9(collectError);
	}
	if(errorSelectedByUser[9]==1&&errorCheckedByUser[9]==0)
	{
		blackLayer.CheckError10(collectError);
	}
	if(errorSelectedByUser[10]==1&&errorCheckedByUser[10]==0)
	{
		blueLayer.CheckError11(blackLayer,brownLayer,collectError);
	}
	if(errorSelectedByUser[11]==1&&errorCheckedByUser[11]==0)
	{
		blueLayer.CheckError12(blackLayer,collectError);
	}
	if(errorSelectedByUser[12]==1&&errorCheckedByUser[12]==0)
	{
		blackLayer.CheckError13(collectError);
	}

	return error;
}

ASErr MapCheckPlugin::GenerateReport(CollectError collectError)
{
	ASErr error = kNoErr;

	collectError.PrintErrorInfo();
	collectError.SetErrorRed();

	return error;
}