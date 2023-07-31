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

ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer)
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
	SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	//进入蓝版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集蓝版信息
	blueLayer.GetPathOfLayer(path);
	SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	//进入棕版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集棕版信息
	brownLayer.GetPathOfLayer(path);
	SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	//进入黑版图层
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//收集黑版信息
	blackLayer.GetPathOfLayer(path);
	SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	return error;
}

ASErr CheckError(GreenLayer greenLayer,BlueLayer blueLayer,BrownLayer brownLayer,BlackLayer blackLayer,CollectError& collectError,AIBoolean errorSelectedByUser[ErrorTypeNum],AIBoolean errorCheckedByUser[ErrorTypeNum])
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

ASErr GenerateReport(CollectError collectError)
{
	ASErr error = kNoErr;
	return error;
}

int RegisterErrList(HINSTANCE hInstance,HWND hwnd)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = ErrListWindProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = TEXT("MainWClass");
	wndClass.hIconSm = nullptr;

	// 注册窗口类
	::RegisterClassEx(&wndClass);
	
	// 创建窗口
	hwnd = ::CreateWindowEx(\
			0, \
			TEXT("MainWClass"), \
			TEXT("Error List"), \
			WS_OVERLAPPEDWINDOW | WS_VSCROLL, \
			100, \
			100, \
			1000, \
			1000, \
			nullptr, \
			nullptr, \
			hInstance, \
			nullptr);

	

	// 显示窗口
	::ShowWindow(hwnd, SW_SHOW);
	// 刷新窗口客户区
	::UpdateWindow(hwnd);

	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK ErrListWindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static ai::int32 vscrollPos,xClient,yClient;
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hwnd, &ps);
		::EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CREATE:
	{
		if(flag == 1)
		{
			vscrollPos = saveVscrollPos;
		    xClient = 1000;
		    yClient = 1000;
		    for(ai::int32 i = 0;i<collectError.ErrorNum;i++)
		    {
			    staticText[i] = CreateWindowEx(0,"Static",collectError.ErrorTypeList[collectError.errors[i].errorType].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | SS_LEFT,10,10+i*50-vscrollPos,400,30,hwnd,NULL,hInstance,NULL);
			    highLightButton[i] = CreateWindowEx(0,"BUTTON","高亮居中显示",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON,450,10+i*50-vscrollPos,150,30,hwnd,NULL,hInstance,NULL);
			    correctedCheckBox[i] = CreateWindowEx(0,"BUTTON","已修正",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX,650,10+i*50-vscrollPos,100,30,hwnd,NULL,hInstance,NULL);
			    if(haveCorrected[i] == 1)
			    {
				    SendMessage(correctedCheckBox[i],BM_SETCHECK,BST_CHECKED,0);
			    }
		    }
		    SetScrollPos(hwnd,SB_VERT,vscrollPos,TRUE);
		}
		else if(flag==0)
		{
			for(ai::int32 i = 0;i<round(ErrorTypeNum/2);i++)
		    {
			    errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i+1].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,10,10+i*50,350,30,hwnd,NULL,hInstance,NULL);
		    }
		    for(ai::int32 i = round(ErrorTypeNum/2);i<ErrorTypeNum;i++)
		    {
			    errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,400,10+(i-round(ErrorTypeNum/2))*50,350,30,hwnd,NULL,hInstance,NULL);
		    }
		    selectAll = CreateWindowEx(0,"BUTTON","全选",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,50,round(ErrorTypeNum/2)*50,100,30,hwnd,NULL,hInstance,NULL);
		    deselectAll = CreateWindowEx(0,"BUTTON","取消全选",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,200,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
		    check = CreateWindowEx(0,"BUTTON","开始检测",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,500,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
		}
		else if(flag==2)
		{
			hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR) NULL, 
                            WS_CHILD | WS_VISIBLE, 450, 
                            450, 
                            300, 100, 
                            hwnd, NULL, hInstance, NULL);
			SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 4));
			SendMessage(hwndPB, PBM_SETSTEP, (WPARAM) 1, 0);
		}
		return 0;
	}
	case WM_SIZE:
	{
		if(flag == 1)
		{
			xClient = LOWORD(lParam);
		    yClient = HIWORD(lParam);
		    SetScrollRange(hwnd,SB_VERT,0,10+collectError.ErrorNum*50,FALSE);
		}
		return 0;
	}
	case WM_VSCROLL:
	{
		switch(LOWORD(wParam))
		{
		case SB_LINEDOWN:
			vscrollPos +=50;
			break;
		case SB_LINEUP:
			vscrollPos -=50;
			break;
		case SB_THUMBTRACK:
			vscrollPos = HIWORD(wParam);
			break;
		}
		SetScrollPos(hwnd,SB_VERT,vscrollPos,TRUE);
		HDWP hdwp = BeginDeferWindowPos(collectError.ErrorNum*3);
		for(ai::int32 i = 0;i<collectError.ErrorNum;i++)
		{
			hdwp = DeferWindowPos(hdwp,staticText[i],NULL,10,10+i*50-vscrollPos,400,30,SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
			hdwp = DeferWindowPos(hdwp,highLightButton[i],NULL,450,10+i*50-vscrollPos,150,30,SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
			hdwp = DeferWindowPos(hdwp,correctedCheckBox[i],NULL,650,10+i*50-vscrollPos,100,30,SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
		}
		EndDeferWindowPos(hdwp);
	}
	case WM_COMMAND:
	{
		for(ai::int32 i = 0;i<collectError.ErrorNum;i++)
		{
			if(HWND(lParam) == highLightButton[i])
			{
				::SendMessageA(hwnd,WM_CLOSE,0,0);
				collectError.SetErrorRedAndCenter(i);
				saveVscrollPos = vscrollPos;
			}
		}
		for(ai::int32 i = 0;i<collectError.ErrorNum;i++)
		{
			if((HWND)lParam == correctedCheckBox[i])
			{
				haveCorrected[i] = 1;
			}
		}
		for(ai::int32 i = 0;i<ErrorTypeNum;i++)
		{
			if((HWND)lParam == errTypeCheckBox[i])
			{
				errorSelectedByUser[i] = SendMessage(errTypeCheckBox[i],BM_GETCHECK,0,0);
			}
		}
		if((HWND)lParam == selectAll)
		{
			for(ai::int32 i = 0;i<ErrorTypeNum;i++)
			{
				errorSelectedByUser[i] = 1;
				SendMessage(errTypeCheckBox[i],BM_SETCHECK,BST_CHECKED,0);
			}
		}
		if((HWND)lParam == deselectAll)
		{
			for(ai::int32 i = 0;i<ErrorTypeNum;i++)
			{
				errorSelectedByUser[i] = 0;
				SendMessage(errTypeCheckBox[i],BM_SETCHECK,BST_UNCHECKED,0);
			}
		}
		if((HWND)lParam == check)
		{
			::SendMessageA(hwnd,WM_CLOSE,0,0);
			CheckError(greenLayer,blueLayer,brownLayer,blackLayer,collectError,errorSelectedByUser,errorCheckedByUser);
		}
		return 0;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		return 0;
	}
	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

int RegisterProcessBarWnd(HINSTANCE hInstance,HWND hwnd)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = ErrListWindProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = TEXT("MainWClass");
	wndClass.hIconSm = nullptr;
	// 注册窗口类
	::RegisterClassEx(&wndClass);
	// 创建窗口
	hwnd = ::CreateWindowEx(\
			0, \
			TEXT("MainWClass"), \
			TEXT("Process..."), \
			WS_OVERLAPPEDWINDOW, \
			400, \
			400, \
			400, \
			200, \
			nullptr, \
			nullptr, \
			hInstance, \
			nullptr);
	// 显示窗口
	::ShowWindow(hwnd, SW_SHOW);
	// 刷新窗口客户区
	::UpdateWindow(hwnd);
	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

int RegisterCheckErrWnd(HINSTANCE hInstance,HWND hwnd)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = ErrListWindProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = TEXT("MainWClass");
	wndClass.hIconSm = nullptr;
	// 注册窗口类
	::RegisterClassEx(&wndClass);
	// 创建窗口
	hwnd = ::CreateWindowEx(\
			0, \
			TEXT("MainWClass"), \
			TEXT("Check Error"), \
			WS_OVERLAPPEDWINDOW, \
			400, \
			400, \
			800, \
			800, \
			nullptr, \
			nullptr, \
			hInstance, \
			nullptr);
	// 显示窗口
	::ShowWindow(hwnd, SW_SHOW);
	// 刷新窗口客户区
	::UpdateWindow(hwnd);
	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
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

	for(ai::int32 i = 0 ;i<ErrorTypeNum;i++)
	{
		errorSelectedByUser[i] = 0;
		errorCheckedByUser[i] = 0;
		haveCorrected[i] = 0;
		saveVscrollPos = 0;
	}
	hInstance = GetModuleHandle(0);
	return error;
}

ASErr MapCheckPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
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
	throwAwayMenuData.itemText = ai::UnicodeString("DisplayError");
	error = sAIMenu->AddMenuItem(message->d.self,NULL,&throwAwayMenuData,0,&fDisplayErrorMenu);
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

	if(message->menuItem == fAnalyseMapMenu)
	{
		flag = 2;
		RegisterProcessBarWnd(hInstance,hwndPBWnd);
		AnalyseMap(greenLayer,blueLayer,brownLayer,blackLayer);
		//进度条
		
		sAIUser->MessageAlert(ai::UnicodeString("分析完成！"));
	}
	else if(message->menuItem == fCheckErrorMenu)
	{
		flag = 0;
		RegisterCheckErrWnd(hInstance,checkErrWnd);
		//进度条
	}
	else if(message->menuItem == fDisplayErrorMenu)
	{
		flag = 1;
		RegisterErrList(hInstance,errList);
	}
	else if(message->menuItem == fGenerateReportMenu)
	{
		
    }
	return error;


}

