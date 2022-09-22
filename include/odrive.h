#pragma once

#include <libusb.h>
#include <stdio.h>
#include <vector>
#include <map>

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

        ReturnStatus search_device(const int vendor_id = 0x1209, const int product_id = 0x0D32);

        ReturnStatus configure_device(const ODriveConfig& config);

        template <typename T>
        ReturnStatus read(short endpoint, T& value);
        template <typename T>
        ReturnStatus write(short endpoint, const T& value);
        ReturnStatus call(short endpoint);

    private:
        libusb_context* context = NULL;
        struct libusb_device_handle* handle = NULL;
        libusb_device* device = NULL;

        bytes request_payload, response_payload;

        ReturnStatus call_usb(short endpoint, short response_size, bool MSB);

        bytes encode_packet();
        bytes decode_packet();
    };
}