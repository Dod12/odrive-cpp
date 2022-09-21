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

enum InputMode {
    
};

enum MotorType {
    MOTOR_TYPE_HIGH_CURRENT = 0,
    MOTOR_TYPE_GIMBAL = 2
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
        Axis(const int serial_number, const int axis_offset, odrive::ODriveUSB& driver);
        ~Axis() = default;

        template <typename T>
        ReturnStatus configure(const int endpoint, const T value);

        template <typename T>
        ReturnStatus set_value(const int endpoint, const T value);

        ODriveSensors get_sensor_readings();
    
    private:
        int serial_number;
        int axis_offset;
        odrive::ODriveUSB& driver;

    };

    class ODrive {
    public:
        explicit ODrive(int serial_number);

        Axis left, right;
        odrive::ODriveUSB driver;
        int serial_number;

        ReturnStatus set_current_limit(float value);
        ReturnStatus set_velocity_limit(float value);
        ReturnStatus set_has_brake_resistor(bool value);
        ReturnStatus set_brake_resistance(float value);
        ReturnStatus set_max_negative_current(float value);
        ReturnStatus set_n_pole_pairs(float value);
        ReturnStatus set_torque_constant(float value);
        ReturnStatus set_motor_type(MotorType value);
        ReturnStatus set_encoder_cpr(int value);
        ReturnStatus calibrate();
        ReturnStatus start_closed_loop_control();
        ReturnStatus set_input_mode(InputMode vaue);
        ReturnStatus set_control_mode(ControlMode value);
        static const int a = odrive::AXIS__CONTROLLER__CONFIG__CONTROL_MODE;
    };
}