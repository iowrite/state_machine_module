



add_executable(main 
    main.c
    main.h
    transitions.c
    transitions.h
    run/boot.c
    run/charge.c
    run/discharge.c
    run/precharge.c
    run/run.c
    run/standby.c
    fault/fault.c
    protect/protect.c
    protect/charge_protect.c
    protect/discharge_protect.c
    sleep/sleep.c
)



find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)


target_link_libraries(main state_machine_module ${GTK4_LIBRARIES})


target_include_directories(main PRIVATE ${GTK4_INCLUDE_DIRS})

target_compile_definitions(main PRIVATE ${GTK4_CFLAGS_OTHER})