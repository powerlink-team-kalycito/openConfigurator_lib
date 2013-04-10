/**
 ************************************************************************************************
 \file			ApplicationProcess.cpp

 \brief			Handles the collection of complex data type 
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
#include "../Include/ApplicationProcess.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

ApplicationProcess::ApplicationProcess(void)
{
	//Add constructor code here
	xddFileName = NULL;
	projectPath = NULL;
}

/*************************************************************************/
/* Destructor */

/**
 

 */

ApplicationProcess::~ApplicationProcess(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			CheckFileStatus
 
 This is a member Function of CApplicationProcess checks the status of the file
 
 \param			fileName	Character Pointer variable hold the value of file status		
 \return	INT32
 */
/*****************************************************************************/

INT32 ApplicationProcess::CheckFileStatus(char* fileName)
{
	ifstream file(fileName, ios::in | ios::binary | ios::ate);

	if (TRUE == (file.is_open()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
/**
 \brief			AddComplexDataType
 
 This ia a member Function of CApplicationProcess which collects complex data type 
 
 \param			objComplexData	 Class variable of CComplexDataType for data type	
 \return	void
 */
/*****************************************************************************/

void ApplicationProcess::AddComplexDataType(ComplexDataType objComplexData)
{
	INT32 iItemPosition = CDTCollection.Add();

	objComplexData.Index = iItemPosition;
	CDTCollection[iItemPosition] = objComplexData;
}

/*****************************************************************************/
/**
 \brief			DeleteComplexDataTypeCollection
 
 This is member Function of CApplicationProcess which clears complex data type collection
 
 \return	void
 */
/*****************************************************************************/

void ApplicationProcess::DeleteComplexDataTypeCollection()
{
	CDTCollection.Clear();
}

/*****************************************************************************/
/**
 \brief			AddParameter
 
 This is a member function of CApplicationProcess to add parameter in the collection list
 
 \param			stParam		Structure Variable of addParameter to add parameter to the collection list
 \return	void
 */
/*****************************************************************************/

void ApplicationProcess::AddParameter(Parameter stParam)
{
	INT32 iItemPosition = ParameterCollection.Add();

	stParam.paraIndex = iItemPosition;
	ParameterCollection[iItemPosition] = stParam;
}

/*****************************************************************************/
/**
 \brief			Deleteparametercollection
 
 This is a member function of CApplicationProcess which clears parameter collection list
 
 \return	void
 */
/*****************************************************************************/

void ApplicationProcess::DeleteParameterCollection()
{
	ParameterCollection.Clear();
}

/*****************************************************************************/
/**
 \brief			GetParameterIndexbyUniqueIDRef
 
 This is a member function of CApplicationProcess to point parameter index with reference to IDs

 \param			uniqueIdRef	Character Pointer variable to hold the value of unique refernce ids	
 \return	INT32
 */
/*****************************************************************************/

INT32 ApplicationProcess::GetParameterIndexbyUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < ParameterCollection.Count();
			iLoopCount++)
	{
		Parameter stParams;
		stParams = ParameterCollection[iLoopCount];
		if (0 == strcmp(uniqueIdRef, stParams.nameIdDtAttr.GetUniqueID()))
		{
			return iLoopCount;
		}
	}
	return 0;
}

/*****************************************************************************/
/**
 \brief			GetParameterbyUniqueIDRef
 
 This is a member function of CApplicationProcess to point parameter with reference to IDs

 \param			uniqueIdRef	Character Pointer variable to hold 	Unique reference ids
 \return	Parameter*
 */
/*****************************************************************************/

Parameter* ApplicationProcess::GetParameterbyUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < ParameterCollection.Count();
			iLoopCount++)
	{
		Parameter stParams;

		stParams = ParameterCollection[iLoopCount];
		if (0 == strcmp(uniqueIdRef, stParams.nameIdDtAttr.GetUniqueID()))
		{
			return &ParameterCollection[iLoopCount];
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			GetCDTUniqueIDRef
 
 This is a member function of CApplicationProcess to point unique reference ids from the collection list

 \param			uniqueIdRef   Character Pointer variable to hold the value of unique reference ids	
 \return	INT32
 */
/*****************************************************************************/

INT32 ApplicationProcess::GetCDTUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		ComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (0 == strcmp(uniqueIdRef, objCDT.nameIdAttr->GetUniqueID()))
		{
			return iLoopCount;
		}
	}
	return 0;
}

/*****************************************************************************/
/**
 \brief			GetCDTbyUniqueID
 
 This is a member function of CApplicationProcess to point CDT values based on unique reference ids from the collection list

 \param			uniqueId	Character Pointer variable to hold the value of unique ids	
 \return	ComplexDataType*
 */
/*****************************************************************************/

ComplexDataType* ApplicationProcess::GetCDTbyUniqueID(char *uniqueId)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		ComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (0 == strcmp(uniqueId, objCDT.nameIdAttr->GetUniqueID()))
		{
			return &CDTCollection[iLoopCount];
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			GetCDTByDtUniqueRefID
 
 This is member function of CApplicationProcess to point complex datatype collection reference to unique ids

 \param			uniqueRefId	Character pointer to hold the value of unique reference ids		
 \return	ComplexDataType*
 */
/*****************************************************************************/

ComplexDataType* ApplicationProcess::GetCDTByDtUniqueRefID(
		char *uniqueRefId)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		ComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (NULL != objCDT.nameIdAttr->GetDtUniqueRefId())
		{
			if (0
					== strcmp(uniqueRefId,
							objCDT.nameIdAttr->GetDtUniqueRefId()))
			{
				return &CDTCollection[iLoopCount];
			}
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			GetCDTByDtIndex
 
 This is a member function of CApplicationProcess to point CDT with reference to index values

 \param			iIndex		Integer variable to hold the Index of CDT collection	
 \return	ComplexDataType*
 */
/*****************************************************************************/

ComplexDataType* ApplicationProcess::GetCDTByDtIndex(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}

/*****************************************************************************/
/**
 \brief			UpdatePreviousCDTUId
 
 This is a member function of CApplicationProcess which updates CDT collection list 

 \param			uniqueID  Character Pointer variable to hold the unique id of CDT collection
 \param			iIndex		Integer variable to hold the Index of CDT collection	
 \return	void
 */
/*****************************************************************************/

void ApplicationProcess::UpdatePreviousCDTUId(char* uniqueID, INT32 iIndex)
{
	ComplexDataType* pobjCDT;

	pobjCDT = CDTCollection.GetAddress(iIndex);
	pobjCDT->previousCDT_UId = new char[strlen(uniqueID) + 1];
	strcpy(pobjCDT->previousCDT_UId, uniqueID);
}

/*****************************************************************************/
/**
 \brief			GetUniqueIDRefbyParameterIndex
 
 This is a member function of CApplicationProcess to point the parameterindex reference to unique ids
 
 \param			iParameterIndex  Integer variable to hold the unique ids.	
 \return	Parameter
 */
/*****************************************************************************/

Parameter ApplicationProcess::GetUniqueIDRefbyParameterIndex(
		INT32 iParameterIndex)
{
	Parameter stParams;

	stParams = ParameterCollection[iParameterIndex];

	return stParams;
}

/*****************************************************************************/
/**
 \brief			GetCDTCount
 
 This is a member function of CApplicationProcess returns CDT collection count

 \return	INT32
 */
/*****************************************************************************/

INT32 ApplicationProcess::GetCDTCount()
{
	return CDTCollection.Count();
}

/*****************************************************************************/
/**
 \brief			GetCDTbyCount
 
 This is a member function of CApplicationProcess returns CDT index collection value 

 \return	ComplexDataType*
 */
/*****************************************************************************/

ComplexDataType* ApplicationProcess::GetCDTbyCount(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}
