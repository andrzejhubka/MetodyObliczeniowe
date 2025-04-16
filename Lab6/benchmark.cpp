#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>

class Timer {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start_time;
    bool show_microseconds;

public:
    // Konstruktor automatycznie rozpoczyna pomiar czasu
    Timer(const std::string& timer_name = "Timer", bool use_microseconds = false)
        : name(timer_name), show_microseconds(use_microseconds) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    // Destruktor automatycznie wyświetla czas wykonania
    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();

        if (show_microseconds) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            std::cout << name << ": " << std::fixed << std::setprecision(3)
                      << duration << " mikrosekund" << std::endl;
        } else {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            std::cout << name << ": " << std::fixed << std::setprecision(3)
                      << duration << " milisekund" << std::endl;
        }
    }

    // Metoda do ręcznego zatrzymania timera i wyświetlenia wyniku
    void stop() {
        auto end_time = std::chrono::high_resolution_clock::now();

        if (show_microseconds) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            std::cout << name << ": " << std::fixed << std::setprecision(3)
                      << duration << " mikrosekund" << std::endl;
        } else {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            std::cout << name << ": " << std::fixed << std::setprecision(3)
                      << duration << " milisekund" << std::endl;
        }
    }

    // Metoda do resetowania timera
    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }
};
