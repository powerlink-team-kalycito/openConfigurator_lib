/**
 ************************************************************************************************
 \file			Utility.cpp

 \brief			Handles datatype conversion 
 ************************************************************************************************
 */

/*
 (c) Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************************************/
/* Includes */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"

using namespace std;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief			ConvertToUpper
 
 This function converts received string to upper case
 
 \param			strBuffer         Character pointer variable to hold the value of string
 \return		char*
 */
/*****************************************************************************/

char* ConvertToUpper(char* strBuffer)
{
	if (NULL == strBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 uiLoopCount = 0;

	for (uiLoopCount = 0; strBuffer[uiLoopCount]; ++uiLoopCount)
	{
		strBuffer[uiLoopCount] = toupper(strBuffer[uiLoopCount]);
	}
	return strBuffer;
}

/*****************************************************************************/
/**
 \brief			StringToUpper
 
 This function checks string length,allocate buffer and covert the string to upper case
 
 \param			strBuffer	Character pointer variable to hold the string length
 \return		char*
 */
/*****************************************************************************/

//TODO: To add a parameter for destination to avoid new delete memory leak.
char* StringToUpper(char* strBuffer)
{
	if (NULL == strBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 uiLoopCount;
	UINT32 uiStringLen = strlen(strBuffer);

	char* strUpp = new char[uiStringLen + STR_ALLOC_BUFFER];

	for (uiLoopCount = 0; uiLoopCount < uiStringLen; ++uiLoopCount)
	{
		strUpp[uiLoopCount] = toupper(strBuffer[uiLoopCount]);
	}

	strUpp[uiLoopCount] = '\0';
	return strUpp;
}

/*****************************************************************************/
/**
 \brief		Reverse
 
 This function reverses the received string
 
 \param		strBuffer  Character pointer variable to hold the string
 
 \return	char*
 */
/*****************************************************************************/

char* Reverse(char* strBuffer)
{
	if (NULL == strBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* charLeft = strBuffer;
	char* charRight = charLeft + strlen(strBuffer) - 1;
	char cTemp;

	while (charLeft < charRight)
	{
		cTemp = *charLeft;
		*charLeft++ = *charRight;
		*charRight-- = cTemp;
	}
	return strBuffer;
}

/*****************************************************************************/
/**
 \brief			PadLeft
 
 This function pads left of the string with 0's
 
 \param			strBuffer      Character pointer variable to hold the string length
 \param			cPadChar         Character Variable to hold the value of padding string  
 \param			iPadLength       Integer Variable to hold the value of Pad Length
 
 \return		char*
 */
/*****************************************************************************/

char* PadLeft(char* strBuffer, char cPadChar, INT32 iPadLength)
{
	if (NULL == strBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	INT32 iStringLen = strlen(strBuffer);

	if (iStringLen < iPadLength)
	{
		char abTempBuff[50];
		INT32 iLoopCount = 0;
		strBuffer = Reverse(strBuffer);
		while (iLoopCount < (iPadLength - iStringLen))
		{
			abTempBuff[iLoopCount] = cPadChar;
			iLoopCount++;
		}

		abTempBuff[iLoopCount] = '\0';
		strcat(strBuffer, abTempBuff);
		strBuffer = Reverse(strBuffer);
	}
	return strBuffer;
}

/*****************************************************************************/
/**
 \brief			SubString
 
 This function returns substring
 
 \param			strBuffer    Character pointer variable to hold the string length
 \param			iStartPos      Integer Variable to hold the value of starting position of the string 
 \param			iStringLen     Integer Variable to hold the value of string length
 
 \return		char*
 */
/*****************************************************************************/

//TODO: Another parameter to be added for substring to avoid new delete memory issues.
char* SubString(char* strBuffer, INT32 iStartPos, INT32 iStringLen)
{
	if (NULL == strBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* subStrBuffer = NULL;

	subStrBuffer = new char[iStringLen + 1];
	strncpy(subStrBuffer, (const char*) (strBuffer + iStartPos),
			iStringLen);
	subStrBuffer[iStringLen] = '\0';
	return subStrBuffer;
}

/*****************************************************************************/
/**
 \brief			UnsignedToAlphaNumeric
 
 This function converts unsigned values to alphanumeric values
 
 \param			uValue           Unsigned Integer Variable to hold the value to be converted to alphanumeric values
 \param			iDigits         Character Pointer Variable to hold the digits
 \param			iBaseValue       Integer Variable to hold the basevalue
 \return		char * / NULL
 */
/*****************************************************************************/

char* UnsignedToAlphaNumeric(unsigned uValue, char *iDigits, INT32 iBaseValue)
{
	char *alphNum = NULL;

	alphNum = (char *) "0123456789abcdefghijklmnopqrstuvwxyz";
	if (0 == iBaseValue)
	{
		iBaseValue = 10;
	}
	if ((NULL == iDigits) || (2 > iBaseValue) || (36 < iBaseValue))
	{
		return NULL;
	}
	if (uValue < (unsigned) iBaseValue)
	{
		iDigits[0] = alphNum[uValue];
		iDigits[1] = '\0';
	}
	else
	{
		char *tempBuffer = NULL;
		for (tempBuffer = UnsignedToAlphaNumeric(uValue / ((unsigned) iBaseValue), iDigits,
				iBaseValue); *tempBuffer; tempBuffer++)
			;
		{
			UnsignedToAlphaNumeric(uValue % ((unsigned) iBaseValue), tempBuffer, iBaseValue);
		}
	}
	return iDigits;

}

/*****************************************************************************/
/**
 \brief			IntToAscii
 
 This function converts INT values to ASCII value
 
 \param			iValue          Long Integer Variable to hold the value to be converted to ASCII value
 \param			iDigits        Character Pointer Variable to hold the digits
 \param			iBaseValue      Integer Variable to hold the basevalue
 
 \return		char *
 */
/*****************************************************************************/

char* IntToAscii(LONG iValue, char *iDigits, INT32 iBaseValue)
{
	char *digit = NULL;
	ULONG uiValue; /* assume unsigned is big enough to hold all the
	 * unsigned values -x could possibly be -- don't
	 * know how well this assumption holds on the
	 * DeathStation 9000, so beware of nasal demons
	 */

	digit = iDigits;
	if (0 == iBaseValue)
	{
		iBaseValue = 10;
	}
	if ((NULL == iDigits) || (2 > iBaseValue) || (36 < iBaseValue))
	{
		return NULL;
	}
	if (iValue < 0)
	{
		uiValue = -(iValue);
		uiValue = ~uiValue + 1; // two's complement
	}
	else
	{
		uiValue = iValue;
	}

	UnsignedToAlphaNumeric(uiValue, digit, iBaseValue);

	return iDigits;
}

/*****************************************************************************/
/**
 \brief			HexToInt
 
 This function converts hex values to int values
 
 \param			strBuffer   Character pointer variable to hold the hex value to be converted to Int
 \return		ULONG
 */
/*****************************************************************************/

ULONG HexToInt(char *strBuffer)
{
	UINT32 uiLoopCount = 0;
	UINT32 uiValue = 0;
	UINT32 uiStrLen = strlen(strBuffer);

	for (uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
	{
		if (IsAscii(strBuffer[uiLoopCount]))
		{
			strBuffer[uiLoopCount] = toupper(strBuffer[uiLoopCount]);
		}
		if (strBuffer[uiLoopCount] <= 57)
		{
			uiValue += (strBuffer[uiLoopCount] - 48)
					* (1 << (4 * ((uiStrLen - 1) - uiLoopCount)));
		}
		else
		{
			uiValue += (strBuffer[uiLoopCount] - 55)
					* (1 << (4 * ((uiStrLen - 1) - uiLoopCount)));
		}
	}
	return uiValue;
}

/*****************************************************************************/
/**
 \brief			IsAscii
 
 This function checks whether the given character is ascii
 
 \param			cArg	Characer Variable to hold the ASCII value
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool IsAscii(char cArg)
{
	UINT32 uiCount;

	uiCount = cArg;

	if (48 <= uiCount && 57 >= uiCount)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*****************************************************************************/
/**
 \brief			CheckIfNotPDO
 
 This function checks whether the given index value is PDO or not
 
 \param			iIndex         Character Pointer Variable to hold the value of index
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CheckIfNotPDO(char* iIndex)
{
	if ((0 == strncmp(iIndex, "14xx", 2)) || (0 == strncmp(iIndex, "16xx", 2))
			|| (0 == strncmp(iIndex, "18xx", 2))
			|| (0 == strncmp(iIndex, "1Axx", 2)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*****************************************************************************/
/**
 \brief			CheckIfManufactureSpecificObject
 
 This function checks whether the device index is manufacturer spcific or not
 
 \param			iIndex     Character Pointer Variable to hold the value of index
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CheckIfManufactureSpecificObject(char* iIndex)
{
	UINT32 uiDeviceIndex;

	uiDeviceIndex = HexToInt((char *) "2000");
	if (HexToInt(iIndex) >= uiDeviceIndex)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			CheckIfMappingPDO
 
 This function checks index for PDO mapping
 
 \param			iIndex            Character Pointer Variable to hold the value of index
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CheckIfMappingPDO(char* iIndex)
{
	if ((0 == strncmp(iIndex, "1A", 2)) || (0 == strncmp(iIndex, "16", 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			CheckAllowedCNIndexes
 
 This function checks whether received CN index falls in the allowed CN indexs
 
 \param			indexValue      Character Pointer Variable to hold the value of index
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

//TODO: Function is not used.
bool CheckAllowedCNIndexes(char* indexValue)
{
	if ((false == CheckIfNotPDO((char*) indexValue))
			|| CheckIfManufactureSpecificObject((char*) indexValue)
			|| (0 == strcmp(indexValue, "1F98"))
			|| (0 == strcmp(indexValue, "1020"))
			|| (0 == strcmp(indexValue, "1F9B"))
			|| (0 == strcmp(indexValue, "1F81"))
			|| (0 == strcmp(indexValue, "1006")))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief		CheckBlockedMNIndexes
 
 This function checks whether received MN index is blocked or not
 
 \param		indexValue		Character Pointer Variable to hold the value of index
 
 \return	BOOL
 \retval		TRUE			if successful
 \retval		FALSE			if there is already a message pending	
 */
/*****************************************************************************/

//TODO: function is not used.
bool CheckBlockedMNIndexes(char* indexValue)
{
	if (0 == strcmp(indexValue, "1F81"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief		ConvertStringToHex
 
 Converts the string to hexadecimal for strings of string Datatype
 
 \param		actString	Character pointer variable to hold the value of string to be converted to hex
 
 \return	char*
 */
/*****************************************************************************/

char* ConvertStringToHex(char* actString)
{
	INT32 ilen = 0;

	if (NULL == actString)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ilen = strlen(actString);
	char* actStringBuffer = new char[ilen + 1];
	char* tempBufHex = new char[(ilen * 2) + 1];
	if ((NULL == actStringBuffer) || (NULL == tempBufHex))
	{
#if defined DEBUG
		cout << "Memory allocation error" << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	strcpy(actStringBuffer, (char*) actString);
	strcpy(actString, "");
	for (INT32 iloopCntr = 0; iloopCntr < ilen; iloopCntr++)
	{
		sprintf(tempBufHex, "%X", actStringBuffer[iloopCntr]);
		strcat(actString, (char*) tempBufHex);
	}
	delete[] actStringBuffer;
	delete[] tempBufHex;
	return actString;
}

/*****************************************************************************/
/**
 \brief			ConvertToHexformat
 
 This function converts to hex value
 
 \param			hexValue       Character Pointer to hold the hex value
 \param			iPadLength       Integer variable to hold value of padding length
 \param			bDoPadding       Boolean Variable to decide padding to be carried out or not 
 
 \return		char*			
 */
/*****************************************************************************/

char* ConvertToHexformat(char* hexValue, INT32 iPadLength, bool bDoPadding)
{
	char* hexTemp = NULL;

	hexTemp = new char[iPadLength + 2 + STR_ALLOC_BUFFER];

	strcpy(hexTemp, "0x");

	if (bDoPadding)
	{
		hexValue = PadLeft(hexValue, '0', iPadLength);
	}
	strcat(hexTemp, hexValue);

	return hexTemp;
}

/*****************************************************************************/
/**
 \brief			CheckIfHex
 
 This function checks whether hex or not
 
 \param			iValue    Character Pointer variable to hold the value of hex
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending			
 */
/*****************************************************************************/

bool CheckIfHex(char* iValue)
{
	if (NULL == strchr(iValue, 'x'))
	{
		if (NULL == strchr(iValue, 'X'))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}

}

/*****************************************************************************/
/**
 \brief			GetConfigDate
 
 This function returns configuration date
 
 \return		INT32						
 */
/*****************************************************************************/

INT32 GetConfigDate()
{
	// to have the total number of days Since 1984
	INT32 iDays = 0;
	// to have the total number of years Since 1984  
	INT32 iYearSince = 0;
	// to have the current Year
	//  INT32 iCurrentYear = 0;
	time_t rawtime;
	struct tm *pstTimeInfo;

	time(&rawtime);
	pstTimeInfo = localtime(&rawtime);

	iYearSince = ((pstTimeInfo->tm_year) - 84);
	//  iCurrentYear = ((pstTimeInfo->tm_year) + 1900);

	// Loops thru all the years except the curent year
	for (INT32 iLoopCount = 0; iLoopCount < iYearSince; iLoopCount++)
	{
		INT32 iTmpYear = 0;
		INT32 iTmpDays = 0;
		iTmpYear = (1984 + iLoopCount);
		if ((0 == iTmpYear % 4) && !(0 == iTmpYear % 100)
				&& (0 != iTmpYear % 400))
		{
			iTmpDays = 366;
		}
		else
		{
			iTmpDays = 365;
		}
		iDays += iTmpDays;
	}
	// To calculate number of days since Jan 1 of current year
	iDays += pstTimeInfo->tm_yday;
	return iDays;
}

/*****************************************************************************/
/**
 \brief			GetConfigTime
 
 This function returns configuration time
 
 \return		INT32						
 */
/*****************************************************************************/

INT32 GetConfigTime()
{
	time_t rawtime;
	struct tm *pstTimeInfo;
	INT32 iMSeconds = 0;

	time(&rawtime);
	pstTimeInfo = localtime(&rawtime);

	iMSeconds += (pstTimeInfo->tm_hour) * 3600000;
	iMSeconds += (pstTimeInfo->tm_min) * 60000;
	iMSeconds += (pstTimeInfo->tm_sec) * 1000;

	return iMSeconds;
}

/*****************************************************************************/
/**
 \brief			ReverseData
 
 This function reverses the data
 
 \param			iArg1       Unsigned integer pointer variable to hold value of the data
 \param			iArg2       Unsigned integer pointer variable to hold value of the data
 \param			uiSize		 Unsigned integer pointer variable to hold size of the data	
 
 \return		INT32						
 */
/*****************************************************************************/

//TODO: function is not used.
INT32 ReverseData(UINT8 *iArg1, UINT8 *iArg2, UINT32 uiSize)
{
	UINT32 uiLoopCount;

	if (NULL == iArg1 || NULL == iArg2)
	{
		return -1;
	}

	for (uiLoopCount = 0; uiLoopCount < uiSize; uiLoopCount++)
	{
		*(iArg1 + uiLoopCount) = *(iArg2 + (uiSize - 1) - uiLoopCount);
	}

	return 0;
}

/*****************************************************************************/
/**
 \brief		CheckAllowedDTForMapping
 
 This function checks for the datatype allowed for mapping
 
 \param		dtName	Character Pointer variable to hold datatype name

 \return	BOOL
 \retval		TRUE			if Allowed Datatype for Mapping
 \retval		FALSE			if not allowed
 */
/*****************************************************************************/

bool CheckAllowedDTForMapping(char* dtName)
{
	if (0 == strcmp(StringToUpper(dtName), "INTEGER8")
			|| 0 == strcmp(StringToUpper(dtName), "INTEGER16")
			|| 0 == strcmp(StringToUpper(dtName), "INTEGER32")
			|| 0 == strcmp(StringToUpper(dtName), "INTEGER64")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED8")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED16")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED32")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED64"))
	{
		return true;

	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			GetLastAvailableCycleNumber
 
 This function returns last available cycle number
 
 \return		char*
 */
/*****************************************************************************/

//TODO: Add a parameter to return the value to avoid new delete memory issues
char* GetLastAvailableCycleNumber()
{
	char* ForcedCycle = new char[20];
	ocfmRetCode Ret;
	INT32 indexPos;
	INT32 subIndexPos;
	Ret = IfSubIndexExists(240, MN, (char*) "1F98", (char*) "07", &subIndexPos,
			&indexPos);
	if (OCFM_ERR_SUCCESS != Ret.code)
	{
		strcpy(ForcedCycle, "");
		return ForcedCycle;
	}
	char* actValue = new char[20];
	Ret = GetSubIndexAttributes(240, MN, (char*) "1F98", (char*) "07",
			ACTUALVALUE, actValue);
	UINT32 uiTempCycleNumber = uiCycleNumber;
	if (OCFM_ERR_SUCCESS != Ret.code)
	{

		UINT32 iCycleValue;
		if (0 == strncmp(actValue, "0x", 2) || 0 == strncmp(actValue, "0X", 2))
			iCycleValue = HexToInt(SubString(actValue, 2, strlen(actValue) - 2));
		else
			iCycleValue = atoi(actValue);

		if (iCycleValue > uiTempCycleNumber + 1)
		{
			uiTempCycleNumber += 1;
		}
		else
		{
			uiTempCycleNumber += 1;
			while (iCycleValue < uiTempCycleNumber)
			{
				uiTempCycleNumber = uiTempCycleNumber - iCycleValue;
			}

		}

	}
	delete[] actValue;

	UINT32 uiFreeCycleNumber = GetFreeCycleNumber(uiTempCycleNumber);
	if (uiFreeCycleNumber == uiTempCycleNumber)
	{
		uiCycleNumber = uiTempCycleNumber;
		ForcedCycle = IntToAscii(uiTempCycleNumber, ForcedCycle, 16);
	}
	else
	{
		ForcedCycle = IntToAscii(uiFreeCycleNumber, ForcedCycle, 16);
	}
	return ForcedCycle;
}

/*****************************************************************************/
/**
 \brief			CheckAndCorrectName
 
 This function corrects the name by adding '_'
 
 \param			checkName		Character Pointer variable to hold the name		
 \return		void
 */
/*****************************************************************************/

void CheckAndCorrectName(char* checkName)
{
	if (NULL == checkName)
	{
		return;
	}

	UINT32 uiStrLen = strlen(checkName);
	for (UINT32 uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
	{
		if ((48 <= checkName[uiLoopCount] && 57 >= checkName[uiLoopCount])
				|| (65 <= checkName[uiLoopCount] && 90 >= checkName[uiLoopCount])
				|| (97 <= checkName[uiLoopCount]
						&& 122 >= checkName[uiLoopCount])
				|| 95 == checkName[uiLoopCount])
		{
		}
		else
		{
			checkName[uiLoopCount] = '_';
		}
	}
}

/*****************************************************************************/
/**
 \brief			CheckIfValueZero
 
 This function checks for zero value
 
 \param			varValue				Character Pointer Variable to hold the value

 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfValueZero(char* varValue)
{
	if (NULL == varValue || 0 == strcmp(varValue, ""))
	{
		return false;
	}

	INT32 iValue = 0;
	if (0 == strncmp(varValue, "0x", 2) || 0 == strncmp(varValue, "0X", 2))
	{
		iValue = HexToInt(SubString(varValue, 2, strlen(varValue) - 2));
	}
	else
	{
		iValue = atoi(varValue);
	}

	if (0 == iValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			GetDecimalValue
 
 This function returns decimal value
 
 \param			varValue			Character Pointer Variable to hold the value	
 \return		INT32			
 */
/*****************************************************************************/

INT32 GetDecimalValue(char* varValue)
{
	if (NULL == varValue || 0 == strcmp(varValue, ""))
	{
		return 0;
	}

	INT32 iValue = 0;
	if (0 == strncmp(varValue, "0x", 2) || 0 == strncmp(varValue, "0X", 2))
	{
		iValue = HexToInt(SubString(varValue, 2, strlen(varValue) - 2));
	}
	else
	{
		iValue = atoi(varValue);
	}

	return iValue;
}

/*****************************************************************************/
/**
 \brief			CheckAccessTypeForInclude
 
 This function checks for access type
 
 \param			accessType		Character Pointer Variable to hold the Access type 

 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckAccessTypeForInclude(char* accessType)
{
	bool bInclude;
	if (NULL == accessType)
	{
		bInclude = true;
	}
	else
	{
		char* upperAccesstype = StringToUpper(accessType);
		if ((0 == strcmp(upperAccesstype, "CONST"))
				|| (0 == strcmp(upperAccesstype, "RO")))
		{
			bInclude = false;
		}
		else
		{
			bInclude = true;
		}
		//pcUpperAccesstype shoul be deleted if stringtoupper function parameters changed
		delete[] upperAccesstype;
	}
	return bInclude;
}

/*****************************************************************************/
/**
 \brief			CheckToolVersion
 
 This function checks for tool version
 
 \param			currentToolVersion		Character Pointer Variable to hold the value of current tool version		
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckToolVersion(char* currentToolVersion)
{
	char* upperCurrentToolVersion = ConvertToUpper(currentToolVersion);
	if ((FALSE == strcmp(upperCurrentToolVersion, TOOL_VERSION))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_1))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_2))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_3))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_4))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_5))
			|| (FALSE == strcmp(upperCurrentToolVersion, PREV_TOOL_VERSION_6))
			|| (FALSE == strcmp(upperCurrentToolVersion, LAST_TOOL_VERSION))
			|| (FALSE
					== strcmp(upperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_1))
			|| (FALSE
					== strcmp(upperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_2))
			|| (FALSE
					== strcmp(upperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_3))
			|| (FALSE
					== strcmp(upperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_4))
			|| (FALSE
					== strcmp(upperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_5)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
