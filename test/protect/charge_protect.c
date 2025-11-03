#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state protect_state; // parent
extern struct state standby_state;
extern struct state fault_state;
extern int g_current;
static void entryAction(void *stateData)
{
    printf("enter charge protect state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in charge protect state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit charge protect state\n");
}

static struct transition trans[] = {
    {
        .nextState = &fault_state,
        .action = check_discharge_critical_error_action,
        .condition = NULL,
        .guard = check_discharge_critical_error,
    },
    {
        .nextState = &standby_state,
        .action = check_charge_critical_error_action,
        .condition = (void *)1,
        .guard = check_charge_critical_error,
    },

};

struct Charge_Protect_Data
{
};
static struct Charge_Protect_Data charge_protect_data;
struct state charge_protect_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = &protect_state,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &charge_protect_data};
