/**
************************************************************************************************
\file			DataTypeCollection.cpp

\brief			
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

#include "../Include/DataTypeCollection.h"
#include "../Include/Internal.h"



				//==========================================================================//
				// 				F U N C T I O N  D E C L A R A T I O N S 					//
				//==========================================================================//


				
/*************************************************************************/
/* Constructor */

/**
        
    @param void
*/

CDataTypeCollection::CDataTypeCollection(void)
{
	m_DataTypeCount = collectionObj.Count();
}


/*************************************************************************/
/* Destructor */

/**
        
    @param void
*/

CDataTypeCollection::~CDataTypeCollection(void)
{
	//Add destructor code here
}


 
/*****************************************************************************/
/**
\brief			addDataType
 
				This Function counts the data type
				
\param			objDataType			
\retval			void
*/
/*****************************************************************************/


void CDataTypeCollection::addDataType(DataType objDataType)
{
	INT32 iItemPosition = collectionObj.Add();
	collectionObj[iItemPosition] = objDataType;
	m_DataTypeCount = collectionObj.Count();
}


 
/*****************************************************************************/
/**
\brief			DeleteDataTypeCollection
 
				This Function clears the datatype collection count
				
\retval			void
*/
/*****************************************************************************/

void CDataTypeCollection::DeleteDataTypeCollection()
{
	collectionObj.Clear();
	m_DataTypeCount = collectionObj.Count();
}


/*****************************************************************************/
/**
\brief			getNumberOfDataTypes
 
				This function returns the total number object count
				
\retval			INT32
*/
/*****************************************************************************/

INT32 CDataTypeCollection::getNumberOfDataTypes()
{
	return collectionObj.Count();
}


/*****************************************************************************/
/**
\brief			getDataTypeElement
 
				This function return the data type ID
				
\param			iDataTypeId				
\retval			DataType
*/
/*****************************************************************************/

DataType* CDataTypeCollection::getDataTypeElement(INT32 iDataTypeId)
{
	return &collectionObj[iDataTypeId];
}


/*****************************************************************************/
/**
\brief			getDataType
 
				This function return the data type value
				
\param			pbDataTypeValue				
\retval			DataType*
*/
/*****************************************************************************/

DataType* CDataTypeCollection::getDataType(char* pbDataTypeValue)
{
	DataType* pobjDataType = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < this->getNumberOfDataTypes();
			iLoopCount++)
	{
		pobjDataType = this->getDataTypeElement(iLoopCount);

		if (0 == strcmp(pobjDataType->DataTypeValue, pbDataTypeValue))
		{
			return pobjDataType;
		}
	}
	pobjDataType = NULL;
	return pobjDataType;
}


/*****************************************************************************/
/**
\brief			getDataTypeByName
 
				This function return the data type name
				
\param			pbDataTypeValue				
\retval			DataType*
*/
/*****************************************************************************/

DataType* CDataTypeCollection::getDataTypeByName(char* pbDataTypeValue)
{
	DataType* pobjDataType = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < this->getNumberOfDataTypes();
			iLoopCount++)
	{
		pobjDataType = this->getDataTypeElement(iLoopCount);

		if (0
				== strcmp(StringToUpper(pobjDataType->getName()),
						StringToUpper(pbDataTypeValue)))
		{
			return pobjDataType;
		}
	}
	pobjDataType = NULL;
	return pobjDataType;
}
