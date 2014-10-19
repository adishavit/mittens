
include_directories(${Boost_INCLUDE_DIR})

include_directories(../include)
include_directories(../test)


add_executable (0_simplest_example 0_simplest_example.cpp)
set_property(TARGET 0_simplest_example PROPERTY FOLDER "examples/general")

add_executable (1_custom_app_return_codes 1_custom_app_return_codes.cpp)
set_property(TARGET 1_custom_app_return_codes PROPERTY FOLDER "examples/general")

add_executable (2_custom_app_return_codes_with_messages 2_custom_app_return_codes_with_messages.cpp)
set_property(TARGET 2_custom_app_return_codes_with_messages PROPERTY FOLDER "examples/general")

add_executable (3_lippincott_blog_post_example 3_lippincott_blog_post_example.cpp)
set_property(TARGET 3_lippincott_blog_post_example PROPERTY FOLDER "examples/general")

add_executable (4_multi_catch 4_multi_catch.cpp)
set_property(TARGET 4_multi_catch PROPERTY FOLDER "examples/general")

if(MSVC)
	add_executable (1_win32_hresult_boundary_function_example 1_win32_hresult_boundary_function_example.cpp)
	set_property(TARGET 1_win32_hresult_boundary_function_example PROPERTY FOLDER "examples/windows")

	add_executable (2_mfc_exception_handling 2_mfc_exception_handling.cpp)
	set_property(TARGET 2_mfc_exception_handling PROPERTY FOLDER "examples/windows")
	
	# add_executable (3_seh_exception_handling 3_seh_exception_handling.cpp)
	# set_property(TARGET 3_seh_exception_handling PROPERTY FOLDER "examples/windows")
	
endif(MSVC)


if(MSVC)
   add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif(MSVC)

