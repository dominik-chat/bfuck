#
# Copyright 2023 Dominik Chat
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.9)
project(bfuck)

add_executable(bfuck src/main.c)
target_include_directories(bfuck PRIVATE src)
target_compile_options(bfuck PRIVATE -O2 -Wall -Wextra)

install(TARGETS bfuck RUNTIME DESTINATION bin)
