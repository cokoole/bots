#pragma once
#include <string>

__int64 compare_images(std::string object_filename, std::string image_filename);
__int64 image_to_hash(std::string object_filename);
int get_number_from_image(std::string image_filename);
std::string get_string_rus_from_image(std::string image_filename);
std::string get_string_eng_from_image(std::string image_filename);