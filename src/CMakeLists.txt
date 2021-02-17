cmake_minimum_required(VERSION 3.13)
project(zfsev VERSION 0.0.0 DESCRIPTION "zfs events, streamed")
add_executable(zfsev ./main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZFSEV REQUIRED libzfs)

target_link_libraries(zfsev ${ZFSEV_LIBRARIES})
target_include_directories(zfsev PUBLIC ${ZFSEV_INCLUDE_DIRS})
target_compile_options(zfsev PUBLIC -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE ${ZFSEV_CFLAGS_OTHER})

include(GNUInstallDirs)
install(TARGETS zfsev)
