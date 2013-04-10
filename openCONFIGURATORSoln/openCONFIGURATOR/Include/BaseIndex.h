/**
 ************************************************************************************************
 \file			BaseIndex.h

 \brief			BaseClass for CIndex/CSubIndex
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

 ***********************************************************************************************/
#ifndef BaseIndex_h
#define BaseIndex_h

/***************************************************************************************************/
/*  Includes  */

#include "TCollection.h"
#include "Declarations.h"

/**
 *****************************************************************************************************
 \class			CBaseIndex
 \brief			This is the BaseClass for CIndex/CSubIndex

 ****************************************************************************************************/

class BaseIndex
{
	public:
		BaseIndex(void);
		~BaseIndex(void);



	public:

		const char* GetName();
		void SetName(char* varName);

		const char* GetIndexValue();
		void SetIndexValue(char* varIndex);

		const char* GetDefaultValue();
		void SetDefaultValue(char* varValue);

		const char* GetActualValue();
		void SetActualValue(char* varValue);

		const char* GetAccessType();
		void SetAccessType(char* varAccess);

		const char* GetLowLimit();
		void SetLowLimit(char* varLowLimit);

		const char* GetHighLimit();
		void SetHighLimit(char* varHighLimit);

		DataType GetDataType();
		void SetDataType(char* dataTypeValue);
		void SetDataTypeName(char* dataTypeName, INT32 varNodeId);

		const char* GetPDOMapping();
		void SetPDOMapping(char* pdoMappingValue);

		const char* GetObjectType();
		ObjectType GetEObjectType();
		void SetObjectType(char* objectType);

		INT32 GetNodeID();
		void SetNodeID(INT32 NodeID);

		bool IsIndexVaueValid(char* varValue);
		void SaveChanges(char* varName, char* varValue);

		char* GetUniqueIDRef();
		void SetUniqueIDRef(char* uniqueID);

		char* GetDataTypeValue();
		void SetDataTypeValue(char* varValue);

		Flag GetFlagIfIncludedCdc();
		void SetFlagIfIncludedCdc(Flag varFlag);

		void SetDataTypeST(DataType varDt);

	protected:
			const char* Name;
			const char* Index;
			const char* lowLimit;
			const char* highLimit;
			ObjectType objectType;
			DataType dataType;
			const char* accessType;
			const char* defaultValue;
			char* actualValue;
			PDOMapping pdoMapping;
			INT32 nodeId;
			char* uniqueIdRef;
			INT32 parameterIndex;
			char* dataTypeValue;/* Only used for ObjectDictionary*/
			Flag isIncludedInCDC;
};
#endif //BaseIndex_h
