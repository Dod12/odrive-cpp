#pragma once
#include <odrive_usb.hpp>

enum ReturnStatus {
    STATUS_OK = 0,
    STATUS_ERROR = 1
};

enum ControlMode {
    UNDEFINED = -1,
    POSITION = 0,
    VELOCITY = 1,
    EFFORT = 2
};

namespace test {
    
    struct ODriveSensors {
        float position, velocity, effort;
        float vbus_voltage, fet_temperature, motor_temperature;
        ControlMode control_mode = ControlMode::UNDEFINED;
        float target_position, target_velocity, target_effort;
        int serial_number, axis_offset;
        float torque_constant;
    };
    
    class Axis {
    public:
        explicit Axis() = default;
        Axis(const int serial_number, const int axis_offset);
        ~Axis() = default;

        template <typename T>
        ReturnStatus configure(const int endpoint, const T value);

        template <typename T>
        ReturnStatus set_value(const int endpoint, const T value);

        ODriveSensors get_sensor_readings();
    
    private:
        int serial_number;
        int axis_offset;

    };

    class ODrive {
    public:
        explicit ODrive() = default;
        explicit ODrive(int serial_number, odrive::ODriveUSB* driver_ptr);

        Axis left, right;
        odrive::ODriveUSB* driver_ptr;
        int serial_number;
    };
}