#ifndef _MTimer
#define _MTimer
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
// CLASS:    MTimer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MTimer)
//
// The MTimer class works similar to timerX MEL command.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// ****************************************************************************
// CLASS DECLARATION (MTimer)

//! \ingroup OpenMaya
//! \brief This class works similar to timerX MEL command.
/*!
  MTimer class provides the functionality for calculating the time
  elapsed/consumed in executing a particular portion of the code. The
  portion of the code to be timed is embedded inbetween the calls
  to'beginTimer()' and 'endTimer()'. A call to 'elapsedTime()' gives
  the time spent in executing that portion of the code.  The
  calculated time in seconds will be with 2 decimal accuracy.
*/
class OPENMAYA_EXPORT MTimer
{

public:
					MTimer();
					MTimer( const MTimer & );
					~MTimer();

					void beginTimer();
					void endTimer();
					double elapsedTime();
	MTimer&			operator =  ( const MTimer& rhs );
	bool			operator == ( const MTimer& rhs ) const;
	bool			operator != ( const MTimer& rhs ) const;
	void			clear();

private:
	double				getCurrentTime();

	double				startTime;
	double				endTime;

};

#endif /* __cplusplus */
#endif /* _MTimer */
