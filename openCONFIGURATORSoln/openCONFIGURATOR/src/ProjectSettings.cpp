/**
 ************************************************************************************************
 \file			ProjectSettings.cpp

 \brief			Handles project settings such as view mode,new settings,generate,ip address.
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
#include "../Include/ProjectSettings.h"
#include "../Include/Internal.h"

using namespace std;

/****************************************************************************************************/
/* Global Variables */

bool PjtSettings::instanceFlag = false;
PjtSettings* PjtSettings::objPjtSettings = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

PjtSettings::PjtSettings(void)
{
#if defined DEBUG
	cout << "\nPjtSettings Objects Initialized!\n" << endl;
#endif
	saveMode = DISCARD_AS;
	generateMode = NO_AG;
	IPopenPOWERLINK = NULL;
	varbExpertViewSelected = false;
	viewMode = SIMPLE; //TODO: Review initialisation
}

/*************************************************************************/
/* Destructor */

/**
 

 */

PjtSettings::~PjtSettings(void)
{
	instanceFlag = false;
#if defined DEBUG
	cout << "\n\n\n\nPjtSettings Object Deleted!\n" << endl;
#endif
}

/*****************************************************************************/
/**
 \brief			GetPjtSettingsPtr
 
 This is a member function of CPjtSettings creats new Pjtsettings 

 \return		PjtSettings*
 */
/*****************************************************************************/

PjtSettings* PjtSettings::GetPjtSettingsPtr()
{
	if (!instanceFlag)
	{
		objPjtSettings = new PjtSettings();
		instanceFlag = true;
#if defined DEBUG
		cout << "\n\n\n\nNew PjtSettings Object Created!\n" << endl;
#endif
	}
	return objPjtSettings;
}

/*****************************************************************************/
/**
 \brief			GetSaveAttr
 
 This is a member function of CPjtSettings saves new Pjtsettings  

 \return		AutoSave
 */
/*****************************************************************************/

AutoSave PjtSettings::GetSaveAttr()
{
	return saveMode;
}

/*****************************************************************************/
/**
 \brief			SetSaveAttr
 
 This is a member function of CPjtSettings sets mode for pjt settings 
 
 \param			autoSaveMode   Enum Variable of EAutoSave to save the attributes
 \return		void
 */
/*****************************************************************************/

void PjtSettings::SetSaveAttr(AutoSave autoSaveMode)
{
	saveMode = autoSaveMode;
}

/*****************************************************************************/
/**
 \brief			GetGenerateAttr
 
 This is a member function of CPjtSettings generates mode for pjt settings    

 \return		AutoGenerate
 */
/*****************************************************************************/

AutoGenerate PjtSettings::GetGenerateAttr()
{
#if defined DEBUG
	//cout << "\n Returning m_generateMode:-" << m_generateMode <<endl;	
#endif	
	return generateMode;
}

/*****************************************************************************/
/**
 \brief		SetGenerateAttr
 
 This is a member function of CPjtSettings sets mode for pjt settings  
 
 \param		autoGenerateMode	 Enum Variable of EAutoGenerate to generate attribute
 
 \return	void
 */
/*****************************************************************************/

void PjtSettings::SetGenerateAttr(AutoGenerate autoGenerateMode)
{
	generateMode = autoGenerateMode;
}

/*****************************************************************************/
/**
 \brief			SetPOWERLINKIP
 
 This is a member function of CPjtSettings assigns IP address for Powerlink  
 
 \param			ipAddr   Character Pointer variable to hold IPaddress
 
 \return		void
 */
/*****************************************************************************/

void PjtSettings::SetPOWERLINKIP(char* ipAddr)
{
//TODO: review
	if (NULL != IPopenPOWERLINK)
	{
		delete[] IPopenPOWERLINK;
	}

	IPopenPOWERLINK = new char[strlen(ipAddr) + STR_ALLOC_BUFFER];
	strcpy((char*) IPopenPOWERLINK, ipAddr);

}

/*****************************************************************************/
/**
 \brief			GetPOWERLINKIP
 
 This is a member function of CPjtSettings returns Powerlink IP  
 
 \return		const char*
 */
/*****************************************************************************/

//TODO: Unused Function
const char* PjtSettings::GetPOWERLINKIP()
{
	if (NULL != IPopenPOWERLINK)
	{
		return IPopenPOWERLINK;
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			GetViewMode
 
 This is a member function of CPjtSettings sets view mode under pjt settings  
 
 \return		ViewMode
 */
/*****************************************************************************/

ViewMode PjtSettings::GetViewMode()
{
	return viewMode;
}

/*****************************************************************************/
/**
 \brief			SetViewMode
 
 This is a member function of CPjtSettings assigns view mode  
 
 \param			varViewMode	Enum Variable of EViewMode to hold the value of mode type			
 \return		void
 */
/*****************************************************************************/

void PjtSettings::SetViewMode(ViewMode varViewMode)
{
	viewMode = varViewMode;
}

/*****************************************************************************/
/**
 \brief			GetExpertViewSelectedFlag
 
 This is a member function of CPjtSettings sets flag for type of view selected under pjt settings  
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool PjtSettings::GetExpertViewSelectedFlag()
{
	return varbExpertViewSelected;
}

/*****************************************************************************/
/**
 \brief			SetExpertViewSelectedFlag
 
 This is a member function of CPjtSettings assigns type of view selected under pjt settings 
 
 \param			bExpertViewSelected	 Boolean Variable to hold the value of Expertview selected			
 \return		void
 */
/*****************************************************************************/

void PjtSettings::SetExpertViewSelectedFlag(bool bExpertViewSelected)
{
	varbExpertViewSelected = bExpertViewSelected;
}

