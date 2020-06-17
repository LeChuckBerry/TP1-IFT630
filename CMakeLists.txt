cmake_minimum_required(VERSION 3.16)
project(TP1_IFT630)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(philo_sem philosophers.cpp)
add_executable(barber_sem barber_sem.cpp)
add_executable(roller_coaster_sem roller_coaster_sem.cpp)
add_executable(philo_monitor philo_monitor.cpp)
add_executable(roller_coaster_mon roller_coaster_monitor.cpp)




target_link_libraries(philo_sem Threads::Threads)
target_link_libraries(barber_sem Threads::Threads)
target_link_libraries(roller_coaster_sem Threads::Threads)
target_link_libraries(philo_monitor Threads::Threads)
target_link_libraries(roller_coaster_mon Threads::Threads)