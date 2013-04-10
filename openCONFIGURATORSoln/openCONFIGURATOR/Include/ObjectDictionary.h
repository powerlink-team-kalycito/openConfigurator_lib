/**
 *****************************************************************************************************
 \file			ObjectDictionary.h

 \brief			Handles the object dictionary for each index
 *****************************************************************************************************
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
#ifndef ObjectDictionary_h
#define ObjectDictionary_h

/****************************************************************************************************/
/* Includes */

#include <libxml/xmlreader.h>
#include "Index.h"
#include "DataTypeCollection.h"
#include "IndexCollection.h"
#include "Declarations.h"
#include "Node.h"
#include "TCollection.h"
#include "Internal.h"

/**
 ******************************************************************************************************
 \class			CObjectDictionary
 \brief			Handles the object dictionary for each index	
 
 ******************************************************************************************************/
class ObjectDictionary
{
	public:
		ObjectDictionary(void);
		~ObjectDictionary(void);
	private:
		static bool instanceFlag;
		INT32 varsattrIdxSIdx;

		enum ObjectType
		{
			INDEX = 0, SUBINDEX
		};

		typedef struct sattrIdxSIdx
		{
				char* Idx;
				char* startIndex;
				char* endIndex;
				ObjectType objectType;
		} sattrIdxSIdx;

		TCollection<sattrIdxSIdx> collectionObj;

	public:
		static Node* objDictNode;
		static ObjectDictionary* objectDictionary;

	public:
		void CheckIfSameIndex(char* Index);
		Index* GetIndexDictAttribues(char* Index);
		void ProcessObjectDictionary(xmlTextReaderPtr reader);
		static ObjectDictionary* GetObjDictPtr();
		void AddSameAttributesObjects(sattrIdxSIdx object);
		void CreateSameattrObject(char* value, ObjectType objType, char*Idx);
		Index* GetObjectDictIndex(char* Idx);
		SubIndex* GetObjectDictSubIndex(char* Idx, char* sIdx);
		INT32 IfObjectDictIndexExists(char* Index);
		INT32 IfObjectDictSubIndexExists(char* Idx, char* sIdx);
		bool CheckInTheRange(char* Idx, char* startIdx, char* endIdx);
		void Printall();
		char* GetIndexName(char* objectIndex, char* objectName);
};
#endif // ObjectDictionary_h
