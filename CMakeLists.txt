############################################################################################
#
#	CS-255 Graphics Programming Assignment
#
############################################################################################

cmake_minimum_required(VERSION 3.8.0)

project(CS255-Assignment)

# Hardcode Qt5 sdk location for now
set(CMAKE_PREFIX_PATH "G:/Qt/5.8/msvc2015_64/")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

############################################################################################

#
#	Main application
#
set(sources
	src/Main.cpp
	src/MainWindow.h
	src/MainWindow.cpp
)

add_executable(Application WIN32
	${sources}
)

target_link_libraries(Application
  PUBLIC
	Qt5::Widgets
)

# Set up IDE source folders
file(TO_NATIVE_PATH "${sources}" sources)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

############################################################################################

# Copy Qt DLL's to the same location as the application executable
foreach(_target
	Qt5::Widgets
	Qt5::Gui
	Qt5::Core
)
	add_custom_command(
		TARGET Application POST_BUILD
		COMMENT "Copying Qt libraries..."
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${_target}>
			$<TARGET_FILE_DIR:Application>
	)
endforeach()

############################################################################################
