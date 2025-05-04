# tests/CMakeLists.txt
add_executable(test_limits test_limits.cpp)
target_link_libraries(test_limits rtos_lib)

add_executable(test_rma test_rma.cpp)
target_link_libraries(test_rma rtos_lib)

add_executable(test_nonpreemptive test_nonpreemptive.cpp)
target_link_libraries(test_nonpreemptive rtos_lib)

add_test(NAME test_limits COMMAND test_limits)
add_test(NAME test_rma COMMAND test_rma)
add_test(NAME test_nonpreemptive COMMAND test_nonpreemptive)
