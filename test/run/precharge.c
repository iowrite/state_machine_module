#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state run_state; // parent
extern struct state standby_state;
extern struct state fault_state;

struct Precharge_Data
{
    int precharege_time;
};
static struct Precharge_Data precharge_data = {
    .precharege_time = 0
};

static void entryAction(void *stateData)
{
    precharge_data.precharege_time = 0;
    printf("enter precharge state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    precharge_data.precharege_time += cycle;

    if (count >= 1000)
    {
        count = 0;
        printf("run in precharge state, precharge time is %d\n", precharge_data.precharege_time);
    }
}

static void exitAction(void *stateData)
{
    precharge_data.precharege_time = 0;
    printf("exit precharge state\n");
}

bool check_precharge(void *condition)
{
    // do some judge herr
    bool res;
    if(precharge_data.precharege_time < 10 * 1000)
    {
        if(g_precharge_pass)
        {
            res = true;
        }else{
            return false;
        }
    }else{
        res =  false;
        if(condition)
        {
            res = !res;
        }
    } 
    return res;

}

void check_precharge_action(void *data, void *newStateData)
{
    if (newStateData == &standby_state.data)
    {
        printf("Precharge successful! Transitioning to standby state.\n");
    }
    else if (newStateData == &fault_state.data)
    {
        printf("Precharge failed! Transitioning to fault state.\n");
    }
}

static struct transition trans[] = {
    {
        .nextState = &standby_state,
        .action = NULL,
        .condition = (void *)0,
        .guard = check_precharge,
    },
    {
        .nextState = &fault_state,
        .action = NULL,
        .condition = (void *)1,                         // not operator
        .guard = check_precharge,
    }};


struct state precharge_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = &run_state,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &precharge_data};
