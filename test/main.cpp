//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#include <iostream>
//#include <odrive.h>
#include <odrive_usb.hpp>

int main(int argc, char* argv[]){
    auto* odrive = new odrive::ODriveUSB();
    int64_t serial = 35632015560013;
    odrive->init(std::vector<std::vector<int64_t>>{{serial}, {serial}});
    int target_pos = 1;
    float vbus_voltage;
    try {
        odrive->read(serial, odrive::VBUS_VOLTAGE, vbus_voltage);
        std::cout << "Vbus voltage: " << vbus_voltage << std::endl;
    } catch (std::exception& e) {
        std::cout << "Error reading VBUS voltage" << std::endl;
        return 1;
    }
    //odrive->write(serial, odrive::AXIS__CONTROLLER__INPUT_POS, target_pos);
}