#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"



extern struct state boot;
extern struct state sleep;
extern struct state fault;
static void entryAction( void *stateData)
{
    printf("enter run state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in run state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit run state\n");
}


static struct transition trans[] = {
    {
        .nextState = &fault,
        .action = check_hardware_faults_action,
        .condition = NULL,
        .guard = check_hardware_faults,
    },
    {
        .nextState = &sleep,
        .action = check_will_go_to_sleep_action,
        .condition = NULL,
        .guard = check_will_go_to_sleep,
    },
};

struct Run_Data{

};
static struct Run_Data run_data = {

};

struct state run = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = &boot,
    .parentState = NULL,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &run_data,
};




