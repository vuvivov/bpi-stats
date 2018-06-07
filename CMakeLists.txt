cmake_minimum_required(VERSION 3.10)
project (bpistats)

#compiler
set(CMAKE_CXX_STANDARD 17)

#packages
set(REQUIRED_BOOST "1.67.0")
set(Boost_USE_STATIC_LIBS on)
find_package(
        Boost ${REQUIRED_BOOST}
        REQUIRED COMPONENTS
     tokenizer
	date_time
	accumulators
	unit_test_framework
	algorithm
	beast
	asio
)
if(Boost_FOUND)
    message(STATUS "Boost: ${Boost_INCLUDE_DIRS}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

#subdirectories
add_subdirectory (src)
add_subdirectory (app)
add_subdirectory (unitTests)
