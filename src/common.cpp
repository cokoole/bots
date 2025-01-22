#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <cmath>

#include "common.h"

void press_key(WORD key) {
  keybd_event(key, 0, 0, 0);
  keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void hold_key(WORD key) {
  keybd_event(key, 0, 0, 0);
}

void release_key(WORD key) {
  keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void remove_quantity(int amount) {
  for (int i = 0; i < amount; i++) {
    press_key(VK_BACK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void click_at_position(int x, int y) {
  POINT originalPos;

  if (!GetCursorPos(&originalPos)) {
    std::cout << "Error code: " << GetLastError() << std::endl;
    return;
  }

  if (!SetCursorPos(x, y)) {
    std::cout << "Error code: " << GetLastError() << std::endl;
    return;
  }

  INPUT input[2] = {};

  input[0].type = INPUT_MOUSE;
  input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  input[1].type = INPUT_MOUSE;
  input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  UINT uSent = SendInput(2, input, sizeof(INPUT));
  if (uSent != 2)
    std::cout << "Error code: " << GetLastError() << std::endl;

  if (!SetCursorPos(originalPos.x, originalPos.y))
    std::cout << "Error code: " << GetLastError() << std::endl;
}

void click_at_position_lock_guard(int x, int y, std::mutex &mutex) {
  std::lock_guard<std::mutex> lock(mutex);
  click_at_position(x, y);
}

void drag_mouse_position(int start_x, int start_y, int end_x, int end_y, std::mutex &mutex) {
  std::lock_guard<std::mutex> lock(mutex);
  POINT originalPos;

  if (!GetCursorPos(&originalPos)) {
    std::cout << "Error code: " << GetLastError() << std::endl;
    return;
  }

  if (!SetCursorPos(start_x, start_y)) {
    std::cout << "Error code: " << GetLastError() << std::endl;
    return;
  }

  INPUT input[3] = {};

  input[0].type = INPUT_MOUSE;
  input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  SendInput(1, &input[0], sizeof(INPUT));

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  input[1].type = INPUT_MOUSE;
  input[1].mi.dx = end_x;
  input[1].mi.dy = end_y;
  input[1].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &input[1], sizeof(INPUT));

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  input[2].type = INPUT_MOUSE;
  input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  SendInput(1, &input[2], sizeof(INPUT));

  if (!SetCursorPos(originalPos.x, originalPos.y))
    std::cout << "Error code: " << GetLastError() << std::endl;
}

void line_input(const std::string &str) {
  for (char c : str) {
    if (c >= 'a' && c <= 'z') {
      press_key(VkKeyScan(c) & 0xFF);
    } else if (c >= 'A' && c <= 'Z') {
      hold_key(VK_SHIFT);
      press_key(VkKeyScan(c) & 0xFF);
      release_key(VK_SHIFT);
    } else if (c == ' ') {
      press_key(VK_SPACE);
    } else {
      switch (c) {
        case '!': hold_key(VK_SHIFT); press_key('1'); release_key(VK_SHIFT); break;
        case '@': hold_key(VK_SHIFT); press_key('2'); release_key(VK_SHIFT); break;
        case '#': hold_key(VK_SHIFT); press_key('3'); release_key(VK_SHIFT); break;
        case '$': hold_key(VK_SHIFT); press_key('4'); release_key(VK_SHIFT); break;
        case '%': hold_key(VK_SHIFT); press_key('5'); release_key(VK_SHIFT); break;
        case '^': hold_key(VK_SHIFT); press_key('6'); release_key(VK_SHIFT); break;
        case '&': hold_key(VK_SHIFT); press_key('7'); release_key(VK_SHIFT); break;
        case '*': hold_key(VK_SHIFT); press_key('8'); release_key(VK_SHIFT); break;
        case '(': hold_key(VK_SHIFT); press_key('9'); release_key(VK_SHIFT); break;
        case ')': hold_key(VK_SHIFT); press_key('0'); release_key(VK_SHIFT); break;
        case '_': hold_key(VK_SHIFT); press_key(0xBD); release_key(VK_SHIFT); break;
        case '1': press_key('1'); break;
        case '2': press_key('2'); break;
        case '3': press_key('3'); break;
        case '4': press_key('4'); break;
        case '5': press_key('5'); break;
        case '6': press_key('6'); break;
        case '7': press_key('7'); break;
        case '8': press_key('8'); break;
        case '9': press_key('9'); break;
        case '0': press_key('0'); break;
        case '.': press_key(VK_DECIMAL); break;
        default: break;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

void press_left_mouse_button() {
  INPUT input = {0};
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &input, sizeof(INPUT));
}

void release_left_mouse_button() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void move_mouse_smoothly(int start_x, int start_y, int end_x, int end_y, std::mutex &mutex, int duration_ms, int steps) {
  std::lock_guard<std::mutex> lock(mutex);
  POINT originalPos;

  if (!GetCursorPos(&originalPos)) {
    std::cout << "Error code: " << GetLastError() << std::endl;
    return;
  }

  SetCursorPos(start_x, start_y);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  press_left_mouse_button();

  double delta_x = static_cast<double>(end_x - start_x) / steps;
  double delta_y = static_cast<double>(end_y - start_y) / steps;

  int delay_per_step = duration_ms / steps;

  for (int i = 1; i <= steps; ++i) {
    int current_x = static_cast<int>(start_x + delta_x * i);
    int current_y = static_cast<int>(start_y + delta_y * i);
    SetCursorPos(current_x, current_y);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_step));
  }

  release_left_mouse_button();

  if (!SetCursorPos(originalPos.x, originalPos.y))
    std::cout << "Error code: " << GetLastError() << std::endl;
}
