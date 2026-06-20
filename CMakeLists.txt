cmake_minimum_required(VERSION 3.10)
project(CaroTitanMI3060 VERSION 1.0)

# Yêu cầu C++ chuẩn 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm kiếm thư viện SFML (Yêu cầu các module: Graphics, Window, System, Audio)

# Gom tất cả các file mã nguồn (.cpp) trong dự án lại
file(GLOB_RECURSE SOURCES 
    "src/main.cpp"
    "src/core/*.cpp"
    "src/ui/*.cpp"
)

# Tạo file thực thi (file game) từ các file mã nguồn trên
add_executable(${PROJECT_NAME} ${SOURCES})

# Liên kết file game với thư viện SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

# Copy thư mục assets vào thư mục build sau khi biên dịch để game load được ảnh/font
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)