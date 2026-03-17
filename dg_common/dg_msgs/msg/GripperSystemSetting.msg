# GripperSystemSetting message
# Struct for setting up gripper communication connections

# Setting comport for RS485 connections
string comport

# Setting the IP for TCP/IP connections
string ip

# Setting the PORT for TCP/IP connections
int32 port

# Setting the communication timeout time
int32 read_timeout

# Setting the Gripper Control Mode
int32 control_mode

# Setting the Gripper Communication Mode
int32 communication_mode

# Setting the Slave ID for a Modbus connection
int32 slave_id

# Setting the baudrate for RS485 connections
int32 baudrate