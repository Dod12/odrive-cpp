#include "odrive.h"

using namespace test;

ODrive::ODrive(const int serial_number, odrive::ODriveUSB* driver_ptr) {
    this->serial_number = serial_number;
    this->driver_ptr = driver_ptr;
}