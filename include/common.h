#pragma once

void press_key(WORD key);
void hold_key(WORD key);
void release_key(WORD key);
void remove_quantity(int amount);
void click_at_position(int x, int y);
void click_at_position_lock_guard(int x, int y, std::mutex & mutex);
void drag_mouse_position(int start_x, int start_y, int end_x, int end_y, std::mutex & mutex);
void line_input(const std::string & str);
void move_mouse_smoothly(int start_x, int start_y, int end_x, int end_y, std::mutex &mutex, int duration_ms = 2000, int steps = 100);
