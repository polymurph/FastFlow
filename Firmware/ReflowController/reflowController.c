#include "reflowController.h"
#include "../hardware/hardware.h"
#include "../tinyFSM/tinyfsm.h"

static fsm_t reflowFSM;

void _state_init();
void _state_menu();
void _state_error();

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

void _state_init()
{
    
    // transition state if init is successfull
    if(hw_init()){
        fsmTransitionState(&reflowFSM, _state_menu, fsmNoAction);
    } else {
        fsmTransitionState(&reflowFSM, _state_error, fsmNoAction);
    }
}

void _state_menu()
{
    
}

void _state_error()
{

}
