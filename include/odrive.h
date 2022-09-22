#pragma once

#include <libusb.h>
#include <stdio.h>
#include <vector>
#include <map>

#include "odrive_endpoints.hpp"

#define ODRIVE_USB_VENDORID 0x1209
#define ODRIVE_USB_PRODUCTID 0x0d32

#define ODRIVE_OUT_ENDPOINT 0x03
#define ODRIVE_IN_ENDPOINT 0x83

#define ODRIVE_PROTOCOL_VERSION 1
#define ODRIVE_MAX_PACKET_SIZE 16

namespace odrive {

    typedef std::vector<uint8_t> bytes;
    
    enum ReturnStatus {
        STATUS_SUCCESS = 0,
        STATUS_ERROR = -1,
    };

    enum Config {

    };

    struct ODriveConfig : std::map<Config, int> {

    };

    class ODrive {
    public:
        ODrive();
        explicit ODrive(libusb_context *context);
        ~ODrive();

        ReturnStatus search_device(int vendor_id = 0x1209, int product_id = 0x0D32);

        ReturnStatus configure_device(const ODriveConfig& config);

        template <typename T>
        ReturnStatus read(short endpoint, T& value);
        template <typename T>
        ReturnStatus write(short endpoint, const T& value);
        ReturnStatus call(short endpoint);

    private:
        libusb_context* context = nullptr;
        struct libusb_device_handle* handle = nullptr;
        libusb_device* device = nullptr;

        short sequence_number = 0;

        static bytes encode(short sequence_number, short endpoint, short response_size, const bytes& request_payload);
        static bytes decode(const bytes& response_packet);

        ReturnStatus transaction(short endpoint, short response_size, const bytes& request_payload, bytes& response_payload, bool MSB);
    };
}