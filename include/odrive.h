//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#ifndef ODRIVE_CPP_ODRIVE_H
#define ODRIVE_CPP_ODRIVE_H

#include <cstdint>
#include <vector>
#include <odrive_usb.hpp>
#include <odrive_endpoints.hpp>

#define RETURN_STATUS(status) {int ret = status; if (ret != 0) std::cout << "Motor Error" << std::endl;}

enum Status {
    STATUS_OK = 0,
    STATUS_ERROR = 1
};

struct Motor{
    double vbus_voltage, position, velocity, effort, fet_temperature, motor_temperature = 0;
    int axis_errors, motor_errrors, encoder_errors, controller_errors = 0;
    int64_t serial_number;
    int16_t axis_offset;
    float torque_constant;

    Motor(int64_t serial_number, int16_t axis_offset, float torque_constant) {};
};

class MotorController {
public:
    //explicit MotorController() = default;
    template <typename T>
    explicit MotorController(const T& serial_numbers, double sampling_rate = 40);
    template <typename T, typename ... Ts>
    explicit MotorController(Ts ...serial_number, double sampling_rate) : MotorController(std::vector<T>{serial_number...}, sampling_rate) {};
    ~MotorController() = default;

protected:
    void sensor_poller(int sleep_millis);

private:
    template <typename T>
    Status read_usb(int64_t& serial_number, int endpoint, T& value);
    template <typename T>
    Status write_usb(int64_t& serial_number, int16_t& endpoint, T& value);

    std::shared_ptr<odrive::ODriveUSB> usb_device;
    bool run_poller = true;

    Motor left, right;
};

#endif //ODRIVE_CPP_ODRIVE_H
