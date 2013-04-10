/**
 ************************************************************************************************
 \file			Validation.cpp

 \brief			Validates Index,Subindex,node id,datatype and Projectversion
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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/Validation.h"
#include "../Include/Exception.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief		IfNodeExists
 
 This API shall be used to check the presence of a Node in a project. This API returns 'ocfmRetCode'. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS',  the 'ExistfFlag' will contain the information about the presence of the node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		iNodeType		Enum to hold the Node type of the node
 \param		iNodePos		Pointer to the Node position
 \param		ExistfFlag		Boolean

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode IfNodeExists(INT32 iNodeID, NodeType iNodeType, INT32 *iNodePos,
		bool& ExistfFlag)
{
	Node objNode;
	NodeCollection *objNodeCollection = NULL;
	ocfmRetCode stErrStruct;

	try
	{

		if (NULL == iNodePos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw &objException;
		}
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == objNodeCollection)
		{
			cout << "IfNodeExists: pobjNodeCollection is NULL!" << endl;
			stErrStruct.code = OCFM_ERR_UNKNOWN;
			return stErrStruct;
		}

		if (objNodeCollection->GetNumberOfNodes() > 0)
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < objNodeCollection->GetNumberOfNodes();
					iLoopCount++)
			{
				objNode = objNodeCollection->GetNodebyCollectionIndex(
						iLoopCount);

				if (objNode.GetNodeType() == iNodeType)
				{
					if (objNode.GetNodeId() == iNodeID)
					{
						*iNodePos = iLoopCount;
						stErrStruct.code = OCFM_ERR_SUCCESS;
						ExistfFlag = true;

						return stErrStruct;
					}
				}
			}
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
			throw &objException;
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	//return stErrStruct;
}

/*****************************************************************************/
/**
 \brief		IfIndexExists
 
 This API shall be used to check the presence of an Index in a node. This API returns 'ocfmRetCode'. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS',  the 'IndexPos' will contain the position of the Index under the node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType	Enum to hold the Node type of the node
 \param		indexID		Character pointer to hold the IndexID
 \param		iIndexPos		Integer Pointer to the  IndexPos

 \return	ocfmRetCode		-2 if the Index doesnot exist. Returns Zero and fills the error string on Index existance or 
 if Node doesn't exist or if NodeType is invalid
 */
/*****************************************************************************/

ocfmRetCode IfIndexExists(INT32 iNodeID, NodeType varNodeType,
		char* indexID, INT32 *iIndexPos)
{
	//CIndex objIndex;
	ocfmRetCode stErrStruct;
	INT32 iNodePos;
	bool bFlag = false;

	try
	{
		if ((NULL == indexID) || (NULL == iIndexPos))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		stErrStruct = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);

		if ((true == bFlag) && (OCFM_ERR_SUCCESS == stErrStruct.code))
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		Node objNode;
		NodeCollection *pobjNodeCollection = NULL;
		IndexCollection *objIndexCollection = NULL;
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);
		objIndexCollection = objNode.GetIndexCollection();

		if (0 == objIndexCollection->GetNumberofIndexes())
		{
			*iIndexPos = 0;
			stErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;
		}
		else if (objIndexCollection->GetNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for (INT32 iIndexCount = 0;
					iIndexCount < objIndexCollection->GetNumberofIndexes();
					iIndexCount++)
			{
				Index *objIndexPtr = NULL;
				char *indexValue = NULL;

				objIndexPtr = objIndexCollection->GetIndex(iIndexCount);
				indexValue =
						new char[strlen((char*) objIndexPtr->GetIndexValue())
								+ STR_ALLOC_BUFFER];
				strcpy(indexValue, (char*) objIndexPtr->GetIndexValue());

				if (0
						== strcmp(StringToUpper(indexValue),
								StringToUpper(indexID)))
				{
					Index *objIndexPtr = NULL;

					objIndexPtr = objIndexCollection->GetIndex(iIndexCount);
					*iIndexPos = iIndexCount;
					stErrStruct.code = OCFM_ERR_SUCCESS;
					return stErrStruct;
				}
				else if (iIndexCount
						== (objIndexCollection->GetNumberofIndexes() - 1))
				{
					// Index Doesn't Exist
					stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
					return stErrStruct;
				}
				else
				{
				}
				delete[] indexValue;
			}
		}
		else
		{
			// Indexes Doesn't Exist
			stErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrStruct;
}

/*****************************************************************************/
/**
 \brief		IfSubIndexExists
 
 This API shall be used to check the presence of a SubIndex in an Index. This API returns 'ocfmRetCode'. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS',  the 'SubIndexPos' will contain the position of the SubIndex under the Index.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		iIndexID			Character pointer to hold the IndexID
 \param		subIndexID		Character pointer to hold the SubIndexID
 \param		iSubIndexPos		Integer Pointer to hold  the  SubIndexPos
 \param		iIndexPos			Integer Pointer to hold the  IndexPos

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode IfSubIndexExists(INT32 iNodeID, NodeType varNodeType,
		char* iIndexID, char* subIndexID, INT32* iSubIndexPos,
		INT32* iIndexPos)
{
	ocfmRetCode stErrStruct;
	try
	{
		if ((NULL == iIndexID) || (NULL == subIndexID)
				|| (NULL == iIndexPos) || (NULL == iSubIndexPos))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}
		Node objNode;
		Index objIndex;
		NodeCollection *objNodeCollection = NULL;
		IndexCollection *objIndexCollection = NULL;
		Index *objSubIndex = NULL;

		stErrStruct = IfIndexExists(iNodeID, varNodeType, iIndexID,
				iIndexPos);

		if (OCFM_ERR_SUCCESS != stErrStruct.code)
		{
			// Node Doesn't Exist
			stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
			return stErrStruct;
		}

		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = objNodeCollection->GetNode(varNodeType, iNodeID);
		objIndex.SetNodeID(objNode.GetNodeId()); //TODO: Tobe removed
		objIndexCollection = objNode.GetIndexCollection();
		objSubIndex = objIndexCollection->GetIndex(*iIndexPos);
		if (objSubIndex->GetNumberofSubIndexes() == 0)
		{
			stErrStruct.code = OCFM_ERR_NO_SUBINDEXS_FOUND;
		}
		else if (objSubIndex->GetNumberofSubIndexes() > 0)
		{
			//Check for existance of the SubIndex
			for (INT32 iSubIndexcount = 0;
					iSubIndexcount < objSubIndex->GetNumberofSubIndexes();
					iSubIndexcount++)
			{
				SubIndex* objSubIndexPtr = NULL;

				objSubIndexPtr = objSubIndex->GetSubIndex(iSubIndexcount);
				if ((strcmp(
						StringToUpper((char*) objSubIndexPtr->GetIndexValue()),
						StringToUpper(subIndexID)) == 0))
				{
					stErrStruct.code = OCFM_ERR_SUCCESS;
					*iSubIndexPos = iSubIndexcount;
					return stErrStruct;
				}
				else if (iSubIndexcount
						== (objSubIndex->GetNumberofSubIndexes() - 1))
				{
					// SubIndex Doesn't Exist
					stErrStruct.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
					return stErrStruct;
				}
				else
				{
					//TODO: operation to be added
				}
			}
		}
		else
		{
			stErrStruct.code = OCFM_ERR_UNKNOWN;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrStruct;
}

/*****************************************************************************/
/**
 \brief		CheckIfDataTypeExists
 
  This API shall be used to check datatype of objects
 
 \param		dataValue	Character pointer to hold the datavalue
 \param		iNodeID		Integer Pointer to hold the value of node id 
 
 \return	BOOL
 \retval		TRUE			if successful
 \retval		FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfDataTypeExists(char* dataValue, INT32 iNodeID)
{
	NodeCollection* objNodeCol = NULL;
	DataTypeCollection* objDataCol = NULL;
	DataType* objDataType = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode = objNodeCol->GetNode(iNodeID);
	objDataCol = objNode.GetDataTypeCollection();

	objDataType = objDataCol->GetDataType(dataValue);

	if (NULL == objDataType)
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
 \brief			CheckIfDataTypeByNameExists
 
 This API shall be used to check datatype of objects with reference to name
 
 \param			dtName        Character Pointer Variable to hold the datatype name
 \param			iNodeID       Integer Pointer to hold the value of node id  
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfDataTypeByNameExists(char* dtName, INT32 iNodeID)
{
	NodeCollection *objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode = objNodeCol->GetNode(iNodeID);
	DataTypeCollection *dtcol = NULL;
	dtcol = objNode.GetDataTypeCollection();
	DataType *dt = NULL;
	dt = dtcol->GetDataTypeByName(dtName);
	if (NULL == dt)
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
 \brief			CheckIfSubIndexExists
 
 This API shall be used to check whether subindex is present with reference to node id and index ids	
 
 \param			iNodeID           Integer Pointer to hold the value of node id
 \param			varNodeType      Enum Variable of  ENodeType to hold the value of Node type    
 \param			indexID         Character Pointer Variable to hold the value of Index Id
 \param			subIndexID      Character Pointer Variable to hold the value of sub Index Id 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfSubIndexExists(INT32 iNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID)
{
	Node objNode;
	//CIndex objIndex;
	NodeCollection *objNodeCollection = NULL;
	IndexCollection *objIndexCollection = NULL;
	Index *objIdx = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCollection->GetNode(varNodeType, iNodeID);
	objIndexCollection = objNode.GetIndexCollection();
	objIdx = objIndexCollection->GetIndexbyIndexValue(indexID);

	if ((0 == objIdx->GetNumberofSubIndexes()))
	{
		return false;
	}
	else if (objIdx->GetNumberofSubIndexes() > 0)
	{
		//Check for existance of the SubIndex
		for (INT32 iSubIndexCount = 0;
				iSubIndexCount < objIdx->GetNumberofSubIndexes();
				iSubIndexCount++)
		{
			SubIndex* objSubIndexPtr;
			objSubIndexPtr = objIdx->GetSubIndex(iSubIndexCount);

			if ((0
					== strcmp(
							StringToUpper(
									(char*) objSubIndexPtr->GetIndexValue()),
							StringToUpper(subIndexID))))
			{
				return true;
			}
			else if (iSubIndexCount == (objIdx->GetNumberofSubIndexes() - 1))
			{
				return false;
			}
			else
			{
				//TODO: Nothing to be added. Continue to run the loop
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			IfVersionNumberMatches
 
 This API shall be used to check whether project version number matches with existing	
 
 \param			pxReader	xml Variable of xmlTextReaderPtr

 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool IfVersionNumberMatches(xmlTextReaderPtr pxReader)
{
	const xmlChar* xcName = NULL;
	const xmlChar* xcValue = NULL;
	//Retrieve the name and Value of an attribute
	xcValue = xmlTextReaderConstValue(pxReader);
	xcName = xmlTextReaderConstName(pxReader);
	bool bReturnValue;
	if (NULL == xcValue || NULL == xcName)
	{
		return false;
	}
#if defined DEBUG
	cout << "\nName:" << xcName << endl;
	cout << "Value:" << xcValue << endl;
#endif
	// Check for Version Tool-Project Version
	if (FALSE == strcmp(ConvertToUpper((char*) xcName), "VERSION"))
	{
		if (true == CheckToolVersion((char*) xcValue))
		{
#if defined DEBUG
			cout << "Version number matched" << endl;
#endif
			bReturnValue = true;
		}
		else
		{
#if defined DEBUG
			cout << "Version number MisMatch" << endl;
#endif
			bReturnValue = false;

		}
	}
	else
	{
		cout << "Error! IfVersionNumberMatches function can't find VERSION"
				<< endl;
		bReturnValue = false;
	}
	return bReturnValue;
}
