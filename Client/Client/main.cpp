// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"

#include "CCore.h"

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;                                    // 메인 윈도우 핸들 전역변수화

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_      HINSTANCE hInstance,        // 현재 프로그램의 시작 주소 인스턴스
                      _In_opt_  HINSTANCE hPrevInstance,    // 이전 프로그램의 시작 주소 인스턴스 (안 씀)
                      _In_      LPWSTR lpCmdLine,
                      _In_      int nCmdShow)
{
    // 메모리 누수 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 메모리 누수 발생시 _CrtSetBreakAlloc() 주석 풀고 ()안에 중단점 입력하면 누수 지점 찾아줌. 찾고나면 다시 주석!
    // ex) {213} normal block at 0x0000019B181F71F0, 4 bytes long. 이렇게 발생시 213을 넣으면 됨.
    //_CrtSetBreakAlloc();

    // 참조 되지 않는 변수를 적어놓는다. 컴파일러는 무시함. 주석의 역할과 동일
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

    // 윈도우 정보 등록
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Core(프로그램) 생성 및 초기화
    if (FAILED(CCore::GetInst()->init( g_hWnd, POINT{ 640, 400 } )))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);
        return FALSE;
    }
    

    // 단축키 테이블 정보 로딩
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 기본 메시지 루프입니다:
    
    // GetMessage
    // 메세지 큐에서 메세지가 확인 될 때 까지 대기, 메시지가 들어오면 값을 반환
    // msg.message == WM_QUIT 인 경우 false를 반환 -> 프로그램 종료

    // PeekMessage
    // 메세지 유뮤와 관계없이 항상 값을 반환
    // 메세지큐에서 메세지를 확인한 경우 true, 메세지가 없는 경우 false를 반환한다. -> 메세지가 없어도 프로그램이 동작

    // 프로그램이 돌아가는 동안, 메시지가 없는 시간이 메시지가 있어서 처리하는 시간보다 훠어어어얼씬 길다(99:1? 정도의 비율).
    // 따라서 메시지를 처리하지 않는 시간을 활용하지 않으면, 아주 비효율적인 프로그램이 된다.
    // -> PeekMessage를 써야한다.

    while (1)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // 메세지가 없는 동안 호출: Game 코드 수행
            // 기본 디자인 패턴(설계 유형): 싱글톤 패턴
            CCore::GetInst()->progress();
        }
    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 새로운 윈도우를 생성하고, 그 윈도우를 다룰 수 있는 윈도우 핸들을 만든다. (윈도우의 ID라고 생각하면 편함)
   // 여러가지 핸들들이 존재한다.(HWND, HPEN, HBRUSH, HDC ... ) 이는 핸들들의 용도를 구별해준다.
   // 실제로 핸들들은 전처리기 구문으로 인해 프로그램 내에 구조체로 만들어지고 멤버 변수로 int값 하나를 갖는다.
   // int값은 ID를 의미하고, 생성될 때 ID값을 부여받는다.
   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT: // 무효화 영역이 발생한 경우 WM_PAINT 발생
        {
            // BeginPaint와 EndPaint가 없으면, 여기서 무한루프를 돌 것임.
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN: // 키보드 정보: wParam
        {
        // ESC 키가 눌렸을 때 프로그램 종료
		if (wParam == VK_ESCAPE) { PostQuitMessage(0); }
        }
        break;

    case WM_LBUTTONDOWN: // 마우스 x: LOWORD(lParam), y: HIWORD(lParam)
        break;

    case WM_LBUTTONUP:
        break;

    case WM_MOUSEMOVE:
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
