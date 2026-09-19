include(FetchContent)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

CPMFindPackage(
    NAME glfw
    VERSION 3.5.1
    GIT_TAG 3.5.1
    GITHUB_REPOSITORY glfw/glfw
    SYSTEM YES
    EXCLUDE_FROM_ALL YES
)

CPMFindPackage(
    NAME imgui
    VERSION 1.92.4
    GITHUB_REPOSITORY ocornut/imgui
    SYSTEM YES
    EXCLUDE_FROM_ALL YES
)

file(GLOB IMGUI_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

add_library(imgui_backend STATIC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_backend PRIVATE
    imgui
    glfw
    OpenGL::GL
)

add_library(imgui_setup INTERFACE)

target_link_libraries(imgui_setup INTERFACE
    OpenGL::GL
    imgui
    imgui_backend
    glfw
)
