/*
  底盘模组
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "component\cmd.h"
#include "component\filter.h"
#include "component\mixer.h"
#include "component\pid.h"
#include "device\can.h"

/* Exported constants --------------------------------------------------------*/
#define CHASSIS_OK (0)
#define CHASSIS_ERR_NULL (-1)
#define CHASSIS_ERR_MODE (-2)
#define CHASSIS_ERR_TYPE (-3)

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* CHASSIS_TYPE_MECANUM: For infantry, hero and engineer. */
/* CHASSIS_TYPE_PARLFIX4: For sentry. */
/* CHASSIS_TYPE_PARLFIX2: For sentry. */
/* CHASSIS_TYPE_OMNI_CROSS: Not implemented. Saved for future. */
/* CHASSIS_TYPE_OMNI_PLUS: Not implemented. Saved for future. */
/* CHASSIS_TYPE_DRONE: For drone. */

typedef enum {
  CHASSIS_TYPE_MECANUM,
  CHASSIS_TYPE_PARLFIX4,
  CHASSIS_TYPE_PARLFIX2,
  CHASSIS_TYPE_OMNI_CROSS,
  CHASSIS_TYPE_OMNI_PLUS,
  CHASSIS_TYPE_DRONE,
} Chassis_Type_t;

typedef struct {
  Chassis_Type_t type;
  const PID_Params_t *motor_pid_param;
  PID_Params_t follow_pid_param;
  float low_pass_cutoff;
} Chassis_Params_t;

typedef struct {
  const Chassis_Params_t *param;

  /* common */
  float dt_sec; /* TODO: 考虑放到Control中实时检测dt */
  CMD_Chassis_Mode_t mode;

  /* Chassis design */
  int8_t num_wheel;
  Mixer_t mixer;

  MoveVector_t move_vec;

  /* Feedback */
  struct {
    float gimbal_yaw_angle;
    float *motor_rpm;
  } feedback;

  struct {
    float *motor_rpm;
  } set_point;

  struct {
    PID_t *motor;
    PID_t follow;
  } pid;

  LowPassFilter2p_t *filter;

  float *out;

} Chassis_t;

/* Exported functions prototypes ---------------------------------------------*/
int8_t Chassis_Init(Chassis_t *c, const Chassis_Params_t *param, float dt_sec);
int8_t Chassis_UpdateFeedback(Chassis_t *c, CAN_t *can);
int8_t Chassis_Control(Chassis_t *c, CMD_Chassis_Ctrl_t *c_ctrl);

#ifdef __cplusplus
}
#endif
