#ifndef DGDATATYPES_H
#define DGDATATYPES_H

#include <stdint.h>

/********************************************************************************************
****    Defines
*********************************************************************************************/
/// It is more intuitive and easier to use than simply using 
/// in arrays or loops, 
/// and it helps prevent the use of incorrect numbers, making it safer to use. 
/// It also makes it easier to modify the numbers

/// @brief Maximum number of joints.
#define MAX_JOINT_COUNT 20

	/// @brief Maximum count of finger
#define MAX_FINGER_COUNT 5

	/// @brief Maximum joint of finger
#define MAX_FINGER_JOINT_COUNT 4

	// @brief Number of Cathexian coordinate system data (x, y, z, rx, ry, rz)
#define CARTESIAN_COORDINATE_POSE_COUNT 6

	/// @brief Maximum communication data size
#define MAX_RECEIVED_DATA_SIZE 1024

	/// @brief Gripper IP Size
#define MAX_GRIPPER_IP_ADDRESS_SIZE 32

	/// @brief Number of gripper IP address zones
#define MAX_GRIPPER_IP_BYTE_LENGTH 4

	/// @brief Comport Name Size
#define MAX_COMPORT_NAME_SIZE 32

	/// @brief Number of Blend Motion Saved
#define MAX_BLEND_COUNT 10

	///  @brief Number of recipes that store a single blend action
#define MAX_BLEND_ADD_POSE_COUNT 50

	/// @brief Number of recipe poses saved
#define MAX_RECIPE_POSE_COUNT 100

	/// @brief Number of recipe gain stores
#define MAX_RECIPE_GAIN_COUNT 20

	/// @brief Number of recipe grasps saved
#define MAX_RECIPE_GRASP_COUNT 20

	/// @brief Number of grasp options
#define MAX_GRASP_OPTION_COUNT 2

	/// @brief Number of GPIOs (3 outputs, 1 input)
#define MAX_GRIPPER_GPIO_SIZE 4

	/// @brief Number of data types you can receive
#define MAX_RECEIVED_DATA_TYPE_COUNT 6 

	/// @ brief Definition of PI and conversion between Radian and Degree
#define PI 3.1415927f
#define DEGREE_TO_RADIAN PI/180.0f
#define RADIAN_TO_DEGREE 180.0f/PI

	/********************************************************************************************
	****    Enums
	*********************************************************************************************/
	/// @brief Error return codes used in the SDK
typedef enum DG_RESULT
{
	DG_RESULT_NONE = 0,
	DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED = 1,

	DG_RESULT_VALUE_IS_NEGATIVE = 100,
	DG_RESULT_OVERFLOW_JOINT_COUNT = 101,
	DG_RESULT_OVERFLOW_FINGER_COUNT = 102,
	DG_RESULT_OVERFLOW_TCP_COUNT = 103,
	DG_RESULT_OVERFLOW_ADD_BLEND_SIZE = 105,
	DG_RESULT_OVERFLOW_RECIPE_BLEND_SIZE,
	DG_RESULT_BLEND_SIZE_ZERO,
	DG_RESULT_NOT_SUPPORTED_MODEL,
	DG_RESULT_DATA_IS_ZERO,
	DG_RESULT_DATA_IS_NOT_BOOLEAN,
	DG_RESULT_NOT_FOUND_MODEL,
	DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT,
	DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT,
	DG_RESULT_OVERFLOW_RECIPE_GRASP_COUNT,
	DG_RESULT_INVALID_CONTROL_MODE,
	DG_RESULT_INVALID_GRASP_MODE_DATA,
	DG_RESULT_OVERFLOW_GRASP_OPTION_DATA,
	DG_RESULT_OVERFLOW_MAX_BYTE_DATA,
	DG_RESULT_OVERFLOW_CURRENT_LIMIT,
	DG_RESULT_IS_NOT_TORQUE_CONTROL_MODE,
	DG_RESULT_NOT_SUPPORTED_DATA_TYPE,
	DG_RESULT_BACKUP_START,
	DG_RESULT_INVALID_PASSWORD,
	DG_RESULT_OVERFLOW_GPIO_COUNT,
	DG_RESULT_OVERFLOW_GRASP_FORCE,
	DG_RESULT_OVERFLOW_BLEND_WAIT_TIME,
	DG_RESULT_NOT_FOUND_RESTORE_DATA,
	DG_RESULT_RESTORE_START,

	DG_RESULT_NOT_ARRIVED = 200,
	DG_RESULT_NOT_START_BLEND_MOVE,
	DG_RESULT_ALREADY_BLEND_MOVE_STATE,
	DG_RESULT_ALREADY_TCP_MOVE,
	DG_RESULT_RECIPE_IS_NOT_JOINT_MODE,
	DG_RESULT_ACTIVATE_CURRENT_CONTROL_MODE,
	DG_RESULT_ACTIVATE_GRASP_MOTION,
	DG_RESULT_ACTIVATE_MANUAL_CONTROL_MODE,

	DG_RESULT_SOCK_EXCEPTION = 500,
	DG_RESULT_SOCK_FAILED_WSA_START_UP = 501,

	DG_RESULT_NO_GRASP_OBJECT = 1002,
	DG_RESULT_3F_ONLY_SUPPORTED_3FINGER,

	DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR,
	DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_DEVELOPER,
	DG_RESULT_NOT_SUPPORTED_PORT_NUM,

	// Modbus Result
	DG_RESULT_PORT_EXCEPTION = 2000,
	DG_RESULT_PORT_FAILED_START_UP,
	DG_RESULT_PORT_SET_CONFIG_ERROR,
	DG_RESULT_PORT_SET_TIMEOUT_ERROR,
	DG_RESULT_PORT_SET_COMMASK_ERROR,

	// DIAGNOSIS RESULTS
	DG_RESULT_DIAGNOSING_SYSTEM = 2009,

}DG_RESULT;


/// @brief Enumerated constants for setting up gripper models
typedef enum DG_MODEL
{
	DG_MODEL_NONE = 0x0000,

	DG_MODEL_DG_3F_B = 0x3F01,
	DG_MODEL_DG_3F_M = 0x3F02,

	DG_MODEL_DG_4F = 0x4F02,

	DG_MODEL_DG_5F_LEFT = 0x5F12,
	DG_MODEL_DG_5F_RIGHT = 0x5F22,
}DG_MODEL;

/// @brief Enumerated constants for checking state in blend behavior
typedef enum BLEND_MOTION_STATUS
{
	BLEND_MOTION_STATUS_STOP = 0,
	BLEND_MOTION_STATUS_RUN = 1,
	BLEND_MOTION_STATUS_COMPLETE = 2
}BLEND_MOTION_STATUS;

/// @brief Enumerated constants to set gripper communication mode
/// The Ethernet is a communication used for Modbus TCP in OPERATOR mode and for DEVELOPER mode, 
/// while RS485 is a communication used for Modbus RTU in OPERATOR mode
typedef enum COMMUNICATION_MODE
{
	COMMUNICATION_MODE_ETHERNET = 0,
	COMMUNICATION_MODE_RS485 = 1
}COMMUNICATION_MODE;

/// @brief Enumerated constants to set gripper control mode
/// OPERATOR mode: This mode is controlled using the controller inside the product. 
///								It supports the Modbus Protocol and allows you to easily control the gripper by simply changing the register map, or by using the UI operations provided.
/// DEVELOPER mode: This mode uses a user-developed controller that receives information from each joint of the gripper and allows you to control each joint directly
typedef enum CONTROL_MODE
{
	CONTROL_MODE_OPERATOR = 0,
	CONTROL_MODE_DEVELOPER = 1
}CONTROL_MODE;

/// @brief Enumerated Constants to Set Gripper PID Control Mode
typedef enum GAIN_MODE
{
	GAIN_MODE_PD = 0,
	GAIN_MODE_PID = 1,
}GAIN_MODE;

/// @brief Enumerated Constants for Gripper DEVELOPER Mode Behavior Commands
typedef enum DEVELOPER_MODE_COMMAND
{
	DEVELOPER_MODE_COMMAND_GET_DATA = 0x01,
	DEVELOPER_MODE_COMMAND_SET_RECEIVED_DATA = 0x02,
	DEVELOPER_MODE_COMMAND_SET_DUTY = 0x05,
	DEVELOPER_MODE_COMMAND_SET_GPIO = 0x06,
	DEVELOPER_MODE_COMMAND_SET_IP_ADDRESS = 0x07,
	DEVELOPER_MODE_COMMAND_GET_ID_AND_VERSION = 0x08,
	DEVELOPER_MODE_COMMAND_GET_JOINT_ID = 0x09,
	DEVELOPER_MODE_COMMAND_SET_BOOT_MODE = 0x0A,
}DG_CONTROL_COMMAND;

/// @brief Enumeration of data types that can be received from the gripper
typedef enum DEVELOPER_MODE_RECEIVED_DATA_TYPE
{
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_JOINT = 0x01,
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_CURRENT,
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_TEMPERATURE,
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_VELOCITY,
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_FINGER_FT_SENSOR,
	DEVELOPER_MODE_RECEIVED_DATA_TYPE_GPIO,
}RECEIVED_DATA_TYPE;

/// @brief Enumerated constants for setting gripper grasp mode
/// DG_GRASP_MODE_3F is a grasp mode available on thexDG-3F-B and DG-3F-M models, 
/// while DG_GRASP_MODE_5F is available on the DG-5F model.
typedef enum DG_GRASP_MODE
{
	DG_GRASP_MODE_NONE,
	DG_GRASP_MODE_3F_3FINGER,
	DG_GRASP_MODE_3F_2FINGER_1_AND_2,
	DG_GRASP_MODE_3F_2FINGER_1_AND_3,
	DG_GRASP_MODE_3F_2FINGER_2_AND_3,
	DG_GRASP_MODE_3F_3FINGER_PARALLEL,
	DG_GRASP_MODE_3F_3FINGER_ENVELOP,

	DG_GRASP_MODE_5F_5FINGER = 21,
	DG_GRASP_MODE_5F_3FINGER,
	DG_GRASP_MODE_5F_3FINGER_PARALLEL,
	DG_GRASP_MODE_5F_2FINGER_1_AND_2,
	DG_GRASP_MODE_5F_2FINGER_1_AND_3,
	DG_GRASP_MODE_5F_2FINGER_1_AND_4,
	DG_GRASP_MODE_5F_2FINGER_1_AND_5,
	DG_GRASP_MODE_5F_5FINGER_PARALLEL,
	DG_GRASP_MODE_5F_5FINGER_ENVELOP, 
}DG_GRASP_MODE;

/// @brief Enumeration constants for setting gripper grasp options
/// DG_GRASP_OPTION_PARALLEL is an option that allows grasping with the fingertip surfaces parallel to each other, 
/// while DG_GRASP_OPTION_FIX_TILT is an option that prevents the fingertip tilt from changing during the initial posture.
typedef enum DG_GRASP_OPTION
{
	DG_GRASP_OPTION_NONE = 0,
	DG_GRASP_OPTION_PARALLEL,
	DG_GRASP_OPTION_FIX_TILT,
	DG_GRASP_OPTION_LOAD_POSTURE,
}DG_GRASP_OPTION;

/// @brief Enumeration for checking diagnostic status during gripper diagnostics
typedef enum DG_DIAGNOSIS
{
	DG_DIAGNOSIS_STEP_JOINT_ID = 1,
	DG_DIAGNOSIS_STEP_PERIOD,
	DG_DIAGNOSIS_STEP_TEMPERATURE,
	DG_DIAGNOSIS_STEP_JOINT,

	DG_DIAGNOSIS_RESULT_NONE,
	DG_DIAGNOSIS_RESULT_STANBY,

	DG_DIAGNOSIS_RESULT_OK = 100,
	DG_DIAGNOSIS_RESULT_FAULT = 200,
}DG_DIAGNOSIS;

#ifdef __linux__
#pragma pack(push, 1)
#endif
/********************************************************************************************
****    structs
*********************************************************************************************/
/// @brief Struct for checking the data provided by the gripper
///Name					Size(byte)		Description
///joint					80					Position information for gripper joints(Unit: degree)
///current				80					Current Information for Gripper Joints/(Unit: mA)
///velocity				80					Velocity Information for Gripper Joints(Unit: rpm) (DG - 3F - B models are not supported)
///temperature			80					Temperature information for gripper joints(Unit: ��C) (DG - 3F - B models are not supported)
///TCP					120				Coordinates of each fingertip endpoint(Unit: Postioin = mm, Rotation = degree) (x, y, z, rx, ry, rz)
///moving				4					Movement state information for all joints of the gripper
///targetArrived		4					Target Position Arrival Status Information for All Gripper Joints
///blendMoveStatus	4					Status information when using the Blend Joint feature
///productID			4					Gripper ID
///firmwareVerision	4					Gripper firmware version
typedef struct ReceivedGripperData
{
	float joint[MAX_JOINT_COUNT];
	int current[MAX_JOINT_COUNT];
	int velocity[MAX_JOINT_COUNT];
	float temperature[MAX_JOINT_COUNT];
	float TCP[6 * MAX_FINGER_COUNT];
	int moving;
	int targetArrived;
	int blendMoveState;
	int currentBlendIndex;
	int productID;
	int firmwareVersion;
}ReceivedGripperData;

/// @brief Recipe Setup Structures for Using Blend Motion
///	Name						Size(byte)				Description
///	recipePoseNumber	4							Recipe Pose Number
///	recipeGainNumber	4							Recipe Pain Number
///	blendWaitTime			4							Setting the wait time after an action completes
///	number					4							Blend action unique number
typedef struct RecipeBlendData
{
	int recipePoseNumber;
	int recipeGainNumber;
	int blendWaitTime;
	int number;
}RecipeBlendData;

/// @brief Struct for setting up gripper communication connections
///	Name							Size(byte)				Description
///	comport						8							Setting comfort for RS485 connections
///	ip								32							Setting the IP for TCP / IP connections
///	port							4							Setting the PORT for TCP / IP connections
///	readTimeout				4							Setting the communication timeout time
///	controlMode				4							Setting the Gripper Control Mode
///	communicationMode		4							Setting the Gripper Communication Mode
///	slaveID						4							Setting the Slave ID for a Modbus connection
///	baudrate						4							Setting the baudrate for RS485 connections
typedef struct GripperSystemSetting
{
	char comport[MAX_COMPORT_NAME_SIZE];
	char ip[MAX_GRIPPER_IP_ADDRESS_SIZE];
	int port;
	int readTimeout;
	int controlMode;
	int communicationMode;
	int slaveID;
	int baudrate;
}GripperSystemSetting;

/// @brief Preference Struct for Driving Grippers
// Name						Size(byte)	Description
// jointOffset				80				Setting the Position Offset for Gripper Joints
// jointInpose				80				Setting the angle to check the reach of a gripper joint
// tcpInpose					20				Setting the distance to determine the reach of a fingertip location
// orientationInpose		20				Setting the angle for fingertip reach checks
// receivedDataType		24				Set the type of data you want to receive
// movingInpose			4				Setting angles to determine movement of gripper joints
// jointCount				4				Number of joints in the gripper model currently in use
// fingerCount				4				The number of fingers of the gripper model currently in use
// model						4				Set the gripper model you are currently using
// dutyByteLength			1				Incoming data size for the current model
typedef struct GripperSetting
{
	float jointOffset[MAX_JOINT_COUNT];
	float jointInpose[MAX_JOINT_COUNT];
	float tcpInpose[MAX_FINGER_COUNT];
	float orientationInpose[MAX_FINGER_COUNT];
	int receivedDataType[MAX_RECEIVED_DATA_TYPE_COUNT];
	float movingInpose;
	int jointCount;
	int fingerCount;
	DG_MODEL model;
	int8_t dutyByteLength;
}GripperSetting;

/// @brief Preference Struct for Driving Grippers
///Name					Size(byte)	Description
///targetJoint			80				Set Target Joint Position Value
///jointMotionTime	80				Set the time it takes to reach your target location
///number				4				Recipe Pose Unique Number
///mode					4				Set position control mode(disabled)
typedef struct RecipePoseData
{
	float targetJoint[MAX_JOINT_COUNT];
	int jointMotionTime[MAX_JOINT_COUNT];
	int number;
	int mode;
}RecipePoseData;

/// @brief Recipe Gain Data Settings Structure
/// Name				Size(byte)	Description
/// gainP					80				Setting the P - gain
/// gainD					80				Setting the D - gain
/// gainI					80				Setting up I - gain
/// iLimit					80				Setting the Error Integral Maximum
/// controlPIDMode	4				Setting the PID Control Mode
/// number				4				Recipe gain unique number
/// mode					4				Set position control mode(disabled)
typedef struct RecipeGainData
{
	float gainP[MAX_JOINT_COUNT];
	float gainD[MAX_JOINT_COUNT];
	float gainI[MAX_JOINT_COUNT];
	float iLimit[MAX_JOINT_COUNT];	// must positive

	int controlPIDMode;
	int number;
	int mode;
}RecipeGainData;

/// @brief Recipe data settings structure
/// Name				Size(byte)	Description
/// graspForce			4				Set grasp strength
/// positionMode		80				Locking joints in position control instead of grasp mode
/// graspMode			4				Setting the grasp mode
/// graspOption		4				Set grasp options
/// smoothGrasping	4				Set smooth grasp mode
/// number				4				Recipe grasp unique number
typedef struct RecipeGraspData
{
	float graspForce;
	int postionMode[MAX_JOINT_COUNT];
	int graspMode;
	int graspOption;
	int smoothGrasping;
	int number;
}RecipeGraspData;

/// @brief Struct for Viewing Gripper Fingertip Sensor Data
/// Name			Size(byte)	Description
/// forceTorque		120			Position information for gripper joints
typedef struct ReceivedFingertipSensorData
{
	float forceTorque[6 * MAX_FINGER_COUNT];
}ReceivedFingertipSensorData;

/// @brief Struct for Checking Gripper GPIO Data
///	Name		Size(byte)	Description
///	GPIO		16				Gripper GPIO Data(Output 1, 2, 3 / Input 1)
typedef struct ReceivedGPIOData
{
	int GPIO[MAX_GRIPPER_GPIO_SIZE];
}ReceivedGPIOData;

/// brief Struct for checking result values in diagnostic functions
/// Name			Size(byte)	Description
/// process			4				Processes that are performing diagnostic functions
/// step				4				Check order for each diagnostic feature
/// jointId			4				Results when checking intermodule communication
/// period			4				Resulting values when checking communication cycles
/// joint				4				Result values when examining joints
/// temperature	4				Resulting values when temperature is checked
typedef struct DiagnosisSystem
{
	int process;
	int step;
	int jointId;
	int period;
	int joint;
	int temperature;
}DiagnosisSystem;

#ifdef __linux__
#pragma pack(pop)
#endif

/********************************************************************************************
****    Callbacks
*********************************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
	/// Callback Gripper datas
	typedef void (*ReceivedGripperDatasCallback)(const ReceivedGripperData gripperData);

	/// Callback connected to gripper server
	typedef void (*ConnectedToGripperCallback)();

	/// Callback Disconnected to gripper server
	typedef void (*DisconnectedToGripperCallback)();

	/// Callback communication period
	typedef void (*CommunicationPeriodCallback)(const int communicationPeriod);

	/// Callback Diagnosis result
	typedef void (*DiagnosisSystemCallback)(const DiagnosisSystem diagnosisSystem);

	/// Callback FingerTip Sensor result
	typedef void (*ReceivedSensorCallback)(const ReceivedFingertipSensorData fingertipSensorData);

	/// Callback Vaccum GPIO result
	typedef void (*ReceivedGPIOCallback)(const ReceivedGPIOData GPIOData);

	/// Callback Data process status
	typedef void (*DataProcessingCallback)(const int status);

#ifdef __cplusplus
}
#endif
#endif // DGDATATYPES_H

// Version 1.3.0