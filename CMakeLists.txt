# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(RTOS_Model)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная библиотека RTOS
add_library(rtos_lib
    src/task.cpp
    src/scheduler.cpp
    src/semaphore.cpp
    src/event.cpp
    src/system_log.cpp
)

target_include_directories(rtos_lib PUBLIC include)

# Тесты
add_executable(rtos_tests
    tests/main_test.cpp
    tests/test_limits.cpp
    tests/test_rma.cpp
    tests/test_nonpreemptive.cpp
    tests/test_pip.cpp
    tests/test_events.cpp
)

target_link_libraries(rtos_tests rtos_lib)

# Добавление опции для потоков
find_package(Threads REQUIRED)
target_link_libraries(rtos_lib ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rtos_tests ${CMAKE_THREAD_LIBS_INIT})

# Включение тестирования
enable_testing()
add_test(NAME rtos_tests COMMAND rtos_tests)
