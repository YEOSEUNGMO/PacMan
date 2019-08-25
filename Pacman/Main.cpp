#include"resource.h"
#include"PlayManager.h"


LPCTSTR lpszClass = TEXT("���¸�_17032044");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 730, 740, NULL, (HMENU)NULL, hInstance, NULL);//730,740
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rectView;
	static int tempx, tempy;
	static char str[10];
	static PlayManager pm;
	static TILE tile[17][17];
	static PLAYER player;
	switch (iMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		pm.bit_wall = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_WALL)); // �� ��Ʈ�� ����
		pm.bit_eating = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_EATING));// ���� ��Ʈ�� ����
		pm.bit_empty = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_EMPTY));// �� ���� ��Ʈ�� ����
		pm.bit_player = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_PLAYER_UP));//�÷��̾� ��Ʈ�� ����
		pm.bit_enemy[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(201));//����1 ��Ʈ�� ����
		pm.bit_enemy[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(205));//����2 ��Ʈ�� ����
		pm.bit_enemy[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(209));//����3 ��Ʈ�� ����
		pm.bit_enemy[3] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(213));//����4 ��Ʈ�� ����
		SetTimer(hWnd, 1, 80, NULL);//�÷��̾�,���� ���۸�Ÿ�̸� ����
		SetTimer(hWnd, 2, 70, NULL);//���� Ÿ�̸� ����
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			pm.Set_NextWay(0);
			break;
		case VK_DOWN:
			pm.Set_NextWay(1);
			break;
		case VK_LEFT:
			pm.Set_NextWay(2);
			break;
		case VK_RIGHT:
			pm.Set_NextWay(3);
			break;
		}
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			pm.OnTimer(); 
			pm.Player_Move();
			
			if (pm.Win_Check()) {// ���̸� �� �Ծ����� üũ
				KillTimer(hWnd, 1);
				if (MessageBox(hWnd,  TEXT("���̸� �� �Ծ����ϴ�!"), TEXT("�¸�!"), MB_OK) == IDOK)
					PostQuitMessage(0);
			}
			else if (pm.Catch_Player())//���ɰ� �Ѹ��� �Ÿ��� 30�̸��϶� true ����
			{
				KillTimer(hWnd, 1);
				if (MessageBox(hWnd, TEXT("�Ѹ��� �������ϴ�!"), TEXT("�й�.."), MB_OK) == IDOK)
					PostQuitMessage(0);
			}
			break;
		case 2:
			pm.Enemy_Move(0);
			pm.Enemy_Move(1);
			pm.Enemy_Move(2);
			pm.Enemy_Move(3);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hBit) {
			pm.DrawBitmap(hdc, 0, 0, hBit);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}