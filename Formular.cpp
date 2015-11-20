// Formular.cpp : Implementation of CFormular

#include "stdafx.h"
#include "Formular.h"


// CFormular

LRESULT CFormular::OnCtlColorDlg(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (long) m_hDialogBrush;
}

LRESULT CFormular::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetBkColor((HDC)wParam, RGB(100,100,100));
	SetTextColor((HDC)wParam, RGB(255,119,48));
	
	return (long) m_hDialogBrush;
}

LRESULT CFormular::OnBnClickedButtonVykreslit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	
	typ = zpusob = 0;
	vykreslit = 1;
	barva = nastav_barvu;
	
	zpusob = formular->formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_GETCURSEL, 0, 0);
	typ = formular->formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_GETCURSEL, 0, 0);
	
	tick = 0.0;
	animuj_kladne=1;
	ogl->Morfuj(0, 0.4, 0, 0);
	ogl->Kresli(vykreslit, typ, zpusob, barva, pozadi);

	HMENU hmenu;
	hmenu = GetParent().GetMenu();
	EnableMenuItem(hmenu,ID_MENU_ITERATOR_ULOZIT, MF_ENABLED);
	EnableMenuItem(hmenu,ID_ITER32786, MF_ENABLED);
	return 0;
}

LRESULT CFormular::OnBnClickedButtonBarva(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
		CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = GetParent();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = nastav_barvu;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (!ChooseColor(&cc))
		return true;
	nastav_barvu = cc.rgbResult;

	DeleteObject(stetec);
	stetec = CreateSolidBrush(nastav_barvu);
	SendMessage(WM_PAINT,0,0);

	return 0;
}

LRESULT CFormular::OnBnClickedButtonPozadi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
		CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = GetParent();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = pozadi;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (!ChooseColor(&cc))
		return true;
	pozadi = cc.rgbResult;

	DeleteObject(stetec2);
	stetec2 = CreateSolidBrush(pozadi);
	SendMessage(WM_PAINT,0,0);

	ogl->Kresli(0, typ, zpusob, barva, pozadi);
	return 0;
}

LRESULT CFormular::OnBnClickedButtonPlus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Zvetsi();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}
	return 0;
}

LRESULT CFormular::OnBnClickedButtonMinus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Zmensi();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonIterplus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->IterZvetsi();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonIterminus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->IterZmensi();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButton6(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ogl->max_iter = 1000;
	ogl->meritko = 40;
	pozadi = 0x00000000;
	if (vykreslit) {
		ogl->Centruj();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	} else {
		ogl->Kresli(0, 0, 0, 0x00ffffff, pozadi);
	}
	DeleteObject(stetec2);
	stetec2 = CreateSolidBrush(pozadi);
	SendMessage(WM_PAINT,0,0);

	return 0;
}
LRESULT CFormular::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT ps;
	RECT rect;
	GetClientRect(&rect);
	HDC hdc = BeginPaint(&ps);
	
	EndPaint(&ps);

	if (GetDeviceCaps(GetDC(), LOGPIXELSX)==96) {
		RECT rc;
		GetClientRect(&rc);

		RECT rc2 = {rc.right-PANEL+34, rc.top+132, rc.right-PANEL+90, rc.top+165};
		FillRect(GetDC(), &rc2, stetec);

		RECT rc3 = {rc.right-PANEL+22, rc.top+410, rc.right-PANEL+55, rc.top+470};
		FillRect(GetDC(), &rc3, stetec2);
	}

	return 0;
}
LRESULT CFormular::OnBnClickedButtonNahoru(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Nahoru();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonDolu(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Dolu();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonDoleva(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Doleva();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonDoprava(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (vykreslit) {
		ogl->Doprava();
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButton5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	typ = zpusob = 0;
	vykreslit = 1;
	barva = nastav_barvu;
	TCHAR r[MAX_PATH];
	int rat;
	double ratio;
	
	zpusob = formular->formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_GETCURSEL, 0, 0);
	typ = formular->formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_GETCURSEL, 0, 0);
	typ2 = formular->formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_GETCURSEL, 0, 0);

    formular->GetDlgItemText(IDC_RATIO, r, MAX_PATH);


	wistringstream stm;
	stm.str(r);
	stm >> rat;
	ratio = rat/100.0;

	ogl->Morfuj(1, ratio, typ, typ2);
	ogl->Kresli(vykreslit, typ, zpusob, barva, pozadi);
	HMENU hmenu;
	hmenu = GetParent().GetMenu();
	EnableMenuItem(hmenu,ID_MENU_ITERATOR_ULOZIT, MF_ENABLED);
	EnableMenuItem(hmenu,ID_ITER32786, MF_ENABLED);

	return 0;
}
LRESULT CFormular::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (animuj) {

		if (tick>=100) { animuj_kladne = 0; }
		if (tick<=0) { animuj_kladne = 1; }
		if (animuj_kladne) { tick+=1.5; } else { tick-=1.5; }
		double f;
		f=tick/100.0;
		if (typ!=typ2) {
			ogl->Morfuj(1, f, typ, typ2);
		}
		ogl->Kresli(1, typ, zpusob, barva, pozadi);
	}

	return 0;
}

LRESULT CFormular::OnBnClickedButtonAni(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!animuj) {
			typ = zpusob = 0;
			vykreslit = 1;
			barva = nastav_barvu;

			zpusob = formular->formular->SendDlgItemMessageW(IDC_COMBO_TB, CB_GETCURSEL, 0, 0);
			typ = formular->formular->SendDlgItemMessageW(IDC_COMBO_TI, CB_GETCURSEL, 0, 0);
			typ2 = formular->formular->SendDlgItemMessageW(IDC_COMBO_TI2, CB_GETCURSEL, 0, 0);

		formular->GetDlgItem(IDC_BUTTON_VYKRESLIT).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_BUTTON_BARVA).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_BUTTON5).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_COMBO_TI).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_COMBO_TB).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_COMBO_TI2).EnableWindow(FALSE);
		formular->GetDlgItem(IDC_RATIO).EnableWindow(FALSE);
		formular->SetDlgItemTextW(IDC_BUTTON_ANI, _T("Stop"));
		HMENU hmenu;
		hmenu = GetParent().GetMenu();
		EnableMenuItem(hmenu,ID_MENU_ITERATOR_ULOZIT, MF_ENABLED);
		EnableMenuItem(hmenu,ID_ITER32786, MF_ENABLED);
		animuj = true;
	} else {
		animuj = false;
		formular->GetDlgItem(IDC_BUTTON_VYKRESLIT).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_BUTTON_BARVA).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_BUTTON5).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_COMBO_TI).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_COMBO_TB).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_COMBO_TI2).EnableWindow(TRUE);
		formular->GetDlgItem(IDC_RATIO).EnableWindow(TRUE);
		formular->SetDlgItemTextW(IDC_BUTTON_ANI, _T("Animate"));
	}

	return 0;
}