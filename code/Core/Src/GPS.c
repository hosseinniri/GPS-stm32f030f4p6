#include "GPSConfig.h"
#include "GPS.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <main.h>

GPS_t GPS;
//##################################################################################################################
double convertDegMinToDecDeg (float degMin)
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}
//##################################################################################################################
void	GPS_Init(void)
{
	GPS.rxIndex=0;
	HAL_UART_Receive_IT(&_GPS_USART, &GPS.rxTmp, 1);
}
//##################################################################################################################
void	GPS_CallBack(void)
{
	GPS.LastTime=HAL_GetTick();
	if(GPS.rxIndex < sizeof(GPS.rxBuffer)-2)
	{
		GPS.rxBuffer[GPS.rxIndex] = GPS.rxTmp;
		GPS.rxIndex++;
	}	
	HAL_UART_Receive_IT(&_GPS_USART,&GPS.rxTmp,1);
}
//##################################################################################################################
void	GNGAA_Process(void)
{
	if( (HAL_GetTick()-GPS.LastTime>50) && (GPS.rxIndex>0))
	{
		char	*str;
		#if (_GPS_DEBUG==1)
		printf("%s",GPS.rxBuffer);
		#endif
		str=strstr((char*)GPS.rxBuffer,"$GNGGA,");
		if(str!=NULL)
		{
			memset(&GPS.GNGGA,0,sizeof(GPS.GNGGA));
			sscanf(str,"$GNGGA,%2hhd%2hhd%2hhd.%3hd,%f,%c,%f,%c,%hhd,%hhd,%f,%f,%c,%hd,%s,*%2s\r\n",&GPS.GNGGA.UTC_Hour,&GPS.GNGGA.UTC_Min,&GPS.GNGGA.UTC_Sec,&GPS.GNGGA.UTC_MicroSec,&GPS.GNGGA.Latitude,&GPS.GNGGA.NS_Indicator,&GPS.GNGGA.Longitude,&GPS.GNGGA.EW_Indicator,&GPS.GNGGA.PositionFixIndicator,&GPS.GNGGA.SatellitesUsed,&GPS.GNGGA.HDOP,&GPS.GNGGA.MSL_Altitude,&GPS.GNGGA.MSL_Units,&GPS.GNGGA.AgeofDiffCorr,GPS.GNGGA.DiffRefStationID,GPS.GNGGA.CheckSum);
			if(GPS.GNGGA.NS_Indicator==0)
				GPS.GNGGA.NS_Indicator='-';
			if(GPS.GNGGA.EW_Indicator==0)
				GPS.GNGGA.EW_Indicator='-';
			if(GPS.GNGGA.Geoid_Units==0)
				GPS.GNGGA.Geoid_Units='-';
			if(GPS.GNGGA.MSL_Units==0)
				GPS.GNGGA.MSL_Units='-';
			GPS.GNGGA.LatitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Latitude);
			GPS.GNGGA.LongitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Longitude);			
		}		
		memset(GPS.rxBuffer,0,sizeof(GPS.rxBuffer));
		GPS.rxIndex=0;
	}
	HAL_UART_Receive_IT(&_GPS_USART,&GPS.rxTmp,1);
}
//******************************************************************************************************************
void	GNRMC_Process(void)
{
	if( (HAL_GetTick()-GPS.LastTime>50) && (GPS.rxIndex>0))
	{
		char	*str;
		#if (_GPS_DEBUG==1)
		printf("%s",GPS.rxBuffer);
		#endif
		str=strstr((char*)GPS.rxBuffer,"$GNGGA,");
		if(str!=NULL)
		{
			memset(&GPS.GNGGA,0,sizeof(GPS.GNGGA));
			sscanf(str,"$GNGGA,%2hhd%2hhd%2hhd.%3hd,%f,%c,%f,%c,%hhd,%hhd,%f,%f,%c,%hd,%s,*%2s\r\n",&GPS.GNGGA.UTC_Hour,&GPS.GNGGA.UTC_Min,&GPS.GNGGA.UTC_Sec,&GPS.GNGGA.UTC_MicroSec,&GPS.GNGGA.Latitude,&GPS.GNGGA.NS_Indicator,&GPS.GNGGA.Longitude,&GPS.GNGGA.EW_Indicator,&GPS.GNGGA.PositionFixIndicator,&GPS.GNGGA.SatellitesUsed,&GPS.GNGGA.HDOP,&GPS.GNGGA.MSL_Altitude,&GPS.GNGGA.MSL_Units,&GPS.GNGGA.AgeofDiffCorr,GPS.GNGGA.DiffRefStationID,GPS.GNGGA.CheckSum);
			if(GPS.GNGGA.NS_Indicator==0)
				GPS.GNGGA.NS_Indicator='-';
			if(GPS.GNGGA.EW_Indicator==0)
				GPS.GNGGA.EW_Indicator='-';
			if(GPS.GNGGA.Geoid_Units==0)
				GPS.GNGGA.Geoid_Units='-';
			if(GPS.GNGGA.MSL_Units==0)
				GPS.GNGGA.MSL_Units='-';
			GPS.GNGGA.LatitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Latitude);
			GPS.GNGGA.LongitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Longitude);			
		}		
		memset(GPS.rxBuffer,0,sizeof(GPS.rxBuffer));
		GPS.rxIndex=0;
	}
	HAL_UART_Receive_IT(&_GPS_USART,&GPS.rxTmp,1);
}
//##################################################################################################################
