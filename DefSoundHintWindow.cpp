// ----------------------------------------------------------------------------
// DefSoundHintWindow.cpp
// DefSound Hint Window (nobody-kolomna source code)
// ----------------------------------------------------------------------------

#include "stdafx.h"

// ----------------------------------------------------------------------------

#include "DefSoundHintWindow.h"

// ----------------------------------------------------------------------------

namespace
{

// ----------------------------------------------------------------------------

struct WindowProperty
{
    static const BYTE Alpha             = 85;           // непрозрачность окна подсказки (в процентах)

    static REAL PositionX() { return 50.0; }            // горизонтальное положение окна подсказки (в процентах от ширины экрана)
    static REAL PositionY() { return 95.0; }            // вертикальное положение окна подсказки (в процентах от высоты экрана)

    static const UINT Timeout           = 1500;         // время видимости окна (милисекунды)

    static const INT Radius             = 10;           // радиус скругления углов окна (пиксели)
    static REAL FontSize()  { return 35.0; }            // размер шрифта в пикселях, оптимальный размер 35 для иконок 48x48

    static const ARGB BackgroundColor   = 0xFF000000;   // цвет окна
    static const ARGB ForegroundColor   = 0xFFCCCCCC;   // цвет текста
};

// ----------------------------------------------------------------------------

struct Message
{
    enum : UINT
    {
        WM_USER_QUIT    = WM_USER + 0,
    };
};

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CHintWindow::CHintWindow(
    const std::wstring &Icon,
    const std::wstring &Text,
    HINSTANCE hInstance,
    std::unique_ptr< CHintWindowClass > &pHintWindowClass
)   : m_Icon(Icon)
    , m_Text(Text)
{
    if (!pHintWindowClass)
        pHintWindowClass.reset( new CHintWindowClass(hInstance, WindowProc, sizeof(CHintWindow *)) );

    ::EnumWindows(WndEnumForceQuitProc, 0);
}

// ----------------------------------------------------------------------------

void CHintWindow::Create()
{
    _ASSERT(!m_hWindow);
    m_hWindow = 
        ::CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
            CHintWindowClass::GetName(),
            m_Text.c_str(),
            WS_DISABLED| WS_POPUP | WS_VISIBLE,
            0,
            0,
            0,
            0,
            nullptr,
            nullptr,
            nullptr,
            this);
    if (!m_hWindow)
        throw CError( MakeDefaultErrorDescription(L"::CreateWindowEx") );

    SetWindowLongPtr(m_hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // timer to window close
    if (!::SetTimer(m_hWindow, Message::WM_USER_QUIT, WindowProperty::Timeout, nullptr))
        throw CError( MakeDefaultErrorDescription(L"::SetTimer") );

    Draw();
}

// ----------------------------------------------------------------------------

void CHintWindow::Show()
{
    // window message loop
    _ASSERT(m_hWindow);

    ::MSG Msg;
    while (::GetMessage(&Msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&Msg);
        ::DispatchMessage(&Msg);
    }

    _ASSERT(!m_hWindow);
}

// ----------------------------------------------------------------------------

BOOL CALLBACK CHintWindow::WndEnumForceQuitProc(HWND hWindow, LPARAM)
{
    const auto ClassNameLen = CHintWindowClass::GetNameLength();

    std::vector<WCHAR> wszClassName(ClassNameLen + 1, L'\0');
    const UINT RetValue = ::GetClassName(hWindow, &wszClassName[0], ClassNameLen + 1);
    if ((RetValue == ClassNameLen) &&
        (0 == wcscmp(&wszClassName[0], CHintWindowClass::GetName())))
    {
        ::SendMessage(hWindow, Message::WM_USER_QUIT, 0, 0);
    }
    return TRUE;
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK CHintWindow::WindowProc(
    HWND hWindow,
    UINT nMessage,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (nMessage)
    {
    case WM_DESTROY:
        ::PostQuitMessage(ERROR_SUCCESS);
        break;

    case WM_TIMER:
        {
            switch (wParam)
            {
            case Message::WM_USER_QUIT:
                return OnMessageQuit(hWindow);
            }
        }
        break;

    case Message::WM_USER_QUIT:
        return OnMessageQuit(hWindow);
    }

    _ASSERT(nMessage != WM_CLOSE);
    return ::DefWindowProc(hWindow, nMessage, wParam, lParam);
}

// ----------------------------------------------------------------------------

LRESULT CHintWindow::OnMessageQuit(HWND hWindow)
{
    CHintWindow *pThis = reinterpret_cast<CHintWindow *>( ::GetWindowLongPtr(hWindow, GWLP_USERDATA) );
    if (!pThis)
        throw CError( MakeDefaultErrorDescription(L"::GetWindowLongPtr") );

    if (!pThis->m_hWindow)
        return FALSE;

    _ASSERT(hWindow == pThis->m_hWindow);

    ::KillTimer(pThis->m_hWindow, Message::WM_USER_QUIT);

    VERIFY(::DestroyWindow(pThis->m_hWindow));
    pThis->m_hWindow = nullptr;
    return TRUE;
}

// ----------------------------------------------------------------------------

void CHintWindow::Draw()
{
    // перерисовка окна
    SIZE WindowSize;

    HDC hDC = ::CreateCompatibleDC(nullptr);
    if (!hDC)
        throw CError( MakeDefaultErrorDescription(L"::CreateCompatibleDC") );
    std::unique_ptr< HDC__, decltype(&::DeleteDC) > hDCDeleter(hDC, &::DeleteDC);

    std::unique_ptr<Graphics> pGraphics;
    pGraphics.reset( Graphics::FromHDC(hDC) );

    PointF PointText( 0, 0 );
    PointText.X = WindowProperty::Radius;
    PointText.Y = WindowProperty::Radius;

    // создание шрифта для отрисовки текста
    const FontFamily Family(L"Tahoma", nullptr);
    Font FontText(&Family, WindowProperty::FontSize(), FontStyleRegular, UnitPixel);

    RectF TextSize;
    pGraphics->MeasureString(m_Text.c_str(), static_cast<INT>(m_Text.size()), &FontText, PointText, &TextSize );
    WindowSize.cx = LONG(TextSize.Width) + WindowProperty::Radius * 2;
    WindowSize.cy = LONG(TextSize.Height) + WindowProperty::Radius * 2;

    // создание битмапа окна
    Bitmap BitmapObj(WindowSize.cx, WindowSize.cy, PixelFormat32bppARGB);
    Color ColorObj(0, 0, 0, 0);

    HBITMAP hBitmap;
    BitmapObj.GetHBITMAP(ColorObj, &hBitmap);
    std::unique_ptr< HBITMAP__, decltype(&::DeleteObject) > hBitmapDeleter(hBitmap, &::DeleteObject);

    SelectObject(hDC, hBitmap);

    // создание графического объекта на основе битмапа окна
    pGraphics.reset( Graphics::FromHDC(hDC) );

    // установка параметров графического объекта
    pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
    pGraphics->SetCompositingMode(CompositingModeSourceOver);
    pGraphics->SetCompositingQuality(CompositingQualityHighQuality);
    pGraphics->SetPixelOffsetMode(PixelOffsetModeHighQuality);
    pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);

    // отрисовка фона окна
    Color ColorBackground(WindowProperty::BackgroundColor);
    SolidBrush BrushBackground(ColorBackground);
    {
        // режим пути
        GraphicsPath RectanglePath;
        RectanglePath.AddLine( WindowProperty::Radius, 0, WindowSize.cx - WindowProperty::Radius, 0 );
        RectanglePath.AddArc ( WindowSize.cx - 2 * WindowProperty::Radius, 0 , 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, -90, 90 );
        RectanglePath.AddLine( WindowSize.cx, WindowProperty::Radius, WindowSize.cx, WindowSize.cy - WindowProperty::Radius );
        RectanglePath.AddArc ( WindowSize.cx - 2 * WindowProperty::Radius, WindowSize.cy - 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, 0, 90 );
        RectanglePath.AddLine( WindowSize.cx - WindowProperty::Radius, WindowSize.cy, WindowProperty::Radius, WindowSize.cy );
        RectanglePath.AddArc ( 0, WindowSize.cy - 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, 90, 90 );
        RectanglePath.AddLine( 0, WindowSize.cy - WindowProperty::Radius, 0, WindowProperty::Radius );
        RectanglePath.AddArc ( 0, 0, 2 * WindowProperty::Radius, 2 * WindowProperty::Radius, 180, 90 );
        RectanglePath.CloseFigure();
        pGraphics->FillPath(&BrushBackground, &RectanglePath);
    }

    // отрисовка текста
    Color ColorForeground(WindowProperty::ForegroundColor);
    SolidBrush BrushForeground(ColorForeground);
    pGraphics->DrawString(m_Text.c_str(), static_cast<INT>(m_Text.size()), &FontText, PointText, &BrushForeground);

    // расчет позиции окна исходя из процентных отношений к экрану
    POINT WindowPoint;
    WindowPoint.x = LONG((GetSystemMetrics(SM_CXFULLSCREEN ) - WindowSize.cx) * (WindowProperty::PositionX() / 100.0));
    WindowPoint.y = LONG((GetSystemMetrics(SM_CYFULLSCREEN ) - WindowSize.cy) * (WindowProperty::PositionY() / 100.0));

    // обновление окна
    ::BLENDFUNCTION BlendFunction;
    BlendFunction.BlendOp = AC_SRC_OVER;
    BlendFunction.BlendFlags = 0;
    BlendFunction.SourceConstantAlpha = (255 * WindowProperty::Alpha) / 100;
    BlendFunction.AlphaFormat = AC_SRC_ALPHA;

    POINT PointZero = {0};
    ::UpdateLayeredWindow(
        m_hWindow,
        nullptr,
        &WindowPoint,
        &WindowSize,
        hDC,
        &PointZero,
        RGB(255,255,255),
        &BlendFunction,
        ULW_ALPHA);
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
