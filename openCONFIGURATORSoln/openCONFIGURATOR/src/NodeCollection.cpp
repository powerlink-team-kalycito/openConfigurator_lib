/**
 ************************************************************************************************
 \file			NodeCollection.cpp

 \brief			Collects node id,count,location of all nodes present in the network
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

#include <stdio.h>
#include "../Include/NodeCollection.h"
#include "../Include/Node.h"
//$S Only for testing

/****************************************************************************************************/
/* Global Variables */

bool NodeCollection::instanceFlag = false;
NodeCollection* NodeCollection::objNodeCollection = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

NodeCollection::NodeCollection(void)
{
	NodeCount = collectionObj.Count();
}

/*************************************************************************/
/* Destructor */

/**
 

 */

NodeCollection::~NodeCollection(void)
{
	instanceFlag = false;
}

/*****************************************************************************/
/**
 \brief			GetNumberOfNodes
 
 This is a member function of CNodeCollection returns node count

 \return	INT32
 */
/*****************************************************************************/

INT32 NodeCollection::GetNumberOfNodes()
{
	return NodeCount;
}

/*****************************************************************************/
/**
 \brief			AddNode
 
 This is a member function of CNodeCollection collects objects under node count
 
 \param			objNode        Class Variable of CNode to hold the value of object node
 \return	void
 */
/*****************************************************************************/

void NodeCollection::AddNode(Node objNode)
{
	INT32 iItemPosition = collectionObj.Add();

	collectionObj[iItemPosition] = objNode;
	NodeCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			DeleteNode
 
 This is a member function of CNodeCollection deletes the node ids
 
 \param			iNodeID    Integer Variable holds the value of Node ID
 \return	void
 */
/*****************************************************************************/

void NodeCollection::DeleteNode(INT32 iNodeID)
{
	collectionObj.Remove(iNodeID);
	NodeCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			GetNodeColObjectPointer
 
 This is a member function of CNodeCollection collects the location of node
 
 \return	NodeCollection*
 */
/*****************************************************************************/

NodeCollection* NodeCollection::GetNodeColObjectPointer()
{
	if (!instanceFlag)
	{
		objNodeCollection = new NodeCollection();
		instanceFlag = true;
	}
	return objNodeCollection;
}

/*****************************************************************************/
/**
 \brief			GetNodeColObject
 
 This is a member function of CNodeCollection collects value of node objects
 
 \return	NodeCollection
 */
/*****************************************************************************/

NodeCollection NodeCollection::GetNodeColObject()
{
	if (!instanceFlag)
	{
		objNodeCollection = new NodeCollection();
		instanceFlag = true;
	}
	return *objNodeCollection;
}

/*****************************************************************************/
/**
 \brief			GetNode
 
 This is a member function of CNodeCollection the type of node objects

 \param			varNodeType  Enum Variable of ENodeType to hold the value of Enum Node type
 \param			iNodeID		  Integer Variable to hold the value of Node id		
 \return	Node
 */
/*****************************************************************************/

Node NodeCollection::GetNode(NodeType varNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	Node objNode;

	for (iLoopCount = 0; iLoopCount < NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.GetNodeType() == varNodeType
				&& objNode.GetNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}

/*****************************************************************************/
/**
 \brief			GetNodePtr

 This is a member function of CNodeCollection collects the location of node objects type 

 \param			varNodeType  Enum Variable of ENodeType to hold the value of Enum Node type
 \param			iNodeID		  Integer Variable to hold the value of Node id					

 \return	Node* / NULL
 */
/*****************************************************************************/

Node* NodeCollection::GetNodePtr(NodeType varNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	Node* objNode = NULL;

	for (iLoopCount = 0; iLoopCount < NodeCount; iLoopCount++)
	{
		objNode = &(objNodeCollection->collectionObj[iLoopCount]);

		if (objNode->GetNodeType() == varNodeType
				&& objNode->GetNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			GetNode
 
 This is a member function of CNodeCollection collects the type of node objects 

 \param			iNodeID			Integer Variable to hold the value of Node id		
 \return	Node
 */
/*****************************************************************************/
//TODO: unused function
Node NodeCollection::GetNode(INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	Node objNode;

	for (iLoopCount = 0; iLoopCount < NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.GetNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}

/*****************************************************************************/
/**
 \brief			GetMNNode
 
 This is a member function of CNodeCollection collects the type of node objects for MN

 \return	Node
 */
/*****************************************************************************/

Node NodeCollection::GetMNNode()
{
	INT32 iLoopCount;
	Node objNode;

	for (iLoopCount = 0; iLoopCount < NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.GetNodeType() == MN)
		{
			return objNode;
		}

	}
	return objNode;
}

/*****************************************************************************/
/**
 \brief			GetNodebyCollectionIndex
 
 This is a member function of CNodeCollection returns node from the node collection index

 \param			iColIndex    Integer Variable to hold value of column index
 \return	Node
 */
/*****************************************************************************/

Node NodeCollection::GetNodebyCollectionIndex(INT32 iColIndex)
{
	return collectionObj[iColIndex];
}

/*****************************************************************************/
/**
 \brief			GetNodebyColIndex
 
 This is a member function of CNodeCollection collects the node with reference to the index location 

 \param			iColIndex   Integer Variable to hold value of column index
 \return	Node*
 */
/*****************************************************************************/

Node* NodeCollection::GetNodebyColIndex(INT32 iColIndex)
{
	return &collectionObj[iColIndex];
}

/*****************************************************************************/
/**
 \brief			GetCNNodesCount
 
 This is a member function of CNodeCollection collects the CN node count 

 \return	INT32
 */
/*****************************************************************************/

INT32 NodeCollection::GetCNNodesCount()
{
	INT32 iCNNodeCount = 0;

	for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		if (collectionObj[iLoopCount].GetNodeType() == CN)
		{
			iCNNodeCount++;
		}
	}
	return iCNNodeCount;
}
