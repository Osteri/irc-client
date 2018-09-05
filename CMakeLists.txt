cmake_minimum_required(VERSION 3.1)

project(irc-client)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "-Wall -Wno-error -fno-omit-frame-pointer -fsanitize=address")
SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

# ignore IDE macro
set(void ${QT_QMAKE_EXECUTABLE})

# find
find_package(Boost REQUIRED COMPONENTS system program_options)
if(Boost_FOUND)
    include_directories(${BOOST_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} main.cpp config.cpp connection.cpp)

# link
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} pthread)
