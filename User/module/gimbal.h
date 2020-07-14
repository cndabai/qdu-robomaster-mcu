#pragma once


/* Includes ------------------------------------------------------------------*/
#include "component\ahrs.h"
#include "component\cmd.h"
#include "component\filter.h"
#include "component\pid.h"
#include "component\user_math.h"

#include "device\can_device.h"
#include "device\dr16.h"
#include "device\bmi088.h"


/* Exported constants --------------------------------------------------------*/
#define GIMBAL_OK		(0)
#define GIMBAL_ERR		(-1)
#define GIMBAL_ERR_MODE	(-2)

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
enum Gimbal_PID_e{
	GIMBAL_PID_YAW_IN = 0,
	GIMBAL_PID_YAW_OUT,
	GIMBAL_PID_PIT_IN,
	GIMBAL_PID_PIT_OUT,
	GIMBAL_PID_NUM,
};

enum Gimbal_LPF_e{
	GIMBAL_LPF_YAW = 0,
	GIMBAL_LPF_PIT,
	GIMBAL_LPF_NUM,
};

typedef struct {
	PID_Params_t pid[GIMBAL_PID_NUM];
	float32_t low_pass_cutoff;
} Gimbal_Params_t;

typedef struct {
	const Gimbal_Params_t *params;
	
	/* common */
	float32_t dt_sec;
	CMD_Gimbal_Mode_t mode;
	
	/* Feedback */
	BMI088_t *imu;
	AHRS_Eulr_t *imu_eulr;
	AHRS_Eulr_t encoder_eulr;
	
	/* PID */
	PID_t pid[GIMBAL_PID_NUM];
	
	/* Output */
	float32_t yaw_cur_out;
	float32_t pit_cur_out;
	
	/* Output filter */
	LowPassFilter2p_t filter[GIMBAL_LPF_NUM];
} Gimbal_t;


/* Exported functions prototypes ---------------------------------------------*/
int8_t Gimbal_Init(Gimbal_t *g, const Gimbal_Params_t *param, float32_t dt_sec, BMI088_t *imu);
int8_t Gimbal_UpdateFeedback(Gimbal_t *g, CAN_Device_t *can_device);
int8_t Gimbal_Control(Gimbal_t *g, CMD_Gimbal_Ctrl_t *g_ctrl);
