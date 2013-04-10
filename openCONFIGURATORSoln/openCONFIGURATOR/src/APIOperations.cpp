/**
 ************************************************************************************************
 \file		APIOperations.cpp

 \brief		This file handles all the operations regarding the API calls
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
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <sys/stat.h> 

#if defined(_WIN32) && defined(_MSC_VER)
#include <direct.h>
#include <time.h>
#else
#include <sys/stat.h>
#endif
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Exception.h"
#include "../Include/Internal.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"

/****************************************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"
#define CDC_BUFFER 5000
#define CDC_MN_BUFFER 200000
//#define PI_VAR_COUNT 4000
#define MAX_FILE_PATH_SIZE 500

/****************************************************************************************************/
/* Global Variables */

INT32 iLastVarIndex = -1;
static bool bCDTCompleted = false;
static bool ObjectDictLoaded = false;

static stOffsets stSize8INOffset;
static stOffsets stSize16INOffset;
static stOffsets stSize32INOffset;
static stOffsets stSize64INOffset;
static stOffsets stSize8OUTOffset;
static stOffsets stSize16OUTOffset;
static stOffsets stSize32OUTOffset;
static stOffsets stSize64OUTOffset;

//INT32 iNo8Offsets = 0;
//INT32 iNo16Offsets = 0;
//INT32 iNo32Offsets = 0;
//INT32 iNo64Offsets = 0;

char abC_DLL_ISOCHR_MAX_PAYL[5] = "1490";
//char abC_DLL_MIN_ASYNC_MTU[4] = "300";
INT32 iConfigDate; //global used in xdcoperations
INT32 iConfigTime; //global used in xdcoperations
UINT32 uiCycleNumber; //global used in Utility

static sBuildTime sBuildTime_g;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief		AddIndexAttributes

 Adds the default attributes to the Index, when addded. The default values are fetched from the object dictionary

 \param		pIndexID			Character pointer to hold the IndexID
 \param		pobjIndex			Class pointer of Index to add the attributes
 \param		pobjDictIndex		Class pointer of Index form object dictionary

 \return	void
 */
/*****************************************************************************/
//TODO: No API but static
static void AddIndexAttributes(char* pIndexID, Index* pobjIndex,
		Index* pobjDictIndex)
{
	if ((NULL == pobjDictIndex) || (NULL == pobjIndex))
	{
#if defined DEBUG
		cout << "AddIndexAttributes FAILED" << endl;
#endif
		return;
	}

	if (NULL == pIndexID)
	{
#if defined DEBUG
		cout << "AddIndexAttributes FAILED - pIndexID empty" << endl;
#endif
		return;
	}
	else
	{
		pobjIndex->SetIndexValue(pIndexID);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != pobjDictIndex->GetName())
	{
		pobjIndex->SetName((char*) pobjDictIndex->GetName());
	}
	else
	{
		pobjIndex->SetName((char*) "");
	}

	if (NULL != pobjDictIndex->GetObjectType())
	{
		pobjIndex->SetObjectType((char*) pobjDictIndex->GetObjectType());
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjIndex->SetObjectType((char*) "");
	}

	if (NULL != pobjDictIndex->GetLowLimit())
	{
		pobjIndex->SetLowLimit((char*) pobjDictIndex->GetLowLimit());
	}
	else
	{
		pobjIndex->SetLowLimit((char*) "");
	}

	if (NULL != pobjDictIndex->GetHighLimit())
	{
		pobjIndex->SetHighLimit((char*) pobjDictIndex->GetHighLimit());
	}
	else
	{
		pobjIndex->SetHighLimit((char*) "");
	}

	if (NULL != pobjDictIndex->GetAccessType())
	{
		pobjIndex->SetAccessType((char*) pobjDictIndex->GetAccessType());
	}
	else
	{
		pobjIndex->SetAccessType((char*) "");
	}
	char* varPdoMapStr = NULL;
	if (NULL != pobjDictIndex->GetPDOMapping())
	{
		varPdoMapStr = new char[strlen(pobjDictIndex->GetPDOMapping())
				+ STR_ALLOC_BUFFER];
		strcpy(varPdoMapStr, pobjDictIndex->GetPDOMapping());
		pobjIndex->SetPDOMapping(varPdoMapStr);
	}
	else
	{
		varPdoMapStr = new char[2 + STR_ALLOC_BUFFER];
		strcpy(varPdoMapStr, (char*) "");
		pobjIndex->SetPDOMapping(varPdoMapStr);
	}
	delete[] varPdoMapStr;

	if (NULL != pobjDictIndex->GetDefaultValue())
	{
		pobjIndex->SetDefaultValue((char*) pobjDictIndex->GetDefaultValue());
	}
	else
	{
		pobjIndex->SetDefaultValue((char*) "");
	}

	if (NULL != pobjDictIndex->GetActualValue())
	{
		pobjIndex->SetActualValue((char*) pobjDictIndex->GetActualValue());
	}
	else
	{
		pobjIndex->SetActualValue((char*) "");
	}

	if (NULL != pobjDictIndex->GetDataType().GetName())
	{
		pobjIndex->SetDataTypeST(pobjDictIndex->GetDataType());
	}
	else
	{
		pobjIndex->SetDataType((char*) "");
	}
}

/*****************************************************************************/
/**
 \brief		AddSubIndexAttributes

 Adds the default attributes to the SubIndex, when addded. The default values are fetched from the object dictionary

 \param		pSubIndexID			Character pointer to hold the IndexID
 \param		pobjSubIndex		Class pointer of SubIndex to add the attributes
 \param		pobjDictSIndex		Class pointer of SubIndex from object dictionary

 \return	void
 */
/*****************************************************************************/

//TODO: No API but static
static void AddSubIndexAttributes(char* pSubIndexID, SubIndex* pobjSubIndex,
		SubIndex* pobjDictSIndex)
{
	if ((NULL == pobjDictSIndex) || (NULL == pobjSubIndex))
	{
#if defined DEBUG
		cout << "AddSubIndexAttributes FAILED" << endl;
#endif
		return;
	}

	if (NULL == pSubIndexID)
	{
#if defined DEBUG
		cout << "AddSubIndexAttributes FAILED - pSubIndexID NULL" << endl;
#endif
		return;
	}
	else
	{
		// Setting the Index Value for the SUBindex--
		pobjSubIndex->SetIndexValue(pSubIndexID);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != pobjDictSIndex->GetName())
	{
		pobjSubIndex->SetName((char*) pobjDictSIndex->GetName());
	}
	else
	{
		pobjSubIndex->SetName((char*) "");
	}

	if (NULL != pobjDictSIndex->GetObjectType())
	{
		pobjSubIndex->SetObjectType((char*) (pobjDictSIndex->GetObjectType()));
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjSubIndex->SetObjectType((char*) "");
	}

	if (NULL != pobjDictSIndex->GetLowLimit())
	{
		pobjSubIndex->SetLowLimit((char*) pobjDictSIndex->GetLowLimit());
	}
	else
	{
		pobjSubIndex->SetLowLimit((char*) "");
	}

	if (NULL != pobjDictSIndex->GetHighLimit())
	{
		pobjSubIndex->SetHighLimit((char*) pobjDictSIndex->GetHighLimit());
	}
	else
	{
		pobjSubIndex->SetHighLimit((char*) "");
	}

	if (NULL != pobjDictSIndex->GetAccessType())
	{
		pobjSubIndex->SetAccessType((char*) pobjDictSIndex->GetAccessType());
	}
	else
	{
		pobjSubIndex->SetAccessType((char*) "");
	}

	if (NULL != pobjDictSIndex->GetDefaultValue())
	{
		pobjSubIndex->SetDefaultValue(
				(char*) pobjDictSIndex->GetDefaultValue());
	}
	else
	{
		pobjSubIndex->SetDefaultValue((char*) "");
	}

	if (NULL != pobjDictSIndex->GetActualValue())
	{
		pobjSubIndex->SetActualValue((char*) pobjDictSIndex->GetActualValue());
	}
	else
	{
		pobjSubIndex->SetActualValue((char*) "");
	}

	if (NULL != pobjDictSIndex->GetDataType().GetName())
	{
		pobjSubIndex->SetDataTypeST(pobjDictSIndex->GetDataType());
	}
	else
	{
		pobjSubIndex->SetDataType((char*) "");
	}
	return;
}

/*****************************************************************************/
/**
 \brief		CreateNode

 This API shall be used to create a node with the given node ID, node type and node name.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varNodeName		Char pointer to hold the name of the node to be created

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CreateNode(INT32 iNodeID, NodeType varNodeType, char* varNodeName)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		cout << __FUNCTION__ << ": " << varNodeType << endl;
		if (MN == varNodeType)
		{
			if (!ObjectDictLoaded)
			{
#if defined(_WIN32) && defined(_MSC_VER)
				LoadObjectDictionary((char*) "od.xml");
#else
				{
					char* tmpCmdBuffer = new char[LINUX_INSTALL_DIR_LEN];
					sprintf(tmpCmdBuffer, "%s/od.xml", LINUX_INSTALL_DIR);
					LoadObjectDictionary(tmpCmdBuffer);
					delete[] tmpCmdBuffer;
				}
#endif
				ObjectDictLoaded = true;
			}
			uiCycleNumber = 0;
		}
		else if (CN == varNodeType)
		{
			INT32 iNodePos = 0;
			bool bFlag = false;
			cout << "Entering If Node Exists" << endl;
			stErrorInfo = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);
			cout << "Completed If Node Exists" << endl;
			if ((stErrorInfo.code == OCFM_ERR_SUCCESS) && (bFlag == true))
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw objException;
			}
			else
			{
#if defined DEBUG
				cout << "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND
				<< endl;
#endif
			}
		}
		else
		{
			//Nothing other than MN & CN
		}
		cout << "Completed" << __LINE__ << endl;
		Node objNode;
		objNode.SetNodeId(iNodeID);
		objNode.SetNodeType(varNodeType);
		if (NULL == varNodeName)
		{
#if defined DEBUG
			cout << "Err_setting_node_name" << endl;
			objNode.SetNodeName((char*) "ERR_NAME");
#endif
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			return (stErrorInfo);
		}
		else
		{
			objNode.SetNodeName(varNodeName);
		}

		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();
		objNode.CreateNetworkManagament();
		cout << "Completed" << __LINE__ << endl;
		NodeCollection* pobjNodeCollection = NULL;
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		cout << "Completed" << __LINE__ << endl;
		pobjNodeCollection->AddNode(objNode);
	} catch (ocfmException* ex)
	{
		stErrorInfo = ex->_ocfmRetCode;
		return (stErrorInfo);
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	cout << "Completed" << __LINE__ << endl;
	return (stErrorInfo);
}

/*****************************************************************************/
/**
 \brief		NewProjectNode

 This API shall be used to create a new node(CN or MN)  with attributes nodeid, nodetype, nodename, nodeConfigurationfile(xdd/xdc path)

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType			Enum to hold the Node type of the node
 \param		varNodeName			Character pointer to hold the name of the node to be created
 \param		varImportXmlFile	Character pointer to hold the full path of the xml(xdd/xdc) file to be imported for the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode NewProjectNode(INT32 iNodeID, NodeType varNodeType,
		char* varNodeName, char* varImportXmlFile)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		//Creates the node with Nodeid & nodeName
		if ((NULL == varNodeName))
		{
#if defined DEBUG
			cout << "varNodeName Null" << endl;
#endif
			return (stErrorInfo);
		}
		stErrorInfo = CreateNode(iNodeID, varNodeType, varNodeName);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return (stErrorInfo);
		}
		//Import the xdd/xdc for the Node created
		stErrorInfo.code = OCFM_ERR_UNKNOWN;
		if (NULL == varImportXmlFile)
		{
#if defined DEBUG
			cout << "varImportXmlFile Null" << endl;
#endif
			return (stErrorInfo);
		}
		stErrorInfo = ImportXML(varImportXmlFile, iNodeID, varNodeType);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return (stErrorInfo);
		}

		stErrorInfo.code = OCFM_ERR_UNKNOWN;
		if (MN == varNodeType)
		{
			//set the loss of SoC tolerance to 50 ms
			INT32 iIndexPos = 0;
			stErrorInfo = IfIndexExists(iNodeID, varNodeType, (char *) "1C14",
					&iIndexPos);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				Index* pobjIndex = NULL;

				pobjIndex = GetMNIndexValues((char*) "1C14");
				if ((NULL != pobjIndex))
				{
					if (NULL != pobjIndex->GetActualValue())
					{
						pobjIndex->SetActualValue((char*) "50000000");
					}
				}
			}
			else
			{
				//TODO: SOC index 1c14 is mandatory and if NotExists Throw ERR 1c14 not found.
				//Set SOC tolerance Fails
			}
		}

	} catch (ocfmException* ex)
	{
		stErrorInfo = ex->_ocfmRetCode;
		return (stErrorInfo);
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return (stErrorInfo);
}

/*****************************************************************************/
/**
 \brief		DeleteNode

 This API shall be used to delete a node with the given respective node ID and node type. Deleting a node will also delete the object dictionary for the corresponding node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteNode(INT32 iNodeID, NodeType varNodeType)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iNodePos = 0;
	bool bFlag = false;

	stErrorInfo = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);

	if (bFlag != true)
	{
		/* Function didnt throw any exception but Node doesnt exist */
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrorInfo.code = OCFM_ERR_NODEID_NOT_FOUND;
		}
		else
		{ /* Function threw exception*/
			// Node Doesn't Exist
		}
		return stErrorInfo;
	}

	Node objNode;
	NodeCollection* pobjNodeCollection = NULL;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	if (NULL == pobjNodeCollection)
	{
		stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
		return stErrorInfo;
	}
	objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);

	if (CN == objNode.GetNodeType())
	{
		ocfmRetCode stErrStructInfo;
		INT32 IndexPos = 0;
		INT32 subIndexPos = 0;
		stErrStructInfo.code = OCFM_ERR_UNKNOWN;
		stErrorInfo.errorString = NULL;

		if (MULTIPLEXED == objNode.GetStationType())
		{
			//Deleted MN's 1F9B and Subindex = old node id
			if (NULL != objNode.GetForcedCycle())
			{
				CheckAndReAssignMultiplex(objNode.GetNodeId(),
						objNode.GetForcedCycle());
			}
			else
			{
				//ForcedCycle NULL
			}
		}
		else
		{
			//nothing to do if not MULTIPLEXED
		}

		char* varSIdx = new char[SUBINDEX_LEN];
		varSIdx = IntToAscii(objNode.GetNodeId(), varSIdx, 16);
		varSIdx = PadLeft(varSIdx, '0', 2);

		stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
				(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, varSIdx, &subIndexPos,
				&IndexPos);
		if (stErrStructInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrStructInfo = SetSubIndexAttributesByAttribute(MN_NODEID, MN,
					(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, varSIdx, ACTUALVALUE,
					(char*) "");
			if (stErrStructInfo.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F9B SetSubIndexAttributesByAttribute Failed" << endl;
			}
		}
		else
		{
			//1F9B->subIdx(CN Nodeid) doesnot exists
		}
		stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
				(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, varSIdx, &subIndexPos,
				&IndexPos);
		if (stErrStructInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrStructInfo = SetSubIndexAttributesByAttribute(MN_NODEID, MN,
					(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, varSIdx,
					ACTUALVALUE, (char*) "");
			if (stErrStructInfo.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F92 SetSubIndexAttributesByAttribute Failed" << endl;
			}
		}
		else
		{
			//1F92->subIdx(CN Nodeid) doesnot exists
		}
		delete[] varSIdx;

		//Delete the auto generated pdo indexes also.
		Node *pobjMNnode = NULL;
		IndexCollection *pobjMnIdxCol = NULL;
		pobjMNnode = pobjNodeCollection->GetNodePtr(MN, MN_NODEID);
		pobjMnIdxCol = pobjMNnode->GetIndexCollection();
		INT32 iTotalMNidx = pobjMnIdxCol->GetNumberofIndexes();
		cout<< "iTotalMNidx" <<iTotalMNidx<<endl;
		for(INT32 iLoopCount = 0; iLoopCount < iTotalMNidx; iLoopCount++)
		{
			Index *pobjMnCommIndex = NULL;
			char* varSubstr1 = new char[SUBINDEX_LEN];
			char* varSubstr2 = new char[SUBINDEX_LEN];
			bool b14entry = false;
			bool b18entry = false;

			pobjMnCommIndex = pobjMnIdxCol->GetIndex(iLoopCount);
			varSubstr1 = SubString((char*) pobjMnCommIndex->GetIndexValue(), 0, 2);
			varSubstr2 = SubString((char*) pobjMnCommIndex->GetIndexValue(), 2, 2);
			cout<<"Entry Processing: "<<varSubstr1<<varSubstr2<<endl;
			if ((0 == strcmp(varSubstr1, "14")))
			{
				b14entry = true;
			}
			else if((0 == strcmp(varSubstr1, "18")))
			{
				b18entry = true;
			}


			if ((b14entry == true) || (b18entry == true))
			{
				SubIndex *pobjSubIndex = NULL;
				char* varActValue = new char[SUBINDEX_LEN];
				pobjSubIndex = pobjMnCommIndex->GetSubIndexbyIndexValue((char*) "01");
				if(NULL != pobjSubIndex)
				{
					INT32 iTempNodeid;
					varActValue = (char*)pobjSubIndex->GetActualValue(); //NULL
					iTempNodeid = GetDecimalValue(varActValue);

					if(iNodeID == iTempNodeid)
					{
						Index *varojMnTxIndex = NULL;
						char *varMnTxIndexId =  new char[INDEX_LEN];
						if(b14entry)
						{
							varMnTxIndexId = strcpy(varMnTxIndexId, "16");
							varMnTxIndexId = strcat(varMnTxIndexId, varSubstr2);

							varojMnTxIndex = pobjMnIdxCol->GetIndexbyIndexValue(varMnTxIndexId);
						}
						else if(b18entry)
						{
							varMnTxIndexId = strcpy(varMnTxIndexId, "1A");
							varMnTxIndexId = strcat(varMnTxIndexId, varSubstr2);

							varojMnTxIndex = pobjMnIdxCol->GetIndexbyIndexValue(varMnTxIndexId);
						}
						else
						{
							//no others will enter
						}

						if (NULL != varojMnTxIndex)
						{
							ocfmRetCode stErrorInfo;
							varojMnTxIndex->DeleteSubIndexCollection();
							stErrorInfo = AddSubIndex(MN_NODEID, MN, varMnTxIndexId, (char*) "00");
							if (stErrorInfo.code != OCFM_ERR_SUCCESS)
							{
								cout<<"00th subindex cannot be added"<<endl;
							}
							pobjSubIndex->SetActualValue((char*)"00");
						}
						cout<<"subidx Coll deleted"<<endl;
						delete[] varMnTxIndexId;
					}
				}
				else
				{
					//00th subindex in the communication index is not present
				}
				//delete[] varActValue;
			}
			delete[] varSubstr1;
			delete[] varSubstr2;
		}
	}

	pobjNodeCollection->DeleteNode(iNodePos);


#if defined DEBUG
	cout << "Deleted " << varNodeType << ". NodeID:" << iNodeID << endl;
#endif

	//stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteIndex

 This API shall be used to delete a particular index of a node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType	Enum to hold the Node type of the node
 \param		pIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteIndex(INT32 iNodeID, NodeType varNodeType, char* pIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		INT32 iIndexPos = 0;

		stErrorInfo = IfIndexExists(iNodeID, varNodeType, pIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node* pobjNode = NULL;
		IndexCollection* pobjIndexCollection = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		//                objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
		//                pobjIndexCollection = objNode.getIndexCollection();

		pobjNode = pobjNodeCollection->GetNodePtr(varNodeType, iNodeID);
		if (NULL == pobjNode)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		pobjIndexCollection = pobjNode->GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		pobjIndexCollection->DeleteIndex(iIndexPos);
#if defined DEBUG
		cout << "Deleted " << varNodeType << ". NodeID:" << iNodeID
		<< ". IndexID:" << pIndexID << endl;
#endif

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteSubIndex

 This API shall be used to delete a particular SuIndex in a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'

 \param		iNodeID			Integer variable to hold the Node Id of a node.
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varIndexID		Character pointer to hold the IndexID
 \param		varSubIndexID	Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteSubIndex(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varSubIndexID)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == varIndexID) || (NULL == varSubIndexID))
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		NodeCollection* pobjNodeCollection = NULL;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;
		SubIndex* pobjSIDx = NULL;
		Node objNode;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		//TODO: why not getNodeptr() ?

		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSIDx = pobjIndex->GetSubIndex(iSubIndexPos);
		if (NULL != pobjSIDx)
		{
			//delete the sub-index and then updated the 00th entry
			pobjIndex->DeleteSubIndex(iSubIndexPos);

			UpdateNumberOfEnteriesSIdx(pobjIndex, varNodeType);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			stErrorInfo.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}
/*****************************************************************************/
/**
 \brief		AddSubIndex

 This API shall be used to add a particular SubIndex to a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType	Enum to hold the Node type of the node
 \param		varIndexID		Character pointer to hold the IndexID
 \param		varSubIndexID	Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode AddSubIndex(INT32 iNodeID, NodeType varNodeType, char* varIndexID,
		char* varSubIndexID)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == varIndexID) || (NULL == varSubIndexID))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
			throw objException;
		}

		if ((OCFM_ERR_NO_SUBINDEXS_FOUND == stErrorInfo.code)
				|| (OCFM_ERR_SUBINDEXID_NOT_FOUND == stErrorInfo.code))
		{
			NodeCollection* pobjNodeCollection = NULL;
			//CNode objNode;
			Node *objNode = NULL;
			IndexCollection* pobjIndexCollection = NULL;
			Index* pobjIndex = NULL;

			//                        CIndex objIndex;

			pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}

			//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
			objNode = pobjNodeCollection->GetNodePtr(varNodeType, iNodeID);

			//pobjIndexCollection = objNode.getIndexCollection();
			pobjIndexCollection = objNode->GetIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
			if (NULL == pobjIndex)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}
			SubIndex* pobjSubIndex = NULL;
			SubIndex* objDictSIdx = NULL;

			/* Get the SubIndex from ObjectDictionary*/
			ObjectDictionary* pobjOBD = NULL;
			pobjOBD = ObjectDictionary::GetObjDictPtr();
			if (NULL == pobjOBD)
			{
				stErrorInfo.code = OCFM_ERR_UNKNOWN;
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_UNKNOWN);
				throw objException;
			}

			objDictSIdx = pobjOBD->GetObjectDictSubIndex(varIndexID,
					varSubIndexID);
			if (NULL != objDictSIdx)
			{
				pobjSubIndex = new SubIndex;
				pobjSubIndex->SetNodeID(iNodeID);
				AddSubIndexAttributes(varSubIndexID, pobjSubIndex, objDictSIdx);
				if (NULL != pobjIndex)
				{
					pobjIndex->AddSubIndex(*pobjSubIndex);
				}
			}
			else if ((varNodeType == MN)
					&& (true == CheckIfProcessImageIdx(varIndexID)))
			{
				pobjSubIndex = new SubIndex;
				pobjSubIndex->SetNodeID(iNodeID);
				pobjSubIndex->SetIndexValue(varSubIndexID);
				if (NULL != pobjIndex)
				{
					pobjIndex->AddSubIndex(*pobjSubIndex);
				}
			}
			else if (true == CheckIfManufactureSpecificObject(varIndexID))
			{
				pobjSubIndex = new SubIndex;
				pobjSubIndex->SetNodeID(iNodeID);
				pobjSubIndex->SetIndexValue(varSubIndexID);

				if (NULL != pobjIndex)
				{
					if ((ARRAY == pobjIndex->GetEObjectType())
							&& (0 != strcmp(varSubIndexID, (const char*) "00")))
					{
						//If objectType='ARRAY', all subobjects (except 0x00) have got the same dataType as the object
						pobjSubIndex->SetDataTypeST(pobjIndex->GetDataType());
					}
					//all the subobjects is of type VAR
					pobjSubIndex->SetObjectType((char*) "VAR");
					pobjSubIndex->SetFlagIfIncludedCdc(TRUE);
					pobjIndex->AddSubIndex(*pobjSubIndex);
				}
			}
			else
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}
			/* Update subindex "00"*/
			if ((NULL != pobjSubIndex)
					&& (NULL != pobjSubIndex->GetIndexValue()))
			{
				if (0
						!= strcmp(pobjSubIndex->GetIndexValue(),
								(const char*) "00"))
				{
					UpdateNumberOfEnteriesSIdx(pobjIndex, varNodeType);
				}
			}

			//AddSubIndexAttributes(varSubIndexID, &pobjSubIndex);

			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		AddSubobject

 This function is to add the 00'th entry for manufacture specific objects automatically while setting all index attributes

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode AddSubobject(INT32 iNodeID, NodeType varNodeType, char* varIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		if (NULL == varIndexID)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = AddSubIndex(iNodeID, varNodeType, varIndexID,
				(char*) "00");
		if ((OCFM_ERR_SUCCESS == stErrorInfo.code)
				&& (true == CheckIfManufactureSpecificObject(varIndexID)))
		{
			NodeCollection* pobjNodeCollection = NULL;
			Node objNode;
			IndexCollection* pobjIndexCollection = NULL;

			Index* pobjIndex = NULL;
			SubIndex* pobjSubIndex = NULL;

			INT32 iIndexPos = 0;
			INT32 iSubIndexPos = 0;

			stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
					(char*) "00", &iSubIndexPos, &iIndexPos);
			if (OCFM_ERR_SUCCESS != stErrorInfo.code)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}

			pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}

			objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

			pobjIndexCollection = objNode.GetIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
			if (NULL == pobjIndex)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}

			pobjSubIndex = pobjIndex->GetSubIndex(iSubIndexPos);
			if (NULL == pobjSubIndex)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
				throw objException;
			}

			pobjSubIndex->SetName((char*) "NumberOfEntries");
			pobjSubIndex->SetObjectType((char*) "VAR");
			char* varDataTypeName = new char[15];

			strcpy(varDataTypeName, (const char*) "UNSIGNED8");
			if (true == (CheckIfDataTypeByNameExists(varDataTypeName, iNodeID)))
			{
				pobjSubIndex->SetDataTypeName(varDataTypeName, iNodeID);
			}
			else
			{
				delete[] varDataTypeName;
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
				throw objException;
			}
			delete[] varDataTypeName;
		}
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		AddIndex

 This API shall be used to add a particular Index to a particular node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode AddIndex(INT32 iNodeID, NodeType varNodeType, char* varIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos = 0;
	ocfmException objException;

	try
	{
		if (NULL == varIndexID)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			objException.OCFMException(OCFM_ERR_INDEX_ALREADY_EXISTS);
			throw objException;
		}

		if ((OCFM_ERR_NO_INDEX_FOUND == stErrorInfo.code)
				|| (OCFM_ERR_INDEXID_NOT_FOUND == stErrorInfo.code))
		{
			NodeCollection* pobjNodeCollection = NULL;
			Node objNode;
			IndexCollection* pobjIndexCollection = NULL;
			ObjectDictionary* pobjOBD = NULL;
			Index* pobjDictIndex = NULL;

			pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);
			pobjIndexCollection = objNode.GetIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			/* Get the Index from ObjectDictionary*/
			pobjOBD = ObjectDictionary::GetObjDictPtr();
			if (NULL == pobjOBD)
			{
				stErrorInfo.code = OCFM_ERR_UNKNOWN;
				objException.OCFMException(OCFM_ERR_UNKNOWN);
				throw objException;
			}
			//Validate for TPDO channels for a CN
			if((CN == objNode.GetNodeType()) && ( (0 == strncmp(varIndexID, "1A", 2)) || (0 == strncmp(varIndexID, "1a", 2))))
			{
				INT32 tpdoCount = 0;
				INT32 rpdoCount = 0;
				objNode.getPDOIndexCollection(&rpdoCount, &tpdoCount);
				#if defined DEBUG
					cout<<"tpdoCount:"<<tpdoCount<<" rpdoCount:"<<rpdoCount<<endl;
				#endif
				//Allowed to add TPDO only if the node has 0 TPDO's(1Axx)
				if(tpdoCount > 0)
				{
					objException.OCFMException(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char acCustomError[200] = { 0 };
					sprintf(acCustomError, "CN cannot have more than one TPDO Channel");
					CopyCustomErrorString(&(objException._ocfmRetCode), acCustomError);
					throw objException;
				}
			}
			pobjDictIndex = pobjOBD->GetObjectDictIndex(varIndexID);
			if (NULL != pobjDictIndex)
			{
				SubIndex* objSIdx = NULL;
				Index objIndex;
				objIndex.SetNodeID(iNodeID);
				AddIndexAttributes(varIndexID, &objIndex, pobjDictIndex);
				for (INT32 iLoopCount = 0;
						iLoopCount < pobjDictIndex->GetNumberofSubIndexes();
						iLoopCount++)
				{
					objSIdx = pobjDictIndex->GetSubIndex(iLoopCount);
					if (NULL != objSIdx)
					{
						objIndex.AddSubIndex(*objSIdx);
					}
				}
				//NULL check for getName
				objIndex.SetName(
						pobjOBD->GetIndexName(SubString(varIndexID, 2, 4),
								(char*) objIndex.GetName()));
				//update the no of entries for the subindex added
				UpdateNumberOfEnteriesSIdx(&objIndex, varNodeType);
				pobjIndexCollection->AddIndex(objIndex);
			}
			else if ((MN == varNodeType)
					&& (true == CheckIfProcessImageIdx(varIndexID)))
			{
				Index objIndex;
				objIndex.SetNodeID(iNodeID);
				objIndex.SetIndexValue(varIndexID);
				pobjIndexCollection->AddIndex(objIndex);
			}
			else if (true == CheckIfManufactureSpecificObject(varIndexID))
			{
				Index objIndex;
				objIndex.SetNodeID(iNodeID);
				objIndex.SetIndexValue(varIndexID);
				objIndex.SetFlagIfIncludedCdc(TRUE);
				pobjIndexCollection->AddIndex(objIndex);
			}
			else
			{
				objException.OCFMException(OCFM_ERR_INVALID_INDEXID);
				throw objException;
			}

			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			// This Part of code is never expected to happen
			objException.OCFMException(OCFM_ERR_INVALID_INDEXID);
			throw objException;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetIndexAttributes

 This API shall be used to set the index attributes [IndexValue, IndexName & flagIfInCdc] of a particular Index of a node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		varNodeType				Enum to hold the Node type of the node
 \param		varIndexID				Character pointer to hold the IndexID
 \param		varIndexValue			Character Pointer to hold the IndexValue
 \param		varIndexName			Character Pointer to hold the IndexName
 \param		varIsIncludedInCdc		Enumeration Flag to flagIfInCdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varIndexValue, char* varIndexName,
		Flag varIsIncludedInCdc)
{
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		if ((NULL == varIndexID) || (NULL == varIndexValue)
				|| (NULL == varIndexName))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node objNode;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;
		Index objIndex;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);
		objIndex.SetNodeID(iNodeID);
		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_INDEX_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			stErrorInfo.code = OCFM_ERR_INDEXID_NOT_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjIndex->SetName(varIndexName);
		pobjIndex->SetFlagIfIncludedCdc(varIsIncludedInCdc);

		if (NULL != varIndexValue)
		{
			if (true == pobjIndex->IsIndexVaueValid(varIndexValue))
			{
				pobjIndex->SetActualValue(varIndexValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetSubIndexAttributes

 This API shall be used to set the SubIndex attributes [IndexValue, IndexName & flagIfInCdc] of a particular SuIndex in a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		varNodeType				Enum to hold the Node type of the node
 \param		varIndexID				Character pointer to hold the IndexID
 \param		varSubIndexID			Character pointer to hold the SubIndexID
 \param		varIndexValue			Character Pointer to hold the IndexValue
 \param		varIndexName			Character Pointer to hold the IndexName
 \param		varIsIncludedInCdc		Enumeration Flag to flag if included in Cdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetSubIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varSubIndexID, char* varIndexValue,
		char* varIndexName, Flag varIsIncludedInCdc)
{
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == varIndexID) || (NULL == varSubIndexID)
				|| (NULL == varIndexValue) || (NULL == varIndexName))
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		if ((MN_NODEID == iNodeID) && (MN == varNodeType)
				&& (0 == strcmp(varIndexID, "1F92"))
				&& (0 != strcmp(varSubIndexID, "00")))
		{
			if (false == ValidateCNPresTimeout(varSubIndexID, varIndexValue))
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_LOW_CNPRESTIMEOUT);
				throw objException;
			}
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node objNode;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;
		SubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex = pobjIndex->GetSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex->SetName(varIndexName);
		pobjSubIndex->SetFlagIfIncludedCdc(varIsIncludedInCdc);

		if (pobjSubIndex->IsIndexVaueValid(varIndexValue))
		{
			pobjSubIndex->SetActualValue(varIndexValue);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			throw objException;
		}

		if ((MN_NODEID == iNodeID) && (MN == varNodeType)
				&& (0 == strcmp(varIndexID, (const char*) "1F98"))
				&& (0 == strcmp(varSubIndexID, (const char*) "07")))
		{
			stErrorInfo = RecalculateMultiplex();
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetALLIndexAttributes

 This API shall be used to set all the attributes of an Index in a single shot.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType			Enum to hold the Node type of the node
 \param		varIndexID			Character pointer to hold the IndexID
 \param		varActualValue		Character pointer to hold the ActualValue
 \param		varIndexName		Character pointer to hold the IndexName
 \param		varAccess			Character pointer to hold the Access type
 \param		varDataTypeName		Character pointer to hold the Datatype
 \param		pdoMappingVal		Character pointer to hold the PDO Mapping
 \param		vardefaultValue		Character pointer to hold the Default value
 \param		varhighLimit		Character pointer to hold the Highlimit
 \param		varlowLimit			Character pointer to hold the Lowlimit
 \param		varobjType			Character pointer to hold the Object type
 \param		varIncludeInCdc		Enumeration flag for include in cdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetAllIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varActualValue, char* varIndexName, char* varAccess,
		char* varDataTypeName, char* pdoMappingVal, char* vardefaultValue,
		char* varhighLimit, char* varlowLimit, char* varobjType,
		Flag varIncludeInCdc)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	try
	{
		// || (NULL == varActualValue) || (NULL == varIndexName) || (NULL == varAccess) || (NULL == varDataTypeName) || (NULL == pdoMappingVal) || (NULL == vardefaultValue) || (NULL == varhighLimit) || (NULL == varlowLimit) || (NULL == varobjType)
		if ((NULL == varIndexID))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node objNode;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_INDEX_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			stErrorInfo.code = OCFM_ERR_INDEXID_NOT_FOUND;
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		/* Check if the value is valid*/
		if (NULL != varIndexName)
		{
			if (strcmp(varIndexName, "") != 0)
				pobjIndex->SetName(varIndexName);
		}

		if (NULL != varAccess)
		{
			if (strcmp(varAccess, "") != 0)
				pobjIndex->SetAccessType(varAccess);
		}

		if (NULL != pdoMappingVal)
		{
			pobjIndex->SetPDOMapping(pdoMappingVal);
		}
		if (NULL != vardefaultValue)
		{
			pobjIndex->SetDefaultValue(vardefaultValue);
		}

		ocfmRetCode stErrorLimitInfo;
		stErrorLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != varhighLimit) && (NULL != varlowLimit))
		{
			stErrorLimitInfo = CheckUpperAndLowerLimits(varlowLimit,
					varhighLimit);
			if (OCFM_ERR_SUCCESS == stErrorLimitInfo.code)
			{
				pobjIndex->SetHighLimit(varhighLimit);
				pobjIndex->SetLowLimit(varlowLimit);
			}
			else
			{
				return stErrorLimitInfo;
			}
		}

		if (NULL != varobjType)
		{
			pobjIndex->SetObjectType(varobjType);
			if (pobjIndex->GetEObjectType() == ARRAY)
			{
				pobjIndex->UpdateArraySubObjects();
			}
		}

		pobjIndex->SetFlagIfIncludedCdc(varIncludeInCdc);

		if ((ARRAY == pobjIndex->GetEObjectType())
				|| (RECORD == pobjIndex->GetEObjectType()))
		{
			stErrorInfo = AddSubobject(iNodeID, varNodeType, varIndexID);
		}

		if (NULL != varDataTypeName)
		{
			if (strcmp(varDataTypeName, "") != 0)
			{
				if ((CheckIfDataTypeByNameExists(varDataTypeName,
						pobjIndex->GetNodeID())) == true)
				{
					pobjIndex->SetDataTypeName(varDataTypeName, iNodeID);
					DataType objDataType;
					objDataType = pobjIndex->GetDataType();
					if (pobjIndex->GetEObjectType() == ARRAY)
					{
						pobjIndex->UpdateArraySubObjects();
					}
				}
				else
				{
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw objException;
				}
			}
		}
		if (NULL != varActualValue)
		{
			if (pobjIndex->IsIndexVaueValid(varActualValue))
			{
				pobjIndex->SetActualValue(varActualValue);

				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetAllSubIndexAttributes

 This API shall be used to set all the attributes of an SubIndex in a single shot.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType			Enum to hold the Node type of the node
 \param		varIndexID			Character pointer to hold the IndexID
 \param		varSubIndexID		Character pointer to hold the SubIndexID
 \param		varActualValue		Character pointer to hold the ActualValue
 \param		varIndexName		Character pointer to hold the IndexName
 \param		varAccess			Character pointer to hold the Access type
 \param		varDataTypeName		Character pointer to hold the Datatype
 \param		varPdoMappingVal	Character pointer to hold the PDO Mapping
 \param		varDefaultValue		Character pointer to hold the Default value
 \param		varHighLimit		Character pointer to hold the Highlimit
 \param		varLowLimit			Character pointer to hold the Lowlimit
 \param		varObjType			Character pointer to hold the Object type
 \param		varIncludeInCdc		Enumeration flag for include in cdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetAllSubIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varSubIndexID, char* varActualValue,
		char* varIndexName, char* varAccess, char* varDataTypeName,
		char* varPdoMappingVal, char* varDefaultValue, char* varHighLimit,
		char* varLowLimit, char* varObjType, Flag varIncludeInCdc)
{
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == varIndexID) || (NULL == varSubIndexID))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node objNode;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;
		SubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		pobjSubIndex = pobjIndex->GetSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		if (NULL != varIndexName)
		{
			pobjSubIndex->SetName(varIndexName);
		}

		if (NULL != varAccess)
		{
			pobjSubIndex->SetAccessType(varAccess);
		}

		if (NULL != varPdoMappingVal)
		{
			pobjSubIndex->SetPDOMapping(varPdoMappingVal);
		}

		if (NULL != varDefaultValue)
		{
			pobjSubIndex->SetDefaultValue(varDefaultValue);
		}

		ocfmRetCode stErrorLimitInfo;
		stErrorLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != varHighLimit) && (NULL != varLowLimit))
		{
			stErrorLimitInfo = CheckUpperAndLowerLimits(varLowLimit,
					varHighLimit);
			if (OCFM_ERR_SUCCESS == stErrorLimitInfo.code)
			{
				pobjSubIndex->SetHighLimit(varHighLimit);
				pobjSubIndex->SetLowLimit(varLowLimit);
			}
			else
			{
				return stErrorLimitInfo;
			}
		}

		if (NULL != varObjType)
		{
			pobjSubIndex->SetObjectType(varObjType);
		}

		pobjSubIndex->SetFlagIfIncludedCdc(varIncludeInCdc);

		if (NULL != varActualValue)
		{
			if (pobjSubIndex->IsIndexVaueValid(varActualValue))
			{
				pobjSubIndex->SetActualValue(varActualValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}
		if (NULL != varDataTypeName)
		{
			if (0 != strcmp(varDataTypeName, (const char*) ""))
			{
				if (true
						== (CheckIfDataTypeByNameExists(varDataTypeName,
								pobjSubIndex->GetNodeID())))
				{
					pobjSubIndex->SetDataTypeName(varDataTypeName, iNodeID);
				}
				else
				{
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw objException;
				}
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetSubIndexAttributesByAttribute

 This function shall be used to set any SubIndex attributes of a particular SuIndex in a particular Index of a node by selecting any of the types from the varerator EAttributeType

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		varNodeType				Enum to hold the Node type of the node
 \param		varIndexID				Character pointer to hold the IndexID
 \param		varSubIndexID			Character pointer to hold the SubIndexID
 \param		varAttributeType		Enum to hold the attribute type of the subindex
 \param		varInAttributeValue		Character pointer to hold the value to be set for the subindex attribute

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetSubIndexAttributesByAttribute(INT32 iNodeID,
		NodeType varNodeType, char* varIndexID, char* varSubIndexID,
		AttributeType varAttributeType, char* varInAttributeValue)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == varIndexID) || (NULL == varSubIndexID))
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return stErrorInfo;
		}

		NodeCollection* pobjNodeCollection = NULL;
		Node objNode;
		IndexCollection* pobjIndexCollection = NULL;
		Index* pobjIndex = NULL;
		SubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex = pobjIndex->GetSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		switch (varAttributeType)
		{
		case NAME:
			// No Operation
			break;
		case OBJECTTYPE:
			// No Operation
			break;
		case DATATYPE:
			// No Operation
			break;
		case ACCESSTYPE:
			// No Operation
			break;
		case DEFAULTVALUE:
			// No Operation
			break;
		case ACTUALVALUE:
			if (NULL != varInAttributeValue)
				pobjSubIndex->SetActualValue(varInAttributeValue);
			else
				pobjSubIndex->SetActualValue((char*) "");
			break;
		case PDOMAPPING:
			if (NULL != varInAttributeValue)
				pobjSubIndex->SetPDOMapping(varInAttributeValue);
			else
				pobjSubIndex->SetPDOMapping((char*) "");
			break;
		case LOWLIMIT:
			// No Operation
			break;
		case HIGHLIMIT:
			// No Operation
			break;
		case FLAGIFINCDC:
			// No Operation
			break;
		default:
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		CheckUpperAndLowerLimits

 This function shall be used to validate the upper & lower limit values while setting the Index/Subindex attributes limits

 \param		varLowLimit		Character pointer to hold the lowerlimit value
 \param		varHighLimit	Character pointer to hold the higherlimit value

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CheckUpperAndLowerLimits(char* varLowLimit, char* varHighLimit)
{
	ocfmRetCode stError;
	stError.code = OCFM_ERR_SUCCESS;

	if ((NULL != varLowLimit) && (NULL != varHighLimit))
	{
		if ((0 != strcmp(varLowLimit, "")) && (0 != strcmp(varHighLimit, "")))
		{
			ULONG ulLowlimit;
			ULONG ulHighLimit;
			if (true == CheckIfHex((char*) varLowLimit))
			{
				ulLowlimit = HexToInt(
						SubString((char*) varLowLimit, 2,
								strlen(varLowLimit) - 2));
			}
			else
			{
				ulLowlimit = atoi(varLowLimit);
			}

			if (true == CheckIfHex((char*) varHighLimit))
			{
				ulHighLimit = HexToInt(
						SubString((char*) varHighLimit, 2,
								strlen(varHighLimit) - 2));
			}
			else
			{
				ulHighLimit = atoi(varHighLimit);
			}

			if (ulHighLimit >= ulLowlimit)
			{
				return stError;
			}
			else
			{
				stError.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
				stError.errorString = new char[150];
				INT32 iErrSprintf = 0;
				stError.errorString[0] = 0;
				iErrSprintf = sprintf(stError.errorString,
						"The lower limit(%s) is greater than upperlimit(%s)",
						varLowLimit, varHighLimit);
				if (iErrSprintf < 0)
				{
					cout << "Sprintf Error:" << __FUNCTION__ << endl;
				}
				else
				{
					//sprintf Success
				}
				return stError;
			}
		}
	}
	return stError;
}

/*****************************************************************************/
/**
 \brief		EnableDisableMappingPDO

 This function Enables or Disables the Mapping pdo's before the pdo contents are written to the cdc by setting the EnableFlag to true or false respectiveley

 \param		pobjIdxCol		Class pointer of IndexCollection for a single node
 \param		pobjIndex		Class pointer of Index for the PDO index
 \param		Buffer			Character pointer to hold the values to be written to the cdc
 \param		EnableFlag		Boolean value to enable or disable the pdo

 \return	void
 */
/*****************************************************************************/

void EnableDisableMappingPDO(IndexCollection* pobjIdxCol, Index* pobjIndex,
		char* Buffer, bool EnableFlag)
{
//TODO: objException thrown is Not catched. Try Catch block needed
	// INT32 len;

	//Get the Index Value
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
		throw objException;
	}

	if (NULL == Buffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	strcpy(Buffer, "");
	if (0 != pobjIndex->GetNumberofSubIndexes())
	{

		//bool resetValueAdded = false;
		SubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "00");
		if (NULL != pobjSubIndex)
		{
			if (NULL != pobjSubIndex->GetActualValue())
			{
				INT32 noOfSubIndexes = 0; //= pobjIndex->getNumberofSubIndexes();
				if (true == CheckIfHex((char*) pobjSubIndex->GetActualValue()))
				{
					noOfSubIndexes = HexToInt(
							SubString((char*) pobjSubIndex->GetActualValue(), 2,
									strlen(pobjSubIndex->GetActualValue())
											- 2));
				}
				else
				{
					noOfSubIndexes = atoi(pobjSubIndex->GetActualValue());
				}

				/* No need to reinitailize mapping pdo to zero again */
				if ((0 == noOfSubIndexes) && (true == EnableFlag))
				{
					return;
				}

				if ((TRUE == pobjSubIndex->GetFlagIfIncludedCdc())
						&& ((true == ReactivateMappingPDO(pobjIdxCol, pobjIndex))
								|| (true
										== IsDefaultActualNotEqual(pobjSubIndex))))
				{
					bool IfStringDT = false;
					//Format: 1600    01   00000000   Act_value
					strcat(Buffer, pobjIndex->GetIndexValue());
					//Place a tab
					strcat(Buffer, "\t");

					strcat(Buffer, pobjSubIndex->GetIndexValue());
					strcat(Buffer, "\t");
					//Add datatype
					DataType dt;
					dt = pobjSubIndex->GetDataType();
					INT32 padLength = 0;
					char* varDataSize = new char[8 + STR_ALLOC_BUFFER];

					if (NULL != dt.Name)
					{
						if (true == CheckIfStringDatatypes(dt.Name))
						{
							INT32 len = strlen(pobjSubIndex->GetActualValue());
							varDataSize = IntToAscii(len, varDataSize, 16);
							varDataSize = PadLeft(varDataSize, '0', 8);
							strcat(Buffer, varDataSize);
							padLength = len * 2;
							IfStringDT = true;
						}
						else
						{
							varDataSize = IntToAscii(dt.dataSize, varDataSize,
									16);
							varDataSize = PadLeft(varDataSize, '0', 8);
							strcat(Buffer, varDataSize);
							padLength = dt.dataSize * 2;
							IfStringDT = false;
						}
					}
					else
					{
						//txt2cdc will not be success
						strcat(Buffer, (const char*) "00000000");
					}
					delete[] varDataSize;
					strcat(Buffer, "\t");

					if (false == EnableFlag)
					{
						// Actual value checked for Empty check for 00'th subindex non-Zero value.
						if ((0 != strcmp(pobjSubIndex->GetActualValue(), ""))
								&& !(CheckIfValueZero(
										(char*) pobjSubIndex->GetActualValue())))
						{
							char actvalue[20];
							strcpy(actvalue, "0");
							strcat(Buffer, PadLeft(actvalue, '0', padLength));
						}
						else
						{
							//No need to enable/disable the mapping pdo if actual value set to zero or empty
						}
					}
					else
					{
						char actvalue[64];
						actvalue[0] = '\0';

						if (true == IfStringDT)
						{
							strcpy(actvalue,
									(char*) pobjSubIndex->GetActualValue());
							strcpy(actvalue,
									ConvertStringToHex((char*) actvalue));
							strcat(Buffer, actvalue);
						}
						else
						{
							//non empty non-zero actual values are only written to cdc
							if ((0 != strcmp(pobjSubIndex->GetActualValue(), ""))
									&& (!(CheckIfValueZero(
											(char*) pobjSubIndex->GetActualValue()))))
							{
								if (true
										== CheckIfHex(
												(char*) pobjSubIndex->GetActualValue()))
								{
									INT32 ilen =
											strlen(
													(char*) pobjSubIndex->GetActualValue());
									strncpy(actvalue,
											((char*) (pobjSubIndex->GetActualValue()
													+ 2)), ilen - 2);
									actvalue[ilen - 2] = '\0';

									strcat(Buffer,
											PadLeft(actvalue, '0', padLength));
								}
								else
								{
									strcpy(actvalue,
											IntToAscii(
													atoi(
															pobjSubIndex->GetActualValue()),
													actvalue, 16));
									strcat(Buffer,
											PadLeft(actvalue, '0', padLength));
								}
							}
						}
					}
					strcat(Buffer, "\n");
				}
			}
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND);
#if defined DEBUG
			cout << "NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND: Index: " << pobjIndex->GetIndexValue() << endl;
#endif
			throw objException;
		}
	}
	else
	{
#if defined DEBUG
		cout << "INDEX: " << pobjIndex->GetIndexValue() << " has Zero SubIndexs"
		<< endl;
#endif
	}
}

/*****************************************************************************/
/**
 \brief		UpdateCNCycleTime

 This function updates the cycle time index value(1006) of the CN

 \param		pobjIdxCol		Class pointer of IndexCollection for a CN node
 \param		varCycleTime		Character pointer to hold the new cycle time value

 \return	void
 */
/*****************************************************************************/

void UpdateCNCycleTime(IndexCollection* pobjIdxCol, char* varCycleTime)
{
	Index* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1006");
	if (NULL != pobjIndex)
	{
		if (NULL != varCycleTime)
		{
			strcpy(varCycleTime, ConvertToUpper(varCycleTime));
			pobjIndex->SetActualValue(varCycleTime);
		}
		else
		{
#if defined DEBUG
			cout << "Err_UpdateCNCycleTime" << endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1006 Not found, UpdateCNCycleTime failed" << endl;
#endif
	}

}

/*****************************************************************************/
/**
 \brief		UpdateCNSoCTolerance

 This function updates the Loss SoC Tolerance index value(1C14) of the CN

 \param		pobjIdxCol			Class pointer of IndexCollection for a CN node
 \param		varSocTolerance		Character pointer to hold the new Loss SoC Tolerance value

 \return	void
 */
/*****************************************************************************/

void UpdateCNSoCTolerance(IndexCollection* pobjIdxCol, char* varSocTolerance)
{
	Index* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1C14");
	if (NULL != pobjIndex)
	{
		if (NULL != varSocTolerance)
		{
			pobjIndex->SetActualValue(varSocTolerance);
			pobjIndex->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
#if defined DEBUG
			cout << "Err_UpdateCNSoCTolerance" << endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1C14 Not found, UpdateCNSoCTolerance failed" << endl;
#endif
	}

}

/*****************************************************************************/
/**
 \brief		UpdateCNAsyncMTUsize

 This function updates the Asynchronous MTU size(1F98/08) of the CN

 \param		pobjIdxCol			Class pointer of IndexCollection for a CN node
 \param		varAsyncMTUsize		Character pointer to hold the new AsyncMTUsize value

 \return	void
 */
/*****************************************************************************/

void UpdateCNAsyncMTUsize(IndexCollection* pobjIdxCol, char* varAsyncMTUsize)
{
	Index* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		SubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "08");
		if (NULL != pobjSubIndex)
		{
			if (NULL != varAsyncMTUsize)
			{
				pobjSubIndex->SetActualValue(varAsyncMTUsize);
			}
			else
			{
#if defined DEBUG
				cout << "Err_UpdateCNAsyncMTUsize" << endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "Index 1F98 / 08 Not found, UpdateCNAsyncMTUsize failed"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1F98 Not found, UpdateCNAsyncMTUsize failed" << endl;
#endif
	}

}

/*****************************************************************************/
/**
 \brief		UpdateCNMultiPrescal

 This function updates the Multilexing prescaler (1F98/07) of the CN

 \param		pobjIdxCol			Class pointer of IndexCollection for a CN node
 \param		varMultiPrescal		Character pointer to hold the new Multilexing prescaler value

 \return	void
 */
/*****************************************************************************/

void UpdateCNMultiPrescal(IndexCollection* pobjIdxCol, char* varMultiPrescal)
{
	Index* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		SubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "07");
		if (NULL != pobjSubIndex)
		{
			if (NULL != varMultiPrescal)
			{
				pobjSubIndex->SetActualValue(varMultiPrescal);
			}
			else
			{
#if defined DEBUG
				cout << "Err_UpdateCNMultiPrescal" << endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "Index 1F98 / 07 Not found, UpdateCNMultiPrescal failed"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1F98 Not found, UpdateCNMultiPrescal failed" << endl;
#endif
	}
}

/*****************************************************************************************/
/**
 \brief		UpdateCNNodeAssignment

 This function updates the subobjects of 1F81 in CN to MN if conditions are satisfied

 \param		pobjNode	Class pointer of Node for a CN node

 \return	void
 */
/******************************************************************************************/
//TODO: unused function
void UpdateCNNodeAssignment(Node* pobjNode)
{
	bool bCopy1F81 = false;
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	bCopy1F81 = ISCNNodeAssignmentValid(pobjNode);

	if (true == bCopy1F81)
	{
		IndexCollection* objCNIndexCollection = NULL;
		Index* pobjCN1F81idx = NULL;

		objCNIndexCollection = pobjNode->GetIndexCollection();
		if (NULL == objCNIndexCollection)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjCN1F81idx = objCNIndexCollection->GetIndexbyIndexValue(
				(char*) "1F81");
		if (NULL == pobjCN1F81idx)
		{
#if defined DEBUG
			cout << "CN: " << pobjNode->GetNodeId() << " INDEX: 1F81 Not found"
			<< endl;
#endif
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjCN1F81idx->SetFlagIfIncludedCdc(TRUE);

		Index* pobjMN1F81idx = NULL;
		pobjMN1F81idx = GetMNIndexValues((char*) "1F81");
		if (NULL == pobjMN1F81idx)
		{
#if defined DEBUG
			cout << "MN INDEX: 1F81 Not found" << endl;
#endif
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		SubIndex* pobjCN1F81Sidx = NULL;
		SubIndex* pobjMN1F81Sidx = NULL;

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjCN1F81idx->GetNumberofSubIndexes();
				iLoopCount++)
		{
			pobjCN1F81Sidx = pobjCN1F81idx->GetSubIndex(iLoopCount);
			if (NULL == pobjCN1F81Sidx)
			{
				continue;
			}

			pobjMN1F81Sidx = pobjMN1F81idx->GetSubIndexbyIndexValue(
					(char*) pobjCN1F81Sidx->GetIndexValue());

			if (NULL == pobjMN1F81Sidx)
			{
				continue;
			}

			if (NULL != pobjMN1F81Sidx->GetActualValue())
			{
				pobjCN1F81Sidx->SetActualValue(
						(char*) pobjMN1F81Sidx->GetActualValue());
			}
			else
			{
				pobjCN1F81Sidx->SetActualValue((char*) "");
			}
			pobjCN1F81Sidx->SetFlagIfIncludedCdc(TRUE);
		}
	}
	else
	{
#if defined DEBUG
		cout << "CN Node Assignment Invalid" << endl;
#endif
	}
}

/*****************************************************************************************/
/**
 \brief		ISCNNodeAssignmentValid

 This function checks whether the CN can implement 1F81 object or not

 \param		pobjNode	Class pointer of Node for a CN node

 \return	BOOL

 \retval		TRUE			if a CN can implement 1F81 object
 \retval		FALSE			if a CN cannot implement 1F81 object
 */
/******************************************************************************************/

bool ISCNNodeAssignmentValid(Node* pobjNode)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	INT32 iNodeId = 0;
	bool bCopy1F81 = false;
	NodeType varNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->GetNodeId();
	varNodeType = pobjNode->GetNodeType();

	stErrorInfo = IfIndexExists(iNodeId, varNodeType, (char*) "1F9B",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		stErrorInfo = IfSubIndexExists(iNodeId, varNodeType, (char*) "1F98",
				(char*) "07", &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			char* varMultiplCycleCnt = new char[20];
			stErrorInfo = GetSubIndexAttributes(iNodeId, varNodeType,
					(char*) "1F98", (char*) "07", ACTUALVALUE,
					varMultiplCycleCnt);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				if ((NULL != varMultiplCycleCnt)
						&& (0 != strcmp(varMultiplCycleCnt, ""))
						&& !(CheckIfValueZero(varMultiplCycleCnt)))
				{
					bCopy1F81 = true;
				}
			}
			else
			{
#if defined DEBUG
				cout << varNodeType << ":" << iNodeId
				<< "1F9B/07 GetSubIndexAttributes Failed" << endl;
#endif
			}
			delete[] varMultiplCycleCnt;
		}
		else
		{
#if defined DEBUG
			cout << varNodeType << ":" << iNodeId << "1F9B/07 Not Found"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << varNodeType << ":" << iNodeId << "1F9B Not Found" << endl;
#endif
	}

	stErrorInfo = IfIndexExists(iNodeId, varNodeType, (char*) "1016",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		bCopy1F81 = true;
	}
	else
	{
#if defined DEBUG
		cout << varNodeType << ":" << iNodeId << "1016 Not Found" << endl;
#endif
	}

	stErrorInfo = IfIndexExists(iNodeId, varNodeType, (char*) "1F8D",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		bCopy1F81 = true;
	}
	else
	{
#if defined DEBUG
		cout << varNodeType << ":" << iNodeId << "1F8D Not Found" << endl;
#endif
	}
	return bCopy1F81;
}

/*****************************************************************************************/
/**
 \brief		UpdateCNMultipleCycleAssign

 This function copies all the subobjects of 1F9B in MN to CN if conditions are satisfied

 \param		pobjNode	Class pointer of Node for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNMultipleCycleAssign(Node* pobjNode)
{
	IndexCollection* pobjCNIdxCol = NULL;
	Index* pobjCN1f9bIdx = NULL;
	Index* pobjMN1f9bIdx = NULL;
	SubIndex* pobjCN1f9bSidx = NULL;
	SubIndex* pobjMN1f9bSidx = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjCNIdxCol = pobjNode->GetIndexCollection();

	if (NULL == pobjCNIdxCol)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		throw objException;
	}

	pobjCN1f9bIdx = pobjCNIdxCol->GetIndexbyIndexValue((char*) "1F9B");
	if (NULL == pobjCN1f9bIdx)
	{
		return;
	}

	pobjCN1f9bIdx->SetFlagIfIncludedCdc(TRUE);
	pobjMN1f9bIdx = GetMNIndexValues((char*) "1F9B");
	if (NULL == pobjMN1f9bIdx)
	{
		return;
	}

	for (INT32 iLoopCount = 0;
			iLoopCount < pobjCN1f9bIdx->GetNumberofSubIndexes(); iLoopCount++)
	{

		pobjCN1f9bSidx = pobjCN1f9bIdx->GetSubIndex(iLoopCount);
		if (NULL == pobjCN1f9bSidx)
			continue;

		if (NULL != pobjCN1f9bSidx->GetIndexValue())
		{
			pobjMN1f9bSidx = pobjMN1f9bIdx->GetSubIndexbyIndexValue(
					(char*) pobjCN1f9bSidx->GetIndexValue());
		}

		if (NULL == pobjMN1f9bSidx)
			continue;

		if (NULL != pobjMN1f9bSidx->GetActualValue())
		{
			pobjCN1f9bSidx->SetActualValue(
					(char*) pobjMN1f9bSidx->GetActualValue());
		}
		else
		{
			pobjCN1f9bSidx->SetActualValue((char*) "");
		}
		pobjCN1f9bSidx->SetFlagIfIncludedCdc(TRUE);
	}
}

/*****************************************************************************************/
/**
 \brief		UpdateCNPresMNActLoad

 This function set the calculated PresMN load value in CN 1F8D object

 \param		pobjNode	Class pointer of Node for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNPresMNActLoad(Node* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	NodeType eNodeType;

	NodeCollection* pobjNodeCollection = NULL;
	IndexCollection* pobjIndexCollection = NULL;
	Index* pobj1F8Didx = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	if (NULL == pobjNodeCollection)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->GetNodeId();
	eNodeType = pobjNode->GetNodeType();
	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F8D",
			(char*) "F0", &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdateCNPresMNActLoad falied. 1F8D/F0 does not exitst" << endl;
#endif
		return;
	}

	char *varSubindexId = new char[SUBINDEX_LEN];
	strcpy(varSubindexId, (char*) "F0");

	pobjIndexCollection = pobjNode->GetIndexCollection();
	if (NULL == pobjIndexCollection)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobj1F8Didx = pobjIndexCollection->GetIndexbyIndexValue((char*) "1F8D");
	if (NULL == pobj1F8Didx)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
		throw objException;
	}
	else
	{
		SubIndex *pobj1f8dSidx = NULL;
		pobj1f8dSidx = pobj1F8Didx->GetSubIndexbyIndexValue(varSubindexId);
		if (NULL == pobj1f8dSidx)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}
		else
		{
			if (CHAINED == pobjNode->GetStationType())
			{
				char convalue[20];
				char actvalue[22];
				Node pobjMNnode;

				memset(convalue, 0, 20 * sizeof(char));
				memset(actvalue, 0, 22 * sizeof(char));
				pobjMNnode = pobjNodeCollection->GetMNNode();

				if (PRES_DEFAULT_PAYLOAD > pobjMNnode.GetPResActPayloadValue())
				{
					IntToAscii(PRES_DEFAULT_PAYLOAD, convalue, 16);
				}
				else
				{
					IntToAscii(pobjMNnode.GetPResActPayloadValue(), convalue,
							16);
				}
				strcpy(actvalue, (char*) "0x");
				strcat(actvalue, convalue);
				pobj1f8dSidx->SetActualValue(actvalue);

				pobj1F8Didx->SetFlagIfIncludedCdc(TRUE);
				pobj1f8dSidx->SetFlagIfIncludedCdc(TRUE);
			}
			else
			{
				pobj1f8dSidx->SetActualValue((char*) "");
				pobj1f8dSidx->SetFlagIfIncludedCdc(FALSE);
			}
		}
	}
	delete[] varSubindexId;
}

/*****************************************************************************************/
/**
 \brief		UpdatePreqActLoad

 This function set the calculated Preq load value in CN

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdatePreqActLoad(Node* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	IndexCollection* pobjIndexColl = NULL;
	Index* pobj1f9bIdx = NULL;

	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	NodeType eNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->GetNodeId();
	eNodeType = pobjNode->GetNodeType();
	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F98",
			(char*) "04", &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdatePreqActLoad falied. 1F98/04 does not exitst" << endl;
#endif
		return;
	}

	pobjIndexColl = pobjNode->GetIndexCollection();
	if (NULL == pobjIndexColl)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobj1f9bIdx = pobjIndexColl->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != pobj1f9bIdx)
	{
		SubIndex* pobjSubIndex = NULL;
		char* varSubindexId = new char[SUBINDEX_LEN];
		strcpy(varSubindexId, (char*) "04");
		pobjSubIndex = pobj1f9bIdx->GetSubIndexbyIndexValue(varSubindexId);
		if (NULL != pobjSubIndex)
		{
			char convalue[20];
			memset(convalue, 0, 20 * sizeof(char));
			char actvalue[22];
			memset(actvalue, 0, 22 * sizeof(char));

			if (PREQ_DEFAULT_PAYLOAD > pobjNode->GetPReqActPayloadValue())
			{
				IntToAscii(PREQ_DEFAULT_PAYLOAD, convalue, 16);
			}
			else
			{
				IntToAscii(pobjNode->GetPReqActPayloadValue(), convalue, 16);
			}
			strcpy((char*) convalue, ConvertToUpper((char*) convalue));
			strcpy(actvalue, (char*) "0x");
			strcat(actvalue, convalue);
			pobjSubIndex->SetActualValue(actvalue);
			pobj1f9bIdx->SetFlagIfIncludedCdc(TRUE);
			pobjSubIndex->SetFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == pobjNode->GetNodeType())
			{
				Index* pobj1f8bMnIdx = NULL;

				IntToAscii(pobjNode->GetNodeId(), varSubindexId, 16);
				varSubindexId = PadLeft(varSubindexId, '0', 2);

				stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
						(char*) "1F8B", varSubindexId, &subIndexPos, &IndexPos);
				if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
				{
					return;
				}
				pobj1f8bMnIdx = GetMNIndexValues((char*) "1F8B");
				if (NULL != pobj1f8bMnIdx)
				{
					SubIndex* pobj1f8bMnSubIdx = NULL;
					pobj1f8bMnSubIdx = pobj1f8bMnIdx->GetSubIndexbyIndexValue(
							varSubindexId);
					if (NULL != pobj1f8bMnSubIdx)
					{
						pobj1f8bMnSubIdx->SetActualValue(actvalue);
						pobj1f8bMnIdx->SetFlagIfIncludedCdc(TRUE);
						pobj1f8bMnSubIdx->SetFlagIfIncludedCdc(TRUE);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					pobj1f8bMnSubIdx = pobj1f8bMnIdx->GetSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != pobj1f8bMnSubIdx)
					{
						strcpy(actvalue, (char*) "0xFE"); //to make display of 1f8b in cdc if act != def value
						pobj1f8bMnSubIdx->SetActualValue(actvalue);
					}
				}
			}
		}
		delete[] varSubindexId;
	}

}

/*****************************************************************************************/
/**
 \brief		UpdatePresActLoad

 This function sets the calculated Pres load value in CN 

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdatePresActLoad(Node* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	Index* pobjIndex = NULL;

	IndexCollection* pobjIndexColl = NULL;
	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	char convalue[20];
	char actvalue[22];
	char* varSubindexId = NULL;
	NodeType eNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	memset(convalue, 0, 20 * sizeof(char));
	memset(actvalue, 0, 22 * sizeof(char));
	iNodeId = pobjNode->GetNodeId();
	eNodeType = pobjNode->GetNodeType();
	varSubindexId = new char[SUBINDEX_LEN];
	strcpy(varSubindexId, (char*) "05");

	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F98",
			varSubindexId, &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdatePresActLoad falied. 1F98/05 does not exitst" << endl;
#endif
		return;
	}

	pobjIndexColl = pobjNode->GetIndexCollection();
	if (NULL == pobjIndexColl)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}
	pobjIndex = pobjIndexColl->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		SubIndex *pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(varSubindexId);
		if (NULL != pobjSubIndex)
		{
			if (PRES_DEFAULT_PAYLOAD > pobjNode->GetPResActPayloadValue())
			{
				IntToAscii(PRES_DEFAULT_PAYLOAD, convalue, 16);
			}
			else
			{
				IntToAscii(pobjNode->GetPResActPayloadValue(), convalue, 16);
			}
			strcpy((char*) convalue, ConvertToUpper((char*) convalue));

			strcpy(actvalue, (char*) "0x");
			strcat(actvalue, convalue);
			pobjSubIndex->SetActualValue(actvalue);
			pobjIndex->SetFlagIfIncludedCdc(TRUE);
			pobjSubIndex->SetFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == pobjNode->GetNodeType())
			{
				IntToAscii(pobjNode->GetNodeId(), varSubindexId, 16);
				varSubindexId = PadLeft(varSubindexId, '0', 2);

				stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
						(char*) "1F8D", varSubindexId, &subIndexPos, &IndexPos);
				if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
				{
#if defined DEBUG
					cout << "UpdatePresActLoad falied. MN 1F8D/" << varSubindexId
					<< " does not exitst" << endl;
#endif
					return;
				}
				pobjIndex = GetMNIndexValues((char *) "1F8D");
				if (NULL != pobjIndex)
				{
					pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(
							varSubindexId);
					if (NULL != pobjSubIndex)
					{
						pobjSubIndex->SetActualValue(actvalue);
						pobjIndex->SetFlagIfIncludedCdc(TRUE);
						pobjSubIndex->SetFlagIfIncludedCdc(TRUE);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != pobjSubIndex)
					{
						strcpy(actvalue, (char*) "0xFE"); //to make display of 1f8d in cdc if act != def value
						pobjSubIndex->SetActualValue(actvalue);
					}
				}
				else
				{
#if defined DEBUG
					cout
					<< "UpdatePresActLoad falied. MN 1F8D Index does not exitst"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "UpdatePresActLoad falied. Processed node is MN"
				<< endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "UpdatePresActLoad falied. 1F98/05 Index does not exitst"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "UpdatePresActLoad falied. 1F98 Index does not exitst" << endl;
#endif
	}
	delete[] varSubindexId;
}

/*****************************************************************************************/
/**
 \brief		UpdateCNVisibleNode

 This function updates 1F81 and 1F8D of cross traffic stations

 \param		pobjNode	Class pointer of Node for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNVisibleNode(Node* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	IndexCollection* objPDOCollection = NULL;
	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	objPDOCollection = pobjNode->getPDOIndexCollection(&tpdoCount, &rpdoCount);

	if (NULL == objPDOCollection)
	{
#if defined DEBUG
		cout << "UpdateCNVisibleNode falied: PDOCollection empty." << endl;
#endif
		return;
	}

	Index* pobjIndex = NULL;
	Index* pobjCommIndex = NULL;
	SubIndex* pobjEntriesSubIndex = NULL;
	SubIndex* pobjNodeIDSubIndex = NULL;
	char* varIdx = NULL;
	char* varCommIdx = NULL;
	char* varMappedNodeId = NULL;
	INT32 iCrossStnCnt = 0;

	ResetAllSubIndexFlag(
			(pobjNode->GetIndexCollection())->GetIndexbyIndexValue(
					(char*) "1F81"));
	ResetAllSubIndexFlag(
			(pobjNode->GetIndexCollection())->GetIndexbyIndexValue(
					(char*) "1F8D"));

	for (INT32 iLoopCount = 0;
			iLoopCount < objPDOCollection->GetNumberofIndexes(); iLoopCount++)
	{
		pobjIndex = objPDOCollection->GetIndex(iLoopCount);
		if ((NULL != pobjIndex)
				&& (0 == strncmp(pobjIndex->GetIndexValue(), "16", 2)))
		{
			pobjEntriesSubIndex = pobjIndex->GetSubIndexbyIndexValue(
					(char*) "00");

			if (NULL != pobjEntriesSubIndex)
			{
				if ((NULL != pobjEntriesSubIndex->GetActualValue())
						&& (0
								!= strcmp(pobjEntriesSubIndex->GetActualValue(),
										""))
						&& !(CheckIfValueZero(
								(char*) pobjEntriesSubIndex->GetActualValue())))
				{
					varCommIdx = new char[INDEX_LEN];
					varIdx = SubString((char*) pobjIndex->GetIndexValue(), 2, 2);
					strcpy(varCommIdx, (char*) "14");
					strcat(varCommIdx, varIdx);
					delete[] varIdx;

					pobjCommIndex = objPDOCollection->GetIndexbyIndexValue(
							varCommIdx);
					delete[] varCommIdx;

					if (NULL != pobjCommIndex)
					{
						pobjNodeIDSubIndex =
								pobjCommIndex->GetSubIndexbyIndexValue(
										(char*) "01");
						if (NULL != pobjNodeIDSubIndex)
						{
							if ((NULL != pobjNodeIDSubIndex->GetActualValue())
									&& (0
											!= strcmp(
													pobjNodeIDSubIndex->GetActualValue(),
													""))
									&& !(CheckIfValueZero(
											(char*) pobjNodeIDSubIndex->GetActualValue())))
							{
								iCrossStnCnt++;
								if (MAX_CN_CROSS_TRAFFIC_STN < iCrossStnCnt)
								{
									ocfmException objocfmException;
									objocfmException._ocfmRetCode.code =
											OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN;
									char acCustomError[200];
									INT32 iSprintf = 0;
									iSprintf =
											sprintf(acCustomError,
													"The cross trafficking in CN Node ID:%d excceds the maximum permissible station %d",
													pobjNode->GetNodeId(),
													MAX_CN_CROSS_TRAFFIC_STN);
									if (iSprintf < 0)
									{
										cout << "Error in sprintf" << __LINE__
												<< endl;
									}
									else
									{
										//sprintf success
									}
									CopyCustomErrorString(
											&(objocfmException._ocfmRetCode),
											acCustomError);
									throw objocfmException;
								}

								//copy the MN objects
								if (CheckIfHex(
										(char*) pobjNodeIDSubIndex->GetActualValue()))
								{
									varMappedNodeId =
											SubString(
													(char*) pobjNodeIDSubIndex->GetActualValue(),
													2, 2);
								}
								else
								{
									varMappedNodeId = new char[SUBINDEX_LEN];
									strcpy(varMappedNodeId,
											pobjNodeIDSubIndex->GetActualValue());
								}
								varMappedNodeId = PadLeft(varMappedNodeId, '0',
										2);

								if (true == ISCNNodeAssignmentValid(pobjNode))
								{
									if (true
											== CopyMNSubindexToCN(pobjNode,
													(char*) "1F81",
													varMappedNodeId))
									{
										if (true
												== CopyMNSubindexToCN(pobjNode,
														(char*) "1F8D",
														varMappedNodeId))
										{
											//CopyMNSubindexToCN 1F8D Success
										}
										else
										{
											cout
													<< "CopyMNSubindexToCN 1F8D Fails:"
													<< __FUNCTION__ << endl;
										}
									}
									else
									{
										cout << "CopyMNSubindexToCN 1F81 Fails:"
												<< __FUNCTION__ << endl;
									}
								}

								delete[] varMappedNodeId;
							}
						}
					}

				}
			}

		}
	}

}

/*****************************************************************************************/
/**
 \brief		CopyMNSubindexToCN

 This function copies the MN subindex value into corresponding CN subindex 

 \param		pobjNode		Class pointer of Node for a CN node
 \param		varIndex		Character pointer to hold the Index value
 \param		varSubIndex		Character pointer to hold the subindex value


 \return	BOOL

 \retval		TRUE			if successfuly copied
 \retval		FALSE			if not copied
 */
/******************************************************************************************/

bool CopyMNSubindexToCN(Node* pobjNode, char* varIndex, char* varSubIndex)
{
	bool bCopied = false;

	IndexCollection* pobjIndexCollection = NULL;
	Index* pobjMNIndex = NULL;
	Index* pobjCNIndex = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjIndexCollection = pobjNode->GetIndexCollection();
	if (NULL == pobjIndexCollection)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	if ((NULL == varIndex) || (NULL == varSubIndex))
	{
#if defined DEBUG
		cout << "CopyMNSubindexToCN falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_UNKNOWN);
		throw objException;
	}
	pobjCNIndex = pobjIndexCollection->GetIndexbyIndexValue(varIndex);
	pobjMNIndex = GetMNIndexValues(varIndex);
	if ((NULL != pobjCNIndex) && (NULL != pobjMNIndex))
	{
		SubIndex* pobjMNSubindex = NULL;
		SubIndex* pobjCNSubindex = NULL;
		pobjCNSubindex = pobjCNIndex->GetSubIndexbyIndexValue(varSubIndex);
		pobjMNSubindex = pobjMNIndex->GetSubIndexbyIndexValue(varSubIndex);

		if ((NULL == pobjCNSubindex) || (NULL == pobjMNSubindex))
		{
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN falied: MNSubindex/CNSubindex Not present"
			<< endl;
#endif
		}
		else if (NULL != pobjMNSubindex->GetActualValue())
		{
			pobjCNSubindex->SetActualValue(
					(char*) pobjMNSubindex->GetActualValue());
			pobjCNSubindex->SetFlagIfIncludedCdc(TRUE);
			pobjCNIndex->SetFlagIfIncludedCdc(TRUE);
			bCopied = true;
		}
		else
		{
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN falied: pobjMNSubindex has No Actual Value configured"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout
		<< "CopyMNSubindexToCN falied: MNIndex/CNIndex is Not Present"
		<< endl;
#endif
	}
	return bCopied;
}

/*****************************************************************************************/
/**
 \brief		ResetAllSubIndexFlag

 This function resets the Include in cdc flag for all the sub index

 \param		pobjIndex	Class pointer of CIndex to get all the subindexes

 \return	void
 */
/******************************************************************************************/

void ResetAllSubIndexFlag(Index* pobjIndex)
{
	if (NULL == pobjIndex)
	{
#if defined DEBUG
		cout << "ResetAllSubIndexFlag falied: Index Not found" << endl;
#endif
		return;
	}
	SubIndex* pobjSubIndex = NULL;
	for (INT32 iLoopCount = 0; iLoopCount < pobjIndex->GetNumberofSubIndexes();
			iLoopCount++)
	{
		pobjSubIndex = pobjIndex->GetSubIndex(iLoopCount);
		if (NULL != pobjSubIndex)
		{
			pobjSubIndex->SetFlagIfIncludedCdc(FALSE);
		}
		else
		{
#if defined DEBUG
			cout << "Some subindexes are NULL. Check for Memory operations"
			<< endl;
#endif
		}
	}
	pobjIndex->SetFlagIfIncludedCdc(FALSE);
}

/*****************************************************************************/
/**
 \brief		ResetAllPdos
 
 This function resets the values in all the PDO(14xx, 16xx, 18xx, 1Axx) indexes and subindexes to 0
 
 \param		nodeId		Integer Variable to hold node id
 \param		nodeType	Enum of type ENodeType to hold node type

 \return	void
 */
/*****************************************************************************/
void ResetAllPdos(INT32 nodeId, NodeType nodeType)
{
	NodeCollection *objNodeCol = NULL;
	Node *objNode = NULL;
	IndexCollection *objIdxCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(nodeType, nodeId);
	objIdxCol = objNode->GetIndexCollection();

	INT32 idxLoopCnt = 0;
	for (idxLoopCnt = 0; idxLoopCnt < objIdxCol->GetNumberofIndexes(); idxLoopCnt++)
	{
		Index *pobjIndex = NULL;
		pobjIndex = objIdxCol->GetIndex(idxLoopCnt);
		if((NULL == pobjIndex) || (NULL == pobjIndex->GetIndexValue()))
		{
			continue;
		}

		if(!CheckIfNotPDO((char*)pobjIndex->GetIndexValue()))
		{
			INT32 totalNoOfSidx = 0;
			INT32 sidxLoopCnt = 0;
			totalNoOfSidx = pobjIndex->GetNumberofSubIndexes();

			char* idxSubString = new char[SUBINDEX_LEN];
			idxSubString = SubString((char*)pobjIndex->GetIndexValue(), 0, 2);
			if ((0 == strcmp(idxSubString, "1A"))
				|| (0 == strcmp(idxSubString, "1a"))
				|| (0 == strcmp(idxSubString, "16"))
				)
			{
				for(sidxLoopCnt = 0; sidxLoopCnt < totalNoOfSidx; sidxLoopCnt++)
				{
					SubIndex *objSubIdx = NULL;
					objSubIdx = pobjIndex->GetSubIndex(sidxLoopCnt);
					if((NULL == objSubIdx) || (NULL == objSubIdx->GetIndexValue()))
					{
						continue;
					}
					if((0 == strcmp(objSubIdx->GetIndexValue(), (char*)"00")))
					{
						objSubIdx->SetActualValue((char*)"0x0");
					}
					else
					{
						objSubIdx->SetActualValue((char*)"0x0000000000000000");
					}

				}
				//get 00 sidx set val 0
				//get all idx set actval 0x000..(16)
			}
			if((0 == strcmp(idxSubString, "14"))
				|| (0 == strcmp(idxSubString, "18"))
				)
			{
				for(sidxLoopCnt = 0; sidxLoopCnt < totalNoOfSidx; sidxLoopCnt++)
				{
					SubIndex *objSubIdx = NULL;
					objSubIdx = pobjIndex->GetSubIndex(sidxLoopCnt);
					if((NULL == objSubIdx) || (NULL == objSubIdx->GetIndexValue()))
					{
						continue;
					}
					if((0 == strcmp(objSubIdx->GetIndexValue(), (char*)"00")))
					{
						//Not to set the value for nr.of entries sidx
					}
					else
					{
						objSubIdx->SetActualValue((char*)"0x0");
					}
				}
			}
			delete[] idxSubString;
		}
	}
}
/*****************************************************************************************/
/**
 \brief		GetIndexData

 This function writes the buffer(char pointer) with the index data(id, datatype value, actual value) to be used for the cdc

 \param		objIndex	Class pointer of Index for node
 \param		Buffer		Character pointer to hold the buffer data

 \return	void
 */
/******************************************************************************************/

void GetIndexData(Index* objIndex, char* Buffer)
{
	if ((NULL == objIndex) || (NULL == Buffer))
	{
#if defined DEBUG
		cout << "GetIndexData falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	bool IfStringDT = false;
	//Get the Index Value

	strcpy(Buffer, "");
	if ((0 == objIndex->GetNumberofSubIndexes()))
	{
		if (NULL != objIndex->GetActualValue())
		{
			if (true == IsDefaultActualNotEqual(objIndex))
			{
				strcpy(Buffer, objIndex->GetIndexValue());
				strcat(Buffer, "\t");

				//There are no subindexes, So add "00"
				strcat(Buffer, "00");
				strcat(Buffer, "\t");

				DataType dt;
				INT32 iPadLength = 0;
				char* varDataSize = new char[8 + STR_ALLOC_BUFFER];

				dt = objIndex->GetDataType();
				if (NULL != dt.Name)
				{
					if (!CheckIfStringDatatypes(dt.Name))
					{
						varDataSize = IntToAscii(dt.dataSize, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						iPadLength = dt.dataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objIndex->GetActualValue());
						varDataSize = IntToAscii(iLen, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						iPadLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					//txt2cdc will not be successful
					strcat(Buffer, "00000000");
				}
				delete[] varDataSize;

				strcat(Buffer, "\t");

				char actvalue[64];
				actvalue[0] = '\0';
				if (IfStringDT)
				{
					strcpy(actvalue, (char*) objIndex->GetActualValue());
					strcpy(actvalue, ConvertStringToHex((char*) actvalue));
					strcat(Buffer, actvalue);
				}
				else
				{
					if (CheckIfHex((char*) objIndex->GetActualValue()))
					{
						INT32 iLen = strlen((char*) objIndex->GetActualValue());
						strncpy(actvalue, (objIndex->GetActualValue() + 2),
								iLen - 2);
						actvalue[iLen - 2] = '\0';
						strcat(Buffer, PadLeft(actvalue, '0', iPadLength));
					}
					else
					{
						strcpy(actvalue,
								IntToAscii(atoi(objIndex->GetActualValue()),
										actvalue, 16));
						strcat(Buffer, PadLeft(actvalue, '0', iPadLength));
					}
				}
				strcat(Buffer, "\n");
			}
			else
			{
				//Default&Actual values are same. So not written to CDC
			}
		}
		else
		{
			//Actual value not configured.
		}
	}
	else
	{
		// If Subobjects presents 
		INT32 noOfSubIndexes = 0;
		INT32 noOfTotalSubIndexes = 0;
		INT32 noOfValidSubIndexes = 0;
		bool Indexadded = false;
		bool resetValueAdded = false;
		bool flag_No_of_enteriesAdded = false;
		bool bMappingPDO = false;
		SubIndex* objSubIndex = NULL;

		bMappingPDO = CheckIfMappingPDO((char*) objIndex->GetIndexValue());
		noOfTotalSubIndexes = objIndex->GetNumberofSubIndexes();
		objSubIndex = objIndex->GetSubIndexbyIndexValue((char*) "00");
		if (objSubIndex != NULL)
		{
			if (objSubIndex->GetActualValue() != NULL)
			{
				if (CheckIfHex((char*) objSubIndex->GetActualValue()))
				{
					noOfSubIndexes = HexToInt(
							SubString((char*) objSubIndex->GetActualValue(), 2,
									strlen(objSubIndex->GetActualValue()) - 2));
				}
				else
				{
					noOfSubIndexes = atoi(objSubIndex->GetActualValue());
				}

				if (0 == noOfSubIndexes)
				{
					return;
				}

				noOfSubIndexes = noOfSubIndexes + 1;
			}
		}
		else
		{
			//00th subindex not present.
			noOfSubIndexes = noOfTotalSubIndexes;
		}

		if (bMappingPDO)
		{
			if (NULL == objSubIndex->GetActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfSubIndexes << endl;
#endif
		for (INT32 i = 0; i < noOfSubIndexes; i++)
		{
			bool bAccessType = false;

			objSubIndex = objIndex->GetSubIndex(i);
#if defined DEBUG
			cout << "Indexx:" << objIndex->GetIndexValue() << " SIdx:"
			<< objSubIndex->GetIndexValue() << endl;
#endif
			bAccessType = CheckAccessTypeForInclude(
					(char*) objSubIndex->GetAccessType());

			if ((objSubIndex->GetActualValue() != NULL)
					&& (objSubIndex->GetFlagIfIncludedCdc() == TRUE)
					&& ((true == bAccessType) || (true == bMappingPDO))
					&& (true == IsDefaultActualNotEqual(objSubIndex)))
			{
				noOfValidSubIndexes = noOfValidSubIndexes + 1;

				if (noOfValidSubIndexes == noOfSubIndexes)
				{
					i = noOfTotalSubIndexes - 1;
				}

				if (bMappingPDO)
				{
					if (0 == strcmp((char*) objSubIndex->GetIndexValue(), "00"))
					{
						continue;
					}
					if (0
							== GetDecimalValue(
									(char*) objSubIndex->GetActualValue()))
					{
						if ((NULL == objSubIndex->GetDefaultValue())
								|| (0
										== GetDecimalValue(
												(char*) objSubIndex->GetDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (Indexadded)
				{
					strcat(Buffer, objIndex->GetIndexValue());
				}
				else
				{
					strcpy(Buffer, objIndex->GetIndexValue());
					Indexadded = true;
				}

				//Place a tab
				strcat(Buffer, "\t");

				strcat(Buffer, objSubIndex->GetIndexValue());
				strcat(Buffer, "\t");
				//Add datatype
				DataType dt;
				dt = objSubIndex->GetDataType();
				INT32 padLength = 0;
				char* varDataSize = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dt.Name)
				{
					if (!CheckIfStringDatatypes(dt.Name))
					{
						varDataSize = IntToAscii(dt.dataSize, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						padLength = dt.dataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objSubIndex->GetActualValue());
						varDataSize = IntToAscii(iLen, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						padLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}

				delete[] varDataSize;
				strcat(Buffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(objSubIndex->GetIndexValue(), "00"))
						&& bMappingPDO && (false == resetValueAdded))
				{
					char actvalue[20];
					strcpy(actvalue, "0");
					strcat(Buffer, PadLeft(actvalue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actvalue[64];
					actvalue[0] = '\0';
					if (IfStringDT)
					{
						strcpy(actvalue, (char*) objSubIndex->GetActualValue());
						strcpy(actvalue, ConvertStringToHex((char*) actvalue));
						strcat(Buffer, actvalue);
					}
					else
					{
						if (CheckIfHex((char*) objSubIndex->GetActualValue()))
						{
							INT32 len = strlen(
									(char*) objSubIndex->GetActualValue());
							strncpy(actvalue,
									(objSubIndex->GetActualValue() + 2),
									len - 2);
							actvalue[len - 2] = '\0';

							strcat(Buffer, PadLeft(actvalue, '0', padLength));
						}
						else
						{
							strcpy(actvalue,
									IntToAscii(
											atoi(objSubIndex->GetActualValue()),
											actvalue, 16));
							strcat(Buffer, PadLeft(actvalue, '0', padLength));
						}
					}
				}

				strcat(Buffer, "\n");
				if ((0 == i) && bMappingPDO
						&& (true == flag_No_of_enteriesAdded))
				{
					i = noOfTotalSubIndexes - 1;
				}
				if ((i == noOfTotalSubIndexes - 1) && bMappingPDO
						&& (false == flag_No_of_enteriesAdded)
						&& (true == resetValueAdded))
				{
					i = -1;
					flag_No_of_enteriesAdded = true;
				}
			}
		}

	}
}

/*****************************************************************************************/
/**
 \brief		BRSpecificGetIndexData

 This function writes the buffer(char pointer) with the index data(id, datatype value, actual value) to be used for the cdc in a BnR specific format.

 \param		objIndex	Class pointer of Index for node
 \param		Buffer		Character pointer to hold the buffer data
 \param		iNodeId		Integer variable to hold the Node id to be written to the 1F81 index

 \return	void
 */
/******************************************************************************************/

void BRSpecificGetIndexData(Index* objIndex, char* Buffer, INT32 iNodeId)
{
	if ((NULL == objIndex) || (NULL == Buffer))
	{
#if defined DEBUG
		cout << "BRSpecificGetIndexData falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	bool IfStringDT = false;
	//Get the Index Value

	strcpy(Buffer, "");
	if ((0 == objIndex->GetNumberofSubIndexes()))
	{
		if (NULL != objIndex->GetActualValue())
		{
			if (true == IsDefaultActualNotEqual(objIndex))
			{
				strcpy(Buffer, objIndex->GetIndexValue());
				strcat(Buffer, "\t");

				//There are no subindexes, So add "00"
				strcat(Buffer, "00");
				strcat(Buffer, "\t");

				DataType dt;
				INT32 iPadLength = 0;
				char* varDataSize = new char[8 + STR_ALLOC_BUFFER];

				dt = objIndex->GetDataType();
				if (NULL != dt.Name)
				{
					if (!CheckIfStringDatatypes(dt.Name))
					{
						varDataSize = IntToAscii(dt.dataSize, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						iPadLength = dt.dataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objIndex->GetActualValue());
						varDataSize = IntToAscii(iLen, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						iPadLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}
				delete[] varDataSize;

				strcat(Buffer, "\t");

				char actvalue[64];
				actvalue[0] = '\0';
				if (IfStringDT)
				{
					strcpy(actvalue, (char*) objIndex->GetActualValue());
					strcpy(actvalue, ConvertStringToHex((char*) actvalue));
					strcat(Buffer, actvalue);
				}
				else
				{
					if (CheckIfHex((char*) objIndex->GetActualValue()))
					{
						INT32 iLen = strlen((char*) objIndex->GetActualValue());
						strncpy(actvalue, (objIndex->GetActualValue() + 2),
								iLen - 2);
						actvalue[iLen - 2] = '\0';
						strcat(Buffer, PadLeft(actvalue, '0', iPadLength));
					}
					else
					{
						strcpy(actvalue,
								IntToAscii(atoi(objIndex->GetActualValue()),
										actvalue, 16));
						strcat(Buffer, PadLeft(actvalue, '0', iPadLength));
					}
				}
				strcat(Buffer, "\n");
			}
			else
			{
				//Default&Actual values are same. So not written to CDC
			}
		}
		else
		{
			//Actual value not configured.
		}
	}
	else
	{
		// If Subobjects presents 
		INT32 noOfSubIndexes = 0;
		INT32 noOfTotalSubIndexes = 0;
		INT32 noOfValidSubIndexes = 0;
		bool Indexadded = false;
		bool resetValueAdded = false;
		bool flag_No_of_enteriesAdded = false;
		bool bMappingPDO = false;
		SubIndex* objSubIndex = NULL;
		char var1F81nodeid[10];

		bMappingPDO = CheckIfMappingPDO((char*) objIndex->GetIndexValue());
		noOfTotalSubIndexes = objIndex->GetNumberofSubIndexes();
		objSubIndex = objIndex->GetSubIndexbyIndexValue((char*) "00");
		if (objSubIndex != NULL)
		{
			if (objSubIndex->GetActualValue() != NULL)
			{
				if (CheckIfHex((char*) objSubIndex->GetActualValue()))
				{
					noOfSubIndexes = HexToInt(
							SubString((char*) objSubIndex->GetActualValue(), 2,
									strlen(objSubIndex->GetActualValue()) - 2));
				}
				else
				{
					noOfSubIndexes = atoi(objSubIndex->GetActualValue());
				}

				if (0 == noOfSubIndexes)
				{
					return;
				}

				noOfSubIndexes = noOfSubIndexes + 1;
			}
		}
		else
		{
			//00th subindex not present.
			noOfSubIndexes = noOfTotalSubIndexes;
		}

		if (bMappingPDO)
		{
			if (NULL == objSubIndex->GetActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}

		if ((0 == strcmp(objIndex->GetIndexValue(), (char*) "1F81")))
		{
			IntToAscii(iNodeId, var1F81nodeid, 16);
			PadLeft(var1F81nodeid, '0', 2);
		}
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfTotalSubIndexes << endl;
#endif
		for (INT32 i = 0; i < noOfTotalSubIndexes; i++)
		{
			bool bAccessType = false;
#if defined DEBUG
			cout << "Indexx:" << objIndex->GetIndexValue() << " SIdx:"
			<< objSubIndex->GetIndexValue() << endl;
#endif
			objSubIndex = objIndex->GetSubIndex(i);

			bAccessType = CheckAccessTypeForInclude(
					(char*) objSubIndex->GetAccessType());

			if ((objSubIndex->GetActualValue() != NULL)
					&& (objSubIndex->GetFlagIfIncludedCdc() == TRUE)
					&& ((true == bAccessType) || (true == bMappingPDO))
					&& (true == IsDefaultActualNotEqual(objSubIndex)))
			{
				if ((0 == strcmp(objIndex->GetIndexValue(), (char*) "1F81")))
				{
					if ((0
							== strcmp(objSubIndex->GetIndexValue(),
									(const char*) "00"))
							|| (0
									== strcmp(objSubIndex->GetIndexValue(),
											var1F81nodeid)))
					{
						continue;
					}
					else
					{
					}
				}
				noOfValidSubIndexes = noOfValidSubIndexes + 1;

				if (noOfValidSubIndexes == noOfSubIndexes)
				{
					i = noOfTotalSubIndexes - 1;
				}

				if (bMappingPDO)
				{
					if (0 == strcmp((char*) objSubIndex->GetIndexValue(), "00"))
					{
						continue;
					}
					if (0
							== GetDecimalValue(
									(char*) objSubIndex->GetActualValue()))
					{
						if ((NULL == objSubIndex->GetDefaultValue())
								|| (0
										== GetDecimalValue(
												(char*) objSubIndex->GetDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (Indexadded)
				{
					strcat(Buffer, objIndex->GetIndexValue());
				}
				else
				{
					strcpy(Buffer, objIndex->GetIndexValue());
					Indexadded = true;
				}

				//Place a tab
				strcat(Buffer, "\t");

				strcat(Buffer, objSubIndex->GetIndexValue());
				strcat(Buffer, "\t");
				//Add datatype
				DataType dt;
				dt = objSubIndex->GetDataType();
				INT32 padLength = 0;
				char* varDataSize = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dt.Name)
				{
					if (!CheckIfStringDatatypes(dt.Name))
					{
						varDataSize = IntToAscii(dt.dataSize, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						padLength = dt.dataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objSubIndex->GetActualValue());
						varDataSize = IntToAscii(iLen, varDataSize, 16);
						varDataSize = PadLeft(varDataSize, '0', 8);
						strcat(Buffer, varDataSize);
						padLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}

				delete[] varDataSize;
				strcat(Buffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(objSubIndex->GetIndexValue(), "00"))
						&& bMappingPDO && (false == resetValueAdded))
				{
					char actvalue[20];
					strcpy(actvalue, "0");
					strcat(Buffer, PadLeft(actvalue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actvalue[64];
					actvalue[0] = '\0';
					if (IfStringDT)
					{
						strcpy(actvalue, (char*) objSubIndex->GetActualValue());
						strcpy(actvalue, ConvertStringToHex((char*) actvalue));
						strcat(Buffer, actvalue);
					}
					else
					{
						if (CheckIfHex((char*) objSubIndex->GetActualValue()))
						{
							INT32 len = strlen(
									(char*) objSubIndex->GetActualValue());
							strncpy(actvalue,
									(objSubIndex->GetActualValue() + 2),
									len - 2);
							actvalue[len - 2] = '\0';

							strcat(Buffer, PadLeft(actvalue, '0', padLength));
						}
						else
						{
							strcpy(actvalue,
									IntToAscii(
											atoi(objSubIndex->GetActualValue()),
											actvalue, 16));
							strcat(Buffer, PadLeft(actvalue, '0', padLength));
						}
					}
				}

				strcat(Buffer, "\n");
				if ((0 == i) && bMappingPDO
						&& (true == flag_No_of_enteriesAdded))
				{
					i = noOfTotalSubIndexes - 1;
				}
				if ((i == noOfTotalSubIndexes - 1) && bMappingPDO
						&& (false == flag_No_of_enteriesAdded)
						&& (true == resetValueAdded))
				{
					i = -1;
					flag_No_of_enteriesAdded = true;
				}
			}
		}

	}
}

/*****************************************************************************************/
/**
 \brief		WriteCNsData

 This function writes the CN's all index datas into CDC file(mnobd.txt)

 \param		fileName	Character pointer to hold the full path of the file (mnobd.txt)

 \return	void
 */
/******************************************************************************************/

void WriteCNsData(char* fileName)
{
	char* Buffer2 = NULL;
	char* Buffer3 = NULL;
	INT32 len = 0;
	char* c = new char[50];
	FILE* fileptr = new FILE();

	NodeCollection* objNodeCollection = NULL;
	Node objNode;
	IndexCollection* objIndexCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	if (NULL == objNodeCollection)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		delete[] c;
		throw objException;
	}

	INT32 CNCount = 0;

	for (INT32 count = 0; count < objNodeCollection->GetNumberOfNodes();
			count++)
	{
		objNode = objNodeCollection->GetNodebyCollectionIndex(count);
		if (CN == objNode.GetNodeType())
		{
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}

			//AddOtherRequiredCNIndexes(objNode.getNodeId());
			objIndexCollection = objNode.GetIndexCollection();
			if (NULL == objIndexCollection)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				delete[] fileptr;
				throw objException;
			}

			char* comment = new char[50];
			IntToAscii(CNCount + 1, c, 10);
			CNCount++;

			strcpy(comment, "////Configuration Data for CN-");
			comment = strcat(comment, c);
			comment = strcat(comment, "\n");
			len = strlen(comment);
			if ((fwrite(comment, sizeof(char), len, fileptr)) != 0)
			{
				fclose(fileptr);
			}

			delete[] comment;

			Index* objIndex = NULL;
			char* Buffer4 = NULL;

			/*************WRITE MN'S 1006,1020 Indexes Values *******************************/
			Buffer4 = new char[30000];
			Buffer2 = new char[60000];

			objIndex = GetMNIndexValues((char*) "1006");
			if (NULL != objIndex)
			{
				if (NULL != (char*) objIndex->GetActualValue())
				{
					UpdateCNCycleTime(objIndexCollection,
							(char*) objIndex->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1006 ActualValue Not Present, Not Updated CNCycleTime"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1006 Not Found, Not Updated CNCycleTime"
				<< endl;
#endif
			}

			objIndex = GetMNIndexValues((char*) "1C14");
			if (NULL != objIndex)
			{
				if (NULL != (char*) objIndex->GetActualValue())
				{
					UpdateCNSoCTolerance(objIndexCollection,
							(char*) objIndex->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1C14 ActualValue Not Present, Not Updated CNCycleTime"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1C14 Not Found, Not Updated CNSoCTolerance"
				<< endl;
#endif
			}

			objIndex = GetMNIndexValues((char*) "1F26");
			if (NULL != objIndex)
			{
				UpdatedCNDateORTime(objIndex, objNode.GetNodeId(), DATE);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F26 Not Found, Not Updated CN Date" << endl;
#endif
			}

			objIndex = GetMNIndexValues((char*) "1F27");
			if (NULL != objIndex)
			{
				UpdatedCNDateORTime(objIndex, objNode.GetNodeId(), TIME);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F27 Not Found, Not Updated CN Time" << endl;
#endif
			}

			objIndex = GetMNIndexValues((char*) "1F98");
			if (NULL != objIndex)
			{
				SubIndex* pobjSubIndex = NULL;
				pobjSubIndex = GetMNSubIndexValues((char*) "1F98",
						(char*) "08");
				if ((NULL != pobjSubIndex)
						&& (NULL != pobjSubIndex->GetActualValue()))
				{
					UpdateCNAsyncMTUsize(objIndexCollection,
							(char*) pobjSubIndex->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1F98/08 Not Found or ActualValue not present"
					<< endl;
#endif
				}

				pobjSubIndex = GetMNSubIndexValues((char*) "1F98",
						(char*) "07");
				if ((NULL != pobjSubIndex)
						&& (NULL != pobjSubIndex->GetActualValue()))
				{
					UpdateCNMultiPrescal(objIndexCollection,
							(char*) pobjSubIndex->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1F98/07 Not Found or ActualValue not present"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout
				<< "MN index: 1F98 Not Found, Not Updated CN AsyncMTUsize & MultiPrescal"
				<< endl;
#endif
			}

			objIndex = GetMNIndexValues((char*) "1F9B");
			if (NULL != objIndex)
			{
				UpdateCNMultipleCycleAssign(&objNode);
			}
			else
			{
#if defined DEBUG
				cout
				<< "MN index: 1F9B Not Found, Not Updated CN MultipleCycleAssign"
				<< endl;
#endif
			}

			UpdateCNVisibleNode(&objNode);
			UpdateCNPresMNActLoad(&objNode);

			strcpy(Buffer2, "");
			strcpy(Buffer4, "");

			char* NoOfenteries = new char[10];
			//workaround for B&R Bus Controller stack
			//NoOfenteries = _IntToAscii(getNodeTotalIndexSubIndex(objNode.getNodeId()), NoOfenteries, 16);
			NoOfenteries = IntToAscii(
					BRSpecificgetCNsTotalIndexSubIndex(objNode.GetNodeId()),
					NoOfenteries, 16);
			/*1 is not added for the size*/
			NoOfenteries = PadLeft(NoOfenteries, '0', 8);
			strcpy(NoOfenteries, ConvertToUpper(NoOfenteries));
			strcpy(Buffer4, NoOfenteries);
			strcat(Buffer4, "\n");
			strcpy(Buffer2, Buffer4);
			delete[] NoOfenteries;

			////workaround for B&R Bus Controller stack
			//FormatCdc(objIndexCollection, Buffer4, fileptr, CN);
			BRSpecificFormatCdc(objIndexCollection, Buffer4, fileptr, CN,
					objNode.GetNodeId());
			strcat(Buffer2, Buffer4);
			delete[] Buffer4;

			//Convert CN NodeID to Hex
			IntToAscii(objNode.GetNodeId(), c, 16);
			char* CNDataLength = new char[50];
			strcpy(CNDataLength, "1F22\t");
			c = PadLeft(c, '0', 2);

			///write CN-n NodeID  in the next to 1F22
			strcat(CNDataLength, c);
			strcat(CNDataLength, "\t");

			//write the size of CN-n Buffer
			INT32 len = GetCNDataLen(Buffer2);

			//Convert length to Hex
			IntToAscii(len, c, 16);
			//printf("c%s",c);

			c = PadLeft(c, '0', 8);
			strcat(CNDataLength, c);

			// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
			Buffer3 = new char[strlen(Buffer2) + 50];
			strcpy(Buffer3, CNDataLength);
			strcat(Buffer3, "\n");
			strcat(Buffer3, Buffer2);
			delete[] CNDataLength;
			delete[] Buffer2;

			//write all CNs data in the file
			len = strlen(Buffer3);
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}
			if (0 != (fwrite(Buffer3, sizeof(char), len, fileptr)))
			{
				fclose(fileptr);
			}
			delete[] Buffer3;
		}

	}
	delete[] c;
}

/*****************************************************************************************/
/**
 \brief		GetNodeTotalIndexSubIndex

 This function returns the total no.of index and subindex that can be written to the CDC

 \param		iNodeId		Integer variable to hold the node id to which the total values to be fetched

 \return	INT32
 */
/******************************************************************************************/

INT32 GetNodeTotalIndexSubIndex(INT32 iNodeId)
{
	NodeCollection *objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode;
	INT32 iNumberOfEntries = 0;
	IndexCollection *pobjIdxCol = NULL;

	if (MN_NODEID == iNodeId)
	{
		/* Number of 1F81 enteries is twice the number of CN's*/
		iNumberOfEntries = 2 * objNodeCol->GetCNNodesCount();
		/* include the number of CN's*/
		iNumberOfEntries = iNumberOfEntries + objNodeCol->GetCNNodesCount();
	}

	objNode = objNodeCol->GetNode(iNodeId);

	pobjIdxCol = objNode.GetIndexCollection();

	for (INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->GetNumberofIndexes();
			iLoopCount++)
	{

		Index *pobjIndex = NULL;
		pobjIndex = pobjIdxCol->GetIndex(iLoopCount);

		if (pobjIndex->GetFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) pobjIndex->GetAccessType())
						|| CheckIfMappingPDO((char*) pobjIndex->GetIndexValue())))
		{
			if (pobjIndex->GetNumberofSubIndexes() == 0)
			{
				if (pobjIndex->GetActualValue() != NULL
						&& true == IsDefaultActualNotEqual(pobjIndex))
				{
					iNumberOfEntries = iNumberOfEntries + 1;
				}
			}
			else
			{
				if (CheckIfMappingPDO((char*) pobjIndex->GetIndexValue()))
				{
					SubIndex* pobjSubIndex = NULL;
					pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(
							(char*) "00");

					if ((NULL != pobjSubIndex)
							&& (NULL != pobjSubIndex->GetActualValue())
							&& (0 != strcmp(pobjSubIndex->GetActualValue(), "")))
					{
						if (true == ReactivateMappingPDO(pobjIdxCol, pobjIndex)
								|| true
										== IsDefaultActualNotEqual(
												pobjSubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1; /* to initalize 00 entry subindex */
						}
						if (CheckIfValueZero(
								(char*) pobjSubIndex->GetActualValue()))
						{
							continue;
						}
						if (true == ReactivateMappingPDO(pobjIdxCol, pobjIndex)
								|| true
										== IsDefaultActualNotEqual(
												pobjSubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
						}
						for (INT32 iLoopCount = 0;
								iLoopCount < pobjIndex->GetNumberofSubIndexes();
								iLoopCount++)
						{
							if (pobjIndex->GetSubIndex(iLoopCount)->GetActualValue()
									!= NULL
									&& TRUE
											== pobjIndex->GetSubIndex(
													iLoopCount)->GetFlagIfIncludedCdc()
									&& true
											== IsDefaultActualNotEqual(
													pobjIndex->GetSubIndex(
															iLoopCount)))
							{
								if (0
										== strcmp(
												(char*) pobjIndex->GetSubIndex(
														iLoopCount)->GetIndexValue(),
												"00"))
								{
									continue;
								}
								if (0
										== GetDecimalValue(
												(char*) pobjIndex->GetSubIndex(
														iLoopCount)->GetActualValue()))
								{
									if (NULL
											== pobjIndex->GetSubIndex(
													iLoopCount)->GetDefaultValue()
											|| 0
													== GetDecimalValue(
															(char*) pobjIndex->GetSubIndex(
																	iLoopCount)->GetDefaultValue()))
									{
										continue;
									}
								}
								iNumberOfEntries = iNumberOfEntries + 1;

							}
						}
					}
					continue;
				}

				if (MN_NODEID == iNodeId)
				{
					if (0 == strcmp((char*) pobjIndex->GetIndexValue(), "1F81"))
					{
						SubIndex* pobj1F81SubIndex;
						pobj1F81SubIndex = pobjIndex->GetSubIndexbyIndexValue(
								(char*) "F0");
						if (NULL != pobj1F81SubIndex
								&& pobj1F81SubIndex->GetActualValue() != NULL
								&& 0
										!= strcmp(
												(char*) pobj1F81SubIndex->GetActualValue(),
												"")
								&& true
										== CheckAccessTypeForInclude(
												(char*) pobj1F81SubIndex->GetAccessType())
								&& true
										== IsDefaultActualNotEqual(
												pobj1F81SubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1;
						}
						continue;

					}
				}

				SubIndex* pobjNoOfEntriesSubIndex = NULL;
				pobjNoOfEntriesSubIndex = pobjIndex->GetSubIndexbyIndexValue(
						(char*) "00");
				if ((NULL != pobjNoOfEntriesSubIndex)
						&& (NULL != pobjNoOfEntriesSubIndex->GetActualValue())
						&& (0
								!= strcmp(
										pobjNoOfEntriesSubIndex->GetActualValue(),
										"")))
				{
					if (CheckIfValueZero(
							(char*) pobjNoOfEntriesSubIndex->GetActualValue()))
						continue;
				}

				for (INT32 iLoopCount = 0;
						iLoopCount < pobjIndex->GetNumberofSubIndexes();
						iLoopCount++)
				{
					if (pobjIndex->GetSubIndex(iLoopCount)->GetActualValue()
							!= NULL
							&& TRUE
									== pobjIndex->GetSubIndex(iLoopCount)->GetFlagIfIncludedCdc()
							&& true
									== CheckAccessTypeForInclude(
											(char*) pobjIndex->GetSubIndex(
													iLoopCount)->GetAccessType())
							&& true
									== IsDefaultActualNotEqual(
											pobjIndex->GetSubIndex(iLoopCount)))
					{
						iNumberOfEntries = iNumberOfEntries + 1;
					}
				}
			}

		}
	}
	return iNumberOfEntries;

}

/*****************************************************************************************/
/**
 \brief		BRSpecificgetCNsTotalIndexSubIndex

 This function returns the total no.of index and subindex(recommended by BnR) that can be written to the CDC

 \param		iNodeID

 \return	INT32
 */
/******************************************************************************************/

INT32 BRSpecificgetCNsTotalIndexSubIndex(INT32 iNodeID)
{
	NodeCollection* objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode;
	INT32 iNumberOfEntries = 0;
	IndexCollection* pobjIdxCol = NULL;
	char var1F81nodeid[10];
	IntToAscii(iNodeID, var1F81nodeid, 16);
	PadLeft(var1F81nodeid, '0', 2);

	objNode = objNodeCol->GetNode(iNodeID);
	pobjIdxCol = objNode.GetIndexCollection();
	for (INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->GetNumberofIndexes();
			iLoopCount++)
	{

		Index* pobjIndex = NULL;
		pobjIndex = pobjIdxCol->GetIndex(iLoopCount);

		if (pobjIndex->GetFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) pobjIndex->GetAccessType())
						|| CheckIfMappingPDO((char*) pobjIndex->GetIndexValue())))
		{
			{
				if (pobjIndex->GetNumberofSubIndexes() == 0)
				{
					if (pobjIndex->GetActualValue() != NULL
							&& true == IsDefaultActualNotEqual(pobjIndex))
					{
						iNumberOfEntries = iNumberOfEntries + 1;
					}
				}
				else
				{
					if (CheckIfMappingPDO((char*) pobjIndex->GetIndexValue()))
					{
						SubIndex* pobjSubIndex = NULL;
						pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(
								(char*) "00");

						//actual value checked for non-Zero
						if ((NULL != pobjSubIndex)
								&& (NULL != pobjSubIndex->GetActualValue())
								&& (0
										!= strcmp(
												pobjSubIndex->GetActualValue(),
												""))
								&& !(CheckIfValueZero(
										(char*) pobjSubIndex->GetActualValue())))
						{
							if (true
									== ReactivateMappingPDO(pobjIdxCol,
											pobjIndex)
									|| true
											== IsDefaultActualNotEqual(
													pobjSubIndex))
							{
								iNumberOfEntries = iNumberOfEntries + 1; /* to initalize 00 entry subindex */
							}

							if (true
									== ReactivateMappingPDO(pobjIdxCol,
											pobjIndex)
									|| true
											== IsDefaultActualNotEqual(
													pobjSubIndex))
							{
								iNumberOfEntries = iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
							}

							INT32 isiTotal = pobjIndex->GetNumberofSubIndexes();

							for (INT32 iLoopCount = 0; iLoopCount < isiTotal;
									iLoopCount++)
							{

								if (pobjIndex->GetSubIndex(iLoopCount)->GetActualValue()
										!= NULL
										&& TRUE
												== pobjIndex->GetSubIndex(
														iLoopCount)->GetFlagIfIncludedCdc()
										&& true
												== IsDefaultActualNotEqual(
														pobjIndex->GetSubIndex(
																iLoopCount)))
								{
									if (0
											== strcmp(
													(char*) pobjIndex->GetSubIndex(
															iLoopCount)->GetIndexValue(),
													"00"))
									{
										//the no.of subindex is determied by the 00'th entry actual value. 1 is added for 00'th entry count
										isiTotal =
												(GetDecimalValue(
														(char*) pobjSubIndex->GetActualValue())
														+ 1);
										continue;
									}
									if (0
											== GetDecimalValue(
													(char*) pobjIndex->GetSubIndex(
															iLoopCount)->GetActualValue()))
									{
										if (NULL
												== pobjIndex->GetSubIndex(
														iLoopCount)->GetDefaultValue()
												|| 0
														== GetDecimalValue(
																(char*) pobjIndex->GetSubIndex(
																		iLoopCount)->GetDefaultValue()))
										{
											continue;
										}
									}
									iNumberOfEntries = iNumberOfEntries + 1;

								}
							}
						}
						continue;
					}

					SubIndex* pobjSubIndex = NULL;
					pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(
							(char*) "00");
					if ((NULL != pobjSubIndex)
							&& (NULL != pobjSubIndex->GetActualValue())
							&& (0 != strcmp(pobjSubIndex->GetActualValue(), "")))
					{
						if (CheckIfValueZero(
								(char*) pobjSubIndex->GetActualValue()))
							continue;
					}

					for (INT32 iLoopCount = 0;
							iLoopCount < pobjIndex->GetNumberofSubIndexes();
							iLoopCount++)
					{
						if (pobjIndex->GetSubIndex(iLoopCount)->GetActualValue()
								!= NULL
								&& TRUE
										== pobjIndex->GetSubIndex(iLoopCount)->GetFlagIfIncludedCdc()
								&& true
										== CheckAccessTypeForInclude(
												(char*) pobjIndex->GetSubIndex(
														iLoopCount)->GetAccessType())
								&& true
										== IsDefaultActualNotEqual(
												pobjIndex->GetSubIndex(
														iLoopCount)))
						{
							if (0
									== strcmp(pobjIndex->GetIndexValue(),
											(char*) "1F81"))
							{

								if (0
										== strcmp(
												pobjIndex->GetSubIndex(
														iLoopCount)->GetIndexValue(),
												"00")
										|| 0
												== strcmp(
														pobjIndex->GetSubIndex(
																iLoopCount)->GetIndexValue(),
														var1F81nodeid))
								{
									continue;
								}
								else
								{
								}
							}
							iNumberOfEntries = iNumberOfEntries + 1;
						}
					}

				}
			}
		}
	}
	return iNumberOfEntries;
}

/*****************************************************************************************/
/**
 \brief		GenerateCDC

 This API shall be used to generate the CDC for the project. This API writes the compiled CDC into the mnobd.txt file. The generated mnobd.txt is converted into binary [mnobd.cdc] file using txt2cdc [elf].

 \param		CDCLocation		Char pointer to hold the path of the location to which the generated cdc contents will be written into.

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateCDC(char* CDCLocation)
{

	Node objMnNode;
	Node *pobjNode = NULL;
	IndexCollection* objIndexCollection;
	//ofstream file;
	char *Buffer1 = NULL;
	char *tempFileName = NULL;
	char *tempOutputFileName = NULL;
	UINT32 len;
	ocfmRetCode retCode;
	ocfmRetCode ret1F81Code;
	ocfmRetCode retCode_temp;
	ocfmException ex;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;

	tempFileName = new char[strlen(CDCLocation) + 10 + strlen("mnobd")];
	sprintf(tempFileName, "%s/%s.txt", CDCLocation, "mnobd");

	tempOutputFileName = new char[1000];
	sprintf(tempOutputFileName, "%s/%s.cdc", CDCLocation, "mnobd");
	retCode.code = OCFM_ERR_UNKNOWN;
	retCode_temp.code = OCFM_ERR_UNKNOWN;
	ret1F81Code.code = OCFM_ERR_UNKNOWN;
	retCode.errorString = NULL;
	retCode_temp.errorString = NULL;
	ret1F81Code.errorString = NULL;

	try
	{
		//get the MN Node object from the NodeCollection
		NodeCollection* objNodeCollection;
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();

		/******************************* Write MN's Indexes ******************************************************************/

		objMnNode = objNodeCollection->GetMNNode();
		if (objNodeCollection->GetNumberOfNodes() == 0)
		{
			ex.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw ex;
		}
		if (objMnNode.IsNull())
		{
			ex.OCFMException(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw ex;
		}
		if (objNodeCollection->GetCNNodesCount() == 0)
		{
			ex.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw ex;
		}

		/* Check if autogeneration of MN's Object Dicitonary is set to true */

		PjtSettings* stPjtSettings;
		stPjtSettings = PjtSettings::GetPjtSettingsPtr();

		if (stPjtSettings->GetGenerateAttr() == YES_AG)
		{
			retCode = CheckMutliplexAssigned();
			if (retCode.code != OCFM_ERR_SUCCESS)
			{
				return retCode;
			}
			/*Generate MNOBD for Auto Generate On*/
			retCode = GenerateMNOBD(true);
			if (OCFM_ERR_EXCESS_CHANNEL == retCode.code)
			{
				//Do not throw exception here as we need the process to complete
				retCode_temp = retCode;
			}
			else if (OCFM_ERR_SUCCESS != retCode.code)
			{
				return retCode;
			}
			else
			{
			}
		}
		else
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < objNodeCollection->GetNumberOfNodes();
					iLoopCount++)
			{
				pobjNode = objNodeCollection->GetNodebyColIndex(iLoopCount);
				/* Process PDO Objects for CN*/
				if (NULL == pobjNode)
				{
#if defined DEBUG
					cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

					ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw ex;
				}

				if (pobjNode->GetNodeType() == MN)
				{
					objIndexCollection = pobjNode->GetIndexCollection();
					if (NULL == objIndexCollection)
					{
#if defined DEBUG
						cout << "Memory allocation error" << __FUNCTION__
						<< endl;
#endif

						ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
						throw ex;
					}
				}
				else
				{
					if (!(pobjNode->HasPdoObjects()))
					{

						continue;
					}
				}
			}
		}

		FILE* fileptr = new FILE();
		if ((fileptr = fopen(tempFileName, "w+")) == NULL)
		{
			ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw ex;
		}

		for (INT32 i = 0; i < objNodeCollection->GetNumberOfNodes(); i++)
		{
			Node objCnNode;
			objCnNode = objNodeCollection->GetNodebyCollectionIndex(i);
			if (objCnNode.GetNodeType() == CN)
			{
				INT32 NodeID = objCnNode.GetNodeId();
				char* hex = new char[10];
				hex = IntToAscii(NodeID, hex, 16);
				hex = PadLeft(hex, '0', 2);

				char* varTemp1F81Data = SetNodeAssigmentBits(&objCnNode);
				char* var1F81Data = new char[strlen(varTemp1F81Data)
						+ ALLOC_BUFFER + 2];
				sprintf(var1F81Data, "0X%s", varTemp1F81Data);
				ret1F81Code = IfSubIndexExists(MN_NODEID, MN, (char*) "1F81",
						hex, &iSubIndexPos, &iIndexPos);
				if (OCFM_ERR_SUCCESS != ret1F81Code.code)
				{
					continue;
				}

				SetSIdxValue((char*) "1F81", hex, var1F81Data,
						objNodeCollection->GetMNNode().GetIndexCollection(),
						MN_NODEID, MN, false);
				delete[] hex;
				delete[] var1F81Data;
			}
		}

		objMnNode = objNodeCollection->GetMNNode();
		INT32 iCNsCount = 0;
		iCNsCount = objNodeCollection->GetCNNodesCount();
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F81",
				true);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F92",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F8D",
				true);
		//1c07,1c08,1f22,1f84,1f8e,1f8f to be added
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F8B",
				true);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F26",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F27",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1C09",
				true);
		if (YES_AG == stPjtSettings->GetGenerateAttr())
		{
			CalculatePayload();
		}

		//Buffer1 = (char*)malloc(CDC_BUFFER);
		Buffer1 = new char[CDC_BUFFER];
		char* NoOfenteries = new char[10];
		NoOfenteries = IntToAscii(GetNodeTotalIndexSubIndex(MN_NODEID),
				NoOfenteries, 16);
		NoOfenteries = PadLeft(NoOfenteries, '0', 8);
		strcpy(Buffer1, NoOfenteries);
		strcat(Buffer1, "\n");
		len = strlen(Buffer1);
		delete[] NoOfenteries;

		/* Write number of enteries */
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
#ifdef DEBUG
			cout << "Write Error in CDC" << endl;
#endif
		}
		delete[] Buffer1;

		// Add 1F81
		for (INT32 i = 0; i < objNodeCollection->GetNumberOfNodes(); i++)
		{
			Node objCnNode;
			objCnNode = objNodeCollection->GetNodebyCollectionIndex(i);
			if (objCnNode.GetNodeType() == CN)
			{

				Buffer1 = new char[CDC_BUFFER];
				strcpy(Buffer1, "//// NodeId Assignment\n");
				strcat(Buffer1, "1F81");
				strcat(Buffer1, "\t");
				INT32 NodeID = objCnNode.GetNodeId();
				char* hex = new char[10];
				hex = IntToAscii(NodeID, hex, 16);
				hex = PadLeft(hex, '0', 2);
				strcat(Buffer1, hex);
				strcat(Buffer1, "\t00000004\t00000007\n");
				strcat(Buffer1, "\n");
				len = strlen(Buffer1);
				// write 1F81 entry in MN text file
				if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
				{
#ifdef DEBUG
					cout << "Write Error in CDC _1" << endl;
#endif
				}
				delete[] hex;
				delete[] Buffer1;
			}
		}

		fclose(fileptr);

		if ((fileptr = fopen(tempFileName, "a+")) == NULL)
		{
			ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw ex;
		}
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objMnNode = objNodeCollection->GetMNNode();
		objIndexCollection = objMnNode.GetIndexCollection();

		//Get all the MN's Default Data in Buffer1
		Buffer1 = new char[CDC_MN_BUFFER];
		memset(Buffer1, 0, CDC_MN_BUFFER * sizeof(char));
		FormatCdc(objIndexCollection, Buffer1, fileptr, MN);

		len = strlen(Buffer1);
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
#ifdef DEBUG
			cout << "Write Error in CDC _2" << endl;
#endif
		}
		delete[] Buffer1;
		fclose(fileptr);

		/*************************Write CN's Data in Buffer2***************************************************/
		WriteCNsData((char*) tempFileName);
		//INT32 ret;

		//Get all the IF81 ENTRY in Buffer1
		if (objNodeCollection->GetNumberOfNodes() != 0)
		{

			for (INT32 i = 0; i < objNodeCollection->GetNumberOfNodes(); i++)
			{
				Node objCnNode;
				objCnNode = objNodeCollection->GetNodebyCollectionIndex(i);
				if (objCnNode.GetNodeType() == CN)
				{
					if ((fileptr = fopen(tempFileName, "a+")) == NULL)
					{
						ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
						throw ex;
					}
					Buffer1 = new char[CDC_BUFFER];
					len = strlen(Buffer1);
					strcpy(Buffer1, "//// NodeId Reassignment\n");
					strcat(Buffer1, "1F81");
					strcat(Buffer1, "\t");
					INT32 NodeID = objCnNode.GetNodeId();
					char* hex = new char[10];
					hex = IntToAscii(NodeID, hex, 16);
					hex = PadLeft(hex, '0', 2);
					strcat(Buffer1, hex);

					strcat(Buffer1, "\t00000004\t");
					char* varTemp1F81Data = SetNodeAssigmentBits(&objCnNode);
					strcat(Buffer1, varTemp1F81Data);

					strcat(Buffer1, "\n");

					len = strlen(Buffer1);
					if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
					{

						//TODO: Need to specify warning or err if fwrite fails.
						fclose(fileptr);

					}
					else
					{
						fclose(fileptr);
					}
					delete[] hex;
					delete[] Buffer1;
				}
			}

		}

		// Convert CDC txt file to Binary
		INT32 iReturnFromtxt2cdc;
		char* cmdBuffer;
#if defined(_WIN32) && defined(_MSC_VER)
		cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25]; //TODO: Calculate size dynamically
		sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", tempFileName,
				tempOutputFileName);
		iReturnFromtxt2cdc = system(cmdBuffer);
#else
		cmdBuffer = new char[LINUX_INSTALL_DIR_LEN + (2 * (strlen(CDCLocation) + 10 + 10)) + 25];
		sprintf(cmdBuffer, "%s/txt2cdc \"%s\" \"%s\"", LINUX_INSTALL_DIR, tempFileName, tempOutputFileName);
		iReturnFromtxt2cdc = system(cmdBuffer);

#endif
		delete[] cmdBuffer;

		if (OCFM_ERR_SUCCESS == iReturnFromtxt2cdc)
		{
			retCode.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ex.OCFMException(OCFM_ERR_INVALID_TXT_FOR_CDC);
			throw ex;
		}
		if (OCFM_ERR_EXCESS_CHANNEL == retCode_temp.code)
		{
			ex.OCFMException(retCode_temp.code);
			throw ex;
		}
		else
		{
			//continue
		}
		delete[] tempFileName;
		delete[] tempOutputFileName;
	} catch (ocfmException & ex)
	{
		delete[] tempFileName;
		delete[] tempOutputFileName;
		return ex._ocfmRetCode;
	}
	return retCode;
}

/*****************************************************************************************/
/**
 \brief		FormatCdc

 This function writes the buffer with the index data in the index collection that can be written in the CDC

 \param		objIndexCollection		Class pointer of IndexCollection for node
 \param		Buffer1					Character pointer to hold the buffer data
 \param		fileptr					
 \param		eNodeType				Enumeration flag for Node type (CN/MN)

 \return	void
 */
/******************************************************************************************/

//TODO: only buffer is used not the fileptr. should be removed in header & releated.
void FormatCdc(IndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, NodeType eNodeType)
{
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char *TempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 NumberOfIndexes = objIndexCollection->GetNumberofIndexes();
	//disable mapping pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);
		if (objIndex->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
			{
				TempBuffer1 = new char[CDC_BUFFER];
				//len = strlen(Buffer1);      
				//GetIndexData(objIndex,Buffer1);
				EnableDisableMappingPDO(objIndexCollection, objIndex,
						TempBuffer1, false);
				strcat(Buffer1, TempBuffer1);
				delete[] TempBuffer1;
			}
		}
	}
	// write all objects except pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);

		if (objIndex->GetFlagIfIncludedCdc() == TRUE
				&& true
						== CheckAccessTypeForInclude(
								(char*) objIndex->GetAccessType())
				&& CheckIfNotPDO((char*) objIndex->GetIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(objIndex->GetIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				TempBuffer1 = new char[15000];
				GetIndexData(objIndex, TempBuffer1);
				strcat(Buffer1, TempBuffer1);

				delete[] TempBuffer1;
			}
			else
			{
				SubIndex* objSubIndex = objIndex->GetSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != objSubIndex
						&& TRUE == objSubIndex->GetFlagIfIncludedCdc()
						&& true
								== CheckAccessTypeForInclude(
										(char*) objSubIndex->GetAccessType())
						&& NULL != objSubIndex->GetActualValue()
						&& 0
								!= strcmp((char*) objSubIndex->GetActualValue(),
										"")
						&& true == IsDefaultActualNotEqual(objSubIndex))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					strcpy(TempBuffer1, "1F81");
					strcat(TempBuffer1, "\t");
					strcat(TempBuffer1, "F0");
					strcat(TempBuffer1, "\t00000004\t");

					char actvalue[20];
					actvalue[0] = '\0';
					if (CheckIfHex((char*) objSubIndex->GetActualValue()))
					{
						INT32 len = strlen(
								(char*) objSubIndex->GetActualValue());
						strncpy(actvalue, (objSubIndex->GetActualValue() + 2),
								len - 2);
						actvalue[len - 2] = '\0';
						strcat(TempBuffer1, PadLeft(actvalue, '0', 8));
					}
					else
					{
						strcpy(actvalue,
								IntToAscii(atoi(objSubIndex->GetActualValue()),
										actvalue, 16));
						strcat(TempBuffer1, PadLeft(actvalue, '0', 8));
					}

					strcat(TempBuffer1, "\n");
					// len = strlen(TempBuffer1);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);
		if (objIndex->GetFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) objIndex->GetAccessType())
						|| CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
				&& !CheckIfNotPDO((char*) objIndex->GetIndexValue()))
		{
			TempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(objIndex, TempBuffer1);
			strcat(Buffer1, TempBuffer1);

			delete[] TempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);
		if (objIndex->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
			{
				TempBuffer1 = new char[CDC_BUFFER];
				EnableDisableMappingPDO(objIndexCollection, objIndex,
						TempBuffer1, true);
				strcat(Buffer1, TempBuffer1);
				delete[] TempBuffer1;
			}
		}
	}
}

/*****************************************************************************************/
/**
 \brief		BRSpecificFormatCdc

 This function writes the buffer with the index data in the index collection that can be written in the CDC

 \param		objIndexCollection		Class pointer of IndexCollection for node
 \param		Buffer1					Character pointer to hold the buffer data
 \param		fileptr
 \param		eNodeType				Enumeration flag for Node type (CN/MN)
 \param		iNodeId					Integer variable to hold the Node id to be written to the 1F81 index

 \return	void
 */
/******************************************************************************************/

//TODO: only buffer is used not the fileptr. should be removed in header & releated.
void BRSpecificFormatCdc(IndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, NodeType eNodeType, INT32 iNodeId)
{
	ocfmException objException;
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	char *TempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 NumberOfIndexes = objIndexCollection->GetNumberofIndexes();
	//disable mapping pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex = NULL;
		objIndex = objIndexCollection->GetIndex(i);
		if (NULL == objIndex)
		{
#if defined DEBUG
			cout << "Memory allocation error 1" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			objException.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw objException;
		}
		if (objIndex->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being disabled.
				SubIndex* pobjSubIndex = NULL;
				pobjSubIndex = objIndex->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == pobjSubIndex)
				{
#if defined DEBUG
					cout << "Memory allocation error2" << __FUNCTION__
					<< __LINE__ << "Index" << objIndex->GetIndexValue()
					<< endl;
#endif

					objException.OCFMException(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw objException;
				}
				if (!CheckIfValueZero((char*) pobjSubIndex->GetActualValue()))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					//len = strlen(Buffer1);      
					EnableDisableMappingPDO(objIndexCollection, objIndex,
							TempBuffer1, false);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	// write all objects except pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);
		if (objIndex->GetFlagIfIncludedCdc() == TRUE
				&& true
						== CheckAccessTypeForInclude(
								(char*) objIndex->GetAccessType())
				&& CheckIfNotPDO((char*) objIndex->GetIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(objIndex->GetIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				TempBuffer1 = new char[15000];
				//commented the GetIndexData fn and BRSpecificGetIndexData is used
				//GetIndexData(objIndex,TempBuffer1);
				BRSpecificGetIndexData(objIndex, TempBuffer1, iNodeId);
				strcat(Buffer1, TempBuffer1);

				delete[] TempBuffer1;
			}
			else
			{
				SubIndex* objSubIndex = objIndex->GetSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != objSubIndex
						&& TRUE == objSubIndex->GetFlagIfIncludedCdc()
						&& true
								== CheckAccessTypeForInclude(
										(char*) objSubIndex->GetAccessType())
						&& NULL != objSubIndex->GetActualValue()
						&& 0
								!= strcmp((char*) objSubIndex->GetActualValue(),
										"")
						&& true == IsDefaultActualNotEqual(objSubIndex))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					strcpy(TempBuffer1, "1F81");
					strcat(TempBuffer1, "\t");
					strcat(TempBuffer1, "F0");
					strcat(TempBuffer1, "\t00000004\t");

					char actvalue[20];
					actvalue[0] = '\0';
					if (CheckIfHex((char*) objSubIndex->GetActualValue()))
					{
						INT32 len = strlen(
								(char*) objSubIndex->GetActualValue());
						strncpy(actvalue, (objSubIndex->GetActualValue() + 2),
								len - 2);
						actvalue[len - 2] = '\0';
						strcat(TempBuffer1, PadLeft(actvalue, '0', 8));
					}
					else
					{
						strcpy(actvalue,
								IntToAscii(atoi(objSubIndex->GetActualValue()),
										actvalue, 16));
						strcat(TempBuffer1, PadLeft(actvalue, '0', 8));
					}

					strcat(TempBuffer1, "\n");
					//  len = strlen(TempBuffer1);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index *objIndex = NULL;
		objIndex = objIndexCollection->GetIndex(i);
		if (objIndex->GetFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) objIndex->GetAccessType())
						|| CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
				&& !CheckIfNotPDO((char*) objIndex->GetIndexValue()))
		{
			TempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(objIndex, TempBuffer1);
			strcat(Buffer1, TempBuffer1);

			delete[] TempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		Index* objIndex;
		objIndex = objIndexCollection->GetIndex(i);
		if (NULL == objIndex)
		{
#if defined DEBUG
			cout << "Memory allocation error3" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			objException.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw objException;
		}
		if (objIndex->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being enabled.
				SubIndex *pobjSubIndex = NULL;
				pobjSubIndex = objIndex->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == pobjSubIndex)
				{
#if defined DEBUG
					cout << "Memory allocation error4" << __FUNCTION__
					<< __LINE__ << endl;
#endif

					objException.OCFMException(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw objException;
				}
				if (!CheckIfValueZero((char*) pobjSubIndex->GetActualValue()))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					EnableDisableMappingPDO(objIndexCollection, objIndex,
							TempBuffer1, true);
					strcat(Buffer1, TempBuffer1);

					delete[] TempBuffer1;
				}
			}
		}
	}
}

/*****************************************************************************************/
/**
 \brief		ProcessCDT

 This function process the Complex datatype collection and generates the process image for the node

 \param		pobjCDT				Class pointer of ComplexDataType for node
 \param		pobjAppProc			Class pointer of ApplicationProcess for node
 \param		pobjNode			Class pointer of Node for node
 \param		pobjParameter		Struct pointer of Parameter for node
 \param		varPdoType			Enumeration flag for PDO type
 \param		varModuleName		Character pointer to hold the Module name
 \param		varModuleIndex		Character pointer to hold the Module Index value

 \return	INT32				Total bytes mapped for that module
 */
/******************************************************************************************/

INT32 ProcessCDT(ComplexDataType* pobjCDT, ApplicationProcess* pobjAppProc,
		Node* pobjNode, Parameter* pobjParameter, PDOType varPdoType,
		char* varModuleName, char* varModuleIndex)
{
	ocfmException objocfmException;
	if (pobjCDT == NULL)
	{
		objocfmException.OCFMException(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
		throw objocfmException;
	}
	if ((NULL == pobjAppProc) || (NULL == pobjNode) || (NULL == pobjParameter)
			|| (NULL == varModuleName) || (NULL == varModuleIndex))
	{
		objocfmException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objocfmException;
	}

	//TODO: No header definitions is for ProcessCDT
	INT32 iStartBitOffset = 0;
	INT32 iOffset = 0;
	bool bIsNewBitStringVar = false;
	INT32 iDataSize = 0;
	INT32 iTotalBytesMapped = 0;
	#if defined DEBUG
	cout<<"iStartBitOffset: "<<iStartBitOffset<<"iOffset"<<iOffset<<"iDataSize"<<iDataSize<<"iTotalBytesMapped"<<iTotalBytesMapped<<endl;
	#endif
	for (INT32 iLoopCount = 0; iLoopCount < pobjCDT->varCollection.Count();
			iLoopCount++)
	{

		varDeclaration objVarDecl;

		objVarDecl.Initialize();
		objVarDecl = pobjCDT->varCollection[iLoopCount];
		if (objVarDecl.namIdDtAttr->GetDtUniqueRefId() != NULL)
		{
			pobjCDT = pobjAppProc->GetCDTbyUniqueID(
					objVarDecl.namIdDtAttr->GetDtUniqueRefId());
			pobjAppProc->UpdatePreviousCDTUId(objVarDecl.structUniqueId,
					pobjCDT->Index);
			iLastVarIndex = iLoopCount;
			#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 starts"<<endl;
			#endif
			ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
					varPdoType, varModuleName, varModuleIndex);
			#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 End"<<endl;
			#endif
		}
		if (!bCDTCompleted)
		{
			// add rest of the contents
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage.BitOffset = -1;
			objProcessImage.ByteOffset = 0;
			if (pobjParameter->Access != NULL)
				strcpy(objProcessImage.Direction,
						GetParameterAccess(pobjParameter->Access));

			if (varPdoType == PDO_TPDO)
			{
				objProcessImage.DirectionType = INPUT;
			}
			else if (varPdoType == PDO_RPDO)
			{
				objProcessImage.DirectionType = OUTPUT;
			}
			else
			{
				//TODO: "else" Added. Nothing to do
			}

			if (objVarDecl.Size != NULL)
			{
				objProcessImage.DataInfo.DataSize = atoi(objVarDecl.Size);
			}
			else
			{
			}
			if (objVarDecl.namIdDtAttr->GetName() != NULL)
			{
				objProcessImage.Name =
						(char*) malloc(
								strlen(
										objVarDecl.namIdDtAttr->GetName()) + strlen(varModuleName) + 6 + ALLOC_BUFFER);
				strcpy(objProcessImage.Name, GetPIName(pobjNode->GetNodeId()));
				strcat(objProcessImage.Name, varModuleName);
				strcat(objProcessImage.Name, ".");
				strcat(objProcessImage.Name,
						objVarDecl.namIdDtAttr->GetName());

				objProcessImage.ModuleName = new char[strlen(varModuleName)
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.ModuleName, varModuleName);

				objProcessImage.ModuleIndex = new char[strlen(varModuleIndex)
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.ModuleIndex, varModuleIndex);

				objProcessImage.VarName =
						new char[strlen(objVarDecl.namIdDtAttr->GetName())
								+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.VarName,
						objVarDecl.namIdDtAttr->GetName());
				//TODO: delete new char (memory Issue)
			}

			objProcessImage.CNNodeID = pobjNode->GetNodeId();

			if (objVarDecl.namIdDtAttr->GetDataType() != NULL)
			{
				objProcessImage.DataInfo._dt_Name = new char[strlen(
						objVarDecl.namIdDtAttr->GetDataType())
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.DataInfo._dt_Name,
						(const char*) objVarDecl.namIdDtAttr->GetDataType());
			}

			/* Set the IEC DT*/
			objProcessImage.DataInfo = *(GetIECDT(
					objVarDecl.namIdDtAttr->GetDataType(),
					objProcessImage.DataInfo.DataSize));

			/* Calculate Offset*/
			if (((objProcessImage.DataInfo._dt_enum != BITSTRING)
					&& (objProcessImage.DataInfo.DataSize >= 8))
					|| ((objProcessImage.DataInfo._dt_enum == BITSTRING)
							&& (iStartBitOffset == 0 || iStartBitOffset == 8
									|| iStartBitOffset == 16
									|| iStartBitOffset == 32
									|| iStartBitOffset == 64)))
			{

				bIsNewBitStringVar = true;
				if (objProcessImage.DataInfo._dt_enum == BITSTRING)
				{
					iStartBitOffset = 0;
					iDataSize = 0;
					for (INT32 iBitStringCount = iLoopCount;
							iBitStringCount < pobjCDT->varCollection.Count();
							iBitStringCount++)
					{
						varDeclaration objVarDeclBitStr;

						objVarDeclBitStr.Initialize();
						objVarDeclBitStr =
								pobjCDT->varCollection[iBitStringCount];
						if (BITSTRING
								== ((*(GetIECDT(
										objVarDeclBitStr.namIdDtAttr->GetDataType(),
										objProcessImage.DataInfo.DataSize)))._dt_enum))
						{
							iDataSize += atoi(objVarDeclBitStr.Size);
							if (8 == iDataSize || 16 == iDataSize
									|| 32 == iDataSize || 64 == iDataSize)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					iDataSize = objProcessImage.DataInfo.DataSize;

				}

				if (varPdoType == PDO_RPDO)
				{
					iOffset = ComputeOUTOffset(iDataSize, varPdoType);
				}
				else if (varPdoType == PDO_TPDO)
				{
					iOffset = ComputeINOffset(iDataSize, varPdoType);
				}
				else
				{
					//TODO: "else" Added.Operation need to be specified
				}
			}
			/* Set the Byte Offet*/
			objProcessImage.ByteOffset = iOffset;
			if (bIsNewBitStringVar)
			{
				/* Total bytes Mapped */
				iTotalBytesMapped = iTotalBytesMapped + (iDataSize / 8);
				#if defined DEBUG
				cout<<"iTotalBytesMapped:"<<iTotalBytesMapped<<endl;
				#endif
				if (iTotalBytesMapped > MAX_PI_SIZE)
				{
					ocfmException objex;
					objex.OCFMException(OCFM_ERR_MAX_PI_SIZE);
					throw objex;
				}
				CreateMNPDOVar(iOffset, iDataSize,
						objProcessImage.DataInfo._dt_enum, varPdoType,
						pobjNode);
			}

			if ((objProcessImage.DataInfo.DataSize >= 8)
					&& (iStartBitOffset != 0)
					&& (objProcessImage.DataInfo._dt_enum != BITSTRING))
			{
				iStartBitOffset = 0;
			}
			else if (objProcessImage.DataInfo._dt_enum == BITSTRING)
			{
				objProcessImage.BitOffset = iStartBitOffset;
				iStartBitOffset = iStartBitOffset
						+ objProcessImage.DataInfo.DataSize;
				objProcessImage.ByteOffset = iOffset;
				bIsNewBitStringVar = false;
			}
			else
			{
				//TODO: "else" Added.Operation need to be specified
			}
			pobjNode->AddProcessImage(objProcessImage);


		}

	}

	if (pobjCDT->previousCDT_UId != NULL)
	{
		pobjCDT = pobjAppProc->GetCDTbyUniqueID(pobjCDT->previousCDT_UId);

		for (INT32 iLoopCount = (iLastVarIndex + 1);
				iLoopCount < pobjCDT->varCollection.Count(); iLoopCount++)
		{
			if (!bCDTCompleted)
			{
				varDeclaration objVarDecl;
				objVarDecl = pobjCDT->varCollection[iLoopCount];
				if (objVarDecl.namIdDtAttr->GetDtUniqueRefId() != NULL)
				{
					pobjCDT = pobjAppProc->GetCDTbyUniqueID(
							objVarDecl.namIdDtAttr->GetDtUniqueRefId());
					pobjAppProc->UpdatePreviousCDTUId(
							objVarDecl.structUniqueId, pobjCDT->Index);

					iLastVarIndex = iLoopCount;
					#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 starts"<<endl;
					#endif
					ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
							varPdoType, varModuleName, varModuleIndex);
					#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 End"<<endl;
					#endif
					

				}
			}
		}
	}
	bCDTCompleted = true;
	#if defined DEBUG
	cout<<"iTotalBytesMapped: "<<iTotalBytesMapped<<" iOffset"<<iOffset<<endl;
	#endif
	//Returned current mapped size in bytes
	return iTotalBytesMapped;
}

/*****************************************************************************************/
/**
 \brief		DecodeUniqiueIDRef

 This function Decodes the uniqiueID Ref & process the complex data type collection accordingly

 \param		uniquedIdref	Character pointer to hold the uniquedId reference
 \param		pobjNode		Class pointer of Node for node
 \param		varPdoType		Enumeration flag for PDO type
 \param		varModuleName	Character pointer to hold the Module name
 \param		varModuleIndex	Character pointer to hold the Module Index value

 \return	INT32		Returns total bytes mapped for that module
 */
/******************************************************************************************/

INT32 DecodeUniqiueIDRef(char* uniquedIdref, Node* pobjNode,
		PDOType varPdoType, char* varModuleName, char* varModuleIndex)
{
	if ((NULL == uniquedIdref) || (NULL == pobjNode) || (NULL == varModuleName)
			|| (NULL == varModuleIndex))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ocfmException objocfmException;
	Parameter* pobjParameter = NULL;
	ApplicationProcess* pobjAppProc = NULL;
	ComplexDataType* pobjCDT = NULL;
	INT32 iTotalBytesMapped = 0;
	try
	{
		if (pobjNode->GetApplicationProcess() != NULL)
		{

			pobjAppProc = pobjNode->GetApplicationProcess();
			if (pobjAppProc->ParameterCollection.Count() != 0)
			{

				pobjParameter = pobjAppProc->GetParameterbyUniqueIDRef(
						uniquedIdref);
				if (pobjParameter == NULL)
				{
					objocfmException.OCFMException(
							OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					throw objocfmException;
				}

				// Check if DataTypeUniqueIDref exists
				if (pobjParameter->nameIdDtAttr.dataTypeUniqueIDRef != NULL)
				{

					pobjCDT = pobjAppProc->GetCDTbyUniqueID(
							pobjParameter->nameIdDtAttr.dataTypeUniqueIDRef);
					if (pobjCDT == NULL)
					{
						objocfmException.OCFMException(
								OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
						throw objocfmException;
					}
					iTotalBytesMapped = ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
							varPdoType, varModuleName, varModuleIndex);
					iLastVarIndex = -1;
					bCDTCompleted = false;
				}
				else
				{
#if defined DEBUG
					cout << "Data type of unique id does not exists" << endl;
#endif
				}
			}

		}

	} catch (ocfmException& ex)
	{
		throw ex;
	}
	return iTotalBytesMapped;
}

/*****************************************************************************************/
/**
 \brief		ProcessPDONodes

 This API Processes the PDO nodes of the Controlled node

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode ProcessPDONodes()
{
	return (ProcessPDONodes(false));
}

/*****************************************************************************************/
/**
 \brief		ProcessPDONodes

 This function Processes the PDO Nodes of the controlled node and generates a pdo collection that is used to create the MN pdo's & the process image variables

 \param		IsBuild		Boolean flag to hold the value true if it is Build process

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode ProcessPDONodes(bool IsBuild)
{
	NodeCollection *objNodeCol = NULL;
	ocfmException objocfmException;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node *pobjNode = NULL;
	//CNode *pobjMNNode = NULL;
	INT32 iTotalBytesMapped = 0;
	INT32 iTotalChainedBytesMapped = 0;
	INT32 iNodeMappedTotalBytes = 0;
	INT32 iNodeRPDOMappedNodeID = 0;

	IndexCollection *objPDOCollection = NULL;
	IndexCollection *pobjIndexCollection = NULL;
	/* Check RPDO Mapped objects*/
	INT32 iNodesCount = 0;
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	// bool bChangeOffset = false;

	iNodesCount = objNodeCol->GetCNNodesCount();

#if defined DEBUG
	cout << "Nodes count" << iNodesCount << endl;
#endif

	try
	{
		if (iNodesCount == 0)
		{
			exit(0);
		}

		stSize8INOffset.currOffset = 0;
		stSize8INOffset.prevOffset = 0;
		stSize16INOffset.currOffset = 0;
		stSize16INOffset.prevOffset = 0;
		stSize32INOffset.currOffset = 0;
		stSize32INOffset.prevOffset = 0;
		stSize64INOffset.currOffset = 0;
		stSize64INOffset.prevOffset = 0;

		stSize8OUTOffset.currOffset = 0;
		stSize8OUTOffset.prevOffset = 0;
		stSize16OUTOffset.currOffset = 0;
		stSize16OUTOffset.prevOffset = 0;
		stSize32OUTOffset.currOffset = 0;
		stSize32OUTOffset.prevOffset = 0;
		stSize64OUTOffset.currOffset = 0;
		stSize64OUTOffset.prevOffset = 0;

		INT32 *pArrangedNodeIDbyStation = NULL;
		pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{

			pobjNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			/* Process PDO Objects for CN*/

			if (pobjNode->GetNodeType() == CN)
			{
				if (!(pobjNode->HasPdoObjects()))
				{
					continue;
				}
				StationType eNodeStation = pobjNode->GetStationType();

				/* Empty ProcessImage collection (Axxx objects)*/
				pobjNode->DeleteCollectionsForPI();

				INT32 countTPDO = 0;
				INT32 countRPDO = 0;
				objPDOCollection = pobjNode->getPDOIndexCollection(&countRPDO, &countTPDO);

				if (objPDOCollection == NULL)
				{
					cout << "objPDOCollection NULL" << endl;
					continue;
				}
				//Validate the number of TPDO's for a CN
				if(countTPDO > 1)
				{
					objocfmException.OCFMException(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char acCustomError[200] = { 0 };
					sprintf(acCustomError, "CN Node-Id: %d cannot have more than one TPDO Channel", pobjNode->GetNodeId());
					CopyCustomErrorString(&(objocfmException._ocfmRetCode), acCustomError);
					throw objocfmException;
				}

				pobjIndexCollection = pobjNode->GetIndexCollection();

				pobjNode->SetPReqActPayloadValue(0);
				pobjNode->SetPResActPayloadValue(0);

				for (INT32 iLoopCount = 0;
						iLoopCount < objPDOCollection->GetNumberofIndexes();
						iLoopCount++)
				{

					Index *pobjBforeSortIndex = NULL;
					Index objIndex;
					pobjBforeSortIndex = objPDOCollection->GetIndex(iLoopCount);
					if (!(CheckIfMappingPDO(
							(char*) pobjBforeSortIndex->GetIndexValue())))
					{
						continue;
					}
					else
					{
#if defined DEBUG
						cout << "index:"
						<< (char*) pobjBforeSortIndex->GetIndexValue()
						<< endl;
#endif
					}
					iNodeMappedTotalBytes = 0;
					// bChangeOffset = false;

					if (pobjBforeSortIndex->GetNumberofSubIndexes() > 0)
					{

						/* Sort the pdo collection */
						objIndex = GetPDOIndexByOffset(pobjBforeSortIndex);

						INT32 iSiCount = 1;

						// Initialised to Zero and the value will be taken from the Actual value or the default value in priority
						INT32 iSiTotal = 0;
						//check whether the channel is activated
						SubIndex *pobjNoofEntriesSubIndex = NULL;
						pobjNoofEntriesSubIndex =
								pobjBforeSortIndex->GetSubIndexbyIndexValue(
										(char *) "00");
						if (NULL == pobjNoofEntriesSubIndex)
						{
							continue;
						}
						// Actual value checked for Null, Empty, non-zero
						if ((pobjNoofEntriesSubIndex->GetActualValue() != NULL)
								&& (0
										!= strcmp(
												pobjNoofEntriesSubIndex->GetActualValue(),
												""))
								&& !(CheckIfValueZero(
										(char*) pobjNoofEntriesSubIndex->GetActualValue())))
						{
							//value is not zero the channel is activated
							iSiTotal =
									GetDecimalValue(
											(char*) pobjNoofEntriesSubIndex->GetActualValue());
#if defined DEBUG
							cout << "iSiTotal:" << iSiTotal << endl;
#endif
						}
						else
						{
							if (0
									== strcmp(
											pobjNoofEntriesSubIndex->GetActualValue(),
											""))
							{
								//pdo channel is deactivated. Empty act value
								continue;
							}
							if (CheckIfValueZero(
									(char*) pobjNoofEntriesSubIndex->GetActualValue()))
							{
								// PDO channel is deactivated
								// Zero is not set here,as it is intialised to Zero previously
								continue;
							}
							else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
							{
								//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
								iSiTotal =
										GetDecimalValue(
												(char*) pobjNoofEntriesSubIndex->GetDefaultValue());
							}
						}

						//Check isiTotal value is valid
						if (iSiTotal
								>= (pobjBforeSortIndex->GetNumberofSubIndexes()))
						{
							objocfmException.OCFMException(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) pobjBforeSortIndex->GetIndexValue(),
									pobjNode->GetNodeId());
							CopyCustomErrorString(
									&(objocfmException._ocfmRetCode),
									acCustomError);

							throw objocfmException;
						}

						//set the correponding 14xx/01 to f0
						if ((true == IsBuild)
								&& (strncmp(objIndex.GetIndexValue(), "16", 2)
										== 0))
						{
							Index *pobjCommIndex = NULL;

							char *varIdx = SubString(
									(char *) objIndex.GetIndexValue(), 2, 4);
							char *varCommIdx = new char[INDEX_LEN];
							strcpy(varCommIdx, (char *) "14");
							strcat(varCommIdx, varIdx);
							pobjCommIndex =
									pobjIndexCollection->GetIndexbyIndexValue(
											varCommIdx);
							if (NULL != pobjCommIndex)
							{
								SubIndex *pobjNodeIDSubIndex = NULL;
								pobjNodeIDSubIndex =
										pobjCommIndex->GetSubIndexbyIndexValue(
												(char *) "01");
								if (NULL != pobjNodeIDSubIndex)
								{
									if ((NULL
											!= pobjNodeIDSubIndex->GetActualValue())
											&& (0
													!= strcmp(
															pobjNodeIDSubIndex->GetActualValue(),
															"")))
									{
										iNodeRPDOMappedNodeID =
												GetDecimalValue(
														(char *) pobjNodeIDSubIndex->GetActualValue());
										if ((CHAINED == eNodeStation)
												&& (BROADCAST_NODEID
														== iNodeRPDOMappedNodeID))
										{
											pobjNodeIDSubIndex->SetActualValue(
													(char *) "0xF0");
											//  bChangeOffset = true;
										}

										if ((CHAINED != eNodeStation)
												&& (MN_NODEID
														== iNodeRPDOMappedNodeID))
										{
											pobjNodeIDSubIndex->SetActualValue(
													(char *) "0x0");
											//  bChangeOffset = true;
										}
										iNodeRPDOMappedNodeID =
												GetDecimalValue(
														(char *) pobjNodeIDSubIndex->GetActualValue());
										if ((BROADCAST_NODEID
												!= iNodeRPDOMappedNodeID))
										{
											if ((MN_NODEID
													!= iNodeRPDOMappedNodeID))
											{
												continue;
											}
										}
									}
								}
							}
							delete[] varCommIdx;
							delete[] varIdx;
						}
						//CN's cannot have 18xx & 1Axx other than 1800 & 1A00 mapped for MN


						while (iSiCount <= iSiTotal)
						{
							SubIndex *pobjSubIdx = NULL;
							pobjSubIdx = objIndex.GetSubIndex(iSiCount);
#if defined DEBUG	
							cout << "\n pobjSubIdx->getIndexValue():"
							<< pobjSubIdx->GetIndexValue() << endl;
							cout << "\n pobjSubIdx->getName():"
							<< pobjSubIdx->GetName() << endl;
#endif
							if ((NULL != pobjSubIdx->GetActualValue())
									&& (0
											!= strcmp(
													pobjSubIdx->GetActualValue(),
													"")))
							{

								const char* varActualVal =
										pobjSubIdx->GetActualValue();
								INT32 iLength = strlen(varActualVal);
								char* varModuleIndex = NULL; // = new char[INDEX_SIZE + ALLOC_BUFFER];
								varModuleIndex = SubString((char*) varActualVal,
										iLength - 4, 4);
								varModuleIndex[5] = '\0';

								/* Get the SubIndex*/
								char* varSubIndex = NULL; // = new char[SUBINDEX_SIZE + ALLOC_BUFFER];
								//varSubIndex = subString(reverseValue,2,2);
								varSubIndex = SubString((char*) varActualVal,
										iLength - 6, 2);
								varSubIndex[3] = '\0';

#if defined DEBUG	
								cout << "varModuleIndex:" << varModuleIndex << "varSubIndex:" << varSubIndex << endl;
#endif

								//Mapped length in bits
								INT32 iMappedLength = 0;
								iMappedLength = HexToInt(SubString((char*) varActualVal, 2, 4));
								cout<<" IntMapLength:"<<iMappedLength<<endl;

								Index *pobjModuleIndex = NULL;
								SubIndex *pobjModuleSIndex = NULL;
								char *uniqueidRefID = NULL;
								char *varSIdxName = NULL;
								char *Access = NULL;
								char *varModuleName = NULL;
								DataType dt;
								dt.Name = NULL;
								bool bObjectMapped = false;

								pobjModuleIndex =
										pobjIndexCollection->GetIndexbyIndexValue(
												varModuleIndex);
								if (pobjModuleIndex == NULL)
								{
									objocfmException.OCFMException(
											OCFM_ERR_MODULE_INDEX_NOT_FOUND);
									char acCustomError[200] =
									{ 0 };
									sprintf(acCustomError,
											"PDO Mapped Module Index Not Found, Index:%s in Node ID:%d",
											varModuleIndex,
											pobjNode->GetNodeId());
									CopyCustomErrorString(
											&(objocfmException._ocfmRetCode),
											acCustomError);

									throw objocfmException;
								}

								if (pobjModuleIndex->GetName() != NULL)
								{

									varModuleName = new char[strlen(
											pobjModuleIndex->GetName())
											+ ALLOC_BUFFER];
									strcpy(varModuleName,
											pobjModuleIndex->GetName());
								}
								if (pobjModuleIndex->GetNumberofSubIndexes()
										== 0 && (strcmp(varSubIndex, "00") == 0))
								{
									bObjectMapped = true;
									if (pobjModuleIndex->GetUniqueIDRef()!= NULL)
									{
										uniqueidRefID =
												new char[strlen(
														pobjModuleIndex->GetUniqueIDRef())
														+ ALLOC_BUFFER];
										strcpy(uniqueidRefID,
												pobjModuleIndex->GetUniqueIDRef());
									}
									else
									{
										if (pobjModuleIndex->GetAccessType() != NULL)
										{
											Access =
													new char[strlen(
															pobjModuleIndex->GetAccessType())
															+ ALLOC_BUFFER];
											strcpy(Access,
													pobjModuleIndex->GetAccessType());

										}
										dt = pobjModuleIndex->GetDataType();
									}

								}
								else
								{
									pobjModuleSIndex =
											pobjModuleIndex->GetSubIndexbyIndexValue(
													varSubIndex);
									if (pobjModuleSIndex == NULL)
									{
										objocfmException.OCFMException(
												OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
										char acCustomError[200] =
										{ 0 };
										sprintf(acCustomError,
												"PDO Mapped Module SubIndex Not Found, Index:%s Subindex:%s in Node ID:%d",
												varModuleIndex, varSubIndex,
												pobjNode->GetNodeId());
										CopyCustomErrorString(
												&(objocfmException._ocfmRetCode),
												acCustomError);
										if (varModuleName != NULL)
										{
											delete[] varModuleName;
										}
										throw objocfmException;
									}
									else
									{
										if (pobjModuleSIndex->GetUniqueIDRef() != NULL)
										{
											uniqueidRefID =
													new char[strlen(
															pobjModuleSIndex->GetUniqueIDRef())
															+ ALLOC_BUFFER];
											strcpy(uniqueidRefID,
													pobjModuleSIndex->GetUniqueIDRef());
										}
										else
										{
											if (pobjModuleSIndex->GetName() != NULL)
											{
												varSIdxName =
														new char[strlen(
																pobjModuleSIndex->GetName())
																+ ALLOC_BUFFER];
												strcpy(varSIdxName,
														pobjModuleSIndex->GetName());

												if (pobjModuleSIndex->GetAccessType() != NULL)
												{
													Access =
															new char[strlen(
																	pobjModuleSIndex->GetAccessType())
																	+ ALLOC_BUFFER];
													strcpy(Access,
															pobjModuleSIndex->GetAccessType());
												}
												dt =
														pobjModuleSIndex->GetDataType();
											}
										}
									}

								}

								PDOType pdoType = objIndex.GetPDOType();
								char* pdoMappingType = new char[strlen(pobjModuleSIndex->GetPDOMapping()) + STR_ALLOC_BUFFER];
								pdoMappingType = strcpy(pdoMappingType, pobjModuleSIndex->GetPDOMapping());

								#if defined DEBUG
								cout<<"Mapping:"<<pdoMappingType<<" pdoType:"<<pdoType<<endl;
								#endif

								if ((0 == strcmp(ConvertToUpper((char*)pdoMappingType), "TPDO")) && (pdoType == PDO_TPDO))
								{
								}
								else if ((0 == strcmp(ConvertToUpper((char*)pdoMappingType), "RPDO")) && (pdoType == PDO_RPDO))
								{
								}
								else
								{
									char* pdoDummyMappingType = new char[ 20 + STR_ALLOC_BUFFER];
									if (0 == strcmp(ConvertToUpper((char*)pdoMappingType), "TPDO"))
									{
										pdoDummyMappingType = strcpy(pdoDummyMappingType, "RPDO");
									}
									else if (0 == strcmp(ConvertToUpper((char*)pdoMappingType), "RPDO"))
									{
										pdoDummyMappingType = strcpy(pdoDummyMappingType, "TPDO");
									}
									else
									{
										if (pdoType == PDO_TPDO)
										{
											pdoDummyMappingType = strcpy(pdoDummyMappingType, "TPDO");
										}
										else
										{
											pdoDummyMappingType = strcpy(pdoDummyMappingType, "RPDO");
										}
										//nothing to do
									}
									ocfmException objex;
									objex.OCFMException(OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO);

									char acCustomError[200] = { 0 };
									//Sample: In Node: 123  invalid object mapped in 1A00 / 01. The object 6000 / 00 has pdo mapping set to OPTIONAL. use an object which has pdo mapping TPDO.
									sprintf(acCustomError, "Node: %d has invalid object mapped in %s / %s. \n The object %s / %s has pdo mapping set to %s. Use an object which has pdo mapping %s.", pobjNode->GetNodeId(), objIndex.GetIndexValue(), pobjSubIdx->GetIndexValue(), varModuleIndex, varSubIndex, pdoMappingType, pdoDummyMappingType);

									CopyCustomErrorString(&(objex._ocfmRetCode), acCustomError);
									delete[] pdoDummyMappingType;
									delete[] pdoMappingType;
									throw objex;
								}
								delete[] pdoMappingType;
								if (uniqueidRefID != NULL)
								{
									INT32 totalBytesMapped = 0;
									totalBytesMapped = DecodeUniqiueIDRef(uniqueidRefID, pobjNode,
											pdoType,
											(char*) pobjModuleIndex->GetName(),
											(char*) pobjModuleIndex->GetIndexValue());
									#if defined DEBUG
										cout<<"iMappedLength:"<<iMappedLength<<" totalBytesMapped:"<<totalBytesMapped<<endl;
									#endif
									if(iMappedLength != (totalBytesMapped*8))
									{
										ocfmException objex;
										objex.OCFMException(OCFM_ERR_INVALID_SIZE_MAPPED);

										char acCustomError[200] = { 0 };
										sprintf(acCustomError, "Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d", (char*)objIndex.GetIndexValue(), (char*)pobjSubIdx->GetIndexValue(), pobjNode->GetNodeId());
										CopyCustomErrorString(&(objex._ocfmRetCode), acCustomError);

										throw objex;
									}
								}
								else
								{
									ProcessImage objProcessImage;
									objProcessImage.Initialize();
									if (dt.GetName() == NULL)
									{
										objocfmException.OCFMException(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										throw objocfmException;
									}
									else if (!CheckAllowedDTForMapping(
											dt.GetName()))
									{
										objocfmException.OCFMException(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										throw objocfmException;
									}
									objProcessImage.CNNodeID =
											pobjNode->GetNodeId();
									/* Name of the Process Image variable*/
									objProcessImage.Name = (char*) malloc(
											6 + ALLOC_BUFFER);
									strcpy(objProcessImage.Name,
											GetPIName(pobjNode->GetNodeId()));

									if (varModuleName != NULL)
									{
										objProcessImage.Name =
												(char*) realloc(
														objProcessImage.Name,
														strlen(
																objProcessImage.Name)+ strlen(varModuleName) + ALLOC_BUFFER);
										strcat(objProcessImage.Name,
												varModuleName);

										objProcessImage.ModuleName =
												new char[strlen(varModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.ModuleName,
												varModuleName);
									}
									else
									{
										/*If the object name is empty, Ixxxx (xxxx is the index number in hex) */
										varModuleName = new char[INDEX_LEN + 1
												+ ALLOC_BUFFER];
										strcpy(varModuleName, "I");
										strcat(varModuleName,
												objIndex.GetIndexValue());

										objProcessImage.Name =
												(char*) realloc(
														objProcessImage.Name,
														strlen(
																objProcessImage.Name)+ strlen(varModuleName) + ALLOC_BUFFER);
										strcat(objProcessImage.Name,
												varModuleName);

										objProcessImage.ModuleName =
												new char[strlen(varModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.ModuleName,
												varModuleName);
									}

									objProcessImage.ModuleIndex =
											new char[strlen(varModuleIndex)
													+ STR_ALLOC_BUFFER];
									strcpy(objProcessImage.ModuleIndex,
											varModuleIndex);

									if (bObjectMapped)
									{
										objProcessImage.VarName =
												new char[strlen(varModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.VarName,
												varModuleName);
									}
									else
									{
										if (varSIdxName != NULL)
										{
											objProcessImage.VarName =
													new char[strlen(varSIdxName)
															+ STR_ALLOC_BUFFER];
											strcpy(objProcessImage.VarName,
													varSIdxName);

											objProcessImage.Name =
													(char*) realloc(
															objProcessImage.Name,
															(strlen(
																	objProcessImage.Name)
																	+ strlen(
																			varSIdxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(objProcessImage.Name, ".");
											strcat(objProcessImage.Name,
													varSIdxName);
										}
										else
										{
											//TODO: pobjModulesubindex may be null. Check for null befor used.

											/*If the subobject name is empty, use Sxx (xx is the subindex number in hex) */
											varSIdxName = new char[SUBINDEX_LEN
													+ 1 + ALLOC_BUFFER];

											strcpy(varSIdxName, "S");
											strcat(varSIdxName,
													pobjModuleSIndex->GetIndexValue());

											objProcessImage.VarName =
													new char[strlen(varSIdxName)
															+ STR_ALLOC_BUFFER];
											strcpy(objProcessImage.VarName,
													varSIdxName);

											objProcessImage.Name =
													(char*) realloc(
															objProcessImage.Name,
															(strlen(
																	objProcessImage.Name)
																	+ strlen(
																			varSIdxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(objProcessImage.Name, ".");
											strcat(objProcessImage.Name,
													varSIdxName);

										}
									}

#if defined DEBUG
									cout<<"DataSize: "<<dt.dataSize*8<<" MappedLength: "<<iMappedLength<<endl;
#endif
									if((dt.dataSize * 8) != iMappedLength)
									{
										ocfmException objex;
										objex.OCFMException(OCFM_ERR_INVALID_SIZE_MAPPED);
										char acCustomError[200] = { 0 };
										sprintf(acCustomError, "Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d", (char*)objIndex.GetIndexValue(), (char*)pobjSubIdx->GetIndexValue(), pobjNode->GetNodeId());
										CopyCustomErrorString(&(objex._ocfmRetCode), acCustomError);

										throw objex;
									}
									objProcessImage.DataInfo.DataSize =
											dt.dataSize * 8;

									/* Total bytes Mapped */
									iTotalBytesMapped = iTotalBytesMapped
											+ dt.dataSize;
									if (iTotalBytesMapped > MAX_PI_SIZE)
									{
										ocfmException objex;
										objex.OCFMException(
												OCFM_ERR_MAX_PI_SIZE);
										delete[] varSIdxName;
										delete[] Access;
										delete[] varModuleName;
										throw objex;
									}
									/* Datatype in hex of the Process Image variable*/
									objProcessImage.DataInfo._dt_Name =
											new char[strlen(dt.Name)
													+ STR_ALLOC_BUFFER];
									strcpy(objProcessImage.DataInfo._dt_Name,
											dt.Name);
									objProcessImage.DataInfo._dt_enum =
											dt.IEC_dt;

									if (pdoType == PDO_TPDO)
									{
										objProcessImage.ByteOffset =
												ComputeINOffset(
														objProcessImage.DataInfo.DataSize,
														pdoType);
										objProcessImage.DirectionType = INPUT;
									}
									else if (pdoType == PDO_RPDO)
									{
										objProcessImage.ByteOffset =
												ComputeOUTOffset(
														objProcessImage.DataInfo.DataSize,
														pdoType);
										objProcessImage.DirectionType = OUTPUT;
									}

									objProcessImage.BitOffset = 0;
									CreateMNPDOVar(objProcessImage.ByteOffset,
											objProcessImage.DataInfo.DataSize,
											objProcessImage.DataInfo._dt_enum,
											pdoType, pobjNode);
									pobjNode->AddProcessImage(objProcessImage);
									delete[] varModuleName;
									delete[] varSIdxName;
								}

								if ((true == IsBuild)
										&& (strncmp(objIndex.GetIndexValue(),
												"16", 2) == 0)
										&& ((MN_NODEID == iNodeRPDOMappedNodeID)
												|| (BROADCAST_NODEID
														== iNodeRPDOMappedNodeID)))
								{
									char* varModOffset = new char[strlen(
											varActualVal) + 1];
									strcpy(varModOffset, varActualVal);
									INT32 iLength = 0;
									char* varLength = NULL;
									//extract the length mapped 1AXX
									varLength = SubString((char *) varActualVal,
											2, 4);
									iLength = HexToInt(varLength);

									char* offset = new char[5];
									memset(offset, 0, 5 * sizeof(char));
									if (CHAINED == eNodeStation)
									{
										offset = IntToAscii(
												iTotalChainedBytesMapped,
												&(offset[0]), 16);
									}
									else
									{
										offset = IntToAscii(
												iNodeMappedTotalBytes,
												&(offset[0]), 16);
									}
									offset = PadLeft(&(offset[0]), '0', 4);
									INT32 iOffsetCopyCount;
									for (iOffsetCopyCount = 0;
											iOffsetCopyCount <= 3;
											iOffsetCopyCount++)
									{
										varModOffset[iOffsetCopyCount + 2 + 4] =
												offset[iOffsetCopyCount];
									}
									strcpy(varModOffset,
											ConvertToUpper(varModOffset));

									pobjIndexCollection->GetIndexbyIndexValue(
											(char *) pobjBforeSortIndex->GetIndexValue())->GetSubIndexbyIndexValue(
											(char *) pobjSubIdx->GetIndexValue())->SetActualValue(
											varModOffset);

									iNodeMappedTotalBytes =
											iNodeMappedTotalBytes + iLength;

									if (CHAINED == eNodeStation)
									{
										iTotalChainedBytesMapped =
												iTotalChainedBytesMapped
														+ iLength;

									}

									delete[] varModOffset;
									delete[] varLength;
									delete[] offset;
								}
							}
							iSiCount++;
						}
					}
				}
			}
			else
			{
				// pobjMNNode = pobjNode;
			}

		}
		delete[] pArrangedNodeIDbyStation;
		//The PI variable name should be unique
		SetUniquePIVarName();
		//find the time of build
		SetBuildTime();

		stRetInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************************/
/**
 \brief		CalculatePayload

 This function calculates the PReq and PRes payload value for node

 \return	void
 */
/******************************************************************************************/

void CalculatePayload()
{
//TODO: objocfmException is not catched. Try catch block should be used.
	NodeCollection *objNodeCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();

	//	INT32 iNodeMappedTotalBytes = 0;

	/*Check RPDO Mapped objects*/

	if (objNodeCol->GetCNNodesCount() == 0)
	{
		exit(0);
	}
	ocfmException objocfmException;
	Node *pobjNode = NULL;
	Node *pobjMNNode = NULL;
	INT32 iTotalChainedBytesMapped = 0;
	INT32* pArrangedNodeIDbyStation = NULL;
	pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
			iLoopCount++)
	{
		pobjNode = objNodeCol->GetNodebyColIndex(iLoopCount);
		/* Process PDO Objects for CN*/

		if (pobjNode->GetNodeType() == MN)
		{
			pobjMNNode = pobjNode;
		}
		else
		{
			if (!(pobjNode->HasPdoObjects()))
			{
				continue;
			}

			StationType eNodeStation = pobjNode->GetStationType();
			IndexCollection* objPDOCollection = NULL;
			INT32 tpdoCount = 0;
			INT32 rpdoCount = 0;
			objPDOCollection = pobjNode->getPDOIndexCollection(&rpdoCount, &tpdoCount);

			if (objPDOCollection != NULL)
			{
				IndexCollection* pobjIndexCollection = NULL;
				pobjIndexCollection = pobjNode->GetIndexCollection();

				pobjNode->SetPReqActPayloadValue(0);
				pobjNode->SetPResActPayloadValue(0);

				for (INT32 iLoopCount = 0;
						iLoopCount < objPDOCollection->GetNumberofIndexes();
						iLoopCount++)
				{
					Index* pobjBforeSortIndex;
					Index objIndex;
					pobjBforeSortIndex = objPDOCollection->GetIndex(iLoopCount);
					if (!(CheckIfMappingPDO(
							(char*) pobjBforeSortIndex->GetIndexValue())))
					{
						continue;
					}
					//	iNodeMappedTotalBytes = 0;

					if (pobjBforeSortIndex->GetNumberofSubIndexes() > 0)
					{
						/* Sort the pdo collection */
						objIndex = GetPDOIndexByOffset(pobjBforeSortIndex);
						INT32 iSiCount = 1;
						INT32 iSiTotal = 0;

						SubIndex *pobjNoofEntriesSubIndex;
						pobjNoofEntriesSubIndex =
								pobjBforeSortIndex->GetSubIndexbyIndexValue(
										(char *) "00");
						if (NULL != pobjNoofEntriesSubIndex)
						{
							if ((pobjNoofEntriesSubIndex->GetActualValue()
									!= NULL) // Actual value checked for Null
									&& (0
											!= strcmp(
													pobjNoofEntriesSubIndex->GetActualValue(),
													"")) // Actual value checked for Empty
									&& !(CheckIfValueZero(
											(char*) pobjNoofEntriesSubIndex->GetActualValue()))) // Actual value checked for non-zero
							{
								//value is not zero the channel is activated
								iSiTotal =
										GetDecimalValue(
												(char*) pobjNoofEntriesSubIndex->GetActualValue());
#if defined DEBUG	
								cout << "iSiTotal:" << iSiTotal << endl;
#endif
							}
							else
							{
								if (0
										== strcmp(
												pobjNoofEntriesSubIndex->GetActualValue(),
												""))
								{
									//pdo channel is deactivated. Empty act value
									continue;
								}
								if (CheckIfValueZero(
										(char*) pobjNoofEntriesSubIndex->GetActualValue()))
								{
									// PDO channel is deactivated
									// Zero is not set here,as it is intialised to Zero previously
									continue;
								}
								else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
								{
									//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
									iSiTotal =
											GetDecimalValue(
													(char*) pobjNoofEntriesSubIndex->GetDefaultValue());
								}
							}

						}
						else
						{
							//no of entries index does not exist
							continue;
						}
						//Check isiTotal value is valid
						if (iSiTotal
								>= (pobjBforeSortIndex->GetNumberofSubIndexes()))
						{
							objocfmException.OCFMException(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) pobjBforeSortIndex->GetIndexValue(),
									pobjNode->GetNodeId());
							CopyCustomErrorString(
									&(objocfmException._ocfmRetCode),
									acCustomError);

							throw objocfmException;
						}

						INT32 iNodeRPDOMappedNodeID = -1;
						if (strncmp(objIndex.GetIndexValue(), "16", 2) == 0)
						{
							Index *pobjCommIndex = NULL;
							char *varIdx = SubString(
									(char *) objIndex.GetIndexValue(), 2, 4);
							char *varCommIdx = new char[INDEX_LEN];
							strcpy(varCommIdx, (char *) "14");
							strcat(varCommIdx, varIdx);
							pobjCommIndex =
									pobjIndexCollection->GetIndexbyIndexValue(
											varCommIdx);
							if (NULL != pobjCommIndex)
							{
								SubIndex *pobjNodeIDSubIndex = NULL;
								pobjNodeIDSubIndex =
										pobjCommIndex->GetSubIndexbyIndexValue(
												(char *) "01");
								if (NULL != pobjNodeIDSubIndex)
								{
									iNodeRPDOMappedNodeID =
											GetDecimalValue(
													(char*) pobjNodeIDSubIndex->GetActualValue());
								}
							}
							delete[] varCommIdx;
							delete[] varIdx;
						}

						while (iSiCount <= iSiTotal)
						{

							SubIndex* pobjSubIdx;
							pobjSubIdx = objIndex.GetSubIndex(iSiCount);

							iSiCount++;

							if ((pobjSubIdx->GetActualValue() == NULL)
									|| (0
											== strcmp(
													pobjSubIdx->GetActualValue(),
													""))
									|| (CheckIfValueZero(
											(char*) pobjSubIdx->GetActualValue())))
							{
								continue;
							}

							const char* varActualVal =
									pobjSubIdx->GetActualValue();

							if ((strncmp(objIndex.GetIndexValue(), "16", 2) == 0)
									&& ((MN_NODEID == iNodeRPDOMappedNodeID)
											|| (BROADCAST_NODEID
													== iNodeRPDOMappedNodeID)))
							{
								char* varModOffset = new char[strlen(varActualVal)
										+ 1];
								strcpy(varModOffset, varActualVal);
								INT32 iLength = 0;
								INT32 iOffset = 0;

								char* varLength = NULL;
								varLength = SubString((char *) varActualVal, 2,
										4);
								iLength = HexToInt(varLength);

								char* offset = NULL;
								offset = SubString((char *) varActualVal, 6, 4);
								iOffset = HexToInt(offset);

								//	iNodeMappedTotalBytes = iOffset + iLength;

								if (CHAINED == eNodeStation)
								{
									iTotalChainedBytesMapped = iOffset
											+ iLength;
								}
								if (BROADCAST_NODEID == iNodeRPDOMappedNodeID)
								{
									pobjNode->SetPReqActPayloadValue(
											(iOffset + iLength) / 8);
								}

								delete[] varModOffset;
								delete[] varLength;
								delete[] offset;
							}
							if (strncmp(objIndex.GetIndexValue(), "1A", 2) == 0)
							{
								char* varModOffset = new char[strlen(varActualVal)
										+ 1];
								strcpy(varModOffset, varActualVal);
								INT32 iLength = 0;
								char* varLength = NULL;
								varLength = SubString((char *) varActualVal, 2,
										4);
								iLength = HexToInt(varLength);

								char* varOffset = NULL;
								varOffset = SubString((char *) varActualVal, 6,
										4);
								INT32 iOffset = 0;
								iOffset = HexToInt(varOffset);

								pobjNode->SetPResActPayloadValue(
										(iOffset + iLength) / 8);

								delete[] varModOffset;
								delete[] varLength;
								delete[] varOffset;
							}
						}
					}
				}
			}
			UpdatePreqActLoad(pobjNode);
			UpdatePresActLoad(pobjNode);
		}
	}
	if ((NULL != pobjMNNode) && (true == IsPresMN()))
	{
		pobjMNNode->SetPResActPayloadValue(iTotalChainedBytesMapped / 8);
		UpdatePresActLoad(pobjMNNode);
	}
	delete[] pArrangedNodeIDbyStation;
}

/*****************************************************************************************/
/**
 \brief		GetCNDataLen

 This function returns the total size of data in the buffer

 \param		varBuffer		Character pointer to hold the buffer data

 \return	INT32
 */
/******************************************************************************************/

INT32 GetCNDataLen(char* varBuffer)
{
	if (NULL == varBuffer)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	INT32 iCount = 0;
	INT32 iNoOfChars = 0;
	char cCharRead = 0;
	INT32 iCtr = 0;
	INT32 iLength = 0;
	INT32 iLoopCount = 0;
	//UINT8 abCnObd[60000];
	//UINT8 abTempCnObd[60000];

	cCharRead = *(varBuffer);

	while (cCharRead != '\0')
	{
		cCharRead = *(varBuffer + iLoopCount);
		if (cCharRead == '/')
		{
			while (cCharRead != '\n')
			{
				cCharRead = *(varBuffer + iLoopCount);
				iLoopCount++;
			}
		}
		else if (cCharRead != '\t' || cCharRead != '\n' || cCharRead != ' ')
		{
			INT32 iCharRead = 0;
			// Convert to Upper case
			iCharRead = toupper(cCharRead);
			if ((iCharRead >= 65 && iCharRead <= 70)
					|| (iCharRead >= 97 && iCharRead <= 102))
			{
				//abCnObd[iNoOfChars] = iCharRead - 55;
				iNoOfChars++;
			}
			else if ((iCharRead >= 48 && iCharRead <= 57))
			{
				//abCnObd[iNoOfChars] = iCharRead - 48;
				iNoOfChars++;
			}
			else
			{
				//TODO: 'else' added. Operation to be specified
			}
			iLoopCount++;
		}
		else
		{
			//TODO: 'else' added. Operation to be specified
		}

		iLength = iNoOfChars;
		//For Byte Packing
		for (iCtr = 0, iCount = 0; iCtr < iLength; iCtr++, iCount++)
		{
			//abTempCnObd[iCount] = (UINT8)( ( abCnObd[ iCtr ] << 4 ) | abCnObd[ iCtr + 1 ] );
			iCtr++;
		}
	}
	return iCount;
}

/*****************************************************************************************/
/**
 \brief		LenOfCNBuffer

 This function returns the total size of data in the buffer

 \param		varBuffer		Character pointer to hold the buffer data

 \return	INT32
 */
/******************************************************************************************/
//TODO: unused function
INT32 LenOfCNBuffer(char* varBuffer)
{
	char cTempChar;
	INT32 iLength = strlen(varBuffer);
	INT32 iLoopCount = 0;
	INT32 iActualLen = 0;

	while (iLoopCount < iLength)
	{
		cTempChar = *(varBuffer + iLoopCount);
		if (cTempChar != '\n' && cTempChar != '\t')
			iActualLen++;
		iLoopCount++;
	}
	return iActualLen;

}

/*****************************************************************************************/
/**
 \brief			GetMNIndexValues

 This function returns the CIndex class pointer for the index id

 \param			varIndex		Character pointer to hold the Index id value

 \return	CIndex*
 */
/******************************************************************************************/

Index* GetMNIndexValues(char* varIndex)
{
	NodeCollection* pobjNodeCollection;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	IndexCollection* objIndexCol;
	Node *objNode;
//TODO: Review
	//CNode objNode;
	//objNode = pobjNodeCollection->getMNNode();
	//objIndexCol = objNode.getIndexCollection();
	objNode = pobjNodeCollection->GetNodePtr(MN, MN_NODEID);
	objIndexCol = objNode->GetIndexCollection();
	Index* pobjIndex = NULL;
	pobjIndex = objIndexCol->GetIndexbyIndexValue(varIndex);
	return pobjIndex;
}

/*****************************************************************************************/
/**
 \brief		GetMNSubIndexValues

 This function returns the CSubIndex class pointer for the specific index & SubIndex

 \param		varIndex		Character pointer to hold the Index id value
 \param		varSubIndex		Character pointer to hold the SubIndex id value

 \return	SubIndex* / NULL
 */
/******************************************************************************************/

SubIndex* GetMNSubIndexValues(char* varIndex, char* varSubIndex)
{
	NodeCollection* pobjNodeCollection;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	IndexCollection* objIndexCol;
	Node objNode;

	objNode = pobjNodeCollection->GetMNNode();
	objIndexCol = objNode.GetIndexCollection();
	Index* pobjIndex = NULL;
	pobjIndex = objIndexCol->GetIndexbyIndexValue(varIndex);
	if (NULL == pobjIndex)
		return NULL;

	SubIndex* pobjSubIndex = NULL;
	pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue(varSubIndex);
	return pobjSubIndex;
}

/*****************************************************************************************/
/**
 \brief		WriteXAPElements

 This function writes the xap configurations

 \param		aobjPICol		struct array of type ProcessImage to hold the collection
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		iVarCount		Integer variable to hold processimage variable count
 \param		piType			Enumeration flag of type EPIDirectionType(input/output)

 \return	void
 */
/******************************************************************************************/

void WriteXAPElements(ProcessImage aobjPICol[], xmlTextWriterPtr& pxtwWriter,
		INT32 iVarCount, EPIDirectionType piType)
{

	if (iVarCount != 0)
	{
		INT32 iHighBitOffset = 0;
		INT32 iHighBitOffsetDatasize = 0;
		INT32 iBytesWritten;
		/* Start an element named "Channel". Since thist is the first
		 * element, this will be the root element of the document. */
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
				BAD_CAST "ProcessImage");
		if (iBytesWritten < 0)
		{
			return;
		}
		iBytesWritten = -1;
		if (piType == INPUT)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "type", BAD_CAST "output");
		}
		else if (piType == OUTPUT)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "type", BAD_CAST "input");
		}
		else
		{
			//TODO: "else" Added.Operation need to be specified
		}

		if (iBytesWritten < 0)
		{
			return;
		}

		//find the size and write it to attribute
		for (INT32 iLoopCount = 0; iLoopCount < iVarCount; iLoopCount++)
		{
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage = aobjPICol[iLoopCount];

			if (iHighBitOffset
					<= ((objProcessImage.ByteOffset * 8)
							+ objProcessImage.BitOffset))
			{
				//save the higher offset of process image and its datasize
				iHighBitOffset = (objProcessImage.ByteOffset * 8)
						+ objProcessImage.BitOffset; //multiply by 8 for byte to bit conversion
				iHighBitOffsetDatasize = objProcessImage.DataInfo.DataSize;
			}
		}
		char *varPISize = new char[20];
		varPISize = IntToAscii((iHighBitOffset + iHighBitOffsetDatasize) / 8,
				varPISize, 10); //divide by 8 for bit to byte conversion
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "size",
				BAD_CAST varPISize);
		delete[] varPISize;
		if (iBytesWritten < 0)
		{
			return;
		}

		for (INT32 iLoopCount = 0; iLoopCount < iVarCount; iLoopCount++)
		{
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage = aobjPICol[iLoopCount];
			/* Start an element named "Channel". Since thist is the first
			 * element, this will be the root element of the document. */
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
					BAD_CAST "Channel");
			if (iBytesWritten < 0)
			{
				return;
			}

			/* Add an attribute with name "Name" and value  to channel. */
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "Name", BAD_CAST objProcessImage.Name);

			if (iBytesWritten < 0)
			{
				return;
			}
			/* Add an attribute with name "DataType" and value to channel */
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "dataType",
					BAD_CAST objProcessImage.DataInfo._dt_Name);
			if (iBytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char * varDataSize = new char[20];
			varDataSize = IntToAscii(objProcessImage.DataInfo.DataSize,
					varDataSize, 10);

			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "dataSize", BAD_CAST varDataSize);
			delete[] varDataSize;
			if (iBytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char* varByteOffset = new char[6];
			varByteOffset = IntToAscii(objProcessImage.ByteOffset, varByteOffset,
					16);
			varByteOffset = ConvertToHexformat(varByteOffset, 4, 1);

			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "PIOffset", BAD_CAST varByteOffset);
			delete[] varByteOffset;
			if (iBytesWritten < 0)
			{
				return;
			}

			if (objProcessImage.BitOffset != -1)
			{
				char* varBitOffset = new char[4];
				varBitOffset = IntToAscii(objProcessImage.BitOffset,
						varBitOffset, 16);
				varBitOffset = ConvertToHexformat(varBitOffset, 2, 1);

				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
						BAD_CAST "BitOffset", BAD_CAST varBitOffset);
				delete[] varBitOffset;
			}
			if (iBytesWritten < 0)
			{
				return;
			}
			///* Close the element named Channel. */
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
			if (iBytesWritten < 0)
			{
				return;
			}
		}
		///* Close the element named ProcessImage. */
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			return;
		}

	}
}

/*****************************************************************************************/
/**
 \brief		StartXAPxml

 This function writes the xml tag & autogeneraged context for the xap.h
 
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		pxdDoc			xml pointer of type xmlDocPtr

 \return	void
 */
/******************************************************************************************/

void StartXAPxml(xmlTextWriterPtr& pxtwWriter, xmlDocPtr& pxdDoc)
{
	INT32 iBytesWritten;

	/* Create a new XmlWriter for DOM, with no compression. */
	pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
	if (pxtwWriter == NULL)
	{
		return;
	}

	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING,
			NULL);
	if (iBytesWritten < 0)
	{
		return;
	}

	/* Write a comment as child of ORDER */
	char* varComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN];
	strcpy(varComment, BUILD_COMMENT);
	strcat(varComment, GetBuildTime());
	iBytesWritten = xmlTextWriterWriteComment(pxtwWriter, BAD_CAST varComment);
	delete[] varComment;
	if (iBytesWritten < 0)
	{
		return;
	}

	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "ApplicationProcess");
	if (iBytesWritten < 0)
	{
		return;
	}
}

/*****************************************************************************************/
/**
 \brief		EndWrtitingXAP

 This function completes the xap configuration & saves the file
 
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		varFileName		Char pointer to hold the file name
 \param		pxdDoc			xml pointer of type xmlDocPtr

 \return	void
 */
/******************************************************************************************/

void EndWrtitingXAP(xmlTextWriterPtr& pxtwWriter, char* varFileName,
		xmlDocPtr& pxdDoc)
{
	INT32 iBytesWritten;

	// Close the element named ApplicationProcess.
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		return;
	}
	iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
	if (iBytesWritten < 0)
	{
		return;
	}

	xmlFreeTextWriter(pxtwWriter);

	xmlSaveFormatFile((const char*) varFileName, pxdDoc, 1);

	xmlFreeDoc(pxdDoc);
}

/*****************************************************************************************/
/**
 \brief		GenerateXAP

 This API writes the process image variables to the XAP files [.h and .xml] in the location specified

 Note: This API is recommended to be called after the GenerateCDC API.

 \param		varFileName		Char pointer to hold the path of the location to which the generated xap contents will be written into

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateXAP(char* varFileName)
{
	NodeCollection* pobjNodeCollection = NULL;
	ocfmException objException;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

	char* xapFileName = new char[strlen(varFileName) + 4 + ALLOC_BUFFER];
	ProcessImage* aobjPiInCol = NULL;
	ProcessImage* aobjPiOutCol = NULL;
	try
	{
		if (pobjNodeCollection->GetNumberOfNodes() == 0)
		{
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->GetCNNodesCount() == 0)
		{
			objException.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw objException;
		}

		//CNode objNode;
		xmlTextWriterPtr pxtwWriter = NULL;
		xmlDocPtr pxdDoc = NULL;

		//ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
		//ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};

		aobjPiInCol = new ProcessImage[PI_VAR_COUNT];
		aobjPiOutCol = new ProcessImage[PI_VAR_COUNT];
		GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);

		StartXAPxml(pxtwWriter, pxdDoc);
		if (iInVars != 0)
			WriteXAPElements(aobjPiInCol, pxtwWriter, iInVars, INPUT);

		if (iOutVars != 0)
			WriteXAPElements(aobjPiOutCol, pxtwWriter, iOutVars, OUTPUT);

		strcpy(xapFileName, varFileName);
		strcat(xapFileName, ".xml");
		EndWrtitingXAP(pxtwWriter, xapFileName, pxdDoc);

		/*Generate Header file */
		GenerateXAPHeaderFile(varFileName, aobjPiInCol, aobjPiOutCol, iInVars,
				iOutVars);
		delete[] aobjPiInCol;
		delete[] aobjPiOutCol;
	} catch (ocfmException& ex)
	{
		delete[] xapFileName;
		return ex._ocfmRetCode;
	}
	delete[] xapFileName;
	return objException._ocfmRetCode;
}

/*****************************************************************************************/
/**
 \brief		GenerateNET

 This API shall be used to generate the process image variables in a C# .NET usable format.

 Note: This API is recommended to be called after the GenerateCDC API.

 \param		varFileName		Char pointer to hold the path of the location to which the generated NET contents will be written into

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateNET(char* varFileName)
{
	NodeCollection* pobjNodeCollection;
	ocfmException objException;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	ProcessImage* aobjPiInCol = NULL;
	ProcessImage* aobjPiOutCol = NULL;
	try
	{
		if (pobjNodeCollection->GetNumberOfNodes() == 0)
		{
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->GetCNNodesCount() == 0)
		{
			objException.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw objException;
		}

		//CNode objNode;
		//                ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
		//                ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};
		aobjPiInCol = new ProcessImage[PI_VAR_COUNT];
		aobjPiOutCol = new ProcessImage[PI_VAR_COUNT];
		GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);
		/*Generate Dot NET Header file */
		GenerateNETHeaderFile(varFileName, aobjPiInCol, aobjPiOutCol, iInVars,
				iOutVars);
		delete[] aobjPiInCol;
		delete[] aobjPiOutCol;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return objException._ocfmRetCode;
}

/*****************************************************************************************/
/**
 \brief		GetIndexAttributes

 This API shall be used to get the attributes of an Index. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', this API fills the 'varOutAttributeValue' variable with the value for the specified 'varAttributeType'. Refer the table for the  'varAttributeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		varNodeType			Enum to hold the Node type of the node
 \param		varIndexID				Character pointer to hold the IndexID
 \param		varAttributeType		Enum to hold the Attribute type of the index 
 \param		varOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GetIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, AttributeType varAttributeType,
		char* varOutAttributeValue)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		Node objNode;
		NodeCollection *pobjNodeCollection;
		IndexCollection *pobjIndexCollection;
		Index objIndex;
		Index* pobjIndexPtr;

		objIndex.SetNodeID(objNode.GetNodeId());
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		pobjIndexPtr = pobjIndexCollection->GetIndex(iIndexPos);

		switch (varAttributeType)
		{
		case NAME:
			if (pobjIndexPtr->GetName() != NULL)
				strcpy(varOutAttributeValue, (char *) pobjIndexPtr->GetName());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjIndexPtr->GetObjectType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetObjectType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjIndexPtr->GetDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(varOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjIndexPtr->GetAccessType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetAccessType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjIndexPtr->GetDefaultValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetDefaultValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjIndexPtr->GetActualValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetActualValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjIndexPtr->GetPDOMapping() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetPDOMapping());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjIndexPtr->GetLowLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetLowLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjIndexPtr->GetHighLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetHighLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (pobjIndexPtr->GetFlagIfIncludedCdc() == TRUE)
				strcpy(varOutAttributeValue, "1");
			else
				strcpy(varOutAttributeValue, "0");
			break;
		default:
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			throw objException;
		}
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************************/
/**
 \brief		GetIndexAttributesbyPositions

 This API shall be used to get the attributes of an Index given the position of the Node, position of the Index and the attribute type. Refer the EAttributeType table for the attributes. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the respective attribute's value is filled in the 'piOutAttributeValue'.

 \param		iNodePos				Integer variable to hold the position of the node
 \param		iIndexPos				Integer variable to hold the position of the Index
 \param		varAttributeType		Enum to hold the Attribute type of the index .
 \param		varOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GetIndexAttributesbyPositions(INT32 iNodePos, INT32 iIndexPos,
		AttributeType varAttributeType, char* varOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node objNode;
		NodeCollection *pobjNodeCollection;
		IndexCollection *pobjIndexCollection;
		Index* pobjIndexPtr;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->GetNumberOfNodes();
		if (iNodePos >= iTempNodeCount)
		{
			ocfmException objException;                //new ocfmException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;                // = new ocfmException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.GetIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->GetNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndexPtr = pobjIndexCollection->GetIndex(iIndexPos);

		switch (varAttributeType)
		{
		case NAME:
			if (pobjIndexPtr->GetName() != NULL)
				strcpy(varOutAttributeValue, (char *) pobjIndexPtr->GetName());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjIndexPtr->GetObjectType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetObjectType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjIndexPtr->GetDataType();
			if (objTmpDataType.Name != NULL)
			{
				strcpy(varOutAttributeValue, objTmpDataType.Name);
			}
			else
			{
				strcpy(varOutAttributeValue, "");
			}
			break;
		case ACCESSTYPE:
			if (pobjIndexPtr->GetAccessType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetAccessType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjIndexPtr->GetDefaultValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetDefaultValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjIndexPtr->GetActualValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetActualValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjIndexPtr->GetPDOMapping() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetPDOMapping());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjIndexPtr->GetLowLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetLowLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjIndexPtr->GetHighLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjIndexPtr->GetHighLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (pobjIndexPtr->GetFlagIfIncludedCdc() == TRUE)
				strcpy(varOutAttributeValue, "1");
			else
				strcpy(varOutAttributeValue, "0");
			break;
		default:
			stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return stErrorInfo;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexAttributes

 This API shall be used to get the attributes of a SubIndex. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', this API fills the 'varOutAttributeValue' variable with the value for the specified 'varAttributeType'. Refer the EattributeType table for the  'varAttributeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		varNodeType				Enum to hold the Node type of the node
 \param		varIndexID				Character pointer to hold the IndexID
 \param		varSubIndexID			Character pointer to hold the SubIndexID
 \param		varAttributeType		Enum to hold the Attribute type of the index .
 \param		varOutAttributeValue	Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexAttributes(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, char* varSubIndexID, AttributeType varAttributeType,
		char* varOutAttributeValue)
{
	Node objNode;
	NodeCollection *pobjNodeCollection = NULL;
	IndexCollection *pobjIndexCollection = NULL;
	Index objIndex;
	Index* pobjSubIndex = NULL;
	INT32 iSubIndexPos = -1;
	INT32 iIndexPos = -1;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{

		stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, varIndexID,
				varSubIndexID, &iSubIndexPos, &iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		objIndex.SetNodeID(objNode.GetNodeId());
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		pobjSubIndex = pobjIndexCollection->GetIndex(iIndexPos);

		SubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjSubIndex->GetSubIndex(iSubIndexPos);

		switch (varAttributeType)
		{
		case NAME:
			if (pobjSubIndexPtr->GetName() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetName());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjSubIndexPtr->GetObjectType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetObjectType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjSubIndexPtr->GetDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(varOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjSubIndexPtr->GetAccessType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetAccessType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjSubIndexPtr->GetDefaultValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetDefaultValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjSubIndexPtr->GetActualValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetActualValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjSubIndexPtr->GetPDOMapping() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetPDOMapping());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjSubIndexPtr->GetLowLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetLowLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjSubIndexPtr->GetHighLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetHighLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (pobjSubIndexPtr->GetFlagIfIncludedCdc() == TRUE)
				strcpy(varOutAttributeValue, "1");
			else
				strcpy(varOutAttributeValue, "0");
			break;
		default:
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexAttributesbyPositions

 This API shall be used to get the attributes of a SubIndex given the position of the Node, position of the Index, position of the SubIndex and the attribute type. Refer the EAttributeType table for the attributes. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the respective attribute's value is filled in the 'piOutAttributeValue'.

 \param		iNodePos				Integer variable to hold the position of the node
 \param		iIndexPos				Integer variable to hold the position of the Index
 \param		iSubIndexPos			Integer variable to hold the position of the SubIndex
 \param		varAttributeType		Enum to hold the Attribute type of the index
 \param		varOutAttributeValue	Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexAttributesbyPositions(INT32 iNodePos, INT32 iIndexPos,
		INT32 iSubIndexPos, AttributeType varAttributeType,
		char* varOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node objNode;
		NodeCollection *pobjNodeCollection = NULL;
		IndexCollection *pobjIndexCollection = NULL;
		Index* pobjIndexPtr = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->GetNumberOfNodes();
		if (iNodePos >= iTempNodeCount)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.GetIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->GetNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndexPtr = pobjIndexCollection->GetIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjIndexPtr->GetNumberofSubIndexes();

		if (iSubIndexPos >= iTempSubIndexCount)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else if (iTempSubIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjIndexPtr->GetSubIndex(iSubIndexPos);

		switch (varAttributeType)
		{
		case NAME:
			if (pobjSubIndexPtr->GetName() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetName());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjSubIndexPtr->GetObjectType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetObjectType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjSubIndexPtr->GetDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(varOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjSubIndexPtr->GetAccessType() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetAccessType());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjSubIndexPtr->GetDefaultValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetDefaultValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjSubIndexPtr->GetActualValue() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetActualValue());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjSubIndexPtr->GetPDOMapping() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetPDOMapping());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjSubIndexPtr->GetLowLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetLowLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjSubIndexPtr->GetHighLimit() != NULL)
				strcpy(varOutAttributeValue,
						(char *) pobjSubIndexPtr->GetHighLimit());
			else
				strcpy(varOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (pobjSubIndexPtr->GetFlagIfIncludedCdc() == TRUE)
				strcpy(varOutAttributeValue, "1");
			else
				strcpy(varOutAttributeValue, "0");
			break;

		default:
			stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return stErrorInfo;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;

}

/*****************************************************************************/
/**
 \brief		GetNodeCount
 
 This API shall be used to get the number of controlled nodes under a MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutNodeCount' is filled with the Node count value.
 
 \param		iMnNodeId				Integer variable to hold the node id
 \param		piOutNodeCount		Integer Pointer to NodeCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

//TODO: iMnNodeId not used. to be removed in header also
ocfmRetCode GetNodeCount(INT32 iMnNodeId, INT32* piOutNodeCount)
{

	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	//CNode objNode;
	NodeCollection *pobjNodeCollection = NULL;
	ocfmException objException;

	try
	{
		if (NULL == piOutNodeCount)
		{
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}
		else
		{
			//TODO: 'else' added. Nothing to do.
		}

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (pobjNodeCollection->GetNumberOfNodes() < 0)
		{
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added.  operation to be added
		}

		*piOutNodeCount = pobjNodeCollection->GetNumberOfNodes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetPIName
 
 This function returns the string with cn node id
 
 \param		iNodeID		Integer variable to hold the Node Id of a node

 \return	char*
 */
/*****************************************************************************/
//TODO: unused function
//TODO: change the returning char* to delete varNodeIdStr
char* GetPIName(INT32 iNodeID)
{
	char* varNodeIdStr = NULL;
	char* varIdAsci = new char[2];

	varNodeIdStr = new char[5];
	strcpy(varNodeIdStr, "CN");
	varIdAsci = IntToAscii(iNodeID, varIdAsci, 10);
	strcat(varNodeIdStr, varIdAsci);
	strcat(varNodeIdStr, ".");
	strcat(varNodeIdStr, "\0");

	delete[] varIdAsci;
	return varNodeIdStr;
}

/*****************************************************************************/
/**
 \brief		GetParameterAccess
 
 This function gets the parameter access code by providing the access type
 
 \param		varAccess	Character pointer to hold the access type

 \return	char*
 */
/*****************************************************************************/

char* GetParameterAccess(char* varAccess)
{
	if (NULL == varAccess)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	if (!strcmp(ConvertToUpper(varAccess), "READ"))
		return (char*) "ro\0";
	else if (!strcmp(ConvertToUpper(varAccess), "READWRITE"))
		return (char*) "rw\0";
	else
		return (char*) "xx";

}

/*****************************************************************************/
/**
 \brief		GetIndexCount

 This API shall be used to get the number of Indexes under a node. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutIndexCount' is filled with the Index count.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		piOutIndexCount		Integer Pointer to hold the  IndexCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexCount(INT32 iNodeID, NodeType varNodeType,
		INT32* piOutIndexCount)
{
	if (NULL == piOutIndexCount)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		Node objNode;
		NodeCollection *pobjNodeCollection = NULL;
		IndexCollection *pobjIndexCollection = NULL;
		//CIndex objIndex;
		bool bFlag = false;

		INT32 iNodePos;
		stErrorInfo = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);
		pobjIndexCollection = objNode.GetIndexCollection();

		*piOutIndexCount = pobjIndexCollection->GetNumberofIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexCount

 This API shall be used to get the number of SubIndexes under an Index. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutSubIndexCount' is filled with the SubIndex count.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varIndexID			Character pointer to hold the IndexID
 \param		piOutSubIndexCount	Integer Pointer to hold the IndexCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexCount(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, INT32* piOutSubIndexCount)
{
	if ((NULL == varIndexID) || (NULL == piOutSubIndexCount))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	Node objNode;
	NodeCollection *pobjNodeCollection = NULL;
	IndexCollection *pobjIndexCollection = NULL;
	Index objIndex;
	Index* pobjSubIndex = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos;

	try
	{
		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (stErrorInfo.code == 0)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		objIndex.SetNodeID(objNode.GetNodeId());
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		pobjSubIndex = pobjIndexCollection->GetIndex(iIndexPos);

		*piOutSubIndexCount = pobjSubIndex->GetNumberofSubIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		LoadObjectDictionary

 This API shall be used to load the predefined object dictionary xml. This API loads the objects that the tool will refer to when adding/editing an Index and/or a SubIndex.

 \param		varFileName		Char pointer  to hold the path of the file [XDD or XDC]

 \return	void
 */
/*****************************************************************************/

void LoadObjectDictionary(char* varFileName)
{

	xmlTextReaderPtr pxReader;

	ObjectDictionary* objDict;
	objDict = ObjectDictionary::GetObjDictPtr();
	/*
	 xmlTextReaderPtr xmlReaderForFile (const char * filename, const char * encoding, int options)
	 Returns: the new reader or NULL in case of error.
	 */

	pxReader = xmlReaderForFile(varFileName, NULL, 0);
	if (pxReader != NULL)
	{
		INT32 iRetVal = 0;
		/*
		 int xmlTextReaderRead(xmlTextReaderPtr reader)
		 Return:   1 if the node was read successfully, 0 if there is no more nodes to read, or -1 in case of error
		 */
		iRetVal = xmlTextReaderRead(pxReader);
		while (iRetVal == 1)
		{
			//const xmlChar* pxcName	= NULL;
			//const xmlChar* pxcValue	= NULL;

			//pxcName = xmlTextReaderConstName(pxReader);

			//pxcValue = xmlTextReaderConstValue(pxReader);
			objDict->ProcessObjectDictionary(pxReader);
			iRetVal = xmlTextReaderRead(pxReader);
		}
		if (iRetVal != 0)
		{
			//continue with process
		}
	}
	xmlCleanupParser();
	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();
}

/*****************************************************************************/
/**
 \brief		GetNodeAttributesbyNodePos

 This API shall be used to get the node attributes [NodeID, NodeName, Stationtype, Forcedcycle_value, Forcedcycle_flag] of a node given the position of the node under a MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the attributes will be filled with the corresponding values in the parameter variables.


 \param		iNodePos			Integer variable to hold the position of the node
 \param		piOutNodeID			Integer Pointer to hold the NodeID
 \param		piOutNodeName		Character pointer to hold the NodeName
 \param		eOutStationType		Character pointer to hold the station type
 \param		varOutForcedCycle	Character pointer to hold the forced cycle value
 \param		bForcedCycleFlag	boolean pointer to hold the forced cycle flag

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetNodeAttributesbyNodePos(INT32 iNodePos, INT32* piOutNodeID,
		char* piOutNodeName, StationType* eOutStationType,
		char* varOutForcedCycle, bool* bForcedCycleFlag)
{
	if ((NULL == piOutNodeID) || (NULL == piOutNodeName)
			|| (NULL == eOutStationType) || (NULL == varOutForcedCycle)
			|| (NULL == bForcedCycleFlag))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iTempNodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		stErrorInfo = GetNodeCount(MN_NODEID, &iTempNodeCount);

		if (iNodePos > iTempNodeCount)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		Node objNode;
		NodeCollection *pobjNodeCollection;
		//CIndexCollection *pobjIndexCollection;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

		//pobjIndexCollection = objNode.getIndexCollection();

		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		*piOutNodeID = objNode.GetNodeId();
		if (objNode.GetNodeName() != NULL)
			strcpy(piOutNodeName, objNode.GetNodeName());
		else
			piOutNodeName = NULL;

		if (objNode.GetForcedCycle() != NULL)
			strcpy(varOutForcedCycle, objNode.GetForcedCycle());
		else
			varOutForcedCycle = NULL;

		*eOutStationType = objNode.GetStationType();
		*bForcedCycleFlag = objNode.GetForceCycleFlag();

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetIndexIDbyIndexPos

 This API shall be used to get the Index ID  of an Index, given the position of the Index under a node and the Node ID. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the Index ID is filled into the 'piOutIndexID'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		iIndexPos		Integer variable to hold the index position
 \param		varOutIndexID	Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexIDbyIndexPos(INT32 iNodeID, NodeType varNodeType,
		INT32 iIndexPos, char* varOutIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	Node objNode;
	NodeCollection *pobjNodeCollection;
	IndexCollection *pobjIndexCollection;
	Index* pobjIndex;

	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.GetIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->GetNumberofIndexes();

		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);

		if (pobjIndex->GetIndexValue() != NULL)
			strcpy(varOutIndexID, (char *) pobjIndex->GetIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetIndexIDbyPositions

 This API shall be used to get the Index ID  of an Index given the position of the  Index under a node and the position of the Node. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the Index ID is filled into the 'piOutIndexID'.

 \param		iNodePos		Integer variable to hold the position of the node
 \param		iIndexPos		Integer variable to hold the position of the Index
 \param		varOutIndexID	Character Pointer to  hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexIDbyPositions(INT32 iNodePos, INT32 iIndexPos,
		char* varOutIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	Node objNode;
	NodeCollection *pobjNodeCollection;
	IndexCollection *pobjIndexCollection;
	Index* pobjIndex;

	try
	{
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->GetNumberOfNodes();

		if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if (iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.GetIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->GetNumberofIndexes();

		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);

		if (pobjIndex->GetIndexValue() != NULL)
			strcpy(varOutIndexID, (char *) pobjIndex->GetIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexIDbySubIndexPos

 This API shall be used to get the SubIndex ID of a SubIndex given the position of the  SubIndex under an Index, the Index ID and the Node ID. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the SubIndex ID is filled into the 'piOutSubIndexID'.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node
 \param		varIndexID			Character pointer to hold the IndexID
 \param		iSubIndexPos		Integer variable to hold the position of SubIndex
 \param		varOutSubIndexID		Character Pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 iNodeID, NodeType varNodeType,
		char* varIndexID, INT32 iSubIndexPos, char* varOutSubIndexID)
{
	Node objNode;
	NodeCollection *pobjNodeCollection;
	IndexCollection *pobjIndexCollection;
	//CIndex objIndex;
	Index* pobjSubIndex;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos;
	try
	{

		stErrorInfo = IfIndexExists(iNodeID, varNodeType, varIndexID,
				&iIndexPos);
		if (stErrorInfo.code == 0)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(varNodeType, iNodeID);

		pobjIndexCollection = objNode.GetIndexCollection();
		pobjSubIndex = pobjIndexCollection->GetIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjSubIndex->GetNumberofSubIndexes();
		if (iTempSubIndexCount == 0)
		{
			ocfmException objException;         // = new ocfmException;
			objException.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else if (iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;         // = new ocfmException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex *pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjSubIndex->GetSubIndex(iSubIndexPos);

		if (pobjSubIndexPtr->GetIndexValue() != NULL)
			strcpy(varOutSubIndexID, (char *) pobjSubIndexPtr->GetIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexIDbyPositions

 This API shall be used to get the SubIndex ID of a SubIndex given the position of the  SubIndex under an Index, the position of the Index under the node and the position of the node under the MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the SubIndex ID is filled into the 'piOutSubIndexID'.

 \param		iNodePos			Integer variable to hold the position of the node
 \param		iIndexPos			Integer variable to hold the position of the Index
 \param		iSubIndexPos		Integer variable to hold the SubIndex position
 \param		varOutSubIndexID		Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexIDbyPositions(INT32 iNodePos, INT32 iIndexPos,
		INT32 iSubIndexPos, char* varOutSubIndexID)

{
	Node objNode;
	NodeCollection *pobjNodeCollection = NULL;
	IndexCollection *pobjIndexCollection = NULL;
	//CIndex objIndex;
	Index* pobjIndex = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	try
	{
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->GetNumberOfNodes();

		if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if (iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->GetNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.GetIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->GetNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->GetIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjIndex->GetNumberofSubIndexes();
		if (iTempSubIndexCount == 0)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else if (iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjIndex->GetSubIndex(iSubIndexPos);

		if (pobjSubIndexPtr->GetIndexValue() != NULL)
			strcpy(varOutSubIndexID, (char *) pobjSubIndexPtr->GetIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteNodeObjDict

 This API shall be used to delete the object dictionary of a node given its respective node ID and node type. This API will not delete the node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType	Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteNodeObjDict(INT32 iNodeID, NodeType varNodeType)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;

	try
	{
		ocfmException ex;
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);

		if (stErrorInfo.code == 0 && bFlag == true)
		{
		}
		else
		{
			ex.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw ex;
		}
		//CNode objNode;
		Node* pobjNode = NULL;
		NodeCollection *pobjNodeCollection = NULL;
		IndexCollection *pobjIndexCollection = NULL;
		DataTypeCollection *pobjDataTypeCollection = NULL;
		Index objIndex;
		//CSubIndex pobjSubIndex;
		NetworkManagement *pobjNetworkManagement = NULL;
		ApplicationProcess *pobjApplicationProcess = NULL;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw ex;
		}
		pobjNode = pobjNodeCollection->GetNodePtr(varNodeType, iNodeID);
		//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID); // (varNodeType, iNodeID);
		objIndex.SetNodeID(pobjNode->GetNodeId());
		//objSubIndex.SetNodeID(objNode->GetNodeId());
		pobjDataTypeCollection = pobjNode->GetDataTypeCollection();

		pobjIndexCollection = pobjNode->GetIndexCollection();
		pobjNetworkManagement = pobjNode->GetNetworkManagement();
		pobjApplicationProcess = pobjNode->GetApplicationProcess();
		if ((NULL == pobjApplicationProcess) || (NULL == pobjIndexCollection)
				|| (NULL == pobjNetworkManagement)
				|| (NULL == pobjDataTypeCollection))
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw ex;
		}
		// Delete IndexCollection

		Index* pobjIndexPtr = NULL;
		INT32 iNofIndexes = pobjIndexCollection->GetNumberofIndexes();

		for (INT32 i = iNofIndexes; i < 0; i--)
		{
			pobjIndexPtr = pobjIndexCollection->GetIndex(i);
			if (pobjIndexPtr->GetNumberofSubIndexes() > 0)
			{
				pobjIndexPtr->DeleteSubIndexCollection();
			}
		}

		pobjIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		pobjDataTypeCollection->DeleteDataTypeCollection();
		//Delete Network management collectionObj
		pobjNetworkManagement->DeleteFeatureCollections();
		//Delete ComplexDataTypeCollection
		pobjApplicationProcess->DeleteParameterCollection();
		pobjApplicationProcess->DeleteComplexDataTypeCollection();
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* exCatch)
	{
		return exCatch->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SaveProject

 This API shall be used to save the project. This API calls the SaveNode API recursively and saves all the nodes under the project and also saves a project file 'ProjectName.oct' in the project location.

 \param		varProjectPath		Character pointer to hold the Project Path
 \param		varProjectName		Character pointer to hold the Project name

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SaveProject(char* varProjectPath, char* varProjectName)
{
	Node objNode;
	NodeCollection *pobjNodeCollection = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	char* varTempPath;
	varTempPath = new char[(strlen(varProjectPath)) + (strlen(varProjectName))
			+ strlen("cdc_xap") + ALLOC_BUFFER];

	try
	{
		char* varTempPjtName;
		struct stat stFileInfo;
		INT32 iIntStat;

		varTempPjtName = new char[300];
		sprintf(varTempPjtName, "%s%s/%s.oct", varProjectPath, varProjectName,
				varProjectName);

		iIntStat = stat(varTempPjtName, &stFileInfo);
		delete[] varTempPjtName;
		if (iIntStat == 0)
		{
#if defined DEBUG
			cout << "\n\n\nProject File Already exists\n" << endl;
#endif
		}
		else
		{
#if defined(_WIN32) && defined(_MSC_VER)
			{
				sprintf(varTempPath, "%s\\%s", varProjectPath, varProjectName);
				_mkdir(varTempPath);
			}
#else
			{
				sprintf(varTempPath, "%s/%s", varProjectPath, varProjectName);
				mkdir(varTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			}
#endif
		}

#if defined(_WIN32) && defined(_MSC_VER)
		{
			sprintf(varTempPath, "%s\\%s\\%s", varProjectPath, varProjectName,
					"cdc_xap");
			_mkdir(varTempPath);
		}
#else
		{
			sprintf(varTempPath, "%s/%s/%s", varProjectPath, varProjectName, "cdc_xap");
			mkdir(varTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		}
#endif

		SaveProjectXML(varProjectPath, varProjectName);
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (pobjNodeCollection == NULL)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->GetNumberOfNodes() > 0)
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < pobjNodeCollection->GetNumberOfNodes();
					iLoopCount++)
			{
				objNode = pobjNodeCollection->GetNodebyCollectionIndex(
						iLoopCount);
				char* varFileName;
				varFileName = new char[MAX_FILE_PATH_SIZE];
#if defined(_WIN32) && defined(_MSC_VER)
				{
					sprintf(varTempPath, "%s\\%s\\octx", varProjectPath,
							varProjectName);
					struct stat stFileInfo;

					iIntStat = stat(varTempPath, &stFileInfo);
					if (iIntStat == 0)
					{

#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						_mkdir(varTempPath);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(varFileName, "%s\\%d.octx", varTempPath,
							objNode.GetNodeId());
				}
#else
				{
					struct stat stFileInfo;

					sprintf(varTempPath, "%s/%s/octx", varProjectPath, varProjectName);
					iIntStat = stat(varTempPath, &stFileInfo);
					if(iIntStat == 0)
					{
#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						mkdir(varTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(varFileName, "%s/%d.octx", varTempPath, objNode.GetNodeId());
				}
#endif				
				SaveNode(varFileName, objNode.GetNodeId(),
						objNode.GetNodeType());
				delete[] varFileName;

			}
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
	} catch (ocfmException* ex)
	{
		delete[] varTempPath;
		return ex->_ocfmRetCode;
	}
	delete[] varTempPath;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetMNPDOSubIndex

 This function sets the MN pdo actual value calculated from the MNPdoVariable struct generated by the Processpdonodes amd the iPrevSubIndex value is updated

 \param		stMNPdoVar		Struct variable of type MNPdoVariable
 \param		iPrevSubIndex	Integer variable to update the previous subindex used
 \param		pobjIdx			Class pointer of the class CIndex of the CN's PDO index
 \param		varMNIndex		Character pointer to hold the MN index value
 \param		iPrevSize		Integer variable to update the previous size used

 \return	void
 */
/*****************************************************************************/

void GetMNPDOSubIndex(MNPdoVariable stMNPdoVar, INT32& iPrevSubIndex,
		Index* pobjIdx, char* varMNIndex, INT32 iPrevSize)
{
	if ((NULL == pobjIdx) || (NULL == varMNIndex))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	SubIndex* pobjSubIndex = NULL;
	char* varIdx = new char[3];
	/* Set the MN's PDO subIndex*/
	iPrevSubIndex = iPrevSubIndex + 1;
	varIdx = IntToAscii(iPrevSubIndex, varIdx, 16);
	varIdx = PadLeft(varIdx, '0', 2);

#if defined DEBUG
	cout << " idx" << varIdx << endl;
	cout << " prevsubindex" << iPrevSubIndex << endl;
#endif
	if (CheckIfSubIndexExists(MN_NODEID, MN, varMNIndex, varIdx))
	{
		pobjSubIndex = pobjIdx->GetSubIndexbyIndexValue(varIdx);
	}
	else
	{
		AddSubIndex(MN_NODEID, MN, varMNIndex, varIdx);
		pobjSubIndex = pobjIdx->GetSubIndexbyIndexValue(varIdx);
	}
	delete[] varIdx;
	if (NULL == pobjSubIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_UNKNOWN);
		cout << "pobjSubIndex NULL:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	/* Calculate the actual value of MN PDO */
	char* varActValue = new char[20];
	strcpy(varActValue, SubString(stMNPdoVar.Value, 0, 6));
	char* varOffset = new char[5];

	varOffset = IntToAscii(iPrevSize, varOffset, 16);
	varOffset = PadLeft(varOffset, '0', 4);
	strcpy(varOffset, ConvertToUpper(varOffset));
	strcat(varActValue, varOffset);
	/* Add reserve byte*/
	strcat(varActValue, "00");

	strcat(varActValue, stMNPdoVar.SubIndex);
	strcat(varActValue, stMNPdoVar.Index);

	pobjSubIndex->SetActualValue(varActValue);
	#if defined DEBUG
	cout<<"Actual Value"<<varActValue<<endl;
	#endif
	AddPDOIndexsToMN(stMNPdoVar.Index, stMNPdoVar.SubIndex, stMNPdoVar.pdoType);
	delete[] varOffset;
	delete[] varActValue;
}

/*****************************************************************************/
/**
 \brief		SetSIdxValue

 This function sets the actual value to the sub index in the index specified

 \param		varIdx				Character pointer to hold the index id
 \param		varSIdx				Character pointer to hold the Sub index id
 \param		varvalue			Character pointer to hold the actual value
 \param		pobjIdxCol			Class pointer of the class CIndexCollection
 \param		iNodeId				Integer variable to hold the node id
 \param		varNodeType			Enum of type ENodeType to hold node type
 \param		setDefaultValue		Boolean flag to set for Default Value

 \return	void
 */
/*****************************************************************************/

void SetSIdxValue(char* varIdx, char* varSIdx, char* varvalue,
		IndexCollection *pobjIdxCol, INT32 iNodeId, NodeType varNodeType,
		bool setDefaultValue)
{
	//TODO: varValue (3rd parameter) to be null checked. Empty value sent in generateotherMNindexes function
	if ((NULL == varIdx) || (NULL == varSIdx) || (NULL == pobjIdxCol))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	Index *pobjIndex = NULL;
	SubIndex* pobjSIdx = NULL;
	ocfmRetCode stRetInfo;

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varIdx);
#if defined DEBUG	
	cout << "sidx idx" << varIdx << varSIdx << endl;

#endif
	if (CheckIfSubIndexExists(iNodeId, varNodeType, varIdx, varSIdx))
	{
#if defined DEBUG	
		cout << "subindex fetched" << endl;
#endif			
		pobjSIdx = pobjIndex->GetSubIndexbyIndexValue(varSIdx);
		if (setDefaultValue)
			pobjSIdx->SetActualValue((char*) pobjSIdx->GetDefaultValue());
		else
			pobjSIdx->SetActualValue(varvalue);
		pobjSIdx->SetFlagIfIncludedCdc(TRUE);
	}
	else
	{
#if defined DEBUG	
		cout << "call addindex" << endl;
#endif
		stRetInfo = AddSubIndex(iNodeId, varNodeType, varIdx, varSIdx);
#if defined DEBUG	
		cout << "retcode" << stRetInfo.code << endl;
		cout << "Index sidx added" << varIdx << varSIdx;
#endif
		pobjSIdx = pobjIndex->GetSubIndexbyIndexValue(varSIdx);
		pobjSIdx->SetFlagIfIncludedCdc(TRUE);
		if (setDefaultValue)
			pobjSIdx->SetActualValue((char*) pobjSIdx->GetDefaultValue());
		else
		{
#if defined DEBUG	
			cout << "value" << varvalue << endl;
			cout << "subindex index" << pobjSIdx->GetIndexValue();
#endif
			pobjSIdx->SetActualValue(varvalue);
		}
	}
}

/*****************************************************************************/
/**
 \brief		AddForEachSIdx

 This function adds the specified attribute for each subindex 

 \param		varIdx				Character pointer to hold the index id
 \param		pobjIdxCol			Class pointer of the class CIndexCollection
 \param		iMNNodeID			Integer variable to hold the node id of the MN
 \param		varValue				Character pointer to hold the value to set to the subindex
 \param		bIsDefaultValueSet	Boolean flag to set for Default Value

 \return	void
 */
/*****************************************************************************/

void AddForEachSIdx(char *varIdx, IndexCollection *pobjIdxCol, INT32 iMNNodeID,
		char *varValue, bool bIsDefaultValueSet)
{
	NodeCollection *objNodeCol = NULL;
	Node objCNNode;
	//CIndex *pobjIndex;
	char *varSIdx = new char[3];
	char *varIndexNo = new char[3];
	char *varHexIndexNo = new char[5];
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	//pobjIndex = pobjIdxCol->getIndexbyIndexValue(varIdx);
	objNodeCol = NodeCollection::GetNodeColObjectPointer();

	// to doStack wasnt booting up with sub index 00 for 1C09, 1F26
	if ((strcmp("1C09", varIdx) == 0) || (strcmp("1F26", varIdx) == 0)
			|| (strcmp("1F8B", varIdx) == 0) || (strcmp("1F8D", varIdx) == 0)
			|| (strcmp("1F27", varIdx) == 0) || (strcmp("1F84", varIdx) == 0))
	{
		strcpy(varSIdx, "00");
		stRetInfo = DeleteSubIndex(iMNNodeID, MN, varIdx, varSIdx);
	}
	else
	{
		if (objNodeCol->GetCNNodesCount() != 0)
		{
			strcpy(varSIdx, "00");
			strcpy(varHexIndexNo, "0x");

			varIndexNo = IntToAscii(objNodeCol->GetCNNodesCount(), varIndexNo,
					16);
			varIndexNo = PadLeft(varIndexNo, '0', 2);
			strcat(varHexIndexNo, varIndexNo);
			SetSIdxValue(varIdx, varSIdx, varHexIndexNo, pobjIdxCol, iMNNodeID, MN,
					false);
		}
	}

	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
			iLoopCount++)
	{
		objCNNode = objNodeCol->GetNodebyCollectionIndex(iLoopCount);

		if (objCNNode.GetNodeType() == CN)
		{
			varSIdx = IntToAscii(objCNNode.GetNodeId(), varSIdx, 16);
			varSIdx = PadLeft(varSIdx, '0', 2);

			if (strcmp("1F84", varIdx) == 0)
			{
				Index* objCNIndex;
				//CSubIndex objCNSIdx;
				IndexCollection* pobjCNIdxCol;
				char* varMIndex = new char[INDEX_LEN];
				pobjCNIdxCol = objCNNode.GetIndexCollection();

				strcpy(varMIndex, "1000");
				objCNIndex = pobjCNIdxCol->GetIndexbyIndexValue(varMIndex);

				if (objCNIndex != NULL)
				{
					if (objCNIndex->GetActualValue() != NULL)
					{
						SetSIdxValue(varIdx, varSIdx,
								(char*) objCNIndex->GetActualValue(),
								pobjIdxCol, iMNNodeID, MN, false);
					}
					else
					{
						SetSIdxValue(varIdx, varSIdx,
								(char*) objCNIndex->GetActualValue(),
								pobjIdxCol, iMNNodeID, MN, true);
					}
				}
				delete[] varMIndex;

			}
			else
			{
				SetSIdxValue(varIdx, varSIdx, varValue, pobjIdxCol, iMNNodeID, MN,
						bIsDefaultValueSet);
			}

		}

	}
	delete[] varHexIndexNo;
	delete[] varIndexNo;
	delete[] varSIdx;
}

/*****************************************************************************/
/**
 \brief		AddOtherMNIndexes

 This function creates other required MN indexes

 \param		NodeID		Integer variable to hold the node id

 \return	ocfmRetCode
 */
/*****************************************************************************/
//TODO: unused function
ocfmRetCode AddOtherMNIndexes(INT32 NodeID)
{
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	char* varMNIndex = new char[INDEX_LEN];
	char* varSidx = new char[SUBINDEX_LEN];
	Index* pobjIndex;
	IndexCollection* pobjIdxCol;

	Node *pobjNode;
	NodeCollection *objNodeCollection = NULL;

	try
	{
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		pobjNode = objNodeCollection->GetNodePtr(MN, NodeID);
		pobjIdxCol = pobjNode->GetIndexCollection();

		/* Add 1006*/
		strcpy(varMNIndex, "1006");
#if defined DEBUG	
		cout << "string copied" << endl;
#endif
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
#if defined DEBUG	
		cout << "retcode" << stRetInfo.code << endl;
		cout << "1006 added" << endl;
#endif

		/* Set 5ms value*/
		SetIndexAttributes(MN_NODEID, MN, varMNIndex, (char*) "50000",
				(char*) "NMT_CycleLen_U32", TRUE);

		/* Add 1020*/
		strcpy(varMNIndex, "1020");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);

		/* Add 1020*/
		strcpy(varMNIndex, "1300");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		/* $:To do by M hard coded*/
		SetIndexAttributes(MN_NODEID, MN, varMNIndex, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);

		/* Add 1C02*/
		strcpy(varMNIndex, "1C02");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);

		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);
			/* Set subindex value 40 or 0000028 */
			strcpy(varSidx, "00");
			SetSIdxValue(varMNIndex, varSidx, (char*) "3", pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex added" << endl;
#endif

			strcpy(varSidx, "01");
			SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex 01 added" << endl;
#endif
			strcpy(varSidx, "02");
			SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);

			strcpy(varSidx, "03");
			SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);

		}

		/* Add 1C09*/
		strcpy(varMNIndex, "1C09");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(),
					(char*) "40", false);

		}

		/* Add 1F26*/
		strcpy(varMNIndex, "1F26");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);

			char* varVal = new char[8];
			iConfigDate = GetConfigDate();
			varVal = IntToAscii(iConfigDate, varVal, 10);
			//hexVal = padLeft(hexVal, '0' , 8);
			strcpy(varVal, ConvertToUpper(varVal));
			AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(), varVal,
					false);

			//AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->getNodeId(), hexVal, false);
			delete[] varVal;
		}

#if defined DEBUG	
		cout << "1F26 subidex added" << endl;
#endif
		/* Add 1F27*/
		strcpy(varMNIndex, "1F27");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);

			char* varVal = new char[50];
			iConfigTime = GetConfigTime();
			varVal = IntToAscii(iConfigTime, varVal, 10);
			strcpy(varVal, ConvertToUpper(varVal));
			AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(), varVal,
					false);
			delete[] varVal;
		}

		/* Add 1F84*/
		strcpy(varMNIndex, "1F84");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(),
					(char*) "", true);

		}

		/* Add 1F89*/
		strcpy(varMNIndex, "1F89");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);
			strcpy(varSidx, "02");
			SetSIdxValue(varMNIndex, varSidx, (char*) " ", pobjIdxCol,
					pobjNode->GetNodeId(), MN, true);

		}

		/* Add 1F8A*/
		strcpy(varMNIndex, "1F8A");
		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);
			strcpy(varSidx, "00");
			SetSIdxValue(varMNIndex, varSidx, (char*) " ", pobjIdxCol,
					pobjNode->GetNodeId(), MN, true);

			//Todo By M as subindex 01 shud be equal to 02, need to find the reason 
			strcpy(varSidx, "01");
			SetSIdxValue(varMNIndex, varSidx, (char*) "100000", pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);

			strcpy(varSidx, "02");
			SetSIdxValue(varMNIndex, varSidx, (char*) " ", pobjIdxCol,
					pobjNode->GetNodeId(), MN, true);

		}

		/* Add 1F98*/
		strcpy(varMNIndex, "1F98");

		stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS)
		{

			pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
			/* $:set Flag to true*/
			pobjIndex->SetFlagIfIncludedCdc(TRUE);

			strcpy(varSidx, "05");
			SetSIdxValue(varMNIndex, varSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol,
					pobjNode->GetNodeId(), MN, false);

		}

		delete[] varMNIndex;
		delete[] varSidx;
	} catch (ocfmException & ex)
	{
		delete[] varMNIndex;
		delete[] varSidx;
		return ex._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		GenerateMNOBD

 This API shall be used to auto generate the MN's object dictionary.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GenerateMNOBD()
{
	return (GenerateMNOBD(false));
}

/*****************************************************************************/
/**
 \brief		GenerateMNOBD

 This function automatically generates the MN's object dictionary 

 \param		IsBuild		Boolean value to set if this is a build process or not

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode GenerateMNOBD(bool IsBuild)
{

	Node *pobjMNNode = NULL;
	NodeCollection *pobjNodeCollection = NULL;
	IndexCollection *objMNIndexCol = NULL;

	char* varMNIndex = new char[INDEX_LEN];
	char* varIdx = new char[SUBINDEX_LEN];
	char* varMappIdx = new char[SUBINDEX_LEN];

	ocfmRetCode stRetInfo;

	ocfmException objocfmException;
	INT32 iOutPrevSubIndex = 0;
	INT32 iOutPrevSize = 0;
	INT32 iChainOutPrevSubIndex = 0;
	INT32 iChainOutPrevSize = 0;
	INT32 iInPrevSubIndex = 0;
	INT32 iInPrevSize = 0;
	INT32 iIndexPos = 0;
	INT32 iRxChannelCount = 0;
	INT32 iTxChannelCount = 0;
	INT32 iMaxNumberOfChannels = 0;

	StationType eCurrCNStation;
	try
	{
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		pobjMNNode = pobjNodeCollection->GetNodePtr(MN, MN_NODEID);
		if (pobjMNNode == NULL)
		{
			objocfmException.OCFMException(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw objocfmException;
		}
		else
		{
			/*Process PDO Nodes*/
			stRetInfo = ProcessPDONodes(IsBuild);
#if defined DEBUG
			cout << "PDO's in CN Nodes Processed" << endl;
#endif
			if (stRetInfo.code != OCFM_ERR_SUCCESS)
			{
				return stRetInfo;
			}

			//CNode objMNNode;
			//CNodeCollection *pobjNodeCollection;
			IndexCollection *pobjIndexCollection = NULL;
			//CIndex objIndex;

			//pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			//objMNNode = pobjNodeCollection->getNode(MN, MN_NODEID);
			//pobjIndexCollection = objMNNode.getIndexCollection();

			NetworkManagement *pobjNwManagement = NULL;
			pobjNwManagement = pobjMNNode->GetNetworkManagement();
			iMaxNumberOfChannels = pobjNwManagement->GetMaxPDOCount();
#if defined DEBUG
			cout << "Max Number Of TPDO Channels" << iMaxNumberOfChannels << endl;
#endif

			/* Delete the MN's old object dictionary*/
			pobjIndexCollection = pobjMNNode->GetIndexCollection();
//DO not delete and try to reset PDO indexes.
			//			pobjIndexCollection->DeletePDOs();
			ResetAllPdos(pobjMNNode->GetNodeId(), pobjMNNode->GetNodeType());
#if defined DEBUG
			//cout << "MN Node PDO's deleted" << endl;
#endif
			// Delete Process Image Objects
			pobjIndexCollection->DeletePIObjects();
#if defined DEBUG
			cout << "Deleted PI objects (Axxx indexes) in MN" << endl;
#endif
			// Autogenertate other indexs 
			AuotgenerateOtherIndexs(pobjMNNode);
			/* Add other Indexes than PDO*/
#if defined DEBUG
			cout << "Auotgenerated Other Indexs in MN" << endl;
#endif
		}

		bool bIsPresMN = false;
		bIsPresMN = IsPresMN();

		if (true == bIsPresMN)
		{
#if defined DEBUG
			cout << "PresMN: iTxChannelCount set to 1" << endl;
#endif
			iTxChannelCount = 1;
		}
		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		pobjMNNode = pobjNodeCollection->GetNodePtr(MN, MN_NODEID);

		INT32* pArrangedNodeIDbyStation = NULL;
		Node objNode;
		pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjNodeCollection->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = pobjNodeCollection->GetNodebyCollectionIndex(iLoopCount);

			if (objNode.GetNodeType() == CN)
			{

				char* varVersionNumber = new char[4];
				varVersionNumber[0] = 0;
				eCurrCNStation = objNode.GetStationType();

				pobjMNNode = pobjNodeCollection->GetNodePtr(MN, MN_NODEID);
				objMNIndexCol = pobjMNNode->GetIndexCollection();

				if (objNode.MNPDOOUTVarCollection.Count() != 0)
				{
					if (CHAINED != eCurrCNStation)
					{
						iOutPrevSubIndex = 0;
						iOutPrevSize = 0;
					}
					else
					{
						iOutPrevSubIndex = iChainOutPrevSubIndex;
						iOutPrevSize = iChainOutPrevSize;
					}
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					Index* pobjIndex;
					iIndexPos = 0;
					char* varMappNodeID = new char[SUBINDEX_LEN];
					strcpy(varMNIndex, "18");
					if (CHAINED != eCurrCNStation)
					{
						varIdx = IntToAscii(iTxChannelCount, varIdx, 16);
						iTxChannelCount++;

						varIdx = PadLeft(varIdx, '0', 2);
						varMNIndex = strcat(varMNIndex, varIdx);
						stRetInfo = IfIndexExists(MN_NODEID, MN, varMNIndex, &iIndexPos);
						if (stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							#if defined DEBUG
								cout<<"AddIndex: "<<varMNIndex<<__LINE__<<endl;
							#endif
							stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
						}

						//to write cn node id in 18XX/01
						varMappNodeID = IntToAscii(objNode.GetNodeId(),
								varMappNodeID, 10);
					}
					else
					{
						//1800 is used of PRes chained station
						strcpy(varMNIndex, (char *) "1800");
						strcpy(varIdx, (char *) "00");
						stRetInfo = IfIndexExists(MN_NODEID, MN, varMNIndex,
								&iIndexPos);
						if (stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							#if defined DEBUG
								cout<<"AddIndex: "<<varMNIndex<<__LINE__<<endl;
							#endif
							stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
						}

						//to write 0 in 18XX/01 to indicate PRes MN
						strcpy(varMappNodeID, (char *) "0x0");
					}
					/* set bFlag to true for 1800*/
					pobjIndex = objMNIndexCol->GetIndexbyIndexValue(varMNIndex);

					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.OCFMException(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					char* varSidx = new char[SUBINDEX_LEN];
					strcpy(varSidx, "01");
					SetSubIndexAttributes(MN_NODEID, MN, varMNIndex, varSidx,
							varMappNodeID, (char*) "NodeID_U8", TRUE);
					delete[] varMappNodeID;

					GetSubIndexAttributes(objNode.GetNodeId(), CN,
							(char*) "1400", (char*) "02", ACTUALVALUE,
							varVersionNumber);
					if ((NULL == varVersionNumber)
							|| (strcmp(varVersionNumber, "") == 0))
					{
						strcpy(varVersionNumber, "0x0");
					}
					SetSubIndexAttributes(MN_NODEID, MN, varMNIndex,
							(char*) "02", varVersionNumber,
							(char*) "MappingVersion_U8", TRUE);

					strcpy(varMNIndex, "1A");
					strcat(varMNIndex, varIdx);
					/* Set the MN's PDO Index*/
					iIndexPos = 0;
					stRetInfo = IfIndexExists(MN_NODEID, MN, varMNIndex,
							&iIndexPos);
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						#if defined DEBUG
						cout<<"AddIndex: "<<varMNIndex<<__LINE__<<endl;
						#endif
						stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
					}

					pobjIndex->SetFlagIfIncludedCdc(TRUE);

					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.OCFMException(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}
					INT32 iLoopCount = 0;
					for (iLoopCount = 0;
							iLoopCount < objNode.MNPDOOUTVarCollection.Count();
							iLoopCount++)
					{
						MNPdoVariable stMNPdoVar;
						stMNPdoVar = objNode.MNPDOOUTVarCollection[iLoopCount];
						pobjIndex = objMNIndexCol->GetIndexbyIndexValue(
								varMNIndex);

						if (pobjIndex != NULL)
						{
							pobjIndex->SetFlagIfIncludedCdc(TRUE);
							GetMNPDOSubIndex(stMNPdoVar, iOutPrevSubIndex,
									pobjIndex, varMNIndex, iOutPrevSize);
							iOutPrevSize = iOutPrevSize + stMNPdoVar.DataSize;
						}
					}
					SubIndex *pbMnPdoSidx = NULL;
					pbMnPdoSidx = pobjIndex->GetSubIndexbyIndexValue((char*)"00");
					if(NULL != pbMnPdoSidx)
					{
						char *temp = new char[INDEX_LEN];
						temp = IntToAscii(iLoopCount, temp, 10);
						#if defined DEBUG
							cout<<"Setting Actual Value: "<<temp<<" in index: "<<pobjIndex->GetIndexValue()<<endl;
						#endif
						//itoa(PdoOutCount, temp, 16);
						pbMnPdoSidx->SetActualValue(temp);
						delete [] temp;
					}

					if (CHAINED == eCurrCNStation)
					{
						iChainOutPrevSubIndex = iOutPrevSubIndex;
						iChainOutPrevSize = iOutPrevSize;
					}
				}

				if (objNode.MNPDOINVarCollection.Count() != 0)
				{
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					Index* pobjIndex;
					strcpy(varMNIndex, "14");
					varIdx = IntToAscii(iRxChannelCount, varIdx, 16);
					iRxChannelCount++;

					varIdx = PadLeft(varIdx, '0', 2);
					varMNIndex = strcat(varMNIndex, varIdx);
					stRetInfo = IfIndexExists(MN_NODEID, MN, varMNIndex, &iIndexPos);
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						#if defined DEBUG
							cout<<"AddIndex: "<<varMNIndex<<__LINE__<<endl;
						#endif
					stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
						/* set bFlag to true for 1800*/
					}
					pobjIndex = objMNIndexCol->GetIndexbyIndexValue(varMNIndex);
					if (pobjIndex != NULL)
						pobjIndex->SetFlagIfIncludedCdc(TRUE);

					iInPrevSubIndex = 0;
					iInPrevSize = 0;
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.OCFMException(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					varMappIdx = IntToAscii((objNode.GetNodeId()), varMappIdx,
							10);
					char* varSidx = new char[SUBINDEX_LEN];
					strcpy(varSidx, "01");
					SetSubIndexAttributes(MN_NODEID, MN, varMNIndex, varSidx,
							varMappIdx, (char*) "NodeID_U8", TRUE);

					GetSubIndexAttributes(objNode.GetNodeId(), CN,
							(char*) "1800", (char*) "02", ACTUALVALUE,
							varVersionNumber);
					if ((NULL == varVersionNumber)
							|| (strcmp(varVersionNumber, "") == 0))
					{
						strcpy(varVersionNumber, "0x0");
					}
					SetSubIndexAttributes(MN_NODEID, MN, varMNIndex,
							(char*) "02", varVersionNumber,
							(char*) "MappingVersion_U8", TRUE);
					delete[] varSidx;

					strcpy(varMNIndex, "16");
					strcat(varMNIndex, varIdx);
					/* Set the MN's PDO Index*/
					stRetInfo = IfIndexExists(MN_NODEID, MN, varMNIndex, &iIndexPos);
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						cout<<"AddIndex: "<<varMNIndex<<__LINE__<<endl;
						stRetInfo = AddIndex(MN_NODEID, MN, varMNIndex);
						if (stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.OCFMException(stRetInfo.code);
							//delete[] pArrangedNodeIDbyStation; no new so no delete
							throw objocfmException;
						}
					}

					pobjIndex = objMNIndexCol->GetIndexbyIndexValue(varMNIndex);
					INT32 iLoopCount = 0;
					for (iLoopCount = 0;
							iLoopCount < objNode.MNPDOINVarCollection.Count();
							iLoopCount++)
					{
						MNPdoVariable stMNPdoVar;
						stMNPdoVar = objNode.MNPDOINVarCollection[iLoopCount];
						pobjIndex = objMNIndexCol->GetIndexbyIndexValue(
								varMNIndex);
						pobjIndex->SetFlagIfIncludedCdc(TRUE);
						GetMNPDOSubIndex(stMNPdoVar, iInPrevSubIndex, pobjIndex,
								varMNIndex, iInPrevSize);
						iInPrevSize = iInPrevSize + stMNPdoVar.DataSize;
					}

					SubIndex *pbMnPdoSidx = NULL;
					pbMnPdoSidx = pobjIndex->GetSubIndexbyIndexValue((char*)"00");
					if(NULL != pbMnPdoSidx)
					{
						char *temp = new char[INDEX_LEN];
						temp = IntToAscii(iLoopCount, temp, 10);
						#if defined DEBUG
							cout<<"Setting Actual Value: "<<temp<<" in index: "<<pobjIndex->GetIndexValue()<<endl;
						#endif
						//itoa(PdoInCount, temp, 16);
						pbMnPdoSidx->SetActualValue(temp);
						delete [] temp;
					}
				}
				delete[] varVersionNumber;
			}
		}
		SetPresMNNodeAssigmentBits();

		delete[] pArrangedNodeIDbyStation;
		SetFlagForRequiredMNIndexes(MN_NODEID);
	}

	catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}

	if (iTxChannelCount > iMaxNumberOfChannels)
	{
		stRetInfo.code = OCFM_ERR_EXCESS_CHANNEL;
	}
	else
	{
		stRetInfo.code = OCFM_ERR_SUCCESS;
	}
	delete[] varMNIndex;
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		CheckIfOffsetUsed

 This function checks for if the offset is used or not.

 \param		iOffset			Integer variable to hold the offset value
 \param		varPdoType		Enum of type PDOType to hold pdo type

 \return	void
 */
/*****************************************************************************/
//TODO: unused function
void CheckIfOffsetUsed(INT32& iOffset, PDOType varPdoType)
{
	NodeCollection* pobjNodeCol;
	pobjNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode;

	for (INT32 iNodeIndex = 0; iNodeIndex < pobjNodeCol->GetNumberOfNodes();
			iNodeIndex++)
	{
		objNode = pobjNodeCol->GetNodebyCollectionIndex(iNodeIndex);
		if (objNode.GetNodeType() == CN)
		{
			if (varPdoType == PDO_RPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOOUTVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOOUTVarCollection[iLoopCount].Offset
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOOUTVarCollection[iLoopCount].DataSize
												/ 8;
				}
			}
			else if (varPdoType == PDO_TPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOINVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOINVarCollection[iLoopCount].Offset
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOINVarCollection[iLoopCount].DataSize
												/ 8;
				}
			}
			else
			{
				//TODO: 'else' added. operation to be specified.
			}
		}
	}

}

/*****************************************************************************/
/**
 \brief		ComputeOUTOffset

 This function computes and returns the OUTPUT offset for the process image variables

 \param		iDataSize		Integer variable to hold the data size
 \param		varPdoType		Enum of type PDOType to hold pdo type

 \return	INT32
 */
/*****************************************************************************/
//TODO: No need of varPdotype for size calculation. to be removed in header
INT32 ComputeOUTOffset(INT32 iDataSize, PDOType varPdoType)
{
	INT32 iOffset = 0;

	switch (iDataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
		iOffset = stSize8OUTOffset.currOffset;
		stSize8OUTOffset.currOffset = stSize8OUTOffset.currOffset + 1;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize16OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
			stSize16OUTOffset.currOffset = stSize16OUTOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (stSize32OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
			stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
		iOffset = stSize16OUTOffset.currOffset;
		stSize16OUTOffset.currOffset = stSize16OUTOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (stSize8OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
		}
		else
		{
			stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
			stSize8OUTOffset.currOffset = stSize16OUTOffset.currOffset;
		}

		/* if greater no change*/
		if (stSize32OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
			//
		}
		else
		{
			stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
			stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
		iOffset = stSize32OUTOffset.currOffset;
		stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize8OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
		{
			//
		}
		else
		{
			stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
			stSize8OUTOffset.currOffset = stSize32OUTOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize16OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
		{
		}
		else
		{
			stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
			stSize16OUTOffset.currOffset = stSize32OUTOffset.currOffset;

		}
		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		/*Unsigned64, Int64*/
		break;
	case 64:
		stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
		iOffset = stSize64OUTOffset.currOffset;
		stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		break;
	default:
		cout << "Undefined DataSize Encountered:" << __FUNCTION__ << endl;
		break;
	}
	return iOffset;
}

/*****************************************************************************/
/**
 \brief		ComputeINOffset

 This function computes and returns the INPUT offset for the process image variables

 \param		iDataSize		Integer variable to hold the data size
 \param		varPdoType		Enum of type PDOType to hold pdo type

 \return	INT32
 */
/*****************************************************************************/

//TODO: No need of varPdotype for size calculation. to be removed in header
INT32 ComputeINOffset(INT32 iDataSize, PDOType varPdoType)
{
	INT32 iOffset = 0;

	switch (iDataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
		iOffset = stSize8INOffset.currOffset;
		stSize8INOffset.currOffset = stSize8INOffset.currOffset + 1;
		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize16INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
			stSize16INOffset.currOffset = stSize16INOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (stSize32INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{

			stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
			stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;

		}

		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
		iOffset = stSize16INOffset.currOffset;
		stSize16INOffset.currOffset = stSize16INOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (stSize8INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
			stSize8INOffset.currOffset = stSize16INOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize32INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
			stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
		iOffset = stSize32INOffset.currOffset;
		stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize8INOffset.currOffset >= stSize32INOffset.currOffset)
		{
		}
		else
		{
			stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
			stSize8INOffset.currOffset = stSize32INOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize16INOffset.currOffset >= stSize32INOffset.currOffset)
		{
		}
		else
		{
			stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
			stSize16INOffset.currOffset = stSize32INOffset.currOffset;

		}
		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned64, Int64*/
	case 64:
		stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
		iOffset = stSize64INOffset.currOffset;
		stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		break;
	default:
		cout << "Undefined DataSize Encountered:" << __FUNCTION__ << endl;
		break;
	}
	return iOffset;
}

/*****************************************************************************/
/**
 \brief		FreeProjectMemory

 This API shall be used to free the memory occupied by the shared library objects for the current project& settings. After calling this API, the project configurations & setting are no more valid.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode FreeProjectMemory()
{
	NodeCollection *pobjNodeCollection = NULL;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	delete pobjNodeCollection;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		OpenProject

 This API shall be used to open a saved openCONFIGURATOR project. This API reads through the 'ProjectName'.oct file and from which reads the respective 'NodeNumber'.octx files in the project location and loads the shared library objects for the project.

 \param		varPjtPath				Character pointer to hold the project path
 \param		varProjectXmlFileName	Character pointer to hold the file name of the project xml

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode OpenProject(char* varPjtPath, char* varProjectXmlFileName)
{
	NodeCollection *pobjNodeCollection;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	xmlTextReaderPtr pxReader;
	INT32 iRetVal;
	char *varFileName = NULL;

#if defined DEBUG
	cout << "\nStrLen for FileName:"
	<< (strlen(varPjtPath) + strlen(varProjectXmlFileName) + 1) << endl;
#endif

	varFileName =
			new char[(strlen(varPjtPath) + strlen(varProjectXmlFileName) + 5)];
#if defined(_WIN32) && defined(_MSC_VER)
	{
		sprintf(varFileName, "%s\\%s", varPjtPath, varProjectXmlFileName);
	}
#else
	{
		sprintf(varFileName, "%s/%s", varPjtPath, varProjectXmlFileName);
	}
#endif

	pxReader = xmlReaderForFile(varFileName, NULL, 0);
	delete[] varFileName;
	try
	{
		if (pxReader != NULL)
		{
			iRetVal = xmlTextReaderRead(pxReader);
			while (iRetVal == 1)
			{
				ProcessProjectXML(pxReader, varPjtPath);
				iRetVal = xmlTextReaderRead(pxReader);
			}
			if (iRetVal != 0)
			{
				ocfmException objException;
#if defined DEBUG 
				cout << "\nOCFM_ERR_PARSE_XML\n" << endl;
#endif
				objException.OCFMException(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else
		{
#if defined DEBUG 
			cout << "\nOCFM_ERR_CANNOT_OPEN_FILE\n" << endl;
#endif
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		Node objNode;
		INT32 iNodeID;
		NodeType iNodeType;

		char* varPresTimeoutVal = new char[50];
		varPresTimeoutVal[0] = 0;

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjNodeCollection->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = pobjNodeCollection->GetNodebyCollectionIndex(iLoopCount);

			iNodeType = objNode.GetNodeType();
			iNodeID = objNode.GetNodeId();
			CopyPDODefToAct(iNodeID, iNodeType);
			CopyMNPropDefToAct(iNodeID, iNodeType);

			if ((iNodeID != MN_NODEID) && (MN != iNodeType))
			{
				char* strConvertedValue = NULL;
				strConvertedValue = new char[SUBINDEX_LEN];
				strConvertedValue = IntToAscii(iNodeID, strConvertedValue, 16);
				strConvertedValue = PadLeft(strConvertedValue, '0', 2);
				GetSubIndexAttributes(MN_NODEID, MN, (char*) "1f92",
						strConvertedValue, ACTUALVALUE, varPresTimeoutVal);
#if defined DEBUG
				cout << "Actual Value" << iNodeID << varPresTimeoutVal << endl;
#endif
				if (((NULL == varPresTimeoutVal)
						|| (strcmp(varPresTimeoutVal, "") == 0))
						|| (!(ValidateCNPresTimeout(strConvertedValue,
								varPresTimeoutVal))))
				{
					CalculateCNPollResponse(iNodeID, iNodeType);
				}

				delete[] strConvertedValue;
			}
		}
		delete[] varPresTimeoutVal;
	}

	catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		ProcessProjectXML

 Process the Node value, Name and its attributes

 \param		pxReader	xml pointer of type xmlTextReaderPtr
 \param		varPjtPath	Character pointer to hold the Project path

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode ProcessProjectXML(xmlTextReaderPtr pxReader, char* varPjtPath)
{
	const xmlChar *pxcName;

	pxcName = xmlTextReaderConstName(pxReader);
	if (pxcName == NULL)
	{
#if defined DEBUG
		cout << "\nGot NULL for Name\n" << endl;
#endif
	}
	//value = xmlTextReaderConstValue(pxReader);
	try
	{
		if (xmlTextReaderNodeType(pxReader) == 1)
		{
			// Check for openCONFIGURATOR Tag
			if (strcmp(((char*) pxcName), "openCONFIGURATOR") == 0)
			{
#if defined DEBUG
				cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					while (xmlTextReaderMoveToNextAttribute(pxReader))
					{
						// Call Check Version number Fn
						if (IfVersionNumberMatches(pxReader) == false)
						{
#if defined DEBUG
							cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH);
							throw objException;
						}
					}
				}
			}
			else if (strcmp(((char*) pxcName), "profile") == 0)
			{
#if defined DEBUG
				cout << "profile Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Auto") == 0)
			{
#if defined DEBUG
				cout << "Auto Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (SetProjectSettingsAuto(pxReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw objException;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Communication") == 0)
			{
#if defined DEBUG
				cout << "Communication Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (SetProjectSettingsCommunication(pxReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw objException;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "NodeCollection") == 0)
			{
#if defined DEBUG
				cout << "NodeCollection Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Node") == 0)
			{
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (GetandCreateNode(pxReader, varPjtPath) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw objException;
					}
					else
					{
						//ocfmRetCode stErrorInfo;
						//stErrorInfo.code = OCFM_ERR_SUCCESS;
						//return stErrorInfo;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
		}

	} catch (ocfmException* objocfmException)
	{
		return objocfmException->_ocfmRetCode;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetProjectSettingsAuto

 Gets the Auto info from the Pjt xml and stores in the object

 \param		pxReader	xml pointer of type xmlTextReaderPtr

 \return	BOOL

 \retval	TRUE		if successful
 \retval	FALSE		if there is already a message pending
 */
/*****************************************************************************/

bool SetProjectSettingsAuto(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	PjtSettings* pobjPjtSettings;
	pobjPjtSettings = PjtSettings::GetPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << pxcName << endl;
		cout << "\nValue:" << pxcValue << endl;
#endif
		if (strcmp(((char*) pxcName), "Generate") == 0)
		{
			if (strcmp(((char*) pxcValue), "YES") == 0)
				pobjPjtSettings->SetGenerateAttr(YES_AG);
			else if (strcmp(((char*) pxcValue), "NO") == 0)
				pobjPjtSettings->SetGenerateAttr(NO_AG);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "Save") == 0)
		{
			if (strcmp(((char*) pxcValue), "YES") == 0)
				pobjPjtSettings->SetSaveAttr(YES_AS);
			else if (strcmp(((char*) pxcValue), "PROMPT") == 0)
				pobjPjtSettings->SetSaveAttr(PROMPT_AS);
			else if (strcmp(((char*) pxcValue), "DISCARD") == 0)
				pobjPjtSettings->SetSaveAttr(DISCARD_AS);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "View") == 0)
		{
			if (strcmp(((char*) pxcValue), "General") == 0)
				pobjPjtSettings->SetViewMode(SIMPLE);
			else if (strcmp(((char*) pxcValue), "Advanced") == 0)
				pobjPjtSettings->SetViewMode(EXPERT);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "ExpertViewSelected") == 0)
		{
			if (strcmp(((char*) pxcValue), "true") == 0)
				pobjPjtSettings->SetExpertViewSelectedFlag(true);
			else if (strcmp(((char*) pxcValue), "false") == 0)
				pobjPjtSettings->SetExpertViewSelectedFlag(false);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else
		{
#if defined DEBUG
			cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
			return false;
		}
	}
	return true;
}

/*****************************************************************************/
/**
 \brief		SetProjectSettingsCommunication

 Gets the communication info from the Pjt xml and stores in the object

 \param		pxReader	xml pointer of type xmlTextReaderPtr

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool SetProjectSettingsCommunication(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	PjtSettings* pobjPjtSettings;
	pobjPjtSettings = PjtSettings::GetPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << pxcName << endl;
		cout << "\nValue:" << pxcValue << endl;
#endif
		if (strcmp(((char*) pxcName), "IP") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				pobjPjtSettings->SetPOWERLINKIP((char*) pxcValue);
			}
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Communication returning false"
				<< endl;
#endif
				return false;
			}

		}
		else
		{
#if defined DEBUG
			cout << "\nsetProjectSettings_Communication returning false"
			<< endl;
#endif
			return false;
		}
	}
	return true;
}

/*****************************************************************************/
/**
 \brief		GetandCreateNode

 Gets the Node properties from the Pjt xml and Creates the Nodes

 \param		pxReader		xml pointer of type xmlTextReaderPtr
 \param		varPjtPath		Character pointer to hold the Project path

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool GetandCreateNode(xmlTextReaderPtr pxReader, char* varPjtPath)
{
	const xmlChar *pxcName = NULL;
	const xmlChar *pxcValue = NULL;

	char* varNodeName = NULL;
	char* varXdcPath = NULL;
	char* varFileName = NULL;
	char* varForceCycleValue = NULL;

	INT32 iNodeID; //can't be initialised. and if it is not present false is returned.
	NodeType varNodeType; //can't be initialised. and if it is not present false is returned.
	bool bForceCycleFlag = false;
	StationType eStationType;
	ocfmRetCode stErrorInfo;

	eStationType = NORMAL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	//CPjtSettings* pobjPjtSettings;
	//pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
		{
			return false;
		}
		else
		{
			//TODO: 'else' added. operation to be specified.
		}

		if (strcmp(((char*) pxcName), "name") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				varNodeName = new char[strlen((char*) pxcValue) + ALLOC_BUFFER];
				strcpy((char*) varNodeName, (char*) pxcValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) pxcName), "NodeId") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				iNodeID = atoi((char*) pxcValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) pxcName), "NodeType") == 0)
		{
			if (strcmp(((char*) pxcValue), "MN") == 0)
			{
				varNodeType = MN;
#if defined DEBUG
				cout << "\nnodeType:" << varNodeType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "CN") == 0)
			{
				varNodeType = CN;
#if defined DEBUG
				cout << "\nnodeType:" << varNodeType << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) pxcName), "xdc") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				varXdcPath = new char[strlen((char*) pxcValue) + 1];
				strcpy((char*) varXdcPath, (char*) pxcValue);
#if defined DEBUG
				cout << "\nxdcPath:" << varXdcPath << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				varXdcPath = NULL;
			}
		}
		else if (strcmp(((char*) pxcName), "ForceCycleFlag") == 0)
		{
			if (strcmp(((char*) pxcValue), "true") == 0)
			{
				bForceCycleFlag = true;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "false") == 0)
			{
				bForceCycleFlag = false;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;
#endif
			}
			else
			{
				bForceCycleFlag = false;
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
			}
		}
		else if (strcmp(((char*) pxcName), "ForceCycle") == 0)
		{
			if ((char*) pxcValue != NULL && strcmp((char*) pxcValue, "") != 0)
			{
				varForceCycleValue = new char[strlen((char*) pxcValue)
						+ ALLOC_BUFFER];
				strcpy((char*) varForceCycleValue, (char*) pxcValue);
			}
			else
			{
				//TODO: 'else' added. operation to be specified.
			}
		}
		else if (strcmp(((char*) pxcName), "StationType") == 0)
		{
			if (strcmp(((char*) pxcValue), "Multiplexed") == 0)
			{
				eStationType = MULTIPLEXED;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "Chained") == 0)
			{
				eStationType = CHAINED;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "Normal") == 0)
			{
				eStationType = NORMAL;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
			}
		}
		else
		{
			//TODO: 'else' added. operation to be specified.
		}
	}
#if defined DEBUG
	cout << "\n\n\nCan Create Node\n\n" << endl;
	cout << "\n varNodeType:" << varNodeType << endl;
#endif
	//varNodeName 
	if (varNodeType == 1)
	{
		stErrorInfo = CreateNode(iNodeID, CN, varNodeName);
	}
	else if (varNodeType == 0)
	{
		stErrorInfo = CreateNode(iNodeID, MN, varNodeName);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	Node* objNode;
	NodeCollection *pobjNodeCollection;
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
	objNode = pobjNodeCollection->GetNodePtr(varNodeType, iNodeID);
	objNode->SetForceCycleFlag(bForceCycleFlag);

#if defined DEBUG
	cout << "\nCreateNode - stErrorInfo.code:" << stErrorInfo.code << endl;
#endif

	varFileName = new char[(strlen(varPjtPath) + strlen(varXdcPath) + 5)];
	sprintf(varFileName, "%s/%s", varPjtPath, varXdcPath);
	delete[] varXdcPath;
	if (varNodeType == 1)
	{
		stErrorInfo = ParseFile(varFileName, iNodeID, CN);
	}
	else if (varNodeType == 0)
	{
		stErrorInfo = ParseFile(varFileName, iNodeID, MN);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (varNodeType == 1 && varForceCycleValue != NULL)
	{
		objNode->SetForcedCycle(varForceCycleValue);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	objNode->SetStationType(eStationType);

	//    copyDefToAct(iNodeID, varNodeType);
	delete[] varFileName;
	delete[] varNodeName;
	return true;
}

/*****************************************************************************/
/**
 \brief		SaveProjectXML

 Saves the project details into the Project location

 \param		varProjectPath	Character pointer to hold the Project Path
 \param		varProjectName	Character pointer to hold the Project Name

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool SaveProjectXML(char* varProjectPath, char* varProjectName)
{

	PjtSettings* pobjPjtSettings;
	pobjPjtSettings = PjtSettings::GetPjtSettingsPtr();

	xmlTextWriterPtr pxtwWriter;
	xmlDocPtr pxdDoc;
	INT32 iBytesWritten;
	char* varFileName = NULL;

	/* Create a new XmlWriter for DOM, with no compression. */
	pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
	if (pxtwWriter == NULL)
	{
		printf("testXmlwriterDoc: Error creating the xml pxtwWriter\n");
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_CREATE_XML_WRITER_FAILED);
		throw objException;
	}
	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING,
			NULL);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_START_DOC_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteComment(pxtwWriter,
			BAD_CAST "This file was autogenerated by openCONFIGURATOR");
	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start openCONFIGURATOR Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "openCONFIGURATOR");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Version",
			BAD_CAST TOOL_VERSION);

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start profile Tag
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "profile");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Auto Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Auto");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}

	if (pobjPjtSettings->GetGenerateAttr() == NO_AG)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "Generate", BAD_CAST "NO");
	}
	else if (pobjPjtSettings->GetGenerateAttr() == YES_AG)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "Generate", BAD_CAST "YES");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->GetSaveAttr() == YES_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "YES");
	else if (pobjPjtSettings->GetSaveAttr() == PROMPT_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "PROMPT");
	else if (pobjPjtSettings->GetSaveAttr() == DISCARD_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "DISCARD");
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->GetViewMode() == SIMPLE)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View",
				BAD_CAST "General");
	}
	else if (pobjPjtSettings->GetViewMode() == EXPERT)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View",
				BAD_CAST "Advanced");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->GetExpertViewSelectedFlag() == true)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "true");
	}
	else if (pobjPjtSettings->GetExpertViewSelectedFlag() == false)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "false");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	// End Auto Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Communication Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "Communication");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "IP",
			BAD_CAST "0.0.0.0");

	// End Communication Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	// End profile Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}
	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start NodeCollection Tag
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "NodeCollection");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	Node objNode;
	NodeCollection *pobjNodeCollection = NULL;
	Index objIndex;
	objIndex.SetNodeID(objNode.GetNodeId());
	pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();

	for (INT32 nodeCount = 0;
			nodeCount < pobjNodeCollection->GetNumberOfNodes(); nodeCount++)
	{
		Node* pobjNode;

		pobjNode = pobjNodeCollection->GetNodebyColIndex(nodeCount);

		xmlTextWriterSetIndent(pxtwWriter, 1);

		// Start Node Tag		
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Node");

		if (iBytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;        // = new ocfmException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		char* varTempNodeName;
		varTempNodeName = new char[50];
		strcpy(varTempNodeName, (char*) pobjNode->GetNodeName());
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name",
				BAD_CAST varTempNodeName);

		char* varTempNodeID;
		varTempNodeID = new char[20];

		IntToAscii(pobjNode->GetNodeId(), varTempNodeID, 0);

		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "NodeId", BAD_CAST varTempNodeID);

		NodeType tmp_NodeType;
		tmp_NodeType = pobjNode->GetNodeType();

		if (tmp_NodeType == 0)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "NodeType", BAD_CAST "MN");
		if (tmp_NodeType == 1)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "NodeType", BAD_CAST "CN");

		char* varTempXdcName = new char[50];

		sprintf(varTempXdcName, "%s/%s.octx", "octx", varTempNodeID);
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "xdc",
				BAD_CAST varTempXdcName);
		if (pobjNode->GetForceCycleFlag() == true)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "true");
		}
		else if (pobjNode->GetForceCycleFlag() == false)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "false");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		if (pobjNode->GetForcedCycle() != NULL
				&& strcmp(pobjNode->GetForcedCycle(), ""))
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycle", BAD_CAST pobjNode->GetForcedCycle());
		}

		if (pobjNode->GetStationType() == NORMAL)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Normal");
		}
		else if (pobjNode->GetStationType() == MULTIPLEXED)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Multiplexed");
		}
		else if (pobjNode->GetStationType() == CHAINED)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Chained");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		// End Node Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;        // = new ocfmException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			delete[] varTempNodeName;
			delete[] varTempNodeID;
			delete[] varTempXdcName;
			throw objException;
		}

		delete[] varTempNodeName;
		delete[] varTempNodeID;
		delete[] varTempXdcName;
	}

	// End NodeCollection Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	// End openCONFIGURATOR Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
		ocfmException objException;        // = new ocfmException;
		objException.OCFMException(OCFM_ERR_XML_END_DOC_FAILED);
		throw objException;
	}

	xmlFreeTextWriter(pxtwWriter);
	varFileName = new char[MAX_FILE_PATH_SIZE];

#if defined(_WIN32) && defined(_MSC_VER)
	{

		sprintf(varFileName, "%s\\%s\\%s.oct", varProjectPath, varProjectName,
				varProjectName);
	}
#else
	{
		sprintf(varFileName, "%s/%s/%s.oct", varProjectPath, varProjectName, varProjectName);
	}
#endif
	xmlSaveFileEnc(varFileName, pxdDoc, MY_ENCODING);

	xmlFreeDoc(pxdDoc);

	delete[] varFileName;
	return true;
}

/*****************************************************************************/
/**
 \brief		CreateMNPDOVar

This function creates the MN pdo's in a collection used for process image manipulation

 \param		iOffset			Integer variable to hold the offset value
 \param		iDataSize		Integer variable to hold the size of the data
 \param		varDataType		Enum of type IEC_Datatype to hold datatype
 \param		varPdoType		Enum of type PDOType to hold pdo type
 \param		pobjNode		Class pointer of the class Node

 \return	void
 */
/*****************************************************************************/

void CreateMNPDOVar(INT32 iOffset, INT32 iDataSize, IEC_Datatype varDataType,
		PDOType varPdoType, Node *pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	MNPdoVariable objPDOvar;
	//CNodeCollection* pobjNodeCol;
	PIObject objpi;
	objpi.Index = NULL;
	objpi.SubIndex = NULL;
	//pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();

	/* Assign the PDO type*/
	objPDOvar.pdoType = varPdoType;
	objPDOvar.DataSize = iDataSize;
	/* Assign Index*/
	objPDOvar.Index = new char[4 + ALLOC_BUFFER];
	objPDOvar.SubIndex = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	switch (varDataType)
	{

	case USINT:
	case BITSTRING:
		PDODataType dt;
		switch (iDataSize)
		{
		case 8:
			dt = UNSIGNED8;
			break;
		case 16:
			dt = UNSIGNED16;
			break;
		case 32:
			dt = UNSIGNED32;
			break;
		case 64:
			dt = UNSIGNED64;
			break;
		default:
			break;
		}
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(dt, INPUT, iOffset, iDataSize);

		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(dt, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case SINT:
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(INTEGER8, INPUT, iOffset, iDataSize);
		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(INTEGER8, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case UINT:
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(UNSIGNED16, INPUT, iOffset, iDataSize);
		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(UNSIGNED16, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case INT:
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(INTEGER16, INPUT, iOffset, iDataSize);
		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(INTEGER16, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case UDINT:
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(UNSIGNED32, INPUT, iOffset, iDataSize);
		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(UNSIGNED32, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case DINT:
	case REAL:
		if (varPdoType == PDO_TPDO)
		{
			objpi = GetPIAddress(INTEGER32, INPUT, iOffset, iDataSize);
		}
		else if (varPdoType == PDO_RPDO)
		{
			objpi = GetPIAddress(INTEGER32, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case LINT:
	case LREAL:
		if(varPdoType == PDO_TPDO)
		{
			objpi =  GetPIAddress(INTEGER64, INPUT, iOffset, iDataSize);
		}
		else if(varPdoType == PDO_RPDO)
		{
			objpi =  GetPIAddress(INTEGER64, OUTPUT, iOffset, iDataSize);
		}
		break;
	case ULINT:
		if(varPdoType == PDO_TPDO)
		{
			objpi =  GetPIAddress(UNSIGNED64, INPUT, iOffset, iDataSize);
		}
		else if(varPdoType == PDO_RPDO)
		{
			objpi =  GetPIAddress(UNSIGNED64, OUTPUT, iOffset, iDataSize);
		}
		break;

		// Handled all values 
	case BOOL:
		cout << "Data type BOOL not handled" << endl;
		break;
	case BYTE:
		cout << "Data type BYTE not handled" << endl;
		break;
	case _CHAR:
		cout << "Data type _CHAR not handled" << endl;
		break;
	case DWORD:
		cout << "Data type DWORD not handled" << endl;
		break;
	case LWORD:
		cout << "Data type LWORD not handled" << endl;
		break;
	case STRING:
		cout << "Data type STRING not handled" << endl;
		break;
	case WSTRING:
		cout << "Data type WSTRING not handled" << endl;
		break;
	default:
		cout << "UnHandled Datatype encountered:" << varDataType << endl;
		break;
	}
#if defined DEBUG
	cout << __FUNCTION__ << " Ind:" << objpi.Index << " SubInd:"
	<< objpi.SubIndex << endl;
#endif
	strcpy(objPDOvar.Index, objpi.Index);
	strcpy(objPDOvar.SubIndex, objpi.SubIndex);
	/* Assign the value*/
	objPDOvar.Value = new char[10 + ALLOC_BUFFER];
	char* varPadBuff = new char[5 + ALLOC_BUFFER];
	varPadBuff = IntToAscii(iDataSize, varPadBuff, 16);
	varPadBuff = PadLeft(varPadBuff, '0', 4);
	strcpy(varPadBuff, ConvertToUpper(varPadBuff));
	strcpy(objPDOvar.Value, "0x");
	strcat(objPDOvar.Value, varPadBuff);
	/* Set the Offset*/
	strcat(objPDOvar.Value, "0000");
	/* Set the Reserved*/
	strcat(objPDOvar.Value, "00");

	pobjNode->AddMNPDOvar(objPDOvar, varPdoType);

}

/*****************************************************************************/
/**
 \brief		GetProjectSettings

 This API shall be used to get the Project settings information [auto save, auto generate] for the active project. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the auto save information is filled into the 'varAutoSave' and the auto generate information is filled into the 'varAutoGen'. Refer to the EAutoGenerate and EautoSave varerations.

 \param		varAutoGen			Enum to hold the Auto generation option of the project.
 \param		varAutoSave			Enum to hold the Auto save option of the project.
 \param		varviewMode			Enum to hold the view mode option of the project.
 \param		bExpertViewAlreadySet	Boolean pointer to hold the view type option of the project.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetProjectSettings(AutoGenerate *varAutoGen,
		AutoSave *varAutoSave, ViewMode *varviewMode,
		bool* bExpertViewAlreadySet)
{

	if ((NULL == varAutoGen) || (NULL == varAutoSave)
			|| (NULL == varviewMode) || (NULL == bExpertViewAlreadySet))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	PjtSettings* pobjPrjtSettings = NULL;
	pobjPrjtSettings = PjtSettings::GetPjtSettingsPtr();

	try
	{
		if (pobjPrjtSettings == NULL)
		{
			ocfmException objException;        // = new ocfmException;
			objException.OCFMException(OCFM_ERR_PROJECT_SETTINGS);
			throw objException;
		}

		*varAutoGen = pobjPrjtSettings->GetGenerateAttr();
		*varAutoSave = pobjPrjtSettings->GetSaveAttr();
		*varviewMode = pobjPrjtSettings->GetViewMode();
		*bExpertViewAlreadySet = pobjPrjtSettings->GetExpertViewSelectedFlag();

		stRetInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		SetProjectSettings

 This API shall be used to set the Project settings information [auto save, auto generate] for the active project. Refer to the EAutoGenerate and EautoSave varerations.

 \param		varAutoGen			Enum to hold the Auto generation option of the project.
 \param		varAutoSave			Enum to hold the Auto save option of the project.
 \param		varViewMode			Enum to hold the view mode option of the project.
 \param		bExpertViewAlreadySet	Boolean to hold the view type option of the project.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetProjectSettings(AutoGenerate varAutoGen,
		AutoSave varAutoSave, ViewMode varViewMode,
		bool bExpertViewAlreadySet)
{

	ocfmRetCode stRetInfo;
	stRetInfo.errorString = NULL;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	PjtSettings* pobjPrjtSettings;
	pobjPrjtSettings = PjtSettings::GetPjtSettingsPtr();

	try
	{
		if (pobjPrjtSettings == NULL)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_PROJECT_SETTINGS);
			throw objException;
		}

		pobjPrjtSettings->SetGenerateAttr(varAutoGen);
		pobjPrjtSettings->SetSaveAttr(varAutoSave);
		pobjPrjtSettings->SetViewMode(varViewMode);
		pobjPrjtSettings->SetExpertViewSelectedFlag(bExpertViewAlreadySet);
		stRetInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		UpdateNumberOfEnteriesSIdx

 Updates subindex "00"/NumberofEnteries with the total number of subindexes excluding subindex with id "00"

 \param		pobjIndex		Class pointer of the class CIndex
 \param		varNodeType	Enum of type ENodeType to hold node type

 \return	void
 */
/*****************************************************************************/

//TODO: varNodetype is not used. To be removed.
void UpdateNumberOfEnteriesSIdx(Index *pobjIndex, NodeType varNodeType)
{
	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	SubIndex *pobjSIdx = NULL;
	INT32 iTotalSIdxs = 0;

	pobjSIdx = pobjIndex->GetSubIndexbyIndexValue((char*) "00");

	/* subindexes excluding "00"*/
	iTotalSIdxs = pobjIndex->GetNumberofSubIndexes() - 1;

	if (pobjSIdx != NULL)
	{
		char varAsciBuff[10];
		char* buffer = new char[10];
		strcpy(buffer, "0x");
		strcat(buffer, IntToAscii(iTotalSIdxs, varAsciBuff, 16));
		pobjSIdx->SetActualValue(buffer);
		delete[] buffer;
	}
}

/*****************************************************************************/
/**
 \brief		AuotgenerateOtherIndexs

 This function Auot generates the values in the other required MN indexes

 \param		pobjNode	Class pointer of the class Node

 \return	void
 */
/*****************************************************************************/

void AuotgenerateOtherIndexs(Node* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode retCode;
	char* varMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];
	char* varSidx = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	Index* pobjIndex;
	IndexCollection* pobjIdxCol;

	INT32 IndexPos;

	pobjIdxCol = pobjNode->GetIndexCollection();
	/* 1006*/
	strcpy(varMNIndex, "1006");
#if defined DEBUG	
	cout << "string copied" << endl;
#endif
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
#if defined DEBUG	
	cout << "retcode" << retCode.code << endl;
	cout << "1006 added" << endl;
#endif

	/*  1300*/
	strcpy(varMNIndex, "1300");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		SetIndexAttributes(MN_NODEID, MN, varMNIndex, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);
	}

	/* 1C02*/
	strcpy(varMNIndex, "1C02");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);
		/* Set subindex value 40 or 0000028 */
		strcpy(varSidx, "00");
		SetSIdxValue(varMNIndex, varSidx, (char*) "3", pobjIdxCol,
				pobjNode->GetNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex added" << endl;
#endif

		strcpy(varSidx, "01");
		SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
				pobjNode->GetNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex 01 added" << endl;
#endif
		strcpy(varSidx, "02");
		SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
				pobjNode->GetNodeId(), MN, false);

		strcpy(varSidx, "03");
		SetSIdxValue(varMNIndex, varSidx, (char*) "40", pobjIdxCol,
				pobjNode->GetNodeId(), MN, false);
	}

	/*  1C09*/
	strcpy(varMNIndex, "1C09");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(),
				(char*) "40", false);

	}

	/*  1F26*/
	strcpy(varMNIndex, "1F26");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);

		char* Val = new char[8];
		INT32 ilConfigDate;
		ilConfigDate = GetConfigDate();
		Val = IntToAscii(ilConfigDate, Val, 10);
		strcpy(Val, ConvertToUpper(Val));
		AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(), Val,
				false);
		delete[] Val;

	}

#if defined DEBUG	
	cout << "1F26 subidex added" << endl;
#endif
	/*  1F27*/
	strcpy(varMNIndex, "1F27");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);

		char* Val = new char[50];
		INT32 ilConfigTime;
		ilConfigTime = GetConfigTime();
		Val = IntToAscii(ilConfigTime, Val, 10);
		strcpy(Val, ConvertToUpper(Val));

		AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(), Val,
				false);
		delete[] Val;
	}

	/* 1F84*/
	strcpy(varMNIndex, "1F84");
	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varMNIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->GetNodeId(), (char*) "",
				true);

	}

	delete[] varMNIndex;
	delete[] varSidx;
}

/*****************************************************************************/
/**
 \brief		UpdatedCNDateORTime

 This function updates the CN's Date or Time in the MN's corresponding CN's node id subindex

 \param		pobjMNIndex		Class pointer of the class CIndex
 \param		iNodeId			Integer variable to hold the Node Id of a node
 \param		varDT			Enum of type DateTime to hold either DATE or TIME to updated

 \return	void
 */
/*****************************************************************************/

void UpdatedCNDateORTime(Index* pobjMNIndex, INT32 iNodeId, DateTime varDT)
{
	if (NULL == pobjMNIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	else
	{
		//nothing to do
	}

	SubIndex *pobjSIdx = NULL;
	char Index[INDEX_LEN];
	INT32 IndexPos;

	IndexCollection* pobjIdxCol = NULL;
	NodeCollection* pobjNodeCol = NULL;
	Node* pobjNode = NULL;
	char* subIndexHexValue = new char[3 + ALLOC_BUFFER];

	pobjNodeCol = NodeCollection::GetNodeColObjectPointer();
	pobjNode = pobjNodeCol->GetNodePtr(CN, iNodeId);
	pobjIdxCol = pobjNode->GetIndexCollection();

	subIndexHexValue = IntToAscii(iNodeId, subIndexHexValue, 16);
	subIndexHexValue = PadLeft(subIndexHexValue, '0', 2);
	pobjSIdx = pobjMNIndex->GetSubIndexbyIndexValue(subIndexHexValue);
	delete[] subIndexHexValue;
	strcpy(Index, "1020");

	if (OCFM_ERR_SUCCESS
			!= (IfIndexExists(pobjNode->GetNodeId(), CN, Index, &IndexPos)).code)
	{
		return;
	}
	//pobjSIdx->getActualValue() convert to upper
	if (pobjSIdx != NULL)
	{
		if (pobjSIdx->GetActualValue() != NULL)
		{
			char Sidx[SUBINDEX_LEN];
			if (varDT == DATE)
			{
				strcpy(Sidx, "01");
				SetSIdxValue(Index, Sidx, (char*) pobjSIdx->GetActualValue(),
						pobjIdxCol, pobjNode->GetNodeId(), CN, false);
			}
			else if (varDT == TIME)
			{
				strcpy(Sidx, "02");
				SetSIdxValue(Index, Sidx, (char*) pobjSIdx->GetActualValue(),
						pobjIdxCol, pobjNode->GetNodeId(), CN, false);
			}
			else
			{
				//TODO: 'else' added. operation to be specified
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		CopyPDODefToAct

 Copy pdos (16xx, 1Axx) default value to actual value

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node

 \return	void
 */
/*****************************************************************************/

void CopyPDODefToAct(INT32 iNodeID, NodeType varNodeType)
{

	SubIndex* pobjSIndex = NULL;
	IndexCollection* pobjIdxCol = NULL;

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(varNodeType, iNodeID);
	pobjIdxCol = pobjNode->GetIndexCollection();
	Index* pobjIndex = NULL;
	for (INT32 iIndexLoopCount = 0;
			iIndexLoopCount < pobjIdxCol->GetNumberofIndexes();
			iIndexLoopCount++)
	{
		//	CIndex* pobjIndex;

		pobjIndex = pobjIdxCol->GetIndex(iIndexLoopCount);

		if (!(CheckIfNotPDO((char*) pobjIndex->GetIndexValue())))
		{

			for (INT32 iSIdxLoopCount = 0;
					iSIdxLoopCount < pobjIndex->GetNumberofSubIndexes();
					iSIdxLoopCount++)
			{
				pobjSIndex = pobjIndex->GetSubIndex(iSIdxLoopCount);
				if (pobjSIndex->GetActualValue() == NULL)
				{
					if (pobjSIndex->GetDefaultValue() != NULL)
					{
						pobjSIndex->SetActualValue(
								(char*) pobjSIndex->GetDefaultValue());
					}

				}
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		GetPDOIndexByOffset

 This function returns the CIndex after sorting the collection by offset value

 \param		pobjIndex	Class pointer of the class Index

 \return	Index
 */
/*****************************************************************************/

Index GetPDOIndexByOffset(Index* pobjIndex)
{
	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	else
	{
		//nothing to do
	}

	Index objIndex;
	objIndex = *pobjIndex;
	SubIndex* pObjSIdx1 = NULL;
	SubIndex* pObjSIdx2 = NULL;

	for (INT32 iIndexLoopCount = 1;
			iIndexLoopCount <= (objIndex.GetNumberofSubIndexes() - 1);
			iIndexLoopCount++)
	{
		for (INT32 iIdxLoopCount = 1;
				iIdxLoopCount
						<= (objIndex.GetNumberofSubIndexes() - 1
								- iIndexLoopCount); iIdxLoopCount++)
		{

			pObjSIdx1 = objIndex.GetSubIndex(iIdxLoopCount);
			if ((pObjSIdx1->GetActualValue() != NULL)
					&& (0 != strcmp(pObjSIdx1->GetActualValue(), ""))
					&& !(CheckIfValueZero((char*) pObjSIdx1->GetActualValue())))
			{
				const char* varActualVal1 = pObjSIdx1->GetActualValue();

				INT32 iLength1 = strlen(varActualVal1);
				char* varOffset1 = NULL;
				INT32 iOffset1 = 0;

				varOffset1 = SubString((char*) varActualVal1, iLength1 - 12, 4);
				iOffset1 = HexToInt(varOffset1);
				pObjSIdx2 = objIndex.GetSubIndex(iIdxLoopCount + 1);
				if ((NULL != pObjSIdx2->GetActualValue())
						&& (0 != strcmp(pObjSIdx2->GetActualValue(), ""))
						&& !(CheckIfValueZero(
								(char*) pObjSIdx2->GetActualValue())))
				{
					const char* varActualVal2 = pObjSIdx2->GetActualValue();
					INT32 iLength2 = strlen(varActualVal2);
					char* varOffset2 = NULL;
					INT32 iOffset2 = 0;

					varOffset2 = SubString((char*) varActualVal2, iLength2 - 12,
							4);
					iOffset2 = HexToInt(varOffset2);

					if (iOffset1 > iOffset2)
					{
						objIndex.SwapSubObjects(iIdxLoopCount,
								iIdxLoopCount + 1);

					}
				}
			}
		}
	}

	return objIndex;
}

/*****************************************************************************/
/**
 \brief		GetFeatureValue

 This API shall be used to get the specific network management feature value for the specified node

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		eNodeType			Enum to hold the Node type of the node
 \param		eFeatureType		Enum to hold the Feature type of the node
 \param		FeatureName			Character pointer to hold the Feature name
 \param		Out_FeatureValue	Character pointer to get the feature value

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetFeatureValue(INT32 iNodeID, NodeType eNodeType,
		FeatureType eFeatureType, char* FeatureName, char* Out_FeatureValue)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iNodePos;

	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		Node objNode;
		NodeCollection *pobjNodeCollection;
		NetworkManagement *pobjNwManagement;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = pobjNodeCollection->GetNode(iNodeID);

		pobjNwManagement = objNode.GetNetworkManagement();
		strcpy(Out_FeatureValue,
				pobjNwManagement->getFeatureValue(eFeatureType, FeatureName));

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		UpdateNodeParams

 This API shall be used to update all the attributes of a node including the nodeid.

 \param		iCurrNodeId				Integer variable to hold the current node id of a node
 \param		iNewNodeID				Integer variable to hold the new node id to be set to the node
 \param		eNodeType				Enum to hold the node type of the node
 \param		NodeName				Character pointer to hold the Node Name
 \param		eStationType			Enum to hold the station type of the node
 \param		ForcedCycle				Character pointer to hold the ForcedCycle value
 \param		ForcedCycleFlag			Boolean to hold the Forced Cycle Flag of the node
 \param		PollResponseTimeout		Character pointer to hold the Poll Response Timeout value for the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID,
		NodeType eNodeType, char* NodeName, StationType eStationType,
		char* ForcedCycle, bool ForcedCycleFlag, char* PollResponseTimeout)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;

		if (iNewNodeID == iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if (stErrorInfo.code == 0 && bFlag == true)
			{

			}
			else
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_INVALID_NODEID);
				throw objException;
			}

		}
		else if (iNewNodeID != iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if (stErrorInfo.code == OCFM_ERR_SUCCESS && bFlag == true)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw objException;
			}
			else
			{
#if defined DEBUG
				cout << "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND
				<< endl;
#endif
			}
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		Node* pobjNode;
		NodeCollection *pobjNodeCollection;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		pobjNode = pobjNodeCollection->GetNodePtr(eNodeType, iCurrNodeId);

		if (eNodeType == CN)
		{
			if (iNewNodeID != iCurrNodeId)
			{
				pobjNode->SetNodeId(iNewNodeID);
				CopyOldNodeIdAssignmentObject(pobjNode, iCurrNodeId);
			}
			else
			{
				//TODO: else added. operation to be added.
			}
		}
		else
		{
			//TODO: else added. operation to be added
		}

		if ((eStationType != MULTIPLEXED)
				|| ((iNewNodeID != iCurrNodeId) && (eStationType == MULTIPLEXED)))
		{
			char* cSIdx = new char[SUBINDEX_LEN];
			cSIdx = IntToAscii(iCurrNodeId, cSIdx, 16);
			cSIdx = PadLeft(cSIdx, '0', 2);
			pobjNode->ResetForcedCycleValue();
			delete[] cSIdx;
		}
		else
		{
			//TODO: else added. operation to be added
		}

		if ((eNodeType == CN)
				&& !((PollResponseTimeout == NULL)
						|| (strcmp(PollResponseTimeout, "") == 0)))
		{
			pobjNode->SetPollResponseTimeout(PollResponseTimeout);
		}
		else
		{
			//TODO: else added. operation to be added
		}
		pobjNode->SetNodeName(NodeName);

		if (eNodeType == CN)
		{
			StationType eNodePrevStationType = pobjNode->GetStationType();
			pobjNode->SetStationType(eStationType);
			if ((eNodePrevStationType == MULTIPLEXED)
					&& (eStationType != eNodePrevStationType))
			{
				CheckAndReAssignMultiplex(iNewNodeID,
						pobjNode->GetForcedCycle());
			}
			else
			{
				//TODO: else added. operation to be added
			}

			if (eStationType != MULTIPLEXED)
			{
				pobjNode->ResetForcedCycleValue();
			}
			else
			{
				//TODO: 'else' added. Operations to be specified.
			}

			if (eStationType == MULTIPLEXED)
			{
				bool bCalcForceCycle = true;
				bool bSetForceCycle = true;
				if (ForcedCycleFlag == true)
				{
					bCalcForceCycle = false;
				}
				else
				{
					//TODO: else added. operation to be added
				}

				if ((ForcedCycle == NULL || strcmp(ForcedCycle, "") == 0)
						&& ForcedCycleFlag == false)
				{
					if (pobjNode->GetForceCycleFlag() == true)
					{

					}
					else if (pobjNode->GetForcedCycle() == NULL
							|| strcmp(pobjNode->GetForcedCycle(), "") == 0)
					{

					}
					else
					{

						INT32 iCNActualValue = 0;
						if (strncmp(pobjNode->GetForcedCycle(), "0x", 2) == 0
								|| strncmp(pobjNode->GetForcedCycle(), "0X", 2)
										== 0)
						{
							iCNActualValue = HexToInt(
									SubString(pobjNode->GetForcedCycle(), 2,
											strlen(pobjNode->GetForcedCycle())
													- 2));
						}
						else
						{
							iCNActualValue = atoi(pobjNode->GetForcedCycle());
						}

						if (true
								== IsMultiplexCycleNumberContinuous(
										iCNActualValue))
						{
							bCalcForceCycle = false;
							bSetForceCycle = false;
						}
						else
						{
							//TODO: else added. operation to be added
						}
					}
				}
				else
				{
					//TODO: 'else' added. operation to be specified
				}

				if (bCalcForceCycle == true)
				{
					ForcedCycle = GetLastAvailableCycleNumber();
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (bSetForceCycle == true)
				{
					stErrorInfo = pobjNode->SetForcedCycle(ForcedCycle);
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (OCFM_ERR_SUCCESS != stErrorInfo.code)
				{
					return stErrorInfo;
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				pobjNode->SetForceCycleFlag(ForcedCycleFlag);
			}
			else
			{
				//TODO: else added. operation to be added
			}
		}
		else
		{
			//TODO: else added. operation to be added
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetNodeDataTypes

 This API shall be used to get all the datatypes available for the particular node.

 \param		iNodeId				Integer variable to hold the Node Id of a node
 \param		eNodeType			Enum to hold the Node type of the node
 \param		varOutDataTypes		Character pointer to hold all the datatypes available for the specifed node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetNodeDataTypes(INT32 iNodeId, NodeType eNodeType,
		char* varOutDataTypes)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeId, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{

		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
			throw objException;
		}

		Node* pobjNode;
		NodeCollection *pobjNodeCollection;
		DataTypeCollection *pobjDtCol;

		pobjNodeCollection = NodeCollection::GetNodeColObjectPointer();
		pobjNode = pobjNodeCollection->GetNodePtr(eNodeType, iNodeId);
		pobjDtCol = pobjNode->GetDataTypeCollection();

		for (UINT16 uiLoopCount = 0;
				uiLoopCount < pobjDtCol->GetNumberOfDataTypes(); uiLoopCount++)
		{
			DataType *dt = NULL;
			char* strdtName = NULL;

			dt = pobjDtCol->GetDataTypeElement(uiLoopCount);
			strdtName = new char[strlen(dt->GetName()) + STR_ALLOC_BUFFER];

			strcpy(strdtName, dt->GetName());
			if (uiLoopCount == 0)
			{
				strcpy(varOutDataTypes, strdtName);
			}
			else
			{
				strcat(varOutDataTypes, strdtName);

			}

			strcat(varOutDataTypes, "\n");
			delete[] strdtName;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		setNodeAssigmentBits

 Returns the 1F81 object data depending upon the condition (if Multiplex set bit 8, if chained set bit 14)

 \param		pobjNode	Class pointer of the class CNode

 \return	char*
 */
/*****************************************************************************/

char* SetNodeAssigmentBits(Node* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* var1F81Data = NULL;
	ULONG ulValue;
	var1F81Data = new char[8 + STR_ALLOC_BUFFER];
	ulValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
			| EPL_NODEASSIGN_NODE_IS_CN | EPL_NODEASSIGN_START_CN;
	switch (pobjNode->GetStationType())
	{
	case NORMAL:
		break;

	case MULTIPLEXED:
		ulValue = ulValue | EPL_NODEASSIGN_MULTIPLEXED_CN;
		break;

	case CHAINED:
		ulValue = ulValue | EPL_NODEASSIGN_CHAINED_CN;
		break;

	default:
		cout << "Invalid station Type:" << pobjNode->GetStationType() << endl;
		break;
	};

	var1F81Data = IntToAscii(ulValue, var1F81Data, 16);
	return var1F81Data;
}

/*****************************************************************************/
/**
 \brief		SetPresMNNodeAssigmentBits

 sets Pres MN Node Assigment Bits for 1F81 index depending upon the condition (if Multiplex set bit 8, if chained set bit 14)

 \return	void*
 */
/*****************************************************************************/

void SetPresMNNodeAssigmentBits()
{
	INT32 IndexPos;
	INT32 subIndexPos;
	ocfmRetCode stErrStructInfo;
	stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *) "1F81",
			(char *) "F0", &subIndexPos, &IndexPos);

	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
		return;
	}

	bool bIsPresMN = false;
	bool bIsMNBroadcastingPRes = false;

	bIsPresMN = IsPresMN();

	//check whether Pres activated in MN
	NodeCollection* pobjNodeCol;
	Node objMNNode;
	IndexCollection* objPDOCollection;
	IndexCollection* pobjIndexCollection;

	pobjNodeCol = NodeCollection::GetNodeColObjectPointer();
	objMNNode = pobjNodeCol->GetMNNode();

	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	objPDOCollection = objMNNode.getPDOIndexCollection(&rpdoCount, &tpdoCount);
	if (objPDOCollection != NULL)
	{
		pobjIndexCollection = objMNNode.GetIndexCollection();
		Index* pobjIndex;
		for (INT32 iLoopCount = 0;
				iLoopCount < objPDOCollection->GetNumberofIndexes();
				iLoopCount++)
		{
			pobjIndex = objPDOCollection->GetIndex(iLoopCount);
			if (0 == strncmp(pobjIndex->GetIndexValue(), "1A", 2))
			{
				Index *pobjCommIndex = NULL;

				char *varIdx = SubString((char *) pobjIndex->GetIndexValue(), 2,
						4);
				char *varCommIdx = new char[INDEX_LEN];
				strcpy(varCommIdx, (char *) "18");
				strcat(varCommIdx, varIdx);
				pobjCommIndex = pobjIndexCollection->GetIndexbyIndexValue(
						varCommIdx);
				if (NULL != pobjCommIndex)
				{
					SubIndex *pobjNodeIDSubIndex = NULL;
					pobjNodeIDSubIndex = pobjCommIndex->GetSubIndexbyIndexValue(
							(char *) "01");
					if (NULL != pobjNodeIDSubIndex)
					{
						if ((NULL != pobjNodeIDSubIndex->GetActualValue())
								&& (0
										!= strcmp(
												pobjNodeIDSubIndex->GetActualValue(),
												"")))
						{
							INT32 iNodeTPDOMappedNodeID = 0;
							iNodeTPDOMappedNodeID =
									GetDecimalValue(
											(char *) pobjNodeIDSubIndex->GetActualValue());
							if (BROADCAST_NODEID == iNodeTPDOMappedNodeID)
							{
								bIsMNBroadcastingPRes = true;
								break;
							}
						}
					}
				}
				delete[] varCommIdx;
				delete[] varIdx;
			}
		}
	}

	//check whether MN is transmitting PRes
	if ((false == bIsPresMN) && (false == bIsMNBroadcastingPRes))
	{
		return;
	}

	SubIndex* pobjSubindex;
	pobjSubindex = GetMNSubIndexValues((char*) "1F81", (char*) "F0");
	if (NULL != pobjSubindex)
	{
		if ((true == bIsPresMN) || (true == bIsMNBroadcastingPRes))
		{
			char* var1F81Data = NULL;
			ULONG ulValue;
			var1F81Data = new char[8 + STR_ALLOC_BUFFER];
			ulValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
					| EPL_NODEASSIGN_MN_PRES;
			strcpy(var1F81Data, (char *) "0x");
			IntToAscii(ulValue, &var1F81Data[2], 16);

			pobjSubindex->SetActualValue(var1F81Data);
			pobjSubindex->SetFlagIfIncludedCdc(TRUE);
			delete[] var1F81Data;
		}
		else
		{
			// no operation
		}
	}

}

/*****************************************************************************/
/**
 \brief		RecalculateMultiplex

 Recalculates the multiplex cycle for CNs

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode RecalculateMultiplex()
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	NodeCollection *objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	//INT32 iNodesCount = 0;
	//iNodesCount = objNodeCol->getCNNodesCount();

	Node* objNode = NULL;

	try
	{
		if (false == CheckIfMultiplexedCNExist())
		{
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return stErrorInfo;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 IndexPos = 0;
		INT32 subIndexPos = 0;
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &IndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			//reset automatically assigned cn force cycle
			ResetMultiplexedCNForceCycle();
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			delete[] actValue;
			return stErrorInfo;
		}
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(stErrorInfo.code);
			delete[] actValue;
			throw objException;
		}

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN)
			{
				ocfmRetCode stErrorInfoSubindexMultiplCycl;
				static char* strConvertedValue = NULL;
				if (strConvertedValue != NULL)
				{
					delete[] strConvertedValue;
					strConvertedValue = NULL;
				}
				strConvertedValue = new char[SUBINDEX_LEN];
				char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
				strConvertedValue = IntToAscii(objNode->GetNodeId(),
						strConvertedValue, 16);
				strConvertedValue = PadLeft(strConvertedValue, '0', 2);
				char* subIndActValue = new char[20];
				subIndActValue[0] = 0;
				try
				{
					stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
							MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, ACTUALVALUE, subIndActValue);
				} catch (...)
				{
					delete[] subIndActValue;
					continue;
				}
				if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
				{
				}
				else
				{
					continue;
				}
				if (objNode->GetStationType() == MULTIPLEXED)
				{
					if (objNode->GetForceCycleFlag() == 1)
					{
						continue;
					}
					else if (objNode->GetForcedCycle() == NULL
							|| strcmp(objNode->GetForcedCycle(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						char* subIndName = new char[50];
						subIndName[0] = 0;
						GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue, NAME,
								subIndName);
						char* subIndFlag = new char[10];
						GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue,
								FLAGIFINCDC, subIndFlag);
						INT32 iCNsubIndFlag = 0;
						iCNsubIndFlag = atoi(subIndFlag);
						SetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue,
								(char*) "", subIndName, (Flag) iCNsubIndFlag);
						delete[] subIndName;
						delete[] subIndFlag;
						continue;
					}
					else
					{
						if (stErrorInfoSubindexMultiplCycl.code
								== OCFM_ERR_SUCCESS)
						{
							INT32 iMNMultiActualValue = 0;
							if (strncmp(actValue, "0x", 2) == 0
									|| strncmp(actValue, "0X", 2) == 0)
								iMNMultiActualValue = HexToInt(
										SubString(actValue, 2,
												strlen(actValue) - 2));
							else
								iMNMultiActualValue = atoi(actValue);

							char* ForcedCycleValue = new char[50];
							strcpy(ForcedCycleValue, objNode->GetForcedCycle());
							INT32 iCNActualValue = 0;
							if (strncmp(ForcedCycleValue, "0x", 2) == 0
									|| strncmp(ForcedCycleValue, "0X", 2) == 0)
								iCNActualValue = HexToInt(
										SubString(ForcedCycleValue, 2,
												strlen(ForcedCycleValue) - 2));
							else
								iCNActualValue = atoi(ForcedCycleValue);

							if (iMNMultiActualValue == 0)
							{

								char* subIndName = new char[50];
								subIndName[0] = 0;
								GetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, NAME, subIndName);
								char* subIndFlag = new char[10];
								GetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, FLAGIFINCDC,
										subIndFlag);
								INT32 iCNsubIndFlag = 0;
								iCNsubIndFlag = atoi(subIndFlag);
								SetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, (char*) "",
										subIndName, (Flag) iCNsubIndFlag);
								delete[] subIndName;
								delete[] subIndFlag;
								continue;
							}
							else if (iCNActualValue <= iMNMultiActualValue)
							{
								continue;
							}
							else
							{
								//continue the function
							}
							delete[] ForcedCycleValue;
						}
						else
						{

						}
					}

					char* ForcedCycle = GetLastAvailableCycleNumber();
					objNode->SetForcedCycle(ForcedCycle);
					delete[] ForcedCycle;
				}
				else
				{
					// station other than multiplexed
					char* subIndName = new char[50];
					subIndName[0] = 0;
					GetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, NAME, subIndName);

					char* subIndFlag = new char[10];
					GetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, FLAGIFINCDC, subIndFlag);

					INT32 iCNsubIndFlag = 0;
					iCNsubIndFlag = atoi(subIndFlag);
					SetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, (char*) "", subIndName,
							(Flag) iCNsubIndFlag);
					delete[] subIndName;
					delete[] subIndFlag;
				}
				delete[] strConvertedValue;
			} // end of if loop 1
			else
			{
				continue;
				//node type == mn
			}
		} //end of for loop
		delete[] actValue;
	} //end of try
	catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		ResetMultiplexedCNForceCycle

 This function resets the Multiplexed CN's forced cycle value

 \return	void
 */
/*****************************************************************************/

void ResetMultiplexedCNForceCycle()
{
	NodeCollection *objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node* objNode = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
			iLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
		if (objNode->GetNodeType() == CN)
		{
			ocfmRetCode stErrorInfoSubindexMultiplCycl;
			static char* strConvertedValue = NULL;
			if (strConvertedValue != NULL)
			{
				delete[] strConvertedValue;
				strConvertedValue = NULL;
			}
			strConvertedValue = new char[SUBINDEX_LEN];
			char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
			strConvertedValue = IntToAscii(objNode->GetNodeId(),
					strConvertedValue, 16);
			strConvertedValue = PadLeft(strConvertedValue, '0', 2);
			char* subIndActValue = new char[20];
			subIndActValue[0] = 0;
			try
			{
				stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
						MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue,
						ACTUALVALUE, subIndActValue);
			} catch (...)
			{
				delete[] subIndActValue;
				continue;
			}
			if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
			{
			}
			else
			{
				continue;
			}
			if (objNode->GetStationType() == MULTIPLEXED)
			{
				if (objNode->GetForceCycleFlag() == 1)
				{
					continue;
				}
				objNode->ResetForcedCycleValue();
				objNode->SetStationType(NORMAL);
			}
			delete[] strConvertedValue;
		}
	}
}

/*****************************************************************************/
/**
 \brief		CopyMNPropDefToAct

 This function copies the (1006, 1F8A/02, 1F98/07, 1F98/08) indexes default value to actual value

 \param		iNodeID			Integer variable to hold the node id
 \param		varNodeType	Enum of type ENodeType to hold the Node type

 \return	void
 */
/*****************************************************************************/

void CopyMNPropDefToAct(INT32 iNodeID, NodeType varNodeType)
{
	if (varNodeType != MN)
	{
		return;
	}
	CopyIndexDefToAct(iNodeID, varNodeType, (char *) "1006");
	CopySubIndexDefToAct(iNodeID, varNodeType, false, (char *) "1F8A",
			(char *) "02");
	CopySubIndexDefToAct(iNodeID, varNodeType, false, (char *) "1F98",
			(char *) "07");
	CopySubIndexDefToAct(iNodeID, varNodeType, false, (char *) "1F98",
			(char *) "08");
}

/*****************************************************************************/
/**
 \brief		CopyIndexDefToAct

 This function copies the index's default value to the actual value.

 \param		iNodeID			Integer variable to hold the node id
 \param		varNodeType		Enum of type NodeType to hold the Node type
 \param		indexId			Character pointer to hold the Index Id

 \return	void
 */
/*****************************************************************************/

void CopyIndexDefToAct(INT32 iNodeID, NodeType varNodeType, char *indexId)
{
	IndexCollection* pobjIdxCol = NULL;

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(varNodeType, iNodeID);
	pobjIdxCol = pobjNode->GetIndexCollection();

	Index* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue(indexId);
	if (pobjIndex == NULL)
	{
		return;
	}
	if (pobjIndex->GetActualValue() == NULL)
	{
		if (pobjIndex->GetDefaultValue() != NULL)
		{
			pobjIndex->SetActualValue((char*) pobjIndex->GetDefaultValue());
		}
	}
}

/*****************************************************************************/
/**
 \brief		CopySubIndexDefToAct

 This function copies the subindexes default value to the actual value. If for copy is set empty values will be copied while the default value is not configured.

 \param		iNodeID			Integer variable to hold the node id
 \param		varNodeType		Enum of type NodeType to hold the Node type
 \param		bForce			Boolean value to set force copy or not
 \param		indexId			Character pointer to hold the Index Id
 \param		subIndexId		Character pointer to hold the subIndex Id

 \return	void
 */
/*****************************************************************************/

void CopySubIndexDefToAct(INT32 iNodeID, NodeType varNodeType, bool bForce,
		char *indexId, char *subIndexId)
{
	SubIndex* pobjSIndex = NULL;
	IndexCollection* pobjIdxCol = NULL;

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(varNodeType, iNodeID);
	pobjIdxCol = pobjNode->GetIndexCollection();

	Index* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue(indexId);
	if (pobjIndex == NULL)
	{
		return;
	}

	pobjSIndex = pobjIndex->GetSubIndexbyIndexValue(subIndexId);
	if (pobjSIndex == NULL)
	{
		return;
	}
	if ((pobjSIndex->GetActualValue() == NULL) || (true == bForce))
	{
		if (pobjSIndex->GetDefaultValue() != NULL)
		{
			pobjSIndex->SetActualValue((char*) pobjSIndex->GetDefaultValue());
		}
		else
		{
			if (true == bForce)
			{
				pobjSIndex->SetActualValue((char*) "");
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		CheckAndReAssignMultiplex

 This function checks and re-assigns the Multiplexd cycle value

 \param		iCNNodeId		Integer variable to hold the node id of the CN
 \param		CycleValue		Character pointer to hold the Multiplexd cycle value

 \return	void
 */
/*****************************************************************************/

void CheckAndReAssignMultiplex(INT32 iCNNodeId, char* CycleValue)
{
	ocfmRetCode stErrorInfo;
	if (CycleValue == NULL || strcmp(CycleValue, "") == 0)
	{
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		//return stErrorInfo;
		return;
	}

	INT32 iCycleValue = 0;
	if (strncmp(CycleValue, "0x", 2) == 0 || strncmp(CycleValue, "0X", 2) == 0)
		iCycleValue = HexToInt(SubString(CycleValue, 2, strlen(CycleValue) - 2));
	else
		iCycleValue = atoi(CycleValue);

	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->GetCNNodesCount();

	Node* objNode;

	try
	{
		if (iNodesCount == 0)
		{
			return;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		try
		{

			stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
					(char*) "07", ACTUALVALUE, actValue);
		} catch (...)
		{
			delete[] actValue;
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return;
		}

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN
					&& objNode->GetNodeId() != iCNNodeId
					&& objNode->GetStationType() == MULTIPLEXED)
			{
				if (objNode->GetForcedCycle() == NULL
						|| strcmp(objNode->GetForcedCycle(), "") == 0)
				{
					continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->GetForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->GetForcedCycle());
					INT32 iCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						iCNActualValue = HexToInt(
								SubString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						iCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (iCycleValue == iCNActualValue)
					{
						return;
					}
					else
					{
						//continue the function
					}
					delete[] ForcedCycleValue;
				}
			} // end of if loop 1
		} //end of for loop
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN
					&& objNode->GetNodeId() != iCNNodeId
					&& objNode->GetStationType() == MULTIPLEXED
					&& objNode->GetForceCycleFlag() == false)
			{
				if (objNode->GetForcedCycle() == NULL
						|| strcmp(objNode->GetForcedCycle(), "") == 0)
				{
					continue;
				}
				else
				{
				}

				char* ForcedCycleValue = new char[strlen(
						objNode->GetForcedCycle()) + ALLOC_BUFFER];
				strcpy(ForcedCycleValue, objNode->GetForcedCycle());
				INT32 iCNActualValue = 0;
				if (strncmp(ForcedCycleValue, "0x", 2) == 0
						|| strncmp(ForcedCycleValue, "0X", 2) == 0)
					iCNActualValue = HexToInt(
							SubString(ForcedCycleValue, 2,
									strlen(ForcedCycleValue) - 2));
				else
					iCNActualValue = atoi(ForcedCycleValue);

				delete[] ForcedCycleValue;
				if (iCycleValue >= iCNActualValue || iCNActualValue == 1)
				{
					continue;
				}
				else
				{
					//continue the function
				}
				iCNActualValue--;
				uiCycleNumber = iCNActualValue;

				static char* strCNActualValue = NULL;
				if (strCNActualValue != NULL)
				{
					delete[] strCNActualValue;
					strCNActualValue = NULL;
				}
				strCNActualValue = new char[50];
				strCNActualValue = IntToAscii(iCNActualValue, strCNActualValue,
						10);

				objNode->SetForcedCycle(strCNActualValue);

				delete[] strCNActualValue;
				delete[] ForcedCycleValue;
			} //end of if loop
		} //end of for loop
		delete[] actValue;
	} //end of try
	catch (...)
	{
		return;
	}
}

/*****************************************************************************/
/**
 \brief		CheckMutliplexAssigned

 This function checks for the multiplexed (1F98/07) assigned or not

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CheckMutliplexAssigned()
{
	ocfmRetCode stErrorInfo;
	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->GetCNNodesCount();
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	stErrorInfo.errorString = NULL;

	bool bErrorFlag = false;
	Node* objNode;

	try
	{
		if (iNodesCount == 0)
		{
			stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
			return stErrorInfo;
		}
		if (false == CheckIfMultiplexedCNExist())
		{
			return stErrorInfo;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 subIndexPos = 0;
		INT32 IndexPos = 0;
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &IndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			CopyCustomErrorString(&stErrorInfo,
					(char*) "The Subindex 07 of Index 1F98 does not exist Multiplexing not supported");
			delete[] actValue;
			return stErrorInfo;
		}
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (stErrorInfo.code != OCFM_ERR_SUCCESS)
		{
			delete[] actValue;
			return stErrorInfo;
		}

		stErrorInfo.errorString = new char[500];
		strcpy(stErrorInfo.errorString, "CN with nodeid ");
		INT32 iMNMultiActualValue = 0;
		if (strncmp(actValue, "0x", 2) == 0 || strncmp(actValue, "0X", 2) == 0)
			iMNMultiActualValue = HexToInt(
					SubString(actValue, 2, strlen(actValue) - 2));
		else
			iMNMultiActualValue = atoi(actValue);

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN)
			{
				if (objNode->GetStationType() == MULTIPLEXED)
				{

					ocfmRetCode stErrorInfoSubindexMultiplCycl;
					static char* strConvertedValue = NULL;
					if (strConvertedValue != NULL)
					{
						delete[] strConvertedValue;
						strConvertedValue = NULL;
					}
					strConvertedValue = new char[SUBINDEX_LEN];
					char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
					strConvertedValue = IntToAscii(objNode->GetNodeId(),
							strConvertedValue, 16);
					strConvertedValue = PadLeft(strConvertedValue, '0', 2);
					char* subIndActValue = new char[20];
					subIndActValue[0] = 0;
					try
					{
						stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
								MN_NODEID, MN, acMultiCycleAssignObj,
								strConvertedValue, ACTUALVALUE, subIndActValue);
					} catch (...)
					{
						delete[] subIndActValue;
						//delete[] strConvertedValue;
						continue;
					}
					if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
					{
					}
					else
					{
						continue;
					}

					if (objNode->GetForcedCycle() == NULL
							|| strcmp(objNode->GetForcedCycle(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						continue;
					}
					else
					{
						char* ForcedCycleValue = new char[50];
						strcpy(ForcedCycleValue, objNode->GetForcedCycle());
						INT32 iCNActualValue = 0;
						if (strncmp(ForcedCycleValue, "0x", 2) == 0
								|| strncmp(ForcedCycleValue, "0X", 2) == 0)
							iCNActualValue = HexToInt(
									SubString(ForcedCycleValue, 2,
											strlen(ForcedCycleValue) - 2));
						else
							iCNActualValue = atoi(ForcedCycleValue);

						if (iCNActualValue <= iMNMultiActualValue)
						{
							continue;
						}
						else
						{
							sprintf(stErrorInfo.errorString, "%s%d, ",
									stErrorInfo.errorString,
									objNode->GetNodeId());
							bErrorFlag = true;
						}
					}
					delete[] strConvertedValue;
				}
				else
				{
				}
			} // end of if loop 1
		} //end of for loop
		if (bErrorFlag == true)
		{
			stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			stErrorInfo.errorString[strlen(stErrorInfo.errorString) - 2] = 0;
			strcat(stErrorInfo.errorString,
					" multiplex cycle value exceeds the multiplex prescalar");
		}
		delete[] actValue;
	} //end of try
	catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetFreeCycleNumber

 This function returns the cycle number which is avaliable

 \param		uiParmCycleNumber	Unsigned Integer variable to hold the maximum limit for the cycle number.

 \return	UINT32
 */
/*****************************************************************************/

UINT32 GetFreeCycleNumber(UINT32 uiParmCycleNumber)
{
	ocfmRetCode stErrorInfo;
	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->GetCNNodesCount();

	Node* objNode;

	if (iNodesCount == 0)
	{
		stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
		ocfmException objException; // = new ocfmException;
		objException.OCFMException(stErrorInfo.code);
		return uiParmCycleNumber;
	}
	for (UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiParmCycleNumber;
			uiCycleNumberCount++)
	{
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN
					&& objNode->GetStationType() == MULTIPLEXED)
			{
				if (objNode->GetForcedCycle() == NULL
						|| strcmp(objNode->GetForcedCycle(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->GetForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->GetForcedCycle());
					UINT32 uiCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						uiCNActualValue = HexToInt(
								SubString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						uiCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (uiCycleNumberCount == uiCNActualValue)
					{
						break;
					}
					else
					{
						//continue the function
					}
				}
			} // end of if loop 1
			if (iLoopCount == objNodeCol->GetNumberOfNodes() - 1)
			{
				return uiCycleNumberCount;
			}
		} //end of for loop

	} //end of for loop
	return uiParmCycleNumber;
}

/*****************************************************************************/
/**
 \brief		IsMultiplexCycleNumberContinuous

 This function validates the Multiplexd Cycle Number is Continuous or not

 \param		uiParmCycleNumber	Unsigned Integer variable to hold the maximum limit for the cycle number.

 \return	BOOL

 \retval	TRUE		if continuous multiplexed cycle number is present
 \retval	FALSE		if multiplexed cycle number is not continuous
 */
/*****************************************************************************/

bool IsMultiplexCycleNumberContinuous(UINT32 uiParmCycleNumber)
{
	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->GetCNNodesCount();

	Node* objNode;

	if (iNodesCount == 0)
	{
		return false;
	}
	for (UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiParmCycleNumber;
			uiCycleNumberCount++)
	{

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
				iLoopCount++)
		{

			objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
			if (objNode->GetNodeType() == CN
					&& objNode->GetStationType() == MULTIPLEXED)
			{

				if (objNode->GetForcedCycle() == NULL
						|| strcmp(objNode->GetForcedCycle(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->GetForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->GetForcedCycle());
					UINT32 uiCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						uiCNActualValue = HexToInt(
								SubString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						uiCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (uiCycleNumberCount == uiCNActualValue)
					{
						break;
					}
					else
					{
						//continue the function
					}
				}
			} // end of if loop 1
			if (iLoopCount == objNodeCol->GetNumberOfNodes() - 1)
			{
				return false;
			}
		} //end of for loop

	} //end of for loop
	return true;
}

/*****************************************************************************/
/**
 \brief		CalculateCNPollResponse

 This function calculates the CN Poll Response time of the CN

 \param		iNodeID			Integer variable to hold the node id.
 \param		varNodeType		Enum to hold the Node type

 \return	void
 */
/*****************************************************************************/

void CalculateCNPollResponse(INT32 iNodeID, NodeType varNodeType)
{
	if (varNodeType != CN)
	{
		return;
	}
	INT32 SubIndexPos;
	INT32 IndexPos;
	ocfmRetCode stErrorInfo;
	stErrorInfo = IfSubIndexExists(iNodeID, varNodeType, (char*) "1F98",
			(char*) "03", &SubIndexPos, &IndexPos);
	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return;
	}

	SubIndex* pobjSIndex = NULL;
	IndexCollection* pobjIdxCol = NULL;

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(varNodeType, iNodeID);
	pobjIdxCol = pobjNode->GetIndexCollection();

	Index* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1F98");
	if (pobjIndex == NULL)
	{
		return;
	}

	pobjSIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "03");
	if (pobjSIndex == NULL)
	{
		return;
	}
	char *varValue = NULL;
	INT32 iValue = 0;
	bool add25microsec = false;
	if (pobjSIndex->GetActualValue() == NULL
			|| strcmp(pobjSIndex->GetActualValue(), "") == 0)
	{
		if (pobjSIndex->GetDefaultValue() == NULL
				|| strcmp(pobjSIndex->GetDefaultValue(), "") == 0)
		{
			varValue = new char[strlen("25000") + ALLOC_BUFFER];
			strcpy(varValue, "25000");
		}
		else
		{
			varValue = new char[strlen(pobjSIndex->GetDefaultValue())
					+ ALLOC_BUFFER];
			strcpy(varValue, pobjSIndex->GetDefaultValue());
			add25microsec = true;
		}
	}
	else
	{
		varValue = new char[strlen(pobjSIndex->GetActualValue()) + ALLOC_BUFFER];
		strcpy(varValue, pobjSIndex->GetActualValue());
	}

	if (strncmp(varValue, "0x", 2) == 0 || strncmp(varValue, "0X", 2) == 0)
		iValue = HexToInt(SubString(varValue, 2, strlen(varValue) - 2));
	else
		iValue = atoi(varValue);

	if (true == add25microsec)
		iValue += 25000;

	//add with 25 micro sec
	char *convValue = new char[30];

	convValue = IntToAscii(iValue, convValue, 10);
	pobjNode->SetPollResponseTimeout(convValue);
	delete[] varValue;
	delete[] convValue;
}

/*****************************************************************************/
/**
 \brief		CopyCustomErrorString

 This function copies the custom error description to be thrown to the exception handler.

 \param		stRet				Struct pointer of type ocfmRetCode to hold the Errorcode & error string
 \param		varCustomErrString	Character pointer to hold the custom error description

 \return	void
 */
/*****************************************************************************/

void CopyCustomErrorString(ocfmRetCode* stRet, char* varCustomErrString)
{
	if ((NULL == stRet) || (NULL == varCustomErrString))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	if (0 == strlen(varCustomErrString))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	stRet->errorString = new char[strlen(varCustomErrString) + ALLOC_BUFFER];
	strcpy(stRet->errorString, varCustomErrString);
}

/*****************************************************************************/
/**
 \brief		CheckIfMultiplexedCNExist

 This function checks for the existence of a CN with muliplexed station type

 \return	BOOL

 \retval	TRUE			if Multiplexed CN Exist
 \retval	FALSE			if Multiplexed CN does not Exist
 */
/*****************************************************************************/
bool CheckIfMultiplexedCNExist()
{
	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node* objNode;
	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->GetNumberOfNodes();
			iLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iLoopCount);
		if (objNode->GetNodeType() == CN)
		{
			if (objNode->GetStationType() == MULTIPLEXED)
			{
				return true;
			}
		}
	}
	return false;
}

/*****************************************************************************/
/**
 \brief			RecalculateCNPresTimeout

 \param			varSubIndexId
 \return	void
 */
/*****************************************************************************/
//TODO: unused function
void RecalculateCNPresTimeout(char* varSubIndexId)
{
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", varSubIndexId,
			&iSubIndexPos, &iIndexPos);
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return;
	}
	char* varMNIndValue = new char[50];
	GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F92", varSubIndexId,
			ACTUALVALUE, varMNIndValue);

	if (NULL == varMNIndValue)
	{
		return;
	}

	INT32 iCNNodeId = HexToInt(varSubIndexId);

	stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			&iSubIndexPos, &iIndexPos);
	if (OCFM_ERR_SUCCESS != stErrorInfo.code)
	{
		delete[] varMNIndValue;
		return;
	}

	char* strConvertedValue;
	strConvertedValue = new char[SUBINDEX_LEN];
	strcpy(strConvertedValue, varSubIndexId);

	char* subIndName = new char[50];
	subIndName[0] = 0;
	INT32 iCNsubIndFlag = 0;

	GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03", NAME,
			subIndName);
	char* subIndFlag = new char[10];
	GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			FLAGIFINCDC, subIndFlag);
	iCNsubIndFlag = atoi(subIndFlag);

	SetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			varMNIndValue, subIndName, (Flag) iCNsubIndFlag);
	delete[] subIndName;
	delete[] subIndFlag;
	delete[] varMNIndValue;
	delete[] strConvertedValue;

}

/*****************************************************************************/
/**
 \brief		UpdateMNNodeAssignmentIndex

This function updates the MN indexes & subindexes with the values corresponding to the cn node id

 \param		pobjNode			Class pointer of the class Node
 \param		CNsCount			Integer variable to hold the total no.of CN's count
 \param		varIndex			Character pointer to hold the IndexID
 \param		allowMNSubindex		Boolean value to allow or disallow the subindex in MN

 \return	void
 */
/*****************************************************************************/

//TODO: CNsCount is not used to be removed.
void UpdateMNNodeAssignmentIndex(Node *pobjNode, INT32 CNsCount, char* varIndex,
		bool allowMNSubindex)
{
	if ((NULL == varIndex) || (NULL == pobjNode))
	{
		return;
	}

	IndexCollection *pobjIdxCol = NULL;
	ocfmRetCode retCode;
	retCode.errorString = NULL;
	INT32 IndexPos;

	pobjIdxCol = pobjNode->GetIndexCollection();
	char* varMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];

	strcpy(varMNIndex, varIndex);

	retCode = IfIndexExists(MN_NODEID, MN, varMNIndex, &IndexPos);
	delete[] varMNIndex;
	if (retCode.code == OCFM_ERR_SUCCESS)
	{
		Index *pobjIndex = NULL;
		pobjIndex = pobjIdxCol->GetIndexbyIndexValue(varIndex);
		/* $:set Flag to true*/
		pobjIndex->SetFlagIfIncludedCdc(TRUE);
		SubIndex* pobjSubIndex;
		for (INT32 iSidxCount = pobjIndex->GetNumberofSubIndexes() - 1;
				iSidxCount >= 0; iSidxCount--)
		{
			pobjSubIndex = pobjIndex->GetSubIndex(iSidxCount);
			if (NULL == pobjSubIndex)
				continue;

			if (0 == strcmp((char*) pobjSubIndex->GetIndexValue(), "00"))
				continue;

			try
			{
				INT32 iNodeidValue = HexToInt(
						(char*) pobjSubIndex->GetIndexValue());
				NodeType iNodeType;
				if (MN_NODEID == iNodeidValue)
				{
					iNodeType = MN;
				}
				else
				{
					iNodeType = CN;
				}
				INT32 iNodePos;
				bool bFlag = false;
				retCode = IfNodeExists(iNodeidValue, iNodeType, &iNodePos,
						bFlag);

				if (OCFM_ERR_SUCCESS == retCode.code && true == bFlag
						&& ((CN == iNodeType) || (true == allowMNSubindex)))
				{
					//continue
				}
				else
				{
					if (NULL == pobjSubIndex->GetDefaultValue())
						pobjSubIndex->SetActualValue((char *) "");
					else
						pobjSubIndex->SetActualValue(
								(char *) pobjSubIndex->GetDefaultValue());
				}
			} catch (...)
			{
				if (NULL == pobjSubIndex->GetDefaultValue())
					pobjSubIndex->SetActualValue((char *) "");
				else
					pobjSubIndex->SetActualValue(
							(char *) pobjSubIndex->GetDefaultValue());
			}

		}
	}
}

/*****************************************************************************/
/**
 \brief		ValidateCNPresTimeout

 This function validates CN's PresTimeout with the Pres default timeout value.

 \param		varSubIndexId	Character pointer to hold the IndexID
 \param		varCheckValue	Character pointer to hold the new PresTimeout value

 \return	BOOL
 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool ValidateCNPresTimeout(char* varSubIndexId, char* varCheckValue)
{
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	bool retval = false;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", varSubIndexId,
			&iSubIndexPos, &iIndexPos);
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return retval;
	}

	INT32 iCNNodeId = HexToInt(varSubIndexId);
	stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			&iSubIndexPos, &iIndexPos);

	char* varCNDefaultValue = new char[50];
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
				DEFAULTVALUE, varCNDefaultValue);
	}
	else
	{
		delete[] varCNDefaultValue;
		return retval;
	}

	if (!(NULL == varCheckValue || strcmp(varCheckValue, "") == 0))
	{
		if (!(NULL == varCNDefaultValue || strcmp(varCNDefaultValue, "") == 0))
		{
			INT32 minimumDefaultValue = GetDecimalValue(varCNDefaultValue);
			INT32 iCheckValue = GetDecimalValue(varCheckValue);
			if (iCheckValue >= minimumDefaultValue)
			{
				retval = true;
			}
			else
			{
				retval = false;
			}
		}
		else
		{
			retval = false;
		}
	}
	else
	{
		retval = false;
	}

	delete[] varCNDefaultValue;
	return retval;
}

/*****************************************************************************/
/**
 \brief			CopyOldNodeIdAssignmentObject


 \param		pobjNode		Class pointer of the class Node
 \param		iOldNodeId		Integer variable to hold the old Node Id of a CN node

 \return	void
 */
/*****************************************************************************/

void CopyOldNodeIdAssignmentObject(Node* pobjNode, INT32 iOldNodeId)
{
	if (NULL == pobjNode)
	{
		return;
	}
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F81");
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F92");
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F9B");

}

/*****************************************************************************/
/**
 \brief			CopyOldNodeIdAssignmentObjectSubindex

This function sets the MN's corresponding subindex of the cn's node id with the actual value of the Mn's (cn's old node id subindex) subindex actual value
 
 \param		pobjNode		Class pointer of the class Node
 \param		iOldNodeId		Integer variable to hold the old Node Id of a CN node
 \param		varIndex		Character pointer to hold the IndexID

 \return	void
 */
/*****************************************************************************/

void CopyOldNodeIdAssignmentObjectSubindex(Node* pobjNode, INT32 iOldNodeId,
		char* varIndex)
{
	if ((NULL == varIndex) || (NULL == pobjNode))
	{
		return;
	}
	INT32 iCNNodeId = pobjNode->GetNodeId();
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	char* varCNNodeId = new char[10];
	varCNNodeId = IntToAscii(iCNNodeId, varCNNodeId, 16);
	varCNNodeId = PadLeft(varCNNodeId, '0', 2);
	char* varCNOldNodeId = new char[10];
	varCNOldNodeId = IntToAscii(iOldNodeId, varCNOldNodeId, 16);
	varCNOldNodeId = PadLeft(varCNOldNodeId, '0', 2);
	char* TempOldActualValue = NULL;

	IndexCollection* pobjMNIdxCol = NULL;
	pobjMNIdxCol = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(MN,
			MN_NODEID)->GetIndexCollection();
	Index *pobjMNIndex;
	SubIndex* pobjMNOldCNNodeIdSubIndex;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, varIndex, varCNOldNodeId,
			&iSubIndexPos, &iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		pobjMNIndex = pobjMNIdxCol->GetIndexbyIndexValue(varIndex);
		pobjMNOldCNNodeIdSubIndex = pobjMNIndex->GetSubIndexbyIndexValue(
				varCNOldNodeId);
		if (NULL != pobjMNOldCNNodeIdSubIndex)
		{
			Flag TempOldFlg = FALSE;
			if (NULL != (char*) pobjMNOldCNNodeIdSubIndex->GetActualValue())
			{
				TempOldActualValue = new char[strlen(
						(char*) pobjMNOldCNNodeIdSubIndex->GetActualValue())
						+ STR_ALLOC_BUFFER];
				strcpy((char*) TempOldActualValue,
						(char*) pobjMNOldCNNodeIdSubIndex->GetActualValue());
				TempOldFlg = pobjMNOldCNNodeIdSubIndex->GetFlagIfIncludedCdc();
			}
			stErrorInfo = IfSubIndexExists(MN_NODEID, MN, varIndex, varCNNodeId,
					&iSubIndexPos, &iIndexPos);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				//set the value alone
			}
			else
			{
				return;
			}

			SubIndex* pobjMNCNNodeIdSubIndex = NULL;
			pobjMNCNNodeIdSubIndex = pobjMNIndex->GetSubIndexbyIndexValue(
					varCNNodeId);
			if (NULL != pobjMNCNNodeIdSubIndex)
			{
				pobjMNCNNodeIdSubIndex->SetActualValue((char*) "");
			}

			if ((NULL != pobjMNCNNodeIdSubIndex)
					&& (NULL != TempOldActualValue))
			{
				//copy the actual value to here
				pobjMNCNNodeIdSubIndex->SetActualValue(TempOldActualValue);
				pobjMNCNNodeIdSubIndex->SetFlagIfIncludedCdc(TempOldFlg);
			}
			// reset the actual value of the old node id subindex
			SetSubIndexAttributesByAttribute(MN_NODEID, MN, varIndex,
					varCNOldNodeId, ACTUALVALUE, (char*) "");
		}
	}
	else
	{
		//no operation
	}
	delete[] varCNNodeId;
	delete[] varCNOldNodeId;
	delete[] TempOldActualValue;
}

/*****************************************************************************/
/**
 \brief		IsDefaultActualNotEqual

 This function checks the default value is same as the actual value or not

 \param		pBaseIndexObject	Class pointer of the class BaseIndex

 \return	BOOL

 \retval	TRUE			if default value equal to actual value
 \retval	FALSE			if default value not equal to actual value
 */
/*****************************************************************************/

bool IsDefaultActualNotEqual(BaseIndex* pBaseIndexObject)
{
	if (NULL == pBaseIndexObject || NULL == pBaseIndexObject->GetActualValue())
		return false;

	if (NULL == pBaseIndexObject->GetDefaultValue())
		return true;
	bool bReturnValue;
	DataType dt = pBaseIndexObject->GetDataType();
	if (dt.Name != NULL)
	{
		if (!CheckIfStringDatatypes(dt.Name))
		{
			INT32 iActualValue = 0;
			INT32 iDefaultValue = 0;
			iDefaultValue = GetDecimalValue(
					(char*) pBaseIndexObject->GetDefaultValue());
			iActualValue = GetDecimalValue(
					(char*) pBaseIndexObject->GetActualValue());
			if (iActualValue == iDefaultValue)
				bReturnValue = false;
			else
				bReturnValue = true;
		}
		else
		{
			if (0
					== strcmp(pBaseIndexObject->GetDefaultValue(),
							pBaseIndexObject->GetActualValue()))
				bReturnValue = false;
			else
				bReturnValue = true;
		}
	}
	else
	{
		if (0
				== strcmp(pBaseIndexObject->GetDefaultValue(),
						pBaseIndexObject->GetActualValue()))
			bReturnValue = false;
		else
			bReturnValue = true;
	}
	return bReturnValue;
}

/*****************************************************************************/
/**
 \brief		ReactivateMappingPDO

This function analyses the settings of a mapping object is returns the eligibility to enable/disable the mapping pdo

 \param		pobjIndexCol	Class pointer of the class IndexCollection
 \param		pobjIndex		Class pointer of the class Index

 \return	BOOL

 \retval	TRUE			if the mapping pdo can be reactivated
 \retval	FALSE			if the mapping pdo cannot be reactivated
 */
/*****************************************************************************/

bool ReactivateMappingPDO(IndexCollection* pobjIndexCol, Index* pobjIndex)
{
	if ((NULL == pobjIndexCol) || (NULL == pobjIndex))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	if (false == CheckIfMappingPDO((char*) pobjIndex->GetIndexValue()))
	{
		return false;
	}

	SubIndex *pobjSubIndex = NULL;
	pobjSubIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "00");
	if ((NULL != pobjSubIndex) && (NULL != pobjSubIndex->GetActualValue())
			&& (0 != strcmp(pobjSubIndex->GetActualValue(), "")))
	{
		if (false == CheckIfValueZero((char*) pobjSubIndex->GetActualValue()))
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < pobjIndex->GetNumberofSubIndexes();
					iLoopCount++)
			{
				if (pobjIndex->GetSubIndex(iLoopCount)->GetActualValue() != NULL
						&& TRUE
								== pobjIndex->GetSubIndex(iLoopCount)->GetFlagIfIncludedCdc()
						&& true
								== IsDefaultActualNotEqual(
										pobjIndex->GetSubIndex(iLoopCount)))
				{
					if (0
							== strcmp(
									pobjIndex->GetSubIndex(iLoopCount)->GetIndexValue(),
									"00"))
					{
						return true;
						//continue;
					}
					else //other than 00'th subindex
					{
						if (true
								== IsDefaultActualNotEqual(
										pobjIndex->GetSubIndex(iLoopCount)))
						{
							return true;
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
		else
		{
			if (false == IsDefaultActualNotEqual(pobjSubIndex))
			{
				return false;
			}

		}
	}

	char aCommParam[5];
	strcpy(aCommParam, (char*) pobjIndex->GetIndexValue());

	if (strncmp((char*) pobjIndex->GetIndexValue(), "1A", 2) == 0)
	{
		aCommParam[1] = '8';
	}
	else if (strncmp((char*) pobjIndex->GetIndexValue(), "16", 2) == 0)
	{
		aCommParam[1] = '4';
	}
	else
	{
		return false;
	}

	Index *pObjCommParam = NULL;
	pObjCommParam = pobjIndexCol->GetIndexbyIndexValue(aCommParam);
	if (NULL == pObjCommParam)
	{
		return false;
	}

	pobjSubIndex = pObjCommParam->GetSubIndexbyIndexValue((char*) "00");
	if ((NULL != pobjSubIndex) && (NULL != pobjSubIndex->GetActualValue())
			&& (0 != strcmp(pobjSubIndex->GetActualValue(), "")))
	{
		if (CheckIfValueZero((char*) pobjSubIndex->GetActualValue()))
			return false;
	}

	for (INT32 iLoopCount = 0;
			iLoopCount < pObjCommParam->GetNumberofSubIndexes(); iLoopCount++)
	{
		if (pObjCommParam->GetSubIndex(iLoopCount)->GetActualValue() != NULL
				&& TRUE
						== pObjCommParam->GetSubIndex(iLoopCount)->GetFlagIfIncludedCdc()
				&& true
						== CheckAccessTypeForInclude(
								(char*) pObjCommParam->GetSubIndex(iLoopCount)->GetAccessType())
				&& true
						== IsDefaultActualNotEqual(
								pObjCommParam->GetSubIndex(iLoopCount)))
		{
			return true;
		}
	}

	return false;
}

/*****************************************************************************/
/**
 \brief		ArrangeNodeIDbyStation

 This function sorts the node id by the station type

 \return	INT32*
 */
/*****************************************************************************/

INT32* ArrangeNodeIDbyStation(void)
{
	NodeCollection* objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node* pobjNode = NULL;

	/* Check RPDO Mapped objects*/
	INT32 iCNNodesCount = 0;

	iCNNodesCount = objNodeCol->GetCNNodesCount();

	if (iCNNodesCount == 0)
	{
		exit(0);
	}

	INT32 *piNodeIDColl = new INT32[iCNNodesCount];
	INT32 iArrangedNodeIDCount = 0;
	INT32 *piArrangedNodeIDColl = new INT32[iCNNodesCount];
	StationType *piStationTypeColl = new StationType[iCNNodesCount];
	INT32 iNodesCount = objNodeCol->GetNumberOfNodes();
	INT32 iLoopCount = 0;
	INT32 iCNNodeLoopCnt = 0;
	for (iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
	{
		pobjNode = objNodeCol->GetNodebyColIndex(iLoopCount);
		if (MN == pobjNode->GetNodeType())
		{
			continue;
		}
		piNodeIDColl[iCNNodeLoopCnt] = pobjNode->GetNodeId();
		piStationTypeColl[iCNNodeLoopCnt] = pobjNode->GetStationType();
		iCNNodeLoopCnt++;
	}
	//get the number of poll response station and other station
	INT32 iChainStnCnt = 0;
	INT32 iOtherStnCnt = 0;
	for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
	{
		if (CHAINED == piStationTypeColl[iLoopCount])
			iChainStnCnt++;
	}
	iOtherStnCnt = iCNNodesCount - iChainStnCnt;

	//create arrays to store nodeid for chained station and other station
	if (0 != iChainStnCnt)
	{
		INT32 *piChainStnColl = new INT32[iChainStnCnt];
		INT32 iChainStnLoopCnt = 0;
		for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
		{
			if (CHAINED == piStationTypeColl[iLoopCount])
			{
				piChainStnColl[iChainStnLoopCnt] = piNodeIDColl[iLoopCount];
				iChainStnLoopCnt++;
			}
		}
		//sort by station no
		SortNodeID(piChainStnColl, iChainStnCnt);

		//copy the poll response staion sorted in asscending order
		for (iLoopCount = 0; iLoopCount < iChainStnCnt; iLoopCount++)
		{
			piArrangedNodeIDColl[iArrangedNodeIDCount] =
					piChainStnColl[iLoopCount];
			iArrangedNodeIDCount++;
		}

		delete[] piChainStnColl;
	}

	if (0 != iOtherStnCnt)
	{
		INT32 *piOtherStnColl = new INT32[iOtherStnCnt];
		INT32 iOtherStnLoopCnt = 0;
		for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
		{
			if (CHAINED != piStationTypeColl[iLoopCount])
			{
				piOtherStnColl[iOtherStnLoopCnt] = piNodeIDColl[iLoopCount];
				iOtherStnLoopCnt++;
			}
		}
		//copy the other station after the poll response staion
		for (iLoopCount = 0; iLoopCount < iOtherStnCnt; iLoopCount++)
		{
			piArrangedNodeIDColl[iArrangedNodeIDCount] =
					piOtherStnColl[iLoopCount];
			iArrangedNodeIDCount++;
		}
		delete[] piOtherStnColl;
	}
	delete[] piNodeIDColl;
	delete[] piStationTypeColl;

	return piArrangedNodeIDColl;
}

/*****************************************************************************/
/**
 \brief		SortNodeID

This function sorts the node id in a given node id collection

 \param		piNodeIDColl	Integer pointer variable to hold the collection of all Nodes
 \param		iColSize		Integer variable to hold the collection size

 \return	void
 */
/*****************************************************************************/

void SortNodeID(INT32 *piNodeIDColl, INT32 iColSize)
{

	try
	{
		if (NULL == piNodeIDColl)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		INT32 iTemp = 0;
		for (INT32 iLoopCount = 0; iLoopCount < iColSize; iLoopCount++)
		{
			for (INT32 iSortCount = iLoopCount + 1; iSortCount <= iLoopCount;
					iSortCount++)
			{
				if (piNodeIDColl[iLoopCount] > piNodeIDColl[iSortCount])
				{
					iTemp = piNodeIDColl[iLoopCount];
					piNodeIDColl[iLoopCount] = piNodeIDColl[iSortCount];
					piNodeIDColl[iSortCount] = iTemp;
				}
			}
		}
	} catch (ocfmException& ex)
	{
		throw ex;
	}
}

/*****************************************************************************/
/**
 \brief		IsPresMN

 This function checks for the MN node is a Pres(chained station) or not

 \return	BOOL

 \retval		TRUE			if pres MN
 \retval		FALSE			if not a pres MN
 */
/*****************************************************************************/

bool IsPresMN()
{
	NodeCollection *objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node *pobjNode = NULL;
	INT32 iLoopCount;
	bool bPres = false;
	INT32 iNodesCount = objNodeCol->GetNumberOfNodes();
	for (iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
	{
		pobjNode = objNodeCol->GetNodebyColIndex(iLoopCount);
		if (MN == pobjNode->GetNodeType())
		{
			continue;
		}
		if (CHAINED == pobjNode->GetStationType())
		{
			//return true;
			bPres = true;
		}
	}
	return bPres;
}

/*****************************************************************************/
/**
 \brief		SetCNLossObjects

 This function sets values to 1COB, 1C0C and 1C0D objects

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType	Enum to hold the Node type of the node

 \return	void
 */
/*****************************************************************************/

void SetCNLossObjects(INT32 iNodeID, NodeType varNodeType)
{
	if (varNodeType != CN)
	{
		return;
	}

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;
	IndexCollection* pobjIdxCol = NULL;
	Index *pobjIndex = NULL;
	SubIndex* pobjSIndex = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(varNodeType, iNodeID);
	pobjIdxCol = pobjNode->GetIndexCollection();

	//loss of SoC
	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1C0B");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->GetActualValue() == NULL)
					|| (strcmp(pobjSIndex->GetActualValue(), "") == 0))
			{
				pobjSIndex->SetActualValue((char*) "0x50");
				pobjSIndex->SetFlagIfIncludedCdc(TRUE);
				pobjIndex->SetFlagIfIncludedCdc(TRUE);
			}
		}
	}

	//loss of SoA
	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1C0C");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->GetActualValue() == NULL)
					|| (strcmp(pobjSIndex->GetActualValue(), "") == 0))
			{
				pobjSIndex->SetActualValue((char*) "0x50");
				pobjSIndex->SetFlagIfIncludedCdc(TRUE);
				pobjIndex->SetFlagIfIncludedCdc(TRUE);
			}
		}
	}

	//loss of PReq
	pobjIndex = pobjIdxCol->GetIndexbyIndexValue((char*) "1C0D");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->GetActualValue() == NULL)
					|| (strcmp(pobjSIndex->GetActualValue(), "") == 0))
			{
				pobjSIndex->SetActualValue((char*) "0x50");
				pobjSIndex->SetFlagIfIncludedCdc(TRUE);
				pobjIndex->SetFlagIfIncludedCdc(TRUE);
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		SetBuildTime

 Gets the system time during time of build - to be called from ProcessPDOnodes

 \return	void
 */
/*****************************************************************************/

void SetBuildTime()
{
	time(&sBuildTime_g.rawtime);
	sBuildTime_g.timeinfo = localtime(&sBuildTime_g.rawtime);
	strftime(sBuildTime_g.buffer, BUILDTIME_BUF_LEN, "%d-%b-%Y %H:%M:%S",
			sBuildTime_g.timeinfo);
}

/*****************************************************************************/
/**
 \brief		GetBuildTime

 Gets the calculated system time during time of build to be called from API generating interface files

 \return	const char*
 */
/*****************************************************************************/
const char* GetBuildTime()
{
	return (const char*) &sBuildTime_g.buffer;
}

