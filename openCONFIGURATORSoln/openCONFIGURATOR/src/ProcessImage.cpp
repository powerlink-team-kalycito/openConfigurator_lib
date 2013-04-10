/**
 ************************************************************************************************
 \file			ProcessImage.cpp

 \brief			Handles Inout,offset values for process image
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
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ProcessImage.h"

/****************************************************************************************************/
/* Defines */

#define HEADER_FILE_BUFFER 5000000
#define TOTAL_MODULES 10000

/****************************************************************************************************/
/* Global Variables */

INT32 iInVars = 0;
INT32 iOutVars = 0;

static tADDRESSTABLE AddressTable[NO_OF_PI_ENTERIES] =
{
{ "A000", INTEGER8, OUTPUT },
{ "A040", UNSIGNED8, OUTPUT },
{ "A0C0", INTEGER16, OUTPUT },
{ "A100", UNSIGNED16, OUTPUT },
{ "A1C0", INTEGER32, OUTPUT },
{ "A200", UNSIGNED32, OUTPUT },
{ "A240", REAL32, OUTPUT},
{ "A400", INTEGER64, OUTPUT},
{ "A440", UNSIGNED64, OUTPUT},
{ "A480", INTEGER8, INPUT },
{ "A4C0", UNSIGNED8, INPUT },
{ "A540", INTEGER16, INPUT },
{ "A580", UNSIGNED16, INPUT },
{ "A640", INTEGER32, INPUT },
{ "A680", UNSIGNED32, INPUT },
{ "A6C0", REAL32, INPUT},
{ "A880", INTEGER64, INPUT},
{ "A8C0", UNSIGNED64, INPUT},

};
ModuleCol astModuleInfo[TOTAL_MODULES];

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief			SetPIOffsets
 
 This function sets offset value for process image
 
 \param			objProcessImage     Class Pointer Variable of ProcessImage
 \param			iStartByteOffset     Integer Variable to hold the value of startbyteoffset  
 \param			iPosition            Integer Variable to hold the value of image position 
 \param			iStartBitOffset      Integer Variable to hold the value of startbitoffset 
 \return		void
 */
/*****************************************************************************/
//TODO: Unused function
void SetPIOffsets(ProcessImage* objProcessImage, INT32& iStartByteOffset,
		INT32 iPosition, INT32& iStartBitOffset)
{
	if (NULL == objProcessImage)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	if (1 == objProcessImage->DataInfo.DataSize)
	{
		if (-1 == iStartBitOffset)
		{
			objProcessImage->ByteOffset = iStartByteOffset;
		}
		iStartBitOffset = iStartBitOffset + 1;
		objProcessImage->BitOffset = iStartBitOffset;
		objProcessImage->ByteOffset = iStartByteOffset;

		if (7 == iStartBitOffset)
		{
			iStartByteOffset = iStartByteOffset + 1;
		}
	}
	else
	{
		objProcessImage->ByteOffset = iStartByteOffset;
		iStartByteOffset = iStartByteOffset
				+ (objProcessImage->DataInfo.DataSize) / 8;
	}
}

/*****************************************************************************/
/**
 \brief			GroupInOutPIVariables
 
 This function assigns inout values for process image variables
 
 \param			aObjPIInCol       Class Variable of ProcessImage to hold the value of object
 \param			aObjPIOutCol      Class Variable of ProcessImage to hold the value of object 
 \return		void
 */
/*****************************************************************************/

void GroupInOutPIVariables(ProcessImage aObjPIInCol[],
		ProcessImage aObjPIOutCol[])
{

	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	iInVars = 0;
	iOutVars = 0;

	for (INT32 iOutLoopCount = 0;
			iOutLoopCount < objNodeCol->GetNumberOfNodes(); iOutLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iOutLoopCount);

		for (INT32 iInLoopCount = 0;
				iInLoopCount < objNode->ProcessImageCollection.Count();
				iInLoopCount++)
		{
			if (INPUT
					== objNode->ProcessImageCollection[iInLoopCount].DirectionType)
			{
				aObjPIInCol[iInVars] =
						objNode->ProcessImageCollection[iInLoopCount];
				iInVars++;
			}
			else if (OUTPUT
					== objNode->ProcessImageCollection[iInLoopCount].DirectionType)
			{
				aObjPIOutCol[iOutVars] =
						objNode->ProcessImageCollection[iInLoopCount];
				iOutVars++;
			}
			else
			{
				//Nothing other than INPUT & OUTPUT
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief			GroupNETPIVariables
 
 
 
 \param			directionType   Class Variable of EPIDirectionType to hold the direction type
 \param			aObjPICol       Class Variable of NETProcessImage to hold the object 
 \return		void
 */
/*****************************************************************************/

INT32 GroupNETPIVariables(EPIDirectionType directionType,
		NETProcessImage aObjPICol[])
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 netPIVarsCount = 0;
	for (INT32 iOutLoopCount = 0;
			iOutLoopCount < objNodeCol->GetNumberOfNodes(); iOutLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iOutLoopCount);

		for (INT32 iInLoopCount = 0;
				iInLoopCount < objNode->NETProcessImageCollection.Count();
				iInLoopCount++)
		{
			if (objNode->NETProcessImageCollection[iInLoopCount].DirectionType
					== directionType)
			{
				aObjPICol[netPIVarsCount] =
						objNode->NETProcessImageCollection[iInLoopCount];
				netPIVarsCount++;
			}
		}
	}
	return netPIVarsCount;
}

/*****************************************************************************/
/**
 \brief			SetUniquePIVarName
 
 This function assigns unique variables for process image
 
 \return		void
 */
/*****************************************************************************/

void SetUniquePIVarName()
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	iInVars = 0;
	iOutVars = 0;
	ProcessImage* piObjTemp = NULL;
	ProcessImage* piObjTempChk = NULL;
	const INT32 iUniqNameLen = 2;

	for (INT32 iOutLoopCount = 0;
			iOutLoopCount < objNodeCol->GetNumberOfNodes(); iOutLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iOutLoopCount);

		for (INT32 iInLoopCount = 0;
				iInLoopCount < objNode->ProcessImageCollection.Count();
				iInLoopCount++)
		{
			INT32 iUniqNameCnt = 1;
			bool bMatchFound = false;

			piObjTemp = &objNode->ProcessImageCollection[iInLoopCount];

			//it is possible that the changed var name matching a previous entry
			for (INT32 iInChkLoopCount = 0;
					iInChkLoopCount < objNode->ProcessImageCollection.Count();
					iInChkLoopCount++)
			{
				if (iInChkLoopCount == iInLoopCount)
				{
					//Both the index are same, do not check same object
					continue;
				}

				piObjTempChk =
						&objNode->ProcessImageCollection[iInChkLoopCount];

				//check module index, module name, directiontype and variable name
				//if all are same then append count variable to variable name
				if ((0
						== strcmp(piObjTemp->ModuleIndex,
								piObjTempChk->ModuleIndex))
						&& (0
								== strcmp(piObjTemp->ModuleName,
										piObjTempChk->ModuleName))
						&& (piObjTemp->DirectionType
								== piObjTempChk->DirectionType)
						&& (0
								== strcmp(piObjTemp->VarName,
										piObjTempChk->VarName)))
				{
					//change the name of VarName
					iUniqNameCnt++; //1 is reserved for first matched entry
					if (NULL != piObjTempChk->VarName)
					{
						delete[] piObjTempChk->VarName;
					}
					piObjTempChk->VarName = new char[strlen(
							piObjTemp->VarName) + iUniqNameLen + ALLOC_BUFFER];
					sprintf(piObjTempChk->VarName, "%s%02d",
							piObjTemp->VarName, iUniqNameCnt);
					bMatchFound = true;

					//change the name of Name
					if (NULL != piObjTempChk->Name)
						delete[] piObjTempChk->Name;

					piObjTempChk->Name = new char[strlen(
							piObjTempChk->VarName)
							+ strlen(piObjTempChk->ModuleName) + 6
							+ ALLOC_BUFFER];
					strcpy(piObjTempChk->Name,
							GetPIName(objNode->GetNodeId()));
					strcat(piObjTempChk->Name, piObjTempChk->ModuleName);
					strcat(piObjTempChk->Name, ".");
					strcat(piObjTempChk->Name, piObjTempChk->VarName);

				}
			}

			if (true == bMatchFound)
			{
				char* tempVarName = NULL;
				tempVarName = new char[strlen(piObjTemp->VarName)
						+ ALLOC_BUFFER];
				strcpy(tempVarName, piObjTemp->VarName);
				if (NULL != piObjTemp->VarName)
				{
					delete[] piObjTemp->VarName;
				}
				piObjTemp->VarName = new char[strlen(tempVarName)
						+ iUniqNameLen + ALLOC_BUFFER];

				sprintf(piObjTemp->VarName, "%s%02d", tempVarName, 1);
				delete[] tempVarName;

				//change the name of Name
				if (NULL != piObjTemp->Name)
				{
					delete[] piObjTemp->Name;
				}
				piObjTemp->Name = new char[strlen(piObjTemp->VarName)
						+ strlen(piObjTemp->ModuleName) + 6 + ALLOC_BUFFER];

				strcpy(piObjTemp->Name, GetPIName(objNode->GetNodeId()));
				strcat(piObjTemp->Name, piObjTemp->ModuleName);
				strcat(piObjTemp->Name, ".");
				strcat(piObjTemp->Name, piObjTemp->VarName);
			}
		}
	}
	//return;
}

/*****************************************************************************/
/**
 \brief			GetIECDT
 
 
 \param			dataStr       Character Pointer Variable to hold the value of data 
 \param			iDataSize		Integer Variable to hold the value of data size		
 \return		PIDataInfo*
 */
/*****************************************************************************/

PIDataInfo* GetIECDT(char* dataStr, INT32 iDataSize)
{
	PIDataInfo *stDataInfo = new PIDataInfo;
	stDataInfo->_dt_Name = new char[strlen(dataStr) + 1];
	strcpy(stDataInfo->_dt_Name, dataStr);

	if (NULL != dataStr)
	{
		if (!strcmp(dataStr, "BITSTRING"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = BITSTRING;
		}
		else if (!strcmp(dataStr, "BOOL"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = BOOL;
		}
		else if (!strcmp(dataStr, "_CHAR"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = _CHAR;
		}
		else if (!strcmp(dataStr, "WORD"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = BYTE;
		}
		else if (!strcmp(dataStr, "DWORD"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = DWORD;
		}
		else if (!strcmp(dataStr, "LWORD"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = LWORD;
		}
		else if (!strcmp(dataStr, "SINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = SINT;
		}
		else if (!strcmp(dataStr, "INT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = INT;
		}
		else if (!strcmp(dataStr, "DINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = DINT;
		}
		else if (!strcmp(dataStr, "LINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = LINT;
		}
		else if (!strcmp(dataStr, "USINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = USINT;
		}
		else if (!strcmp(dataStr, "UINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = UINT;
		}
		else if (strcmp(dataStr, "UDINT") == 0)
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = UDINT;
		}
		else if (!strcmp(dataStr, "ULINT"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = ULINT;
		}
		else if (!strcmp(dataStr, "REAL"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = REAL;
		}
		else if (!strcmp(dataStr, "LREAL"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = LREAL;
		}
		else if (!strcmp(dataStr, "STRING"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = STRING;
		}
		else if (!strcmp(dataStr, "WSTRING"))
		{
			stDataInfo->DataSize = iDataSize;
			stDataInfo->_dt_enum = WSTRING;
		}
		else
		{
			//TODO: else added. operation to be specified
		}
	}
	return stDataInfo;
}

/*****************************************************************************/
/**
 \brief			CheckIfModuleExists
 
 This function checks whether the module is present or not
 
 \param			moduleName        Character Pointer Variable to hold the value of Module Name      
 \param			moduleNo            Integer Variable to hold the value of Module number  
 \param			iNoOfModules        Integer Variable to hold the value of NUmber of modules 
 \param			astModCol           Structure Class Variable of ModuleCol
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/
//TODO: Unused function
bool CheckIfModuleExists(char* moduleName, INT32 &moduleNo,
		INT32 iNoOfModules, ModuleCol astModCol[])
{
	for (INT32 iLoopCount = 0; iLoopCount <= iNoOfModules; iLoopCount++)
	{
		if (!strcmp(moduleName, astModuleInfo[iLoopCount].ModuleName))
		{
			moduleNo = astModCol[iLoopCount].ModuleNo;
			return true;
		}
	}
	return false;
}

/*****************************************************************************/
/**
 \brief			GenerateXAPHeaderFile
 
 This function generates the header file
 
 \param			fileName
 \param			objPIInCol
 \param			objPIOutCol
 \param			iInVar
 \param			iOutVar
 
 \return		void	
 */
/*****************************************************************************/

void GenerateXAPHeaderFile(char* fileName, ProcessImage objPIInCol[],
		ProcessImage objPIOutCol[], INT32 iInVar, INT32 iOutVar)
{
	char* xapFileName = new char[strlen(fileName) + ALLOC_BUFFER];
	FILE* xapFile = new FILE();

	strcpy(xapFileName, fileName);
	strcat(xapFileName, ".h");

	if (NULL == (xapFile = fopen(xapFileName, "w+ ")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] xapFileName;
		throw ex;
	}

	//write comment
	char* varComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN
			+ STR_ALLOC_BUFFER + 8]; // 8 is for comment lines
	strcpy(varComment, "/* ");
	strcat(varComment, BUILD_COMMENT);
	strcat(varComment, GetBuildTime());
	strcat(varComment, " */\n");
	UINT32 uiStrLength = strlen(varComment);
	if ((uiStrLength != fwrite(varComment, sizeof(char), uiStrLength, xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] varComment;
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}
	delete[] varComment;
	//write include guard for the headerfile
	char* xapHeaderIncludeGuard = new char[strlen(INCLUDE_GUARD_CHECK)
			+ strlen(INCLUDE_GUARD_START) + ALLOC_BUFFER];
	if (NULL == xapHeaderIncludeGuard)
	{
#if defined DEBUG
		cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}
	strcpy(xapHeaderIncludeGuard, INCLUDE_GUARD_CHECK);
	strcat(xapHeaderIncludeGuard, "\n");
	strcat(xapHeaderIncludeGuard, INCLUDE_GUARD_START);
	strcat(xapHeaderIncludeGuard, "\n\n");
	uiStrLength = strlen(xapHeaderIncludeGuard);

	if ((uiStrLength
			!= fwrite(xapHeaderIncludeGuard, sizeof(char), uiStrLength,
					xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] xapHeaderIncludeGuard;
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}

	delete[] xapHeaderIncludeGuard;
	/* write Input structure */
	if (0 != iInVar)
	{
		WriteXAPHeaderContents(objPIInCol, iInVar, INPUT, xapFile);
	}

	fclose(xapFile);

	/* write Output structure */
	if (NULL == (xapFile = fopen(xapFileName, "a+")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] xapFileName;
		throw ex;
	}
	delete[] xapFileName;
	if (0 != iOutVar)
	{
		WriteXAPHeaderContents(objPIOutCol, iOutVar, OUTPUT, xapFile);
	}

	//close include guard for the XAP header file
	xapHeaderIncludeGuard =
			new char[strlen(INCLUDE_GUARD_END) + ALLOC_BUFFER];

	strcpy(xapHeaderIncludeGuard, "\n");
	strcat(xapHeaderIncludeGuard, INCLUDE_GUARD_END);
	uiStrLength = strlen(xapHeaderIncludeGuard);
	if ((uiStrLength
			!= fwrite(xapHeaderIncludeGuard, sizeof(char), uiStrLength,
					xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] xapHeaderIncludeGuard;
		throw ex;
	}

	fclose(xapFile);
	delete[] xapHeaderIncludeGuard;
}

/*****************************************************************************/
/**
 \brief			WriteXAPHeaderContents
 
 This function writes content into header file
 
 \param			objProcessImage       Class Variable of ProcessImage to hold the process image value
 \param			iNumberOfVars         Integer variable to hold the value of total number of variables 
 \param			dirType           Enum Variable of  EPIDirectionType
 \param			xapHeader           File Pointer Varible to hold the header file
 \return		void	
 */
/*****************************************************************************/

void WriteXAPHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars,
		EPIDirectionType dirType, FILE* xapHeader)
{
	char* iBuffer = new char[HEADER_FILE_BUFFER];
	char* iBuffer1 = new char[200];

	if (0 != iNumberOfVars)
	{
		strcpy(iBuffer, "\ntypedef struct \n{\n");
		INT32 iHoleFilledIdNo = 1;
		INT32 iTotalsize = 0;
		for (INT32 iLoopCount = 0; iLoopCount < iNumberOfVars; iLoopCount++)
		{
			//INT32 iNodeId;
			INT32 iDataSize = 0;
			char* strCNID = new char[50];
			char* modName = new char[50];
			char* strModuleNo = new char[20];

			iDataSize = objProcessImage[iLoopCount].DataInfo.DataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if ((0 == iDataSize % 32) || (0 == iDataSize % 16)
					|| (0 == iDataSize % 8))
			{
				if (0 != iTotalsize % iDataSize)
				{
					char abHoleid[20];
					char* fbits = new char[2 + ALLOC_BUFFER];
					INT32 iFilledBits = iDataSize - (iTotalsize % iDataSize);

					iTotalsize = iTotalsize + iFilledBits;
					strcat(iBuffer, "\tunsigned");
					strcat(iBuffer, " ");
					strcat(iBuffer, "PADDING_VAR_");
					strcat(iBuffer,
							IntToAscii(iHoleFilledIdNo, abHoleid, 10));
					strcat(iBuffer, ":");
					fbits = IntToAscii(iFilledBits, fbits, 10);
					strcat(iBuffer, fbits);
					strcat(iBuffer, ";\n");
					iHoleFilledIdNo = iHoleFilledIdNo + 1;
					delete[] fbits;
				}
			}

			//iNodeId = objProcessImage[iLoopCount].CNNodeID;

			strCNID = IntToAscii(objProcessImage[iLoopCount].CNNodeID,
					strCNID, 10);
			strcpy(strModuleNo,
					SubString(objProcessImage[iLoopCount].ModuleIndex, 2, 2));
			strcpy(modName, objProcessImage[iLoopCount].ModuleName);

			strcat(iBuffer, "\tunsigned");
			strcat(iBuffer, " ");
			char* varName = new char[100];
			strcpy(varName, "CN");
			strCNID = IntToAscii(objProcessImage[iLoopCount].CNNodeID,
					strCNID, 10);
			strcat(varName, strCNID);
			strcat(varName, "_");

			/* Add Mod NO*/
			strcat(varName, "M");
			strcat(varName, strModuleNo);
			strcat(varName, "_");

			strcat(varName, modName);
			strcat(varName, "_");
			strcat(varName, objProcessImage[iLoopCount].VarName);

			strcat(iBuffer, varName);
			strcat(iBuffer, ":");

			char* iBuff = new char[50];
			iBuff = IntToAscii(iDataSize, iBuff, 10);
			iTotalsize = iDataSize + iTotalsize;
			strcat(iBuffer, iBuff);
			strcat(iBuffer, ";");

			strcat(iBuffer, "\n");
			delete[] varName;
			delete[] modName;
			delete[] strModuleNo;
			delete[] strCNID;
		}
		/* Check if the whole struct is 32 bit aligned*/
		if (0 != iTotalsize % 32)
		{
			char abHoleid[20];
			char* fbits = new char[2 + ALLOC_BUFFER];
			INT32 iFilledBits = 32 - (iTotalsize % 32);

			iTotalsize = iTotalsize + iFilledBits;
			strcat(iBuffer, "\tunsigned");
			strcat(iBuffer, " ");
			strcat(iBuffer, "PADDING_VAR_");
			strcat(iBuffer, IntToAscii(iHoleFilledIdNo, abHoleid, 10));
			strcat(iBuffer, ":");
			fbits = IntToAscii(iFilledBits, fbits, 10);
			strcat(iBuffer, fbits);
			strcat(iBuffer, ";\n");
			//iHoleFilledIdNo = iHoleFilledIdNo + 1; Unused operation

			delete[] fbits;
		}
		strcat(iBuffer, "}");

		if (INPUT == dirType)
		{
			strcpy(iBuffer1, "# define COMPUTED_PI_OUT_SIZE ");
			strcat(iBuffer, " PI_OUT;");
		}
		else if (OUTPUT == dirType)
		{

			strcpy(iBuffer1, "\n\n# define COMPUTED_PI_IN_SIZE ");
			strcat(iBuffer, " PI_IN;");
			strcat(iBuffer, "\n");
		}
		else
		{
			//Nothing to do
		}

		char* strSize = new char[50];

		/* write the size in bytes*/
		iTotalsize = iTotalsize / 8;
		strSize = IntToAscii(iTotalsize, strSize, 10);
		strcat(iBuffer1, strSize);
		delete[] strSize;
	}
	else
	{
		strcpy(iBuffer1, "");
	}

	UINT32 uiStrLength = strlen(iBuffer1);

	if ((uiStrLength
			!= fwrite(iBuffer1, sizeof(char), uiStrLength, xapHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] iBuffer;
		delete[] iBuffer1;
		throw ex;
	}
	delete[] iBuffer1;

	uiStrLength = strlen(iBuffer);

	if ((uiStrLength != fwrite(iBuffer, sizeof(char), uiStrLength, xapHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] iBuffer;
		throw ex;
	}
	delete[] iBuffer;
}

/*****************************************************************************/
/**
 \brief			GenerateNETHeaderFile
 
 This function generates header file interface
 
 \param			fileName          Character POinter Variable to hold the value of file name
 \param			objPIInCol          Class variable of ProcessImage
 \param			objPIOutCol         Class variable of ProcessImage
 \param			iInVar              Integer Variable to hold input value   
 \param			iOutVar             Integer Variable to hold output value 
 \return		void	
 */
/*****************************************************************************/

void GenerateNETHeaderFile(char* fileName, ProcessImage objPIInCol[],
		ProcessImage objPIOutCol[], INT32 iInVar, INT32 iOutVar)
{
	char* netFileName = new char[strlen(fileName) + ALLOC_BUFFER];
	FILE* netFile = new FILE();

	strcpy(netFileName, fileName);
	strcat(netFileName, ".cs");
	/* write Input structure */
	if (NULL == (netFile = fopen(netFileName, "w+")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] netFileName;
		throw ex;
	}
	delete[] netFileName;
	NodeCollection* objNodeCol = NULL;
	Node* objNode;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	for (INT32 iNodeColLoopCount = 0;
			iNodeColLoopCount < objNodeCol->GetNumberOfNodes();
			iNodeColLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iNodeColLoopCount);
		objNode->DeleteCollectionsForNETPI();
	}
	if ((0 != iInVar) || (0 != iOutVar))
	{
		/*Writng Header of .NET Interface*/
		char* iBuffer = new char[HEADER_FILE_BUFFER];
		strcpy(iBuffer, "using System;\n");
		strcat(iBuffer, "using System.Runtime.InteropServices;\n");
		//writing comments
		strcat(iBuffer, "/// <summary>\n");
		strcat(iBuffer, "/// ");
		strcat(iBuffer, BUILD_COMMENT);
		strcat(iBuffer, GetBuildTime());
		strcat(iBuffer, "\n");
		strcat(iBuffer, "/// </summary>\n");

		strcat(iBuffer, "\nnamespace openPOWERLINK\n");
		strcat(iBuffer, "{\n");
		UINT32 uiStrLength = strlen(iBuffer);

		if ((uiStrLength
				!= fwrite(iBuffer, sizeof(char), uiStrLength, netFile)))
		{
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
			fclose(netFile);
			delete[] iBuffer;
			throw ex;
		}
		delete[] iBuffer;
	}
	if (0 != iInVar)
	{
		WriteNETHeaderContents(objPIInCol, iInVar, INPUT, netFile);
	}

	/* write Output structure */
	if (0 != iOutVar)
	{
		WriteNETHeaderContents(objPIOutCol, iOutVar, OUTPUT, netFile);
	}
	if ((0 != iInVar) || (0 != iOutVar))
	{
		char* iBuffer = new char[10];
		strcpy(iBuffer, "}\n");
		UINT32 uiStrLength = strlen(iBuffer);

		if ((uiStrLength
				!= fwrite(iBuffer, sizeof(char), uiStrLength, netFile)))
		{
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
			fclose(netFile);
			delete[] iBuffer;
			throw ex;
		}
		delete[] iBuffer;
	}
	fclose(netFile);
}

/*****************************************************************************/
/**
 \brief			WriteNETHeaderContents
 
 This function writes header contents
 
 \param			objProcessImage       Class Variable of ProcessImage
 \param			iNumberOfVars         Integer Variabl to hold the value of number of variables 
 \param			dirType           Enum Variable of EPIDirectionType 
 \param			netHeader           File Pointer Variable to hold the header
 \return		void	
 */
/*****************************************************************************/

void WriteNETHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars,
		EPIDirectionType dirType, FILE* netHeader)
{

	char* iBuffer = new char[HEADER_FILE_BUFFER];
	iBuffer[0] = 0;
	char* iBuffer1 = new char[200];
	char* iBuffer2 = new char[500];
	char Offset[10];
	INT32 iOffset = 0;
	NETProcessImage *aobjNetPiCol = NULL;
	INT32 iTotalsize = GroupNETHeaderContents(objProcessImage, iNumberOfVars,
			dirType, netHeader);
	//NETProcessImage aobjNetPiCol[PI_VAR_COUNT] =	{ };
	aobjNetPiCol = new NETProcessImage[PI_VAR_COUNT];
	INT32 netPIVarsCount = GroupNETPIVariables(dirType, aobjNetPiCol);
	for (INT32 iLoopCount = 0; iLoopCount < netPIVarsCount; iLoopCount++)
	{
		strcat(iBuffer, "\t\t[FieldOffset(");
		memset(Offset, 0, 10);
		IntToAscii(iOffset, Offset, 10);
		strcat(iBuffer, Offset);
		strcat(iBuffer, ")]\n");
		strcat(iBuffer, "\t\tpublic ");
		strcat(iBuffer,
				GetDatatypeNETPI(aobjNetPiCol[iLoopCount].DataInfo._dt_enum));
		iOffset += GetDatasizeNETPI(aobjNetPiCol[iLoopCount].DataInfo._dt_enum);
		strcat(iBuffer, " ");
		strcat(iBuffer, aobjNetPiCol[iLoopCount].ModuleName);
		strcat(iBuffer, "_");
		strcat(iBuffer, aobjNetPiCol[iLoopCount].Name);
		if (aobjNetPiCol[iLoopCount].count > 0)
		{
			strcat(iBuffer, "_to_");
			strcat(iBuffer, aobjNetPiCol[iLoopCount].LastName);
		}
		strcat(iBuffer, ";\n");

	}
	delete[] aobjNetPiCol;
	char* abTotalsize = new char[20];
	IntToAscii(iTotalsize, abTotalsize, 10);

	if (INPUT == dirType)
	{
		strcpy(iBuffer1, "\n\t/// <summary>\n");
		strcat(iBuffer1, "\t/// Struct : ProcessImage Out\n");
		strcat(iBuffer1, "\t/// </summary>\n");
		strcat(iBuffer1,
				"\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
		strcat(iBuffer1, abTotalsize);
		strcat(iBuffer1, ")]\n");
		strcat(iBuffer1, "\tpublic struct AppProcessImageOut\n");
		strcat(iBuffer1, "\t{\n");

		strcpy(iBuffer2, "\t}\n");
	}
	else if (OUTPUT == dirType)
	{

		strcpy(iBuffer1, "\n\t/// <summary>\n");
		strcat(iBuffer1, "\t/// Struct : ProcessImage In\n");
		strcat(iBuffer1, "\t/// </summary>\n");
		strcat(iBuffer1,
				"\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
		strcat(iBuffer1, abTotalsize);
		strcat(iBuffer1, ")]\n");
		strcat(iBuffer1, "\tpublic struct AppProcessImageIn\n");
		strcat(iBuffer1, "\t{\n");

		strcpy(iBuffer2, "\t}\n");
	}
	else
	{
		//TODO: Else added. Nothing to do
	}
	delete[] abTotalsize;

	UINT32 uiStrLength = strlen(iBuffer1);

	if ((uiStrLength
			!= fwrite(iBuffer1, sizeof(char), uiStrLength, netHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] iBuffer;
		delete[] iBuffer1;
		delete[] iBuffer2;
		throw ex;
	}
	delete[] iBuffer1;

	uiStrLength = strlen(iBuffer);

	if ((uiStrLength != fwrite(iBuffer, sizeof(char), uiStrLength, netHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] iBuffer2;
		delete[] iBuffer;
		throw ex;
	}
	delete[] iBuffer;

	uiStrLength = strlen(iBuffer2);

	if ((uiStrLength
			!= fwrite(iBuffer2, sizeof(char), uiStrLength, netHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] iBuffer2;
		throw ex;
	}
	delete[] iBuffer2;

}

/*****************************************************************************/
/**
 \brief			GroupNETHeaderContents
 
 This Function groups the header contents
 
 This function writes header contents
 
 \param			objProcessImage       Class Variable of ProcessImage
 \param			iNumberOfVars         Integer Variabl to hold the value of number of variables 
 \param			dirType           Enum Variable of EPIDirectionType 
 \param			netHeader           File Pointer Variable to hold the header
 
 \return		INT32	
 */
/*****************************************************************************/

INT32 GroupNETHeaderContents(ProcessImage objProcessImage[],
		INT32 iNumberOfVars, EPIDirectionType dirType, FILE* netHeader)
{

	INT32 iTotalsize = 0;

	if (0 != iNumberOfVars)
	{
		//iModuleNo = 0;
		INT32 iHoleFilledIdNo = 1;
		NodeCollection* objNodeCol = NULL;
		Node* objNode = NULL;
		objNodeCol = NodeCollection::GetNodeColObjectPointer();

		for (INT32 iLoopCount = 0; iLoopCount < iNumberOfVars; iLoopCount++)
		{
			objNode = objNodeCol->GetNodePtr(CN,
					objProcessImage[iLoopCount].CNNodeID);
			//INT32 iNodeId;
			INT32 iDataSize = 0;
			char* strCNID = new char[50];
			char* modName = new char[50];
			char* strModuleNo = new char[20];

			iDataSize = objProcessImage[iLoopCount].DataInfo.DataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if ((0 == iDataSize % 32) || (0 == iDataSize % 16)
					|| (0 == iDataSize % 8))
			{
				if (0 != iTotalsize % iDataSize)
				{
					char* abHoleid = new char[20];
					char* fBits = new char[2 + ALLOC_BUFFER];
					INT32 iFilledBits = iDataSize - (iTotalsize % iDataSize);

					iTotalsize = iTotalsize + iFilledBits;
					for (INT32 iLoopCount = 1; iLoopCount <= (iFilledBits / 8);
							iLoopCount++)
					{
						strcpy(abHoleid, "");
						strcpy(fBits, "");
						NETProcessImage objNETProcessImage;
						abHoleid = IntToAscii(iHoleFilledIdNo, abHoleid, 10);
						objNETProcessImage.Name = new char[strlen(abHoleid)
								+ ALLOC_BUFFER];
						strcpy(objNETProcessImage.Name, abHoleid);
						objNETProcessImage.ModuleName = new char[strlen(
								"PADDING_VAR") + ALLOC_BUFFER];
						strcpy(objNETProcessImage.ModuleName, "PADDING_VAR");

						objNETProcessImage.count = 0;
						objNETProcessImage.DataInfo._dt_enum = BYTE;
						objNETProcessImage.DataInfo.DataSize = 8;
						objNETProcessImage.DataInfo._dt_Name = new char[strlen(
								"byte") + ALLOC_BUFFER];
						strcpy(objNETProcessImage.DataInfo._dt_Name, "byte");
						objNETProcessImage.iTotalDataSize = 8;
						objNETProcessImage.DirectionType = dirType;
						objNode->AddNETProcessImage(objNETProcessImage);
						iHoleFilledIdNo = iHoleFilledIdNo + 1;
					}
					delete[] fBits;
					delete[] abHoleid;
				}
			}

			//iNodeId = objProcessImage[iLoopCount].CNNodeID;

			strCNID = IntToAscii(objProcessImage[iLoopCount].CNNodeID,
					strCNID, 10);
			strcpy(strModuleNo,
					SubString(objProcessImage[iLoopCount].ModuleIndex, 2, 2));
			strcpy(modName, objProcessImage[iLoopCount].ModuleName);

			char* varName = new char[100];
			strcpy(varName, "CN");
			strCNID = IntToAscii(objProcessImage[iLoopCount].CNNodeID,
					strCNID, 10);
			strcat(varName, strCNID);
			strcat(varName, "_");

			/* Add Mod NO*/
			strcat(varName, "M");
			strcat(varName, strModuleNo);
			strcat(varName, "_");

			strcat(varName, modName);

			if (objProcessImage[iLoopCount].DataInfo._dt_enum == BITSTRING)
			{
				INT32 iPIIndexCount = 0;
				INT32 iNetPIIndex;
				bool NetPICreated = false;
				do
				{
					iNetPIIndex = SearchModuleNameNETProcessImageCollection(
							objProcessImage[iLoopCount].CNNodeID, iPIIndexCount,
							varName);
					if (-1 == iNetPIIndex)
					{
						//create new
						NetPICreated = true;
						NETProcessImage objNETProcessImage;
						objNETProcessImage.Initialize();
						CopyPItoNETPICollection(objProcessImage[iLoopCount],
								objNETProcessImage, varName);

					}
					else if (8
							> objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize)
					{
						NetPICreated = true;
						objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize =
								objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize
										+ iDataSize;
						objNode->NETProcessImageCollection[iNetPIIndex].count++;
						objNode->NETProcessImageCollection[iNetPIIndex].LastName =
								new char[strlen(
										objProcessImage[iLoopCount].VarName)
										+ ALLOC_BUFFER];
						strcpy(
								objNode->NETProcessImageCollection[iNetPIIndex].LastName,
								objProcessImage[iLoopCount].VarName);
						if (8
								== objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize)
						{
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_enum =
									BYTE;
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name =
									new char[strlen("byte") + ALLOC_BUFFER];
							strcpy(
									objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name,
									"byte");
						}
						else if (16
								== objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize)
						{
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_enum =
									UINT;
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name =
									new char[strlen("UInt16") + ALLOC_BUFFER];
							strcpy(
									objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name,
									"UInt16");
						}
						else if (32
								== objNode->NETProcessImageCollection[iNetPIIndex].iTotalDataSize)
						{
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_enum =
									UDINT;
							objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name =
									new char[strlen("UInt32") + ALLOC_BUFFER];
							strcpy(
									objNode->NETProcessImageCollection[iNetPIIndex].DataInfo._dt_Name,
									"UInt32");
						}
						else
						{
							//TODO: else added. Operation to be specified
						}
					}
					else
					{
						iPIIndexCount = iNetPIIndex + 1;
					}
				} while (false == NetPICreated);
			}
			else
			{
				//copy as it is
				NETProcessImage objNETProcessImage;
				objNETProcessImage.Initialize();
				CopyPItoNETPICollection(objProcessImage[iLoopCount],
						objNETProcessImage, varName);
			}

			iTotalsize = iDataSize + iTotalsize;

			delete[] varName;
			delete[] modName;
			delete[] strModuleNo;
			delete[] strCNID;
		}
		if (0 != iTotalsize % 32)
		{
			char* abHoleid = new char[20];
			char* fBits = new char[2 + ALLOC_BUFFER];
			INT32 iFilledBits = 32 - (iTotalsize % 32);

			iTotalsize = iTotalsize + iFilledBits;
			for (INT32 iLoopCount = 1; iLoopCount <= (iFilledBits / 8);
					iLoopCount++)
			{
				strcpy(abHoleid, "");
				strcpy(fBits, "");
				NETProcessImage objNETProcessImage;
				abHoleid = IntToAscii(iHoleFilledIdNo, abHoleid, 10);
				objNETProcessImage.Name = new char[strlen(abHoleid)
						+ ALLOC_BUFFER];
				strcpy(objNETProcessImage.Name, abHoleid);
				objNETProcessImage.ModuleName = new char[strlen("PADDING_VAR")
						+ ALLOC_BUFFER];
				strcpy(objNETProcessImage.ModuleName, "PADDING_VAR");

				objNETProcessImage.count = 0;
				objNETProcessImage.DataInfo._dt_enum = BYTE;
				objNETProcessImage.DataInfo.DataSize = 8;
				objNETProcessImage.DataInfo._dt_Name = new char[strlen("byte")
						+ ALLOC_BUFFER];
				strcpy(objNETProcessImage.DataInfo._dt_Name, "byte");
				objNETProcessImage.iTotalDataSize = 8;
				objNETProcessImage.DirectionType = dirType;
				objNode->AddNETProcessImage(objNETProcessImage);
				iHoleFilledIdNo = iHoleFilledIdNo + 1;
			}
			delete[] fBits;
			delete[] abHoleid;
		}
	}

	iTotalsize = iTotalsize / 8;

	return iTotalsize;
}

/*****************************************************************************/
/**
 \brief			SetSIdxDataType
 
 This fumction sets index for all data types
 
 \param			objDataType     Class Pointer variable of DataType
 \param			idx            Character Pointer Variable to hold the value of Index
 \param			sIdx           Character Pointer Variable to hold the value of Sub Index
 
 \return		void	
 */
/*****************************************************************************/

void SetSIdxDataType(DataType *objDataType, char* idx, char* sIdx)
{
	if ((NULL == objDataType) || (NULL == idx) || (NULL == sIdx))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	NodeCollection *objNodeCollection = NULL;
	IndexCollection *objIndexCol = NULL;
	Index *objIndex = NULL;
	SubIndex *objSIdx = NULL;
	Node objNode;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();

	objNode = objNodeCollection->GetMNNode();
	objIndexCol = objNode.GetIndexCollection();

	objIndex = objIndexCol->GetIndexbyIndexValue(idx);
	objSIdx = objIndex->GetSubIndexbyIndexValue(sIdx);

	objSIdx->SetDataTypeST(*objDataType);
}

/*****************************************************************************/
/**
 \brief			AddPDOIndexsToMN
 
 This function adds PDO indexes to MN
 
 \param			iIndex		Character Pointer Variable to hold the value of Index
 \param			subIndex	Character Pointer Variable to hold the value of Sub Index
 \param			pdoType		Enum Variable of  EPDOType
 
 \return		void	
 */
/*****************************************************************************/

void AddPDOIndexsToMN(char* iIndex, char* subIndex, PDOType pdoType)
{
	ocfmRetCode stRetCode;
	Node objMNNode;
	DataType *objDataType = NULL;
	Index *objIndex = NULL;
	NodeCollection *objNodeCol = NULL;
	IndexCollection *objIdxCol = NULL;
	DataTypeCollection *objDTCol = NULL;
	char *varName = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objMNNode = objNodeCol->GetNode(MN, MN_NODEID);
	objDTCol = objMNNode.GetDataTypeCollection();

	stRetCode = AddIndex(MN_NODEID, MN, iIndex);
	objIdxCol = objMNNode.GetIndexCollection();
	if (NULL != objIdxCol)
	{
		objIndex = objIdxCol->GetIndexbyIndexValue(iIndex);
		if (NULL != objIndex
				&& (OCFM_ERR_INDEX_ALREADY_EXISTS != stRetCode.code))
		{
			char abObjectName[14 + ALLOC_BUFFER];
			objIndex->SetObjectType((char*) "ARRAY");
			//name from network point of view
			if (PDO_RPDO == pdoType)
			{
				strcpy(abObjectName, "PI_INPUTS_A");
				strcat(abObjectName, GetPIName(iIndex));
				objIndex->SetName(abObjectName);
			}
			else if (PDO_TPDO == pdoType)
			{
				strcpy(abObjectName, "PI_OUTPUTS_A");
				strcat(abObjectName, GetPIName(iIndex));
				objIndex->SetName(abObjectName);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _1\n" << endl;
			}
			/* Add subindex 00 */
			stRetCode = AddSubIndex(MN_NODEID, MN, iIndex, (char*) "00");
			if (OCFM_ERR_SUCCESS != stRetCode.code)
			{
				ocfmException objException;
				objException.OCFMException(stRetCode.code);
				throw objException;
			}
		}
	}

	stRetCode = AddSubIndex(MN_NODEID, MN, iIndex, subIndex);
	if (OCFM_ERR_SUCCESS != stRetCode.code)
	{
		//TODO: Error occurred once while mapping tpdo from cn to mn xdc
		cout<<__LINE__<<" add subindex error in line"<<endl;
		ocfmException objException;
		objException.OCFMException(stRetCode.code);
		throw objException;
	}

	if (NULL != objIndex)
	{
		SubIndex *objSIdx = NULL;
		objSIdx = objIndex->GetSubIndexbyIndexValue(subIndex);
		if (NULL != objSIdx)
		{
			char abObjectName[14 + ALLOC_BUFFER];
			char *pdoMap = new char[4 + ALLOC_BUFFER];
			objSIdx->SetObjectType((char*) "VAR");
			/* Its reversed because CN's RPDO is MN's TPDO*/
			if (PDO_RPDO == pdoType)
			{
				strcpy(pdoMap, "TPDO");
				strcpy(abObjectName, "PI_INPUTS_");
				strcat(abObjectName, GetPIName(iIndex));
				objSIdx->SetAccessType((char*) "ro");

				objSIdx->SetName(abObjectName);
				objSIdx->SetPDOMapping(pdoMap);
			}
			else if (PDO_TPDO == pdoType)
			{
				strcpy(pdoMap, "RPDO");
				strcpy(abObjectName, "PI_OUTPUTS_");
				strcat(abObjectName, GetPIName(iIndex));
				objSIdx->SetAccessType((char*) "rw");

				objSIdx->SetName(abObjectName);
				objSIdx->SetPDOMapping(pdoMap);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _2\n" << endl;
			}
			delete[] pdoMap;
		}
	}
	varName = GetPIDataTypeName(iIndex);

	if (NULL != varName)
	{
		objDataType = objDTCol->GetDataTypeByName(varName);
	}

	if ((NULL != objDataType) && (NULL != iIndex) && (NULL != subIndex))
	{
		SetSIdxDataType(objDataType, iIndex, subIndex);
	}

	if (NULL != objIndex)
	{
		UpdateNumberOfEnteriesSIdx(objIndex, MN);
	}
}

/*****************************************************************************/
/**
 \brief			GetPIAddress
 
 This function sets process image address
 
 \param			dt                 Class Variable of PDODataType to hold the datatype
 \param			dirType            Class Variable of EPIDirectionType to hold the value of direction type
 \param			iOffset            Integer Variable to hold offset value
 \param			iDataSize          Integer Varibale to hold the value of datasize   
 
 \return		PIObject	
 */
/*****************************************************************************/

PIObject GetPIAddress(PDODataType dt, EPIDirectionType dirType, INT32 iOffset,
		INT32 iDataSize)
{
	INT32 idx;
	INT32 subIndex;
	PIObject stPIObject;
#if defined DEBUG
	cout << __FUNCTION__ << ":: Datatype:" << dt << " DirectionType:" << dirType << " iOffset:" << iOffset << " iDataSize:" << iDataSize << endl;
#endif
	stPIObject.Index = new char[INDEX_LEN];
	stPIObject.SubIndex = new char[SUBINDEX_LEN];

	for (idx = 0; idx < NO_OF_PI_ENTERIES; idx++)
	{
		if ((AddressTable[idx].dt == dt)
				&& (AddressTable[idx].Direction == dirType))
		{
			INT32 iDataSizeBytes = iDataSize / 8;
			subIndex = (iOffset / iDataSizeBytes) + 1;
			if (subIndex > 254)
			{
				INT32 div = subIndex / 254;
				INT32 mod = subIndex % 254;
				INT32 iAddress;
				iAddress = HexToInt(AddressTable[idx].Address);
				iAddress = iAddress + div;
				stPIObject.Index = IntToAscii(iAddress, stPIObject.Index, 16);
				stPIObject.SubIndex = IntToAscii(mod, stPIObject.SubIndex, 16);
				stPIObject.SubIndex = PadLeft(stPIObject.SubIndex, '0', 2);

			}
			else
			{
				strcpy(stPIObject.Index, AddressTable[idx].Address);
				stPIObject.SubIndex = IntToAscii(subIndex, stPIObject.SubIndex,
						16);
				stPIObject.SubIndex = PadLeft(stPIObject.SubIndex, '0', 2);

			}
		}
	}
	//Handled error case and returned dummy value to avoid warning
	return stPIObject;
}

/*****************************************************************************/
/**
 \brief			GetPIDataTypeName
 
 This function sets value for process image datatype
 
 \param			iAddress       Character Pointer Variable to hold the Process image address
 \return		char*	
 */
/*****************************************************************************/

char* GetPIDataTypeName(char* iAddress)
{
	char *retString = NULL;
	PDODataType dt;
	for (INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{
		if (0 == strcmp(AddressTable[iLoopCount].Address, iAddress))
		{
			dt = AddressTable[iLoopCount].dt;
		}
		else if (0 < strcmp(AddressTable[iLoopCount].Address, iAddress))
		{
			dt = (0 < iLoopCount) ?
					AddressTable[iLoopCount - 1].dt :
					static_cast<PDODataType>(-1);
		}
		else
		{
			//TODO: else added. operation to be specified
		}

		switch (dt)
		{
		case UNSIGNED8:
		{
			retString = (char*) "Unsigned8";
			break;
		}
		case INTEGER8:
		{
			retString = (char*) "Integer8";
			break;
		}
		case UNSIGNED16:
		{
			retString = (char*) "Unsigned16";
			break;
		}
		case INTEGER16:
		{
			retString = (char*) "Integer8";
			break;
		}
		case UNSIGNED32:
		{
			retString = (char*) "Unsigned32";
			break;
		}
		case INTEGER32:
		{
			retString = (char*) "Integer32";
			break;
		}
		case INTEGER64:
		{
			retString = (char*) "Integer64";
			break;
		}
		case UNSIGNED64:
		{
			retString = (char*) "Unsigned64";
			break;
		}
		default:
		{
			//Handled error case and returned dummy value to avoid warning
			retString = (char*) "Error";
			break;
		}
		}

	}
	return retString;
}

/*****************************************************************************/
/**
 \brief			GetPIName
 
 This function returns process image name
 
 \param			iAddress  Character Pointer Variable to hold the Process image address
 \return		char*	
 */
/*****************************************************************************/

char* GetPIName(char* iAddress)
{
	char *retString = NULL;
	PDODataType dt;
	for (INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{
		if (0 == strcmp(AddressTable[iLoopCount].Address, iAddress))
		{
			dt = AddressTable[iLoopCount].dt;
		}
		else if (0 < strcmp(AddressTable[iLoopCount].Address, iAddress))
		{
			dt = (0 < iLoopCount) ?
					AddressTable[iLoopCount - 1].dt :
					static_cast<PDODataType>(-1);
		}
		else
		{
			//TOOD: else added, operation to be specified
		}

		switch (dt)
		{
		case UNSIGNED8:
		{
			retString = (char*) "U8";
			break;
		}
		case INTEGER8:
		{
			retString = (char*) "I8";
			break;
		}
		case UNSIGNED16:
		{
			retString = (char*) "U16";
			break;
		}
		case INTEGER16:
		{
			retString = (char*) "I16";
			break;
		}
		case UNSIGNED32:
		{
			retString = (char*) "U32";
			break;
		}
		case INTEGER32:
		{
			retString = (char*) "I32";
			break;
		}
		case UNSIGNED64:
		{
			retString = (char*) "U64";
			break;
		}
		case INTEGER64:
		{
			retString = (char*) "I64";
			break;
		}

		default:
		{
			//Handled error case and returned dummy value to avoid warning
			retString = (char*) "Err";
			break;
		}
		}
	}

	return retString;
}

/*****************************************************************************/
/**
 \brief			CheckIfProcessImageIdx
 
 This function checks for process image index
 
 \param			iIndex		Character Pointer Variable to hold the value of index
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CheckIfProcessImageIdx(char* iIndex)
{
	return ((0 <= strcmp(iIndex, "A000")) && (0 >= strcmp(iIndex, "AFFF")));
}

/*****************************************************************************/
/**
 \brief			SearchModuleNameNETProcessImageCollection
 
 This function collects process image based on the module name
 
 \param			CNNodeID         Integer Variable to hold the node id      
 \param			iItemLoopCount   Integer Variable to hold the value of loop count   
 \param			schModuleName    Character Pointer Variable to hold the name of the search module
 \return		INT32	
 */
/*****************************************************************************/

INT32 SearchModuleNameNETProcessImageCollection(INT32 CNNodeID,
		INT32 iItemLoopCount, char* schModuleName)
{

	NodeCollection* objNodeCol = NULL;
	Node* objNode;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(CN, CNNodeID);

	INT32 iItemCount = objNode->NETProcessImageCollection.Count();
	for (; iItemLoopCount < iItemCount; iItemLoopCount++)
	{
		if (NULL
				== objNode->NETProcessImageCollection[iItemLoopCount].ModuleName)
		{
			continue;
		}

		if (0
				== strcmp(
						objNode->NETProcessImageCollection[iItemLoopCount].ModuleName,
						schModuleName))
		{
			return iItemLoopCount;
		}

	}
	return -1;
}

/*****************************************************************************/
/**
 \brief			CopyPItoNETPICollection
 
 This function copies process image to interface process image collection
 
 \param			objProcessImage               Class Variable of ProcessImage
 \param			objNETProcessImage            Class Variable of NETProcessImage 
 \param			moduleName                    Character Pointer Variable to hold the module name
 \return		void	
 */
/*****************************************************************************/

void CopyPItoNETPICollection(ProcessImage objProcessImage,
		NETProcessImage objNETProcessImage, char* moduleName)
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(CN, objProcessImage.CNNodeID);

	//NETProcessImage objNETProcessImage;
	if (NULL != objProcessImage.VarName)
	{
		objNETProcessImage.Name = new char[strlen(objProcessImage.VarName)
				+ ALLOC_BUFFER];
		strcpy(objNETProcessImage.Name, objProcessImage.VarName);
	}
	objNETProcessImage.ModuleName = new char[strlen(moduleName) + ALLOC_BUFFER];
	strcpy(objNETProcessImage.ModuleName, moduleName);
	objNETProcessImage.DataInfo._dt_enum = objProcessImage.DataInfo._dt_enum;
	objNETProcessImage.DataInfo.DataSize = objProcessImage.DataInfo.DataSize;
	if (NULL != objProcessImage.DataInfo._dt_Name)
	{
		objNETProcessImage.DataInfo._dt_Name = new char[strlen(
				objProcessImage.DataInfo._dt_Name) + ALLOC_BUFFER];
		strcpy(objNETProcessImage.DataInfo._dt_Name,
				objProcessImage.DataInfo._dt_Name);
	}
	objNETProcessImage.CNNodeID = objProcessImage.CNNodeID;
	objNETProcessImage.DirectionType = objProcessImage.DirectionType;
	objNETProcessImage.count = 0;
	objNETProcessImage.iTotalDataSize = objProcessImage.DataInfo.DataSize;

	objNode->AddNETProcessImage(objNETProcessImage);
}

/*****************************************************************************/
/**
 \brief			GetDatatypeNETPI
 
 This function returns data type for process image
 
 \param			varIECdt       Enum Variable of IEC_Datatype to hold the datatype of process image
 \return		char*	
 */
/*****************************************************************************/

char* GetDatatypeNETPI(IEC_Datatype varIECdt)
{
	char* dtNetString;
	switch (varIECdt)
	{
	case BITSTRING:
		dtNetString = (char*) "";
		break;
	case BOOL:
		dtNetString = (char*) "byte";
		break;
	case BYTE:
		dtNetString = (char*) "byte";
		break;
	case _CHAR:
		dtNetString = (char*) "byte";
		break;
	case DWORD:
		dtNetString = (char*) "UInt32";
		break;
	case LWORD:
		dtNetString = (char*) "UInt64";
		break;
	case SINT:
		dtNetString = (char*) "sbyte";
		break;
	case INT:
		dtNetString = (char*) "Int16";
		break;
	case DINT:
		dtNetString = (char*) "Int32";
		break;
	case LINT:
		dtNetString = (char*) "Int64";
		break;
	case USINT:
		dtNetString = (char*) "byte";
		break;
	case UINT:
		dtNetString = (char*) "UInt16";
		break;
	case UDINT:
		dtNetString = (char*) "UInt32";
		break;
	case ULINT:
		dtNetString = (char*) "UInt64";
		break;
	case REAL:
		dtNetString = (char*) "";
		break;
	case LREAL:
		dtNetString = (char*) "";
		break;
	case STRING:
		dtNetString = (char*) "";
		break;
	case WSTRING:
		dtNetString = (char*) "";
		break;
	default:
		dtNetString = (char*) "";
		break;
	}
	return dtNetString;
}

/*****************************************************************************/
/**
 \brief			GetDatasizeNETPI
 
 This function returns data size for process image
 
 \param			varIECdt Enum Variable of IEC_Datatype to hold the datatype of process image
 \return		INT32	
 */
/*****************************************************************************/

INT32 GetDatasizeNETPI(IEC_Datatype varIECdt)
{
	INT32 iDSNetString = 0;
	switch (varIECdt)
	{
	case BOOL:
	case BYTE:
	case _CHAR:
	case SINT:
	case USINT:
		iDSNetString = 1;
		break;
	case INT:
	case UINT:
		iDSNetString = 2;
		break;
	case DINT:
	case DWORD:
	case UDINT:
		iDSNetString = 4;
		break;
	case LINT:
	case LWORD:
	case ULINT:
		iDSNetString = 8;
		break;
	default:
		break;
	}
	return iDSNetString;
}
