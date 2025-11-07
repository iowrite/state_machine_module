#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state run_state; // parent
extern struct state discharge_protect_state;
extern struct state standby_state;
extern int g_current;
static void entryAction(void *stateData)
{
    printf("enter discharge state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in discharge state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit discharge state\n");
}

static struct transition trans[] = {
    {
        .nextState = &discharge_protect_state,
        .action = check_discharge_critical_error_action,
        .condition = NULL,
        .guard = check_discharge_critical_error,
    },
    {
        .nextState = &standby_state,
        .action = check_curr_action,
        .condition = (void *)0,
        .guard = check_curr,
    },
    {
        .nextState = &charge_state,
        .action = check_curr_action,
        .condition = (void *)1,
        .guard = check_curr,
    },

};

struct Discharge_Data
{
};
static struct Discharge_Data discharge_data;

struct state discharge_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = &run_state,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &discharge_data,
};
