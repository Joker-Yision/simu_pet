cmake_minimum_required(VERSION 3.16)
project(Simu_PET)

find_package(Geant4 REQUIRED ui_all vis_all)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(simu_pet simu_pet.cc ${sources} ${headers})

target_include_directories(simu_pet PRIVATE include)
target_link_libraries(simu_pet PRIVATE ${Geant4_LIBRARIES})

# 在你的 CMakeLists.txt 中添加
set(CMAKE_INSTALL_RPATH "/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu")