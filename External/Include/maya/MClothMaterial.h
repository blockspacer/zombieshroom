#ifndef _MClothMaterial
#define _MClothMaterial
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
// ****************************************************************************
//
// CLASS:    MClothMaterial
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MVector.h>

// MayaCloth Export
#ifdef _WIN32
#	ifndef MAYACLOTH_EXPORT
#		ifdef MAYACLOTH_DLL
#			define MAYACLOTH_EXPORT _declspec( dllexport )
#		else
#			define MAYACLOTH_EXPORT _declspec( dllimport )
#		endif // MAYACLOTH_DLL
#	endif // MAYACLOTH_EXPORT
#elif defined(AW_HIDE_SYMBOLS) && defined(MAYACLOTH_DLL)
#	ifndef MAYACLOTH_EXPORT
#		define MAYACLOTH_EXPORT __attribute__ ((visibility("default")))
#	endif
#else
#	ifndef MAYACLOTH_EXPORT
#		define MAYACLOTH_EXPORT
#	endif
#endif // _WIN32

// ****************************************************************************
// CLASS DECLARATION (MClothMaterial)

/*!
\internal
\ingroup cloth
\brief Access to various cloth material properties.

This class provides access to various cloth material
properties. Cloth solver writers can override this
class to get custom material properties.
*/
class MAYACLOTH_EXPORT MClothMaterial
{
public:
	MClothMaterial();
	virtual ~MClothMaterial();

	virtual MClothMaterial& operator += ( MClothMaterial& other );
	virtual MClothMaterial& operator *= ( double value );
	virtual bool equal( MClothMaterial* other );
	
	virtual void update();
	virtual void setupRelaxProperty();

public:
	double density;
	double uStretchResistance;
	double vStretchResistance;
	double uScale;
	double vScale;
	double uBend;
	double vBend;
	double uBendRate;
	double vBendRate;
	double shear;
	double airDamping;
	double clothDamping;
	double clothFriction;
	double thickness;
	double mat_thickness;
	double thicknessForce;
	double staticFriction;
	double dynamicFriction;
	MVector gravity;

protected:
	// No protected members

private:
	// No private members
};

#endif /* __cplusplus */
#endif /* _MClothMaterial */
