//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#include <iostream>
#include <odrive.h>
#include <odrive_usb.hpp>

int main(int argc, char* argv[]){
    auto* odrive = new odrive::ODriveUSB();
    odrive->init();
}