//
// Created by Daniel Carlström Schad on 2022-09-11.
//

#include <iostream>
#include <odrive.h>

int main(int argc, char* argv[]){
    auto odrive = odrive::ODrive();
    odrive.search_device();
}