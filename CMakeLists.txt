cmake_minimum_required(VERSION 3.14)
project(MyQtProject)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt5 package
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)

# Include the current directory for the generated files like ui_*.h files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add source and header files
file(GLOB SOURCES "source/*.cpp")
file(GLOB HEADERS "headers/*.h")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link the Qt5 libraries
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Core Qt6::Gui)


# Include headers directory
target_include_directories(${PROJECT_NAME} PRIVATE headers)

# Post-build copy command
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets to build directory"
)
