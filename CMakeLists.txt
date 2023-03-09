cmake_minimum_required(VERSION 3.23)
project(project1 C)
set(CMAKE_C_STANDARD 99)

#For Clion the make file will be differnt I think? idk
#God I am tired
add_executable(project1
                #files for the projecy
                        queue.c
                        queue.h
                        process.h
                        driver.c
                        multilevelQueueScheduler.c
                        multilevelQueueScheduler.h
                        processSimulator.c
                        processSimulator.h
                )
include_directories("${PROJECT_SOURCE_DIR}/")
