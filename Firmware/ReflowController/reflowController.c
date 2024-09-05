#include "reflowController.h"
#include "../hardware/hardware.h"
#include "../tinyFSM/tinyfsm.h"

static fsm_t reflowFSM;

////////////////////////////////////////////////////////////////
// predefinitions of local functions

void _state_init();
void _state_
void _state_error();


////////////////////////////////////////////////////////////////
// implementation of api functions

void run()
{
    fsmInitSingleThreaded(
        &reflowFSM,
        _state_init,
        fsmNoAction,
        fsmNoAction);
    
    while(1){
        fsmRun(&reflowFSM);
    }
}

////////////////////////////////////////////////////////////////
// implementation of local functions

void _state_init()
{
    
    // transition state if init is successfull
    if(hw_init()){
        fsmTransitionState(&reflowFSM,);
    }
}

void _state_error()
{

}