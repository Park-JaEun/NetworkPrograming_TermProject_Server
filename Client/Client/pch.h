#pragma once
// �̸� �����ϵ� ��� (��� cpp ���ϵ鿡 �� ����� �����ϵ��� ���������Ѵ�. ������ ����)
// : �ڵ带 �ٽ� ���������� �ʵ��� ������ش�. stdfax �̸� �ٲ۰���
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <winsock2.h> // ����2 ���� ���
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

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