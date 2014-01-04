#include "A4Device.h"

A4Device::A4Device() : isOpen(false), device(NULL)
{

}

A4Device::~A4Device()
{
	if(isOpen){
		closeDevice();
	}
}

bool A4Device::openDevice()
{
    if (!isOpen) {
        device = a4_open_device();
        if (device != NULL) {
            isOpen = true;
        }
    }
    return isOpen;
}

void A4Device::closeDevice()
{
    if (isOpen) {
        a4_close_device(device);
        device = NULL;
        isOpen = false;
    }
}

bool A4Device::isDeviceOpen() const
{
    return isOpen;
}


