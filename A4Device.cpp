/*
 C *opyright (c) 2014 Tomas Poledny
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

int A4Device::numberOfDevice()
{
	if(!openDevice()){
		return 0;
	}
	return a4_device_mouse_count(device);
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


