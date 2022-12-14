
#define FIBRE_ENABLE_SERVER 0
#define FIBRE_ENABLE_CLIENT 1

#if defined(__linux__)
// event loop currently only implemented on Linux
#define FIBRE_ENABLE_EVENT_LOOP 1
#endif

#define FIBRE_ALLOW_HEAP 1
#define FIBRE_ENABLE_TEXT_LOGGING 1

#if defined(__EMSCRIPTEN__)
#define FIBRE_ENABLE_WEBUSB_BACKEND 1
#else
#define FIBRE_ENABLE_LIBUSB_BACKEND 1
#endif

#if defined(__linux__)
#define FIBRE_ENABLE_TCP_CLIENT_BACKEND 1
#define FIBRE_ENABLE_TCP_SERVER_BACKEND 1
#define FIBRE_ENABLE_SOCKET_CAN_BACKEND 1
#endif

#if FIBRE_ENABLE_SOCKET_CAN_BACKEND
#define FIBRE_ENABLE_CAN_ADAPTER 1
#endif
