cmake_minimum_required(VERSION 3.23)
project(seqcli C)

set(CMAKE_C_STANDARD 99)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif ()

add_executable(seqcli main.c cursor.c cursor.h colors.c colors.h grid.h grid.c select.c select.h viewport.c viewport.h brightness.h brightness.c screen.h gridscreen.c gridscreen.h)

target_link_libraries(seqcli ncurses)
