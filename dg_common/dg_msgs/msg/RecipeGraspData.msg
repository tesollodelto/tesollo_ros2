# RecipeGraspData message
# Recipe data settings structure

# Set grasp strength
float32 grasp_force

# Locking joints in position control instead of grasp mode
int32[20] position_mode

# Setting the grasp mode
int32 grasp_mode

# Set grasp options
int32 grasp_option

# Set smooth grasp mode
int32 smooth_grasping

# Recipe grasp unique number
int32 number