import time

import odrive
import odrive.enums as enums

device = odrive.find_any()
axes = [device.axis0, device.axis1]

if device is not None:
    print("Found ODrive", device.serial_number)
else:
    raise IOError("Could not find ODrive")

control_mode = "bandwidth"  # Control mode for filtering target position. Must be one of [None, "bandwidth", "trap"].

# Setup global settings
device.config.enable_brake_resistor = True
device.config.brake_resistance = 2
device.config.dc_max_negative_current = 0.001

# Setup per-axis settings
for axis in axes:
    axis.motor.config.current_lim = 10
    axis.controller.config.vel_limit = 10
    axis.motor.config.calibration_current = 0.1
    axis.motor.config.pole_pairs = 7
    axis.motor.config.torque_constant = 8.27 / 150
    axis.motor.config.motor_type = enums.MOTOR_TYPE_HIGH_CURRENT
    axis.encoder.config.cpr = 8192
    if control_mode == "trap":
        axis.trap_traj.config.vel_limit = 5
        axis.trap_traj.config.accel_limit = 1
        axis.trap_traj.config.decel_limit = 1
        axis.controller.config.inertia = 0
        axis.controller.config.input_mode = enums.INPUT_MODE_TRAP_TRAJ
    elif control_mode == "bandwidth":
        axis.controller.config.input_filter_bandwidth = 40 / 2  # Set to half the position update frequency.
        axis.controller.input_mode = enums.INPUT_MODE_POS_FILTER
    axis.config.startup_motor_calibration = True
    axis.config.startup_encoder_index_search = True
    axis.config.startup_encoder_offset_calibration = True
    axis.config.startup_closed_loop_control = True

for axis in axes:
    axis.requested_state = enums.AXIS_STATE_IDLE

device.save_configuration()

for axis in axes:
    axis.requested_state = enums.AXIS_STATE_FULL_CALIBRATION_SEQUENCE
    time.sleep(3)

for axis in axes:
    axis.requested_state = enums.AXIS_STATE_CLOSED_LOOP_CONTROL
