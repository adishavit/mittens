
include_directories(${Boost_INCLUDE_DIR})

include_directories(../include)
include_directories(../test)


add_executable (example0 example0.cpp)
set_property(TARGET example0 PROPERTY FOLDER "examples")

add_executable (example1 example1.cpp)
set_property(TARGET example1 PROPERTY FOLDER "examples")

add_executable (example2 example2.cpp)
set_property(TARGET example2 PROPERTY FOLDER "examples")

add_executable (example3 example3.cpp)
set_property(TARGET example3 PROPERTY FOLDER "examples")


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)

