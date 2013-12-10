#ifndef _MContainerMessage
#define _MContainerMessage
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MContainerMessage
//
// ****************************************************************************
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MMessage.h>

// ****************************************************************************
// DECLARATIONS

class MPlug;
class MObject;
class MDGModifier;
class MDagPath;

// ****************************************************************************
// CLASS DECLARATION (MContainerMessage)

//! \ingroup OpenMaya
//! \brief Container-related messages.
/*!
    The container node in Maya bundles together a group of related nodes.
	The user can then publish attribute names to the container, and bind
	attributes from nodes in the container to the published names.

	MContainerMessage class allows the API user to register callbacks that
	inform them about changes to published attributes on containers.

    To remove a callback use MMessage::removeCallback.  All callbacks
    that are registered by a plug-in must be removed by that plug-in
    when it is unloaded. Failure to do so will result in a fatal
    error.
*/
class OPENMAYA_EXPORT MContainerMessage : public MMessage
{
public:

	static MCallbackId	addPublishAttrCallback(
								MMessage::MNodeStringBoolFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addBoundAttrCallback(
								MMessage::MNodeStringBoolFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

protected:
	// No protected.
};

#endif /* __cplusplus */
#endif /* _MContainerMessage */
