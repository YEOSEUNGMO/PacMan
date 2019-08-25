#include"PlayManager.h"
#include"resource.h"

#pragma comment(lib,"msimg32.lib")
HINSTANCE g_hInst;
HWND hWndMain;
HBITMAP hBit;

#define SIZE 20
double LengthPts(int x1, int y1, int x2, int y2)//두점 사이 거리 함수
{
	return (sqrt((float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));//sqrt:제곱근함수(#include <math.h> 필요)
}

/*맵의 정보를 저장, 1:벽 0:길*/
int MapInfo[17][17] = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1 },
{ 1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1 },
{ 1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1 },
{ 1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1 },
{ 1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1 },
{ 1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1 },
{ 1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1 },
{ 1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1 },
{ 1,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1 },
{ 1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,1 },
{ 1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1 },
{ 1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1 },
{ 1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};


PlayManager::PlayManager() {
	int tempx = 40;
	int tempy = 40;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (MapInfo[i][j] == 1) {
				tile[i][j].draw_type = WALL;
			}
			else
				tile[i][j].draw_type = FOOD;
			tile[i][j].x = tempx;
			tile[i][j].y = tempy;
			tempx += 40;
		}
		tempx = 40;
		tempy += 40;
	}
	/*플레이어 값 초기화*/
	player.x = 320;//플레이어 X시작 좌표
	player.y = 640;//플레이어 Y시작 좌표
	player.Index_X = 7;
	player.Index_Y = 15;
	player.way = 0;
	player.next_way = 0;

	/*유령 값 초기화*/
	enemy[0].x = 80;
	enemy[0].y = 80;
	enemy[0].Index_X = 1;
	enemy[0].Index_Y = 1;
	enemy[0].way = 1;
	enemy[0].next_way = 1;

	enemy[1].x = 640;
	enemy[1].y = 80;
	enemy[1].Index_X = 15;
	enemy[1].Index_Y = 1;
	enemy[1].way = 2;
	enemy[1].next_way = 2;

	enemy[2].x = 80;
	enemy[2].y = 640;
	enemy[2].Index_X = 1;
	enemy[2].Index_Y = 15;
	enemy[2].way = 0;
	enemy[2].next_way = 0;

	enemy[3].x = 640;
	enemy[3].y = 320;
	enemy[3].Index_X = 15;
	enemy[3].Index_Y = 7;
	enemy[3].way = 1;
	enemy[3].next_way = 1;

	/*유령의 상,하,좌,우 이미지를 저장하는 반복문*/
	for (int i = 0; i < 4; i++)
	{
		enemy[i].UpImage = IDB_BITMAP_ENEMY1_UP + (i * 4);
		enemy[i].DownImage = IDB_BITMAP_ENEMY1_DOWN + (i * 4);
		enemy[i].LeftImage = IDB_BITMAP_ENEMY1_LEFT + (i * 4);
		enemy[i].RightImage = IDB_BITMAP_ENEMY1_RIGHT + (i * 4);
	}
	srand(clock());

}
bool PlayManager::Player_Rotaion(int n)//n:회전하고자 하는 방향
{
	switch (n)
	{
	case 0://상
		if (tile[player.Index_Y - 1][player.Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_player);
			bit_player = nullptr;
			bit_player = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAYER_UP));//위쪽 방향 전환
			player.way = player.next_way;
			return true;
		}
		else
			return false;
		break;
	case 1://하
		if (tile[player.Index_Y + 1][player.Index_X].draw_type != WALL)// 다음 칸 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_player);//현재 플레이어 이미지 삭제
			bit_player = nullptr;
			bit_player = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAYER_DOWN));//아래쪽방향 이미지 호출
			player.way = player.next_way;//현재 이동방향을 다음 이동방향으로 변경
			return true;
		}
		else
			return false;
		break;
	case 2://좌
		if (tile[player.Index_Y][player.Index_X - 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_player);
			bit_player = nullptr;
			bit_player = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAYER_LEFT));//왼쪽 방향 회전
			player.way = player.next_way;
			return true;
		}
		else
			return false;
		break;
	case 3://우
		if (tile[player.Index_Y][player.Index_X + 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_player);
			bit_player = nullptr;
			bit_player = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAYER_RIGHT));//오른쪽 방향 회전
			player.way = player.next_way;
			return true;
		}
		else
			return false;
		break;
	}
	return false;
}
void PlayManager::Player_Move()
{
	switch (player.way)
	{
	case 0://상
		if ((player.y % 40) == 0) {//플레이어가 맵의 정확하게 한칸(40)에 위치하였는지 확인
			Eat();//먹이를 먹음.
			if (player.way != player.next_way)//현재 진행방향과 유저가 키보드로 누른 진행방향이 다르면
			{
				if (Player_Rotaion(player.next_way))//팩맨 회전
					break;
			}
			if (tile[player.Index_Y - 1][player.Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				player.y -= 10;//Y축으로로 -10만큼 이동
				player.Index_Y -= 1;//플레이어 현재 Y인덱스 값 -1
			}
		}
		else//플레이어가 맵에 정확하게 한칸(40)에 위치하지 않았을 경우
		{
			player.y -= 10;//Y축으로로 -10만큼 이동
		}
		break;
	case 1://하
		if ((player.y % 40) == 0) {
			Eat();
			if (player.way != player.next_way)
			{
				if (Player_Rotaion(player.next_way))
					break;
			}
			if (tile[player.Index_Y + 1][player.Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				player.y += 10;
				player.Index_Y += 1;
			}
		}
		else
		{
			player.y += 10;
		}
		break;
	case 2://좌
		if ((player.x % 40) == 0) {
			Eat();
			if (player.way != player.next_way)
			{
				if (Player_Rotaion(player.next_way))
					break;
			}
			if (tile[player.Index_Y][player.Index_X - 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				player.x -= 10;
				player.Index_X -= 1;
			}
		}
		else
		{
			player.x -= 10;
		}
		break;
	case 3://우
		if ((player.x % 40) == 0) {
			Eat();
			if (player.way != player.next_way)
			{
				if (Player_Rotaion(player.next_way))
					break;
			}
			if (tile[player.Index_Y][player.Index_X + 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				player.x += 10;
				player.Index_X += 1;
			}
		}
		else
		{
			player.x += 10;
		}
		break;
	}
}
bool PlayManager::Enemy_Rotaion(int way, int num)
{

	switch (way)
	{
	case 0://상
		if (tile[enemy[num].Index_Y - 1][enemy[num].Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_enemy[num]);
			bit_enemy[num] = nullptr;
			bit_enemy[num] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(enemy[num].UpImage));//위쪽 방향 전환
			enemy[num].way = way;
			return true;
		}
		else
			return false;
		break;
	case 1://하
		if (tile[enemy[num].Index_Y + 1][enemy[num].Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_enemy[num]);
			bit_enemy[num] = nullptr;
			bit_enemy[num] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(enemy[num].DownImage));//아래쪽 방향 전환
			enemy[num].way = way;
			return true;
		}
		else
			return false;
		break;
	case 2://좌
		if (tile[enemy[num].Index_Y][enemy[num].Index_X - 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_enemy[num]);
			bit_enemy[num] = nullptr;
			bit_enemy[num] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(enemy[num].LeftImage));//왼쪽 방향 회전
			enemy[num].way = way;
			return true;
		}
		else
			return false;
		break;
	case 3://우
		if (tile[enemy[num].Index_Y][enemy[num].Index_X + 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
		{
			DeleteObject(bit_enemy[num]);
			bit_enemy[num] = nullptr;
			bit_enemy[num] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(enemy[num].RightImage));//오른쪽 방향 회전
			enemy[num].way = way;
			return true;
		}
		else
			return false;
		break;
	}
	return false;
}
void PlayManager::Enemy_Move(int num)
{
	int ran = rand();
	switch (enemy[num].way)
	{
	case 0://상
		if ((enemy[num].y % 40) == 0) {
			enemy[num].next_way = (ran % 2) + 2;
			if (tile[enemy[num].Index_Y - 1][enemy[num].Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				enemy[num].y -= 10;
				enemy[num].Index_Y -= 1;
			}
			else if (enemy[num].way != enemy[num].next_way)
			{
				if (Enemy_Rotaion(enemy[num].next_way, num))//(횐전할 방향, 유령 번호)
					break;
			}
		}
		else
		{
			enemy[num].y -= 10;
		}
		break;
	case 1://하
		if ((enemy[num].y % 40) == 0) {
			enemy[num].next_way = (ran % 2) + 2;
			if (tile[enemy[num].Index_Y + 1][enemy[num].Index_X].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				enemy[num].y += 10;
				enemy[num].Index_Y += 1;
			}
			else if (enemy[num].way != enemy[num].next_way)
			{
				if (Enemy_Rotaion(enemy[num].next_way, num))//(횐전할 방향, 유령 번호)
					break;
			}
		}
		else
		{
			enemy[num].y += 10;
		}
		break;
	case 2://좌
		if ((enemy[num].x % 40) == 0) {
			enemy[num].next_way = (ran % 2);
			if (tile[enemy[num].Index_Y][enemy[num].Index_X - 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				enemy[num].x -= 10;
				enemy[num].Index_X -= 1;
			}
			else if (enemy[num].way != enemy[num].next_way)
			{
				if (Enemy_Rotaion(enemy[num].next_way, num))//(횐전할 방향, 유령 번호)
					break;
			}
		}
		else
		{
			enemy[num].x -= 10;
		}
		break;
	case 3://우
		if ((enemy[num].x % 40) == 0) {
			enemy[num].next_way = (ran % 2);
			if (tile[enemy[num].Index_Y][enemy[num].Index_X + 1].draw_type != WALL)// 다음 블록이 벽이 아닌지 확인
			{
				enemy[num].x += 10;
				enemy[num].Index_X += 1;
			}
			else if (enemy[num].way != enemy[num].next_way)
			{
				if (Enemy_Rotaion(enemy[num].next_way, num))//(횐전할 방향, 유령 번호)
					break;
			}
		}
		else
		{
			enemy[num].x += 10;
		}
		break;
	}
}
void PlayManager::OnTimer()
{
	RECT crt;
	HDC hDC, hMemDC;
	HBITMAP OldBit;
	static int n = 0;
	static int m = 0;
	GetClientRect(hWndMain, &crt);
	hDC = GetDC(hWndMain);

	hBit = CreateCompatibleBitmap(hDC, crt.right, crt.bottom);

	hMemDC = CreateCompatibleDC(hDC);
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW));

	for (int i = 0; i < 17; i++)//맵 그리기 반복문
	{
		for (int j = 0; j < 17; j++)
		{
			if (tile[i][j].draw_type == WALL) {
				DrawBitmap(hMemDC, tile[i][j].x, tile[i][j].y, bit_wall);//벽 그리기
			}
			else if (tile[i][j].draw_type == FOOD)
			{
				DrawBitmap(hMemDC, tile[i][j].x, tile[i][j].y, bit_eating);//먹이 그리기
			}
			else if (tile[i][j].draw_type == EMPTY)
			{
				DrawBitmap(hMemDC, tile[i][j].x, tile[i][j].y, bit_empty);//빈 공간 그리기
			}
		}
	}

	DrawBitmap(hMemDC, player.x, player.y, n, bit_player);//캐릭터 그리기
	DrawBitmap(hMemDC, enemy[0].x, enemy[0].y, m, bit_enemy[0]);// 유령 그리기
	DrawBitmap(hMemDC, enemy[1].x, enemy[1].y, m, bit_enemy[1]);// 유령 그리기
	DrawBitmap(hMemDC, enemy[2].x, enemy[2].y, m, bit_enemy[2]);// 유령 그리기
	DrawBitmap(hMemDC, enemy[3].x, enemy[3].y, m, bit_enemy[3]);// 유령 그리기

	n += 40;//애니메이션을 위한 hMemDC의  x좌표 시작위치 증가	
	if (n >= 200)
		n = 0;

	m += 40;
	if (m >= 80)
		m = 0;
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hDC);
	InvalidateRect(hWndMain, NULL, FALSE);			// 갱신
}
void PlayManager::DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	BitBlt(hdc, x - SIZE, y - SIZE, bx, by, MemDC, 0, 0, SRCCOPY);
	/*memdc에 있는 그림을 (0,0)부터 가로332 세로 240 만큼의 그림을 hdc의 (0,0)에 그려준다. */

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void PlayManager::DrawBitmap(HDC hdc, int x, int y, int n, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	TransparentBlt(hdc, x - SIZE, y - SIZE, 40, 40, MemDC, n, 0, 40, 40, RGB(255, 0, 255));
	/*hdc의 (x - SIZE, y - SIZE)위치에 가로 40,세로40,MemDC의 (n,0)위치부터 가로40,세로40,RGB(255,0,255)색 제외
	(애니메이션을 위해 n값(가로 시작 위치)이 40씩 증가된다)*/
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void PlayManager::Eat()
{
	if (tile[player.Index_Y][player.Index_X].draw_type == FOOD) {
		tile[player.Index_Y][player.Index_X].draw_type = EMPTY;//타일의 타입을 먹이->빈공간 으로 변경
	}
}
bool PlayManager::Win_Check()
{
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (tile[i][j].draw_type == FOOD)//타일 전체에서 먹이가 있는지 검사
				return false;//먹이가 1개라도 나오면  false 리턴
		}
	}
	return true;//먹이가 없으면 true 리턴
}

/*유령과 팩맨이 닿았는지 확인*/
bool PlayManager::Catch_Player()
{
	for (int i = 0; i < 4; i++)
	{
		if (LengthPts(player.x, player.y, enemy[i].x, enemy[i].y) < 30)//팩맨과 유령의 거리가 30미만이면
			return true;//true 리턴
	}
	return false;
}