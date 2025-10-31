#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;                // parent
extern struct state standby;
extern struct state fault;
static void entryAction( void *stateData)
{
    printf("enter precharge state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in precharge state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit precharge state\n");
}



bool check_precharge( void *condition)
{
    // do some judge herr
    bool res;
    bool result_not_operated = (bool)condition;
    if(result_not_operated)
    {
        return !res;
    }
    return res;

}

void check_precharge_action( void *data, void *newStateData)
{
    if(newStateData == &standby.data)
    {
        printf("Precharge successful! Transitioning to standby state.\n");
    }else if(newStateData == &fault.data)
    {
        printf("Precharge failed! Transitioning to fault state.\n");
    }
    
}









static struct transition trans[] = {
    {
        .nextState = &standby,
        .action = NULL,
        .condition = (void *)1,
        .guard = check_precharge,
    },
    {
        .nextState = &fault,
        .action = NULL,
        .condition = (void *)0,
        .guard = check_precharge,
    }
};

struct Precharge_Data{

};
static struct Precharge_Data precharge_data = {

};

struct state precharge = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &run,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &precharge_data
};



