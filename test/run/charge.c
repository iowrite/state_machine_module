#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;                // parent
extern struct state charge_protect;
extern struct state standby;
extern int g_current;
static void entryAction( void *stateData)
{
    printf("enter charge state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in charge state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit charge state\n");
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
        .nextState = &charge_protect,
        .action = check_charge_critical_error_action,
        .condition = NULL,
        .guard = check_charge_critical_error,
    },
    {
        .nextState = &standby,
        .action = check_curr_exist_action,
        .condition = (void *)(BMS_CURRENT_WINDOW),
        .guard = check_curr_exist,
    }
};


struct Charge_Data{

};
static struct Charge_Data charge_data;
struct state charge = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &run,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &charge_data
};



