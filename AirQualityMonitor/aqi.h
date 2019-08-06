#ifndef AQI_H
#define AQI_H

/* Connect the DSM501 sensor as follow
 * https://www.elektronik.ropla.eu/pdf/stock/smy/dsm501.pdf
 * 1 green vert - Not used
 * 2 yellow jaune - Vout2 - 1 microns (PM1.0)
 * 3 white blanc - Vcc
 * 4 red rouge - Vout1 - 2.5 microns (PM2.5)
 * 5 black noir - GND
*/

#define COUNTRY                       2         // 0. France, 1. Europe, 2. USA/China
#define EXCELLENT                     "Excellent"
#define GOOD                          "Good"
#define ACCEPTABLE                    "Acceptable"
#define MODERATE                      "Moderate"
#define HEAVY                         "Heavy"
#define SEVERE                        "Severe"
#define HAZARDOUS                     "Hazardous"

struct structAQI{
  //recorder variables
  unsigned long   durationPM10;
  unsigned long   lowpulseoccupancyPM10 = 0;
  unsigned long   durationPM25;
  unsigned long   lowpulseoccupancyPM25 = 0;
  unsigned long   starttime;
  unsigned long   endtime;
  // Sensor AQI data
  float         concentrationPM25 = 0;
  float         concentrationPM10  = 0;
  int           AqiPM10            = -1;
  int           AqiPM25            = -1;
  // Indicateurs AQI - AQI display
  int           AQI                = 0;
  String        AqiString          = "";
  int           AqiColor           = 0;
};


void SetupAQISensor();
void updateAQILevel();
void performAQISensorReading();
int getATMO( int, float );
void updateAQIDisplay();
int getACQI( int, float );
float calcAQI(float, float, float, float, float);
int getAQI(int, float);

#endif
