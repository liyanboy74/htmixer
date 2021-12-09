cmake_minimum_required(VERSION 3.6)

project(htmixer)

add_executable(${PROJECT_NAME} htmixer.c)

install(TARGETS htmixer DESTINATION bin)
