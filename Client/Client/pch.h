#pragma once
// 미리 컴파일된 헤더 (모든 cpp 파일들에 이 헤더를 참조하도록 만들어줘야한다. 없으면 에러)
// : 코드를 다시 컴파일하지 않도록 만들어준다. stdfax 이름 바꾼거임
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <winsock2.h> // 윈속2 메인 헤더
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <Windows.h>
#pragma comment(lib, "Msimg32.lib")

#include <vector>
#include <array>
#include <string>
#include <map>
#include <list>

#include <cassert>

#include "define.h" 
#include "structs.h"
#include "enums.h"
#include "functions.h"