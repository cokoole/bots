#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>

#include <iostream>
#include <string>

#include <workingImages.h>

namespace {
  __int64 __calc_image_hash(const cv::Mat& src) {
    if (src.empty()) {
      return 0;
    }

    cv::Mat res, gray, bin;

    cv::resize(src, res, cv::Size(9, 9));

    cv::cvtColor(res, gray, cv::COLOR_BGR2GRAY);

    cv::Scalar average = cv::mean(gray);

    cv::threshold(gray, bin, average[0], 255, cv::THRESH_BINARY);

    __int64 hash = 0;
    int i = 0;

    for (int y = 0; y < bin.rows; y++) {
      const uchar* ptr = bin.ptr<uchar>(y);
      for (int x = 0; x < bin.cols; x++) {
        if (ptr[x]) {
          hash |= 1LL << i;
        }
        i++;
      }
    }
    //std::cout << "hash: " << std::hex << hash << std::endl;
    return hash;
  }

  int64_t __calc_hamming_distance(int64_t x, int64_t y) {
    int64_t dist = 0;
    int64_t val = x ^ y;

    while (val) {
      ++dist; 
      val &= val - 1;
    }

    return dist;
  }

  void __remove_commas(std::string &str) {
      str.erase(std::remove(str.begin(), str.end(), ','), str.end());
  }
}

__int64 image_to_hash(std::string object_filename) {
  cv::Mat object = cv::imread(object_filename, cv::IMREAD_COLOR);

  if (object.empty()) {
    std::cerr << "Error: can't load object image: " << object_filename << std::endl;
    return -1;
  }

  return __calc_image_hash(object);
}

std::string get_string_rus_from_image(std::string image_filename) {
  cv::Mat img = cv::imread(image_filename);
  if (img.empty()) {
      std::cerr << "Not open image!" << std::endl;
      return "-1";
  }

  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  cv::Mat binary;
  cv::threshold(gray, binary, 150, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
  if (ocr->Init("C:/tesseract/tessdata", "rus")) {
      std::cerr << "Not init Tesseract!" << std::endl;
      return "-1";
  }

  ocr->SetImage(binary.data, binary.cols, binary.rows, 1, binary.step[0]);

  char* out_text = ocr->GetUTF8Text();
  std::string recognized_text(out_text);

  delete[] out_text;
  ocr->End();

  __remove_commas(recognized_text);
  
  return recognized_text;
}

std::string get_string_eng_from_image(std::string image_filename) {
  cv::Mat img = cv::imread(image_filename);
  if (img.empty()) {
      std::cerr << "Not open image!" << std::endl;
      return "-1";
  }

  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  cv::Mat binary;
  cv::threshold(gray, binary, 150, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
  if (ocr->Init("C:/tesseract/tessdata", "eng")) {
      std::cerr << "Not init Tesseract!" << std::endl;
      return "-1";
  }

  ocr->SetImage(binary.data, binary.cols, binary.rows, 1, binary.step[0]);

  char* out_text = ocr->GetUTF8Text();
  std::string recognized_text(out_text);

  delete[] out_text;
  ocr->End();

  __remove_commas(recognized_text);
  
  return recognized_text;
}

int get_number_from_image(std::string image_filename) {
  cv::Mat img = cv::imread(image_filename);
  if (img.empty()) {
      std::cerr << "Not open image!" << std::endl;
      return -1;
  }

  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  cv::Mat binary;
  cv::threshold(gray, binary, 150, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
  if (ocr->Init("extern", "eng")) {
      std::cerr << "Not init Tesseract!" << std::endl;
      return -1;
  }

  ocr->SetImage(binary.data, binary.cols, binary.rows, 1, binary.step[0]);

  char* out_text = ocr->GetUTF8Text();
  std::string recognized_text(out_text);

  delete[] out_text;
  ocr->End();

  __remove_commas(recognized_text);

  int result = -1;
  try {
    result = std::stoi(recognized_text);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  
  return result;
}

__int64 compare_images(std::string object_filename, std::string image_filename) {
  cv::Mat object = cv::imread(object_filename, cv::IMREAD_COLOR);
  cv::Mat image = cv::imread(image_filename, cv::IMREAD_COLOR);
  
  //std::cout << "object: " << object_filename << std::endl;
  //std::cout << "image: " << image_filename << std::endl;

  if (object.empty()) {
    std::cerr << "Error: can't load object image: " << object_filename << std::endl;
    return -1;
  }
  
  if (image.empty()) {
    std::cerr << "Error: can't load test image: " << image_filename << std::endl;
    return -1;
  }
  
  __int64 hashO = __calc_image_hash(object);
  __int64 hashI = __calc_image_hash(image);
  
  __int64 dist = __calc_hamming_distance(hashO, hashI);
  
  return dist;
}

