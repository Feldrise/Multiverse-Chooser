cmake_minimum_required(VERSION 3.6)
project(multiverse-chooser)

# CMake variables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# user config config file
include("${CMAKE_SOURCE_DIR}\\user-config.cmake")

# include packages
find_package(Qt5 5.11.2 REQUIRED COMPONENTS Core Widgets Network)

# includes sources files
include_directories(${CMAKE_SOURCE_DIR}/src)
file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.hpp
    src/*.cpp
)

add_executable(multiverse-chooser WIN32 ${SOURCE_FILES} ic_multiverse.rc)

# include libs 
set(MULTIVERSE_LIBS Qt5::Widgets Qt5::Network)
target_link_libraries(multiverse-chooser ${MULTIVERSE_LIBS})

# deploy
get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

add_custom_command(TARGET multiverse-chooser POST_BUILD COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:multiverse-chooser>)

# Visual Studio Filters
foreach(_source IN ITEMS ${SOURCE_FILES})
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel "${CMAKE_SOURCE_DIR}" "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()