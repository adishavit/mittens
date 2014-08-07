
include_directories(${Boost_INCLUDE_DIR})

include_directories(../include)
include_directories(../test)


add_executable (0_simplest_example 0_simplest_example.cpp)
set_property(TARGET 0_simplest_example PROPERTY FOLDER "examples")

add_executable (1_custom_app_return_codes 1_custom_app_return_codes.cpp)
set_property(TARGET 1_custom_app_return_codes PROPERTY FOLDER "examples")

add_executable (2_custom_app_return_codes_with_messages 2_custom_app_return_codes_with_messages.cpp)
set_property(TARGET 2_custom_app_return_codes_with_messages PROPERTY FOLDER "examples")

add_executable (3_win32_hresult_boundary_function_example 3_win32_hresult_boundary_function_example.cpp)
set_property(TARGET 3_win32_hresult_boundary_function_example PROPERTY FOLDER "examples")

add_executable (4_lippincott_blog_post_example 4_lippincott_blog_post_example.cpp)
set_property(TARGET 4_lippincott_blog_post_example PROPERTY FOLDER "examples")


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)

