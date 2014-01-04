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
private:
    bool isOpen;
};

#endif // A4Device_H
