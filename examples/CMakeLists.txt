

include_directories(../include)
include_directories(../test)


add_executable (example1 example1.cpp)
add_executable (example2 example2.cpp)

set_property(TARGET example1 PROPERTY FOLDER "examples")
set_property(TARGET example2 PROPERTY FOLDER "examples")


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)

