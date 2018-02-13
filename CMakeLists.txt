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
	src/Volume.h
	src/Volume.cpp
	src/VolumeProcessing.h
	src/VolumeSubimage.h
	src/VolumeSubimage.cpp
	src/VolumeEqualization.h
	src/Samplers.h
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

#
#	Find CThead data set
#
find_file(CT_DATASET NAME "CThead" PATHS "${CMAKE_SOURCE_DIR}" DOC "Volume dataset" NO_DEFAULT_PATH)

if (NOT EXISTS ${CT_DATASET})
	message(ERROR "Could not find CThead dataset")
endif()

# Copy dataset to working directory
file(COPY ${CT_DATASET} DESTINATION ${CMAKE_BINARY_DIR})

############################################################################################

if (WIN32)
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
endif()

############################################################################################
