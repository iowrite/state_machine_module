#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"


extern struct state run;                // parent
extern struct state charge;
extern struct state discharge;
extern struct state fault;
extern int g_current;
static void entryAction( void *stateData)
{
    printf("enter standby state\n");
}

static void runAction( void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if(count >= 1000)
    {
        count = 0;
        printf("run in standby state\n");
    }
}

static void exitAction( void *stateData)
{
    printf("exit standby state\n");
}



bool check_current_dir( void *condition)
{
    int cur_window = (int)condition;
    if(cur_window < 0 && g_current < cur_window)
    {
        return true;
    }
    if(cur_window > 0 && g_current > cur_window)
    {
        return true;
    }
    return false;
}

void check_current_dir_action( void *data, void *newStateData)
{
    if(newStateData == &charge.data)
    {
        printf(" current dir is positive, go to charge\n");
    }else if(newStateData == &discharge.data)
    {
        printf(" current dir is negative, go to discharge\n");
    }
}






static struct transition trans[] = {
    {
        .nextState = &charge,
        .action = check_current_dir_action,
        .condition = (void *)BMS_CURRENT_WINDOW,
        .guard = check_current_dir,
    },
    {
        .nextState = &fault,
        .action = check_current_dir_action,
        .condition = (void *)(-BMS_CURRENT_WINDOW),
        .guard = check_current_dir,
    }
};

struct Standby_Data{

};
static struct Standby_Data standby_data;

struct state standby = 
{
    .entryAction = entryAction,
    .exitAction = exitAction,
    .runAction = runAction,
    .entryState = NULL,
    .parentState = &run,
    .numTransitions = ARRAY_SIZE(trans),
    .transitions = trans,
    .data = &standby_data
};



