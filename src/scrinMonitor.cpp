// g++ scrinMonitor.cpp -lgdi32 -lgdiplus -o scrinMonitor.exe

#include <chrono>
#include <thread>
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace Gdiplus;
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

Bitmap* ConvertHBITMAPToBitmap(HBITMAP hBitmap, HDC hdc) {
  Bitmap* bitmap = Bitmap::FromHBITMAP(hBitmap, NULL);
  if (bitmap->GetLastStatus() != Ok) {
    delete bitmap;
    return nullptr;
  }
  return bitmap;
}

bool SaveHBITMAPToJPEG(HBITMAP hBitmap, const WCHAR* filename) {
  // Получение HDC
  HDC hdc = GetDC(NULL);
  if (!hdc) {
    std::cout << "Failed to get HDC" << std::endl;
    return false;
  }

  // Преобразование HBITMAP в GDI+ Bitmap
  Bitmap* bitmap = ConvertHBITMAPToBitmap(hBitmap, hdc);
  ReleaseDC(NULL, hdc);
  if (!bitmap) {
    std::cout << "Failed to convert HBITMAP to Bitmap" << std::endl;
    return false;
  }

  // Получение CLSID для JPEG
  CLSID clsid;
  if (GetEncoderClsid(L"image/jpeg", &clsid) == -1) {
    std::cout << "Could not find CLSID for JPEG" << std::endl;
    delete bitmap;
    return false;
  }

  // Сохранение Bitmap в файл JPEG
  Status status = bitmap->Save(filename, &clsid, NULL);
  delete bitmap;

  if (status != Ok) {
    std::cout << "Failed to save image in JPEG format" << std::endl;
    return false;
  }

  return true;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
  UINT  num = 0;          // Количество доступных энкодеров
  UINT  size = 0;         // Размер необходимой памяти

  ImageCodecInfo* pImageCodecInfo = NULL;

  // Получение количества энкодеров
  GetImageEncodersSize(&num, &size);
  if (size == 0)
    return -1;

  pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
  if (pImageCodecInfo == NULL)
    return -1;

  // Получение информации об энкодерах
  GetImageEncoders(num, size, pImageCodecInfo);

  for (UINT j = 0; j < num; ++j)
  {
    if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
    {
      *pClsid = pImageCodecInfo[j].Clsid;
      free(pImageCodecInfo);
      return j;  // Успех
    }
  }

  free(pImageCodecInfo);
  return -1;
}

bool CaptureScreenToJPEG(int x, int y, int width, int height, const WCHAR* filename) {
  // Получение дескриптора устройства экрана
  HDC hScreenDC = GetDC(NULL);
  if (!hScreenDC) {
    std::cout << "Failed to get screen device handle" << std::endl;
    return false;
  }

  // Создание совместимого контекста устройства
  HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
  if (!hMemoryDC) {
    std::cout << "Failed to create a compatible device context" << std::endl;
    ReleaseDC(NULL, hScreenDC);
    return false;
  }

  // Создание совместимого битмапа
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
  if (!hBitmap) {
    std::cout << "Failed to create compatible bitmap" << std::endl;
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    return false;
  }

  // Выбор битмапа в контекст устройства
  HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));

  // Копирование пикселей экрана в битмап
  if (!BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, x, y, SRCCOPY)) {
    std::cerr << "Failed to execute BitBlt" << std::endl;
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    return false;
  }

  // Восстановление оригинального битмапа
  SelectObject(hMemoryDC, hOldBitmap);

  // Освобождение дескрипторов
  DeleteDC(hMemoryDC);
  ReleaseDC(NULL, hScreenDC);

  bool result = SaveHBITMAPToJPEG(hBitmap, filename);

  DeleteObject(hBitmap);

  return result;
}

int scrin(int x, int y, int width, int height, const WCHAR* filename) {
  // Инициализация GDI+
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  Status status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  if (status != Ok) {
    std::cout << "Failed to initialize GDI+" << std::endl;
    return -1;
  }

  if (!CaptureScreenToJPEG(x, y, width, height, filename)) {
    std::cout << "Failed to save screenshot" << std::endl;
  }

  // Завершение работы GDI+
  GdiplusShutdown(gdiplusToken);

  return 0;
}
