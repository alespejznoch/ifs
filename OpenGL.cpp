#include "StdAfx.h"
#include "OpenGL.h"
#include "Data.h"

#define random(x) (rand()*(x)/RAND_MAX)  // pocita nahodu pro vyber tranformace


BOOL OpenGL::SetupPixelFormat(HDC hdc) { // nastaveni opengl pro win api 
    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd

            1,                           // version number

            PFD_DRAW_TO_WINDOW |         // support window

            PFD_SUPPORT_OPENGL |         // support OpenGL

            PFD_DOUBLEBUFFER,            // double buffered

            PFD_TYPE_RGBA,               // RGBA type

            24,                          // 24-bit color depth

            0, 0, 0, 0, 0, 0,            // color bits ignored

            0,                           // no alpha buffer

            0,                           // shift bit ignored

            0,                           // no accumulation buffer

            0, 0, 0, 0,                  // accum bits ignored

            32,                          // 32-bit z-buffer

            0,                           // no stencil buffer

            0,                           // no auxiliary buffer

            PFD_MAIN_PLANE,              // main layer

            0,                           // reserved

            0, 0, 0                      // layer masks ignored

    };
    int pixelformat;

    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0) {
        ATLASSERT(FALSE);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
        ATLASSERT(FALSE);
        return FALSE;
    }

    return TRUE;
}



void OpenGL::CreateContext(HDC hdc) {  // vytvoreni opengl kontextu

    PIXELFORMATDESCRIPTOR pfd;
    if (!SetupPixelFormat(hdc))
        return;

    int n = GetPixelFormat(hdc);
    DescribePixelFormat(hdc, n, sizeof(pfd), &pfd);
    m_hRC = wglCreateContext(hdc);
    wglMakeCurrent(hdc, m_hRC);
    
}

LRESULT OpenGL::OnCreate(UINT , WPARAM , LPARAM, BOOL&) { // zacatek

    HDC hdc = GetDC();
    CreateContext(hdc);
	RECT rc;
	GetClientRect(&rc);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	int sirka = rc.right - rc.left;
    int vyska = rc.bottom - rc.top;
	pixIFS=VytvorPixmapu(sirka, vyska);

    return 0;
}

LRESULT OpenGL::OnDestroy(UINT , WPARAM, LPARAM , BOOL& ) { // konec

  wglMakeCurrent(NULL, NULL);

  if (m_hRC)
  {
    wglDeleteContext(m_hRC);
    m_hRC = NULL;
  }

  return 0;
}

LRESULT OpenGL::OnSize(UINT , WPARAM , LPARAM , BOOL& ) {	// zmena velikosti

	RECT rc;
    GetClientRect(&rc);
	int sirka = rc.right - rc.left;
    int vyska = rc.bottom - rc.top;
		ZrusPixmapu(&pixIFS);
		pixIFS=VytvorPixmapu(sirka, vyska);


	glViewport(0, 0, sirka, vyska);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0, sirka, 0, vyska, -1, 1); 

    return 0;
}


HRESULT OpenGL::OnDraw(UINT , WPARAM , LPARAM , BOOL& ) {

	PAINTSTRUCT ps;
	RECT rc;
	GetClientRect(&rc);
	HDC hdc = BeginPaint(&ps);
	EndPaint(&ps);

	float ppr, ppg, ppb;
	ppr = (float)pr/255;
	ppg = (float)pg/255;
	ppb = (float)pb/255;
    glClearColor(ppr, ppg, ppb, 10.0);

	
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_BACK);
	
	if (vykreslit) {	
	        VycistiPixmapu(pixIFS);
			VypoctiIFS(pixIFS, max_iter, meritko, xpoz, ypoz); // prepocet fraktalu
			VykresliPixmapu(pixIFS);                      // vykresleni pixmapy
	}

    
		glFlush();   
		SwapBuffers(wglGetCurrentDC());
	
	

    return S_OK;
}

pixmapa * OpenGL::VytvorPixmapu(const unsigned int sirka, const unsigned int vyska) {

    pixmapa *p = new pixmapa;
    if (!p) return NULL;
    p->sirka=sirka;
    p->vyska=vyska;
    p->pixely = new unsigned char [3*sirka*vyska];
    if (!p->pixely) {
		delete p;
        return NULL;
    }
    else {
        memset(p->pixely, 0, 3*sirka*vyska);
    }
    return p;
}

void OpenGL::ZrusPixmapu(pixmapa** p) {
  pixmapa* map = *p;
  if (map->pixely) { 
    delete[] map->pixely;
  } 
  if (map) { 
    delete map;
  } 
  *p = NULL;
}

void OpenGL::VycistiPixmapu(const pixmapa *p) {

    if (!p) return;
    if (!p->pixely) return;
	unsigned int i=0;
	while (i<(3*p->sirka*p->vyska)) {
		p->pixely[i] = pr;
		p->pixely[i+1] = pg;
		p->pixely[i+2] = pb;
		i+=3;
	}
}

void OpenGL::VykresliPixmapu(const pixmapa *p) {

    if (!p || !p->pixely) return;
	
    glDrawPixels(
            p->sirka, p->vyska,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            p->pixely);
}

void OpenGL::VlozPixel(pixmapa *p,
              const unsigned int x, const unsigned int y,
              const unsigned char r, const unsigned char g, const unsigned char b) {

    int poz;

    if (!p || !p->pixely || x>=p->sirka || y>=p->vyska) return;
    poz=3*(x+y*p->sirka);
    p->pixely[poz++]=r;
    p->pixely[poz++]=g;
    p->pixely[poz]=b;
}

void OpenGL::VlozPixel2(pixmapa *p, const unsigned int x, const unsigned int y,
                         const unsigned char dr, const unsigned char dg, const unsigned char db)
{
    int poz;
    if (!p || !p->pixely || x>=p->sirka || y>=p->vyska) return;
    poz=3*(x+y*p->sirka);
	if ((pr+pg+pb)<382) {
		if (p->pixely[poz]<256-dr) p->pixely[poz]+=dr;
			poz++;
		if (p->pixely[poz]<256-dg) p->pixely[poz]+=dg;
			poz++;
		if (p->pixely[poz]<256-db) p->pixely[poz]+=db;
	} else {
		if (p->pixely[poz]>dr) p->pixely[poz]-=dr-1;
			poz++;
		if (p->pixely[poz]>dg) p->pixely[poz]-=dg-1;
			poz++;
		if (p->pixely[poz]>db) p->pixely[poz]-=db-1;
	}
}

void OpenGL::PripravMorfing() {
	unsigned int r = 1;
	unsigned int m = 1;
	unsigned int i,j;
	i=j=0;
	if (data[typ].size()>data[typ2].size()) {
		r = data[typ].size();
	} else {
		r = data[typ2].size();
	}

	morf_data.resize(3);
	morf_data[0].resize(r);
	morf_data[1].resize(r);
	morf_data[2].resize(r);
	for (unsigned int i=0; i<morf_data[2].size(); i++) { morf_data[2][i].resize(7);	}

	if (r>data[typ].size()) {
		m=r;
	} else {
		m=data[typ].size();
	}

	i=0;
	while (i<m) {
				morf_data[0][i].push_back(1);
				morf_data[0][i].push_back(0);
				morf_data[0][i].push_back(0);
				morf_data[0][i].push_back(1);
				morf_data[0][i].push_back(0);
				morf_data[0][i].push_back(0);
				morf_data[0][i].push_back(1);
		i++;
	}

	i=0;
	m=data[typ].size();
	while (i<m) {
			for (j=0; j<7; j++) {
				morf_data[0][i][j] = data[typ][i][j];
			}
		i++;
	}

	if (r>data[typ2].size()) {
		m=r;
	} else {
		m=data[typ2].size();
	}

	
	i=0;
	while (i<m) {
				morf_data[1][i].push_back(1);
				morf_data[1][i].push_back(0);
				morf_data[1][i].push_back(0);
				morf_data[1][i].push_back(1);
				morf_data[1][i].push_back(0);
				morf_data[1][i].push_back(0);
				morf_data[1][i].push_back(1);
		i++;
	}

	i=0;
	m=data[typ2].size();
	while (i<m) {
			for (j=0; j<7; j++) {
				morf_data[1][i][j] = data[typ2][i][j];
			}
		i++;
	}
}

void OpenGL::VymazMorfData() {

	morf_data.clear();
}

void OpenGL::VypoctiMorfing(double ratio) {
	for (unsigned int i=0; i<morf_data[2].size(); i++) {
		for (int j=0; j<7; j++) {
			morf_data[2][i][j] =
				(1.0-ratio)*morf_data[0][i][j] + 
				(ratio)*morf_data[1][i][j];

		}
    }
}

void OpenGL::VypoctiIFS(pixmapa *pix, int max_iter, double meritko, double xpoz, double ypoz) {

    int    iter=0;
    int    threshold=50;

    double  x, y, hlp;
    double  pp;
    double  sum=0.0;                
    int    k=0;

	int    maxTransf=0;
    x=0;
    y=0;
    srand(0);
    xpoz+=pix->sirka/2;
    ypoz+=pix->vyska/2;

	unsigned char red=rf ? dr : 0;
    unsigned char green=gf ? dg : 0;
    unsigned char blue=bf ? db : 0;



	
	if (morfuj) {

		maxTransf = morf_data[2].size()-1;
		VypoctiMorfing(ratio);

		while (iter++<max_iter*100) {               // iteracni smycka
		for (k=0; k<=maxTransf; k++) {
			double xn=morf_data[2][k][0]*x+morf_data[2][k][1]*y+morf_data[2][k][4];
            double yn=morf_data[2][k][2]*x+morf_data[2][k][3]*y+morf_data[2][k][5];
			if (iter > threshold) {
				switch (zpusob) {                 // vyber vykreslovaciho rezimu
                    case 0:
                        VlozPixel(pix, (unsigned int)(xn*meritko+xpoz), (unsigned int)(yn*meritko+ypoz), rf, gf, bf);
                        break;
                    case 1:
                        VlozPixel2(pix, (unsigned int)(xn*meritko+xpoz), (unsigned int)(yn*meritko+ypoz), red, green, blue);
                        break;
				}
			}
		}
		pp=((float)rand())/RAND_MAX;                // p leží v rozsahu 0.0-1.0
        k=0;                                        // promìnná cyklu vyhledání transformace
        sum=0;
        while (sum<=pp) {                           // podle náhodného èísla
            sum=sum+morf_data[2][k][6];                     // vybrat transformaci
            k++;
			if (k>maxTransf) break;
        }                                          // k urèuje index transformace
        k--;                                        // úprava ze smyèky while
        hlp= morf_data[2][k][0]*x+morf_data[2][k][1]*y+morf_data[2][k][4];   // provedení
        y  = morf_data[2][k][2]*x+morf_data[2][k][3]*y+morf_data[2][k][5];   // transformace
        x  = hlp;                                    // a výpoèet bodu pro další iteraci

	}
	///////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		            //////////////////////////////////////////
		                        //////////////////////
		                                 /////

	} else {
		maxTransf = data[typ].size()-1;
		while (iter++<max_iter*100) {               // iteracni smycka
			for (k=0; k<=maxTransf; k++) {
				double xn=data[typ][k][0]*x+data[typ][k][1]*y+data[typ][k][4];
				double yn=data[typ][k][2]*x+data[typ][k][3]*y+data[typ][k][5];
				if (iter > threshold) {
					switch (zpusob) {                 // vyber vykreslovaciho rezimu
						case 0:
							VlozPixel(pix, (unsigned int)(xn*meritko+xpoz), (unsigned int)(yn*meritko+ypoz), rf, gf, bf);
							break;
						case 1:
	                        VlozPixel2(pix, (unsigned int)(xn*meritko+xpoz), (unsigned int)(yn*meritko+ypoz), red, green, blue);
							break;
					}
				}
			}
			pp=((float)rand())/RAND_MAX;                // p leží v rozsahu 0.0-1.0
			k=0;                                        // promìnná cyklu vyhledání transformace
	        sum=0;
			while (sum<=pp) {                           // podle náhodného èísla
	            sum=sum+data[typ][k][6];                     // vybrat transformaci
				k++;
				if (k>maxTransf) break;
			}                                          // k urèuje index transformace
			k--;                                        // úprava ze smyèky while
			hlp=data[typ][k][0]*x+data[typ][k][1]*y+data[typ][k][4];   // provedení
			y  =data[typ][k][2]*x+data[typ][k][3]*y+data[typ][k][5];   // transformace
			x  =hlp;                                    // a výpoèet bodu pro další iteraci

		}
	}
}

void OpenGL::savePixmap(const pixmapa *p, const char *fileName) {
    FILE *fout;
    unsigned int i, j, k;

    unsigned char tgaHeader[18]={               // hlavicka TGA
                        0x00,                   // typ hlavicky
                        0x00,                   // nepouzivame paletu
                        0x02,                   // typ obrazku je RGB TrueColor
                        0x00, 0x00,             // delka palety je nulova
                        0x00, 0x00, 0x00,       // pozice v palete nas nezajima
                        0x00, 0x00, 0x00, 0x00, // obrazek je umisteny na pozici [0, 0]
                        0x00, 0x00, 0x00, 0x00, // sirka a vyska obrazku (dva byty na polozku)
                        0x18,                   // format je 24 bitu na pixel
                        0x20                    // orientace bitmapy v obrazku
    };

    if (!p || !p->pixely) return;
    memcpy(tgaHeader+12, &(p->sirka), 2);
    memcpy(tgaHeader+14, &(p->vyska), 2);
    fopen_s(&fout, fileName, "wb");
    if (fout) {
        fwrite(tgaHeader, 18, 1, fout);
        for (j=p->vyska; j; j--) {
            unsigned int yoff=(j-1)*3*p->sirka;
            unsigned int xoff=0;
            for (i=0; i<p->sirka; i++) {
                for (k=0; k<3; k++) {
                    fputc(p->pixely[xoff+yoff+2-k], fout);
                }
                xoff+=3;
            }
        }
        fclose(fout);
    }
}