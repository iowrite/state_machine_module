#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state fault;
static void entryAction( void *stateData)
{
    printf("enter protect state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in protect state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit protect state\n");
}


static struct transition trans[] = {
    {
        .nextState = &fault,
        .action = check_hardware_faults_action,
        .condition = NULL,
        .guard = check_hardware_faults,
    },

};

struct Protect_Data{

};
static struct Protect_Data protect_data = {

};

struct state protect = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = NULL,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &protect_data,
};




