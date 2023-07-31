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

ASErr ChooseMap()
{
	ASErr error = kNoErr;

	return error;
}

ASErr AnalyseMap(GreenLayer& greenLayer,BlueLayer& blueLayer,BrownLayer& brownLayer,BlackLayer& blackLayer)
{
	ASErr error = kNoErr;

	ai::int32 layerListCount;//ͼ���б���
	ai::int32 layerCount;//һ��ͼ���б��е�ͼ����

	AILayerList list;//ͼ���б�
	AILayerHandle layer;//ͼ��

	AIArtHandle path;//·��
	
	//��ȡ·��
	error = sAILayerList->Count(&layerListCount);//��ȡ
	error = sAILayerList->GetFirst(&list);
	error = sAILayerList->CountLayers(list,&layerCount);
	error = sAILayerList->GetFirstLayer(list,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);//��ȡ��һ��·��

	//�ռ��̰���Ϣ
	greenLayer.GetPathOfLayer(path);

	//��������ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ�������Ϣ
	blueLayer.GetPathOfLayer(path);

	//�����ذ�ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ��ذ���Ϣ
	brownLayer.GetPathOfLayer(path);

	//����ڰ�ͼ��
	error = sAILayer->GetNextLayer(layer,&layer);
	error = sAIArt->GetFirstArtOfLayer(layer,&path);
	error = sAIArt->GetArtFirstChild(path,&path);
	//�ռ��ڰ���Ϣ
	blackLayer.GetPathOfLayer(path);

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

	// ע�ᴰ����
	::RegisterClassEx(&wndClass);
	
	// ��������
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

	

	// ��ʾ����
	::ShowWindow(hwnd, SW_SHOW);
	// ˢ�´��ڿͻ���
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
			    highLightButton[i] = CreateWindowEx(0,"BUTTON","����������ʾ",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON,450,10+i*50-vscrollPos,150,30,hwnd,NULL,hInstance,NULL);
			    correctedCheckBox[i] = CreateWindowEx(0,"BUTTON","������",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX,650,10+i*50-vscrollPos,100,30,hwnd,NULL,hInstance,NULL);
			    if(haveCorrected[i] == 1)
			    {
				    SendMessage(correctedCheckBox[i],BM_SETCHECK,BST_CHECKED,0);
			    }
		    }
		    SetScrollPos(hwnd,SB_VERT,vscrollPos,TRUE);
		}
		else
		{
			for(ai::int32 i = 0;i<round(ErrorTypeNum/2);i++)
		    {
			    errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i+1].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,10,10+i*50,350,30,hwnd,NULL,hInstance,NULL);
		    }
		    for(ai::int32 i = round(ErrorTypeNum/2);i<ErrorTypeNum;i++)
		    {
			    errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,400,10+(i-round(ErrorTypeNum/2))*50,350,30,hwnd,NULL,hInstance,NULL);
		    }
		    selectAll = CreateWindowEx(0,"BUTTON","ȫѡ",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,50,round(ErrorTypeNum/2)*50,100,30,hwnd,NULL,hInstance,NULL);
		    deselectAll = CreateWindowEx(0,"BUTTON","ȡ��ȫѡ",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,200,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
		    check = CreateWindowEx(0,"BUTTON","��ʼ���",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,500,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
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
	// ע�ᴰ����
	::RegisterClassEx(&wndClass);
	// ��������
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
	// ��ʾ����
	::ShowWindow(hwnd, SW_SHOW);
	// ˢ�´��ڿͻ���
	::UpdateWindow(hwnd);
	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

//LRESULT CALLBACK CheckErrWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = ::BeginPaint(hwnd, &ps);
//		::EndPaint(hwnd, &ps);
//		return 0;
//	}
//	case WM_CREATE:
//	{
//		for(ai::int32 i = 0;i<round(ErrorTypeNum/2);i++)
//		{
//			errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i+1].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,10,10+i*50,350,30,hwnd,NULL,hInstance,NULL);
//		}
//		for(ai::int32 i = round(ErrorTypeNum/2);i<ErrorTypeNum;i++)
//		{
//			errTypeCheckBox[i] = CreateWindowEx(0,"BUTTON",collectError.ErrorTypeList[i].c_str(),WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX | BS_LEFTTEXT,400,10+(i-round(ErrorTypeNum/2))*50,350,30,hwnd,NULL,hInstance,NULL);
//		}
//		selectAll = CreateWindowEx(0,"BUTTON","ȫѡ",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,50,round(ErrorTypeNum/2)*50,100,30,hwnd,NULL,hInstance,NULL);
//		deselectAll = CreateWindowEx(0,"BUTTON","ȡ��ȫѡ",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,200,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
//		check = CreateWindowEx(0,"BUTTON","��ʼ���",WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON | BS_CENTER,500,round(ErrorTypeNum/2)*50,200,30,hwnd,NULL,hInstance,NULL);
//		return 0;
//	}
//	case WM_SIZE:
//	{
//		return 0;
//	}
//	case WM_COMMAND:
//	{
//		for(ai::int32 i = 0;i<ErrorTypeNum;i++)
//		{
//			if((HWND)lParam == errTypeCheckBox[i])
//			{
//				errorSelectedByUser[i] = SendMessage(errTypeCheckBox[i],BM_GETCHECK,0,0);
//			}
//		}
//		if((HWND)lParam == selectAll)
//		{
//			for(ai::int32 i = 0;i<ErrorTypeNum;i++)
//			{
//				errorSelectedByUser[i] = 1;
//				SendMessage(errTypeCheckBox[i],BM_SETCHECK,BST_CHECKED,0);
//			}
//		}
//		if((HWND)lParam == deselectAll)
//		{
//			for(ai::int32 i = 0;i<ErrorTypeNum;i++)
//			{
//				errorSelectedByUser[i] = 0;
//				SendMessage(errTypeCheckBox[i],BM_SETCHECK,BST_UNCHECKED,0);
//			}
//		}
//		if((HWND)lParam == check)
//		{
//			::SendMessageA(hwnd,WM_CLOSE,0,0);
//			CheckError(greenLayer,blueLayer,brownLayer,blackLayer,collectError,errorSelectedByUser,errorCheckedByUser);
//		}
//		return 0;
//	}
//	case WM_DESTROY:
//	{
//		::PostQuitMessage(0);
//		return 0;
//	}
//	}
//
//	return ::DefWindowProc(hwnd, message, wParam, lParam);
//}

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
	/*AISize pnSize = {240, 320};
	error = sAIPanel->Create(fPluginRef, ai::UnicodeString("Third Party Panel"), ai::UnicodeString("Third Party Panel"), 3, pnSize, true, NULL, this, fPanel);
	if (error)
		return error;
	AIPanelPlatformWindow hDlg = NULL;
	error = sAIPanel->GetPlatformWindow(fPanel, hDlg);
	hInstance = (HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE);

	error = sAIPanel->Create(fPluginRef, ai::UnicodeString("Third Party Panel"), ai::UnicodeString("Third Party Panel"), 3, pnSize, true, NULL, this, fPanel2);
	if (error)
		return error;
	AIPanelPlatformWindow hDlg2 = NULL;
	error = sAIPanel->GetPlatformWindow(fPanel2, hDlg2);
	hInstance2 = (HINSTANCE)GetWindowLongPtr(hDlg2,GWLP_HINSTANCE);*/

	return error;
}

ASErr MapCheckPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
	error = Plugin::ShutdownPlugin(message);
	return kNoErr;
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


//������ѡ�˵���Ĺ���
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

		sAIUser->MessageAlert(ai::UnicodeString("������ɣ�"));
	}
	else if(message->menuItem == fCheckErrorMenu)
	{
		flag = 0;
		RegisterCheckErrWnd(hInstance,checkErrWnd);
	}
	else if(message->menuItem == fGenerateReportMenu)
	{
		flag = 1;
		RegisterErrList(hInstance,errList);
    }
	return error;


}

