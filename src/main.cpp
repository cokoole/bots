#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <fstream>
#include <gdiplus.h>
#include <clocale>
#include <unordered_map>
#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>
#include <opencv2/opencv.hpp>
#include <locale>
#include <algorithm>

#include <workingImages.h>
#include "scrinMonitor.h"
#include "common.h"
#include "sqlite3.h"
#include "bot.h"
#include "type.h"

#pragma comment(lib, "Ws2_32.lib")

typedef struct {
  char name[9];
  int type_res;
  int amount_resource;
} DataBots;

void deliver_resource(std::string name, int type_res) {
  std::cout << "Received resource: " << name << ", Type: " << type_res << std::endl;
}

#define loop_work_time 15
#define event_time_loop 2

#define offset_send_res_time_image_x 144
#define offset_send_res_time_image_y 148
#define offset_send_res_time_image_w 56
#define offset_send_res_time_image_h 21

#define tmp_send_res_time_img_L L"scrinShots/tmpSendResTime"
#define tmp_send_res_time_img    "scrinShots/tmpSendResTime"

//#define res_presence_res_img_L L"scrinShots/resPresenceRes.jpg"
//#define res_presence_res_img    "scrinShots/resPresenceRes.jpg"

std::mutex mtx;
std::condition_variable cond_var;
std::atomic<int> current_thread_id(0);

void thread_function(int thread_id, int bot_id, int number_loop) {
  int restart = 0;
  
  for (int restart_count = 0; restart_count < 5; ++restart_count) {
    Bot bot(bot_id, thread_id);

    if (bot.IsActive) {
      std::cout << "Good\n";
      bot.cure_soldier();
      if (!bot.start_mantis()) {
        std::cout << "Faile\n" << number_loop << std::endl;
        bot.exit();
        continue;
      }
    } else {
      std::cout << "Faile\n" << number_loop << std::endl;
      bot.exit();
      continue;
    }

    std::cout << "paw bot_" << bot_id << ": " << bot.get_paw() << std::endl;
    bot.exit();
  }

  {
    std::lock_guard<std::mutex> lock(mtx);
    current_thread_id.store(thread_id + 1);
  }
  cond_var.notify_one();
}


int main() {
  const int num_threads = 4;
  const int num_bots = 8;

  int number_loop = 0;

  while (true) {
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
      threads.emplace_back(thread_function, i, i, number_loop);
    }

    for (int bot_id = num_threads; bot_id < num_bots; ++bot_id) {
      std::unique_lock<std::mutex> lock(mtx);
      cond_var.wait(lock, [&bot_id, num_threads]() {
                return current_thread_id.load() == bot_id - num_threads + 1;
            });

      threads[bot_id % num_threads].join();
      threads[bot_id % num_threads] = std::thread(thread_function, bot_id % num_threads, bot_id, number_loop);
    }

    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }

    std::cout << "All bots have finished. Waiting for 2 hours before restarting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::hours(2));
    ++number_loop;
  }

  return 0; //
  // Bot bot0(1, 3);
  // int amount_res = 2400000;
  // bot0.buy_res(Type_res::food, amount_res);
  //bot0.__open_res(Type_res::food, amount_res);
  //bot0.__open_res(Type_res::food, amount_res);
  //bot0.__open_res(Type_res::water, 100000);
  //bot0.__open_res(Type_res::mushrooms, amount_res);
  //bot0.deliver_resource({215, 439}, Type_res::food, amount_res);
  //amount_res = 100000;
  //bot0.deliver_resource({215, 439}, Type_res::water, amount_res);
  //std::cout << "Time deliver: " << bot0.get_total_seconds_delivery_time().count() << "\n";
  //bot0.exit();
  //return 0;
  // std::wstring image_number_box_res_wstr = tmp_send_res_time_img_L + std::to_wstring(0) + L".jpg";
  // std::string  image_number_box_res_str =  tmp_send_res_time_img +   std::to_string(0) +   ".jpg";
// 
  // scrin(-1920 + offset_send_res_time_image_x, offset_send_res_time_image_y, offset_send_res_time_image_w, offset_send_res_time_image_h, image_number_box_res_wstr.c_str());
// 
  // std::string timeStr = get_string_eng_from_image(image_number_box_res_str);
// 
  // return 0;
// 
  // Bot bot_0(0, 0);
  // std::cout << "paw bot_0: " << bot_0.get_paw() << std::endl;
// 
  // return 0; //

  const std::chrono::hours max_duration(loop_work_time);
  const std::chrono::hours check_interval(event_time_loop);

  auto start_time = std::chrono::steady_clock::now();

  int i = 0;

  while (true) {
    std::cout << "loop: " << i << std::endl;

  int number_thread = 3;

  for (size_t number_bot = 0; number_bot <= 5; number_bot++) {

    int restart = 0;
  bot:
      if (restart > 5)
        continue;

      Bot bot(number_bot, number_thread);

      if (bot.IsActive) {
        std::cout << "Good\n";
        bot.cure_soldier();
        if (!bot.start_mantis()) {
          std::cout << "Faile\n" << i << std::endl;
          bot.exit();
          ++restart;
          goto bot;
        }
      } else {
        std::cout << "Faile\n" << i << std::endl;
        ++restart;
        bot.exit();
        goto bot;
      }

      std::cout << "paw bot_" << number_bot << ": " << bot.get_paw() << std::endl;
      bot.exit();
    }

    auto elapsed_time = std::chrono::steady_clock::now() - start_time;
    if (elapsed_time >= max_duration) {
      std::cout << "Worked successfully " << loop_work_time << " hours" << std::endl;
      return 0;
    }

    ++i;
    std::this_thread::sleep_for(check_interval);

  }

  return 0; //

  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed." << std::endl;
    return 1;
  }

  int serverSock = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSock == INVALID_SOCKET) {
    std::cerr << "Socket creation failed." << std::endl;
    WSACleanup();
    return 1;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cerr << "Bind failed." << std::endl;
    closesocket(serverSock);
    WSACleanup();
    return 1;
  }

  if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
    std::cerr << "Listen failed." << std::endl;
    closesocket(serverSock);
    WSACleanup();
    return 1;
  }

  std::cout << "Server listening on 127.0.0.1:8080" << std::endl;

  while (true) {
    int clientSock = accept(serverSock, nullptr, nullptr);
    if (clientSock == INVALID_SOCKET) {
      std::cerr << "Accept failed." << std::endl;
      continue;
    }

    char buffer[1024] = {0};
    recv(clientSock, buffer, sizeof(buffer), 0);
    
    std::cout << "Received command: " << buffer << std::endl;

    if (strcmp(buffer, "get_all_data") == 0) {
      const char* response = "Resource1, Resource2, Resource3";
      send(clientSock, response, strlen(response), 0);
    } else if (strncmp(buffer, "deliver_resource:", 17) == 0) {
        std::cout << "Delivering resource: " << (buffer + 17) << std::endl;
    }

    closesocket(clientSock);
  }

  closesocket(serverSock);
  WSACleanup();

  return 0; //
}
