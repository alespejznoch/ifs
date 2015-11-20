// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit,

#define _ATL_ALL_WARNINGS
#define _ATL_NO_COM_SUPPORT
#define _INC_WINDOWSX

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlcoll.h>
#include <atlfile.h>
#include <atlimage.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atltime.h>
#include <atlconv.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>


// OpenGL knihovny
#include <gl/Gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


using namespace ATL;