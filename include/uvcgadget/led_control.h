#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int find_led_controller_pid()
{
    if (0 == system("ps aux | grep 'led_controller.py' | grep 'sudo' | awk '{print $2}'"))
    {
        printf("led_controller.py found\n");
        FILE *fptr = fopen("/tmp/led_controller_pid", "r");
        char pid[100];
        fgets(pid, 100, fptr);
        printf("%s\n", pid); // DEBUG
        return atoi(pid);
    }
    else
    {
        printf("led_controller.py not found\n");
        return -1;
    }
}

void turn_led_on()
{
    int led_controller_pid = find_led_controller_pid();
    if (led_controller_pid > 0)
    {
        kill(led_controller_pid, SIGUSR2);
    }
}

void turn_led_off()
{
    int led_controller_pid = find_led_controller_pid();
    if (led_controller_pid > 0)
    {
        kill(led_controller_pid, SIGUSR2);
    }
}