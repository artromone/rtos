# tests/CMakeLists.txt
add_executable(test_limits test_limits.cpp)
target_link_libraries(test_limits rtos_lib)

add_executable(test_rma test_rma.cpp)
target_link_libraries(test_rma rtos_lib)

add_executable(test_nonpreemptive test_nonpreemptive.cpp)
target_link_libraries(test_nonpreemptive rtos_lib)

add_executable(test_semaphores test_semaphores.cpp)
target_link_libraries(test_semaphores rtos_lib)

add_executable(test_events test_events.cpp)
target_link_libraries(test_events rtos_lib)

add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration rtos_lib)

add_test(NAME test_semaphores COMMAND test_semaphores)
add_test(NAME test_events COMMAND test_events)
add_test(NAME test_integration COMMAND test_integration)
add_test(NAME test_limits COMMAND test_limits)
add_test(NAME test_rma COMMAND test_rma)
add_test(NAME test_nonpreemptive COMMAND test_nonpreemptive)
