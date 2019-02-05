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

#include <GlobalHalHandlers.h>
#include <string.h>

#include <SerialDHelper.h>

// Singleton SerialBHelper instance
SerialDHelper SerialDHelper::instance;
uint8_t SerialDHelper::txBuffer[SERIALDBUFFERSIZE];
uint8_t SerialDHelper::rxBuffer = '\000';
volatile bool SerialDHelper::rxError = false;

SerialDHelper::SerialDHelper() {
}

SerialDHelper::~SerialDHelper() {
}

void SerialDHelper::init() const {
	HAL_UART_Receive_IT(&huart4, &rxBuffer, 1);
}

void SerialDHelper::onDataRx(bool halfBuffer) {
	putByte(rxBuffer);
	HAL_UART_Receive_IT(&huart4, &rxBuffer, 1);
}

uint16_t SerialDHelper::write(char* buffer) const {
	uint8_t len = strlen(buffer);
	return write(buffer, len);
}

uint16_t SerialDHelper::write(char* buffer, uint16_t len) const {

	len = (len > SERIALDBUFFERSIZE)? SERIALDBUFFERSIZE : len;
	uint32_t timeout = HAL_GetTick();
	while((huart4.gState != HAL_UART_STATE_READY) &&
			(HAL_GetTick() - timeout) < 1000) { };

	memcpy(txBuffer, buffer, len);
	HAL_UART_Transmit_IT(&huart4, (uint8_t*)txBuffer, len);
	return len;
}

bool SerialDHelper::available() const {
	if (rxError) {
		rxError = false;
		HAL_UART_Receive_IT(&huart4, &rxBuffer, 1);
	}

	return dataReady();
}

uint8_t SerialDHelper::read() {
	return getByte();
}

void SerialDHelper::onErrorCallback() {
	rxError = true;
}
