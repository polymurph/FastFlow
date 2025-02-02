#include "pid.h"

float _P(pid_t *pidObjp, float error);
float _I(pid_t *pidObjp, float error);
float _DT(pid_t *pidObjp, float error);


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
{
    // this is a precalculation done for a factor needed for the bilinear interpolation
    // it is done here as this only has to be calculated once at the initialisation of
    // the pid.
    pidObjp->K_D2 = (T_s * (1 + K_D) - 1) / (1 + T_s * (1 - K_D));
    pidObjp->T_s = T_s;
    pidObjp->K_P = K_P;
    pidObjp->K_I = K_I;
    pidObjp->K_D = K_D;
    pidObjp->I_min = I_min;
    pidObjp->I_max = I_max;
    pidObjp->error_old = 0.0;
    pidObjp->I_old = 0.0;
    pidObjp->D_old = 0.0;
    pidObjp->out_max = out_max;
    pidObjp->out_min = out_min;
}

void pid_reset(pid_t* pidObjp)
{
    pidObjp->error_old = 0.0;
    pidObjp->I_old = 0.0;
    pidObjp->D_old = 0.0;
}

float pid_update(pid_t* pidObjp, float error)
{
    float P = _P(pidObjp,error);
    float I = _I(pidObjp,error);
    float DT = _DT(pidObjp,error);
    float output = P + I + DT;
    
    // update old error
    pidObjp->error_old = error;

    // output clamping
    if (output > pidObjp->out_max) {
        return pidObjp->out_max;
    } else if (output < pidObjp->out_min) {
        return pidObjp->out_min;
    }

    return  output;
}

float _P(pid_t *pidObjp, float error)
{
    return pidObjp->K_P * error;
}

float _I(pid_t *pidObjp, float error)
{
    float I = pidObjp->K_I * (error + pidObjp->error_old) + pidObjp->I_old;

    // integral saturation (windup prevention)
    if(I > pidObjp->I_max){ pidObjp->I_old = pidObjp->I_max;}
    else if(I < pidObjp->I_min){pidObjp->I_old = pidObjp->I_min;}
    else { pidObjp->I_old = I;}

    return pidObjp->I_old;
}

float _DT(pid_t *pidObjp, float error)
{
    pidObjp->D_old = 2 * pidObjp->K_D * (error - pidObjp->error_old) - pidObjp->K_D2 * pidObjp->D_old;
    return pidObjp->D_old;
}
