#pragma once

#include "resource.h"
#include "Formular.h"
#include "OpenGL.h"
#include "Data.h"

#define OKRAJ 5


class OknoHlavni : public CWindowImpl <OknoHlavni>
{
public:
	static CWndClassInfo& GetWndClassInfo() {
		static CWndClassInfo wc = {
			{
				sizeof(WNDCLASSEX),
				CS_HREDRAW | CS_VREDRAW,
				StartWindowProc,
				0, 0,
				NULL,
				(HICON)LoadIcon(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDI_ICON1)),
				(HCURSOR)NULL,
				(HBRUSH)GetStockObject(DKGRAY_BRUSH),
				MAKEINTRESOURCE(IDR_MENU1),
				_T("f"),
				(HICON)LoadIcon(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDI_ICON1))
			},
			NULL,
			NULL,
			IDC_ARROW,
			TRUE,
			0,
			L""
		};
		return wc;
	}

	CFormular* formular;
	OpenGL* ogl;
	int PANEL;


	OknoHlavni(void) {
		formular = NULL;
		ogl = NULL;
	}

	~OknoHlavni(void) {
		if (formular) {
			delete formular;
		}

		if (ogl) {
			delete ogl;
		}
	}

	BEGIN_MSG_MAP(OknoHlavni);
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		COMMAND_ID_HANDLER(ID_MENU_ITERATOR_NOVY, OnMenuIteratorNovy)
		COMMAND_ID_HANDLER(ID_MENU_KONEC, OnMenuKonec)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		COMMAND_ID_HANDLER(ID_MENU_ITERATOR_ULOZIT, OnMenuIteratorUlozit)
		COMMAND_ID_HANDLER(ID_ITER32786, OnIter32786)
		COMMAND_ID_HANDLER(ID_ITER32787, OnIter32787)
	END_MSG_MAP()

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bhandled) {
		PostQuitMessage(0);
		return DefWindowProc();
	}

	bool Vytvorit(void) {
		int width(900), height(700);

		RECT rect = {0, 0, width, height };
		HWND hwnd = Create(NULL, &rect, _T("IFS editor"), WS_OVERLAPPEDWINDOW);
		if (!hwnd) {
			return false;
		}

		int lpx = GetDeviceCaps(GetDC(), LOGPIXELSX);
		if (lpx > 96) { width = 1200;  height = 850; }  // 144

		SetWindowPos(hwnd, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		/////////////////////////////////////////////////////////////////////////////
		//////////////////////// INICIALIZACE ///////////////////////////////////////
	

		Data *data = new Data;
		
		if (!data->NactiData()) {
			MessageBox(_T("Error: File data.dat missing or corrupted"), _T("Error"),
				MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
		}

		
		if (lpx <= 96) { PANEL = 215; }  // 96
		if (lpx > 96) { PANEL = 240; }  // 120
		if (lpx > 120) { PANEL = 300; } // 144
		if (lpx > 144) { PANEL = 400; } // 192
		SendMessage(WM_SETICON, ICON_BIG, IDI_ICON1);


		// form formular vlozeny do okna aplikace
		formular = new CFormular();
		formular->Create(m_hWnd);
		formular->PANEL = PANEL;

		formular->SetWindowPos(NULL,
			rect.right-PANEL, 0, PANEL,rect.bottom,
			SWP_NOZORDER);
		
		// Combo Boxy
		formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_SETITEMHEIGHT, (WPARAM)20, 0);
		formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_SETITEMHEIGHT, (WPARAM)20, 0);
		formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_SETMINVISIBLE, (WPARAM)10, 0);
		formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_SETMINVISIBLE, (WPARAM)10, 0);
		int p_typy = data->DejTypy().size();
		int i=0;
		while (i<p_typy) {
			formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_INSERTSTRING, i,
				(LPARAM)data->DejTypy().at(i).c_str());
			formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_INSERTSTRING, i,
				(LPARAM)data->DejTypy().at(i).c_str());
			i++;
		}
		formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_SETCURSEL, 0, 0);
		formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_SETCURSEL, 1, 0);

		formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_INSERTSTRING, 0,
				(LPARAM)_T("Constant"));
		formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_INSERTSTRING, 1,
				(LPARAM)_T("Histogram"));

		formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_SETCURSEL, 0, 0);

		formular->SendDlgItemMessageW(IDC_RATIO, EM_LIMITTEXT, 2, 0);
		formular->SetDlgItemText(IDC_RATIO, _T("40"));

		formular->ShowWindow(SW_SHOW);
	

		// OpenGL
		RECT vnitrek;
		GetClientRect(&vnitrek);
		RECT rct = {vnitrek.left+(vnitrek.right-PANEL)/100*OKRAJ, vnitrek.top+vnitrek.bottom/100*OKRAJ,
					vnitrek.right-(vnitrek.right-PANEL)/100*OKRAJ-PANEL, vnitrek.bottom-vnitrek.bottom/100*OKRAJ};
	
		ogl = new OpenGL;
		ogl->Create(m_hWnd, rct, NULL, WS_CHILD | WS_CLIPSIBLINGS |
							WS_CLIPCHILDREN | WS_VISIBLE, 0, IDC_OPENGL);

		
		ogl->VlozData(data->DejData());
		formular->Spoj(ogl, formular);

		
		CenterWindow();
		ShowWindow(SW_SHOW);


		return true;
	}


	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMenuIteratorNovy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenuKonec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBarva(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenuIteratorUlozit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnIter32786(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnIter32787(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};