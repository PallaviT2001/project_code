cmake_minimum_required(VERSION 3.16)

project(project_code LANGUAGES C)

add_executable(project_code main.c
    main1.h
    admin.h
    student.h
    faculty.h
    fees.h
    section.h
    fileoperation.h
    student.c
    faculty.c
    fees.c
    section.c
    main1.c
    fileoperation.c
    admin.c)

include(GNUInstallDirs)
install(TARGETS project_code
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
