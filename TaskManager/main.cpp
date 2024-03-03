#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <Tlhelp32.h>

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp) {
	switch (mes)
	{
	case WM_INITDIALOG: {
	
	}
	break;
	case WM_COMMAND: {
		switch (LOWORD(wp)) {
		case IDC_BUTTON1: {
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe32;
			memset(&pe32, 0, sizeof(PROCESSENTRY32));
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnapShot, &pe32)) {
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
				while (Process32Next(hSnapShot, &pe32)) {
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
				}
			}
			CloseHandle(hSnapShot);
		}
		break;
		case IDC_BUTTON2: {
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			int selectedIndex = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (selectedIndex != LB_ERR) {
				TCHAR szProcessName[MAX_PATH];
				SendMessage(hList, LB_GETTEXT, selectedIndex, (LPARAM)szProcessName);
				HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

				PROCESSENTRY32 pe32;
				memset(&pe32, 0, sizeof(PROCESSENTRY32));
				pe32.dwSize = sizeof(PROCESSENTRY32);
				if (Process32First(hSnapShot, &pe32)) {
					do {
						if (_tcscmp(szProcessName, pe32.szExeFile) == 0) {
							HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
							if (hProcess != NULL) {
								TerminateProcess(hProcess, 0);
								CloseHandle(hProcess);
							}
							break;
						}
					} while (Process32Next(hSnapShot, &pe32));
				}
				CloseHandle(hSnapShot);
			}
		}
		break;
		case IDC_BUTTON3: {
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			int selectedIndex = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (selectedIndex != LB_ERR) {
				TCHAR szProcessName[MAX_PATH];
				SendMessage(hList, LB_GETTEXT, selectedIndex, (LPARAM)szProcessName);
				HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				PROCESSENTRY32 pe32;
				memset(&pe32, 0, sizeof(PROCESSENTRY32));
				pe32.dwSize = sizeof(PROCESSENTRY32);
				if (Process32First(hSnapShot, &pe32)) {
					do {
						if (_tcscmp(szProcessName, pe32.szExeFile) == 0) {
							TCHAR szMessage[MAX_PATH];
							wsprintf(szMessage, _T("ID: %u\nThreads: %u\nPriority: %u\nName: %s"), pe32.th32ProcessID, pe32.cntThreads, pe32.pcPriClassBase, pe32.szExeFile);
							MessageBox(hWnd, szMessage, _T("Process Information"), MB_OK | MB_ICONINFORMATION);
							break;
						}
					} while (Process32Next(hSnapShot, &pe32));
				}
				CloseHandle(hSnapShot);
			}
		}
		break;
		case IDC_BUTTON4: {
			TCHAR szProcessName[MAX_PATH];
			GetDlgItemText(hWnd, IDC_EDIT1, szProcessName, MAX_PATH);
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			if (!CreateProcess(NULL, szProcessName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
				MessageBox(hWnd, _T("Failed to create process."), _T("Error"), MB_OK | MB_ICONERROR);
			}
			else {
				MessageBox(hWnd, _T("Process created successfully."), _T("Success"), MB_OK | MB_ICONINFORMATION);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
		}
		break;
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(hWnd, 0);
		return TRUE;
	}
	}
	return FALSE;
}