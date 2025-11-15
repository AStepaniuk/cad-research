add_subdirectory(main_window)
add_subdirectory(doc)
add_subdirectory(view)
add_subdirectory(editor)

include(FetchContent)

add_executable(
  cadgui
  main.cpp
  ${GUI_MAIN_WINDOW_SOURCES}
  ${GUI_DOC_SOURCES}
  ${GUI_VIEW_SOURCES}
  ${GUI_EDITOR_SOURCES}
)

target_include_directories(
  cadgui
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GUI_MAIN_WINDOW_INCLUDES}
  ${GUI_DOC_INCLUDES}
  ${GUI_VIEW_INCLUDES}
  ${GUI_EDITOR_INCLUDES}
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.4
)

set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(IMGUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/*.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

add_library(imgui_backend STATIC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
)

target_link_libraries(
    cadgui
    PRIVATE
    OpenGL::GL
    imgui
    imgui_backend
    glfw
    corecad
    plan
)
