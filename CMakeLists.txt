cmake_minimum_required(VERSION 3.25)

project(bots VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenCV_DIR "C:/opt/opencv/build")
set(Tesseract_DIR "C:/tesseract")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${Tesseract_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/extern")

link_directories("${Tesseract_DIR}/lib")

find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
  message(FATAL_ERROR "OpenCV не найден. Установите OpenCV и повторите попытку.")
endif()

find_package(Tesseract CONFIG REQUIRED)
if(NOT Tesseract_FOUND)
  message(FATAL_ERROR "Tesseract не найден. Установите Tesseract и повторите попытку.")
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCES
  src/main.cpp
  src/scrinMonitor.cpp
  src/common.cpp
  src/workingImages.cpp
  src/bot.cpp
  extern/sqlite3.c
)

set(CMAKE_C_RUNTIME_LIBRARY "MultiThreaded")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_RUNTIME_LIBRARY "MultiThreadedDebug")
endif()
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} gdi32 gdiplus Tesseract::libtesseract Ws2_32)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/scrinShots"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scrinShots"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/hashImage"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hashImage"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/extern"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/extern"
)

set(FOLDERS_TO_COPY scrinShots hashImage extern)

foreach(folder IN LISTS FOLDERS_TO_COPY)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/${folder}"
    "$<TARGET_FILE_DIR:bots>/${folder}"
    COMMENT "Копирование каталога ${folder} в выходной каталог"
  )
endforeach()

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)
install(DIRECTORY scrinShots/ DESTINATION bin/scrinShots)
install(DIRECTORY hashImage/ DESTINATION bin/hashImage)
install(DIRECTORY extern/ DESTINATION bin/extern)

include(CPack)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "MyBots")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "cokoole@cokoole.ru")
set(CPACK_NSIS_DISPLAY_NAME "MyBots")
set(CPACK_NSIS_INSTALL_ROOT "C:/Program Files/MyBots")