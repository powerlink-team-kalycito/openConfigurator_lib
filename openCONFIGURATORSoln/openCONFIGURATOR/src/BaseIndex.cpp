/**
 ************************************************************************************************
 \file			BaseIndex.cpp

 \brief			This Adds the feature details reference to the index for each datatype 
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

#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/****************************************************************************************************/
/* Constructor */

/**
 

 */

BaseIndex::BaseIndex(void)
{
	Index = NULL;
	Name = NULL;
	highLimit = NULL;
	dataType.Name = NULL;
	dataTypeValue = NULL;
	defaultValue = NULL;
	actualValue = NULL;
	accessType = NULL;
	lowLimit = NULL;
	uniqueIdRef = NULL;
	dataTypeValue = NULL;
	dataType.dataTypeValue = NULL;
	nodeId = 0;
	dataType.dataSize = 0;
	//m_objectType = 0; //TODO: Review initialisation
	//m_pdoMapping = 0;
	parameterIndex = -1;
	isIncludedInCDC = FALSE;
}

/****************************************************************************************************/

/* Destructor */

/**

 */

BaseIndex::~BaseIndex(void)
{
	//Add destructor code here
}
#ifndef __GNUC__
#pragma region Properties
#endif

/*****************************************************************************/
/**
 \brief			GetName
 
 This is a member function of CBaseIndex points the Name of the Index Object
 
 \return	const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetName()
{
	return Name;
}

/*****************************************************************************/
/**
 \brief			SetName
 
 This is a member function of CBaseIndex to set the Name of the Index Object

 \param			varName	Pointer variable to hold the name of the index object
 \return	void
 */
/*****************************************************************************/

void BaseIndex::SetName(char* varName)
{
	if(NULL != Name)
	{
		delete[] Name;
	}
	if (NULL != varName)
	{
		Name = new char[strlen(varName) + STR_ALLOC_BUFFER];
		strcpy((char*) Name, varName);
	}
	else
	{
		Name = new char[1 + STR_ALLOC_BUFFER];
		strcpy((char*) Name, "");
	}
}

/*****************************************************************************/
/**
 \brief			GetIndexValue
 
 This is a member function of CBaseIndex returns the Index value of the Object
 
 \return	const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetIndexValue()
{
	return Index;
}

/*****************************************************************************/
/**
 \brief			SetIndexValue
 
 This is a member function of CBaseIndex to set the Index of the Object
 
 \param		varIndex	 Pointer variable to hold the Index value
 
 \return	void
 */
/*****************************************************************************/

void BaseIndex::SetIndexValue(char* varIndex)
{
	if(NULL != Index)
	{
		delete[] Index;
	}
	Index = new char[strlen(varIndex) + STR_ALLOC_BUFFER];
	strcpy((char*) Index, varIndex);
}

/*****************************************************************************/
/**
 \brief			GetLowLimit
 
 This is a member function of CBaseIndex to return the LowLimit of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetLowLimit()
{
	if (NULL != lowLimit)
	{
		return lowLimit;
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			SetLowLimit
 
 This is a member function of CBaseIndex to set the LowLimit of the Index Object
 
 \param			varLowLimit		Pointer variable to hold lowlimit value
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetLowLimit(char* varLowLimit)
{
	if(NULL != lowLimit)
	{
		delete[] lowLimit;
	}
	lowLimit = new char[strlen(varLowLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) lowLimit, varLowLimit);
}

/*****************************************************************************/
/**
 \brief			GetHighLimit
 
 This is a member function of CBaseIndex to return the HighLimit of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetHighLimit()
{
	if (NULL != highLimit)
	{
		return highLimit;
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			SetHighLimit
 
 This is a member function of CBaseIndex to set the LowLimit of the Index Object
 
 \param			varHighLimit		Pointer variable to hold highlimit value	
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetHighLimit(char* varHighLimit)
{
	if(NULL != highLimit)
	{
		delete[] highLimit;
	}
	highLimit = new char[strlen(varHighLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) highLimit, varHighLimit);
}

/*****************************************************************************/
/**
 \brief			GetDefaultValue
 
 This is a member function of CBaseIndex to return the Default Value of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetDefaultValue()
{
	return defaultValue;
}

/*****************************************************************************/
/**
 \brief			SetDefaultValue
 
 This is a member function of CBaseIndex to set the Default Value of the Index Object
 
 \param			varValue		Pointer variable to hold default value	
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetDefaultValue(char* varValue)
{
	if(NULL != defaultValue)
	{
		delete[] defaultValue;
	}
	defaultValue = new char[strlen(varValue) + STR_ALLOC_BUFFER];
	strcpy((char*) defaultValue, varValue);
}

/*****************************************************************************/
/**
 \brief			GetActualValue
 
 This is a member function of CBaseIndex to set the Default Value of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetActualValue()
{
	if (NULL == actualValue)
	{
		actualValue = NULL;
	}
	return actualValue;
}

/*****************************************************************************/
/**
 \brief			SetActualValue
 
 This is a member function of CBaseIndex to set the Actual Value of the Index Object
 
 \param			varValue		Pointer variable to hold Actual value 

 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetActualValue(char* varValue)
{
	if(NULL != actualValue)
	{
		delete[] actualValue;
	}
	actualValue = new char[strlen(varValue) + STR_ALLOC_BUFFER];
	strcpy((char*) actualValue, varValue);
}

/*****************************************************************************/
/**
 \brief			GetAccessType
 
 This is a member function of CBaseIndex returns the Access Type of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetAccessType()
{
	return accessType;
}

/*****************************************************************************/
/**
 \brief			SetAccessType
 
 This is a member function of CBaseIndex sets the Access Type of the Index Object
 
 \param			varAccessType Pointer variable to hold Access type of the object
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetAccessType(char* varAccessType)
{
	if(NULL != accessType)
	{
		delete[] accessType;
	}
	accessType = new char[strlen(varAccessType) + STR_ALLOC_BUFFER];
	strcpy((char*) accessType, varAccessType);
}

/*****************************************************************************/
/**
 \brief			GetObjectType
 
 This is a member function of CBaseIndex returns the Object Type of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetObjectType()
{
	switch (objectType)
	{
	case DEFTYPE:
		return "DEFTYPE";
	case DEFSTRUCT:
		return "DEFSTRUCT";
	case VAR:
		return "VAR";
	case ARRAY:
		return "ARRAY";
	case RECORD:
		return "RECORD";
	default:
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			GetEObjectType
 
 This is a member function of CBaseIndex returns the Object Type of the Index Object as Enum
 
 \return		ObjectType
 */
/*****************************************************************************/

ObjectType BaseIndex::GetEObjectType()
{
	return objectType;
}

/******************************************************************************/
/**
 \brief			SetObjectType
 
 This is a member function of CBaseIndex sets the Object of the Index Object
 
 \param			varObjectType	Pointer variable to hold object type
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetObjectType(char* varObjectType)
{
	char* varStrBuff = new char[strlen(varObjectType) + STR_ALLOC_BUFFER];

	strcpy(varStrBuff, varObjectType);
	if ((0 == strcmp(varStrBuff, "5"))
			|| (0 == strcmp(ConvertToUpper(varStrBuff), "DEFTYPE")))
	{
		objectType = DEFTYPE;
	}
	else if ((0 == strcmp(varStrBuff, "6"))
			|| (0 == strcmp(varStrBuff, "DEFSTRUCT")))
	{
		objectType = DEFSTRUCT;
	}
	else if ((0 == strcmp(varStrBuff, "7")) || (0 == strcmp(varStrBuff, "VAR")))
	{
		objectType = VAR;
	}
	else if ((0 == strcmp(varStrBuff, "8")) || (0 == strcmp(varStrBuff, "ARRAY")))
	{
		objectType = ARRAY;
	}
	else if ((0 == strcmp(varStrBuff, "9"))
			|| (0 == strcmp(varStrBuff, "RECORD")))
	{
		objectType = RECORD;
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setObjectType failed\n" << endl;
#endif
	}
	delete[] varStrBuff;
}

/******************************************************************************/
/**
 \brief			GetPDOMapping
 
 This is a member function of CBaseIndex returns the PDOMapping of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* BaseIndex::GetPDOMapping()
{
	switch (pdoMapping)
	{
	case NO:
		return "NO";
	case DEFAULT:
		return "DEFAULT";
	case OPTIONAL:
		return "OPTIONAL";
	case RPDO:
		return "RPDO";
	case TPDO:
		return "TPDO";
	default:
		return NULL;
	}
}

/******************************************************************************/
/**
 \brief			SetPDOMapping
 
 This is a member function of CBaseIndex sets the PDOMapping of the Index Object
 
 \param			varPdoMapping  Character Pointer Variable to hold PDOMapping of the Index Object
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetPDOMapping(char* varPdoMapping)
{
	char* varStrBuff = new char[strlen(varPdoMapping) + STR_ALLOC_BUFFER];

	strcpy(varStrBuff, ConvertToUpper((char*) varPdoMapping));
	if (0 == strcmp(ConvertToUpper(varStrBuff), "DEFAULT"))
	{
		pdoMapping = DEFAULT;
	}
	else if (0 == strcmp(ConvertToUpper(varStrBuff), "NO"))
	{
		pdoMapping = NO;
	}
	else if (0 == strcmp(ConvertToUpper(varStrBuff), "OPTIONAL"))
	{
		pdoMapping = OPTIONAL;
	}
	else if (0 == strcmp(ConvertToUpper(varStrBuff), "RPDO"))
	{
		pdoMapping = RPDO;
	}
	else if (0 == strcmp(ConvertToUpper(varStrBuff), "TPDO"))
	{
		pdoMapping = TPDO;
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setPDOMapping failed: "<<varPdoMapping<<" index: "<< Index<<endl;
#endif
	}
	delete[] varStrBuff;
}

/******************************************************************************/
/**
 \brief			GetDataType
 
 This is member function of CBaseIndex returns the DataType of the Index Object
 
 \return		DataType
 */
/*****************************************************************************/

DataType BaseIndex::GetDataType()
{
	return dataType;
}

/******************************************************************************/
/**
 \brief			SetDataType
 
 This is member function of CBaseIndex sets the DataType of the Index Object
 
 \param			varDataTypeName	Character pointer variable to hold datatype name
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetDataType(char* varDataTypeName)
{
	DataType* objDataType = NULL;
	NodeCollection* objNodeCol = NULL;
	DataTypeCollection* objDataCol = NULL;
	Node objNode;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNode(nodeId);
	objDataCol = objNode.GetDataTypeCollection();
	objDataType = objDataCol->GetDataType(varDataTypeName);

	if (NULL != objDataType)
	{
		dataType = *objDataType;
	}
}

/******************************************************************************/
/**
 \brief			SetDataTypeName
 
 This is member function of CBaseIndex sets the DataType of the Index Object
 
 \param			varDataTypeName Character pointer variable to hold datatype name
 \param			iNodeID		   Integer variable to hold node id of each node 		
 \return		void
 */
/*****************************************************************************/
//TODO: unused function
void BaseIndex::SetDataTypeName(char* varDataTypeName, INT32 iNodeID)
{
	DataType* objDataType = NULL;
	NodeCollection* objNodeCol = NULL;
	DataTypeCollection* objDataCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode = objNodeCol->GetNode(iNodeID);

	objDataCol = objNode.GetDataTypeCollection();
	objDataType = objDataCol->GetDataTypeByName(varDataTypeName);
	dataType = *objDataType;
}

/******************************************************************************/
/**
 \brief			GetNodeID
 
 This is member function of CBaseIndex returns the NodeID of Index Object
 
 \return		INT32
 */
/*****************************************************************************/

INT32 BaseIndex::GetNodeID()
{
	return nodeId;
}

/******************************************************************************/
/**
 \brief			SetNodeID
 
 This is member function of CBaseIndex sets the NodeID of the Index Object
 
 \param			NodeID	Integer Variable to hold the node id of the objects			
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetNodeID(INT32 NodeID)
{
	nodeId = NodeID;
}

/******************************************************************************/
/**
 \brief			GetUniqueIDRef
 
 This is member function of CBaseIndex returns the UniqueIDRef of the Node 
 
 \return		char*
 */
/*****************************************************************************/

char* BaseIndex::GetUniqueIDRef()
{
	return uniqueIdRef;
}

/******************************************************************************/
/**
 \brief			SetUniqueIDRef
 
 This is member function of CBaseIndex sets the UniqueIDRef of the Index Object
 
 \param        uniqueID  Character pointer variable to hold unique id of the objects
 
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetUniqueIDRef(char* uniqueID)
{
	uniqueIdRef = new char[strlen(uniqueID) + STR_ALLOC_BUFFER];
	strcpy((char*) uniqueIdRef, uniqueID);
}

/******************************************************************************/
/**
 \brief			GetDataTypeValue
 
 This is member function of CBaseIndex returns datatype of Node
 
 \return		char*
 */
/*****************************************************************************/

char* BaseIndex::GetDataTypeValue()
{
	if (NULL != dataTypeValue)
	{
		return dataTypeValue;
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************/
/**
 \brief			SetDataTypeValue
 
 This is member function of CBaseIndex sets the datatype value of the Index Object
 
 \param			varValue 	
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetDataTypeValue(char* varValue)
{
	dataTypeValue = new char[strlen(varValue) + STR_ALLOC_BUFFER];
	strcpy((char*) dataTypeValue, varValue);
}

/******************************************************************************/
/**
 \brief			SetDataTypeST
 
 This is member function of CBaseIndex sets the datatype value of the Index Object
 
 \param			objDataType		Class Variable of DataType to hold value of object datatype		
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SetDataTypeST(DataType objDataType)
{
	dataType = objDataType;
}
#ifndef __GNUC__
#pragma endregion Properties
#endif

#ifndef __GNUC__
#pragma region MemberFunctions
#endif

/******************************************************************************/
/**
 \brief			SaveChanges
 
 This is member function of CBaseIndex to Save the changes of the Index properties
 
 \param			varIndex  Character pointer varibale to hold the value of index
 \param			varName	 Character pointer varibale to hold the value of name			
 \return		void
 */
/*****************************************************************************/

void BaseIndex::SaveChanges(char* varIndex, char* varName)
{
	Index = varIndex;
	Name = varName;
}

/******************************************************************************/
/**
 \brief			IsIndexVaueValid
 
 This is member function of CBaseIndex to check the valid index value
 
 \param			hexValue  Character pointer variable to hold the hex value
 
 \return		BOOL
 
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool BaseIndex::IsIndexVaueValid(char* hexValue)
{
	ULONG ulValue;
	bool bFlag = true;

	if (0 == strcmp(hexValue, ""))
	{
		return true;
	}

	if (CheckIfHex(hexValue))
	{
		ulValue = HexToInt(SubString(hexValue, 2, (strlen(hexValue) - 2)));
	}
	else
	{
		return true;
	}

	if (NULL != this->lowLimit)
	{
		if (0 != strcmp(this->lowLimit, ""))
		{
			ULONG ulLowlimit;
			if (CheckIfHex((char*) this->lowLimit))
			{
				ulLowlimit = HexToInt(
						SubString((char*) lowLimit, 2,
								strlen(lowLimit) - 2));
			}
			else
			{
				ulLowlimit = atoi(lowLimit);
			}
			if (ulValue >= ulLowlimit)
			{
				bFlag = true;
			}
			else
			{
				ocfmException objException;
				objException._ocfmRetCode.code =
						OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
				objException._ocfmRetCode.errorString = new char[150];
				objException._ocfmRetCode.errorString[0] = 0;
				sprintf(objException._ocfmRetCode.errorString,
						"The entered value(%s) is less than the lower limit(%s)",
						hexValue, this->lowLimit);
				throw objException;
				//bFlag = false;
				//return bFlag;
			}
		}
	}

	if (NULL != this->highLimit)
	{
		if (0 != strcmp(this->highLimit, ""))
		{
			ULONG ulHighLimit;
			if (CheckIfHex((char*) this->highLimit))
			{
				ulHighLimit = HexToInt(
						SubString((char*) highLimit, 2,
								(strlen(highLimit) - 2)));
			}
			else
			{
				ulHighLimit = atoi(highLimit);
			}
			if (ulValue <= ulHighLimit)
			{
				bFlag = true;
			}
			else
			{
				ocfmException objException;
				objException._ocfmRetCode.code =
						OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
				objException._ocfmRetCode.errorString = new char[150];
				objException._ocfmRetCode.errorString[0] = 0;
				sprintf(objException._ocfmRetCode.errorString,
						"The entered value(%s) is greater than the upper limit(%s)",
						hexValue, this->highLimit);
				throw objException;
				//bFlag = false;
			}
		}
	}
	return bFlag;
}

/******************************************************************************/
/**
 \brief			SetFlagIfIncludedCdc
 
 This is a member function of CBaseIndex sets the Enumflag 
 
 \param			varFlag  Enumeration Flag to flagIfInCdc  
 
 \return		void			
 */
/*****************************************************************************/

void BaseIndex::SetFlagIfIncludedCdc(Flag varFlag)
{
	isIncludedInCDC = varFlag;
}

/******************************************************************************/
/**
 \brief			GetFlagIfIncludedCdc
 
 This is a member function of CBaseIndex checks whether the Index shall be included in cdc
 
 \return		Flag			
 */
/*****************************************************************************/

Flag BaseIndex::GetFlagIfIncludedCdc()
{
	return isIncludedInCDC;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
