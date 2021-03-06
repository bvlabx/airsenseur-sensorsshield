/* ===========================================================================
 * Copyright 2015 EUROPEAN UNION
 *
 * Licensed under the EUPL, Version 1.1 or subsequent versions of the
 * EUPL (the "License"); You may not use this work except in compliance
 * with the License. You may obtain a copy of the License at
 * http://ec.europa.eu/idabc/eupl
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Date: 02/04/2015
 * Authors:
 * - Michel Gerboles, michel.gerboles@jrc.ec.europa.eu, 
 *   Laurent Spinelle, laurent.spinelle@jrc.ec.europa.eu and 
 *   Alexander Kotsev, alexander.kotsev@jrc.ec.europa.eu:
 *			European Commission - Joint Research Centre, 
 * - Marco Signorini, marco.signorini@liberaintentio.com
 *
 * ===========================================================================
 */

#include "LMP91000.h"
#include "Persistence.h"
#include "EEPROMHelper.h"
#include "I2CAHelper.h"
#include "GPIOHelper.h"

/* I2C device address */
#define LMP91000_ADDRESS                0x90

#define STATUS_UNLOCK                   0x00
#define STATUS_LOCK                     0x01

LMP91000::LMP91000(const unsigned char menbPin) : m_menbPin(menbPin)  {
	AS_GPIO.digitalWrite(m_menbPin, true);
}

LMP91000::~LMP91000() {
};


bool LMP91000::lock() {
    return writeRegister(LMP91000_REG_LOCK, STATUS_LOCK);
}

bool LMP91000::unLock() {
    return writeRegister(LMP91000_REG_LOCK, STATUS_UNLOCK);
}

bool LMP91000::setGainAndLoad(unsigned char gain, unsigned char load) {
    return writeRegister(LMP91000_REG_TIACN, gain | load);
}

bool LMP91000::setReferenceZeroAndBias(unsigned char refSource, unsigned char intZero, unsigned char biasSign, unsigned char bias) {
    return writeRegister(LMP91000_REG_REFCN, refSource | intZero | biasSign | bias);
}

bool LMP91000::setModeControl(unsigned char fetShort, unsigned char opMode) {
    return writeRegister(LMP91000_REG_MODECN, fetShort | opMode);
}

bool LMP91000::writeRegisters(unsigned char tia, unsigned char ref, unsigned char mode) {
    bool result = true;
    
    result &= writeRegister(LMP91000_REG_TIACN, tia);
    result &= writeRegister(LMP91000_REG_REFCN, ref);
    result &= writeRegister(LMP91000_REG_MODECN, mode);
    
    return result;
}

bool LMP91000::readRegisters(unsigned char* tia, unsigned char* ref, unsigned char* mode) {

    bool result = true;
    
    result &= readRegister(LMP91000_REG_TIACN, tia);
    result &= readRegister(LMP91000_REG_REFCN, ref);
    result &= readRegister(LMP91000_REG_MODECN, mode);
    
    return result;
}

bool LMP91000::writeRegister(unsigned char address, unsigned char value) {

    bool result;

    AS_GPIO.digitalWrite(m_menbPin, LOW);
    result = I2CA.write(LMP91000_ADDRESS, address, 0x01, &value, 0x01);
    AS_GPIO.digitalWrite(m_menbPin, HIGH);
    
    return result;
}

bool LMP91000::readRegister(unsigned char address, unsigned char* value) {
    
    bool result = false;

    AS_GPIO.digitalWrite(m_menbPin, LOW);
    result = I2CA.read(LMP91000_ADDRESS, address, 0x01, value, 0x01);
    AS_GPIO.digitalWrite(m_menbPin, HIGH);
    
    return result;
}

bool LMP91000::storePreset() {
    
    // Store the registers
    unsigned char value;
    bool result = readRegister(LMP91000_REG_TIACN, &value);
    EEPROM.write(LMP9100_TIA(m_menbPin), value);
    
    result &= readRegister(LMP91000_REG_REFCN, &value);
    EEPROM.write(LMP9100_REF(m_menbPin), value);

    result &= readRegister(LMP91000_REG_MODECN, &value);
    EEPROM.write(LMP9100_MODE(m_menbPin), value);
        
    return result;
}

bool LMP91000::loadPreset() {
    
    unsigned char ucRead;
    bool result = true;

    unLock();
    ucRead = EEPROM.read(LMP9100_TIA(m_menbPin));
    result &= writeRegister(LMP91000_REG_TIACN, ucRead);

    ucRead = EEPROM.read(LMP9100_REF(m_menbPin));
    result &= writeRegister(LMP91000_REG_REFCN, ucRead);

    ucRead = EEPROM.read(LMP9100_MODE(m_menbPin));
    result &= writeRegister(LMP91000_REG_MODECN, ucRead);
    lock();
    
    return result;
}

