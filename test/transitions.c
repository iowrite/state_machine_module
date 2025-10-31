#include <stdbool.h>
#include <stdio.h>

extern int g_current;
bool check_hardware_faults(void *stateData)
{

}
void check_hardware_faults_action(void *stateData, void *newStateData)
{
    printf("Hardware fault detected! Transitioning to fault state.\n");

}



bool check_charge_critical_error( void *condition)
{
    // do some judge herr
    bool res;
    bool result_not_operated = (bool)condition;
    if(result_not_operated)
    {
        return !res;
    }
    return res;
}

void check_charge_critical_error_action( void *data, void *newStateData)
{
    printf("charge critical error detected, go to charge protect\n");
}



bool check_discharge_critical_error( void *condition)
{
    // do some judge herr
    bool res;
    bool result_not_operated = (bool)condition;
    if(result_not_operated)
    {
        return !res;
    }
    return res;
}

void check_discharge_critical_error_action( void *data, void *newStateData)
{
    printf("charge critical error detected, go to discharge protect\n");
}



bool check_will_go_to_sleep( void *condition)
{

}

void check_will_go_to_sleep_action( void *data, void *newStateData)
{
    printf("sleep condition fullfill, transitioning to sleep state.\n");
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
