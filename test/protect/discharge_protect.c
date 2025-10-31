#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state protect;                // parent
extern struct state standby;
extern struct state fault;
extern int g_current;
static void entryAction( void *stateData)
{
    printf("enter discharge protect state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in discharge protect state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit discharge protect state\n");
}






static struct transition trans[] = {
    {
        .nextState = &fault,
        .action = check_charge_critical_error_action,
        .condition = NULL,
        .guard = check_charge_critical_error,
    },
    {
        .nextState = &standby,
        .action = check_charge_critical_error_action,
        .condition = (void *)1,                                 // not operator on the result
        .guard = check_charge_critical_error,
    },

};


struct Discharge_Protect_Data{

};
static struct Discharge_Protect_Data discharge_protect_data;
struct state charge = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &protect,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &discharge_protect_data
};



