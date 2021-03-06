// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "KDeque.h"
#include "stream.h"
#include "resource.h"
#include <fstream>
#include <vector>
HWND _hdialog;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
double get_value(KIstream& is) {
	double buf;
	is >> buf;
	return buf;
}
template<class T,size_t grow_const>
void modify(KDeque<T, grow_const>& deq, KIstream& is, HWND hwnd, int id_list_box) {
	T buf;
	is >> buf;
	int i = SendDlgItemMessage(hwnd, id_list_box, LB_GETCURSEL, 0, 0);
	if (i >= deq.size()||i<0)
		throw std::runtime_error("invalid item selected");
	deq[i] = buf;
}

KDeque<double>a, b;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	MSG message;
	while (GetMessage(&message, nullptr, 0, 0)) {
		if (!IsDialogMessageW(_hdialog, &message)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		_hdialog = hwnd;
		std::ifstream fin("data.txt");
		fin >> a >> b;

		KOstream winapi_out_A(GetDlgItem(hwnd, IDC_LIST_A)), winapi_out_B(GetDlgItem(hwnd, IDC_LIST_B));
		winapi_out_A << a;
		winapi_out_B << b;

		break;
	}
	case WM_COMMAND: {
		KOstream winapi_out_A(GetDlgItem(hwnd, IDC_LIST_A)), winapi_out_B(GetDlgItem(hwnd, IDC_LIST_B)), winapi_out_Result(GetDlgItem(hwnd, IDC_LIST_Result));
		KIstream winapi_in_A(GetDlgItem(hwnd, IDC_EDIT_A)), winapi_in_B(GetDlgItem(hwnd, IDC_EDIT_B));
		auto control_id = LOWORD(wparam);
		bool changed = true;
		winapi_out_Result.clear();
		try {
			switch (control_id)
			{
				//Удаление
			case IDC_BUTTON_Pop_Front:
				a.pop_front();
				break;
			case IDC_BUTTON_Pop_Front2:
				b.pop_front();
				break;
			case IDC_BUTTON_Clear:
				a.clear();
				break;
			case IDC_BUTTON_Clear2:
				b.clear();
				break;
			case IDC_BUTTON_Pop_Back:
				a.pop_back();
				break;
			case IDC_BUTTON_Pop_Back2:
				b.pop_back();
				break;
				//Добавление
			case IDC_PushFront:
				a.emplace_front(get_value(winapi_in_A));
				break;

			case IDC_PushFront2:
				b.emplace_front(get_value(winapi_in_B));
				break;

			case IDC_Push_Back:
				a.emplace_back(get_value(winapi_in_A));
				break;

			case IDC_Push_Back2:
				b.emplace_back(get_value(winapi_in_B));
				break;

			case IDC_Modify:
				modify(a, winapi_in_A, hwnd, IDC_LIST_A);
				break;

			case IDC_Modify2:
				modify(b, winapi_in_B, hwnd, IDC_LIST_B);
				break;

				//Actions
			case IDC_BUTTON_Plus:
				winapi_out_Result << (a + b);
				break;

			case IDC_BUTTON_PlusEq:
				a += b;
				break;

			case IDC_BUTTON_Eq:
				a = b;
				break;

			case IDC_BUTTON_Bool_Eq:
				winapi_out_Result << (a == b);
				break;

			case IDC_BUTTON_Swap:
				swap(a, b);
				break;

			default:
				changed = false;
				break;
			}
		}
		catch (std::exception& e) {
			MessageBoxA(hwnd, e.what(), "Error", MB_OK | MB_ICONERROR);
			a.clear();
			b.clear();
		}

		if (changed) {
			winapi_out_A << a;
			winapi_out_B << b;
		}
		
		break;
	}

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_DESTROY:
		DestroyWindow(hwnd);
		exit(0);
		break;

	default:
		//DefDlgProc(hwnd, msg, wparam, lparam);
		return FALSE;
	}
	return TRUE;
}