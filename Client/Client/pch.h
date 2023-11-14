#pragma once
// 미리 컴파일된 헤더 (모든 cpp 파일들에 이 헤더를 참조하도록 만들어줘야한다. 없으면 에러)
// : 코드를 다시 컴파일하지 않도록 만들어준다. stdfax 이름 바꾼거임

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