#include <tchar.h>
#include "resource.h"

#include <iostream>
#include <string>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

const int MAXSTRLEN = 256;
WSADATA wsaData;

SOCKET _socket;
sockaddr_in addr;

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wp))
		{
		case IDC_BUTTON1:
		{
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			addr.sin_family = AF_INET;

			inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

			addr.sin_port = htons(20000);

			connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
		}
		break;
		case IDC_BUTTON2:
		{
			char buf[MAXSTRLEN];

			HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
			GetWindowTextA(hEdit, buf, MAXSTRLEN);

			send(_socket, buf, strlen(buf), 0);

			char buf1[MAXSTRLEN];

			int i = recv(_socket, buf1, MAXSTRLEN - 1, 0);

			buf1[i] = '\0';
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)buf1);

			closesocket(_socket);

			WSACleanup();
		}
		break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}