#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define NOMINMAX
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <memory>
#include <functional>
#include <filesystem>
#include <thread>
#include <mutex>

#include <vector>
#include <string>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <algorithm>

#include <mmsystem.h>
#include <dinput.h>

#include <limits>
#include <nlohmann/json.hpp>
#include <locale>
#include <codecvt>

#include "Math.h"
#include "Enums.h"
#include "Graphics/RGraphics.h"

#define BIT(x) (1 << x)
#define CAST_UINT(value) static_cast<UINT>(value)
#define CAST_FLOAT(value) static_cast<float>(value)

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if (p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; }

#define J_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

using namespace std;
using namespace nlohmann;


