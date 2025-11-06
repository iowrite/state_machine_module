#ifndef _MAIN_H
#define _MAIN_H 


#define BMS_STATE_MACHINE_PERIOD            1000

#define BMS_CURRENT_WINDOW                  2

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


extern int g_current;
extern int g_hardware_fault;
extern int g_sleep;
extern int g_charge_critical_error;
extern int g_discharge_critical_error;
extern int g_precharge_pass;
extern int g_poweron_CMD;

#endif 