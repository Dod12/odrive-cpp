import time

import odrive
import odrive.enums as enums
#from odrive import fibre

device = odrive.find_any()

if device is None:
    raise IOError("Could not find ODrive")

print("Found ODrive", device.serial_number)
axes = [device.axis0, device.axis1]

control_mode = "none"  # Control mode for filtering target position. Must be one of [None, "bandwidth", "trap"].
tick_rate = 2

# Setup global settings
device.config.enable_brake_resistor = True
device.config.brake_resistance = 2.7
device.config.dc_max_negative_current = -0.001

# Setup per-axis settings
for axis in axes:
    axis.motor.config.current_lim = 10
    axis.controller.config.vel_limit = 15
    #axis.controller.config.vel_gain = 1.493466854095459
    #axis.controller.config.pos_gain = 132.88357543945312
    axis.motor.config.calibration_current = 10
    axis.motor.config.pole_pairs = 7
    axis.motor.config.torque_constant = 8.27 / 150
    axis.motor.config.motor_type = enums.MOTOR_TYPE_HIGH_CURRENT
    axis.encoder.config.cpr = 8192
    # axis.encoder.config.direction = 1 # Invert control direction for left encoder position.
    if control_mode == "trap":
        axis.trap_traj.config.vel_limit = 5
        axis.trap_traj.config.accel_limit = 1
        axis.trap_traj.config.decel_limit = 1
        axis.controller.config.inertia = 0
        axis.controller.config.input_mode = enums.INPUT_MODE_TRAP_TRAJ
    elif control_mode == "bandwidth":
        axis.controller.config.input_filter_bandwidth = tick_rate / 2  # Set to half the position update frequency.
        axis.controller.config.input_mode = enums.INPUT_MODE_POS_FILTER
    else:
        axis.controller.config.input_mode = enums.INPUT_MODE_PASSTHROUGH

    # Startup calibration
    axis.motor.config.pre_calibrated = False
    axis.encoder.config.pre_calibrated = False

    axis.config.startup_encoder_index_search = True
    axis.config.startup_closed_loop_control = True
    axis.config.startup_encoder_offset_calibration = True
    axis.config.startup_motor_calibration = True

for axis in axes:
    axis.requested_state = enums.AXIS_STATE_IDLE

try:
    device.save_configuration()
except:
    print("Rebooting...")
    pass
