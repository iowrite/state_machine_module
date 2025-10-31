#include <stdbool.h>
#ifndef  _TRANSITIONS_H
#define  _TRANSITIONS_H



bool check_hardware_faults(void *stateData);
void check_hardware_faults_action(void *stateData, void *newStateData);

bool check_charge_critical_error( void *condition);
void check_charge_critical_error_action( void *data, void *newStateData);

bool check_discharge_critical_error( void *condition);
void check_discharge_critical_error_action( void *data, void *newStateData);


bool check_will_go_to_sleep( void *condition);
void check_will_go_to_sleep_action( void *data, void *newStateData);

#endif