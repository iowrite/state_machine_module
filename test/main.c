#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include "state_machine.h"
#include "main.h"

extern struct state boot;
extern struct state precharge;
extern struct state run;
extern struct state charge;
extern struct state discharge;
extern struct state charge_protect;
extern struct state discharge_protect;
extern struct state fault;
extern struct state sleep;
extern struct state standby;

uint32_t get_tick_ms(void);

struct stateMachine BMS;


// event variables
int g_current = 0;
int g_hardware_fault = 0;
int g_sleep = 0;
int g_charge_critical_error = 0;
int g_discharge_critical_error = 0;
int g_precharge_pass = 0;

static void on_activate (GtkApplication *app) {
  // Create a new window
  GtkWidget *window = gtk_application_window_new (app);
  // Create a new button
  GtkWidget *button = gtk_button_new_with_label ("Hello, World!");
  // When the button is clicked, close the window passed as an argument
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_close), window);
  gtk_window_set_child (GTK_WINDOW (window), button);
  gtk_window_present (GTK_WINDOW (window));
}


// GUI 线程函数
void* gui_thread_func(void *data)
{
  // Create a new application
  GtkApplication *app = gtk_application_new ("com.example.GtkApplication",
                                             G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  return g_application_run (G_APPLICATION (app), NULL, NULL);
}





int main()
{
    printf("State Machine Module Test\n");
    state_machine_init( &BMS, &run);
    pthread_t gui_thread;
    if (pthread_create(&gui_thread, NULL, gui_thread_func, NULL) != 0) {
        fprintf(stderr, "Failed to create GUI thread\n");
        return -1;
    }
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




