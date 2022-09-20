#include <cmath>
#include <thread>
#include "odrive.h"

template <typename T>
MotorController::MotorController(const T& serial_numbers, double sampling_rate) {
    left = Motor(serial_numbers.at(0), 0, 0);
    right = Motor(serial_numbers.at(1), odrive::per_axis_offset, 0);
    usb_device = std::make_unique<odrive::ODriveUSB>();
    usb_device->init(std::vector<T>{NULL, T{left.serial_number, right.serial_number}});
    poller_thread = std::thread(&MotorController::sensor_poller, this, 1000 / sampling_rate);
    poller_thread.detach();
}

void MotorController::sensor_poller(int sleep_millis) {
    while (get_poller()) {
        for (auto motor : {left, right}) {
            float Iq_measured, pos_estim, vel_estim;

            read_usb(motor.serial_number, odrive::VBUS_VOLTAGE, motor.vbus_voltage);

            read_usb(motor.serial_number, odrive::AXIS__MOTOR__CURRENT_CONTROL__IQ_MEASURED + motor.axis_offset, Iq_measured);
            motor.effort = Iq_measured * motor.torque_constant;

            read_usb(motor.serial_number, odrive::AXIS__ENCODER__POS_ESTIMATE + motor.axis_offset, pos_estim);
            motor.position = pos_estim * 2 * M_PI;

            read_usb(motor.serial_number, odrive::AXIS__ENCODER__VEL_ESTIMATE + motor.axis_offset, vel_estim);
            motor.velocity = vel_estim * 2 * M_PI;

            read_usb(motor.serial_number, odrive::AXIS__ERROR + motor.axis_offset, motor.axis_errors);
            read_usb(motor.serial_number, odrive::AXIS__MOTOR__ERROR + motor.axis_offset, motor.motor_errors);
            read_usb(motor.serial_number, odrive::AXIS__ENCODER__ERROR + motor.axis_offset, motor.encoder_errors);
            read_usb(motor.serial_number, odrive::AXIS__CONTROLLER__ERROR + motor.axis_offset, motor.controller_errors);
            read_usb(motor.serial_number, odrive::AXIS__MOTOR__FET_THERMISTOR__TEMPERATURE + motor.axis_offset, motor.fet_temperature);
            read_usb(motor.serial_number, odrive::AXIS__MOTOR__MOTOR_THERMISTOR__TEMPERATURE + motor.axis_offset, motor.motor_temperature);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_millis));
    }
}

template <typename T>
Status MotorController::read_usb(int64_t& serial_number, int endpoint, T &value) {
    int res = usb_device->read(serial_number, endpoint, value);
    if (res != 0) {
        return Status::STATUS_ERROR;
    }else {
        return Status::STATUS_OK;
    }
}

template <typename T>
Status MotorController::write_usb(int64_t& serial_number, int16_t& endpoint, T &value) {
    int res = usb_device->write(serial_number, endpoint, value);
    if (res != 0) {
        return Status::STATUS_ERROR;
    }else {
        return Status::STATUS_OK;
    }
}

MotorController::~MotorController() {
    set_poller(false);
    this->usb_device.release();
}
