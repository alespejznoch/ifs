#include "stdafx.h"
#include "Data.h"


Data::Data(void)
{
}

Data::~Data(void)
{
}

bool Data::NactiData(void) {
	ifstream fin(_T("data.dat"));
	if (fin.fail()) {
		return false;
	}

	string radek;
	wstring ss;
	istringstream stm;
	bool prvni=1;
	int poradi = 0;
	unsigned int sekce = 1;
	int j = 0;
	double g;

	while (!getline(fin, radek).fail()) {
		if (radek.length()==0) {
			poradi = 0;
			prvni = 1;
			sekce++;
			continue;
		}
		poradi++;

		ss.clear();

		istringstream stm;
		stm.str(radek);
		wistringstream wstm;
		wstm.str(StringToWString(radek));
	
		if (prvni) {

			wstm >> ss;
			typy.push_back(ss);
			ss.clear();
			prvni=0;
		} else {

			for (j=0; j<7; j++) {
				stm >> g;
				data.resize(sekce);
				data[sekce-1].resize(poradi-1);
				data[sekce-1][poradi-2].push_back(g);
			}
		}

		
	}

		fin.close();

		data[0][0][2];

 return true;
}