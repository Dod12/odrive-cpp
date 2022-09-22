//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#include <iostream>
#include <odrive.h>
#include <odrive_endpoints.hpp>

int main(int argc, char* argv[]){
    auto odrive = odrive::ODrive();
    if (odrive.search_device() != odrive::ReturnStatus::STATUS_ERROR) { fprintf(stderr, "Cannot find ODrive"); }
    
    float vbus_voltage;
    odrive.read(odrive::VBUS_VOLTAGE, vbus_voltage);
}