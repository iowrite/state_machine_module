#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;                // parent
extern struct state precharge;
extern struct state fault;
static void entryAction( void *stateData)
{
    printf("enter boot state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in boot state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit boot state\n");
}











static struct transition trans[] = {
    {
        .nextState = &precharge,
        .action = NULL,
        .condition = NULL,
        .guard = NULL,
    },
};


struct Boot_Data{

};
static struct Boot_Data boot_data;

struct state boot = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &run,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &boot_data
};



