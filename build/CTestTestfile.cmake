# CMake generated Testfile for 
# Source directory: Z:/CLion_C++/StudentManagementSystem
# Build directory: Z:/CLion_C++/StudentManagementSystem/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[cli_help_command]=] "python3" "-c" "import subprocess,sys; p=subprocess.run([sys.argv[1]], input='sms help\\n', text=True, capture_output=True, timeout=3); sys.stdout.write(p.stdout); sys.exit(0 if 'Commands:' in p.stdout else 1)" "Z:/CLion_C++/StudentManagementSystem/build/StudentManagementSystem.exe")
set_tests_properties([=[cli_help_command]=] PROPERTIES  PASS_REGULAR_EXPRESSION "Commands:" _BACKTRACE_TRIPLES "Z:/CLion_C++/StudentManagementSystem/CMakeLists.txt;20;add_test;Z:/CLion_C++/StudentManagementSystem/CMakeLists.txt;0;")
