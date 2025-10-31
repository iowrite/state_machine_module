#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;
static void entryAction( void *stateData)
{
    printf("enter sleep state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in sleep state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit sleep state\n");
}



bool check_wakeup( void *condition)
{

}

void check_wakeup_action( void *data, void *newStateData)
{
    printf("wakeup condition fullfill, transitioning to run state(group).\n");
}





static struct transition trans[] = {
    {
        .nextState = &run,
        .action = check_wakeup_action,
        .condition = NULL,
        .guard = check_wakeup,
    },

};

struct Sleep_Data{

};
static struct Sleep_Data sleep_data = {

};

struct state sleep = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = NULL,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &sleep_data,
};



