cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 7e5cd23b4c47972b70b99c39617d6e38f05d1b3a 
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(ftxui-play
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(ftxui-play src/main.cpp)
target_include_directories(ftxui-play PRIVATE src)

target_link_libraries(ftxui-play
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
)

install(TARGETS ftxui-play RUNTIME DESTINATION "bin")
