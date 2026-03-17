# RecipeGainData message
# Recipe Gain Data Settings Structure

# Setting the P-gain
float32[20] gain_p

# Setting the D-gain
float32[20] gain_d

# Setting up I-gain
float32[20] gain_i

# Setting the Error Integral Maximum
float32[20] i_limit

# Setting the PID Control Mode
int32 control_pid_mode

# Recipe gain unique number
int32 number

# Set position control mode (disabled)
int32 mode