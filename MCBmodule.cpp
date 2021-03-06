/*=========================================================================//

	Motor Class
	
	This class handles the control of an individual motor
	
	Designed for use with Teensy 3.1 and Motor Control Board (Rev 1.1+)
	
	
	Trevor Bruns		
		
//=========================================================================*/

#include "MCBmodule.h"
#include <stdint.h>
#include <SPI.h>
#include "LS7366R.h"
#include "PID_f32.h"
#include <math.h>

MCBmodule::MCBmodule(uint8_t csEnc)
	: enc_(csEnc) // create encoder interface
{
}

bool MCBmodule::init(float kp, float ki, float kd)
{
    // set up PID controller
    pid_.init(kp, ki, kd); 

    // set up encoder IC (LS7366R)
    int maxAttempts = 5;
    int attempts = 0;
    while (attempts < maxAttempts) {
        attempts++;
        if (enc_.init()) {
            configured_ = true;
        }
    }
    return configured_;
}

bool MCBmodule::init(void)
{
    // set up PID controller
    pid_.init();

    // set up encoder IC (LS7366R)
    int maxAttempts = 5;
    int attempts = 0;
    while (attempts < maxAttempts) {
        attempts++;
        if (enc_.init()) {
            configured_ = true;
            break;
        }
    }
    return configured_;
}

bool MCBmodule::isConfigured(void)
{
    return configured_;
}

uint16_t MCBmodule::step(void)
{
    uint16_t dacCmd; // DAC command

    int8_t polarity = 1;
    if (!motorPolarity_) {
        polarity = -1;
    }

	if (isConfigured()){
		// read current motor position and compute error
		countError_ = countDesired_ - readCount();

		// step PID controller to compute effort in Amps
		effort_ = polarity * pid_.step(float(countError_));

		// enforce voltage output bounds (typically -10V to +10V) and convert to int16 for DAC
		dacCmd = effortToDacCommand(effort_);

	}

	return dacCmd;
}

float MCBmodule::getEffort(void)
{
	return effort_;
}

void MCBmodule::restartPid(void)
{
	pid_.reset();
}

int32_t MCBmodule::getError(void)
{
	return countError_;
}

uint16_t MCBmodule::effortToDacCommand(float effort)
{
	float effortTemp = effort;

	// check for saturation
	if (effort > dacRange_[1]) { 
        effortTemp = dacRange_[1]; 
    }
	else if (effort < dacRange_[0]) { 
        effortTemp = dacRange_[0]; 
    }

    // encode effort to 16-bit DAC code
    // DAC code = (2^16)*(effort - Vmin)/(Vmax - Vmin)
    return static_cast<uint16_t>( 65535.0 * (effortTemp - dacRange_[0]) / (dacRange_[1] - dacRange_[0]) );
}

void MCBmodule::setMotorPolarity(bool polarity)
{
    motorPolarity_ = polarity;
}

void MCBmodule::setCountDesired(int32_t countDesired)
{
	
	countDesired_ = countDesired;
}

int32_t MCBmodule::getCountDesired(void)
{
	return countDesired_;
}

int32_t MCBmodule::readCount(void)
{
	// read LS7366R
	countLast_ = enc_.getCount();

	return countLast_;
}

int32_t MCBmodule::getCountLast(void)
{
	return countLast_;
}

bool MCBmodule::resetCount(void)
{
    bool success = false;
    uint8_t maxAttempts = 5; // number of reset attempts before giving up

    for (uint8_t attempt = 0; attempt < maxAttempts; attempt++) {
        // reset count register to zero
        enc_.resetCount();

        // call readCount() to make sure we update countLast_
        if (!readCount()) { // should be zero
            success = true;
            
            // prevent suddent movement upon re-enabling motor
            restartPid();
            setCountDesired(0);

            break;
        }
    }
    
    return success;
}

void MCBmodule::setGains(float kp, float ki, float kd)
{
	pid_.setGains(kp, ki, kd);
}

void MCBmodule::setKp(float kp)
{
	pid_.setKp(kp);
}

void MCBmodule::setKi(float ki)
{
	pid_.setKi(ki);
}

void MCBmodule::setKd(float kd)
{
	pid_.setKd(kd);
}

float MCBmodule::getKp(void)
{
	return pid_.getKp();
}

float MCBmodule::getKi(void)
{
	return pid_.getKi();
}

float MCBmodule::getKd(void)
{
	return pid_.getKd();
}

MCBmodule::~MCBmodule(void)
{
}
