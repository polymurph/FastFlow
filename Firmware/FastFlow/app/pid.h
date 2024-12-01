/**
 * @file pid.h
 * @brief Header file for a Proportional-Integral-Derivative (PID) controller.
 * 
 * The PID is implemented ad a PIDT1 implementation and discretized with the bilinear transform.
 * 
 * @author Edwin Koch
 * @author Kerem Yollu
 */
#ifndef _PID_H_
#define _PID_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**
 * @struct pid_t
 * @brief Structure representing the PID controller parameters and state.
 */
typedef struct{
    float K_P;      ///< Proportional gain
    float K_I;      ///< Integral gain
    float K_D;      ///< Derivative gain
    float K_D2;     ///< Secondary derivative gain
    float T_s;      ///< Sampling time
    float error_old;///< Previous error value
    float I_old;    ///< Previous integral value
    float D_old;    ///< Previous derivative value
    float I_max;    ///< Maximum integral windup limit
    float I_min;    ///< Minimum integral windup limit
    float out_max;  ///< maximum output value
    float out_min;  ///< minimum output value
}pid_t;

/**
 * @brief Initializes the PID controller with the provided parameters.
 * @param pidObjp Pointer to the PID controller object.
 * @param T_s Sampling time.
 * @param K_P Proportional gain.
 * @param K_I Integral gain.
 * @param K_D Derivative gain.
 * @param I_max Maximum integral windup limit.
 * @param I_min Minimum integral windup limit.
 * @param out_max Maximum pid output value
 * @param out_min Maximum pid output value
 */
void pid_init(
    pid_t* pidObjp,
    float T_s,
    float K_P,
    float K_I,
    float K_D,
    float I_max,
    float I_min,
    float out_max,
    float out_min
    )

/**
 * @brief Resets the PID controller to its initial state.
 * @param pidObjp Pointer to the PID controller object.
 */
void pid_reset(pid_t* pidObjp);

/**
 * @brief Calculates the PID control output based on the current error.
 * @param pidObjp Pointer to the PID controller object.
 * @param error Current error value.
 * @return PID control output.
 */
float pid_update(pid_t* pidObjp,float error);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif _PID_H_
