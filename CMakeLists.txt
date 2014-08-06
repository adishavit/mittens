project (Mittens)
cmake_minimum_required(VERSION 2.6)

set(TargetName mittens)

# Find and add Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(include)

file(GLOB files include/*.hpp
				test/*)

add_executable (${TargetName} ${files})
target_link_libraries(${TargetName} ${Boost_LIBRARIES})


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)
