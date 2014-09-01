#include "Screen.h"

LRESULT CALLBACK Screen::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_CREATE) {
    long This = reinterpret_cast<long>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, This);
  }

  Screen *window = reinterpret_cast<Screen*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  if (window) {
    window->ProcessEvents(uMsg, wParam, lParam);
  }

  if (uMsg == WM_CLOSE)
    return 0;

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Screen::~Screen() {
  KillWindow();
}

Screen::Screen() : bits(32), fullscreen(false), nameOfWindow(L"Undef") {
  hWnd = NULL;
  isActive = true;
  isOpened = true;
}

Screen::Screen(wstring name, int width, int height, bool fullscreen, int bits) {
  hWnd = NULL;
  this->width = width;
  this->height = height;
  this->bits = bits;
  this->fullscreen = fullscreen;
  isActive = true;
  nameOfWindow = name;
  isOpened = true;
  CreateUserWindow();
}

void Screen::CreateUserWindow() {
  hInstance = GetModuleHandle(NULL);
  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hInstance = hInstance;
  wc.hbrBackground = NULL;
  wc.lpfnWndProc = (WNDPROC)&Screen::WndProc;
  wc.lpszClassName = nameOfWindow.c_str();
  wc.lpszMenuName = NULL;
  wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  
  if (!RegisterClass(&wc)) {
    MessageBox(NULL, L"Failed To Register A Window Class.", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  if (fullscreen) {
    DEVMODE dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    dm.dmBitsPerPel = bits;
    dm.dmPelsHeight = height;
    dm.dmPelsWidth = width;
    dm.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      if (MessageBox(NULL, L"Fullscreen doesn't support.\nRun in windowed mode?", nameOfWindow.c_str(), MB_YESNO | MB_ICONEXCLAMATION)==IDYES)
        fullscreen = false;
      else {
        MessageBox(NULL, L"Programm Will Now Close", nameOfWindow.c_str(), MB_OK | MB_ICONSTOP);
        exit(1);
      }
    }
  }

  DWORD dwExStyle = WS_EX_APPWINDOW;
  DWORD dwStyle;

  if (fullscreen) {
    dwStyle = WS_POPUP;
    ShowCursor(FALSE);
  }
  else {
    dwExStyle |= WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  RECT windowRect;
  windowRect.left = (long)0;
  windowRect.right = (long)width;
  windowRect.top = (long)0;
  windowRect.bottom = (long)height;

  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  if (!(hWnd = CreateWindowEx(dwExStyle,
      wc.lpszClassName, wc.lpszClassName,
      dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      0, 0,
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      NULL, NULL, hInstance, this))) {
        KillWindow();
        MessageBox(NULL, L"Failed To Create a Window.", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
        exit(1);
  }

  static PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    (BYTE)bits,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    16,
    0,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  if (!(hDC = ::GetDC(hWnd))) {
    KillWindow();
    MessageBox(NULL, L"Can't Create A GL Device Context", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  GLuint PixelFormat;
  if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
    KillWindow();
    MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
    KillWindow();
    MessageBox(NULL, L"Can't Set The PixelFormat", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  if (!(hRC = wglCreateContext(hDC))) {
    KillWindow();
    MessageBox(NULL, L"Can't Create A GL Rendering Context", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  if (!wglMakeCurrent(hDC, hRC)) {
    KillWindow();
    MessageBox(NULL, L"Can't Activate The GL Rendering Context", nameOfWindow.c_str(), MB_OK | MB_ICONEXCLAMATION);
    exit(1);
  }

  ShowWindow(hWnd, SW_SHOW);
  SetForegroundWindow(hWnd);
  SetFocus(hWnd);
  ResizeWindow(width, height);
  InitGL();
}

void Screen::ProcessEvents() {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void Screen::ProcessEvents(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (hWnd == NULL)
    return ;

  switch (uMsg) {
    case WM_MOUSEMOVE:
      input.SetMouseX(GET_X_LPARAM(lParam));
      input.SetMouseY(GET_Y_LPARAM(lParam));
      break;
    case WM_KEYDOWN:
      input.PressKey(wParam);
      break;
    case WM_KEYUP:
      input.UnPressKey(wParam);
      break;
    case WM_LBUTTONDOWN:
      input.PressLeftMouse();
      break;
    case WM_LBUTTONUP:
      input.UnPressLeftMouse();
      break;
    case WM_RBUTTONDOWN:
      input.PressRightMouse();
      break;
    case WM_RBUTTONUP:
      input.UnPressRightMouse();
      break;
    case WM_ACTIVATE:
      if (!HIWORD(wParam))
        isActive = true;
      else
        isActive = false;
      break;
    case WM_CLOSE:
      isOpened = false;
      break;
    case WM_SIZE:
      ResizeWindow(LOWORD(lParam), HIWORD(lParam));
      break;
    case WM_SYSCOMMAND:
      switch (wParam) {
      case SC_SCREENSAVE:
      case SC_MONITORPOWER:
        break;
      }
      break;
    default:
      break;
  }
}

void Screen::ResizeWindow(int width, int height) {
  glViewport(0, 0, width, height);
}

void Screen::KillWindow() {
  if (fullscreen) {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
  }
  if (hRC) {
    if (!wglMakeCurrent(NULL, NULL))
      MessageBox(NULL, L"Release Of DC And RC Failed.", nameOfWindow.c_str(), MB_OK | MB_ICONINFORMATION);
    if (!wglDeleteContext(hRC))
      MessageBox(NULL, L"Release Rendering Context Failed.", nameOfWindow.c_str(), MB_OK | MB_ICONINFORMATION);
    hRC = NULL;
  }
  if (hDC && !ReleaseDC(hWnd, hDC)) {
    MessageBox(NULL, L"Release Of Device Context Failed.", nameOfWindow.c_str(), MB_OK | MB_ICONINFORMATION);
    hDC = NULL;
  }
  if (hWnd && !DestroyWindow(hWnd)) {
    MessageBox(NULL, L"Could Not Release Window.", nameOfWindow.c_str(), MB_OK | MB_ICONINFORMATION);
    hWnd = NULL;
  }
  if (!UnregisterClass(nameOfWindow.c_str(), hInstance)) {
    MessageBox(NULL, L"Could Not Unregister Class.", nameOfWindow.c_str(), MB_OK | MB_ICONINFORMATION);
    hInstance = NULL;
  }
}

bool Screen::IsOpened() const {
  return isOpened;
}

bool Screen::IsActive() const {
  return isActive;
}

void Screen::InitGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glShadeModel(GL_SMOOTH);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glViewport(0, 0, width, height);
  glOrtho(0, width, 0, height, 0, 1);
}

void Screen::SwitchBuffers() {
  SwapBuffers(hDC);
}

void Screen::Clear(se::Color color) {
  glClearColor(color.r, color.g, color.b, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT);
}

Input& Screen::GetInput() {
  return input;
}

int Screen::GetWidth() const {
	return width;
}

int Screen::GetHeight() const {
	return height;
}

void Screen::SetWidth(int width) {
  this->width = width;
}

void Screen::SetHeight(int height) {
  this->height = height;
}

void Screen::SetFullscreen(bool fullscreen) {
  this->fullscreen = fullscreen;
}

wstring Screen::GetName() const {
	return nameOfWindow;
}

HWND Screen::GetHandle() const {
  return hWnd;
}

HDC Screen::GetDC() const {
  return hDC;
}