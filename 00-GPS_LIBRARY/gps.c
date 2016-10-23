/**
* |----------------------------------------------------------------------
* | Copyright (c) 2016 Tilen Majerle
* |
* | Permission is hereby granted, free of charge, to any person
* | obtaining a copy of this software and associated documentation
* | files (the "Software"), to deal in the Software without restriction,
* | including without limitation the rights to use, copy, modify, merge,
* | publish, distribute, sublicense, and/or sell copies of the Software,
* | and to permit persons to whom the Software is furnished to do so,
* | subject to the following conditions:
* |
* | The above copyright notice and this permission notice shall be
* | included in all copies or substantial portions of the Software.
* |
* | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* | OTHER DEALINGS IN THE SOFTWARE.
* |----------------------------------------------------------------------
*/
#include "gps.h"

/******************************************************************************/
/******************************************************************************/
/***                            Private structures                           **/
/******************************************************************************/
/******************************************************************************/
typedef struct {
	union {
		struct {
			uint8_t Star:1;									/* Star detected flag */
			uint8_t Term_Num:5;								/* Term number in received stringline */
			uint8_t Term_Pos:5;								/* Term position for adding new character to string object */
			uint8_t Statement:4;							/* Statement number identification */
			uint8_t GPGSV_Num:3;							/* GPGSV statement number */
			uint8_t GPGSV_Nums:3;							/* Number of all GPGSV statements */
            uint8_t Received:6;                             /* Received flags for statements */
		} F;
	} Flags;
	char Term[13];											/* Term string */
	char Statement[7];										/* Current statement name as string for unknown statements */
	uint8_t CRC;											/* Calculated CRC */
} GPS_Int_t;

/******************************************************************************/
/******************************************************************************/
/***                            Private macros                               **/
/******************************************************************************/
/******************************************************************************/
#define CHARISNUM(x)					((x) >= '0' && (x) <= '9')
#define CHARISHEXNUM(x)					(((x) >= '0' && (x) <= '9') || ((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F'))
#define CHARTONUM(x)					((x) - '0')
#define CHARHEXTONUM(x)					(((x) >= '0' && (x) <= '9') ? ((x) - '0') : (((x) >= 'a' && (x) <= 'z') ? ((x) - 'a' + 10) : (((x) >= 'A' && (x) <= 'Z') ? ((x) - 'A' + 10) : 0)))
#define FROMMEM(x)						((const char *)(x))

#define GPS_ADDTOCRC(ch)				do { Int.CRC ^= (uint8_t)(ch); } while (0)
#define GPS_ADDTOTERM(ch)				do { Int.Term[Int.Flags.F.Term_Pos++] = (ch); Int.Term[Int.Flags.F.Term_Pos] = 0; } while (0);	/*Addnewelementtotermobject*/
#define GPS_START_NEXT_TERM()			do { Int.Term[0]= 0; Int.Flags.F.Term_Pos = 0; Int.Flags.F.Term_Num++; } while (0);

#define GPS_EARTH_RADIUS				6371				/* Earth radius */
#define GPS_DEGREES2RADIANS(x)			((x) * 0.01745329251994f)	/* Degreestoradiansconverter */
#define GPS_RADIANS2DEGREES(x)			((x) * 57.29577951308232f)	/* Radianstodegrees */
#define GPS_MAX_SATS_IN_VIEW			24							/* Maximalnumberofsatellitesinview */

/* GPS statements definitions */
#define GPS_UNKNOWN						0
#define GPS_GPGGA						1
#define GPS_GPGSA						2
#define GPS_GPGSV						3
#define GPS_GPRMC						4
#define GPS_FLAGS_ALL                   (1 << GPS_GPGGA | 1 << GPS_GPGSA | 1 << GPS_GPGSV | 1 << GPS_GPRMC)

#define GPS_CONCAT(x,y)					(uint16_t)((x) << 8 | (y))

/******************************************************************************/
/******************************************************************************/
/***                            Private variables                            **/
/******************************************************************************/
/******************************************************************************/
static uint8_t BufferData[GPS_BUFFER_SIZE];
static BUFFER_t BUFFER;
static GPS_Int_t Int;
static uint8_t ReceivedFlags = 0x00;

/******************************************************************************/
/******************************************************************************/
/***                            Private functions                            **/
/******************************************************************************/
/******************************************************************************/
/* Parses and returns number from string */
static
int32_t ParseNumber(const char* ptr, uint8_t* cnt){
	uint8_t minus = 0, i = 0;
	int32_t sum = 0;

	if (*ptr == '-') {										/* Check for minus character */
		minus = 1;
		ptr++;
		i++;
	}
	while (CHARISNUM(*ptr)) {								/* Parse number */
		sum = 10 * sum + CHARTONUM(*ptr);
		ptr++;
		i++;
	}
	if (cnt != NULL) {										/* Save number of characters used for number */
		*cnt = i;
	}
	if (minus) {											/* Minus detected */
		return -sum;
	}
	return sum;												/* Return number */
}

/* Parse HEX number */
static
uint32_t ParseHexNumber(const char* ptr, uint8_t* cnt) {
	uint8_t i = 0;
	uint32_t sum = 0;

	while (CHARISHEXNUM(*ptr)) {							/* Parse number */
		sum = 16 * sum + CHARHEXTONUM(*ptr);
		ptr++;
		i++;
	}
	if (cnt != NULL) {										/* Save number of characters used for number*/
		*cnt = i;
	}
	return sum;												/* Return number */
}

/* Parse float number */
static
float ParseFloatNumber(const char* ptr, uint8_t* cnt) {
	uint8_t i = 0, j = 0;
	float sum = 0.0f;

	sum = (float)ParseNumber(ptr, &i);						/* Parse number */
	j += i;
	ptr += i;
	if (*ptr == '.') {										/* Check decimals */
		float dec;
		dec = (float)ParseNumber(ptr + 1, &i) / (float)pow(10, i);
		if (sum >= 0) {
			sum += dec;
		} else {
			sum -= dec;
		}
		j += i + 1;
	}

	if (cnt != NULL) {										/* Save number of characters used for number*/
		*cnt = j;
	}
	return sum;												/* Return number */
}

/* Parses latitude and longitude coordinates */
static
void ParseLatLong(GPS_t* GPS, const char* term, uint8_t id, float* out) {
	float num;
	uint8_t cnt;

	if (term[4] == '.') {
		num = (float)(10 * CHARTONUM(term[0]) + CHARTONUM(term[1]));	/* Parse degrees */
		num += (float)(10 * CHARTONUM(term[2]) + CHARTONUM(term[3])) / 60.0f;	/* Parse minutes */
		num += (float)ParseNumber(&term[5], &cnt) / (60.0f * (float)pow(10, cnt));	/* Parse seconds */
	} else {
		num = (float)(100 * CHARTONUM(term[0]) + 10 * CHARTONUM(term[1]) + CHARTONUM(term[2]));	/* Parse degrees */
		num += (float)(10 * CHARTONUM(term[3]) + CHARTONUM(term[4])) / 60.0f;	/* Parse minutes */
		num += (float)ParseNumber(&term[6], &cnt) / (60.0f * (float)pow(10, cnt));	/* Parse seconds */
	}
	*out = num;
}

/* Parse term value from GPS*/
static
void ParseValue(GPS_t* GPS) {
	uint8_t i;
	if (Int.Flags.F.Term_Num == 0) {						/* First term in single line */
		if (strcmp(Int.Term, FROMMEM("$GPGGA")) == 0) {
			Int.Flags.F.Statement = GPS_GPGGA;
		} else if (strcmp(Int.Term, FROMMEM("$GPGSA")) == 0) {
			Int.Flags.F.Statement = GPS_GPGSA;
		} else if (strcmp(Int.Term, FROMMEM("$GPGSV")) == 0) {
			Int.Flags.F.Statement = GPS_GPGSV;
		} else if (strcmp(Int.Term, FROMMEM("$GPRMC")) == 0) {
			Int.Flags.F.Statement = GPS_GPRMC;
		} else {
			Int.Flags.F.Statement = GPS_UNKNOWN;
		}
		strcpy(Int.Statement, Int.Term);					/* Copy active string as term statement */
		return;												/* Finish here */
	}

	/* Check custom terms */
	for (i = 0; i < GPS->CustomStatementsCount; i++) {		/* Check each term separatelly */
		if (Int.Flags.F.Term_Num == GPS->CustomStatements[i]->TermNumber && strcmp(GPS->CustomStatements[i]->Statement, Int.Statement) == 0) {
			strcpy(GPS->CustomStatements[i]->Value, Int.Term);  /* Copy value*/
            GPS->CustomStatements[i]->Updated = 1;          /* Set flag as updated value */
		}
	}

	/* Parse core statements */
	switch (GPS_CONCAT(Int.Flags.F.Statement,Int.Flags.F.Term_Num)) {	/* Match statement and term */
		/* GPGGA statement */
		case GPS_CONCAT(GPS_GPGGA, 1):						/* Current UTC time */
			GPS->Time.Hours = 10 * CHARTONUM(Int.Term[0]) + CHARTONUM(Int.Term[1]);
			GPS->Time.Minutes = 10 * CHARTONUM(Int.Term[2]) + CHARTONUM(Int.Term[3]);
			GPS->Time.Seconds = 10 * CHARTONUM(Int.Term[4]) + CHARTONUM(Int.Term[5]);
			if (Int.Term[6] == '.') {
				uint8_t cnt;
				uint16_t tmp = ParseNumber(&Int.Term[7], &cnt);

				switch (cnt) {
					case 1:
						GPS->Time.Hundreds = 10 * tmp;
						GPS->Time.Thousands = 100 * tmp;
						break;
					case 2:
						GPS->Time.Hundreds = tmp;
						GPS->Time.Thousands = 10 * tmp;
						break;
					case 3:
						GPS->Time.Hundreds = tmp / 10;
						GPS->Time.Thousands = tmp;
						break;
				}
			}
			break;
		case GPS_CONCAT(GPS_GPGGA, 2):						/* Latitude */
			ParseLatLong(GPS,Int.Term,0,&GPS->Latitude);	/* Parse latitude and save data */
			break;
		case GPS_CONCAT(GPS_GPGGA, 3):						/* Latitude north or south */
			if (Int.Term[0] == 'S' || Int.Term[0] == 's') {
				GPS->Latitude = -GPS->Latitude;
			}
			break;
		case GPS_CONCAT(GPS_GPGGA, 4):						/* Longitude*/
			ParseLatLong(GPS, Int.Term, 0, &GPS->Longitude);/* Parse longitude and save data */
			break;
		case GPS_CONCAT(GPS_GPGGA, 5):						/* Longitude west or east */
			if (Int.Term[0] == 'W' || Int.Term[0] == 'w') {
				GPS->Latitude = -GPS->Latitude;
			}
			break;
		case GPS_CONCAT(GPS_GPGGA, 6):						/* GPS fix status */
			GPS->Fix = (GPS_Fix_t)CHARTONUM(Int.Term[0]);	/* Fix status */
			break;
		case GPS_CONCAT(GPS_GPGGA, 7):						/* Satellites in use */
			GPS->SatsInUse = ParseNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPGGA, 9):						/* Altitude */
			GPS->Altitude = ParseFloatNumber(Int.Term, NULL);
			break;
		
		/* GPGSA statement */
		case GPS_CONCAT(GPS_GPGSA, 2):						/* Fix mode */
			GPS->FixMode = (GPS_FixMode_t)ParseNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPGSA, 3):
		case GPS_CONCAT(GPS_GPGSA, 4):
		case GPS_CONCAT(GPS_GPGSA, 5):
		case GPS_CONCAT(GPS_GPGSA, 6):
		case GPS_CONCAT(GPS_GPGSA, 7):
		case GPS_CONCAT(GPS_GPGSA, 8):
		case GPS_CONCAT(GPS_GPGSA, 9):
		case GPS_CONCAT(GPS_GPGSA, 10):
		case GPS_CONCAT(GPS_GPGSA, 11):
		case GPS_CONCAT(GPS_GPGSA, 12):
		case GPS_CONCAT(GPS_GPGSA, 13):
		case GPS_CONCAT(GPS_GPGSA, 14):
			GPS->SatelliteIDs[Int.Flags.F.Term_Num - 3] = ParseNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPGSA, 15):						/* Position Dilution of Precision */
			GPS->PDOP = ParseFloatNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPGSA, 16):						/* Horizontal Dilution of Precision */
			GPS->HDOP = ParseFloatNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPGSA, 17):						/* Vertical Dilution of Precision */
			GPS->VDOP = ParseFloatNumber(Int.Term, NULL);
			break;

		/* GPRMC statement */
		case GPS_CONCAT(GPS_GPRMC, 2):						/* Valid GPS fix */
			GPS->Valid = Int.Term[0] == 'A';
			break;
		case GPS_CONCAT(GPS_GPRMC, 7):						/* Speed in knots */
			GPS->Speed = ParseFloatNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPRMC, 8):						/* True ground coarse */
			GPS->Coarse = ParseFloatNumber(Int.Term, NULL);
			break;
		case GPS_CONCAT(GPS_GPRMC, 9):						/* Date */
			GPS->Date.Day = 10 * CHARTONUM(Int.Term[0]) + CHARTONUM(Int.Term[1]);
			GPS->Date.Month = 10 * CHARTONUM(Int.Term[2]) + CHARTONUM(Int.Term[3]);
			GPS->Date.Year = 10 * CHARTONUM(Int.Term[4]) + CHARTONUM(Int.Term[5]);
			break;
		case GPS_CONCAT(GPS_GPRMC, 10):						/* Variation */
			GPS->Variation = ParseFloatNumber(Int.Term, NULL);
			break;

		/* GPGSV statement */
		case GPS_CONCAT(GPS_GPGSV, 1):						/* Get number of all GPGSV statements */
			Int.Flags.F.GPGSV_Nums = CHARTONUM(Int.Term[0]);
			break;
		case GPS_CONCAT(GPS_GPGSV, 2):						/* Get current GPGSV statement number */
			Int.Flags.F.GPGSV_Num = CHARTONUM(Int.Term[0]);
			break;
		case GPS_CONCAT(GPS_GPGSV, 3):						/* Get current GPGSV statement number */
			GPS->SatsInView = ParseNumber(Int.Term, NULL);
			break;
		default:
			if (Int.Flags.F.Statement == GPS_GPGSV && Int.Flags.F.Term_Num >= 4) {	/* Process data in GPGSV statement */
				uint32_t tmp;
				uint8_t mod, term_num;
					
				tmp = ParseNumber(Int.Term, NULL);			/* Parse received number */
				term_num = Int.Flags.F.Term_Num - 4;		/* Normalize number */

				mod = term_num % 4;							/* Get division by zero */
				term_num = (Int.Flags.F.GPGSV_Num - 1) * 4 + (term_num / 4);	/* Calculate array position for data */

				if (term_num < GPS_MAX_SATS_IN_VIEW) {		/* If still memory available */
					switch (mod) {
						case 0:								/* Satellite ID */
							GPS->SatsDesc[term_num].ID = tmp;
							break;
						case 1:								/* Satellite elevation */
							GPS->SatsDesc[term_num].Elevation = tmp;
							break;
						case 2:								/* Satellite azimuth */
							GPS->SatsDesc[term_num].Azimuth = tmp;
							break;
						case 3:								/* Satellite signal to noise ratio */
							GPS->SatsDesc[term_num].SNR = tmp;
							break;
						default:
							break;
					}
				}
			}
			break;
	}
}

/* Parse each received character */
static
GPS_Result_t ParseReceived(GPS_t* GPS, char ch) {
    
	if (ch == '$') {										/* Start of string detected */
		memset((void *)&Int, 0x00, sizeof(GPS_Int_t));		/* Reset data structure */
		Int.CRC = 0x00;
		GPS_ADDTOTERM(ch);									/* Add character to first term */
	} else if (ch == ',') {
		GPS_ADDTOCRC(ch);									/* Compute CRC */
		ParseValue(GPS);									/* Check term */
		GPS_START_NEXT_TERM();								/* Start next term */
	} else if (ch == '*') {
		Int.Flags.F.Star = 1;								/* Star detected */
		ParseValue(GPS);									/* Check term */
		GPS_START_NEXT_TERM();								/* Start next term */
	} else if (ch == '\r') {
		if ((uint8_t)ParseHexNumber(Int.Term, NULL) == Int.CRC) {	/* CRC is not OK, data failed somewhere */
			switch (Int.Flags.F.Statement) {
                case GPS_GPGGA:
                case GPS_GPGSA:
                case GPS_GPRMC:
                    ReceivedFlags |= 1 << Int.Flags.F.Statement;
                    break;
                case GPS_GPGSV:
                    if (Int.Flags.F.GPGSV_Num == Int.Flags.F.GPGSV_Nums) {
                        ReceivedFlags |= 1 << Int.Flags.F.Statement;
                    }
                default:
                    break;
            }
		}
	} else if (ch != ' ') {									/* Other characters detected */
		if (!Int.Flags.F.Star) {							/* If star is not detected yet */
			GPS_ADDTOCRC(ch);								/* Compute CRC */
		}
		GPS_ADDTOTERM(ch);									/* Add received character to instance */
	}
    if ((ReceivedFlags & GPS_FLAGS_ALL) == GPS_FLAGS_ALL) {  /* If all statements are properly received */
        uint8_t ok = 1, i;
        for (i = 0; i < GPS->CustomStatementsCount; i++) {  /* Check all custom statements */
            if (!GPS->CustomStatements[i]->Updated) {
                ok = 0;
                break;
            }
        }
            
        if (ok) {
            ReceivedFlags = 0x00;                           /* Reset data */
            for (i = 0; i < GPS->CustomStatementsCount; i++) {  /* Reset other flags */
                GPS->CustomStatements[i]->Updated = 0;
            }
            return gpsNEWDATA;                              /* We have new data */
        }
    }
    return gpsNODATA;
}

/******************************************************************************/
/******************************************************************************/
/***                            Public API                                   **/
/******************************************************************************/
/******************************************************************************/
GPS_Result_t GPS_Init(GPS_t*GPS){
	memset((void *)GPS, 0x00, sizeof(GPS_t));				/* Reset structure for GPS */
	BUFFER_Init(&BUFFER, sizeof(BufferData), BufferData);	/* Initialize buffer for received data */

	return gpsOK;
}

uint32_t GPS_DataReceived(uint8_t* ch, size_t count) {
	return BUFFER_Write(&BUFFER, ch, count);				/* Write received data to buffer */
}

GPS_Result_t GPS_Update(GPS_t* GPS) {
	uint8_t ch;
	
	while (BUFFER_Read(&BUFFER, &ch, 1)) {					/* Read character by character from device */
		if (ParseReceived(GPS, ch) == gpsNEWDATA) {         /* Process each character */
            return gpsNEWDATA;
        }								
	}
    return gpsNODATA;
}

GPS_Result_t GPS_Custom_Add(GPS_t* GPS, GPS_Custom_t* Custom, const char* GPG_Statement, uint8_t TermNumber) {
	if (GPS->CustomStatementsCount >= GPS_CUSTOM_COUNT) {
		return gpsERROR;
	}
	
	Custom->Statement = GPG_Statement;						/* Save term start name */
	Custom->TermNumber = TermNumber;						/* Save term number */

	GPS->CustomStatements[GPS->CustomStatementsCount++] = Custom;	/* Save pointer to custom object */
	return gpsOK;											/* Return OK */
}

GPS_Result_t GPS_DistanceBetween(GPS_Distance_t* Distance) {
	float f1, f2, l1, l2, df, dfi, a;

	/* Calculate distance between 2 pointes */
	f1 = GPS_DEGREES2RADIANS(Distance->Latitude1);
	f2 = GPS_DEGREES2RADIANS(Distance->Latitude2);
	l1 = GPS_DEGREES2RADIANS(Distance->Longitude1);
	l2 = GPS_DEGREES2RADIANS(Distance->Longitude2);
	df = GPS_DEGREES2RADIANS(Distance->Latitude2 - Distance->Latitude1);
	dfi = GPS_DEGREES2RADIANS(Distance->Longitude2 - Distance->Longitude1);

	a = sin(df * 0.5f) * sin(df * 0.5f) + cos(f1) * cos(f2) * sin(dfi * 0.5f) * sin(dfi * 0.5f);
	Distance->Distance = GPS_EARTH_RADIUS * 2 * atan2(sqrt(a), sqrt(1 - a)) * 1000;	/* Get distance in meters */

	/* Calculate bearing between two points from point1 to point2 */
	df = sin(l2 - l1) * cos(f2);
	dfi = cos(f1) * sin(f2) - sin(f1) * cos(f2) * cos(l2 - l1);
	Distance->Bearing = (GPS_RADIANS2DEGREES(atan2(df, dfi)));

	/* Make bearing always positive from 0 - 360 degrees instead of -180 to 180 */
	if (Distance->Bearing < 0) {
		Distance->Bearing += 360;
	}
    return gpsOK;
}

float GPS_ConvertSpeed(float SpeedInKnots, GPS_Speed_t toSpeed) {
	switch (toSpeed) {
		/* Metric */
		case GPS_Speed_KilometerPerSecond:	return SpeedInKnots * 0.000514f;
		case GPS_Speed_MeterPerSecond: 		return SpeedInKnots * 0.5144f;
		case GPS_Speed_KilometerPerHour:	return SpeedInKnots * 1.852f;
		case GPS_Speed_MeterPerMinute: 		return SpeedInKnots * 30.87f;

		/* Imperial */
		case GPS_Speed_MilePerSecond: 		return SpeedInKnots * 0.0003197f;
		case GPS_Speed_MilePerHour: 		return SpeedInKnots * 1.151f;
		case GPS_Speed_FootPerSecond: 		return SpeedInKnots * 1.688f;
		case GPS_Speed_FootPerMinute: 		return SpeedInKnots * 101.3f;

		/* For Runners and Joggers */
		case GPS_Speed_MinutePerKilometer: 	return SpeedInKnots * 32.4f;
		case GPS_Speed_SecondPerKilometer: 	return SpeedInKnots * 1944.0f;
		case GPS_Speed_SecondPer100Meters: 	return SpeedInKnots * 194.4f;
		case GPS_Speed_MinutePerMile: 		return SpeedInKnots * 52.14f;
		case GPS_Speed_SecondPerMile: 		return SpeedInKnots * 3128.0f;
		case GPS_Speed_SecondPer100Yards: 	return SpeedInKnots * 177.7f;

		/* Nautical */
		case GPS_Speed_SeaMilePerHour: 		return SpeedInKnots * 1.0f;
		default:
			return 0;
	}
}
