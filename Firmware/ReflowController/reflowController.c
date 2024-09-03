#include "reflowController.h"
#include "../hardware/hardware.h"
#include "../tinyFSM/tinyfsm.h"

////////////////////////////////////////////////////////////////
// predefinitions of local functions

void _state_Init();
void _state_error();

////////////////////////////////////////////////////////////////
// implementation of api functions

void run()
{
    fsm_t reflowFSM;


    fsmInit(
        &reflowFSM,

    )

}

////////////////////////////////////////////////////////////////
// implementation of local functions

void _state_Init()
{
    hw_init();
}

void _state_error()
{
    
}