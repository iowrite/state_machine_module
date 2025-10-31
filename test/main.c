#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include "state_machine.h"
#include "main.h"

extern struct state boot;
extern struct state precharge;
extern struct state normal;
extern struct state charge;
extern struct state discharge;
extern struct state charge_protect;
extern struct state discharge_protect;
extern struct state fault;
extern struct state sleep;
extern struct state standby;

uint32_t get_tick_ms(void);

struct stateMachine BMS;


int main()
{
    printf("State Machine Module Test\n");
    stateM_init( &BMS, &normal);

    while(1)
    {
        static uint32_t last = 0;
        uint32_t now = get_tick_ms();
        if(now - last > BMS_STATE_MACHINE_PERIOD)
        {
            last = now;
            state_machine_run( &BMS);
        }
    }

    return 0;
}





uint32_t get_tick_ms(void)
{


    struct timeval tv; 
    gettimeofday(&tv, NULL);
    uint32_t now_ms = (uint32_t)(tv.tv_sec*1000+tv.tv_usec/1000);
    return now_ms;

}




