#pragma once
#include "main.h"
#include <fstream>
#include <iostream>

HINSTANCE nInst;
#define abs(x) (x) > 0 ? (x) : (-(x))

#define SCROLL_UPPER 10
#define SCROLL_lOWER 11

int frame = 0;
int yScr = 0;

table<object_rectangle<int>> topBar, botBar;
table<object_image> botIcon, topIcon;
table<object_text> topBarText;

listElements musicList(0), searchList(0), genreList(1), albumList(1), * nList, * bList;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX win_main;

	HICON hICON = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(APP_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	char szTitle[50], szClass[50];
	LoadString(hInstance, APP_NAME, szTitle, 32);
	LoadString(hInstance, APP_CLASS_NAME, szClass, 32);
	
	win_main.cbSize = sizeof(WNDCLASSEX);
	win_main.hInstance = nInst = hInstance;
	win_main.style = CS_HREDRAW | CS_VREDRAW;
	win_main.cbClsExtra = 0;
	win_main.cbWndExtra = 0;
	win_main.lpszClassName = szClass;
	win_main.lpszMenuName = NULL;
	win_main.lpfnWndProc = WindowProcedure;
	win_main.hIcon = hICON;
	win_main.hIconSm = hICON;
	win_main.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClassEx(&win_main))
		return 0;

	hwnd = CreateWindow(szClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
		CW_USEDEFAULT, SIZEWW, SIZEHH, HWND_DESKTOP, NULL, hInstance, NULL);
	
	if (!hwnd)
		return -1;

	SetProcessWorkingSetSize(hwnd, -1, -1);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

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


string GetInputText(HWND dlg, int resid) {
	HWND hc = GetDlgItem(dlg, resid);
	int n = GetWindowTextLength(hc) + 1;
	string s(n, 0);
	GetWindowText(hc, &s[0], n);
	return s;
}

bool musicInput = false;
musicInfo inputed;
BOOL CALLBACK musicInputProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
	switch (uMsg) 
	{
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND: 
		{
			int ctl = LOWORD(wp);
			int event = HIWORD(wp);
				if (ctl == INPUT_OK) 
				{
					inputed.name = GetInputText(hwnd, NAME);
					inputed.album = GetInputText(hwnd, ALBUM);
					inputed.artist = GetInputText(hwnd, ARTIST);
					inputed.genre = GetInputText(hwnd, GENRE);
					DestroyWindow(hwnd);
					return TRUE;
				}
			return FALSE;
			}
		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return TRUE;
	}
	return FALSE;
}
bool wheelTimer = false;
void CALLBACK wheelTimerProc(HWND hwnd, UINT uMsg, UINT id, DWORD dwTime)
{
	switch (id)
	{
		case 10:
		{
			nList->y -= (abs(nList->y)) / 10;
			InvalidateRect(hwnd, NULL, FALSE);
			if (nList->y < 0)
				KillTimer(hwnd, 10);
			break;
		}
		case 11:
		{
			nList->y += (abs(nList->y - SIZEW + nList->maxY())/ 10);
			InvalidateRect(hwnd, NULL, FALSE);
			if (nList->y > SIZEW - nList->maxY())
				KillTimer(hwnd, 11);
			break;
		}
		default:
			break;
	}
}

bool searchOn = false;
void keySearch(const string& str, HWND& hwnd)
{
	if (str.size())
	{
		vector<musicInfo> result, temp;
		searchList.clear();
		if (nList != &searchList)
			bList = nList;
		nList = &searchList;
		temp = bList->findName(str);
		for (musicInfo info : temp)
			result.push_back(info);
		temp = bList->findAlbum(str);
		for (musicInfo info : temp)
			result.push_back(info);
		temp = bList->findArtist(str);
		for (musicInfo info : temp)
			result.push_back(info);
		sort(result.begin(), result.end());
		result.erase(unique(result.begin(), result.end()), result.end());
		for (musicInfo info : result)
			searchList.insert(info);
		searchList.sort();
	}
	else
		nList = bList;
	nList->y = 50;
	nList->select = 0;
	SetTimer(hwnd, 10, 10, wheelTimerProc);
}
void searchOff()
{
	searchOn = false;
	topBarText.find(1).visible = true;
	topBarText.find(2).visible = false;
	if(bList != NULL)
		nList = bList;
}

void changeFrame(int f, HWND& hwnd)
{
	musicList.y = genreList.y = 40;

	KillTimer(hwnd, 1);
	KillTimer(hwnd, 11);
	switch (f)
	{
	case 0:
		nList = &musicList;
		break;
	case 1:
		nList = &genreList;
		break;
	case 2:
		nList = &albumList;
		break;
	}
	bList = nList;
	searchOff();
	nList->y = 200;
	nList->select = 0;
	SetTimer(hwnd, 10, 10, wheelTimerProc);
}

bool lOpen = false;
void musicListOpen(string file, HWND& hWnd)
{
	string temp;
	ifstream in(file);
	if (!in.is_open())
		return;

	musicList.clear();
	genreList.clear();
	albumList.clear();

	int size; in >> size;
	getline(in, temp);
	while (size--)
	{
		string name, album, artist, genre;
		getline(in, name);
		getline(in, album);
		getline(in, artist);
		getline(in, genre);
		musicList.insert(createInfo(name, album, artist, genre));
		albumList.insert(createInfo(name, album, artist, genre));
		genreList.insert(createInfo(name, genre, album, artist ));
	}

	in.close();

	genreList.sort();
	musicList.sort();
	albumList.sort();

	changeFrame(0,hWnd);
	InvalidateRect(hWnd, NULL, FALSE);
}
void musicListSave(string file, HWND& hwnd)
{
	string temp;
	ofstream out(file);
	if (!out.is_open())
		return;

	out << musicList.size() << endl;
	FOREACH_LIST(musicList, i, info)
	{
		out << info.name << endl << info.album << endl
			<< info.artist << endl << info.genre << endl;
	}

	out.close();
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
						nowPlay(115, 9, 275, 25, true, 22, "지금 재생중인 음악"),
						searchText(115, 9, 275, 25, false, 22, "");
			topBarText.setPivot(textbox);
			topBarText.insert(nowPlay);
			topBarText.insert(searchText);

			object_image topIconMap(LoadBitmap(nInst, MAKEINTRESOURCE(TOPBARICONS)), 0, 5, 0, 0, false),
				iMINUS(NULL, 42, 0, 32, 32, true, 0, 0), iADD(NULL, 5, 0, 32, 32, true, 64, 0),
				iSearch(NULL, SIZEW - 35, 0, 32, 32, true, 0, 32), iSave(NULL, 79, 0, 32, 32, true, 64, 32);
			topIcon.setVisible(false);
			topIcon.setPivot(topIconMap);
			topIcon.insert(iADD);
			topIcon.insert(iMINUS);
			topIcon.insert(iSave);
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

			albumList.x = genreList.x = musicList.x = searchList.x = 5;
			albumList.y = genreList.y = musicList.y = searchList.y = 0;
			albumList.w = genreList.w = musicList.w = searchList.w = SIZEW - 10;
			albumList.h = genreList.h = musicList.h = searchList.h = SIZEH - 40 - 60;
			albumList.margin = genreList.margin = musicList.margin = searchList.margin = 50;

			musicListOpen("musicList.ini", hwnd);
			changeFrame(0, hwnd);
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
			nList->y += e;

			InvalidateRect(hwnd, NULL, FALSE);
			if (nList->y < SIZEW - nList->maxY())
			{
				KillTimer(hwnd, 10);
				KillTimer(hwnd, 11);
				SetTimer(hwnd, 11, 10, wheelTimerProc);
			}
			else if (nList->y > 0)
			{
				KillTimer(hwnd, 10);
				KillTimer(hwnd, 11);
				SetTimer(hwnd, 10, 10, wheelTimerProc);
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
					{
						topBarText.find(2).text.pop_back();
						keySearch(topBarText.find(2).text, hwnd);
					}
					break;
				case VK_SPACE:
					topBarText.find(2).text += ' ';
					break;
				case VK_ESCAPE:
				case VK_RETURN:
				{
					if (searchOn)
						searchOff();
					break;
				}
				default:
					if (wParam >= 'A' && wParam <= 'Z')
					{
						topBarText.find(2).text += (char)(wParam - 'A' + 'a');
						keySearch(topBarText.find(2).text, hwnd);
					}
					else if (wParam >= '0' && wParam <= '9')
					{
						topBarText.find(2).text += (char)(wParam);
						keySearch(topBarText.find(2).text, hwnd);
					}
					break;
				}
			}
			else
				switch (wParam)
				{
					case VK_DOWN:
						nList->y -= 25;

						InvalidateRect(hwnd, NULL, FALSE);
						if (nList->y < SIZEW - nList->maxY())
						{
							KillTimer(hwnd, 10);
							KillTimer(hwnd, 11);
							SetTimer(hwnd, 11, 10, wheelTimerProc);
						}
						break;
					case VK_UP:
						nList->y += 25;

						InvalidateRect(hwnd, NULL, FALSE);
						if (nList->y > 41)
						{
							KillTimer(hwnd, 10);
							KillTimer(hwnd, 11);
							SetTimer(hwnd, 10, 10, wheelTimerProc);
						}
						break;
					case VK_DELETE:
						{
							musicInfo info = nList->find(nList->select);
							musicList.remove(info);
							albumList.remove(info);
							genreList.remove(createInfo(info.name, info.genre, info.album, info.artist));
							break;
						}
					case VK_INSERT:
						if (!musicInput)
						{
							musicInput = true;
							HWND add = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(MUSICINPUT), hwnd, musicInputProc);
							MSG mmsg;
							ShowWindow(add, 10);
							while (GetMessage(&mmsg, 0, 0, 0))
							{
								if (!IsDialogMessage(add, &mmsg))
								{
									TranslateMessage(&mmsg);
									DispatchMessage(&mmsg);
								}
							}

							size_t idx = musicList.insert(inputed);
							albumList.insert(inputed);
							genreList.insert(inputed);
							musicList.sort();
							albumList.sort();
							genreList.sort();

							nList->select = idx;

							musicInput = false;
							InvalidateRect(hwnd, NULL, FALSE);
						}
						break;
					case VK_RETURN:
						searchOff();
						break;
					default:
					{//단축키
						case 'O':
							{//불러오기
								OpenFileDialog* oFD = new OpenFileDialog();

								oFD->FilterIndex = 1;
								oFD->Flags |= OFN_SHOWHELP;
								oFD->InitialDir = "";
								oFD->Title = "Open Music List File";
								if (!lOpen)
								{
									lOpen = true;
									if (oFD->ShowDialog())
										musicListOpen(oFD->FileName, hwnd);
									lOpen = false;
								}
								break;
							}
						case 'S':
							{//저장
								SaveFileDialog* oFD = new SaveFileDialog();

								oFD->FilterIndex = 1;
								oFD->Flags |= OFN_SHOWHELP;
								oFD->InitialDir = "";
								oFD->Title = "Save to Music List File";
								if (!lOpen)
								{
									lOpen = true;
									if (oFD->ShowDialog())
										musicListSave(oFD->FileName, hwnd);
									lOpen = false;
								}
								break;
							}
						case 'F':
							{//찾기
								searchOn = true;
								searchList.clear();
								topBarText.find(1).visible = false;
								topBarText.find(2).text = "검색어를 입력하세요.";
								topBarText.find(2).visible = true;
								break;
							}
					}
				}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			point.x = lParam & 0xffff;
			point.y = lParam >> 16;

			if (point.y < 40 && point.y > 5)
			{
				if (point.x < 37)
				{//ADD
					if (!musicInput && !searchOn)
					{
						musicInput = true;
						HWND add = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(MUSICINPUT),hwnd, musicInputProc);
						MSG mmsg;
						ShowWindow(add, 10);
						while (GetMessage(&mmsg, 0, 0, 0))
						{
							if (!IsDialogMessage(add, &mmsg))
							{
								TranslateMessage(&mmsg);
								DispatchMessage(&mmsg);
							}
						}
						
						
						size_t idx = musicList.insert(inputed);
						albumList.insert(inputed);
						genreList.insert(inputed);
						musicList.sort();
						albumList.sort();
						genreList.sort();

						nList->select = idx;

						musicInput = false;
						InvalidateRect(hwnd, NULL, FALSE);
					}
				}
				else if (point.x < 74)
				{//Delete
					if (!searchOn)
					{
						musicInfo info = nList->find(nList->select);
						musicList.remove(info);
						albumList.remove(info);
						genreList.remove(createInfo(info.name,info.genre,info.album,info.artist));
					}
				}
				else if (point.x < 111)
				{//save
					SaveFileDialog* oFD = new SaveFileDialog();
					
					oFD->FilterIndex = 1;
					oFD->Flags |= OFN_SHOWHELP;
					oFD->InitialDir = "";
					oFD->Title = "Save to Music List File";
					if (!lOpen)
					{
						lOpen = true;
						if (oFD->ShowDialog())
							musicListSave(oFD->FileName, hwnd);
						lOpen = false;
					}
				}
				else if (point.x > SIZEW - 35)
				{
					if (searchOn)
						searchOff();
					else
					{
						searchOn = true;
						searchList.clear();
						topBarText.find(1).visible = false;
						topBarText.find(2).visible = true;
						topBarText.find(2).text = "검색어를 입력하세요.";
					}
				}
			}
			else if (point.y > 50 && point.y < SIZEH - 55)
			{
				for (int i = 0; i < nList->size(); i++)
					if (-nList->y + point.y  < 30 + nList->margin * (i+1))
					{
						nList->select = i;
						break;
					}
			}
			else if (point.y > SIZEH - 55)
			{
				switch (point.x / 100)
				{
					case 0:
						changeFrame(0, hwnd);
						break;
					case 1:
					{
						OpenFileDialog* oFD = new OpenFileDialog();

						oFD->FilterIndex = 1;
						oFD->Flags |= OFN_SHOWHELP;
						oFD->InitialDir = "";
						oFD->Title = "Open Music List File";
						if (!lOpen)
						{
							lOpen = true;
							if (oFD->ShowDialog())
								musicListOpen(oFD->FileName,hwnd);
							lOpen = false;
						}
					}
						break;
					case 2:
						changeFrame(1,hwnd);
						break;
					case 3:
						changeFrame(2, hwnd);
						break;
					default:
						break;
					break;
				}
			}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			
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
				if(searchOn && !nList->size())
					drawText(&mDC, 100, 300, 250, 200, 22, "일치하는 결과가 없습니다.");
				else
					draw_loop_list(&mDC, (*nList));

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

