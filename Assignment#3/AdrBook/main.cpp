#include "main.h"
#include "drawing.h"
#include "listElements.h"
#include <fstream>
#include <iostream>
HINSTANCE nInst;

#define abs(x) (x) > 0 ? (x) : (-(x))

int frame = 0;
bool searchOn = false;

table<object_rectangle<int>> topBar, botBar;
table<object_image> botIcon, topIcon;
table<object_text> topBarText;


listElements musicList;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX win_main;

	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

	HICON hICON = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	win_main.hInstance = nInst = hInstance;
	win_main.style = CS_HREDRAW | CS_VREDRAW;
	win_main.cbSize = sizeof(WNDCLASSEX);
	win_main.cbClsExtra = 0;
	win_main.cbWndExtra = 0;
	win_main.lpszClassName = TITLE;
	win_main.lpszMenuName = NULL;
	win_main.lpfnWndProc = WindowProcedure;
	win_main.hIcon = hICON;
	win_main.hIconSm = hICON;
	win_main.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClassEx(&win_main))
		return 0;

	hwnd = CreateWindowEx(0, TITLE, TITLE, WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
		CW_USEDEFAULT, SIZEWW, SIZEHH, HWND_DESKTOP, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);

	ZeroMemory(&messages, sizeof(messages));
	while (messages.message != WM_QUIT)
	{
		if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}
	return messages.wParam;
}

bool wheelTimer = false;
void CALLBACK wheelTimerProc(HWND hwnd, UINT uMsg, UINT id, DWORD dwTime)
{
	switch (id)
	{
		case 10:
		{
			musicList.y -= (abs(musicList.y - 40)) / 7;
			InvalidateRect(hwnd, NULL, FALSE);
			if (musicList.y < 41)
				KillTimer(hwnd, 10);
			break;
		}
		case 11:
		{
			musicList.y += (abs(musicList.y - SIZEW + musicList.maxY())/ 7);
			InvalidateRect(hwnd, NULL, FALSE);
			if (musicList.y > SIZEW - musicList.maxY())
				KillTimer(hwnd, 11);
			break;
		}
		default:
			break;
	}
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			object_rectangle<int>	tbox(-2, 1, SIZEW + 4, 40, true, 1, 2, 0xffffff, 0x00000),
									bbox(-2, SIZEH-60, SIZEW + 4, 60, true, 1, 2, 0xffffff, 0x00000);
			topBar.setVisible(true);
			botBar.setVisible(true);
			topBar.setPivot(tbox);
			botBar.setPivot(bbox);

			object_text textbox(-2, 1, SIZEW + 4, 50, true, 0, ""),
						nowPlay(95, 9, 275, 25, true, 22, "지금 재생중인 음악"),
						searchText(95, 9, 275, 25, false, 22, "");
			topBarText.setPivot(textbox);
			topBarText.insert(nowPlay);
			topBarText.insert(searchText);

			object_image topIconMap(LoadBitmap(nInst, MAKEINTRESOURCE(TOPBARICONS)), 0, 5, 0, 0, false),
				iPlay(NULL, 5, 0, 30, 30, false, 0, 0), iPause(NULL, 5, 0, 30, 30, true, 60, 0),
				iNext(NULL, 45, 0, 30, 30, true, 0, 30), iSearch(NULL, SIZEW - 35, 0, 30, 30, true, 60, 30);
			topIcon.setVisible(false);
			topIcon.setPivot(topIconMap);
			topIcon.insert(iPlay);
			topIcon.insert(iPause);
			topIcon.insert(iNext);
			topIcon.insert(iSearch);

			object_image botIconMap(LoadBitmap(nInst, MAKEINTRESOURCE(MENUICONS)), 0, SIZEH-60, 0, 0, false),
				iMusic(NULL, 40, 5, 50, 50, true, 0, 0), iPlaylist(NULL, 130, 5, 50, 50, true, 100, 0),
				iGenre(NULL, 220, 5, 50, 50, true, 0, 50), iAlbum(NULL, 310, 5, 50, 50, true, 100, 50);
			botIcon.setVisible(false);
			botIcon.setPivot(botIconMap);
			botIcon.insert(iMusic);
			botIcon.insert(iPlaylist);
			botIcon.insert(iGenre);
			botIcon.insert(iAlbum);

			musicList.x = 5;
			musicList.y = 40;
			musicList.w = SIZEW - 10;
			musicList.h = SIZEH - 40 - 60;
			string temp;
			ifstream in("musicList.ini");
			int size; in >> size;
			getline(in, temp);
			while (size--)
			{
				string name, album, artist;
				getline(in, name);
				getline(in, album);
				getline(in, artist);
				musicList.insert(createInfo(name, album, artist));
			}
			break;
		}
		case WM_TIMER:
		{
			break;
		}
		case WM_SIZE:
		{
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int e = ((SHORT)HIWORD(wParam) > 0) ? (-15) : (((SHORT)HIWORD(wParam) < 0) ? (15) : (0));
			musicList.y += e;
			InvalidateRect(hwnd, NULL, FALSE);
			if (musicList.y < SIZEW - musicList.maxY())
			{
				KillTimer(hwnd, 10);
				KillTimer(hwnd, 11);
				SetTimer(hwnd, 11, 20, wheelTimerProc);
			}
			else if (musicList.y > 41)
			{
				KillTimer(hwnd, 10);
				KillTimer(hwnd, 11);
				SetTimer(hwnd, 10, 20, wheelTimerProc);
			}
			break;
		}
		case WM_KEYDOWN:
		{

			if (searchOn)
			{
				if (topBarText.find(2).text == "검색어를 입력하세요.")
					topBarText.find(2).text = "";
				
				switch (wParam)
				{
					case VK_BACK:
						if (topBarText.find(2).text.size())
							topBarText.find(2).text.pop_back();
						break;
					case VK_RETURN:
						break;
					default:
						if ((wParam >= 'a' && wParam <= 'z') || (wParam >= 'A' && wParam <= 'Z'))
							topBarText.find(2).text += (char)(wParam-'A'+'a');
						break;
				}
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			point.x = lParam & 0xffff;
			point.y = lParam >> 16;

			if (point.y < 40 && point.y > 5)
				if (point.x > SIZEW - 35)
				{
					searchOn = !searchOn;
					topBarText.find(1).visible = !topBarText.find(1).visible;
					topBarText.find(2).visible = !topBarText.find(2).visible;
					if (searchOn)
						topBarText.find(2).text = "검색어를 입력하세요.";
					InvalidateRect(hwnd, NULL, FALSE);
				}
			else if (point.y > 50 && point.y < SIZEH - 100)
			{
			
			}

			break;
		}
		case WM_COMMAND:
		{
			break;
		}
		case WM_MOUSEMOVE:
		{
			point.x = lParam & 0xffff;
			point.y = lParam >> 16;
			//if (!mainPage)
				//InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			RECT		mRC;
			HDC			hdc, mDC;
			HBITMAP		mBitmap, mOldBitmap;

			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &mRC);
			mDC = CreateCompatibleDC(hdc);
			mBitmap = CreateCompatibleBitmap(hdc, mRC.right, mRC.bottom);
			mOldBitmap = (HBITMAP)SelectObject(mDC, mBitmap);
			PatBlt(mDC, 0, 0, mRC.right, mRC.bottom, WHITENESS);
			SetBkMode(mDC, TRANSPARENT);
			SetStretchBltMode(mDC, COLORONCOLOR);
			{//draw
				draw_loop_list(&mDC, musicList, "");

				draw_loop_proc(&mDC, botBar);
				draw_loop_blt(&mDC, botIcon);

				draw_loop_proc(&mDC, topBar);
				draw_loop_text(&mDC, topBarText);
				draw_loop_blt(&mDC, topIcon);

			}
			BitBlt(hdc, 0, 0, mRC.right, mRC.bottom, mDC, 0, 0, SRCCOPY);
			SelectObject(mDC, mOldBitmap);
			DeleteObject(mBitmap);
			DeleteDC(mDC);

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

