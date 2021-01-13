#include <windows.h>
#include <string>
#pragma warning(disable : 4996)

#define ID_0 4000
#define ID_1 4001
#define ID_2 4002
#define ID_3 4003
#define ID_4 4004
#define ID_5 4005
#define ID_6 4006
#define ID_7 4007
#define ID_8 4008
#define ID_9 4009
#define ID_CE 4010
#define ID_C 4011
#define ID_UNDO 4012
#define ID_SUBSTRACT 4013
#define ID_ADD 4014
#define ID_MULTIPLE 4015
#define ID_DIVIDE 4016
#define ID_NEGATION 4017
#define ID_COMMA 4018
#define ID_EQUAL 4019

enum LastOperation {
	idle,
	add,
	substract,
	multiple,
	divide
};

HWND editWindow, buttonCE, buttonC, buttonUndo, buttonDivide, button7, button8, button9, buttonMultiple,
button4, button5, button6, buttonMinus, button1, button2, button3, buttonPlus, buttonNegative,
button0, buttonComma, buttonEqual;

int numberQueue[20];
int queueCount = 0;
char* beforeNumber = new char;
LPCSTR display = "0";
int number = 0;
bool editWindowReset = true;
LastOperation lastOperation = idle;
BOOL bSuccess;
bool firstPress = true;
bool canUndo = true;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE: {

		//EDIT WINDOW
		editWindow = CreateWindow("static", display, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 10, 260, 50, hwnd, NULL, GetModuleHandle(NULL), NULL);

		//FIRST ROW
		buttonCE = CreateWindow("button", "CE",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 80, 60, 60, hwnd, (HMENU)ID_CE, GetModuleHandle(NULL), NULL);
		buttonC = CreateWindow("button", "C",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			75, 80, 60, 60, hwnd, (HMENU)ID_C, GetModuleHandle(NULL), NULL);
		buttonUndo = CreateWindow("button", "UNDO",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 80, 60, 60, hwnd, (HMENU)ID_UNDO, GetModuleHandle(NULL), NULL);
		buttonDivide = CreateWindow("button", "/",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			205, 80, 60, 60, hwnd, (HMENU)ID_DIVIDE, GetModuleHandle(NULL), NULL);

		//SECOND ROW
		button7 = CreateWindow("button", "7",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 145, 60, 60, hwnd, (HMENU)ID_7, GetModuleHandle(NULL), NULL);
		button8 = CreateWindow("button", "8",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			75, 145, 60, 60, hwnd, (HMENU)ID_8, GetModuleHandle(NULL), NULL);
		button9 = CreateWindow("button", "9",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 145, 60, 60, hwnd, (HMENU)ID_9, GetModuleHandle(NULL), NULL);
		buttonMultiple = CreateWindow("button", "X",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			205, 145, 60, 60, hwnd, (HMENU)ID_MULTIPLE, GetModuleHandle(NULL), NULL);

		//THIRD ROW
		button4 = CreateWindow("button", "4",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 210, 60, 60, hwnd, (HMENU)ID_4, GetModuleHandle(NULL), NULL);
		button5 = CreateWindow("button", "5",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			75, 210, 60, 60, hwnd, (HMENU)ID_5, GetModuleHandle(NULL), NULL);
		button6 = CreateWindow("button", "6",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 210, 60, 60, hwnd, (HMENU)ID_6, GetModuleHandle(NULL), NULL);
		buttonMinus = CreateWindow("button", "-",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			205, 210, 60, 60, hwnd, (HMENU)ID_SUBSTRACT, GetModuleHandle(NULL), NULL);

		//FOURTH ROW
		button1 = CreateWindow("button", "1",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 275, 60, 60, hwnd, (HMENU)ID_1, GetModuleHandle(NULL), NULL);
		button2 = CreateWindow("button", "2",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			75, 275, 60, 60, hwnd, (HMENU)ID_2, GetModuleHandle(NULL), NULL);
		button3 = CreateWindow("button", "3",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 275, 60, 60, hwnd, (HMENU)ID_3, GetModuleHandle(NULL), NULL);
		buttonPlus = CreateWindow("button", "+",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			205, 275, 60, 60, hwnd, (HMENU)ID_ADD, GetModuleHandle(NULL), NULL);

		//FIFTH ROW
		buttonNegative = CreateWindow("button", "+/-",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 340, 60, 60, hwnd, (HMENU)ID_NEGATION, GetModuleHandle(NULL), NULL);
		button0 = CreateWindow("button", "0",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			75, 340, 60, 60, hwnd, (HMENU)ID_0, GetModuleHandle(NULL), NULL);
		buttonComma = CreateWindow("button", ",",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 340, 60, 60, hwnd, (HMENU)ID_COMMA, GetModuleHandle(NULL), NULL);
		buttonEqual = CreateWindow("button", "=",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			205, 340, 60, 60, hwnd, (HMENU)ID_EQUAL, GetModuleHandle(NULL), NULL);

		break;
	}

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case ID_0:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				//SetWindowText(editWindow, "0");
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "0");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_1:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "1");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "1");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_2:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "2");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "2");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_3:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "3");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "3");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_4:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "4");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "4");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_5:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "5");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "5");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_6:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "6");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "6");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_7:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "7");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "7");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_8:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "8");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "8");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_9:
		{
			if (GetWindowTextLength(editWindow) >= 15)
				break;

			GetWindowText(editWindow, beforeNumber, 15);

			if (editWindowReset) {
				SetWindowText(editWindow, "9");
				editWindowReset = false;
			}
			else
			{
				char* ret = new char;
				ret = strcat(beforeNumber, "9");
				SetWindowText(editWindow, ret);
			}
		}
		break;

		case ID_ADD:
		{
			canUndo = false;
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			if (lastOperation == add) {
				number += atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == substract) {
				number -= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == multiple) {
				number *= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == divide) {

				if (atoi(numberText) != 0)
					number /= atoi(numberText);

				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			lastOperation = add;

			GetWindowText(editWindow, numberText, 15);
			//int len = strlen(numberText);
			if (firstPress) {
				number = atoi(numberText);
				firstPress = false;
				editWindowReset = true;
			}
			else {
				number += atoi(numberText);
				//numberQueue[queueCount] = atoi(numberText) + numberQueue[queueCount - 1];
				//queueCount++;
				itoa(number, numberText, 10);
				std::string test = std::to_string(number);
				SetWindowText(editWindow, test.c_str());
				editWindowReset = true;
			}
		}
		break;

		case ID_MULTIPLE:
		{
			canUndo = false;
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			if (lastOperation == add) {
				number += atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == substract) {
				number -= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == multiple) {
				number *= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == divide) {

				if (atoi(numberText) != 0)
					number /= atoi(numberText);

				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			lastOperation = multiple;

			GetWindowText(editWindow, numberText, 15);
			if (firstPress) {
				number = atoi(numberText);
				firstPress = false;
				editWindowReset = true;
			}
			else {
				number *= atoi(numberText);
				itoa(number, numberText, 10);
				std::string test = std::to_string(number);
				SetWindowText(editWindow, test.c_str());
				editWindowReset = true;
			}
		}
		break;


		case ID_SUBSTRACT:
		{
			canUndo = false;
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			if (lastOperation == add) {
				number += atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == substract) {
				number -= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == multiple) {
				number *= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == divide) {

				if(atoi(numberText) != 0)
				number /= atoi(numberText);

				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			lastOperation = substract;
			
			GetWindowText(editWindow, numberText, 15);
			if (firstPress) {
				number = atoi(numberText);
				firstPress = false;
				editWindowReset = true;
			}
			else {
				number -= atoi(numberText);
				itoa(number, numberText, 10);
				std::string test = std::to_string(number);
				SetWindowText(editWindow, test.c_str());
				editWindowReset = true;
			}
		}
		break;

		case ID_DIVIDE:
		{
			canUndo = false;
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			if (lastOperation == add) {
				number += atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == substract) {
				number -= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == multiple) {
				number *= atoi(numberText);
				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			if (lastOperation == divide) {

				if (atoi(numberText) != 0)
					number /= atoi(numberText);

				firstPress = true;
				itoa(number, numberText, 10);
				SetWindowText(editWindow, numberText);
				editWindowReset = true;
				lastOperation = idle;
			}

			lastOperation = divide;

			GetWindowText(editWindow, numberText, 15);
			if (firstPress) {
				number = atoi(numberText);
				firstPress = false;
				editWindowReset = true;
			}
			else {
				number /= atoi(numberText);
				itoa(number, numberText, 10);
				std::string test = std::to_string(number);
				SetWindowText(editWindow, test.c_str());
				editWindowReset = true;
			}
		}
		break;

		case ID_EQUAL:
		{
			canUndo = false;
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			if (lastOperation == add)
				number += atoi(numberText);

			if (lastOperation == substract)
				number -= atoi(numberText);

			if (lastOperation == multiple)
				number *= atoi(numberText);

			if (lastOperation == divide) {
				if (atoi(numberText) != 0)
					number /= atoi(numberText);
			}

			lastOperation = idle;
			itoa(number, numberText, 10);
			SetWindowText(editWindow, numberText);
			editWindowReset = true;
			firstPress = true;
		}
		break;

		case ID_NEGATION:
		{
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			number = -atoi(numberText);
			itoa(number, numberText, 10);
			SetWindowText(editWindow, numberText);
		}
		break;

		case ID_UNDO:
		{
			char* numberText = new char;
			GetWindowText(editWindow, numberText, 15);
			std::string test = numberText;
			test.erase(test.length() - 1);
			if (canUndo) {
				if (test.length() == 0) {
					SetWindowText(editWindow, "0");
					editWindowReset = true;
				}
				else {
					SetWindowText(editWindow, test.c_str());
				}
			}
		}
		break;

		case ID_CE:
		{
			SetWindowText(editWindow, "0");
			editWindowReset = true;
			firstPress = true;
			lastOperation = idle;
			number = 0;
		}
		break;

		}
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX windowClass = {};
	HWND MainWindow;
	MSG Msg;

	//Step 1: Registering the Window Class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "MainWindoWClass";
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&windowClass))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	MainWindow = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"MainWindoWClass",
		"Calculator",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 450,
		NULL, NULL, hInstance, NULL);

	if (MainWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(MainWindow, nCmdShow);
	UpdateWindow(MainWindow);


	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;


}


