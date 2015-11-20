// Formular.h : Declaration of the CFormular

#pragma once

#include "resource.h"       // main symbols
#include "OpenGL.h"

//#define PANEL 220


// CFormular

class CFormular : 
	public CDialogImpl<CFormular>
{
public:
	CFormular()
	{
		m_hDialogBrush = CreateSolidBrush(RGB(100, 100, 100));
		nastav_barvu = 0x00ffffff;
		barva = 0x00ffffff;
		pozadi = 0x00000000;
		stetec = CreateSolidBrush(barva);
		stetec2 = CreateSolidBrush(pozadi);
		tick = 0;
	}

	~CFormular()
	{
		DeleteObject(m_hDialogBrush);
	}

	enum { IDD = IDD_FORMULAR };

	HBRUSH m_hDialogBrush;

BEGIN_MSG_MAP(CFormular)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
	COMMAND_HANDLER(IDC_BUTTON_VYKRESLIT, BN_CLICKED, OnBnClickedButtonVykreslit)
	COMMAND_HANDLER(IDC_BUTTON_BARVA, BN_CLICKED, OnBnClickedButtonBarva)
	COMMAND_HANDLER(IDC_BUTTON_POZADI, BN_CLICKED, OnBnClickedButtonPozadi)
	COMMAND_HANDLER(IDC_BUTTON_PLUS, BN_CLICKED, OnBnClickedButtonPlus)
	COMMAND_HANDLER(IDC_BUTTON_MINUS, BN_CLICKED, OnBnClickedButtonMinus)
	COMMAND_HANDLER(IDC_BUTTON_ITERPLUS, BN_CLICKED, OnBnClickedButtonIterplus)
	COMMAND_HANDLER(IDC_BUTTON_ITERMINUS, BN_CLICKED, OnBnClickedButtonIterminus)
	COMMAND_HANDLER(IDC_BUTTON6, BN_CLICKED, OnBnClickedButton6)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	COMMAND_HANDLER(IDC_BUTTON_NAHORU, BN_CLICKED, OnBnClickedButtonNahoru)
	COMMAND_HANDLER(IDC_BUTTON_DOLU, BN_CLICKED, OnBnClickedButtonDolu)
	COMMAND_HANDLER(IDC_BUTTON_DOLEVA, BN_CLICKED, OnBnClickedButtonDoleva)
	COMMAND_HANDLER(IDC_BUTTON_DOPRAVA, BN_CLICKED, OnBnClickedButtonDoprava)
	COMMAND_HANDLER(IDC_BUTTON5, BN_CLICKED, OnBnClickedButton5)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	COMMAND_HANDLER(IDC_BUTTON_ANI, BN_CLICKED, OnBnClickedButtonAni)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SetTimer(IDC_TIMER, 20);
		bHandled = TRUE;
		HFONT hFont;
		hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Cambria"));
		SendMessage(GetDlgItem(IDC_BUTTON_NAHORU), WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(GetDlgItem(IDC_BUTTON_DOLU), WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(GetDlgItem(IDC_BUTTON_DOLEVA), WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(GetDlgItem(IDC_BUTTON_DOPRAVA), WM_SETFONT, (WPARAM)hFont, 0);
		return 1;  // Let the system set the focus
	}


LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
LRESULT OnBnClickedButtonVykreslit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

void Spoj(OpenGL *o, CFormular *f) {
	ogl = o;
	formular = f;
	vykreslit = 0;
	animuj = 0;
	animuj_kladne=1;
}


	HBRUSH stetec, stetec2;
	int PANEL;
	COLORREF nastav_barvu, barva, pozadi;
	int typ, zpusob, typ2;
	bool vykreslit;
	double ro;
private:
	OpenGL *ogl;
	CFormular *formular;
	double tick;
	bool animuj, animuj_kladne;

public:
	LRESULT OnBnClickedButtonBarva(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPozadi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPlus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonMinus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonIterplus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonIterminus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton6(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonNahoru(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDolu(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDoleva(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDoprava(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonAni(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


