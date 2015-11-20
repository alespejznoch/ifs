#pragma once

#include "stdafx.h"
using namespace std;

class Data
{
public:
	Data(void);
	~Data(void);

	bool NactiData(void);
	vector<vector<vector<double>>> DejData() {
		return data;
	}

	vector<wstring> DejTypy() {
		return typy;
	}

	std::wstring StringToWString(const std::string& s) {
		vector<WCHAR> s2(100);
		wstring temp;
		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			s.c_str(),
			s.length(),
			&s2[0],				// wide-character buffer
			100					// buffer
		);
		temp = &s2[0];
		return temp;
	}

	
	vector<vector<vector<double>>> data;
	vector<wstring> typy;
};
