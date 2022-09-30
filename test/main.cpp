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
