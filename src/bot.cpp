#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <locale>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
//
#define UNICODE
#define _UNICODE
#include "bot.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>

#include "common.h"
#include "workingImages.h"
#include "scrinMonitor.h"

// mutex
std::mutex _controller_mutex;
std::mutex _working_with_multiwindow_mutex;
std::mutex _resources_sent_mutex;

//////////////////////////////////////////// Delivery resources
#define offset_click_go_outside_x 40
#define offset_click_go_outside_y 785

#define offset_click_search_player_coord_x 158
#define offset_click_search_player_coord_y 111

#define offset_click_X_search_player_x 215
#define offset_click_X_search_player_y 370

#define offset_click_Y_search_player_x 315
#define offset_click_Y_search_player_y 370

#define offset_click_search_player_coord_go_x 210
#define offset_click_search_player_coord_go_y 460

#define offset_click_res_send_player_food_x 350
#define offset_click_res_send_player_food_y 215

#define offset_click_res_send_player_leaves_x 350
#define offset_click_res_send_player_leaves_y 300

#define offset_click_res_send_player_water_x 350
#define offset_click_res_send_player_water_y 385

#define offset_click_res_send_player_mushrooms_x 350
#define offset_click_res_send_player_mushrooms_y 470

#define offset_send_res_time_image_x 143
#define offset_send_res_time_image_y 147
#define offset_send_res_time_image_w 58
#define offset_send_res_time_image_h 21

#define tmp_send_res_time_img_L L"scrinShots/tmpSendResTime"
#define tmp_send_res_time_img    "scrinShots/tmpSendResTime"

#define offset_click_player_x 210
#define offset_click_player_y 360

#define offset_send_resources_x 144
#define offset_send_resources_y 454

#define offset_food_filling_start_x 136
#define offset_food_filling_start_y 240
#define offset_food_filling_end_x 352
#define offset_food_filling_end_y 244

#define offset_leaves_filling_start_x 136
#define offset_leaves_filling_start_y 328
#define offset_leaves_filling_end_x 352
#define offset_leaves_filling_end_y 328

#define offset_water_filling_start_x 136
#define offset_water_filling_start_y 413
#define offset_water_filling_end_x 352
#define offset_water_filling_end_y 413

#define offset_mushrooms_filling_start_x 136
#define offset_mushrooms_filling_start_y 498
#define offset_mushrooms_filling_end_x 352
#define offset_mushrooms_filling_end_y 498

#define offset_click_send_x 210
#define offset_click_send_y 735

#define offset_click_yes_send_x 305
#define offset_click_yes_send_y 460

//////////////////////////////////////////// Mantis
#define offset_click_ok_input_line_x 340
#define offset_click_ok_input_line_y 761

#define paw_search_home_page "700"

#define offset_click_mantis_x 30
#define offset_click_mantis_y 370

#define offset_click_mantis_3lvl_x 345
#define offset_click_mantis_3lvl_y 500

#define offset_click_random_pos_x 390
#define offset_click_random_pos_y 105

#define offset_image_check_busy_paw_x 5
#define offset_image_check_busy_paw_y 180
#define offset_image_check_busy_paw_w 70
#define offset_image_check_busy_paw_h 80

#define ref_mantis_check_busy_paw_image_file_L L"scrinShots/refMantisCheckBusyPaw.jpg"
#define tmp_mantis_check_busy_paw_image_file_L L"scrinShots/tmpMantisCheckBusyPaw"

#define ref_mantis_check_busy_paw_image_file "scrinShots/refMantisCheckBusyPaw.jpg"
#define tmp_mantis_check_busy_paw_image_file "scrinShots/tmpMantisCheckBusyPaw"

#define offset_click_mantis_recall_x 35
#define offset_click_mantis_recall_y 255

#define offset_click_mantis_exit_yes_x 320
#define offset_click_mantis_exit_yes_y 505

#define offset_click_mantis_recall_yes_x 300
#define offset_click_mantis_recall_yes_y 470

#define offset_click_mantis_recall_confirm_collection_x 300
#define offset_click_mantis_recall_confirm_collection_y 440

#define offset_click_mantis_page_section_x 207
#define offset_click_mantis_page_section_y 692

#define offset_click_mantis_page_next_x 294
#define offset_click_mantis_page_next_y 692

#define offset_click_mantis_capture_x 215
#define offset_click_mantis_capture_y 540

#define offset_click_mantis_capture_yes_x 305
#define offset_click_mantis_capture_yes_y 490

#define offset_click_mantis_capture_quick_choice_x 215
#define offset_click_mantis_capture_quick_choice_y 770

#define offset_click_mantis_capture_send_x 215
#define offset_click_mantis_capture_send_y 770

#define offset_paw_image_x 100
#define offset_paw_image_y 250
#define offset_paw_image_w 180
#define offset_paw_image_h 18

#define image_paw_file_L L"scrinShots/value"
#define image_paw_file "scrinShots/value"

#define offset_back_bag_x 173
#define offset_back_bag_y 39
#define offset_back_bag_w 80
#define offset_back_bag_h 31

#define ref_back_bag_image_file_L L"scrinShots/refBackBag.jpg"
#define tmp_back_bag_image_file_L L"scrinShots/tmpBackBag.jpg"

#define ref_back_bag_image_file "scrinShots/refBackBag.jpg"
#define tmp_back_bag_image_file "scrinShots/tmpBackBag.jpg"

// Resetting lair
#define offset_click_mantis_resetting_lair_x 125
#define offset_click_mantis_resetting_lair_y 510

/////////////////////////////////////////// Emulator launch window

#define offset_click_find_bot_x 630
#define offset_click_find_bot_y 72

#define offset_click_start_bot_x 570
#define offset_click_start_bot_y 155

#define offset_click_start_game_x 258
#define offset_click_start_game_y 180

/////////////////////////////////////////// Start game, main menu anthill
#define offset_main_menu_x 160
#define offset_main_menu_y 140
#define offset_main_menu_w 75
#define offset_main_menu_h 100

#define ref_main_menu_image_file_L L"scrinShots/refMainMenuGame.jpg"
#define tmp_main_menu_image_file_L L"scrinShots/tmpMainMenuGame"

#define ref_main_menu_image_file "scrinShots/refMainMenuGame.jpg"
#define tmp_main_menu_image_file "scrinShots/tmpMainMenuGame"

#define loop_work_time 5
#define event_time_loop 1

#define offset_submenu_close_x 10
#define offset_submenu_close_y 40
#define offset_submenu_close_w 38
#define offset_submenu_close_h 24

#define ref_submenu_close_file_L L"scrinShots/refAdvertising.jpg"
#define tmp_submenu_close_file_L L"scrinShots/tmpAdvertising"

#define ref_submenu_close_file "scrinShots/refAdvertising.jpg"
#define tmp_submenu_close_file "scrinShots/tmpAdvertising"

#define offset_click_submenu_exit_x 40
#define offset_click_submenu_exit_y 50

/////////////////////////////////////////// Healer
#define offset_drag_main_in_healer_start_x 380
#define offset_drag_main_in_healer_start_y 430

#define offset_drag_main_in_healer_end_x 360
#define offset_drag_main_in_healer_end_y 430

#define offset_click_healer_x 300
#define offset_click_healer_y 380

#define offset_image_healer_x 15
#define offset_image_healer_y 335
#define offset_image_healer_w 390
#define offset_image_healer_h 95

#define ref_free_healer_file_L L"scrinShots/refFreeHealer.jpg"
#define tmp_free_healer_file_L L"scrinShots/tmpFreeHealer.jpg"

#define ref_free_healer_file "scrinShots/refFreeHealer.jpg"
#define tmp_free_healer_file "scrinShots/tmpFreeHealer.jpg"

#define offset_all_clear_image_healer_x 25
#define offset_all_clear_image_healer_y 723
#define offset_all_clear_image_healer_w 75
#define offset_all_clear_image_healer_h 20

#define ref_all_clear_file_L L"scrinShots/refAllClearHealer.jpg"
#define tmp_all_clear_file_L L"scrinShots/tmpAllClearHealer.jpg"

#define ref_all_clear_file "scrinShots/refAllClearHealer.jpg"
#define tmp_all_clear_file "scrinShots/tmpAllClearHealer.jpg"

#define offset_click_all_clear_x 65
#define offset_click_all_clear_y 735

#define offset_click_plus_one_x 375
#define offset_click_plus_one_y 400

#define offset_click_auto_use_x 70
#define offset_click_auto_use_y 660

#define offset_click_yes_replenish_use_x 295
#define offset_click_yes_replenish_use_y 620

#define offset_click_heal_use_x 360
#define offset_click_heal_use_y 740

#define offset_open_res_healer_x 120
#define offset_open_res_healer_y 190
#define offset_open_res_healer_w 180
#define offset_open_res_healer_h 27

#define ref_auto_open_res_file_L L"scrinShots/refAutoOpenRes.jpg"
#define tmp_auto_open_res_file_L L"scrinShots/tmpAutoOpenRes.jpg"

#define ref_auto_open_res_file "scrinShots/refAutoOpenRes.jpg"
#define tmp_auto_open_res_file "scrinShots/tmpAutoOpenRes.jpg"

/////////////////////////////////////////// Exit
#define offset_exit_yes_x 290
#define offset_exit_yes_y 355

/////////////////////////////////////////// Open res
#define offset_click_food_x 30
#define offset_click_food_y 50

#define offset_click_leaves_x 107
#define offset_click_leaves_y 50

#define offset_click_water_x 190
#define offset_click_water_y 50

#define offset_click_mushrooms_x 270
#define offset_click_mushrooms_y 50

#define area_number_res_w 60
#define area_number_res_h 21

#define offset_food_res_x 141
#define offset_leaves_res_x 149
#define offset_water_res_x 136
#define offset_mushrooms_res_x 146

#define food_box      50000
#define leaves_box    50000
#define water_box     10000
#define mushrooms_box 2500

#define offset_presence_res_image_res_w 88
#define offset_presence_res_image_res_h 27

#define offset_presence_res_image_res_x 308
const std::vector array_offset_res_presence_pos_y {411, 506, 601, 696};

#define offset_open_res_x 350
const std::vector array_offset_open_res_pos_y {430, 525, 620, 715};

#define res_presence_res_img_L L"scrinShots/resPresenceRes.jpg"
#define res_presence_res_img    "scrinShots/resPresenceRes.jpg"

#define tmp_presence_res_img_L L"scrinShots/tmpPresenceRes"
#define tmp_presence_res_img    "scrinShots/tmpPresenceRes"

const std::vector array_offset_res_pos_y {370, 465, 560, 653215};

#define tmp_res_img_file_L L"scrinShots/tmpRes"
#define tmp_res_img_file    "scrinShots/tmpRes"

#define offset_number_box_res_image_x 345
#define offset_number_box_res_image_y 441
#define offset_number_box_res_image_w 42
#define offset_number_box_res_image_h 18

#define tmp_number_box_res_img_L L"scrinShots/tmpNumberBoxRes"
#define tmp_number_box_res_img    "scrinShots/tmpNumberBoxRes"

#define offser_click_input_number_box_open_x 305
#define offser_click_input_number_box_open_y 450

#define offser_click_apply_box_open_x 210
#define offser_click_apply_box_open_y 520

/////////////////////////////////////////// buy res
#define offser_click_exchange_res_open_x 365
#define offser_click_exchange_res_open_y 260

#define offset_drag_main_in_exchange_res_start_x 197
#define offset_drag_main_in_exchange_res_start_y 762

#define offset_drag_main_in_exchange_res_end_x 196
#define offset_drag_main_in_exchange_res_end_y 459

#define offser_click_exchange_res_food_x 350
#define offser_click_exchange_res_food_y 380

#define offser_click_exchange_res_leaves_x 350
#define offser_click_exchange_res_leaves_y 480

#define offser_click_exchange_res_water_x 350
#define offser_click_exchange_res_water_y 580

#define offser_click_exchange_res_mushrooms_x 350
#define offser_click_exchange_res_mushrooms_y 680

///////////////////////////////////////////

const std::unordered_map<std::string, Mantis_coord> mantis_map {
  //Pos  off_x     off_y     width    height   off_coord_click
  {"1",  {155,     150,      70,      35,      {186,  174}  }},
  {"2",  {313,     248,      64,      29,      {363,  263}  }},
  {"3",  {100,     223,      67,      28,      {122,  240}  }},
  {"4",  {230,     284,      60,      27,      {280,  301}  }},
  {"5",  {140,     308,      62,      25,      {154,  322}  }},
  {"6",  {353,     318,      57,      25,      {397,  333}  }},
  {"7",  {21,      337,      70,      24,      {36,   351}  }},
  {"8",  {276,     361,      52,      22,      {322,  375}  }},
  {"9",  {307,     422,      52,      24,      {351,  436}  }},
  {"10", {34,      444,      56,      23,      {48,   457}  }},
  {"11", {218,     471,      54,      23,      {261,  484}  }},
  {"12", {83,      516,      56,      24,      {97,   532}  }},
  {"13", {190,     540,      53,      24,      {233,  554}  }}
};

const std::unordered_map<int, Coordinates> window_position {
//number_thread  x        y
  {0,           {-1920,   0}},
  {1,           {-1440,   0}},
  {2,           {-960 ,   0}},
  {3,           {-480 ,   0}},
};

void Bot::__goto_resource_submission() {
  int click_coord_x = _rect.left + offset_click_player_x;
  int click_coord_y = _rect.top + offset_click_player_y;

  click_at_position_lock_guard(click_coord_x, click_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  click_coord_x = _rect.left + offset_send_resources_x;
  click_coord_y = _rect.top + offset_send_resources_y;

  click_at_position_lock_guard(click_coord_x, click_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(3500));
}

void Bot::__send_click() {
  int click_coord_x = _rect.left + offset_click_send_x;
  int click_coord_y = _rect.top + offset_click_send_y;

  click_at_position_lock_guard(click_coord_x, click_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  click_coord_x = _rect.left + offset_click_yes_send_x;
  click_coord_y = _rect.top + offset_click_yes_send_y;

  click_at_position_lock_guard(click_coord_x, click_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

bool Bot::deliver_resource(Coordinates coord, Type_res type_res, int &amount_res) {
  if (amount_res < 1)
    return false;

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  click_at_position_lock_guard(_rect.left + offset_click_go_outside_x, _rect.top + offset_click_go_outside_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  // search
  click_at_position_lock_guard(_rect.left + offset_click_search_player_coord_x, _rect.top + offset_click_search_player_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  // x
  click_at_position_lock_guard(_rect.left + offset_click_X_search_player_x, _rect.top + offset_click_X_search_player_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  _controller_mutex.lock();
  remove_quantity(4);
  line_input(std::to_string(coord.x));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  _controller_mutex.unlock();

  click_at_position_lock_guard(_rect.left + offset_click_ok_input_line_x, _rect.top + offset_click_ok_input_line_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // y
  click_at_position_lock_guard(_rect.left + offset_click_Y_search_player_x, _rect.top + offset_click_Y_search_player_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  _controller_mutex.lock();
  remove_quantity(4);
  line_input(std::to_string(coord.y));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  _controller_mutex.unlock();

  click_at_position_lock_guard(_rect.left + offset_click_ok_input_line_x, _rect.top + offset_click_ok_input_line_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //
  click_at_position_lock_guard(_rect.left + offset_click_search_player_coord_go_x, _rect.top + offset_click_search_player_coord_go_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  // in send
  for (size_t i = 0; i < 3 && amount_res > 0; i++) {
    _resources_sent_mutex.lock();
    int send_res = 800000;
    amount_res -= send_res;

    if (amount_res < 0) {
      send_res += amount_res;
      amount_res = 0;
    }

    send_res = (float)send_res / 0.75;
    _resources_sent_mutex.unlock();

    __goto_resource_submission();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // time deliver
    __check_dispatch_time();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    switch (type_res) {
      case Type_res::food:
        click_at_position_lock_guard(_rect.left + offset_click_res_send_player_food_x, _rect.top + offset_click_res_send_player_food_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        break;

      case Type_res::leaves:
        click_at_position_lock_guard(_rect.left + offset_click_res_send_player_leaves_x, _rect.top + offset_click_res_send_player_leaves_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        break;

      case Type_res::water:
        click_at_position_lock_guard(_rect.left + offset_click_res_send_player_water_x, _rect.top + offset_click_res_send_player_water_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        break;

      case Type_res::mushrooms:
        click_at_position_lock_guard(_rect.left + offset_click_res_send_player_mushrooms_x, _rect.top + offset_click_res_send_player_mushrooms_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        break;

      default:
        return false;
    }

    // send res
    _controller_mutex.lock();
    remove_quantity(7);
    line_input(std::to_string(send_res));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    _controller_mutex.unlock();

    click_at_position_lock_guard(_rect.left + offset_click_ok_input_line_x, _rect.top + offset_click_ok_input_line_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    __send_click();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }
  
  click_at_position_lock_guard(_rect.left + offset_click_go_outside_x, _rect.top + offset_click_go_outside_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  return true;
}

std::chrono::seconds Bot::get_total_seconds_delivery_time() {
  return _total_seconds_delivery_time;
}

bool Bot::__check_dispatch_time() {
  if (_total_seconds_delivery_time.count() != 0)
    return true;

  std::wstring image_number_box_res_wstr = tmp_send_res_time_img_L + std::to_wstring(0) + L".jpg";
  std::string  image_number_box_res_str =  tmp_send_res_time_img +   std::to_string(0) +   ".jpg";

  scrin(_rect.left + offset_send_res_time_image_x, _rect.top + offset_send_res_time_image_y, offset_send_res_time_image_w, offset_send_res_time_image_h, image_number_box_res_wstr.c_str());

  std::string timeStr = get_string_eng_from_image(image_number_box_res_str);

  timeStr = __process_string_with_time(timeStr);

  std::tm tm = {};
  std::istringstream ss(timeStr);
  ss >> std::get_time(&tm, "%H:%M:%S");
  
  if (ss.fail()) {
      std::cout << "Faild format time" << std::endl;
      return false;
  }

  int time =  tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
  if (time < 20)
    time = 60;

  _total_seconds_delivery_time = std::chrono::seconds{time};

  return true;
}

bool Bot::__bot_event_multiwindow_emulator() {
  std::wstring class_name = L"LDRemoteLoginFrame";
  //std::wstring class_name = L"LDMultiPlayerMainFrame";
  LPCWSTR windowName = L"LDMultiPlayer";
  HWND multi_window_hwnd = FindWindowW(class_name.c_str(), NULL);

  if (multi_window_hwnd == NULL) {
      DWORD error = GetLastError();
      std::wcout << L"Window not found for class. Error code: " << error << std::endl;
      multi_window_hwnd = FindWindowW(NULL, windowName);

      if (multi_window_hwnd == NULL) {
        std::wcout << L"Window not found for name. Error code: " << error << std::endl;
        return false;
      }
  }

  SetForegroundWindow(multi_window_hwnd);
  
  GetWindowRect(multi_window_hwnd, &_multi_window_rect);

  int new_width = 720;
  int new_height = 570;
  int new_left = 648;
  int new_top = 291;

  UINT flags = SWP_NOZORDER | SWP_NOACTIVATE; // move
  SetWindowPos(multi_window_hwnd, NULL, new_left,new_top, new_width, new_height, flags);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  _controller_mutex.lock();
  click_at_position(new_left + offset_click_find_bot_x, new_top + offset_click_find_bot_y);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  remove_quantity(10);
  line_input(_name_bot);

  click_at_position(new_left + offset_click_start_bot_x, new_top + offset_click_start_bot_y);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  _controller_mutex.unlock();

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return true;
}

Bot::Bot(int number_bot, int number_thread) : _number_bot{number_bot}, _number_thread{number_thread}  {
  _paw = 0;
  _total_seconds_delivery_time = std::chrono::seconds{0};
  IsActive = false;

  _name_bot = ("bot_" + std::to_string(number_bot) + "_");
  std::cout << "Start bot: " << _name_bot << "\n";

  _working_with_multiwindow_mutex.lock();
  if (!__bot_event_multiwindow_emulator()) {
    _working_with_multiwindow_mutex.unlock();
    return;
  }
  _working_with_multiwindow_mutex.unlock();

  this->_hwnd = FindWindowA(NULL, _name_bot.c_str());
  if (_hwnd == NULL) {
      std::cout << "\nNot found window" << std::endl;
      return;
  }

  GetWindowRect(_hwnd, &this->_rect);
  std::this_thread::sleep_for(std::chrono::milliseconds(25000));
  if (!__set_pos_window()) {
    std::cout << "\nFailed set pos window" << std::endl;
    return;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  if (!__start_game()) {
    std::cout << "\nFailed start game" << std::endl;
    return;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  __close_submenu();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  __close_submenu();

  IsActive = true;
}

Bot::~Bot() {
  //this->_hwnd = FindWindowA(NULL, _name_bot.c_str());
  //if (_hwnd != NULL) {
  //    exit();
  //}
}

bool Bot::start_mantis() {
  __close_submenu();
  return __take_paw_mantis();
}

void Bot::exit() {
  std::lock_guard<std::mutex> lock(_working_with_multiwindow_mutex);
  __bot_event_multiwindow_emulator();

  click_at_position_lock_guard(_multi_window_rect.left + offset_exit_yes_x, _multi_window_rect.top + offset_exit_yes_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int Bot::get_paw() {
  __close_submenu();
  if (_paw == 0) {
    click_at_position_lock_guard(_rect.left + offset_click_mantis_x, _rect.top + offset_click_mantis_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000));

    std::wstring image_paw_file_wstr = tmp_main_menu_image_file_L + std::to_wstring(_number_thread) + L".jpg";
    std::string  image_paw_file_str =  tmp_main_menu_image_file +   std::to_string(_number_thread) +   ".jpg";

    scrin(_rect.left + offset_main_menu_x, _rect.top + offset_main_menu_y, offset_main_menu_w, offset_main_menu_h, image_paw_file_wstr.c_str());

    if (compare_images(ref_main_menu_image_file, image_paw_file_str) < 5) {
      click_at_position_lock_guard(_rect.left + offset_click_mantis_x, _rect.top + offset_click_mantis_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    }

    scrin(_rect.left + offset_paw_image_x, _rect.top + offset_paw_image_y, offset_paw_image_w, offset_paw_image_h, image_paw_file_wstr.c_str());
    _paw = get_number_from_image(image_paw_file_str);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  __close_submenu();

  return _paw;
}

bool Bot::__find_paw_mantis() {
  size_t size_map = mantis_map.size() + 1;
  for (size_t i = 1; i < size_map; i++) {
    std::string f1 = "scrinShots/refMantisPos" + std::to_string(i) + ".jpg";

    std::string f2 = "scrinShots/MantisPos" + std::to_string(_number_thread) + std::to_string(i) + ".jpg";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, f2.c_str(), (int)f2.size(), NULL, 0);
    std::wstring wstr2(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, f2.c_str(), (int)f2.size(), &wstr2[0], size_needed);

    auto it = mantis_map.find(std::to_string(i));
    scrin(_rect.left + it->second.offset_x, _rect.top + it->second.offset_y, it->second.width, it->second.height, wstr2.c_str());

    __int64 dist = compare_images(f1,f2);

    if (dist < 5) {
      click_at_position_lock_guard(_rect.left + mantis_map.at(std::to_string(i)).coord_click.x, mantis_map.at(std::to_string(i)).coord_click.y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      return true;
    }
  }

  return false;
}

bool Bot::__take_paw_mantis() {
  //mantis
  click_at_position_lock_guard(_rect.left + offset_click_mantis_x, _rect.top + offset_click_mantis_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(7000));

  std::wstring image_paw_file_wstr = tmp_main_menu_image_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_paw_file_str =  tmp_main_menu_image_file +   std::to_string(_number_thread) +   ".jpg";

  scrin(_rect.left + offset_main_menu_x, _rect.top + offset_main_menu_y, offset_main_menu_w, offset_main_menu_h, image_paw_file_wstr.c_str());

  if (compare_images(ref_main_menu_image_file, image_paw_file_str) < 5) {
    click_at_position_lock_guard(_rect.left + offset_click_mantis_x, _rect.top + offset_click_mantis_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000));
  }

  // 3 lvl
  click_at_position_lock_guard(_rect.left + offset_click_mantis_3lvl_x, _rect.top + offset_click_mantis_3lvl_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));

  // recall
  scrin(_rect.left + offset_image_check_busy_paw_x, _rect.top + offset_image_check_busy_paw_y, offset_image_check_busy_paw_w, offset_image_check_busy_paw_h, image_paw_file_wstr.c_str());

  if (compare_images(ref_mantis_check_busy_paw_image_file, image_paw_file_str) > 10) {
    click_at_position_lock_guard(_rect.left + offset_click_mantis_recall_x, _rect.top + offset_click_mantis_recall_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    click_at_position_lock_guard(_rect.left + offset_click_mantis_recall_yes_x, _rect.top + offset_click_mantis_recall_yes_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  // Resetting lair
  __close_submenu();
  click_at_position_lock_guard(_rect.left + offset_click_mantis_resetting_lair_x, _rect.top + offset_click_mantis_resetting_lair_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  // 3 lvl
  click_at_position_lock_guard(_rect.left + offset_click_mantis_3lvl_x, _rect.top + offset_click_mantis_3lvl_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));

  click_at_position_lock_guard(_rect.left + offset_click_mantis_recall_confirm_collection_x, _rect.top + offset_click_mantis_recall_confirm_collection_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(8000));

  // page
  _controller_mutex.lock();
  click_at_position(_rect.left + offset_click_mantis_page_section_x, _rect.top + offset_click_mantis_page_section_y);

  std::this_thread::sleep_for(std::chrono::milliseconds(7000));
  
  remove_quantity(4);
  line_input(paw_search_home_page);

  click_at_position(_rect.left + offset_click_ok_input_line_x, _rect.top + offset_click_ok_input_line_y);
  _controller_mutex.unlock();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  for (size_t i = 0; i < 1000; i++) {
    if (__find_paw_mantis()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(4000));
      click_at_position_lock_guard(_rect.left + offset_click_mantis_capture_x, _rect.top + offset_click_mantis_capture_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      click_at_position_lock_guard(_rect.left + offset_click_mantis_capture_yes_x, _rect.top + offset_click_mantis_capture_yes_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));

      click_at_position_lock_guard(_rect.left + offset_click_mantis_capture_quick_choice_x, _rect.top + offset_click_mantis_capture_quick_choice_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      click_at_position_lock_guard(_rect.left + offset_click_mantis_capture_send_x, _rect.top + offset_click_mantis_capture_send_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));

      // Additional check that the paw was captured
      click_at_position_lock_guard(_rect.left + offset_click_random_pos_x, _rect.top + offset_click_random_pos_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      // back bag
      scrin(_rect.left + offset_back_bag_x, _rect.top + offset_back_bag_y, offset_back_bag_w, offset_back_bag_h, tmp_back_bag_image_file_L);
      if (compare_images(ref_back_bag_image_file, tmp_back_bag_image_file) < 10)
        __close_submenu();

      click_at_position_lock_guard(_rect.left + offset_click_random_pos_x, _rect.top + offset_click_random_pos_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      scrin(_rect.left + offset_image_check_busy_paw_x, _rect.top + offset_image_check_busy_paw_y, offset_image_check_busy_paw_w, offset_image_check_busy_paw_h, image_paw_file_wstr.c_str());

      if (compare_images(ref_mantis_check_busy_paw_image_file, image_paw_file_str) < 10) {
        click_at_position_lock_guard(_rect.left + offset_click_mantis_page_next_x, _rect.top + offset_click_mantis_page_next_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        continue;
        std::cout << "continue not work\n";
      }
        
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      __close_submenu();
      click_at_position_lock_guard(_rect.left + offset_click_mantis_exit_yes_x, _rect.top + offset_click_mantis_exit_yes_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      __close_submenu();

      scrin(_rect.left + offset_image_check_busy_paw_x, _rect.top + offset_image_check_busy_paw_y, offset_image_check_busy_paw_w, offset_image_check_busy_paw_h, image_paw_file_wstr.c_str());

      if (compare_images(ref_mantis_check_busy_paw_image_file, image_paw_file_str) < 10) {
        click_at_position_lock_guard(_rect.left + offset_click_mantis_page_next_x, _rect.top + offset_click_mantis_page_next_y, _controller_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        continue;
        std::cout << "continue not work\n";
      }

      return true;
    }

    click_at_position_lock_guard(_rect.left + offset_click_mantis_page_next_x, _rect.top + offset_click_mantis_page_next_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
  }

  return false;
}

bool Bot::__set_pos_window() {
  int new_width = 455;  // 417 LDPlayr
  int new_height = 809; // 775 LDPlayr
  UINT flags = SWP_NOZORDER | SWP_NOACTIVATE; // move
  //UINT flags = SWP_NOMOVE; // no move
  SetForegroundWindow(_hwnd);
  if (SetWindowPos(_hwnd, NULL, window_position.at(_number_thread).x, window_position.at(_number_thread).y, new_width, new_height, flags)) {
    GetWindowRect(_hwnd, &this->_rect);
    return true;
  }
  return false;
}

bool Bot::__start_game() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  click_at_position_lock_guard(_rect.left + offset_click_start_game_x, _rect.top + offset_click_start_game_y, _controller_mutex);

  const std::chrono::minutes max_duration(loop_work_time);
  const std::chrono::seconds check_interval(event_time_loop);

  auto start_time = std::chrono::steady_clock::now();

  while (true) {
    __close_submenu();

    std::wstring image_paw_file_wstr = tmp_main_menu_image_file_L + std::to_wstring(_number_thread) + L".jpg";
    std::string  image_paw_file_str =  tmp_main_menu_image_file +   std::to_string(_number_thread) +   ".jpg";

    scrin(_rect.left + offset_main_menu_x, _rect.top + offset_main_menu_y, offset_main_menu_w, offset_main_menu_h, image_paw_file_wstr.c_str());

    if (compare_images(ref_main_menu_image_file, image_paw_file_str) < 5)
      return true;

    auto elapsed_time = std::chrono::steady_clock::now() - start_time;
    if (elapsed_time >= max_duration) {
      std::cout << "The game launch time has exceeded " << loop_work_time << " minutes" << std::endl;
      return false;
    }

    std::this_thread::sleep_for(check_interval);
  }
  return false;
}

bool Bot::__open_res(Type_res type_res, unsigned int amount_res) {
  __close_submenu();

  amount_res = (float)amount_res / 0.75;

  unsigned int number_resource_boxes = 0;
  switch (type_res) {
  case Type_res::food:
    number_resource_boxes = amount_res / 50000;

    if (amount_res % 50000 > 0)
      ++number_resource_boxes;

    return __open_food(number_resource_boxes);

  case Type_res::leaves:
    number_resource_boxes = amount_res / 50000;

    if (amount_res % 50000 > 0)
      ++number_resource_boxes;

    return __open_leaves(number_resource_boxes);

  case Type_res::water:
    number_resource_boxes = amount_res / 10000;

    if (amount_res % 10000 > 0)
      ++number_resource_boxes;

    return __open_water(number_resource_boxes);

  case Type_res::mushrooms:
    number_resource_boxes = amount_res / 2500;

    if (amount_res % 2500 > 0)
      ++number_resource_boxes;

    return __open_mushrooms(number_resource_boxes);

  default:
    return false;
  }
  return false;
}

bool Bot::__open_food(unsigned int number_resource_boxes) {
  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  click_at_position_lock_guard(_rect.left + offset_click_food_x, _rect.top + offset_click_food_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  std::wstring image_paw_file_wstr = tmp_res_img_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_paw_file_str =  tmp_res_img_file +   std::to_string(_number_thread) +   ".jpg";

  int i = 0;
  while (true) {
    scrin(_rect.left + offset_food_res_x, _rect.top + array_offset_res_pos_y[i], area_number_res_w, area_number_res_h, image_paw_file_wstr.c_str());
    if (get_number_from_image(image_paw_file_str) == food_box)
      break;

    if(++i >= array_offset_res_pos_y.size())
      return false;
  }

  int number_box = __get_number_box(i);

  if (number_box < number_resource_boxes || number_box == -1) {
    click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    __close_submenu();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __open_box_from_opening_menu(number_resource_boxes);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return true;
}

bool Bot::__open_leaves(unsigned int number_resource_boxes) {
  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  click_at_position_lock_guard(_rect.left + offset_click_leaves_x, _rect.top + offset_click_leaves_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  std::wstring image_paw_file_wstr = tmp_res_img_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_paw_file_str =  tmp_res_img_file +   std::to_string(_number_thread) +   ".jpg";

  int i = 0;
  while (true) {
    scrin(_rect.left + offset_leaves_res_x, _rect.top + array_offset_res_pos_y[i], area_number_res_w, area_number_res_h, image_paw_file_wstr.c_str());
    if (get_number_from_image(image_paw_file_str) == leaves_box)
      break;

    if(++i >= array_offset_res_pos_y.size())
      return false;
  }

  int number_box = __get_number_box(i);

  if (number_box < number_resource_boxes || number_box == -1) {
    click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    __close_submenu();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __open_box_from_opening_menu(number_resource_boxes);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return true;
}

bool Bot::__open_water(unsigned int number_resource_boxes) {
  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  click_at_position_lock_guard(_rect.left + offset_click_water_x, _rect.top + offset_click_water_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  std::wstring image_paw_file_wstr = tmp_res_img_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_paw_file_str =  tmp_res_img_file +   std::to_string(_number_thread) +   ".jpg";

  int i = 0;
  while (true) {
    scrin(_rect.left + offset_water_res_x, _rect.top + array_offset_res_pos_y[i], area_number_res_w, area_number_res_h, image_paw_file_wstr.c_str());
    if (get_number_from_image(image_paw_file_str) == water_box)
      break;

    if(++i >= array_offset_res_pos_y.size())
      return false;
  }

  int number_box = __get_number_box(i);

  if (number_box < number_resource_boxes || number_box == -1) {
    click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    __close_submenu();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __open_box_from_opening_menu(number_resource_boxes);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return true;
}

bool Bot::__open_mushrooms(unsigned int number_resource_boxes) {
  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  click_at_position_lock_guard(_rect.left + offset_click_mushrooms_x, _rect.top + offset_click_mushrooms_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  std::wstring image_paw_file_wstr = tmp_res_img_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_paw_file_str =  tmp_res_img_file +   std::to_string(_number_thread) +   ".jpg";

  int i = 0;
  while (true) {
    scrin(_rect.left + offset_mushrooms_res_x, _rect.top + array_offset_res_pos_y[i], area_number_res_w, area_number_res_h, image_paw_file_wstr.c_str());
    if (get_number_from_image(image_paw_file_str) == mushrooms_box)
      break;

    if(++i >= array_offset_res_pos_y.size())
      return false;
  }

  int number_box = __get_number_box(i);

  if (number_box < number_resource_boxes || number_box == -1) {
    click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    __close_submenu();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __open_box_from_opening_menu(number_resource_boxes);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return true;
}

bool Bot::__presence_res(int number_pos_y) {
  if (number_pos_y >= array_offset_res_presence_pos_y.size() || number_pos_y < 0)
    return false;

  std::wstring image_presence_res_wstr = tmp_presence_res_img_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_presence_res_str =  tmp_presence_res_img +   std::to_string(_number_thread) +   ".jpg";

  scrin(_rect.left + offset_presence_res_image_res_x, _rect.top + array_offset_res_presence_pos_y[number_pos_y], offset_presence_res_image_res_w, offset_presence_res_image_res_h, image_presence_res_wstr.c_str());

  if(compare_images(res_presence_res_img, image_presence_res_str) < 21)
    return true;

  return false;
}

bool Bot::__click_open_box_res(int number_pos_y) {
  if (number_pos_y >= array_offset_res_presence_pos_y.size() || number_pos_y < 0)
    return false;

  click_at_position_lock_guard(_rect.left + offset_open_res_x, _rect.top + array_offset_open_res_pos_y[number_pos_y], _controller_mutex);
  return true;
}

int Bot::__get_number_box(int number_pos_y) {
  if (!__presence_res(number_pos_y))
    return -1;

  if (!__click_open_box_res(number_pos_y))
    return -1;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::wstring image_number_box_res_wstr = tmp_number_box_res_img_L + std::to_wstring(0) + L".jpg";
  std::string  image_number_box_res_str =  tmp_number_box_res_img +   std::to_string(0) +   ".jpg";

  scrin(_rect.left + offset_number_box_res_image_x, _rect.top + offset_number_box_res_image_y, offset_number_box_res_image_w, offset_number_box_res_image_h, image_number_box_res_wstr.c_str());

  return get_number_from_image(image_number_box_res_str);
}

void Bot::__open_box_from_opening_menu(int number_resource_boxes) {
  click_at_position_lock_guard(_rect.left + offser_click_input_number_box_open_x, _rect.top + offser_click_input_number_box_open_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  _controller_mutex.lock();
  remove_quantity(3);
  line_input(std::to_string(number_resource_boxes));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  _controller_mutex.unlock();

  click_at_position_lock_guard(_rect.left + offset_click_ok_input_line_x, _rect.top + offset_click_ok_input_line_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  click_at_position_lock_guard(_rect.left + offser_click_apply_box_open_x, _rect.top + offser_click_apply_box_open_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  __close_submenu();
}

void Bot::__close_submenu() {
  std::wstring image_presence_res_wstr = tmp_submenu_close_file_L + std::to_wstring(_number_thread) + L".jpg";
  std::string  image_presence_res_str =  tmp_submenu_close_file +   std::to_string(_number_thread) +   ".jpg";

  scrin(_rect.left + offset_submenu_close_x, _rect.top + offset_submenu_close_y, offset_submenu_close_w, offset_submenu_close_h, image_presence_res_wstr.c_str());

  if (compare_images(ref_submenu_close_file, image_presence_res_str) < 5) {
    click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void Bot::cure_soldier() {
  int start_coord_x = _rect.left + offset_drag_main_in_healer_start_x;
  int start_coord_y = _rect.top + offset_drag_main_in_healer_start_y;

  int end_coord_x = _rect.left + offset_drag_main_in_healer_end_x;
  int end_coord_y = _rect.top + offset_drag_main_in_healer_end_y;

  drag_mouse_position(start_coord_x, start_coord_y, end_coord_x, end_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  __close_submenu();

  drag_mouse_position(start_coord_x, start_coord_y, end_coord_x, end_coord_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  __close_submenu();

  click_at_position_lock_guard(_rect.left + offset_click_healer_x, _rect.top + offset_click_healer_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  scrin(_rect.left + offset_image_healer_x, _rect.top + offset_image_healer_y, offset_image_healer_w, offset_image_healer_h, tmp_free_healer_file_L);

  if (compare_images(ref_free_healer_file, tmp_free_healer_file) > 5) {
    scrin(_rect.left + offset_all_clear_image_healer_x ,_rect.top + offset_all_clear_image_healer_y, offset_all_clear_image_healer_w, offset_all_clear_image_healer_h, tmp_all_clear_file_L);

    if (compare_images(ref_all_clear_file, tmp_all_clear_file) < 5) {
      click_at_position_lock_guard(_rect.left + offset_click_all_clear_x, _rect.top + offset_click_all_clear_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    click_at_position_lock_guard(_rect.left + offset_click_plus_one_x, _rect.top + offset_click_plus_one_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    click_at_position_lock_guard(_rect.left + offset_click_heal_use_x, _rect.top + offset_click_heal_use_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    scrin(_rect.left + offset_open_res_healer_x, _rect.top + offset_open_res_healer_y, offset_open_res_healer_w, offset_open_res_healer_h, tmp_auto_open_res_file_L);

    if (compare_images(ref_auto_open_res_file, tmp_auto_open_res_file) < 5) {
      click_at_position_lock_guard(_rect.left + offset_click_auto_use_x, _rect.top + offset_click_auto_use_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      click_at_position_lock_guard(_rect.left + offset_click_yes_replenish_use_x, _rect.top + offset_click_yes_replenish_use_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      click_at_position_lock_guard(_rect.left + offset_click_heal_use_x, _rect.top + offset_click_heal_use_y, _controller_mutex);
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    __close_submenu();
    __close_submenu();
  }
}

bool Bot::buy_res(Type_res type_res, unsigned int amount_res) {
  __close_submenu();

  amount_res = (float)amount_res / 0.75;

  click_at_position_lock_guard(_rect.left + offset_click_mantis_x, _rect.top + offset_click_mantis_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(7000));

  click_at_position_lock_guard(_rect.left + offser_click_exchange_res_open_x, _rect.top + offser_click_exchange_res_open_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  int start_coord_x = _rect.left + offset_drag_main_in_exchange_res_start_x;
  int start_coord_y = _rect.top + offset_drag_main_in_exchange_res_start_y;

  int end_coord_x = _rect.left + offset_drag_main_in_exchange_res_end_x;
  int end_coord_y = _rect.top + offset_drag_main_in_exchange_res_end_y;

  for (size_t i = 0; i < 5; i++) {
    move_mouse_smoothly(start_coord_x, start_coord_y, end_coord_x, end_coord_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  unsigned int number_resource_boxes = 0;
  switch (type_res) {
  case Type_res::food:
    number_resource_boxes = amount_res / 50000;
    if (amount_res % 50000 > 0)
      ++number_resource_boxes;

    click_at_position_lock_guard(_rect.left + offser_click_exchange_res_food_x,  _rect.top + offser_click_exchange_res_food_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    break;

  case Type_res::leaves:
    number_resource_boxes = amount_res / 50000;
    if (amount_res % 50000 > 0)
      ++number_resource_boxes;

    click_at_position_lock_guard(_rect.left + offser_click_exchange_res_leaves_x,  _rect.top + offser_click_exchange_res_leaves_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    break;

  case Type_res::water:
    number_resource_boxes = amount_res / 10000;
    if (amount_res % 10000 > 0)
      ++number_resource_boxes;

    click_at_position_lock_guard(_rect.left + offser_click_exchange_res_water_x,  _rect.top + offser_click_exchange_res_water_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    break;

  case Type_res::mushrooms:
    number_resource_boxes = amount_res / 2500;
    if (amount_res % 2500 > 0)
      ++number_resource_boxes;

    click_at_position_lock_guard(_rect.left + offser_click_exchange_res_mushrooms_x,  _rect.top + offser_click_exchange_res_mushrooms_y, _controller_mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    break;

  default:
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::wstring image_number_box_res_wstr = tmp_number_box_res_img_L + std::to_wstring(0) + L".jpg";
  std::string  image_number_box_res_str =  tmp_number_box_res_img +   std::to_string(0) +   ".jpg";

  scrin(_rect.left + offset_number_box_res_image_x, _rect.top + offset_number_box_res_image_y, offset_number_box_res_image_w, offset_number_box_res_image_h, image_number_box_res_wstr.c_str());

  int number_resource_boxes_current = get_number_from_image(image_number_box_res_str);

  if (number_resource_boxes_current < number_resource_boxes)
    return false;

  __open_box_from_opening_menu(number_resource_boxes);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  click_at_position_lock_guard(_rect.left + offset_click_submenu_exit_x, _rect.top + offset_click_submenu_exit_y, _controller_mutex);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  __close_submenu();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return false;
}

std::string Bot::__process_string_with_time(const std::string& input) {
  std::string result;
  int digitCount = 0;

  for (size_t i = 0; i < input.length(); ++i) {
    char currentChar = input[i];

    if (std::isdigit(currentChar)) {
      result += currentChar;
      digitCount++;

      if (digitCount == 2) {
        if (i + 1 < input.length() && input[i + 1] != ':') {
          result += ':';
        }
        digitCount = 0;
      }
    }
    else if (currentChar == ':') {
      result += currentChar;
      digitCount = 0;
    }
  }

  return result;
}