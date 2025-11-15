# Create an empty executable (you can add sources later)
add_executable(cadconsole main.cpp)

target_link_libraries(cadconsole PRIVATE corecad plan)
