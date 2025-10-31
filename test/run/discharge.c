#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;                // parent
extern struct state discharge_protect;
extern struct state standby;
extern int g_current;
static void entryAction( void *stateData)
{
    printf("enter discharge state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in discharge state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit discharge state\n");
}







bool check_curr_exist( void *condition)
{
    int cur_window = (int)condition;
    if(abs(g_current) < abs(cur_window))
    {
        return true;
    }
    return false;
}

void check_curr_exist_action( void *data, void *newStateData)
{
    printf(" current less than current window, go to standby\n");
}







static struct transition trans[] = {
    {
        .nextState = &discharge_protect,
        .action = check_discharge_critical_error_action,
        .condition = NULL,
        .guard = check_discharge_critical_error,
    },
    {
        .nextState = &standby,
        .action = check_curr_exist_action,
        .condition = (void *)(BMS_CURRENT_WINDOW),
        .guard = check_curr_exist,
    }
};

struct Discharge_Data{

};
static struct Discharge_Data discharge_data;

struct state discharge = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &run,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &discharge_data,
};
