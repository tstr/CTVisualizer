############################################################################################
#
#	CS-255 Graphics Programming Assignment
#
############################################################################################

cmake_minimum_required(VERSION 3.8.0)

project(CS255-Assignment)

# Configure CMake variables

set(CMAKE_DEBUG_POSTFIX "d")

# Qt tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# default build type is release
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "No CMAKE_BUILD_TYPE was specified, setting to Release")
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()

############################################################################################
#	Finding Qt SDK
############################################################################################

message(STATUS "Finding Qt5 SDK...")

find_package(Qt5 CONFIG COMPONENTS Widgets Concurrent PATH_SUFFIXES "lib/cmake/Qt5" REQUIRED)

if (Qt5_FOUND)
	message(STATUS "Qt5 SDK found: ${Qt5_DIR}")
else()
	message(FATAL_ERROR "Qt5 SDK not found")
endif()

############################################################################################
#	Main application
############################################################################################

set(sources
	config.ini
	src/Main.cpp

	src/gui/MainWindow.h
	src/gui/MainWindow.cpp
	src/gui/SubimageView.h
	src/gui/SubimageView.cpp
	src/gui/LabelledSlider.h
	src/gui/LabelledSlider.cpp
	
	src/gfx/Volume.h
	src/gfx/Volume.cpp
	src/gfx/VolumeRender.h
	src/gfx/VolumeRender.cpp
	src/gfx/VolumeSubimage.h
	src/gfx/VolumeSubimage.cpp
	src/gfx/VolumeSubimageRange.h
	src/gfx/Samplers.h
    src/gfx/PixmapDrawer.h
	src/gfx/HistogramEqualization.h
	src/gfx/HistogramEqualization.cpp
	src/gfx/Camera.h

	src/util/ImageBuffer.h
	src/util/CountingIterator.h
)

add_executable(Application WIN32
	${sources}
)

target_include_directories(Application
  PRIVATE
    src
)

target_link_libraries(Application
  PUBLIC
	Qt5::Widgets
    Qt5::Concurrent
)

############################################################################################
#	Set up IDE source folders
############################################################################################

# Project source group
file(TO_NATIVE_PATH "${sources}" sources)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

# Qt generated files source group
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "generated")

############################################################################################
#	Find volume dataset
############################################################################################

set(CT_DATASET "${PROJECT_SOURCE_DIR}/CThead" CACHE FILEPATH "Volume dataset")

if (NOT EXISTS ${CT_DATASET})
	message(FATAL_ERROR "Could not find volume dataset")
endif()

# Copy dataset to working directory
file(COPY ${CT_DATASET} DESTINATION ${PROJECT_BINARY_DIR})

# Copy config file to working directory
configure_file(
	"${PROJECT_SOURCE_DIR}/config.ini"
	"${PROJECT_BINARY_DIR}/config.ini"
)

############################################################################################
#	Setup installation rules
############################################################################################

# Install redist
include(InstallRequiredSystemLibraries)
# Install application
install(TARGETS Application DESTINATION bin)
# Install CT dataset
install(FILES ${CT_DATASET} DESTINATION bin)
# Install config file
install(FILES "${PROJECT_BINARY_DIR}/config.ini" DESTINATION bin)

#
#	Script adapted from this answer:
#	https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building
#
if (WIN32)
	
	if (TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
	
		# Set up imported target for convenience
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)

		execute_process(
			COMMAND "${_qmake_executable}" -query QT_INSTALL_PREFIX
			RESULT_VARIABLE return_code
			OUTPUT_VARIABLE qt5_install_prefix
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		
		set(_imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

		if(EXISTS ${_imported_location})
			add_executable(Qt5::windeployqt IMPORTED)

			set_target_properties(Qt5::windeployqt PROPERTIES
				IMPORTED_LOCATION ${_imported_location}
			)
		else()
			message(WARNING "Unable to find windeployqt.exe")
		endif()
	endif()
	
	if (TARGET Qt5::windeployqt)
		# execute windeployqt in a tmp directory
		add_custom_target(DEPLOY ALL
			COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
			COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
			COMMAND Qt5::windeployqt
					$<$<CONFIG:Debug>:"--debug">
					$<$<CONFIG:Release>:"--release">
					#--release
					--no-translations
					--no-system-d3d-compiler
					--dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
					"$<TARGET_FILE:Application>"
		)

		# copy deployment directory during installation
		install(
			DIRECTORY
			"${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
			DESTINATION bin
		)
	endif()	
endif()

############################################################################################
#	Copy windows DLL's to working directory
############################################################################################

if (WIN32)
	# Copy Qt DLL's to the same location as the application executable
	foreach(_target
        Qt5::Concurrent
		Qt5::Widgets
		Qt5::Gui
		Qt5::Core
	)
		add_custom_command(
			TARGET Application POST_BUILD
			COMMENT "Copying ${_target}"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:${_target}>
				$<TARGET_FILE_DIR:Application>
		)
	endforeach()
endif()

############################################################################################
