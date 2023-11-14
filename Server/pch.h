#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <winsock2.h> // 윈속2 메인 헤더
#include <Windows.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <list>
#include <mutex>
#include <cassert>

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include "protocol.h"
#include "define.h" 
#include "struct.h"
#include "enum.h"
#include "function.h"