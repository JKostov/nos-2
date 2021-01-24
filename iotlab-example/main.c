#include <stdlib.h>
#include <stdio.h>
#include <shell.h>
#include <hts221_params.h>
#include <hts221_regs.h>
#include <hts221.h>

static hts221_t dev;

static int init_sensor(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (hts221_init(&dev, &hts221_params[0]) != HTS221_OK)
    {
        puts("[ERROR] Failed to initialize");
        return 1;
    }
    if (hts221_power_on(&dev) != HTS221_OK)
    {
        puts("[ERROR] Failed to power on");
        return 2;
    }
    if (hts221_set_rate(&dev, dev.p.rate) != HTS221_OK)
    {
        puts("[ERROR] Failed to set continuous mode");
        return 3;
    }

    return 0;
}

static int read_temperature(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int16_t temp;
    if (hts221_read_temperature(&dev, &temp) != HTS221_OK)
    {
        puts("[ERROR] Failed to read temperature!");
        return -1;
    }

    printf("Temperature: %d.%d\n", (temp / 10), abs(temp % 10));
    return 0;
}

const shell_command_t shell_commands[] = {
    {"init", "Sensor init", init_sensor},
    {"temp", "Reads temperature from sensor", read_temperature},
    {NULL, NULL, NULL}};

int main(void)
{
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
