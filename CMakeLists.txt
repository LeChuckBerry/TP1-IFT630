cmake_minimum_required(VERSION 3.15)
project(TP1_IFT630)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(philo_sem philosopher/philo_sem.cpp)
add_executable(barber_sem barber/barber_sem.cpp)
add_executable(roller_coaster_sem roller_coaster/roller_coaster_sem.cpp)
add_executable(philo_monitor philosopher/philo_monitor.cpp)
add_executable(roller_coaster_mon roller_coaster/roller_coaster_monitor.cpp)
add_executable(roller_coaster_channel roller_coaster/roller_coaster_messages.cpp)
add_executable(test mailboxMain.cpp)



target_link_libraries(philo_sem Threads::Threads)
target_link_libraries(barber_sem Threads::Threads)
target_link_libraries(roller_coaster_sem Threads::Threads)
target_link_libraries(philo_monitor Threads::Threads)
target_link_libraries(roller_coaster_mon Threads::Threads)
target_link_libraries(roller_coaster_channel Threads::Threads)
target_link_libraries(test Threads::Threads)