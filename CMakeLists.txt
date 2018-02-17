############################################################################################
#
#	CS-255 Graphics Programming Assignment
#
############################################################################################

cmake_minimum_required(VERSION 3.8.0)

project(CS255-Assignment)

# Hardcode Qt5 sdk location for now
#set(CMAKE_PREFIX_PATH "G:/Qt/5.8/msvc2015_64/")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

message(STATUS "Finding Qt5 SDK...")

find_package(Qt5 COMPONENTS Widgets Concurrent REQUIRED)

if (Qt5_FOUND)
	message(STATUS "Qt5 SDK found: ${Qt5_DIR}")
else()
	message(FATAL_ERROR "Qt5 SDK not found")
endif()

############################################################################################
#	Main application
############################################################################################

set(sources
	src/Main.cpp
	src/MainWindow.h
	src/MainWindow.cpp
	src/Volume.h
	src/Volume.cpp
	src/VolumeRender.h
	src/VolumeRender.cpp
	src/VolumeSubimage.h
	src/VolumeSubimage.cpp
	src/Samplers.h
    src/Effect.h
	src/HistogramEqualization.h
	src/HistogramEqualization.cpp
	src/util/CountingIterator.h
	src/util/LabelledSlider.h
	src/util/LabelledSlider.cpp
)

add_executable(Application WIN32
	${sources}
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
#	Find CThead data set
############################################################################################

find_file(CT_DATASET NAME "CThead" PATHS "${CMAKE_SOURCE_DIR}" DOC "Volume dataset" NO_DEFAULT_PATH)

if (NOT EXISTS ${CT_DATASET})
	message(ERROR "Could not find CThead dataset")
endif()

# Copy dataset to working directory
file(COPY ${CT_DATASET} DESTINATION ${CMAKE_BINARY_DIR})

############################################################################################
#	Setup installation rules
############################################################################################

# Install redist
include(InstallRequiredSystemLibraries)
# Install application
install(TARGETS Application DESTINATION bin)
# Install CT dataset
install(FILES ${CT_DATASET} DESTINATION bin)

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
			#COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
			COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE:Application>"
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
			COMMENT "Copying Qt libraries..."
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:${_target}>
				$<TARGET_FILE_DIR:Application>
		)
	endforeach()
endif()

############################################################################################
