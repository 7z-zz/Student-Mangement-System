cmake_minimum_required(VERSION 3.20)
project(StudentManagementSystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_executable(StudentManagementSystem
        src/obj/main.cpp
        src/Head/Student.h
        src/obj/Student.cpp
        src/Head/Print_util.h
        src/obj/CmdHandler.cpp
        src/Head/CmdHandler.h
        src/obj/StudentStore.cpp
        src/Head/StudentStore.h)

if(BUILD_TESTING)
    add_test(
            NAME cli_help_command
            COMMAND python3 -c "import subprocess,sys; p=subprocess.run([sys.argv[1]], input='sms help\\n', text=True, capture_output=True, timeout=3); sys.stdout.write(p.stdout); sys.exit(0 if 'Commands:' in p.stdout else 1)" $<TARGET_FILE:StudentManagementSystem>)
    set_tests_properties(cli_help_command PROPERTIES
            PASS_REGULAR_EXPRESSION "Commands:")
endif()
