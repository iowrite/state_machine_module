#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state sleep_state;
static void entryAction(void *stateData)
{
    printf("enter fault state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in fault state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit fault state\n");
}

static struct transition trans[] = {
    {
        .nextState = &sleep_state,
        .action = check_will_go_to_sleep_action,
        .condition = NULL,
        .guard = check_will_go_to_sleep,
    },

};

struct Fault_Data
{
};
static struct Fault_Data fault_data = {

};

struct state fault_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = NULL,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &fault_data,
};
