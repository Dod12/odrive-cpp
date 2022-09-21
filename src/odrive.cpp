#include "odrive.h"

using namespace test;

ODrive::ODrive(const int serial_number) {
    this->serial_number = serial_number;

    driver = odrive::ODriveUSB();
    driver.init(std::vector<std::vector<int64_t>>{NULL, std::vector<int64_t>{serial_number}});

    this->left = Axis(serial_number, 0, driver);
    this->right = Axis(serial_number, odrive::per_axis_offset, driver);
}

Axis::Axis(const int serial_number, const int axis_offset, odrive::ODriveUSB& driver) {
    
}