#ifndef TP1_IFT630_UTILS_H
#define TP1_IFT630_UTILS_H

#pragma once
#include <cstdint>
#include <limits>
#include <random>
#include "sync_primitives/monitor.h"
#include <string>
#include <iostream>

// Crédit Karim Elmougi-Côté, modifié par Charles Coupal-Jetté pour portabilité

namespace cppUtils {
    template <typename T>
    T random(const T lower_bound, const T upper_bound) noexcept {
        static_assert(std::is_integral<T>::value);
        static_assert(!std::is_same<T, bool>::value);
        auto dist = [lower_bound, upper_bound]() {
            return std::uniform_int_distribution<T>{lower_bound, upper_bound};
        }();
        static std::random_device r{};
        auto engine = std::default_random_engine{r()};
        return dist(engine);
    }
    template <typename T>
    T random() noexcept {
        return random<T>(std::numeric_limits<T>::min(),
                         std::numeric_limits<T>::max());
    }
    template <>
    bool random<bool>() noexcept {
        return static_cast<bool>(random<uint16_t>(0, 1));
    }

    class Printer_Sem{
        Semaphore sem;


        public:
            Printer_Sem() : sem(Semaphore(1)){}
            void printLine(std::string text){
                sem.P();
                std::cout << text << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                sem.V();
            }
    };

    class Printer_Mon{
        Semaphore_Monitor sem;


    public:
        Printer_Mon() : sem(Semaphore_Monitor()){}

        void printLine(std::string text){
            sem.P();
            std::cout << text << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            sem.V();
        }
    };


} // namespace cppUtils





#endif //TP1_IFT630_UTILS_H
