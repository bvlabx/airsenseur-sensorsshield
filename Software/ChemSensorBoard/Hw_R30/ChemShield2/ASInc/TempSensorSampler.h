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

#ifndef TEMPSENSORSAMPLER_H
#define	TEMPSENSORSAMPLER_H

#include "Sampler.h"
#include "SHT31.h"

class TempSensorSampler :public Sampler {
public:
    TempSensorSampler(const SHT31 &temp);
    virtual ~TempSensorSampler();
    
    virtual void setPreScaler(unsigned char value);
    virtual bool sampleTick();
    virtual bool sampleLoop();
    
    unsigned short getLastHumiditySample();
    bool getHumiditySampleReady();
    bool getHumidityChannelName(unsigned char* name);
    
    virtual bool saveChannelName(unsigned char myID, unsigned char* name);
    virtual bool getChannelName(unsigned char myID, unsigned char* buffer, unsigned char buffSize) const;

    virtual const char* getMeasurementUnit() const;
    virtual double evaluateMeasurement(unsigned short lastSample) const;

private:
    unsigned char startMeasureTime;     // we need to start sampling before reading
    bool startConversion;
    
    unsigned short lastHumiditySample;  // the SHT31 is able to provide either the humidity value
                                        // We optimized the code footprint and timing having only one
                                        // sampler that handles temperature and humidity
    bool humiditySampleReady;
    
    const SHT31 &sensor;              // the reference to the sensor
};

#endif	/* TEMPSENSORSAMPLER_H */
