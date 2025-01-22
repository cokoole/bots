#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_set>

using json = nlohmann::json;

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delimiter)) {
      tokens.push_back(item);
    }
    return tokens;
}

struct Authorized_user {
  long long id;
  std::string username;
};

json read_config(const std::string& filename) {
  std::ifstream config_file(filename);
  if (!config_file) {
    throw std::runtime_error("Failed to open configuration file: " + filename);
  }
  json config;
  config_file >> config;
  return config;
}

void initialize_db(sqlite3* db) {
  const char* create_table_SQL = 
    "CREATE TABLE IF NOT EXISTS authorized_users ("
    "id INTEGER PRIMARY KEY, "
    "username TEXT NOT NULL"
    ");";

  char* err_msg = nullptr;
  int rc = sqlite3_exec(db, create_table_SQL, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    std::string error = "SQL error: " + std::string(err_msg);
    sqlite3_free(err_msg);
    throw std::runtime_error(error);
  }
}

void add_users_to_db(sqlite3* db, const std::vector<Authorized_user>& users) {
  const char* insert_SQL = "INSERT OR IGNORE INTO authorized_users (id, username) VALUES (?, ?);";
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, insert_SQL, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Failed to prepare SQL query for insert");
  }

  for (const auto& user : users) {
    sqlite3_bind_int64(stmt, 1, user.id);
    sqlite3_bind_text(stmt, 2, user.username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      sqlite3_finalize(stmt);
      throw std::runtime_error("Failed to insert user into database");
    }

    sqlite3_reset(stmt);
  }

  sqlite3_finalize(stmt);
}

bool is_user_authorized(sqlite3* db, long long user_id) {
  const char* select_SQL = "SELECT COUNT(*) FROM authorized_users WHERE id = ?;";
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, select_SQL, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Failed SQL query to validate user");
  }

  sqlite3_bind_int64(stmt, 1, user_id);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to complete user verification request");
  }

  int count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return count > 0;
}

void get_all_data_resources(int sock) {
    send(sock, "get_all_data_resources", strlen("get_all_data_resources"), 0);
    
    char buffer[1024];
    recv(sock, buffer, sizeof(buffer), 0);
    
    std::cout << "Received resources:n" << buffer << std::endl;
}

void deliver_resource(int sock, const std::string& name, int type_res) {
    std::string command = "deliver_resource:" + name + "," + std::to_string(type_res);
    send(sock, command.c_str(), command.size(), 0);
}

int main() {
  sqlite3* db;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  try {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    
    get_all_data_resources(sock);
    
    deliver_resource(sock, "NewResource", 3);

////////////
    json config = read_config("config.json");

    std::string bot_token = config["bot_token"];
    std::string db_path = config["database_path"];

    std::vector<Authorized_user> authorized_users;
    for (const auto& user : config["authorized_users"]) {
      Authorized_user au;
      au.id = user["id"];
      au.username = user["username"];
      authorized_users.push_back(au);
    }

    
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
      std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
      return EXIT_FAILURE;
    }

    initialize_db(db);
    add_users_to_db(db, authorized_users);

    TgBot::Bot bot(bot_token);

    bot.getEvents().onCommand("start", [&bot, db](TgBot::Message::Ptr message) {
      long long user_id = message->from->id;

      try {
        long long user_id = message->from->id;
        if (!is_user_authorized(db, user_id)) {
          bot.getApi().sendMessage(message->chat->id, "Вы не авторизованный пользователь, идите на Юг!");
          return;
        }
      } catch (const std::exception& e) {
        std::cerr << "Ошибка проверки авторизации: " << e.what() << std::endl;
        bot.getApi().sendMessage(message->chat->id, "Ошибка в процессе запроса");
        return;
      }

      bot.getApi().sendMessage(message->chat->id, "Здаров! Го бухать");
    });

    bot.getEvents().onCommand("help", [&bot, db](TgBot::Message::Ptr message) {
      try {
        long long user_id = message->from->id;
        if (!is_user_authorized(db, user_id)) {
          bot.getApi().sendMessage(message->chat->id, "Вы не авторизованный пользователь, идите на Юг!");
          return;
        }

      bot.getApi().sendMessage(message->chat->id, "/deliver type_res coord: x y - отправить тип ресурса по координатам x y\n   food - еда,\n   leaves - листва,\n   water - вода,\n   mushrooms - грибы,\n/get_all_data - получить все данные по лапкам богомолов у ботов рессурсам у ботов\n");
      return;

      } catch (const std::exception& e) {
        std::cerr << "Ошибка проверки авторизации: " << e.what() << std::endl;
        bot.getApi().sendMessage(message->chat->id, "Ошибка в процессе запроса");
        return;
      }
    });

    bot.getEvents().onCommand("get_all_data", [&bot, db](TgBot::Message::Ptr message) {
      try {
        long long user_id = message->from->id;
        if (!is_user_authorized(db, user_id)) {
          bot.getApi().sendMessage(message->chat->id, "Вы не авторизованный пользователь, идите на Юг!");
          return;
        }

      bot.getApi().sendMessage(message->chat->id, "В разработке");
      return;

      // to do

      } catch (const std::exception& e) {
        std::cerr << "Ошибка проверки авторизации: " << e.what() << std::endl;
        bot.getApi().sendMessage(message->chat->id, "Ошибка в процессе запроса");
        return;
      }
    });

    bot.getEvents().onCommand("deliver", [&bot, db](TgBot::Message::Ptr message) {
      try {
        long long user_id = message->from->id;
        if (!is_user_authorized(db, user_id)) {
          bot.getApi().sendMessage(message->chat->id, "Вы не авторизованный пользователь, идите на Юг!");
          return;
        }

        if (message->text.empty()) {
            return;
        }

        std::vector<std::string> parts = split(message->text, ' ');

        if (parts.size() != 5) {
          bot.getApi().sendMessage(message->chat->id, "Неверный ввод. Пример: /deliver food 10 20 amount");
          return;
        }

        std::string type_res{parts[1]};
        int x = std::stoi(parts[2]);
        int y = std::stoi(parts[3]);
        int amount = std::stoi(parts[4])

        if (x > 511 || x < 0 || y > 1023 || y < 0) {
          bot.getApi().sendMessage(message->chat->id, "Некорректные значения координат, минимум: 0 и 0; максимум: 511 и 1023");
          return;
        }

        if (type_res != "food" && type_res != "leaves" && type_res != "water" && type_res != "mushrooms") {
          bot.getApi().sendMessage(message->chat->id, "Введён неверный тип ресурса");
          return;
        }

        if (amount < 0) {
          bot.getApi().sendMessage(message->chat->id, "Количество ресурсов не может быть отрицательным");
          return;
        }

        // to do

        bot.getApi().sendMessage(message->chat->id, ("Товар ушёл по координатам: " + std::to_string(x) + " " + std::to_string(y)));
        return;
      } catch (const std::invalid_argument& e) {
        bot.getApi().sendMessage(message->chat->id, "Невалидные значения чисел, необходимо ввести 3 целочисленных числа! Координаты и количество ресурса");
        return;
      } catch (const std::out_of_range& e) {
        bot.getApi().sendMessage(message->chat->id, "Числа вышли за рэндж инта");
        return;
      } catch (const std::exception& e) {
        std::cerr << "Ошибка проверки авторизации: " << e.what() << std::endl;
        bot.getApi().sendMessage(message->chat->id, "Ошибка в процессе запроса");
        return;
      } catch (...) {
        bot.getApi().sendMessage(message->chat->id, "Случился полный Pi****!");
        return;
      }
    }
    );

    std::cout << "Bot started..." << std::endl;
    TgBot::TgLongPoll long_poll(bot);
    while (true) {
      long_poll.start();
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  sqlite3_close(db);
  close(sock);

  return 0;
}