#include <Windows.h>

// ============================================================
// Клас для малювання сонця
// ============================================================
class Sun
{
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(255, 220, 0));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        Ellipse(dc, X, Y, X + 90, Y + 90);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

// ============================================================
// Клас для малювання озера
// ============================================================
class Lake
{
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(40, 130, 240));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        Ellipse(dc, X, Y, X + 220, Y + 100);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

// ============================================================
// Клас для малювання дерева
// ============================================================
class TreePicture
{
private:
    void line(HDC dc, int x1, int y1, int x2, int y2)
    {
        MoveToEx(dc, x1, y1, nullptr);
        LineTo(dc, x2, y2);
    }
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(0, 200, 0));
        HGDIOBJ oldBrush = SelectObject(dc, brush);

        // Стовбур
        line(dc, X, Y, X, Y - 120);

        // Крона (гілки та еліпси)
        line(dc, X, Y - 60, X - 40, Y - 80);
        Ellipse(dc, X - 60, Y - 95, X - 35, Y - 65);

        line(dc, X, Y - 80, X + 50, Y - 130);
        Ellipse(dc, X + 35, Y - 140, X + 65, Y - 115);

        Ellipse(dc, X - 20, Y - 130, X + 20, Y - 110);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

// ============================================================
// Клас для малювання вежі
// ============================================================
class Tower
{
public:
    void show(HDC dc, int X, int Y)
    {
        // Основна частина вежі (циліндр/прямокутник)
        HBRUSH brush = CreateSolidBrush(RGB(210, 210, 210));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        Rectangle(dc, X, Y + 80, X + 70, Y + 250);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Дах вежі (трикутник)
        POINT poly[3] = { {X - 10, Y + 80}, {X + 80, Y + 80}, {X + 35, Y + 20} };
        brush = CreateSolidBrush(RGB(180, 40, 40));
        oldBrush = SelectObject(dc, brush);
        Polygon(dc, poly, 3);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Двері
        brush = CreateSolidBrush(RGB(120, 60, 20));
        oldBrush = SelectObject(dc, brush);
        Rectangle(dc, X + 22, Y + 190, X + 48, Y + 250);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Вікна вежі
        brush = CreateSolidBrush(RGB(255, 255, 0));
        oldBrush = SelectObject(dc, brush);
        Ellipse(dc, X + 22, Y + 100, X + 48, Y + 130);
        Ellipse(dc, X + 22, Y + 145, X + 48, Y + 175);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

// ============================================================
// Малювання всієї сцени
// ============================================================
void DrawScene(HDC dc, int width, int height)
{
    HBRUSH brush;
    HGDIOBJ oldBrush;

    // ========================================================
    // ФОН (Земля)
    // ========================================================
    brush = CreateSolidBrush(RGB(120, 210, 80));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, 250, width, height);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    // ========================================================
    // НЕБО
    // ========================================================
    brush = CreateSolidBrush(RGB(135, 206, 235));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, 0, width, 250);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    // ========================================================
    // СОНЦЕ
    // ========================================================
    Sun sun;
    sun.show(dc, width - 150, 40);

    // ========================================================
    // ОЗЕРО
    // ========================================================
    Lake lake;
    lake.show(dc, 250, 320);

    // ========================================================
    // ВЕЖІ (повторюються 2 рази)
    // ========================================================
    Tower tower;
    tower.show(dc, 80, 150);
    tower.show(dc, 620, 130);

    // ========================================================
    // ДЕРЕВА (повторюються 3 рази)
    // ========================================================
    TreePicture tree;
    tree.show(dc, 180, 380);
    tree.show(dc, 540, 420);
    tree.show(dc, 750, 360);
}

// ============================================================
// Процедура обробки повідомлень
// ============================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        DrawScene(dc, width, height);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        MessageBox(hwnd, L"Клік лівою кнопкою миші на сцені!", L"WM_LBUTTONDOWN", MB_OK);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    }

    case WM_SIZE:
    {
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// ============================================================
// Головна функція Windows-програми
// ============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"LandscapeApplication";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Індивідуальний проєкт: Вежі, Дерева, Озеро, Сонце",
        WS_OVERLAPPEDWINDOW,
        100, 100, 900, 600, nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}