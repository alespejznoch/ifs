// Ales Pejznoch IFS.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "OknoHlavni.h"

OknoHlavni OH;  // instance hlavniho okna

class CAlesPejznochIFSModule : public CAtlExeModuleT< CAlesPejznochIFSModule >
{
public :
	DECLARE_LIBID(LIBID_ATLLib)
	HRESULT PreMessageLoop(int nShowCmd);
	
	void RunMessageLoop() throw()
	{
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0) > 0)
		{
			if (!OH.formular || !IsDialogMessage(OH.formular->m_hWnd, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

};

CAlesPejznochIFSModule _AtlModule;

extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


HRESULT CAlesPejznochIFSModule::PreMessageLoop(int nShowCmd)
{
	
	CAtlExeModuleT<CAlesPejznochIFSModule>::PreMessageLoop(nShowCmd);
	return OH.Vytvorit() ? S_OK : S_FALSE;
}