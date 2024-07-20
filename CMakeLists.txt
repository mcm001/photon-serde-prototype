project(meme)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(meme src/main.cpp)
target_include_directories(meme PRIVATE src)
