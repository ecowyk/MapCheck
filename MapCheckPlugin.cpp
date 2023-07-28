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
	hDlg(NULL),
	fPanel2(NULL),
	hDlg2(NULL)
{
	strncpy(fPluginName, kMapCheckPluginName, kMaxStringLength);
	fDefaultWindProc = NULL;
	fDefaultWindProc2 = NULL;
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

	//创建错误列表面板
	AISize pnSize2 = {240, 320};
	error = sAIPanel->Create(fPluginRef, ai::UnicodeString("ErrorList"),ai::UnicodeString("ErrorList"),3,pnSize2,true,NULL,this,fPanel2);
	if(error)
		return error;
    AISize size2 = {800,800};
	error = sAIPanel->SetSize(fPanel2,size2);
	error = sAIPanel->Show(fPanel2,true);
	error = sAIPanel->GetPlatformWindow(fPanel2, hDlg2);
	return error;
}

ASErr MapCheckPlugin::AddWidgets()
{
	ASErr error = kNoErr;
	
	//创建选择检测类型面板
	AISize pnSize = {240, 320};
	error = sAIPanel->Create(fPluginRef, ai::UnicodeString("MapCheck"),ai::UnicodeString("MapCheck"),3,pnSize,true,NULL,this,fPanel);
	if(error)
		return error;
	AISize size = {600,600};
	error = sAIPanel->SetSize(fPanel,size);
	error = sAIPanel->Show(fPanel,true);
	AIPanelPlatformWindow hDlg = NULL;
	error = sAIPanel->GetPlatformWindow(fPanel, hDlg);
	//创建复选框

	HWND text = CreateWindowEx(0,"Static","请选择需要检测的错误",WS_VISIBLE|WS_CHILD|WS_BORDER,200,10,200,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	button1 = CreateWindowEx(0,"BUTTON","开始检测",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,50,500,100,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	button2 = CreateWindowEx(0,"BUTTON","全选",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,250,500,100,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	button3 = CreateWindowEx(0,"BUTTON","取消全选",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,450,500,100,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[0] = CreateWindowEx(0,"BUTTON","Error1",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,50,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[1] = CreateWindowEx(0,"BUTTON","Error2",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,100,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[2] = CreateWindowEx(0,"BUTTON","Error3",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,150,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[3] = CreateWindowEx(0,"BUTTON","Error4",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,200,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[4] = CreateWindowEx(0,"BUTTON","Error5",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,250,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[5] = CreateWindowEx(0,"BUTTON","Error6",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,300,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[6] = CreateWindowEx(0,"BUTTON","Error7",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,10,350,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[7] = CreateWindowEx(0,"BUTTON","Error8",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,50,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[8] = CreateWindowEx(0,"BUTTON","Error9",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,100,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[9] = CreateWindowEx(0,"BUTTON","Error10",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,150,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[10] = CreateWindowEx(0,"BUTTON","Error11",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,200,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[11] = CreateWindowEx(0,"BUTTON","Error12",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,250,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[12] = CreateWindowEx(0,"BUTTON","Error13",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,300,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);
	checkBoxs[13] = CreateWindowEx(0,"BUTTON","Error14",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,310,350,150,30,hDlg,NULL,(HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),NULL);

	::SetPropA(hDlg, "TPNL", this);
	fDefaultWindProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hDlg, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MapCheckPlugin::NewWindowProc)));

	return error;
}

ASErr MapCheckPlugin::CreateErrorListDlg(CollectError& collectError)
{
	ASErr error = kNoErr;

	

	HWND text = CreateWindowEx(0,"Static","错误列表",WS_VISIBLE|WS_CHILD|WS_BORDER,350,10,100,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	collectError.PageNum = collectError.ErrorNum / ErrorNumPerPage + 1;
	collectError.CurPage = 1;
	if(collectError.PageNum > 1)
	{
		for(ai::int32 i = 0;i<ErrorNumPerPage;i++)
	    {
		    errorDescription[i] = CreateWindowEx(0,"Static",collectError.ErrorTypeList[collectError.errors[i].errorType].c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,10,50+i*50,400,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorHighlight[i] = CreateWindowEx(0,"Button","错误高亮显示",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,450,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorCorrected[i] = CreateWindowEx(0,"Button","错误已修正",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,650,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	    }
	}
	else
	{
		for(ai::int32 i = 0;i<collectError.ErrorNum;i++)
	    {
		    errorDescription[i] = CreateWindowEx(0,"Static",collectError.ErrorTypeList[collectError.errors[i].errorType].c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,10,50+i*50,400,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorHighlight[i] = CreateWindowEx(0,"Button","错误高亮显示",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,450,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorCorrected[i] = CreateWindowEx(0,"Button","错误已修正",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,650,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	    }
	}
	previousPageButton = CreateWindowEx(0,"BUTTON","上一页",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,500,700,80,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	nextPageButton = CreateWindowEx(0,"BUTTON","下一页",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,700,700,80,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	string page = "1/"+to_string(static_cast<long long>(collectError.PageNum));
	HWND pageText = CreateWindowEx(0,"Static",page.c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,600,700,30,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);

	::SetPropA(hDlg2, "TPNL", this);
	fDefaultWindProc2 = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hDlg2, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MapCheckPlugin::NewWindowProc2)));

	return error;
}

ASErr MapCheckPlugin::UpdateErrorList(CollectError& collectError)
{
	ASErr error = kNoErr;

	for(ai::int32 i = 0;i<ErrorNumPerPage*(collectError.CurPage-1);i++)
	{
		ShowWindow(errorDescription[i],SW_HIDE);
		ShowWindow(errorHighlight[i],SW_HIDE);
		ShowWindow(errorCorrected[i],SW_HIDE);
	}
	if(collectError.PageNum > collectError.CurPage)
	{
		for(ai::int32 i = 0;i<ErrorNumPerPage;i++)
	    {
			errorDescription[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Static",collectError.ErrorTypeList[collectError.errors[i+(collectError.CurPage-1)*ErrorNumPerPage].errorType].c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,10,50+i*50,400,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorHighlight[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Button","错误高亮显示",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,450,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorCorrected[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Button","错误已修正",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,650,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	    }
	}
	else
	{
		for(ai::int32 i = 0;i<collectError.ErrorNum-(collectError.CurPage-1)*ErrorNumPerPage;i++)
	    {
		    errorDescription[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Static",collectError.ErrorTypeList[collectError.errors[i+(collectError.CurPage-1)*ErrorNumPerPage].errorType].c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,10,50+i*50,400,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorHighlight[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Button","错误高亮显示",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,450,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
			errorCorrected[i+(collectError.CurPage-1)*ErrorNumPerPage] = CreateWindowEx(0,"Button","错误已修正",WS_VISIBLE|WS_CHILD|WS_BORDER|BS_AUTOCHECKBOX,650,50+i*50,150,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);
	    }
	}
	string page = to_string(static_cast<long long>(collectError.CurPage))+"/"+to_string(static_cast<long long>(collectError.PageNum));
	HWND pageText = CreateWindowEx(0,"Static",page.c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,600,700,30,30,hDlg2,NULL,(HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE),NULL);

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

LRESULT CALLBACK MapCheckPlugin::NewWindowProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MapCheckPlugin* panel = (MapCheckPlugin*)GetPropA(hWnd, "TPNL");
	LRESULT result = 0;
	bool handled = panel->PanelWindowProc2(result, hWnd, msg, wParam, lParam);
	if(!handled)
		result =  panel->CallDefaultWindowProc2(hWnd, msg, wParam, lParam);
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
				AIBoolean flag1 = 0;//用户是否选择了检测项;
				AIBoolean flag2 = 0;//用户是否已检测了所有项目
				for(ai::int32 i = 0;i<ErrorTypeNum;i++)
				{
					if(errorSelectedByUser[i] == 1)
					{
						flag1++;
					}
					if(errorCheckedByUser[i] == 1)
					{
						flag2++;
					}
				}
				if(flag1 == 0)
				{
					sAIUser->MessageAlert(ai::UnicodeString("请选择您要检测的项目！"));
				}
				else
				{
					if(flag2 == ErrorTypeNum)
					{
						sAIUser->MessageAlert(ai::UnicodeString("您已检测了所有项目，请打印报告！"));
					}
					else
					{
						CheckError(greenLayer,blueLayer,brownLayer,blackLayer,collectError,errorSelectedByUser,errorCheckedByUser);
				        for(ai::int32 i = 0;i<ErrorTypeNum;i++)
		        		{
			        		if(errorCheckedByUser[i] == 0&&errorSelectedByUser[i] == 1)
				        	{
				        		errorCheckedByUser[i] = 1;
			        		}
		        		}
		        		sAIUser->MessageAlert(ai::UnicodeString("检测完毕！"));
					}
				}
			}
			if((HWND)lParam == button2)
			{
				for(ai::int32 i = 0;i<ErrorTypeNum;i++)
				{
					Button_SetCheck(checkBoxs[i],1);
					errorSelectedByUser[i] = (int)SendMessage(checkBoxs[i],BM_GETCHECK,0,0);
				}
			}
			if((HWND)lParam == button3)
			{
				for(ai::int32 i = 0;i<ErrorTypeNum;i++)
				{
					Button_SetCheck(checkBoxs[i],BST_UNCHECKED);
					errorSelectedByUser[i] = (int)SendMessage(checkBoxs[i],BM_GETCHECK,0,0);
				}
			}
			if((HWND)lParam == checkBoxs[0])
			{
				errorSelectedByUser[0] = (int)SendMessage(checkBoxs[0],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[1])
			{
				errorSelectedByUser[1] = (int)SendMessage(checkBoxs[1],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[2])
			{
				errorSelectedByUser[2] = (int)SendMessage(checkBoxs[2],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[3])
			{
				errorSelectedByUser[3] = (int)SendMessage(checkBoxs[3],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[4])
			{
				errorSelectedByUser[4] = (int)SendMessage(checkBoxs[4],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[5])
			{
				errorSelectedByUser[5] = (int)SendMessage(checkBoxs[5],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[6])
			{
				errorSelectedByUser[6] = (int)SendMessage(checkBoxs[6],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[7])
			{
				errorSelectedByUser[7] = (int)SendMessage(checkBoxs[7],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[8])
			{
				errorSelectedByUser[8] = (int)SendMessage(checkBoxs[8],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[9])
			{
				errorSelectedByUser[9] = (int)SendMessage(checkBoxs[9],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[10])
			{
				errorSelectedByUser[10] = (int)SendMessage(checkBoxs[10],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[11])
			{
				errorSelectedByUser[11] = (int)SendMessage(checkBoxs[11],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBoxs[12])
			{
				errorSelectedByUser[12] = (int)SendMessage(checkBoxs[12],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == checkBox[13])
			{
				errorSelectedByUser[13] = (int)SendMessage(checkBox[13],BM_GETCHECK,0,0);
			}
			if((HWND)lParam == previousPageButton)
			{
				collectError.CurPage--;
				UpdateErrorList(collectError);
			}
			if((HWND)lParam == nextPageButton)
			{
				collectError.CurPage++;
				UpdateErrorList(collectError);
				sAIUser->MessageAlert(ai::UnicodeString("nextPageButton"));
			}
		}
	}
	return handled;
}

bool MapCheckPlugin::PanelWindowProc2(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	switch (msg) 
	{
	case WM_COMMAND: 
		{
			if((HWND)lParam == previousPageButton)
			{
				collectError.CurPage--;
				UpdateErrorList(collectError);
			}
			if((HWND)lParam == nextPageButton)
			{
				collectError.CurPage++;
				UpdateErrorList(collectError);
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

LRESULT CALLBACK MapCheckPlugin::CallDefaultWindowProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(fDefaultWindProc2)
		return ::CallWindowProc(fDefaultWindProc2, hWnd, msg, wParam, lParam);
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
	if(fPanel)
	{
		error = sAIPanel->Destroy(fPanel2);
    	fPanel2 = NULL;
	}

	error = Plugin::ShutdownPlugin(message);
	return kNoErr;
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
		sAIUser->MessageAlert(ai::UnicodeString("分析完成！"));
	}
	else if(message->menuItem == fCheckErrorMenu)
	{
		if(fPanel)
		{
			AIBoolean isShown = false;
			error = sAIPanel->IsShown(fPanel, isShown);
			error = sAIPanel->Show(fPanel, !isShown);
		}
	}
	else if(message->menuItem == fGenerateReportMenu)
	{
		CreateErrorListDlg(collectError);
		if(fPanel2)
		{
			AIBoolean isShown = false;
			error = sAIPanel->IsShown(fPanel2, isShown);
			error = sAIPanel->Show(fPanel2, !isShown);
		}
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
	if(errorSelectedByUser[13]==1&&errorCheckedByUser[13]==0)
	{
		brownLayer.CheckError14(collectError);
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
