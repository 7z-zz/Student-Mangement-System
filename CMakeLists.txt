cmake_minimum_required(VERSION 3.20)
project(StudentManagementSystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

set(POSTGRESQL_ROOT "Z:/Postgresql18" CACHE PATH "PostgreSQL installation root")
unset(POSTGRESQL_INCLUDE_DIR CACHE)
unset(POSTGRESQL_LIBRARY CACHE)
find_path(POSTGRESQL_INCLUDE_DIR
        NAMES libpq-fe.h
        HINTS "${POSTGRESQL_ROOT}/include")

if(WIN32)
    if(EXISTS "${POSTGRESQL_ROOT}/lib/libpq.lib")
        set(POSTGRESQL_LIBRARY "${POSTGRESQL_ROOT}/lib/libpq.lib" CACHE FILEPATH "PostgreSQL client library" FORCE)
    else()
        find_file(POSTGRESQL_LIBRARY
                NAMES libpq.lib
                HINTS "${POSTGRESQL_ROOT}/lib")
    endif()
else()
    find_library(POSTGRESQL_LIBRARY
            NAMES pq libpq
            HINTS "${POSTGRESQL_ROOT}/lib")
endif()

if(NOT POSTGRESQL_INCLUDE_DIR OR NOT POSTGRESQL_LIBRARY)
    message(FATAL_ERROR "PostgreSQL libpq not found. Set POSTGRESQL_ROOT to your PostgreSQL install path.")
endif()

add_executable(StudentManagementSystem
        src/obj/main.cpp
        src/Head/Student.h
        src/obj/Student.cpp
        src/Head/Print_util.h
        src/obj/CmdHandler.cpp
        src/Head/CmdHandler.h
        src/Head/DatabaseOps.h
        src/obj/DatabaseOps.cpp
        src/obj/StudentStore.cpp
        src/Head/StudentStore.h)

target_include_directories(StudentManagementSystem PRIVATE ${POSTGRESQL_INCLUDE_DIR})
target_link_libraries(StudentManagementSystem PRIVATE ${POSTGRESQL_LIBRARY})

if(WIN32)
    find_file(POSTGRESQL_DLL
            NAMES libpq.dll
            HINTS "${POSTGRESQL_ROOT}/bin")
    if(POSTGRESQL_DLL)
        add_custom_command(TARGET StudentManagementSystem POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${POSTGRESQL_DLL}" "$<TARGET_FILE_DIR:StudentManagementSystem>")
    endif()
endif()

if(BUILD_TESTING)
    add_test(
            NAME cli_help_command
            COMMAND python3 -c "import subprocess,sys; p=subprocess.run([sys.argv[1]], input='sms help\\n', text=True, capture_output=True, timeout=3); sys.stdout.write(p.stdout); sys.exit(0 if 'Commands:' in p.stdout else 1)" $<TARGET_FILE:StudentManagementSystem>)
    set_tests_properties(cli_help_command PROPERTIES
            PASS_REGULAR_EXPRESSION "Commands:")
endif()
