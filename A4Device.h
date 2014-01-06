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

#ifndef A4Device_H
#define A4Device_H
#include <libgmouse/system_a4.h>

class A4Device
{
public:
    A4Device();
    virtual ~A4Device();
    /**
     * @brief Open device
     *
     * @return bool It returns true if device is opened or false if it is not.
     */
    bool openDevice();

    /**
     * @brief Close device
     */
    void closeDevice();

    /**
     * @brief Is device opened?
     *
     * @return bool It returns true if device is opened or false if it is not.
     */
    bool isDeviceOpen() const;

    /**
     * @brief A4 device structure
     *
     */
    a4_device* device;
	
    static const int batteryWarningLevel = 85;
private:
    bool isOpen;
};

#endif // A4Device_H
