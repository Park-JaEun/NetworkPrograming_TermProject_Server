#pragma once

#include <Windows.h>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <list>
#include <mutex>
#include <cassert>

#include "protocol.h"
#include "define.h" 
#include "struct.h"
#include "enum.h"
#include "function.h"