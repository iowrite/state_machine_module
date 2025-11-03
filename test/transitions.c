#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int g_current;
bool check_hardware_faults(void *stateData)
{


    return false;
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


    return false;
}

void check_will_go_to_sleep_action( void *data, void *newStateData)
{
    printf("sleep condition fullfill, transitioning to sleep state.\n");
}



bool check_curr_exist( void *condition)
{
    int cur_window = (int)(long long int)condition;                     // just avoid warning
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
