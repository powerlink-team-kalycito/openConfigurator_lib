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
 \param		piNodePos		Pointer to the Node position
 \param		ExistfFlag		Boolean

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode IfNodeExists(INT32 iNodeID, ENodeType iNodeType, INT32 *piNodePos,
		bool& ExistfFlag)
{
	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	ocfmRetCode stErrStruct;

	try
	{

		if (NULL == piNodePos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw &objException;
		}
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			cout << "IfNodeExists: pobjNodeCollection is NULL!" << endl;
			stErrStruct.code = OCFM_ERR_UNKNOWN;
			return stErrStruct;
		}

		if (pobjNodeCollection->getNumberOfNodes() > 0)
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < pobjNodeCollection->getNumberOfNodes();
					iLoopCount++)
			{
				objNode = pobjNodeCollection->getNodebyCollectionIndex(
						iLoopCount);

				if (objNode.getNodeType() == iNodeType)
				{
					if (objNode.getNodeId() == iNodeID)
					{
						*piNodePos = iLoopCount;
						stErrStruct.code = OCFM_ERR_SUCCESS;
						ExistfFlag = true;

						return stErrStruct;
					}
				}
			}
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
			throw &objException;
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
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
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbIndexID		Character pointer to hold the IndexID
 \param		piIndexPos		Integer Pointer to the  IndexPos

 \return	ocfmRetCode		-2 if the Index doesnot exist. Returns Zero and fills the error string on Index existance or 
 if Node doesn't exist or if NodeType is invalid
 */
/*****************************************************************************/

ocfmRetCode IfIndexExists(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, INT32 *piIndexPos)
{
	//CIndex objIndex;
	ocfmRetCode stErrStruct;
	INT32 iNodePos;
	bool bFlag = false;

	try
	{
		if ((NULL == pbIndexID) || (NULL == piIndexPos))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		stErrStruct = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);

		if ((true == bFlag) && (OCFM_ERR_SUCCESS == stErrStruct.code))
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		CNode objNode;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection *pobjIndexCollection = NULL;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();

		if (0 == pobjIndexCollection->getNumberofIndexes())
		{
			*piIndexPos = 0;
			stErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;
		}
		else if (pobjIndexCollection->getNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for (INT32 iIndexCount = 0;
					iIndexCount < pobjIndexCollection->getNumberofIndexes();
					iIndexCount++)
			{
				CIndex *objIndexPtr = NULL;
				char *pbIndexValue = NULL;

				objIndexPtr = pobjIndexCollection->getIndex(iIndexCount);
				pbIndexValue =
						new char[strlen((char*) objIndexPtr->getIndexValue())
								+ STR_ALLOC_BUFFER];
				strcpy(pbIndexValue, (char*) objIndexPtr->getIndexValue());

				if (0
						== strcmp(StringToUpper(pbIndexValue),
								StringToUpper(pbIndexID)))
				{
					CIndex *objIndexPtr = NULL;

					objIndexPtr = pobjIndexCollection->getIndex(iIndexCount);
					*piIndexPos = iIndexCount;
					stErrStruct.code = OCFM_ERR_SUCCESS;
					return stErrStruct;
				}
				else if (iIndexCount
						== (pobjIndexCollection->getNumberofIndexes() - 1))
				{
					// Index Doesn't Exist
					stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
					return stErrStruct;
				}
				else
				{
				}
				delete[] pbIndexValue;
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
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbIndexID			Character pointer to hold the IndexID
 \param		pbSubIndexID		Character pointer to hold the SubIndexID
 \param		piSubIndexPos		Integer Pointer to hold  the  SubIndexPos
 \param		piIndexPos			Integer Pointer to hold the  IndexPos

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode IfSubIndexExists(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID, INT32* piSubIndexPos,
		INT32* piIndexPos)
{
	ocfmRetCode stErrStruct;
	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID)
				|| (NULL == piIndexPos) || (NULL == piSubIndexPos))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}
		CNode objNode;
		CIndex objIndex;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection *pobjIndexCollection = NULL;
		CIndex *pobjSubIndex = NULL;

		stErrStruct = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				piIndexPos);

		if (OCFM_ERR_SUCCESS != stErrStruct.code)
		{
			// Node Doesn't Exist
			stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
			return stErrStruct;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		objIndex.setNodeID(objNode.getNodeId()); //TODO: Tobe removed
		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex = pobjIndexCollection->getIndex(*piIndexPos);
		if (pobjSubIndex->getNumberofSubIndexes() == 0)
		{
			stErrStruct.code = OCFM_ERR_NO_SUBINDEXS_FOUND;
		}
		else if (pobjSubIndex->getNumberofSubIndexes() > 0)
		{
			//Check for existance of the SubIndex
			for (INT32 iSubIndexcount = 0;
					iSubIndexcount < pobjSubIndex->getNumberofSubIndexes();
					iSubIndexcount++)
			{
				CSubIndex* objSubIndexPtr = NULL;

				objSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexcount);
				if ((strcmp(
						StringToUpper((char*) objSubIndexPtr->getIndexValue()),
						StringToUpper(pbSubIndexID)) == 0))
				{
					stErrStruct.code = OCFM_ERR_SUCCESS;
					*piSubIndexPos = iSubIndexcount;
					return stErrStruct;
				}
				else if (iSubIndexcount
						== (pobjSubIndex->getNumberofSubIndexes() - 1))
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
 \brief			CheckIfDataTypeExists
 
 This API shall be used to check datatype of objects
 
 \param			pbDataValue   Character pointer to hold the datavalue
 \param			iNodeID       Integer Pointer to hold the value of node id 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfDataTypeExists(char* pbDataValue, INT32 iNodeID)
{
	CNodeCollection* pobjNodeCol = NULL;
	CDataTypeCollection* pobjDataCol = NULL;
	DataType* pobjDataType = NULL;

	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode = pobjNodeCol->getNode(iNodeID);
	pobjDataCol = objNode.getDataTypeCollection();

	pobjDataType = pobjDataCol->getDataType(pbDataValue);

	if (NULL == pobjDataType)
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
	CNodeCollection *objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode = objNodeCol->getNode(iNodeID);
	CDataTypeCollection *dtcol = NULL;
	dtcol = objNode.getDataTypeCollection();
	DataType *dt = NULL;
	dt = dtcol->getDataTypeByName(dtName);
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
 \param			enumNodeType      Enum Variable of  ENodeType to hold the value of Node type    
 \param			pbIndexID         Character Pointer Variable to hold the value of Index Id
 \param			pbSubIndexID      Character Pointer Variable to hold the value of sub Index Id 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfSubIndexExists(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID)
{
	CNode objNode;
	//CIndex objIndex;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *pobjIndexCollection = NULL;
	CIndex *pobjIdx = NULL;

	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
	pobjIndexCollection = objNode.getIndexCollection();
	pobjIdx = pobjIndexCollection->getIndexbyIndexValue(pbIndexID);

	if ((0 == pobjIdx->getNumberofSubIndexes()))
	{
		return false;
	}
	else if (pobjIdx->getNumberofSubIndexes() > 0)
	{
		//Check for existance of the SubIndex
		for (INT32 iSubIndexCount = 0;
				iSubIndexCount < pobjIdx->getNumberofSubIndexes();
				iSubIndexCount++)
		{
			CSubIndex* objSubIndexPtr;
			objSubIndexPtr = pobjIdx->getSubIndex(iSubIndexCount);

			if ((0
					== strcmp(
							StringToUpper(
									(char*) objSubIndexPtr->getIndexValue()),
							StringToUpper(pbSubIndexID))))
			{
				return true;
			}
			else if (iSubIndexCount == (pobjIdx->getNumberofSubIndexes() - 1))
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
	const xmlChar* pxcName = NULL;
	const xmlChar* pxcValue = NULL;
	//Retrieve the name and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxReader);
	pxcName = xmlTextReaderConstName(pxReader);
	bool bReturnValue;
	if (NULL == pxcValue || NULL == pxcName)
	{
		return false;
	}
#if defined DEBUG
	cout << "\nName:" << pxcName << endl;
	cout << "Value:" << pxcValue << endl;
#endif
	// Check for Version Tool-Project Version
	if (FALSE == strcmp(ConvertToUpper((char*) pxcName), "VERSION"))
	{
		if (true == CheckToolVersion((char*) pxcValue))
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
