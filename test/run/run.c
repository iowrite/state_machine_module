#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state boot_state;
extern struct state sleep_state;
extern struct state fault_state;
static void entryAction(void *stateData)
{
    printf("enter run state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in run state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit run state\n");
}

static struct transition trans[] = {
    {
        .nextState = &fault_state,
        .action = check_hardware_faults_action,
        .condition = NULL,
        .guard = check_hardware_faults,
    },
    {
        .nextState = &sleep_state,
        .action = check_will_go_to_sleep_action,
        .condition = NULL,
        .guard = check_will_go_to_sleep,
    },
};

struct Run_Data
{
};
static struct Run_Data run_data = {

};

struct state run_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = &boot_state,
        .parentState = NULL,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &run_data,
};
