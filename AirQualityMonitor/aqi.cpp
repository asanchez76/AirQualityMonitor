//AQI sensor code and calculations goes here
#include "Arduino.h"
#include "aqi.h"
#include "hardwarepins.h"
#include "display.h"
#include "network.h"

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
struct structAQI AQI;
unsigned long   aqi_sampletime_ms = AQI_REFRESH;  // sample time (ms)

void refreshAQISensor()
{
	AQI.lowpulseoccupancyPM10 += pulseIn(DUST_SENSOR_DIGITAL_PIN_PM10, LOW);
	AQI.lowpulseoccupancyPM25 += pulseIn(DUST_SENSOR_DIGITAL_PIN_PM25, LOW);
}

void InitAQISensor()
{
	pinMode(DUST_SENSOR_DIGITAL_PIN_PM10,INPUT);
	pinMode(DUST_SENSOR_DIGITAL_PIN_PM25,INPUT);
	// wait 60s for DSM501 to warm up
	Serial.println("[AQI]Please wait 10s for DSM501 to warm up");
	LCDPrint("Sensor warmup:");
	char charBuf[10];
	for (int i = 10; i >= 1; i--)
	{
		snprintf(charBuf, 10, "Wait %d", i);
		Serial.print(".");
		LCDPrint("",charBuf, true, PRIORITY);
		delay(1000); // 1s
	}

	Serial.println("[AQI]Ready!");
	AQI.starttime = millis();
}

void updateAQILevel(){
  AQI.AQI = AQI.AqiPM10;
}

void performAQISensorReading() {
  Serial.println("performAQISensorReading function called");
  //update measurements
  AQI.endtime = millis();
  float ratio = AQI.lowpulseoccupancyPM10 / (aqi_sampletime_ms * 10.0);
  float concentration = 1.1 * pow( ratio, 3) - 3.8 *pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
  if ( aqi_sampletime_ms < 3600000 ) { concentration = concentration * ( aqi_sampletime_ms / 3600000.0 ); }
  AQI.lowpulseoccupancyPM10 = 0;
  AQI.concentrationPM10 = concentration;

  ratio = AQI.lowpulseoccupancyPM25 / (aqi_sampletime_ms * 10.0);
  concentration = 1.1 * pow( ratio, 3) - 3.8 *pow(ratio, 2) + 520 * ratio + 0.62;
  if ( aqi_sampletime_ms < 3600000 ) { concentration = concentration * ( aqi_sampletime_ms / 3600000.0 ); }
  AQI.lowpulseoccupancyPM25 = 0;
  AQI.concentrationPM25 = concentration;

  Serial.print("[AQI]Concentrations => PM2.5: "); Serial.print(AQI.concentrationPM25); Serial.print(" | PM10: "); Serial.println(AQI.concentrationPM10);

  AQI.starttime = millis();

  //update AQI for each sensor
  if ( COUNTRY == 0 ) {
    // France
    AQI.AqiPM25 = getATMO( 0, AQI.concentrationPM25 );
    AQI.AqiPM10 = getATMO( 1, AQI.concentrationPM10 );
  } else if ( COUNTRY == 1 ) {
    // Europe
    AQI.AqiPM25 = getACQI( 0, AQI.concentrationPM25 );
    AQI.AqiPM10 = getACQI( 1, AQI.concentrationPM10 );
  } else {
    // USA / China
    AQI.AqiPM25 = getAQI( 0, AQI.concentrationPM25 );
    AQI.AqiPM10 = getAQI( 0, AQI.concentrationPM10 );
  }

  //update AQI index
  updateAQILevel();
  updateAQIDisplay();

  Serial.print("[AQI]AQIs => PM25: "); Serial.print(AQI.AqiPM25); Serial.print(" | PM10: "); Serial.println(AQI.AqiPM10);
  Serial.print(" | AQI: "); Serial.println(AQI.AQI); Serial.print(" | Message: "); Serial.println(AQI.AqiString);
  setIoTField(3,AQI.AqiPM25);
  setIoTField(4,AQI.AqiPM10);
  setIoTField(5,AQI.AQI);
  setIoTStatus(AQI.AqiString);

  char line1Buf[17], line2Buf[17];
  snprintf(line1Buf, 17, "PM25:%d PM10:%d", AQI.AqiPM25, AQI.AqiPM10);
  snprintf(line2Buf, 17, "Cond:%s",AQI.AqiString);
  LCDPrint(line1Buf,line2Buf,false, AIR_QUALITY);

}


/*
 * Calculate French ATMO AQI indicator
 */
int getATMO( int sensor, float density ){
  if ( sensor == 0 ) { //PM2,5
    if ( density <= 11 ) {
      return 1;
    } else if ( density > 11 && density <= 24 ) {
      return 2;
    } else if ( density > 24 && density <= 36 ) {
      return 3;
    } else if ( density > 36 && density <= 41 ) {
      return 4;
    } else if ( density > 41 && density <= 47 ) {
      return 5;
    } else if ( density > 47 && density <= 53 ) {
      return 6;
    } else if ( density > 53 && density <= 58 ) {
      return 7;
    } else if ( density > 58 && density <= 64 ) {
      return 8;
    } else if ( density > 64 && density <= 69 ) {
      return 9;
    } else {
      return 10;
    }
  } else {
    if ( density <= 6 ) {
      return 1;
    } else if ( density > 6 && density <= 13 ) {
      return 2;
    } else if ( density > 13 && density <= 20 ) {
      return 3;
    } else if ( density > 20 && density <= 27 ) {
      return 4;
    } else if ( density > 27 && density <= 34 ) {
      return 5;
    } else if ( density > 34 && density <= 41 ) {
      return 6;
    } else if ( density > 41 && density <= 49 ) {
      return 7;
    } else if ( density > 49 && density <= 64 ) {
      return 8;
    } else if ( density > 64 && density <= 79 ) {
      return 9;
    } else {
      return 10;
    }
  }
}

void updateAQIDisplay(){
  /*
   * 1 EXCELLENT
   * 2 GOOD
   * 3 ACCEPTABLE
   * 4 MODERATE
   * 5 HEAVY
   * 6 SEVERE
   * 7 HAZARDOUS
   */
  if ( COUNTRY == 0 ) {
    // Système ATMO français - French ATMO AQI system
    switch ( AQI.AQI) {
      case 10:
        AQI.AqiString = SEVERE;
        break;
      case 9:
        AQI.AqiString = HEAVY;
        break;
      case 8:
        AQI.AqiString = HEAVY;
        break;
      case 7:
        AQI.AqiString = MODERATE;
        break;
      case 6:
        AQI.AqiString = MODERATE;
        break;
      case 5:
        AQI.AqiString = ACCEPTABLE;
        break;
      case 4:
        AQI.AqiString = GOOD;
        break;
      case 3:
        AQI.AqiString = GOOD;
        break;
      case 2:
        AQI.AqiString = EXCELLENT;
        break;
      case 1:
        AQI.AqiString = EXCELLENT;
        break;
      }
  } else if ( COUNTRY == 1 ) {
    // European CAQI
    switch ( AQI.AQI) {
      case 25:
        AQI.AqiString = GOOD;
        break;
      case 50:
        AQI.AqiString = ACCEPTABLE;
        break;
      case 75:
        AQI.AqiString = MODERATE;
        break;
      case 100:
        AQI.AqiString = HEAVY;
        break;
      default:
        AQI.AqiString = SEVERE;
      }
  } else if ( COUNTRY == 2 ) {
    // USA / CN
    if ( AQI.AQI <= 50 ) {
        AQI.AqiString = GOOD;
    } else if ( AQI.AQI > 50 && AQI.AQI <= 100 ) {
        AQI.AqiString = ACCEPTABLE;
    } else if ( AQI.AQI > 100 && AQI.AQI <= 150 ) {
        AQI.AqiString = MODERATE;
    } else if ( AQI.AQI > 150 && AQI.AQI <= 200 ) {
        AQI.AqiString = HEAVY;
    } else if ( AQI.AQI > 200 && AQI.AQI <= 300 ) {
        AQI.AqiString = SEVERE;
    } else {
       AQI.AqiString = HAZARDOUS;
    }
  }
}
/*
 * CAQI Européen - European CAQI level
 * source : http://www.airqualitynow.eu/about_indices_definition.php
 */

int getACQI( int sensor, float density ){
  if ( sensor == 0 ) {  //PM2,5
    if ( density == 0 ) {
      return 0;
    } else if ( density <= 15 ) {
      return 25 ;
    } else if ( density > 15 && density <= 30 ) {
      return 50;
    } else if ( density > 30 && density <= 55 ) {
      return 75;
    } else if ( density > 55 && density <= 110 ) {
      return 100;
    } else {
      return 150;
    }
  } else {              //PM10
    if ( density == 0 ) {
      return 0;
    } else if ( density <= 25 ) {
      return 25 ;
    } else if ( density > 25 && density <= 50 ) {
      return 50;
    } else if ( density > 50 && density <= 90 ) {
      return 75;
    } else if ( density > 90 && density <= 180 ) {
      return 100;
    } else {
      return 150;
    }
  }
}

/*
 * AQI formula: https://en.wikipedia.org/wiki/Air_Quality_Index#United_States
 * Arduino code https://gist.github.com/nfjinjing/8d63012c18feea3ed04e
 * On line AQI calculator https://www.airnow.gov/index.cfm?action=resources.conc_aqi_calc
 */
float calcAQI(float I_high, float I_low, float C_high, float C_low, float C) {
  return (I_high - I_low) * (C - C_low) / (C_high - C_low) + I_low;
}

int getAQI(int sensor, float density) {
  int d10 = (int)(density * 10);
  if ( sensor == 0 ) {
    if (d10 <= 0) {
      return 0;
    }
    else if(d10 <= 120) {
      return calcAQI(50, 0, 120, 0, d10);
    }
    else if (d10 <= 354) {
      return calcAQI(100, 51, 354, 121, d10);
    }
    else if (d10 <= 554) {
      return calcAQI(150, 101, 554, 355, d10);
    }
    else if (d10 <= 1504) {
      return calcAQI(200, 151, 1504, 555, d10);
    }
    else if (d10 <= 2504) {
      return calcAQI(300, 201, 2504, 1505, d10);
    }
    else if (d10 <= 3504) {
      return calcAQI(400, 301, 3504, 2505, d10);
    }
    else if (d10 <= 5004) {
      return calcAQI(500, 401, 5004, 3505, d10);
    }
    else if (d10 <= 10000) {
      return calcAQI(1000, 501, 10000, 5005, d10);
    }
    else {
      return 1001;
    }
  } else {
    if (d10 <= 0) {
      return 0;
    }
    else if(d10 <= 540) {
      return calcAQI(50, 0, 540, 0, d10);
    }
    else if (d10 <= 1540) {
      return calcAQI(100, 51, 1540, 541, d10);
    }
    else if (d10 <= 2540) {
      return calcAQI(150, 101, 2540, 1541, d10);
    }
    else if (d10 <= 3550) {
      return calcAQI(200, 151, 3550, 2541, d10);
    }
    else if (d10 <= 4250) {
      return calcAQI(300, 201, 4250, 3551, d10);
    }
    else if (d10 <= 5050) {
      return calcAQI(400, 301, 5050, 4251, d10);
    }
    else if (d10 <= 6050) {
      return calcAQI(500, 401, 6050, 5051, d10);
    }
    else {
      return 1001;
    }
  }
}
