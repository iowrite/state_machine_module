add_library(state_machine_module STATIC
    state_machine.c
    state_machine.h
)
target_include_directories(state_machine_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  
)
target_compile_options(state_machine_module PRIVATE
	-std=c99
	-Wall
	-Wextra
	-Wconversion
	-Wstrict-prototypes
	-Wnewline-eof
	-ffunction-sections
	-fdata-sections
	-O0
)


