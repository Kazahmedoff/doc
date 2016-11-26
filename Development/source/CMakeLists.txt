project (PixelsProcessing)
cmake_minimum_required(VERSION 3.5.1)

#подключение библиотеки Boost
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(DCMTK_DIR "D:/DCMTK/cmake")


find_package (DCMTK REQUIRED PATH ${DCMTK_DIR} NO_DEFAULT_PATH)

if (DCMTK_FOUND)
	message ("DCMTK was found at ${DCMTK_DIR}")
elseif (NOT DCMTK_FOUND)
	message (FATAL_ERROR "Could not find DCMTK at ${DCMTK_PATH}")
endif (DCMTK_FOUND)

include_directories("D:/DCMTK/include")
link_directories("D:/DCMTK/lib")

FIND_PACKAGE(Boost 1.60.0 COMPONENTS filesystem system)

add_executable (PixelsProcessing PixelsProcessing.cpp)

IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	TARGET_LINK_LIBRARIES(PixelsProcessing ${Boost_LIBRARIES})
ELSE()
	TARGET_LINK_LIBRARIES(PixelsProcessing ${CONAN_LIBS})
ENDIF()

target_link_libraries(PixelsProcessing ${DCMTK_LIBRARIES})