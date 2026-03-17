# ReceivedGripperData message
# Struct for checking the data provided by the gripper

# Position information for gripper joints (Unit: degree)
float32[20] joint

# Current Information for Gripper Joints (Unit: mA)
int32[20] current

# Velocity Information for Gripper Joints (Unit: rpm) (DG-3F-B models are not supported)
int32[20] velocity

# Temperature information for gripper joints (Unit: °C) (DG-3F-B models are not supported)
float32[20] temperature

# Coordinates of each fingertip endpoint (Unit: Position = mm, Rotation = degree) (x, y, z, rx, ry, rz)
float32[30] tcp  # 6 * MAX_FINGER_COUNT(5)

# Movement state information for all joints of the gripper
int32 moving

# Target Position Arrival Status Information for All Gripper Joints
int32 target_arrived

# Status information when using the Blend Joint feature
int32 blend_move_state

# Current blend index
int32 current_blend_index

# Gripper ID
int32 product_id

# Gripper firmware version
int32 firmware_version