# GripperSetting message
# Preference Struct for Driving Grippers

# Setting the Position Offset for Gripper Joints
float32[20] joint_offset

# Setting the angle to check the reach of a gripper joint
float32[20] joint_inpose

# Setting the distance to determine the reach of a fingertip location
float32[5] tcp_inpose

# Setting the angle for fingertip reach checks
float32[5] orientation_inpose

# Set the type of data you want to receive
int32[6] received_data_type

# Setting angles to determine movement of gripper joints
float32 moving_inpose

# Number of joints in the gripper model currently in use
int32 joint_count

# The number of fingers of the gripper model currently in use
int32 finger_count

# Set the gripper model you are currently using
int32 model

# Incoming data size for the current model
int8 duty_byte_length