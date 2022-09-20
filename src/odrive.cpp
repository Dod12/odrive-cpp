#include "odrive.h"

using namespace test;

ODrive::ODrive(const int serial_number, const int axis_offset, ODrive::ODriveUSB* driver_ptr) {
    this->serial_number = serial_number;
    this->axis_offset = axis_offset;
    this->driver_ptr = driver_ptr;
}