#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

#include "main.h"
extern int g_current;
bool check_hardware_faults(void *stateData)
{


    return g_hardware_fault;
}
void check_hardware_faults_action(void *stateData, void *newStateData)
{
    printf("Hardware fault detected! Transitioning to fault state.\n");

}



bool check_charge_critical_error( void *condition)
{
    // do some judge herr
    bool res = g_charge_critical_error;
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
    bool res = g_discharge_critical_error;
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


    return g_sleep;
}

void check_will_go_to_sleep_action( void *data, void *newStateData)
{
    printf("sleep condition fullfill, transitioning to sleep state.\n");
}



bool check_curr( void *condition)
{
    int check_case = (int)(long long int)condition;
    if(check_case == 0)
    {    
        if(abs(g_current) < BMS_CURRENT_WINDOW)
        {
            return true;
        }else{
            return false;
        }
    }else if(check_case == 1)
    {
        if(g_current > BMS_CURRENT_WINDOW)
        {
            return true;
        }else{
            return false;

        }
    }else if(check_case == 2)
    {
        if(g_current < -BMS_CURRENT_WINDOW)
        {
            return true;
        }else{
            return false;
        }
    }
    return false;
}

void check_curr_action( void *data, void *newStateData)
{
    printf(" current less than current window, go to standby\n");
}






