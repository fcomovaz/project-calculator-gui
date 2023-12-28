// Created by: Francisco Javier Moreno Vazquez
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// ===============================
// === Add custom defines here ===
// ===============================
#define WINDOWS_TITLE _T("Calculator")
#define WINDOWS_WIDTH 455
#define WINDOWS_HEIGHT 650

/**
 * @brief Example of a simple GUI application
 *        Compile as: g++ -std=c++20 -o main file.cpp -mwindows -lcomdlg32 -lgdi32 ; ./main
 *
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 * @return int
 */

// ==============================
// == GUI elements declaration ==
// ==============================
HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6, hButton7, hButton8, hButton9, hButton0;
HWND hButtonPlus, hButtonMinus, hButtonMultiply, hButtonDivide, hButtonPower, hButtonEqual;
HWND hButtonClear, hButtonDelete, hButtonParenthesisOpen, hButtonParenthesisClose;
HWND hLabelDisplay, hLabelWarning;
std::wstring displayText = L"";
std::string result = "";

// ==============================
// ====== Custom Functions ======
// ==============================
int precedence(std::string c)
{
    if (c == "+" || c == "-")
    {
        return 1;
    }
    else if (c == "*" || c == "/")
    {
        return 2;
    }
    else if (c == "^")
    {
        return 3;
    }
    return 0;
}

std::vector<std::string> separate_by_strings(std::string expression)
{
    // string to store the separated strings
    std::vector<std::string> separated;
    // string to store the temporary string (substring)
    std::string temp = "";

    // iterate in the expression
    for (auto c : expression)
    {
        // while the character is alphanumeric
        if (isalnum(c))
        {
            // append it to the temporary string
            temp += c;
        }
        else
        {
            // if the temporary string is not empty
            // add the numeric string
            if (temp != "")
            {
                // push it to the vector
                separated.push_back(temp);
                // reset the temporary string
                temp = "";
            }
            // push the operator to the vector
            temp += c;
            // push the operator to the vector
            separated.push_back(temp);
            temp = "";
        }
    }
    if (temp != "")
    {
        separated.push_back(temp);
    }
    return separated;
}

std::vector<std::string> infix_to_postfix(std::vector<std::string> separated)
{

    // vector to store the output
    std::vector<std::string> output;
    // vector to store the stack
    std::vector<std::string> stack;

    // iterate in the expression
    for (auto c : separated)
    {
        // if it is an operand
        if (isalnum(c[0]))
        {
            output.push_back(c);
        }
        // if it is an operator
        else if (c == "+" || c == "-" || c == "*" || c == "/" || c == "^")
        {
            // if the stack is empty
            if (stack.empty())
            {
                stack.push_back(c);
            }
            // if the stack is not empty
            else
            {
                // if the precedence of the operator is greater than the top of the stack
                if (precedence(c) > precedence(stack.back()))
                {
                    stack.push_back(c);
                }
                // if the precedence of the operator is less than the top of the stack
                else
                {
                    while (!stack.empty() && precedence(c) <= precedence(stack.back()))
                    {
                        output.push_back(stack.back());
                        stack.pop_back();
                    }
                    stack.push_back(c);
                }
            }
        }
        // if it is a left parenthesis
        else if (c == "(")
        {
            stack.push_back(c);
        }
        // if it is a right parenthesis
        else if (c == ")")
        {
            while (stack.back() != "(")
            {
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
    }
    while (!stack.empty())
    {
        output.push_back(stack.back());
        stack.pop_back();
    }
    return output;
}

double evaluate_postfix(std::vector<std::string> postfix)
{
    std::vector<double> stack;

    // iterate in the postfix expression
    for (auto c : postfix)
    {
        // if it is an operand
        if (isalnum(c[0]))
        {
            stack.push_back(std::stod(c));
        }
        // if it is an operator
        else if (c == "+" || c == "-" || c == "*" || c == "/" || c == "^")
        {
            double operand2 = stack.back();
            stack.pop_back();
            double operand1 = stack.back();
            stack.pop_back();
            if (c == "+")
            {
                stack.push_back(operand1 + operand2);
            }
            else if (c == "-")
            {
                stack.push_back(operand1 - operand2);
            }
            else if (c == "*")
            {
                stack.push_back(operand1 * operand2);
            }
            else if (c == "/")
            {
                stack.push_back(operand1 / operand2);
            }
            else if (c == "^")
            {
                stack.push_back(std::pow(operand1, operand2));
            }
        }
    }
    return stack.back();
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        // Create the window elements here
        // ===============================================
        // =================== DISPLAY ===================
        // ===============================================
        hLabelDisplay = CreateWindowEx(
            0, _T("STATIC"), _T("0"), WS_CHILD | WS_VISIBLE | SS_RIGHT,
            20, 10, 400, 30,
            hWnd, NULL, NULL, NULL);

        // ===============================================
        // =================== BUTTONS ===================
        // ===============================================
        hButton1 = CreateWindowEx(
            0, _T("BUTTON"), _T("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 50, 100, 100,
            hWnd, (HMENU)1, NULL, NULL);
        hButton2 = CreateWindowEx(
            0, _T("BUTTON"), _T("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 50, 100, 100,
            hWnd, (HMENU)2, NULL, NULL);
        hButton3 = CreateWindowEx(
            0, _T("BUTTON"), _T("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 50, 100, 100,
            hWnd, (HMENU)3, NULL, NULL);
        hButtonPlus = CreateWindowEx(
            0, _T("BUTTON"), _T("+"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 50, 100, 100,
            hWnd, (HMENU)10, NULL, NULL);
        hButton4 = CreateWindowEx(
            0, _T("BUTTON"), _T("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 150, 100, 100,
            hWnd, (HMENU)4, NULL, NULL);
        hButton5 = CreateWindowEx(
            0, _T("BUTTON"), _T("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 150, 100, 100,
            hWnd, (HMENU)5, NULL, NULL);
        hButton6 = CreateWindowEx(
            0, _T("BUTTON"), _T("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 150, 100, 100,
            hWnd, (HMENU)6, NULL, NULL);
        hButtonMinus = CreateWindowEx(
            0, _T("BUTTON"), _T("-"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 150, 100, 100,
            hWnd, (HMENU)11, NULL, NULL);
        hButton7 = CreateWindowEx(
            0, _T("BUTTON"), _T("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 250, 100, 100,
            hWnd, (HMENU)7, NULL, NULL);
        hButton8 = CreateWindowEx(
            0, _T("BUTTON"), _T("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 250, 100, 100,
            hWnd, (HMENU)8, NULL, NULL);
        hButton9 = CreateWindowEx(
            0, _T("BUTTON"), _T("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 250, 100, 100,
            hWnd, (HMENU)9, NULL, NULL);
        hButtonMultiply = CreateWindowEx(
            0, _T("BUTTON"), _T("*"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 250, 100, 100,
            hWnd, (HMENU)12, NULL, NULL);
        hButton0 = CreateWindowEx(
            0, _T("BUTTON"), _T("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 350, 100, 100,
            hWnd, (HMENU)0, NULL, NULL);
        hButtonParenthesisOpen = CreateWindowEx(
            0, _T("BUTTON"), _T("("), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 350, 100, 100,
            hWnd, (HMENU)13, NULL, NULL);
        hButtonParenthesisClose = CreateWindowEx(
            0, _T("BUTTON"), _T(")"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 350, 100, 100,
            hWnd, (HMENU)14, NULL, NULL);
        hButtonDivide = CreateWindowEx(
            0, _T("BUTTON"), _T("/"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 350, 100, 100,
            hWnd, (HMENU)15, NULL, NULL);
        hButtonPower = CreateWindowEx(
            0, _T("BUTTON"), _T("^"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 450, 100, 100,
            hWnd, (HMENU)16, NULL, NULL);
        hButtonClear = CreateWindowEx(
            0, _T("BUTTON"), _T("C"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 450, 100, 100,
            hWnd, (HMENU)17, NULL, NULL);
        hButtonDelete = CreateWindowEx(
            0, _T("BUTTON"), _T("AC"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 450, 100, 100,
            hWnd, (HMENU)18, NULL, NULL);
        hButtonEqual = CreateWindowEx(
            0, _T("BUTTON"), _T("="), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 450, 100, 100,
            hWnd, (HMENU)19, NULL, NULL);

        // ===============================================
        // =================== WARNING ===================
        // ===============================================
        hLabelWarning = CreateWindowEx(
            0, _T("STATIC"), _T("If you want to use negative numbers need to do -1 = (0-1)"), WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 570, 400, 20,
            hWnd, NULL, NULL, NULL);

        break;

    case WM_COMMAND:
    {
        // This cases controls the buttons Actions
        // append the number to the display
        int id = LOWORD(wParam);
        if (id >= 0 && id <= 9)
        {
            // Acción para los números del 0 al 9
            std::wstring number = std::to_wstring(id); // Convertir el número a wstring
            displayText += number;
        }

        if (id == 10)
        {
            displayText += L"+"; // Acción para el botón +
        }
        if (id == 11)
        {
            displayText += L"-"; // Acción para el botón -
        }
        if (id == 12)
        {
            displayText += L"*"; // Acción para el botón *
        }
        if (id == 13)
        {
            displayText += L"("; // Acción para el botón (
        }
        if (id == 14)
        {
            displayText += L")"; // Acción para el botón )
        }
        if (id == 15)
        {
            displayText += L"/"; // Acción para el botón /
        }
        if (id == 16)
        {
            displayText += L"^"; // Acción para el botón ^
        }
        if (id == 17)
        {
            if (displayText.length() > 0)
            {
                displayText.pop_back(); // Acción para el botón C
            }
        }
        if (id == 18)
        {
            displayText = L""; // Acción para el botón AC
        }
        if (id == 19)
        {
            // displayText = L"="; // Acción para el botón =
            // get the display and convert it to string
            std::string expression = std::string(displayText.begin(), displayText.end());
            // separate the string by strings
            std::vector<std::string> separated = separate_by_strings(expression);
            // convert the infix expression to postfix
            std::vector<std::string> postfix = infix_to_postfix(separated);
            // evaluate the postfix expression
            double resultFloat = evaluate_postfix(postfix);
            // convert the result to string
            result = std::to_string(resultFloat);
            // convert the result to wstring
            displayText = std::wstring(result.begin(), result.end());

        }

        SetWindowTextW(hLabelDisplay, displayText.c_str());
        break;
    }

    case WM_DESTROY:
        // Destroy the window elements here
        PostQuitMessage(0);
        break;

    default:
        // Default actions
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX wc; // Structure that contains the Window class attributes
    HWND hWnd;     // Window handler
    MSG msg;       // Message structure

    // Step 1: Registering the Window Class
    ZeroMemory(&wc, sizeof(WNDCLASSEX));           // Filling the structure with zeros
    wc.cbSize = sizeof(WNDCLASSEX);                // Size of the structure in bytes
    wc.style = CS_HREDRAW | CS_VREDRAW;            // Class styles
    wc.lpfnWndProc = WindowProcedure;              // Pointer to the window procedure
    wc.hInstance = hInstance;                      // Instance of the application
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); // Background color of the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);      // Cursor that will be displayed over our window
    wc.lpszClassName = _T("WindowClass");          // Name of the window class
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // Icon of the window class
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  // Small icon of the window class

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, _T("Error registering class"), _T("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Step 2: Creating the Window
    hWnd = CreateWindowEx(
        0,                                // Extended style of the window
        _T("WindowClass"),                // Name of the window class
        WINDOWS_TITLE,                    // Title of the window
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Style of the window
        CW_USEDEFAULT,                    // X-Position of the window
        CW_USEDEFAULT,                    // Y-Position of the window
        WINDOWS_WIDTH,                    // Width of the window
        WINDOWS_HEIGHT,                   // Height of the window
        NULL,                             // Handle to the parent window
        NULL,                             // Handle to the menu
        hInstance,                        // Application Instance
        NULL);                            // Pointer to the window creation data

    if (!hWnd)
    {
        MessageBox(NULL, _T("Error creating window"), _T("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Step 3: The Message Loop
    ShowWindow(hWnd, nCmdShow); // Shows the window
    UpdateWindow(hWnd);         // Sends WM_PAINT message

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatches a message to a window procedure
    }

    return (int)msg.wParam;
}
