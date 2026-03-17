#pragma once

#ifndef DGSDK_H
#define DGSDK_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "DGDataTypes.h"

#ifdef DG_SDK_STATIC
	#define DGSDK
#else
	#ifdef DG_SDK_EXPORTS
		#ifdef _WIN32
			#define DGSDK __declspec(dllexport)
		#else
			#define DGSDK __attribute__((visibility("default")))
		#endif
	#else
		#ifdef _WIN32
			#define DGSDK __declspec(dllexport)
		#else
			#define DGSDK __attribute__((visibility("default")))
		#endif
	#endif
#endif

	/********************************************************************************************
	****    System Setting Functions
	*********************************************************************************************/
	/// @ brief Set to establish communication with the gripper. This function should be the highest priority, and if it is not performed, all other functions cannot be performed.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								GripperSystemSetting						struct GripperSystemSetting
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_INVALID_CONTROL_MODE									Control Mode Value Errors
	///		DG_RESULT_SYSTEM_SETTING_UNKNOWN_BUDRATE				Baud rate input value error
	DGSDK DG_RESULT SetGripperSystem(GripperSystemSetting setting);

	/// @ brief Options to set default values for gripper Motion. If you do not perform this option, you will not be able to control the gripper
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								GripperSystemSetting 					struct GripperSetting
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_FOUND_MODEL	Model								number entry error
	DGSDK DG_RESULT SetGripperOption(GripperSetting setting);

	/// @ brief Attempt to establish a communication connection to the gripper
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_SOCK_FAILED_WSA_START_UP	WSA						Start Up Errors
	///		
	/// 	Socket									Communication Exception Thrown
	DGSDK DG_RESULT ConnectToGripper();

	/// @ brief Options to set default values for gripper Motion. If you do not perform this option, you will not be able to control the gripper
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT DisconnectToGripper();

	/// @ brief Start the system. Only after you perform this function can you receive data from the gripper and control the gripper
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT SystemStart();

	/// @ brief Stop the system. When this function is performed, no data is received from the gripper, and no control can be performed
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT SystemStop();

	/// @ brief Set the gripper IP and Port
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									ip												Enter the IP to change
	///		int									port											Enter the port to change
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT SetIp(int* ip, int port);


	/********************************************************************************************
	****    Callback
	*********************************************************************************************/
	/// @ brief Receive events about communication connections with the gripper
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT CallbackForOnConnected(ConnectedToGripperCallback connectedCallback);

	/// @ brief Receive an event when communication with the gripper is lost.
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT CallbackForOnDisconnected(DisconnectedToGripperCallback disconnectedCallback);

	/// @ brief Receive data from the gripper.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								ReceivedGripperData						struct ReceivedGripperData
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT CallbackForOnReceivedGripperData(ReceivedGripperDatasCallback gripperDatasCallback);

	/// @ brief Receive the communication cycles with the gripper
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									period										Communication cycles
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT CallbackForOnCommunicationPeriod(CommunicationPeriodCallback communicationPeriodCallbak);

	/// @ brief Receive the gripper diagnostic process, status, and results.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								DiagnosisSystem							struct DiagnosisSystem
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT CallbackForOnDiagnosisSystem(DiagnosisSystemCallback diagnosisSystemCallback);

	/// @ brief Receive gripper fingertip sensor data.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								ReceivedSensorData						struct ReceivedSensorData
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT CallbackForOnReceivedFingertipSensorData(ReceivedSensorCallback fingertipSensorDataCallback);

	/// @ brief Receive gripper GPIO data.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		struct								ReceivedGPIOData						struct ReceivedGPIOData
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT CallbackForOnReceivedGPIOData(ReceivedGPIOCallback GPIODataCallback);


	/// @ brief Receive Data Processing status
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									status											Data processing status
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	DGSDK DG_RESULT CallbackForOnDataProcessing(DataProcessingCallback status);

	/********************************************************************************************
	****    Other System functions
	*********************************************************************************************/
	/// @ brief Sets the low-pass filter gain. The lower the set gain value, the smoother the motion.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									isOn											Enable filters
	///		float								alpha											Set filter gain values
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	///		DG_RESULT_DATA_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetLowPassFilterAlpha(int isUsed, float alpha);

	/// @ brief Boot mode entry function for firmware download
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									isOn											Setting the boot mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_INVALID_PASSWORD											Incorrect password
	DGSDK DG_RESULT SetBootMode(char* password);

	/// @ brief Set the gripper GPIO output
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									gpio											Set output values
	///		int									outputNumber								Set the output number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetGPIOOuput(int gpio, int outputNumber);

	/// @ brief Sets the gripper global GPIO output. 
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int*								gpio											Set output values
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetGPIOOuputAll(int* output);

	/// @ brief Set the torque limit mode for the gripper.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									isOn											Setting the Torque Limit Mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetTorqueLimitMode(int isOn);

	/// @ brief Set the recipe to start upon gripper booting.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									recipePoseNumber						Set the pose recipe number
	///		int									recipeGainNumber						Set the gain recipe number
	///		int									recipeGraspNumber						Set the grasp recipe number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Recipe pose number entry error
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Recipe gain number entry error
	///		DG_RESULT_OVERFLOW_RECIPE_GRASP_COUNT						Recipe grasp number entry error
	DGSDK DG_RESULT SetBootRecipe(int recipePoseNumber, int recipeGainNumber, int recipeGraspNumber);

	/// @ brief Set the recipe to start upon gripper booting.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									recipePoseNumber						Set the pose recipe number
	///		int									recipeGainNumber						Set the gain recipe number
	///		int									recipeGraspNumber						Set the grasp recipe number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Recipe pose number entry error
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Recipe gain number entry error
	///		DG_RESULT_OVERFLOW_RECIPE_GRASP_COUNT						Recipe grasp number entry error
	DGSDK DG_RESULT EEPROMWrite();

	/// @ brief Run the gripper self-diagnostics. Diagnoses motor communication, control cycles, temperature, current, articulation, and vibration. 
///		No commands can be performed during diagnostics, and if any abnormalities occur, diagnostics will end, and control will be aborted.
/// @ returns
/// 	#Value																			#Description
///		DG_RESULT_NONE															Normal
///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
///		DG_RESULT_IS_MOVING														Gripper in motion
	DGSDK DG_RESULT SystemDiagnosis();

	/// @ Backup Settings is a feature that allows you to back up the currently applied data and save it to your PC
	/// @ param
	///		#Data Types					#Variables									#Description
	///		char*								path											Enter the save path
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_DEVELOPER			No DEVELOPER mode support
	DGSDK DG_RESULT BackupRecipeData(char* path);

	/// @ brief This feature is supported by operator mode only. This function reads data from a backed up file on the given path and restores it to the associated gripper.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		char*								path											Enter the save path
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_IS_MOVING														Gripper in motion
	DGSDK DG_RESULT RestoreRecipeData(char* path);

	/********************************************************************************************
	****    Motion Setting Functions
	*********************************************************************************************/
	/// @ brief Sets the grasp algorithm based on force control. six basic grasp modes: three-legged, two-legged, parallel, and enveloping, and you can utilize grasp options, position control modes, and more to perform grasp actions in different ways.
	/// @ param
	///		#Data Types					#Variables									#Description
	//		int									graspMode									Setting the grasp mode
	//		float								graspForce									Set grasp strength
	//		int									graspOption								Set grasp options
	//		int									smoothGrasping							Using soft grasps	
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_MODEL								Model number entry error
	///		DG_RESULT_OVERFLOW_GRASP_MODE_DATA						Entering an invalid grasp mode value
	///		DG_RESULT_OVERFLOW_GRASP_OPTION_DATA					Entering an invalid grasp option value
	DGSDK DG_RESULT SetGraspData(DG_GRASP_MODE graspMode, float graspForce, int graspOption, int smoothGrasping);

	/// @ brief Change grasp strength. This function allows you to change the grasp strength of any grasp without setting anything else
	/// @ param
	///		#Data Types					#Variables									#Description
	//		float								graspForce									Set grasp strength
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetGraspForce(float graspForce);

	/// @ brief Sets the P Gain value for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float								gainP											Enter P Gain
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	DGSDK DG_RESULT SetJointGainP(float gainP, int jointNumber);

	/// @ brief Sets the P gain value for a single finger joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainP											Enter P Gain
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetJointGainPFinger(float* gainP, int fingerNumber);

	/// @ brief Sets the P Gain value for all joints.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainP											Enter P Gain
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetJointGainPAll(float* gainP);

	/// @ brief Sets the D gain value for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float								gainD											Enter D Gain
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	DGSDK DG_RESULT SetJointGainD(float gainD, int jointNumber);

	/// @ brief Sets the D gain value for a single finger joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainD											Enter D Gain
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetJointGainDFinger(float* gainD, int fingerNumber);

	/// @ brief Sets the D Gain value for all joints.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float								gainD											Enter D Gain
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetJointGainDAll(float* gainD);

	/// @ brief Sets the I gain value and error integral maximum for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float								gainI											Enter the I gain
	///		float								iLimit											Setting the Error Integral Maximum
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	DGSDK DG_RESULT SetJointGainI(float gainI, float iLimit, int jointNumber);

	/// @ brief Set the I-gain value and error integral maximum for a single finger.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Setting the Error Integral Maximum
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetJointGainIFinger(float* gainI, float* iLimit, int fingerNumber);

	/// @ brief Sets the I-gain value and error integral maximum for all joints.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Setting the Error Integral Maximum
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetJointGainIAll(float* gainI, float* iLimit);

	/// @ brief Sets the gripper controller. This function can set the gripper controller to PD mode and PID mode
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									mode											Setting the Gain Mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetControlPIDMode(int mode);

	/// @ brief Set the maximum values for the P, I, and D gains and error integrals for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float								gainP											Enter P Gain
	///		float								gainD											Enter the D gain
	///		float								gainI											Enter the I gain
	///		float								iLimit											Setting the Error Integral Maximum
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	DGSDK DG_RESULT SetJointGainPID(float gainP, float gainD, float gainI, float iLimit, int jointNumber);

	/// @ brief Set the maximum values for P, I, D gain and error integration for a single finger.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								gainP											Enter P Gain
	///		float*								gainD											Enter the D gain
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Setting the Error Integral Maximum
	///		int									fingerNumber								Enter the finger  number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetJointGainPIDFinger(float* gainP, float* gainD, float* gainI, float* iLimit, int fingerNumber);

	/// @ brief Set the maximum values for the P, I, and D gains and error integrals for all joints
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								gainP											Enter P Gain
	///		float*								gainD											Enter the D gain
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Setting the Error Integral Maximum
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetJointGainPIDAll(float* gainP, float* gainD, float* gainI, float* iLimit);

	/// @ brief Set the P, I, D gain and error integral max values for all joints in bulk.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float								gainP											Enter P Gain
	///		float								gainD											Enter the D gain
	///		float								gainI											Enter the I gain
	///		float								iLimit											Setting the Error Integral Maximum
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetJointGainPIDAllEqual(float gainP, float gainD, float gainI, float iLimit);

	/// @ brief Sets the position travel time for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									motionTime									Set the time to move the location (in ms)
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetMotionTimeJoint(int motionTime, int jointNumber);

	/// @ brief Set the time to move the position of a single finger.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int*								motionTime									Set the time to move the location (in ms)
	///		int									fingerNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetMotionTimeFinger(int* motionTime, int fingerNumber);

	/// @ brief Set the position travel time for every joint individually.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int*									motionTime									Set the time to move the location (in ms)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetMotionTimeAll(int* motionTime);

	/// @ brief Set the position travel time for all joints in bulk.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									motionTime									Set the time to move the location (in ms)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetMotionTimeAllEqual(int motionTime);

	/// @ brief  the position control mode for one joint. 
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									positionMode								Enable position control mode
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_OVERFLOW_JOINT_COUNT								Errors in entering joint numbers
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetPositionModeJoint(int positionMode, int jointNumber);

	/// @ brief  Set the position control mode for single finger joints
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int*									positionMode								Enable position control mode
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetPositionModeFinger(int* positionMode, int fingerNumber);

	/// @ brief  Set the position control mode for single finger joints
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int*								positionMode								Enable position control mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetPositionModeAll(int* positionMode);

	/// @ brief Set the gripper to current-based position control mode
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									isOn											Setting the current control mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT SetCurrentControlMode(int isOn);

	/// @ brief Set the target current value for one joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float								targetCurrent								Enter a target current value (mA)
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_JOINT_COUNT									Errors in entering joint numbers
	///		DG_RESULT_OVERFLOW_CURRENT_LIMIT								Maximum current value exceeded
	DGSDK DG_RESULT SetTargetCurrentJoint(int targetCurrent, int jointNumber);

	/// @ brief Set the target current value for a single finger joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								targetCurrent								Enter a target current value (mA)
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	///		DG_RESULT_OVERFLOW_CURRENT_LIMIT								Maximum current value exceeded
	DGSDK DG_RESULT SetTargetCurrentFinger(int* targetCurrent, int fingerNumber);

	/// @ brief Set the target current value for a single finger joint.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								targetCurrent								Enter a target current value (mA)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_CURRENT_LIMIT								Maximum current value exceeded
	DGSDK DG_RESULT SetTargetCurrentAll(int* targetCurrent);

	/// @ brief Saves the current gripper joint position information, the currently set action time, to the entered number of pose recipes. 
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									recipeNumber								Recipe Pose Number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Errors in entering recipe pose numbers
	DGSDK DG_RESULT UpdateRecipeCurrentPoseData(int poseNumber);

	/// @ brief 	/// @ brief Saves the currently set gripper joint's target position information and action time to the entered number of pose recipes
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									recipeNumber								Recipe Pose Number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Errors in entering recipe pose numbers
	DGSDK DG_RESULT UpdateRecipeTargetPoseData(int poseNumber);

	/// @ brief This works by reading the pose recipe information for the entered number
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									recipeNumber								Recipe Pose Number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Errors in entering recipe pose numbers
	///		DG_RESULT_DATA_IS_ZERO													0 cannot be entered
	DGSDK DG_RESULT LoadRecipePoseData(int poseNumber);

	/// @ brief Saves the currently set P, I, and D gains, error integral maximum, and PID control mode to the entered number of gain recipes.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									recipeNumber								Recipe gain number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Errors in entering recipe gain numbers
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT UpdateRecipeGainData(int gainNumber);

	/// @ brief eads and sets the gain recipe data for the entered number
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									recipeNumber								Recipe gain number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Errors in entering recipe gain numbers
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT LoadRecipeGainData(int gainNumber);

	/// @ brief Saves the currently set Grasp Options, Grasp Mode, Smooth Grasp Mode, Position Control Mode, and Grasp Strength settings to the entered Grasp Recipe
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									graspNumber								Recipe grasp number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_GRASP _COUNT						Error entering recipe grasp number
	///		DG_RESULT_OVERFLOW_GRASP_OPTION_DATA						Errors in entering grasp option values
	///		DG_RESULT_OVERFLOW_GRASP_MODE_DATA							Errors in entering grasp mode values
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT UpdateRecipeGraspData(int graspNumber);

	/// @ brief Reads and sets the grasp recipe data for the entered number.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									graspNumber								Recipe grasp number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_GRASP _COUNT						Error entering recipe grasp number
	///		DG_RESULT_DATA_IS_ZERO													0 Cannot be entered
	DGSDK DG_RESULT LoadRecipeGraspData(int graspNumber);

	/// @ brief Adds the pose recipe, gain recipe, and wait time after completion of the action from the input blend action data to the blend action recipe.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									graspNumber								Recipe grasp number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_ADD_BLEND_SIZE								Exceeding blend data addition
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Errors in entering recipe pose numbers
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Error entering recipe gain number
	///		DG_RESULT_VALUE_IS_NEGATIVE											0 cannot be entered
	DGSDK DG_RESULT UpdateBlendJoint(RecipeBlendData blendData);

	/// @ brief Adds the pose recipe, gain recipe, and wait time after completion of the action from the input blend action data to the blend action recipe.
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_READY_BLEND_MOVE_STATE								Performing a blend move
	DGSDK DG_RESULT ClearMoveBlendJoint();

	/// @ brief Deletes the list of currently added blend data.
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_ADD_BLEND_SIZE								Exceeding blend data addition
	///		DG_RESULT_OVERFLOW_RECIPE_POSE_COUNT						Errors in entering recipe pose numbers
	///		DG_RESULT_OVERFLOW_RECIPE_GAIN_COUNT						Error entering recipe gain number
	///		DG_RESULT_DATA_IS_ZERO													No saved blend motion
	DGSDK DG_RESULT AddMoveBlendJoint();

	/// @ brief Saves the list of currently added blends to the Blend Motion list
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									blendNumber								Enter a Blend Motion Number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_RECIPE_BLEND_SIZE							Blend Motion Number Entry Error
	///		DG_RESULT_VALUE_IS_NEGATIVE											0 cannot be entered
	DGSDK DG_RESULT SetMoveBlendJoint(int blendNumber);

	/// @ brief Performs a blend motion with the entered blend number
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									blendNumber								Enter a Blend Motion Number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_READY_BLEND_MOVE_STATE								Performing a blend move
	///		DG_RESULT_OVERFLOW_RECIPE_BLEND_SIZE							Blend Motion Number Entry Error
	///		DG_RESULT_BLEND_SIZE_ZERO												No List in Blend Motion
	DGSDK DG_RESULT StartMoveBlendJoint(int blendNumber);

	/// @ brief Stop the blend motion currently being performed.
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_BLEND_SIZE_ZERO												No List in Blend Motion
	DGSDK DG_RESULT StopMoveBlendJoint();

	/********************************************************************************************
	****    Motion Functions
	*********************************************************************************************/
	/// @ brief Set the gripper to manual teaching mode. Manual teaching mode allows a human to set the gripper pose and teach all joints directly.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									isOn											Setting manual operation mode
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT ManualTeachMode(int isOn);

	/// @ brief Perform a grasp motion
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    int									isGrasp										Setting up grasp motion performance
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_DATA_IS_NOT_BOOLEAN									Only 0 and 1 can be entered
	DGSDK DG_RESULT StartGraspMotion(int isGrasp);

	/// @ brief Performs a repositioning of one joint
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float								targetJoint									Enter the target position value (in degrees)
	///		int									jointNumber								Enter the joint number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_JOINT_COUNT									Errors in entering joint numbers
	DGSDK DG_RESULT MoveJoint(float targetJoint, int jointNumber);

	/// @ brief Performs a repositioning of a single finger joint
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								targetJoint									Enter the target position value (in degrees)
	///		int									fingerNumber								Enter the finger  number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT MoveJointFinger(float* targetJoint, int fingerNumber);

	/// @ brief Performs a repositioning of all joints
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								targetJoint									Enter the target position value (in degrees)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT MoveJointAll(float* targetJoint);

	/// @ brief Performs real-time position movement for all joints. 
	///				This function is not affected by the motion time setting and executes movements immediately. 
	///				It is available only in Developer Mode
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								targetJoint									Enter the target position value (in degrees)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	/// 	DG_RESULT_ACTIVATE_CURRENT_CONTROL_MODE					Running Current Control Mode
	DGSDK DG_RESULT MoveServoJoint(float* targetJoint);

	/********************************************************************************************
	****    Experimental Functions
	*********************************************************************************************/
	/// This chapter introduces you to experimental functions using Delto Gripper.
	/// Experimental functions are only available in developer mode.
	/// Because experimental functions are not officially released, they may not perform the expected Motion at certain settings and in certain locations.
	/// Nevertheless, they allow you to experience characteristics that are unique to Delto Gripper.

	/// @ briefYou can set gain values for the X, Y, Z, RX, RY, and RZ axes per finger.
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								gainP											Enter P Gain
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetTCPGainPFinger(float* gainP, int fingerNumber);

	/// @ brief Set the P-gain of the entire finger for gripper TCP positioning
	/// @ param
	///		#Data Types					#Variables									#Description
	///	    float*								gainP											Enter P Gain
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPGainPAll(float* gainP);

	/// @ brief  Set the D-gain for one finger for gripper TCP positioning control.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainD											Enter the D gain
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetTCPGainDFinger(float* gainD, int fingerNumber);

	/// @ brief  Sets the D-gain of the entire finger for gripper TCP positioning
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainD											Enter the D gain
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPGainDAll(float* gainD);

	/// @ brief Sets the maximum I-gain and error integral for one finger for gripper¨s TCP position control.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Enter the maximum value of the error integral
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetTCPGainIFinger(float* gainI, float* iLimit, int fingerNumber);

	/// @ brief Sets the maximum I-gain and error integral for one finger for gripper¨s TCP position control.
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Enter the maximum value of the error integral
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPGainIAll(float* gainI, float* iLimit);

	/// @ brief Set the maximum values for P, I, D gain and error integration for one finger for gripper TCP position control
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainP											Enter P Gain
	///		float*								gainD											Enter the D gain
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Enter the maximum value of the error integral
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT SetTCPGainPIDFinger(float* gainP, float* gainD, float* gainI, float* iLimit, int fingerNumber);

	/// @ brief Set the maximum values of the P, I, D gains and error integrals for all fingers for gripper TCP position control
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainP											Enter P Gain
	///		float*								gainD											Enter the D gain
	///		float*								gainI											Enter the I gain
	///		float*								iLimit											Enter the maximum value of the error integral
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPGainPIDAll(float* gainP, float* gainD, float* gainI, float* iLimit);

	/// @ brief Sets the target coordinate arrival time for one finger when controlling gripper TCP positioning
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									motionTime									Set the time to move the location(in ms)
	///		int									fingerNumber								Enter the finger number
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	///		DG_RESULT_OVERFLOW_FINGER											Finger number entry error
	DGSDK DG_RESULT SetTCPMotionTimeFinger(int motionTime, int fingerNumber);

	/// @ brief Sets the target coordinate arrival time for all fingers when controlling gripper TCP positioning
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int*									motionTime									Set the time to move the location(in ms)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPMotionTimeAll(int* motionTime);

	/// @ brief Set the same target coordinate arrival time for all fingers when controlling gripper TCP positioning
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int									motionTime									Set the time to move the location(in ms)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetTCPMotionTimeAllEqual(int motionTime);

	/// The gripper performs TCP position control rather than articulation. 
	/// TCP coordinates use the Cartesian coordinate system and are calculated relative to the center point of the bottom of the gripper base.
	/// @ brief One gripper finger controls the TCP position
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								targetTCP									Target Cartesian Coordinate Array(x, y, z, rx, ry, rz)
	///		int									fingerNumber								Finger number to control position
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_OVERFLOW_FINGER_COUNT								Finger number entry error
	DGSDK DG_RESULT MoveTCPFinger(float* targetTCP, int fingerNumber);

	/// @ brief Control the TCP position of the entire gripper finger
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								targetTCP									Target Cartesian Coordinate Array(x, y, z, rx, ry, rz)
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT MoveTCPAll(float* targetTCP);

	/// @ brief Retrieves the TCP coordinates of the current gripper
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								currentTCP									Get the current TCP coordinates in the entered variable
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT GetCurrentTcpPose(float* currentTCP);

	/// @ brief Set the P, I, D gains and error integral maximums for inhand manipulation Motion. 
	///				Sets the gains for the position components X, Y, and Z in the Cartesian coordinate system. 
	///				This feature is only supported by DG-3F-B and DG-3F-M
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								gainP											P-gain input to control the position of the object to be manipulated
	///		float*								gainD											D - gain input for positioning the object to be manipulated
	///		float*								gainI											I - gain input to control the position of the object to be manipulated
	///		float*								iLimit											Enter the maximum value of the error integral to control the position of the object to be manipulated
	/// @ returns
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	///		DG_RESULT_VALUE_IS_NEGATIVE											Cannot enter negative values
	DGSDK DG_RESULT SetManipulationGainPIDAll(float* gainP, float* gainD, float* gainI, float* iLimit);

	/// @ brief Manipulate the grasped object. It only be performed in the basic 3-finger grasp action, 
	///				and can only be performed after performing the grasp action function. 
	///				This function is only supported by DG-3F-B and DG-3F-M
	/// @ param
	///		#Data Types					#Variables									#Description
	///		float*								targetOffset									Offset to move the grasped object by (mm)(ツX, ツY, ツZ, ツRX, ツRY, ツRZ)
	///		int									motionTime									Set offset travel time(ms)
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_SYSTEM_SETTING_NOT_PERFORMED						No system setup
	///		DG_RESULT_NOT_SUPPORTED_CONTROL_MODE_OPERATOR		DEVELOPER mode only
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT InHandManipulation(float* targetOffset, int motionTime);

	/// @ brief Initializes the finger tip sensor data to zero
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT SetFingerTipDataZero();

	/// @ brief This function is a getter that retrieves data from the ReceivedGripperData struct
	/// @ param
	///		#Data Types					#Variables									#Description
	///		ReceivedGripperData*		recvGripperData							input a pointer to the ReceivedGripperData struct
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	///		DG_RESULT_DIAGNOSING_SYSTEM										Running diagnostic mode
	DGSDK DG_RESULT GetReceivedGripperData(ReceivedGripperData* recvGripperData);

	/// @ brief This function is a getter that retrieves communication period data
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int*								countPeriod									input a pointer to the int various to get Communication Period ( Hz )
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT GetCommunicationPeriod(int* countPeriod);

	/// @ brief This function is a getter that retrieves communication period data
	/// @ param
	///		#Data Types									#Variables									#Description
	///		ReceivedFingertipSensorData*				recvFingerTipData							input a pointer to the recvFingerTipData struct
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT GetReceivedFingertipSensorData(ReceivedFingertipSensorData* recvFingerTipData);

	/// @ brief This function is a getter that retrieves communication period data
	/// @ param
	///		#Data Types					#Variables									#Description
	///		ReceivedGPIOData*			recvGPIOData								input a pointer to the recvGPIOData struct
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT GetReceivedGPIOData(ReceivedGPIOData* recvGPIOData);

	/// @ brief This function is a getter that retrieves backup status data
	/// @ param
	///		#Data Types					#Variables									#Description
	///		int*								status											input a pointer to the int various to get backup result ( bool )
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT GetDataProcessing(int* status);

	/// @ brief This function is a getter that retrieves data from the DiagnosisSystem struct
	/// @ param
	///		#Data Types					#Variables									#Description
	///		DiagnosisSystem*				diagnosisData								
	/// @ returns	
	/// 	#Value																			#Description
	///		DG_RESULT_NONE															Normal
	DGSDK DG_RESULT	GetDiagnosisSystem(DiagnosisSystem* diagnosisData);
#ifdef __cplusplus
}
#endif
#endif // DGSDK_H

// Version 1.3.0