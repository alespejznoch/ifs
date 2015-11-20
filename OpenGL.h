#pragma once

#include "resource.h"
#include "stdafx.h"
using namespace std;

typedef struct {                                // struktura pixmapy
		unsigned int sirka;
		unsigned int vyska;
		unsigned char *pixely;
} pixmapa;

class OpenGL : public CWindowImpl <OpenGL>
{
public:

OpenGL(void){
	max_iter = 800;
	meritko = 40.0;
	xpoz = 0.0;
	ypoz = -150.0;
	redraw=1;
	pixIFS = NULL;
	vykreslit = 0;
	zpusob = 0;
	pr = pg = pb = 0;
	morfuj = 0;
	ratio = 0.4;
	};
~OpenGL(void){};

	HGLRC m_hRC;


	BEGIN_MSG_MAP(OpenGL);
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_PAINT, OnDraw)
	END_MSG_MAP()


	pixmapa *pixIFS;							// pixmapa pro IFS

	int    max_iter;							// maximalni pocet iteraci
	double meritko;								// meritko fraktalu
	double xpoz;
	double ypoz;                             
	bool redraw;
	int typ, zpusob, typ2;
	int rf, gf, bf;
	int dr, dg, db;
	int pr, pg, pb;
	bool morfuj;
	double ratio;

	

	BOOL SetupPixelFormat(HDC hdc);
	void CreateContext(HDC hdc);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	HRESULT OpenGL::OnDraw(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);



	pixmapa* VytvorPixmapu(const unsigned int sirka, const unsigned int vyska);
	void ZrusPixmapu(pixmapa **p);
	void VycistiPixmapu(const pixmapa *p);
	void VykresliPixmapu(const pixmapa *p);

	void Export( const char *fileName) {
			pixIFS=VytvorPixmapu(1600, 1200);
			VycistiPixmapu(pixIFS);
			VypoctiIFS(pixIFS, max_iter, meritko, xpoz, ypoz);
			savePixmap( pixIFS, fileName);
			RECT rc;
			GetClientRect(&rc);
			int sirka = rc.right - rc.left;
			int vyska = rc.bottom - rc.top;
			pixIFS=VytvorPixmapu(sirka, vyska);
	}

	void savePixmap(const pixmapa *p, const char *fileName);


	void VlozPixel(pixmapa *p,
				const unsigned int x, const unsigned int y, 
				const unsigned char r, const unsigned char g, const unsigned char b);

	void OpenGL::VlozPixel2(pixmapa *p, const unsigned int x, const unsigned int y,
                         const unsigned char dr, const unsigned char dg, const unsigned char db);

	void Morfuj(bool m, double r, int t1, int t2) { 
		morfuj = m; ratio = r;
		if ((typ!=t1) || (typ2!=t2)) {
			typ=t1; typ2=t2; 
			VymazMorfData();
			PripravMorfing();
		}
	}
	
	void PripravMorfing();

	void VymazMorfData();

	void VypoctiMorfing(double ratio);

	void VypoctiIFS(pixmapa *pix, int max_iter, double meritko, double xpoz, double ypoz);
	
	void Kresli(bool i, int t, int z, COLORREF b, COLORREF p) {
		if (!vykreslit) { vykreslit = i; }
		typ = t;
		zpusob = z;
		
		dr = GetRValue(b);
		dg = GetGValue(b);
		db = GetBValue(b);

		if (zpusob==1) { b = 0x00ffffff; }
		rf = GetRValue(b);
		gf = GetGValue(b);
		bf = GetBValue(b);

		pr = GetRValue(p);
		pg = GetGValue(p);
		pb = GetBValue(p);

		SendMessage(WM_PAINT);
	}

	void VlozData(vector<vector<vector<double>>> d) {
		data = d;
	}

	void Zvetsi() {
		meritko+=20;
	}

	void Zmensi() {
		meritko-=20;
	}

	void IterZvetsi() {
		max_iter+=100;
	}

	void IterZmensi() {
		max_iter-=100;
		if (max_iter<=0){ max_iter= 1; }
	}

	void Nahoru() {
		ypoz+=20;
	}

	void Dolu() {
		ypoz-=20;
	}

	void Doleva() {
		xpoz-=20;
	}

	void Doprava() {
		xpoz+=20;
	}

	void Centruj() {
		xpoz = 0;
		ypoz = -150;
	}

private:
	bool vykreslit;
	vector<vector<vector<double>>> data;
	vector<vector<vector<double>>> morf_data;

};