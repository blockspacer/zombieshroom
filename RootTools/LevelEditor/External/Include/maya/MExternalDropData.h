#ifndef _MExternalDropData
#define _MExternalDropData
//-
// ==========================================================================
// Copyright (C) 2010 Autodesk, Inc., and/or its licensors.  All
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

#if defined __cplusplus

// ****************************************************************************


#include <maya/MStatus.h>
#include <maya/MTypes.h>

class MColor;
class MImage;
class MString;
class MStringArray;

// ****************************************************************************

//! \ingroup OpenMayaUI
//! \brief Drag-and-drop data, used with MExternalDropCallback.
/*!
MExternalDropData is the data that a drag-and-drop operation carries if dragging from an external application and dropping onto Maya. 

It typically arrives from a MExternalDropCallback callback method.
*/
class OPENMAYAUI_EXPORT MExternalDropData
{
public:
	enum MouseButtons {
		kLeftButton		= 0x00000001,
		kRightButton	= 0x00000002,
		kMidButton		= 0x00000004,
		kMiddleButton	= kMidButton
	};

	enum KeyboardModifiers {
		kNoModifier			= 0x00000000,
		kShiftModifier		= 0x02000000,
		kControlModifier	= 0x04000000,
		kAltModifier		= 0x08000000
	};

	bool hasText() const;
	MString text() const;

	bool hasUrls() const;
	MStringArray urls() const;

	bool hasHtml() const;
	MString html() const;

	bool hasColor() const;
	MColor color() const;

	bool hasImage() const;
	MImage image() const;

	MStringArray formats() const;
	bool hasFormat( const MString& format ) const;

	unsigned int dataSize( const MString& format ) const;
BEGIN_NO_SCRIPT_SUPPORT:
	MStatus copyData( const MString& format, void* buffer, unsigned int bufferSize ) const;
END_NO_SCRIPT_SUPPORT:

	int mouseButtons() const;
	int keyboardModifiers() const;

protected:

private:
	MExternalDropData( const void* data, int mouseButtons, int keyModifiers );

	const void*	fData;
	int fMouseButtons;
	int fKeyModifiers;
};

#endif /* __cplusplus */
#endif /* _MExternalDropData */
