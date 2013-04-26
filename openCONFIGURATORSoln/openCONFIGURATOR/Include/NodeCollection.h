/**
 *****************************************************************************
 \file		NodeCollection.h

 \brief		This file contains the class declaration to manage the inclusion and retrieval of Nodes in the collection list
 *****************************************************************************
 */

/*
 � Kalycito Infotech Private Limited

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
#ifndef NodeCollection_h
#define NodeCollection_h

/****************************************************************************/
/* Includes */

#include "Declarations.h"
#include "Node.h"

/**
 *****************************************************************************
 \class		NodeCollection
 \brief		Handles collection of each node's information on id,name,type present inside the network
 
 *****************************************************************************/
class NodeCollection
{
	public:
		NodeCollection(void);
		~NodeCollection(void);

	public:

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the count of Node objects in the collection list
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNumberOfNodes();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add a Node object to a collection list and update the count
		 
		 \param		nodeObj       Class Variable of Node to hold the Node object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddNode(Node nodeObj);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the Node objects at given position in the collection list
		 
		 \param		nodePos    Integer to hold the position of Node object to be deleted
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteNode(INT32 nodePos);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the pointer to NodeCollection object to be used as the collection list
		 
		 \return	NodeCollection*
		 */
		/*****************************************************************************/
		static NodeCollection* GetNodeColObjectPointer();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object of given type and ID from the collection list
		 
		 \param		nodeType  	Enum variable of NodeType to hold the value of Node type
		 \param		nodeId		Integer to hold the value of Node id
		 
		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNode(NodeType nodeType, INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object of ID from the collection list
		 
		 \param		nodeId		Integer to hold the value of Node ID
		 
		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNode(INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object at the given position from the collection list
		 
		 \param		position    Integer to hold value of column index
		 
		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNodebyCollectionIndex(INT32 position);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the MN Node object from the collection list
		 
		 \return	Node
		 */
		/*****************************************************************************/
		Node GetMNNode();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return pointer to the Node object of given type and ID from the collection list

		 \param		nodeType  	Enum variable of NodeType to hold the value of Node type
		 \param		nodeId		Integer to hold the value of Node id

		 \return	Node* / NULL
		 */
		/*****************************************************************************/
		Node* GetNodePtr(NodeType nodeType, INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return pointer to the Node object at the given position from the collection list
		 
		 \param		position   Integer to hold value of column index
		 
		 \return	Node*
		 */
		/*****************************************************************************/
		Node* GetNodebyColIndex(INT32 position);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the count of CN objects in the collection list
		 
		 This is a member function of CNodeCollection collects the CN node count 

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetCNNodesCount();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the NodeCollection object to be used as the collection list
		 
		 \return	NodeCollection
		 */
		/*****************************************************************************/
		static NodeCollection GetNodeColObject();

	private:
		static bool instanceFlag;
		static NodeCollection *objNodeColl;
		TCollection<Node> nodeCollObj;
		INT32 nodeCount;
};
#endif // NodeCollection_h
