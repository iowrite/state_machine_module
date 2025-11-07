#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "state_machine.h"
#include "../transitions.h"
#include "../main.h"

extern struct state run_state; // parent
extern struct state precharge_state;
extern struct state fault_state;
static void entryAction(void *stateData)
{
    printf("enter boot state\n");
}

static void runAction(void *stateData)
{
    uint32_t cycle = BMS_STATE_MACHINE_PERIOD;
    static uint32_t count = 0;
    count += cycle;
    if (count >= 1000)
    {
        count = 0;
        printf("run in boot state\n");
    }
}

static void exitAction(void *stateData)
{
    printf("exit boot state\n");
}


bool check_poweronCMD(void *condition)
{

    return g_poweron_CMD;
}




static struct transition trans[] = {
    {
        .nextState = &precharge_state,
        .action = NULL,
        .condition = NULL,
        .guard = check_poweronCMD,
    },
};

struct Boot_Data
{
};
static struct Boot_Data boot_data;

struct state boot_state =
    {
        .entryAction = entryAction,
        .exitAction = exitAction,
        .runAction = runAction,
        .entryState = NULL,
        .parentState = &run_state,
        .numTransitions = ARRAY_SIZE(trans),
        .transitions = trans,
        .data = &boot_data};
