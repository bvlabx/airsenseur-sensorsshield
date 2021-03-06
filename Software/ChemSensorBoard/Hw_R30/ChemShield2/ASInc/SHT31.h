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
 *      European Commission - Joint Research Centre, 
 * - Marco Signorini, marco.signorini@liberaintentio.com
 *
 * ===========================================================================
 */

#ifndef _SHT31_H_
#define _SHT31_H_

class SHT31 {

  private:
    SHT31();
  public:
    SHT31(bool internal);
    virtual ~SHT31();

    // Initialize the sensor and reads
    bool begin();
    
    bool startConvertion() const;
    bool getSamples(unsigned short *temperature, unsigned short *humidity) const;

    bool isAvailable() const;

    bool getTemperatureChannelName(unsigned char* buffer) const;
    bool getHumidityChannelName(unsigned char* buffer) const;
      
  private:
    char sendCommand(unsigned short command) const;
    char readData(unsigned short *temperature, unsigned short *humidity) const;

  private:
    char sensorAddress;
    bool available;
};

#endif /* _SHT31_H_ */


