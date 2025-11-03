#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state run_state; // parent
extern struct state charge_protect_state;
extern struct state standby_state;
extern int g_current;
static void entryAction(void *stateData)
{
    printf("enter charge state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in charge state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit charge state\n");
}

static struct transition trans[] = {
    {
        .nextState = &charge_protect_state,
        .action = check_charge_critical_error_action,
        .condition = NULL,
        .guard = check_charge_critical_error,
    },
    {
        .nextState = &standby_state,
        .action = check_curr_exist_action,
        .condition = (void *)(BMS_CURRENT_WINDOW),
        .guard = check_curr_exist,
    }};

struct Charge_Data
{
};
static struct Charge_Data charge_data;
struct state charge_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = &run_state,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &charge_data};
