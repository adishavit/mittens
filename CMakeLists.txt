project (Mittens)
cmake_minimum_required(VERSION 2.6)


# Find and add Boost for Boost.Test
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(include)

file(GLOB include_files include/*.hpp)

add_executable (unit_tests 	${include_files}
							tests/generic_exception_handler_test.cpp
							tests/std_exception_handler_test.cpp
							tests/all_catcher_test.cpp
							tests/handler_composition_test.cpp
							tests/bad_alloc_handler_test.cpp
							tests/exception_generators.hpp
							tests/main.cpp)
						
						
target_link_libraries(unit_tests ${Boost_LIBRARIES})


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory (examples)

