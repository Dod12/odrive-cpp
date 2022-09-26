//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#include <iostream>
#include <cstdio>
#include <odrive.h>

int main(int argc, char* argv[]){
    auto odrive = odrive::ODrive();
    if (odrive.search_device() != odrive::STATUS_SUCCESS) { fprintf(stderr, "Cannot find ODrive"); return -1; }
    
    float vbus_voltage;
    odrive.read(odrive::endpoints::VBUS_VOLTAGE, vbus_voltage);
    std::cout << "Vbus voltage: " << vbus_voltage << std::endl;
    float left_pos = -1;
    odrive.write(odrive::endpoints::AXIS__CONTROLLER__INPUT_POS, left_pos);
    return 0;
}

struct euclidean : public std::pair<float, float> {
    euclidean(float x, float y) : std::pair<float, float>(x, y) {};
    float x = first;
    float y = second;
};

struct differential : public std::pair<float, float> {
    differential(float left, float right) : std::pair<float, float>(left, right) {};
    float left = first;
    float right = second;
};

class DifferentialDrive {
public:
    DifferentialDrive(float wheel_base, float wheel_radius) {
        this->wheel_base = wheel_base;
        this->wheel_radius = wheel_radius;
    };

public:
    float wheel_base, wheel_radius;

    differential calculate_velocity(float v, float omega) {
        return {(2 * v + omega * wheel_base ) / (2 * wheel_radius), (2 * v - omega * wheel_base ) / (2 * wheel_radius)};
    }
};
