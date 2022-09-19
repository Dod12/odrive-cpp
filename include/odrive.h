//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#ifndef ODRIVE_CPP_ODRIVE_H
#define ODRIVE_CPP_ODRIVE_H

#include <cstdint>
#include <vector>
#include <odrive_usb.hpp>
#include <odrive_endpoints.hpp>

enum Status {
    STATUS_OK = 0,
    STATUS_ERROR = 1
};

struct Motor{
    double vbus_voltage = 0, position = 0, velocity = 0, effort = 0, fet_temperature = 0, motor_temperature = 0;
    int axis_errors = 0, motor_errors = 0, encoder_errors = 0, controller_errors = 0;
    int64_t serial_number;
    int16_t axis_offset;
    float torque_constant;

    Motor(int64_t serial_number, int16_t axis_offset, float torque_constant) {this->serial_number = serial_number; this->axis_offset = axis_offset; this->torque_constant = torque_constant;};
};

class MotorController {
public:
    //explicit MotorController() = default;
    template <typename T>
    explicit MotorController(const T& serial_numbers, double sampling_rate = 40);
    template <typename T, typename ... Ts>
    explicit MotorController(Ts ...serial_number, double sampling_rate) : MotorController(std::vector<T>{serial_number...}, sampling_rate) {};
    ~MotorController();

protected:
    [[noreturn]] void sensor_poller(int sleep_millis);

private:
    template <typename T>
    Status read_usb(int64_t& serial_number, int endpoint, T& value);
    template <typename T>
    Status write_usb(int64_t& serial_number, int16_t& endpoint, T& value);

    std::unique_ptr<odrive::ODriveUSB> usb_device;

    bool run_poller = true;
    std::mutex poller_mutex;
    std::thread poller_thread;

    bool get_poller() { poller_mutex.lock(); bool result = run_poller; poller_mutex.unlock(); return result; }
    void set_poller(bool value) { poller_mutex.lock(); run_poller = value; poller_mutex.unlock(); }

    Motor left, right;
};

#endif //ODRIVE_CPP_ODRIVE_H
