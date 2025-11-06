#include "main.h"
#include "glib.h"
#include "state_machine.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

extern struct state boot_state;
extern struct state precharge_state;
extern struct state run_state;
extern struct state charge_state;
extern struct state discharge_state;
extern struct state charge_protect_state;
extern struct state discharge_protect_state;
extern struct state fault_state;
extern struct state sleep_state;
extern struct state standby_state;

uint32_t get_tick_ms(void);

struct stateMachine BMS;

// event variables
int g_current = 0;
int g_hardware_fault = 0;
int g_sleep = 0;
int g_charge_critical_error = 0;
int g_discharge_critical_error = 0;
int g_precharge_pass = 0;
int g_poweron_CMD = 0;

// 线程执行函数
void *state_thread_func(void *arg) {
  state_machine_init(&BMS, &run_state);
  while (1) {

    state_machine_run(&BMS);
    sleep(1);
  }
  return NULL;
}

GtkWidget *button_hardware_fault;
GtkWidget *button_charge_critical_alarm;
GtkWidget *button_discharge_critical_alarm;
GtkWidget *button_sleep;
GtkWidget *button_precharge;
GtkWidget *scale_cur;

static void print_hello(GtkWidget *widget, gpointer data) {
  g_print("event trigger: ");
  if (widget == button_hardware_fault) {
    g_print("hardware fault signal\n");
    g_hardware_fault =
        gtk_check_button_get_active(GTK_CHECK_BUTTON(button_hardware_fault));
    g_print("hardware fault value:%d\n", g_hardware_fault);
  } else if (widget == button_charge_critical_alarm) {
    g_print("charge critical alarm signal\n");
    g_charge_critical_error = gtk_check_button_get_active(
        GTK_CHECK_BUTTON(button_charge_critical_alarm));
    g_print("charge critical alarm value:%d\n", g_charge_critical_error);
  } else if (widget == button_discharge_critical_alarm) {
    g_print("discharge critical alarm signal\n");
    g_discharge_critical_error = gtk_check_button_get_active(
        GTK_CHECK_BUTTON(button_discharge_critical_alarm));
    g_print("discharge critical alarm value:%d\n", g_discharge_critical_error);
  } else if (widget == button_sleep) {
    g_print("sleep signal\n");
    g_sleep = gtk_check_button_get_active(GTK_CHECK_BUTTON(button_sleep));
    g_print("sleep value:%d\n", g_sleep);
  } else if (widget == button_precharge) {
    g_print("precharge signal\n");
    g_precharge_pass =
        gtk_check_button_get_active(GTK_CHECK_BUTTON(button_precharge));
    g_print("sleep value:%d\n", g_precharge_pass);
  } else if (widget == scale_cur) {
    g_print("current signal\n");
    g_current = gtk_range_get_value(GTK_RANGE(widget));
    g_print("current value:%d\n", g_current);
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hello");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);

  gtk_window_set_child(GTK_WINDOW(window), box);

  GtkWidget *head = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  GtkWidget *foot = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  button_hardware_fault =
      gtk_check_button_new_with_label("hardware fault signal");
  g_signal_connect(button_hardware_fault, "toggled", G_CALLBACK(print_hello),
                   NULL);

  button_charge_critical_alarm =
      gtk_check_button_new_with_label("charge critial alarm signal");
  g_signal_connect(button_charge_critical_alarm, "toggled",
                   G_CALLBACK(print_hello), NULL);

  button_discharge_critical_alarm =
      gtk_check_button_new_with_label("discharge critial alarm signal");
  g_signal_connect(button_discharge_critical_alarm, "toggled",
                   G_CALLBACK(print_hello), NULL);

  button_sleep = gtk_check_button_new_with_label("sleep signal");
  g_signal_connect(button_sleep, "toggled", G_CALLBACK(print_hello), NULL);

  button_precharge = gtk_check_button_new_with_label("precharge pass signal");
  g_signal_connect(button_precharge, "toggled", G_CALLBACK(print_hello), NULL);

  scale_cur = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -10, 10, 1);
  gtk_scale_set_draw_value(GTK_SCALE(scale_cur), true);
  gtk_scale_set_has_origin(GTK_SCALE(scale_cur), false);
  gtk_scale_add_mark(GTK_SCALE(scale_cur), 0, GTK_POS_BOTTOM, "0");
  gtk_scale_add_mark(GTK_SCALE(scale_cur), -3, GTK_POS_BOTTOM, "-3");
  gtk_scale_add_mark(GTK_SCALE(scale_cur), 3, GTK_POS_BOTTOM, "3");
  gtk_range_set_value(GTK_RANGE(scale_cur), 0);
  g_signal_connect(scale_cur, "value-changed", G_CALLBACK(print_hello), NULL);

  gtk_box_append(GTK_BOX(box), head);
  gtk_box_append(GTK_BOX(box), button_hardware_fault);
  gtk_box_append(GTK_BOX(box), button_charge_critical_alarm);
  gtk_box_append(GTK_BOX(box), button_discharge_critical_alarm);
  gtk_box_append(GTK_BOX(box), button_sleep);
  gtk_box_append(GTK_BOX(box), button_precharge);
  gtk_box_append(GTK_BOX(box), scale_cur);
  gtk_box_append(GTK_BOX(box), foot);

  gtk_window_present(GTK_WINDOW(window));
}
int main(int argc, char **argv) {
  printf("State Machine Module Test\n");

  pthread_t state_thread;
  if (pthread_create(&state_thread, NULL, state_thread_func, NULL) != 0) {
    fprintf(stderr, "Failed to create state machine thread\n");
    return -1;
  }

  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

uint32_t get_tick_ms(void) {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint32_t now_ms = (uint32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
  return now_ms;
}
