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

#ifndef SAMPLER_H
#define	SAMPLER_H

#define IIR1    0
#define IIR2    1

class DitherTool;

/* This is the base class for a sampler unit. 
   A sampler unit implements a basic sampler with prescaler, IIR and decimation timing option 
*/
class Sampler {
    
public:    
    Sampler();
    virtual ~Sampler() { };
    
    virtual void setPreScaler(unsigned char value);
    virtual unsigned char getPrescaler();
    
    virtual void setDecimation(unsigned char value);
    virtual unsigned char getDecimation();
    
    virtual void setIIRDenom(unsigned char iIRID, unsigned char value);
    virtual unsigned char getIIRDenom(unsigned char iIRID);
    
    virtual void setDitherTool(DitherTool* tool);
    
    virtual bool savePreset(unsigned char myID);
    virtual bool loadPreset(unsigned char myID);
    
    virtual bool sampleTick() = 0;
    virtual bool sampleLoop() = 0;
    
    virtual unsigned short getLastSample();
    
protected:
    virtual bool applyDecimationFilter();
    virtual void applyIIRFilter(unsigned char iiRID);
    virtual void onReadSample(unsigned short newSample);
    
protected:
    volatile bool  go;                   // it's time for a new sample (shared info from interrupt)
    unsigned char  prescaler;            // basic samples are taken at sampleTick/prescaler
    unsigned char  timer;                // this is the prescaler counter
    
    unsigned char  decimation;           // decimation filter length
    unsigned char  decimationTimer;      // this is the decimation counter
    unsigned short workSample;           // working register for FIR filtering
    unsigned short lastSample;           // last valid sample read

    unsigned char  iIRDenum[2];          // The IIRs denominators
    double         iIRAccumulator[2];    // The IIRs temporary variable

    unsigned char  blankTimer;           // Generates a basic data not-validity period when FIR starts
    
    static DitherTool* ditherTool;       // A static pointer to a singleton dithering tool object
};

#endif	/* SAMPLER_H */
