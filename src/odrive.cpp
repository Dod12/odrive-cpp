#include "odrive.h"

using namespace odrive;

ODrive::ODrive() {
}

ODrive::ODrive(libusb_context *context) {
    this->context = context;
}

ODrive::~ODrive() {
    if (handle) {
        libusb_release_interface(handle, 2);
        libusb_close(handle);
    };

    if (context) {
        libusb_exit(NULL);
    }
}

ReturnStatus ODrive::search_device(const int vendor_id, const int product_id) {

    if (libusb_init(&context) != LIBUSB_SUCCESS) {
        fprintf(stdout, "Cannot initialize libusb\n");
    }

    printf("Opening device %04X:%04X...\n", vendor_id, product_id);
    handle = libusb_open_device_with_vid_pid(context, vendor_id, product_id);

    if (handle == NULL) {
        fprintf(stderr, "Unable to open device %04X:%04X\n", vendor_id, product_id);
        libusb_exit(context);
        return ReturnStatus::STATUS_ERROR;
    }

    if (libusb_kernel_driver_active(handle, 2) != LIBUSB_SUCCESS  &&
        libusb_detach_kernel_driver(handle, 2) != LIBUSB_SUCCESS) {
        fprintf(stderr, "Cannot aquire driver\n");
        libusb_close(handle);
        libusb_exit(context);
        return ReturnStatus::STATUS_ERROR;
    }

    if (libusb_claim_interface(handle, 2) != LIBUSB_SUCCESS) {
        fprintf(stderr, "Cannot claim interface\n");
        libusb_close(handle);
        libusb_exit(context);
        return ReturnStatus::STATUS_ERROR;
    }

    device = libusb_get_device(handle);

    if (device == NULL) {
        fprintf(stderr, "Unable to get device\n");
        libusb_release_interface(handle, 2);
        libusb_close(handle);
        libusb_exit(context);
        return ReturnStatus::STATUS_ERROR;
    }

    libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    int serial_number = descriptor.iSerialNumber;
    printf("Serial number: %d\n", serial_number);
    return ReturnStatus::STATUS_SUCCESS;
}

template <typename T>
ReturnStatus ODrive::write(short endpoint, const T& value) {

    request_payload.clear();
    response_payload.clear();

    for (size_t i = 0; i < sizeof(value); ++i) {
        request_payload.push_back(((unsigned char*)&value)[i]);
    }

    return ReturnStatus::STATUS_SUCCESS;
}