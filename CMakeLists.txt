cmake_minimum_required(VERSION 3.0)
project("c-utils")

# common objects
set(INCLUDE_DIRS "include")

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

if (WIN32)
	# getting rid of deprecation warnings on windows
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# creating targets for all test sources
file(GLOB TEST_SOURCES "src/test/*.c")
foreach(TEST ${TEST_SOURCES})
	get_filename_component(TEST_TARGET_BASENAME ${TEST} NAME_WE)
	set(TEST_TARGET_NAME test_${TEST_TARGET_BASENAME})
	add_executable(${TEST_TARGET_NAME} ${TEST})
	list(APPEND TARGETS ${TEST_TARGET_NAME})
endforeach()

# adding common properties for all targets
foreach(TARGET ${TARGETS})
	set_target_properties(${TARGET} PROPERTIES C_STANDARD 99)
	target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})
endforeach()
