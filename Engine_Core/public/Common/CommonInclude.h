#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define NOMINMAX
#include <Windows.h>
#include <assert.h>
#include <vector>
#include <string>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <math.h>
#include <cmath>
#include <limits>
#include <mutex>
#include <thread>
#include <mmsystem.h>
#include <dinput.h>
#include <nlohmann/json.hpp>
#include <locale>
#include <codecvt>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include "Math.h"
#include "Enums.h"
#include "Graphics/RGraphics.h"

#define CAST_UINT(value) static_cast<UINT>(value)
#define CAST_FLOAT(value) static_cast<float>(value)

using namespace std;
using namespace nlohmann;


