cmake_minimum_required(VERSION 3.10)
project(PUCCH_FORMAT_2)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    main.cpp
    mods.cpp
    work_with_json.cpp
    channel_simulation/AWGN_channel_model.cpp
    channel_simulation/QPSK_mod_and_demod.cpp
    channel_simulation/encoder_and_decoder.cpp
)

add_executable(pucchf2 ${SOURCES})