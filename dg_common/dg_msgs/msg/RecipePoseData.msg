# RecipePoseData message
# Preference Struct for Driving Grippers

# Set Target Joint Position Value
float32[20] target_joint

# Set the time it takes to reach your target location
int32[20] joint_motion_time

# Recipe Pose Unique Number
int32 number

# Set position control mode (disabled)
int32 mode