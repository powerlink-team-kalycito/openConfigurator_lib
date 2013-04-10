/**
 ************************************************************************************************
 \file			ObjectDictionary.cpp

 \brief			Creats object dictionary for each node and Process the Node value,Name and its attributes
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
#include "../Include/ObjectDictionary.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

using namespace std;

/****************************************************************************************************/
/* Global Variables */

INT32 iLastObjDictIndexParsed = 0;
bool ObjectDictionary::instanceFlag = false;
ObjectDictionary* ObjectDictionary::objectDictionary = NULL;
Node* ObjectDictionary::objDictNode = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

ObjectDictionary::ObjectDictionary(void)
{
	objDictNode = new Node();
	objDictNode->SetNodeId(-100);
	objDictNode->CreateIndexCollection();
	objDictNode->CreateDataTypeCollection();
	objDictNode->CreateApplicationProcess();
	objDictNode->CreateNetworkManagament();
	varsattrIdxSIdx = collectionObj.Count();
}

/*************************************************************************/
/* Destructor */

/**
 

 */

ObjectDictionary::~ObjectDictionary(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			GetObjDictPtr
 
 This is a member function of CObjectDictionary returns object dictionary based on the value of instance flag 

 \return		ObjectDictionary*
 */
/*****************************************************************************/

ObjectDictionary* ObjectDictionary::GetObjDictPtr()
{
	if (!instanceFlag)
	{
		objectDictionary = new ObjectDictionary();
		instanceFlag = true;
	}
	return objectDictionary;
}

/*****************************************************************************/
/**
 \brief			ProcessObjectDictionary
 
 This is a member function of CObjectDictionary Process the Node value,Name and its attributes
 
 \param			reader	xml Variable of type xmlTextReaderPtr 
 \return		void
 */
/*****************************************************************************/

void ObjectDictionary::ProcessObjectDictionary(xmlTextReaderPtr reader)
{
	const xmlChar *name;
	const xmlChar *value;
	name = xmlTextReaderConstName(reader);

	value = xmlTextReaderConstValue(reader);
	try
	{
		if ((NULL == name) || (NULL == value))
		{
			//cout << "Got NULL for name or value\n" << endl;
		}

		//If the NodeTYPE is ELEMENT
		if (1 == xmlTextReaderNodeType(reader))
		{
			if (0 == strcmp(((char*) name), "defType"))
			{
				DataTypeCollection* objDataTypeCollection = NULL;
				DataType objDataType;
				objDataType.dataTypeValue = NULL;
				objDataType.Name = NULL;

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetDataTypeAttributes(reader, &objDataType);
					}
				}
				objDataTypeCollection = objDictNode->GetDataTypeCollection();
				objDataTypeCollection->AddDataType(objDataType);
			}

			else if (0 == strcmp(((char*) name), "Object"))
			{
				IndexCollection* objIndexCollection = NULL;
				Index objIndex;

				//Set the NodeID
				objIndex.SetNodeID(-100);

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						value = xmlTextReaderConstValue(reader);
						name = xmlTextReaderConstName(reader);

						if (0
								== strcmp(ConvertToUpper((char*) name),
										"DATATYPE"))
						{
							DataType *dt = NULL;
							dt =
									objDictNode->GetDataTypeCollection()->GetDataType(
											(char*) value);
							objIndex.SetDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							CreateSameattrObject((char*) value, INDEX,
									(char*) objIndex.GetIndexValue());
						}
						else
						{
							bool hasPDO;
							SetIndexAttributes(reader, &objIndex, hasPDO);
						}
					}
				}
				objIndexCollection = objDictNode->GetIndexCollection();

				//Add Index object to the IndexCollection
				objIndexCollection->AddIndex(objIndex);
				iLastObjDictIndexParsed =
						objIndexCollection->GetNumberofIndexes() - 1;
			}
			else if (0 == strcmp(((char*) name), "SubObject"))
			{
				IndexCollection* objIndexCollection = NULL;
				SubIndex objSubIndex;
				//bool same = false;
				Index* objIndexPtr = NULL;
				objIndexCollection = objDictNode->GetIndexCollection();
				objIndexPtr = objIndexCollection->GetIndex(
						iLastObjDictIndexParsed);

				//Set the NodeID
				objSubIndex.SetNodeID(-100);
				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						value = xmlTextReaderConstValue(reader);
						name = xmlTextReaderConstName(reader);
						if (0
								== strcmp(ConvertToUpper((char*) name),
										"DATATYPE"))
						{
							DataType *dt = NULL;
							dt =
									objDictNode->GetDataTypeCollection()->GetDataType(
											(char*) value);
							objSubIndex.SetDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							//same =true;
							CreateSameattrObject((char*) value, SUBINDEX,
									(char*) objIndexPtr->GetIndexValue());
						}
						else
						{
							SetSubIndexAttributes(reader, &objSubIndex);
						}
					}
				}
				objIndexPtr->AddSubIndex(objSubIndex);
			}
			else
			{
				//TODO: Else added. Operation to be specified
			}
		}
	} catch (ocfmException& ex)
	{
		throw ex;
	}
}

/*****************************************************************************/
/**
 \brief			AddSameAttributesObjects
 
 This is a member function of CObjectDictionary add the SubIndex in the Index Object
 
 \param			stAttrIdx Structure Variable of s_attrIdx_SIdx to hold the attribute index
 \return		void
 */
/*****************************************************************************/

void ObjectDictionary::AddSameAttributesObjects(sattrIdxSIdx stAttrIdx)
{
	INT32 iItemPosition = collectionObj.Add();

	collectionObj[iItemPosition] = stAttrIdx;
	varsattrIdxSIdx = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			CreateSameattrObject
 
 This is a member function of CObjectDictionary creats object dictionary
 
 \param			varValue          Character Pointer Variable to hold the value of the object
 \param			varObjType      Enum Variable of  ObjectType to hold the value of Object type
 \param			varIdx            Character Pointer Variable to hold the value of Index
 
 \return		void
 */
/*****************************************************************************/

void ObjectDictionary::CreateSameattrObject(char* varValue,
		ObjectType varObjType, char* varIdx)
{
	sattrIdxSIdx stAttrIdx;
	char* subIdx = new char[RANGE_INDEX];
	char* idx = NULL;

	idx = strchr(varValue, '-');
	if (NULL != idx)
	{
		subIdx = SubString(varValue, 0, strlen(idx) - 1);
		idx = SubString(varValue, strlen(subIdx) + 1, strlen(varValue));

		stAttrIdx.objectType = varObjType;
		stAttrIdx.Idx = new char[INDEX_LEN];

		if (varObjType == INDEX)
		{
			stAttrIdx.startIndex = new char[INDEX_LEN];
			stAttrIdx.endIndex = new char[INDEX_LEN];
			strcpy(stAttrIdx.endIndex,
					SubString(varIdx, 0, 4 - strlen(subIdx)));
			strcat(stAttrIdx.endIndex, idx);
			strcpy(stAttrIdx.startIndex, varIdx);
		}
		else
		{
			stAttrIdx.startIndex = new char[SUBINDEX_LEN];
			stAttrIdx.endIndex = new char[SUBINDEX_LEN];
			strcpy(stAttrIdx.startIndex, subIdx);
			strcpy(stAttrIdx.endIndex, idx);
		}

		strcpy(stAttrIdx.Idx, varIdx);
		AddSameAttributesObjects(stAttrIdx);
	}
	delete[] subIdx;
}

/*****************************************************************************/
/**
 \brief			GetObjectDictIndex
 
 This is a member function of CObjectDictionary returns index value of the object dictionary
 
 \param			idx            Character Pointer Variable to hold the value of Index
 
 \return		Index*
 */
/*****************************************************************************/

Index* ObjectDictionary::GetObjectDictIndex(char* idx)
{
	Index* objIndex = NULL;
	IndexCollection* objIndexCol = NULL;
	//CIndex objIndex;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(idx);

	if (NULL != objIndex)
	{
		return objIndex;
	}
	else
	{
		for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count();
				iLoopCount++)
		{
			sattrIdxSIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];

			if (stAttrIdx.objectType == INDEX)
			{
				if (CheckInTheRange(idx, stAttrIdx.startIndex,
						stAttrIdx.endIndex))
				{
					objIndex = objIndexCol->GetIndexbyIndexValue(
							stAttrIdx.startIndex);
					return objIndex;
				}
			}
		}
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			GetObjectDictSubIndex
 
 This is a member function of CObjectDictionary returns the object dictonary for sub index
 
 \param			idx       Character pointer variable to hold the value of Index
 \param			sIdx      Character pointer variable to hold the value of Sub-Index
 
 \return		SubIndex*
 */
/*****************************************************************************/

SubIndex* ObjectDictionary::GetObjectDictSubIndex(char* idx, char* sIdx)
{
	SubIndex* objSIdx = NULL;
	Index* objIndex = NULL;
	IndexCollection* objIndexCol = NULL;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(idx);

	if (NULL == objIndex)
	{
		objIndex = GetObjectDictIndex(idx);

		if (NULL != objIndex)
		{
			idx = (char*) objIndex->GetIndexValue();
		}
		else
		{
			return objSIdx;
		}
	}

	objSIdx = objIndex->GetSubIndexbyIndexValue(sIdx);

	if (NULL != objSIdx)
	{
		return objSIdx;
	}
	else
	{
		for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count();
				iLoopCount++)
		{
			sattrIdxSIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];

			if ((stAttrIdx.objectType == SUBINDEX)
					&& (strcmp(stAttrIdx.Idx, idx) == 0))
			{
				if (CheckInTheRange(sIdx, stAttrIdx.startIndex,
						stAttrIdx.endIndex))
				{
					objSIdx = objIndex->GetSubIndexbyIndexValue(
							stAttrIdx.startIndex);
					if (NULL != objSIdx)
					{
						return objSIdx;
					}
				}
			}
		}
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			CheckInTheRange
 
 This is a member function of CObjectDictionary returns the range of the object dictionary ids
 
 \param			iIdx              Character Pointer Variable to hold the value of Index
 \param			startIdx         Character Pointer Variable to hold the value of Start Index 
 \param			endIdx           Character Pointer Variable to hold the value of End Index
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool ObjectDictionary::CheckInTheRange(char* iIdx, char* startIdx,
		char* endIdx)
{
	if (HexToInt(iIdx) >= HexToInt(startIdx)
			&& (HexToInt(iIdx) <= HexToInt(endIdx)))
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
 \brief			Printall
 
 This is a member function of CObjectDictionary collects the object ids,start index and end index
 
 \return		void
 */
/*****************************************************************************/
//TODO: Unused function
void ObjectDictionary::Printall()
{
	for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		sattrIdxSIdx stAttrIdx;
		stAttrIdx = collectionObj[iLoopCount];
	}
}

/*****************************************************************************/
/**
 \brief			IfObjectDictIndexExists
 
 This is a member function of CObjectDictionaryreturns the obj dictionary value from the collection list based on the index value
 
 \param			iIdx		Character Pointer Variable to hold the value of index		
 \return		INT32
 */
/*****************************************************************************/
//TODO: Unused function
INT32 ObjectDictionary::IfObjectDictIndexExists(char* iIdx)
{
	Index* objIndex = NULL;
	IndexCollection* objIndexCol;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(iIdx);

	if (NULL == objIndex)
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
 \brief			IfObjectDictSubIndexExists
 
 This is a member function of CObjectDictionary returns the obj dictionary value from the collection list based on the subindex value
 
 \param			iIdx      Character Pointer Variable to hold the value of index		
 \param			sIdx	   Character Pointer Variable to hold the value of Subindex		
 
 \return		INT32
 */
/*****************************************************************************/
//TODO: Unused function
INT32 ObjectDictionary::IfObjectDictSubIndexExists(char* iIdx, char* sIdx)
{
	SubIndex* objSIdx = NULL;
	Index* objIndex = NULL;
	IndexCollection* objIndexCol;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(iIdx);

	if (NULL == objIndex)
	{
		return TRUE;
	}

	objSIdx = objIndex->GetSubIndexbyIndexValue(sIdx);

	if (NULL == objSIdx)
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
 \brief			GetIndexName
 
 This is a member function of CObjectDictionary  returns the index name
 
 \param			objectIndex  Character Pointer Variable to hold the value of Object index
 \param			objectName   Character Pointer Variable to hold the value of Object name				
 
 \return		char*
 */
/*****************************************************************************/

char* ObjectDictionary::GetIndexName(char* objectIndex, char* objectName)
{
	char* varName = NULL;
	char* modifiedName = new char[strlen(objectName) + STR_ALLOC_BUFFER];

	INT32 iLen = 0;
	if (NULL == objectName)
	{
		return NULL;
	}

	varName = strchr(objectName, 'X');
	if (NULL != varName)
	{
		iLen = 1;
		if (0 == strcmp(SubString(varName, 1, 1), "X"))
		{
			iLen++;
		}

		INT32 pos = strlen(varName);
		INT32 iCount = strlen(objectIndex) - iLen;
		strcpy(modifiedName,
				SubString(objectName, 0, strlen(objectName) - pos));
		strcat(modifiedName, SubString(objectIndex, iCount, iLen));
		strcat(modifiedName, SubString(varName, iLen, strlen(varName) - iLen));
		return modifiedName;
	}
	else
	{
		return objectName;
	}
}
