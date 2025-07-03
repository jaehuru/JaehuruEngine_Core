#pragma once

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
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include "huruMath.h"
#include "huruEnums.h"
#include "Graphics/huruGraphics.h"

#define CAST_UINT(value) static_cast<UINT>(value)
#define CAST_FLOAT(value) static_cast<float>(value)

using namespace std;
using namespace huru;
using namespace huru::math;
using namespace huru::enums;
using namespace huru::graphics;

