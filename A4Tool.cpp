/*
 Copyright (c) 2014 Tomas Poledny
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.
*/

#include "A4Tool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libgmouse/system_a4.h"
#include "libgmouse/pairing.h"
#include "libgmouse/control_a4.h"

bool initMultifunc(a4_device* dev)
{
    return a4_btnmask_set(dev, 0xFFBF) == A4_SUCCESS;
}

bool isInit(a4_device* dev){
	int i = a4_btnmask_get(dev);
	return (i == 65535);
}

int getProfile(a4_device* dev)
{
    int prof = a4_profile_get(dev);

    if (prof == A4_ERROR) {
        fprintf(stderr, "IO Error\n");
        return EXIT_FAILURE;
    }
}

bool setProfile(a4_device* dev, int profile)
{
    if (profile < 0 || profile > 4) {
        return false;
    }

    usleep(LITLE_SLEEP);

    if (a4_profile_set(dev, profile) == A4_SUCCESS)
        return true;

    return false;
}

int getSignal(a4_device* dev)
{
    int signal = (int)((255.0 - (float)a4_rf_get_signal_level(dev)) / 2.55);
    return signal;
}

int getBatteryMouse(a4_device* dev)
{
    if (a4_device_mouse_count(dev) > 0) {
        return a4_power_mouse_get(dev);
    }
    return A4_ERROR;
}

int getBatteryKeyboard(a4_device* dev)
{
    if (a4_device_keybd_count(dev) > 0) {
        return a4_power_keybd_get(dev);
    }
    return A4_ERROR;
}


int getMrr(a4_device* dev)
{
    return a4_mrr_get(dev);
}

bool setMrr(a4_device* dev, int mrr)
{
    if (mrr != 125 && mrr != 250 && mrr != 500) {
        return false;
    }

    usleep(LITLE_SLEEP);

    if (a4_mrr_set(dev, mrr) == A4_SUCCESS) {
        return true;
    }
    return false;
}

int getWakeUpTime(a4_device* dev)
{
    a4_wake_mode mod = a4_wake_get_mode(dev);
    if (mod.type == A4_ERROR) {
        return A4_ERROR;
    }
    return mod.time;
}

WakeAfter getWakeUpBy(a4_device* dev)
{
    a4_wake_mode mod = a4_wake_get_mode(dev);
    if (mod.type == A4_ERROR) {
        return UNKNOWN;
    }
    if (mod.type == CLICK) {
        return CLICK;
    } else if (mod.type == MOVE) {
        return MOVE;
    }
    return UNKNOWN;
}

bool setWakeUp(a4_device* dev, int time, WakeAfter after)
{
    a4_wake_mode mod;

    if (after == CLICK)
        mod.type = A4_WAKE_BY_CLICK;
    else if (after == MOVE)
        mod.type = A4_WAKE_BY_MOVE;
    else {
        return false;
    }


    if (time < 3 || time > 10) {
        return false;
    }

    usleep(LITLE_SLEEP);

    mod.time = time;

    if (a4_wake_set_mode(dev, mod) == A4_SUCCESS) {
        return true;
    }
    return false;
}

int getDistance(a4_device* dev)
{
    return a4_rf_get_distance(dev);
}

bool setDistance(a4_device* dev, int idx)
{
    if (idx != 15 && idx != 20) {
        return false;
    }

    usleep(LITLE_SLEEP);

    if (a4_rf_set_distance(dev, idx) == A4_SUCCESS) {
        return true;
    }
    return false;
}

int getChannel(a4_device* dev)
{
    a4_rf_channel chn = a4_rf_get_channel(dev);
    if (chn.type != A4_ERROR) {
        return chn.channel;
    }
    return A4_ERROR;
}

ChannelMod getChannelMode(a4_device* dev)
{
    a4_rf_channel chn = a4_rf_get_channel(dev);
    if (chn.type == A4_ERROR) {
        return ERROR;
    } else if (chn.type == AUTO) {
        return AUTO;
    } else if (chn.type == MANUAL) {
        return MANUAL;
    }
    return ERROR;
}

bool setChannel(a4_device* dev, int channel, ChannelMod mode)
{
    if (mode == ERROR) {
        return false;
    }

    if (mode == AUTO)
        channel = 0;


    if (channel < 0 || channel > 14) {
        return false;
    }

    usleep(LITLE_SLEEP);

    if (a4_rf_set_channel(dev, channel) == A4_SUCCESS)
        return true;
    return false;
}

a4_device* getDevice()
{
    a4_device* dev = a4_open_device();
    return dev;
}

//TODO

int last = -1;

void progress_print(int prog)
{
    if (((prog % 10) == 0) && (prog != last)) {
        last = prog;
        printf("%d %%\n", prog);
    }
}

int dump_func(a4_device* dev, int argc, char* argv[])
{
    if (argc > 0) {
        if (a4_dump(dev, argv[0], progress_print) == A4_SUCCESS)
            printf("Success\n");
        else
            printf("Fail\n");
    } else {
        fprintf(stderr, "No file specified\n");
        return EXIT_FAILURE;
    }

    return 0;
}



int pair_func(a4_device* dev, int argc, char* argv[])
{
    if (argc > 0) {
        if (strcmp(argv[0], "list") == 0) {
            printf("Device list:\n\n");
            a4_pair_devlist lst = a4_pair_get_list_mouse(dev);
            printf("Mouse:\n");
            for (int i = 0; i < 5; i++)
                if (lst.ID[i] != A4_PAIR_NONE) {
                    printf("\t%.8X", lst.ID[i]);
                    if (lst.disabled[i])
                        printf(" disabled\n");
                    else
                        printf("\n");
                }
            lst = a4_pair_get_list_keyboard(dev);
            printf("\nKeyboard:\n");
            for (int i = 0; i < 5; i++)
                if (lst.ID[i] != A4_PAIR_NONE) {
                    printf("\t%.8X", lst.ID[i]);
                    if (lst.disabled[i])
                        printf(" disabled\n");
                    else
                        printf("\n");
                }
        } else if (strcmp(argv[0], "new") == 0) {
            printf("Adding new device\n\nSwitch power on new device to OFF(if it in ON), and switch it ON.\n");
            printf("Waiting for new device...\n");

            int count = 0;

            a4_pair_set_find_mode(dev, A4_PAIR_FIND_ON);

            int result = A4_PAIR_MOD_OK;

            while (count < 60) {
                a4_pair_device device = a4_pair_get_new_device(dev);

                if (device.type != A4_PAIR_NONE) {
                    a4_pair_set_find_mode(dev, A4_PAIR_FIND_OFF);

                    if (device.type == A4_PAIR_MOUSE) {
                        printf("Got mouse with ID %.8X\n", device.ID);
                        printf("\nAdding to dongle...\n");

                        result = a4_pair_add_new_device(dev, device);
                    } else if (device.type == A4_PAIR_KBD) {
                        printf("Got keyboard with ID %.8X\n", device.ID);
                        printf("\nAdding to dongle...\n");

                        result = a4_pair_add_new_device(dev, device);
                    }

                    break;
                }

                count++;
                sleep(1);
            }

            if (count >= 60) {
                a4_pair_set_find_mode(dev, A4_PAIR_FIND_OFF);
                printf("Time out.\n");
                return EXIT_FAILURE;
            }

            printf("%s.\n", a4_pair_get_error_string(result));

            if (result != A4_PAIR_MOD_OK)
                return EXIT_FAILURE;
        } else if (strcmp(argv[0], "del") == 0) {
            if (argc < 2) {
                fprintf(stderr, "No ID specified\n");
                return EXIT_FAILURE;
            }

            unsigned int ID = 0;

            int res = sscanf(argv[1], "%x", &ID);
            if (res != 1 || ID == 0) {
                fprintf(stderr, "Invalid ID\n");
                return EXIT_FAILURE;
            }

            printf("Deleting device with ID: %.8X ...\n", ID);
            res = a4_pair_del_device_by_id(dev, ID);

            printf("%s.\n", a4_pair_get_error_string(res));

            if (res != A4_PAIR_MOD_OK)
                return EXIT_FAILURE;
        } else if (strcmp(argv[0], "disable") == 0) {
            if (argc < 2) {
                fprintf(stderr, "No ID specified\n");
                return EXIT_FAILURE;
            }

            unsigned int ID = 0;

            int res = sscanf(argv[1], "%x", &ID);
            if (res != 1 || ID == 0) {
                fprintf(stderr, "Invalid ID\n");
                return EXIT_FAILURE;
            }

            printf("Disabling device with ID: %.8X ...\n", ID);
            res = a4_pair_switch_device_by_id(dev, ID, A4_PAIR_STATE_DISABLE);

            printf("%s.\n", a4_pair_get_error_string(res));

            if (res != A4_PAIR_MOD_OK)
                return EXIT_FAILURE;
        } else if (strcmp(argv[0], "enable") == 0) {
            if (argc < 2) {
                fprintf(stderr, "No ID specified\n");
                return EXIT_FAILURE;
            }

            unsigned int ID = 0;

            int res = sscanf(argv[1], "%x", &ID);
            if (res != 1 || ID == 0) {
                fprintf(stderr, "Invalid ID\n");
                return EXIT_FAILURE;
            }

            printf("Enabling device with ID: %.8X ...\n", ID);
            res = a4_pair_switch_device_by_id(dev, ID, A4_PAIR_STATE_ENABLE);

            printf("%s.\n", a4_pair_get_error_string(res));

            if (res != A4_PAIR_MOD_OK)
                return EXIT_FAILURE;
        } else {
            fprintf(stderr, "Unknown option \"%s\"\n", argv[0]);
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "No options specified\n");
        return EXIT_FAILURE;
    }

    return 0;
}