
/**
 * @brief areaDetector driver that is a V4 neutron data client for nED.
 *
 * @author Matt Pearson
 * @date Sept 2014
 */

#ifndef ADNED_H
#define ADNED_H

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <epicsTime.h>
#include <epicsTypes.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsMutex.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <cantProceed.h>
#include <epicsTypes.h>

#include <asynOctetSyncIO.h>

#include "ADDriver.h"
#include "nEDChannel.h"

/* These are the drvInfo strings that are used to identify the parameters.
 * They are used by asyn clients, including standard asyn device support */
//System wide settings
#define ADnEDFirstParamString              "ADNED_FIRST"
#define ADnEDLastParamString               "ADNED_LAST"
#define ADnEDResetParamString              "ADNED_RESET"
#define ADnEDEventDebugParamString         "ADNED_EVENT_DEBUG"
#define ADnEDPulseCounterParamString       "ADNED_PULSE_COUNTER"
#define ADnEDPulseIDParamString            "ADNED_PULSE_ID"
#define ADnEDPChargeParamString            "ADNED_PCHARGE"
#define ADnEDPChargeIntParamString         "ADNED_PCHARGE_INT"
#define ADnEDEventUpdatePeriodParamString  "ADNED_EVENT_UPDATE_PERIOD"
#define ADnEDDetPVNameParamString          "ADNED_DET_PV_NAME"
#define ADnEDDet1PixelNumStartParamString  "ADNED_DET1_PIXEL_NUM_START"
#define ADnEDDet2PixelNumStartParamString  "ADNED_DET2_PIXEL_NUM_START"
#define ADnEDDet1PixelNumEndParamString    "ADNED_DET1_PIXEL_NUM_END"
#define ADnEDDet2PixelNumEndParamString    "ADNED_DET2_PIXEL_NUM_END"

#define ADNED_MAX_STRING_SIZE 256

extern "C" {
  int ADnEDConfig(const char *portName, int maxBuffers, size_t maxMemory, int debug);
}

namespace epics {
  namespace pvData {
    class PVStructure;
  }
}

class ADnED : public ADDriver {

 public:
  ADnED(const char *portName, int maxBuffers, size_t maxMemory, int debug);
  virtual ~ADnED();

  /* These are the methods that we override from asynPortDriver */
  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, 
                                  size_t nChars, size_t *nActual);
  virtual void report(FILE *fp, int details);

  void eventTask(void);
  void frameTask(void);
  void eventHandler(std::tr1::shared_ptr<epics::pvData::PVStructure> const &pv_struct);
  asynStatus allocArray(void); 

 private:

  //Put private functions here

  //Put private static data members here
  static const epicsInt32 s_ADNED_MAX_STRING_SIZE;

  //Put private dynamic here
  epicsUInt32 m_acquiring; 
  epicsUInt32 m_pulseCounter;
  epicsFloat64 m_pChargeInt;
  epicsTimeStamp m_nowTime;
  double m_nowTimeSecs;
  double m_lastTimeSecs;
  epicsUInt32 *p_Data;
  bool m_dataAlloc;
  epicsUInt32 m_dataMaxSize;
  epicsUInt32 m_det1Size;
  epicsUInt32 m_det2Size;

  //Constructor parameters.
  const epicsUInt32 m_debug;

  epicsEventId m_startEvent;
  epicsEventId m_stopEvent;
  epicsEventId m_startFrame;
  epicsEventId m_stopFrame;
  
  //Values used for pasynUser->reason, and indexes into the parameter library.
  int ADnEDFirstParam;
  #define ADNED_FIRST_DRIVER_COMMAND ADnEDFirstParam
  int ADnEDResetParam;
  int ADnEDEventDebugParam;
  int ADnEDPulseCounterParam;
  int ADnEDPulseIDParam;
  int ADnEDPChargeParam;
  int ADnEDPChargeIntParam;
  int ADnEDEventUpdatePeriodParam;
  int ADnEDDetPVNameParam;
  int ADnEDDet1PixelNumStartParam;
  int ADnEDDet2PixelNumStartParam;
  int ADnEDDet1PixelNumEndParam;
  int ADnEDDet2PixelNumEndParam;
  int ADnEDLastParam;
  #define ADNED_LAST_DRIVER_COMMAND ADnEDLastParam

};

#define NUM_DRIVER_PARAMS (&ADNED_LAST_DRIVER_COMMAND - &ADNED_FIRST_DRIVER_COMMAND + 1)

#endif //ADNED_H
