cmake_minimum_required(VERSION 3.10)
project(CompClub)


set(CMAKE_CXX_STANDARD 20)

add_library(ClubManager ClubManager.cpp)
add_library(Time Time.cpp)
add_library(FileParser FileParser.cpp)

target_include_directories(ClubManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(FileParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main
	main.cpp
	ClubManager.cpp
	Time.cpp
	FileParser.cpp)
