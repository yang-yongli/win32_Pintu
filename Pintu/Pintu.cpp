// Pintu.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Pintu.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND hWnd;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 函数声明
void init();
void load(char * filename);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PINTU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINTU));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// 初始化时间
	srand((unsigned)time(NULL));
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"拼图", WS_OVERLAPPEDWINDOW,
		100, 100, 400, 420, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 初始化
	init();
	SetTimer(hWnd, 1, 500, NULL);//设置计时器
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


#define NEXT 10
//定义宽高
#define WIDTH 130
#define HEIGHT 130
//定义数组
int map[10][10];
//定义设备描述表
HDC memhdc;
//定义鼠标所在行列
int mouse_row, mouse_col;
//定义空白格所在行列
int row9 = 2, col9 = 2;

// 初始化
void init()
{
	int s = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			s++;
			map[i][j] = s;
		}
	}

	//随机交换位置
	int r1, c1, c2, r2;
	int a;
	int b;
	for (int t = 0; t < 100; t++)
	{
		a = rand() % 8 + 1;
		b = rand() % 8 + 1;
		r1 = (a - 1) / 3;
		c1 = (a - 1) % 3;
		r2 = (b - 1) / 3;
		c2 = (b - 1) % 3;
		int m;
		m = map[r1][c1];
		map[r1][c1] = map[r2][c2];
		map[r2][c2] = m;
	}
	// 加载图片
	char buf[100];
	wsprintf(buf, "1.bmp", 1);
	load(buf);
}

void load(char *filename)
{
	HDC hdc = GetDC(hWnd);
	memhdc = ::CreateCompatibleDC(hdc);
	ReleaseDC(hWnd, hdc);
	HBITMAP bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(memhdc, bmp);
}

//转换到行列
void TurnToRowCol(int x, int y)
{
	mouse_row = y / HEIGHT;
	mouse_col = x / WIDTH;
	if (mouse_row > 2)
		mouse_row = 2;
	if (mouse_col > 2)
		mouse_col = 2;
}


//判断是否和空白块相邻
int isNearBlank()
{
	if (mouse_row == row9)
	{
		if (abs(col9 - mouse_col) == 1)
		{
			return true;
		}
	}
	if (mouse_col == col9)
	{
		if (abs(mouse_row - row9) == 1)
		{
			return true;
		}
	}
	return false;
}

//交换数据
void ExChange()
{
	int tmp = map[mouse_row][mouse_col];
	map[mouse_row][mouse_col] = map[row9][col9];
	map[row9][col9] = tmp;
	row9 = mouse_row;
	col9 = mouse_col;
}

//判断游戏结束
int isGameOver()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] != (i * 3 + j + 1))
			{
				return false;
			}
		}
	return true;
}

//绘制游戏界面
void draw()
{
	HDC hdc = GetDC(hWnd);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] != 9)
			{
				int r, c;
				r = (map[i][j] - 1) / 3;
				c = (map[i][j] - 1) % 3;

				BitBlt(hdc, j*WIDTH, i*HEIGHT, WIDTH, HEIGHT, memhdc, c*WIDTH, r*HEIGHT, SRCCOPY);
			}
			else
			{
				Rectangle(hdc, j*WIDTH, i*HEIGHT, (j + 1)*WIDTH, (i + 1)*HEIGHT);
			}
		}
	ReleaseDC(hWnd, hdc);

}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	//LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);


	switch (message)
	{
		// 当鼠标右键按下时，直接拼图完成
	case WM_RBUTTONDOWN:
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				map[i][j] = i * 3 + j + 1;
			}
		col9 = mouse_col;
		row9 = mouse_row;
		draw();
	}
	break;
	// 当鼠标左键按下时候，判断是否能移动，移动后是否拼图完成
	case WM_LBUTTONDOWN:
	{
		// 将鼠标参数传入进行转化
		TurnToRowCol(LOWORD(lParam), HIWORD(lParam));
		if (isNearBlank())	// 判断是否能移动（是否挨着空白格）
		{
			//交换鼠标点击位置与空白位置
			ExChange();
			draw();
			if (isGameOver())	// 判断是否游戏结束
			{
				//显示游戏结束
				if (MessageBox(hWnd, "拼图完成,是否重新开始？", "", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
				{
					init();	//重新初始化数据
					draw();	//重新绘制
				}
				else
				{
					DestroyWindow(hWnd);//关闭窗口
				}

			}
		}

	}
	break;
	case WM_COMMAND:
	{
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
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
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		draw();
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
