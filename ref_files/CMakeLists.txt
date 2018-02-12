cmake_minimum_required(VERSION 2.8)

add_executable(projet main.cpp socket_client.cpp socket_server.cpp input_interface.cpp video_capture.cpp)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(projet ${OpenCV_LIBRARIES})
else(OpenCV_FOUND)
    message(FATAL ERROR "Librarie OpenCV introuvable!")
endif(OpenCV_FOUND)
