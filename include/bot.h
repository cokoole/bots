#pragma once
#include <unordered_map>
#include <string>
#include <windows.h>
#include <iostream>
#include <chrono>

#include "type.h"

class Bot {
public:
  Bot(int number_bot, int number_thread);
  ~Bot();

  bool IsActive;

  void exit();

  bool start_mantis();
  int get_paw();

  bool buy_res(Type_res type_res, unsigned int amount_res);
  bool __open_res(Type_res type_res, unsigned int amount_res);
  bool deliver_resource(Coordinates coord, Type_res type_res, int &amount_res);
  std::chrono::seconds get_total_seconds_delivery_time();

  void cure_soldier();
private:

  HWND _hwnd;
  RECT _rect;

  RECT _multi_window_rect;

  int _number_bot;
  int _number_thread;
  std::string _name_bot;
  std::chrono::seconds _total_seconds_delivery_time;

  int _paw;

  bool __find_paw_mantis();
  bool __take_paw_mantis();

  void __close_submenu();

  void __goto_resource_submission();
  void __send_click();
  std::string __process_string_with_time(const std::string & input);
  bool __check_dispatch_time();

  bool __bot_event_multiwindow_emulator();
  bool __set_pos_window();
  bool __start_game();

  bool __open_food(unsigned int number_resource_boxes);
  bool __open_leaves(unsigned int number_resource_boxes);
  bool __open_water(unsigned int number_resource_boxes);
  bool __open_mushrooms(unsigned int number_resource_boxes);
  bool __presence_res(int pos_y);
  bool __click_open_box_res(int number_pos_y);
  int __get_number_box(int number_pos_y);
  void __open_box_from_opening_menu(int number_resource_boxes);
};