#ifndef _MFnNumericAttribute
#define _MFnNumericAttribute
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
// CLASS:    MFnNumericAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MObject.h>

// ****************************************************************************
// DECLARATIONS

class MString;

// ****************************************************************************
// CLASS DECLARATION (MFnNumericAttribute)

//! \ingroup OpenMaya MFn
//! \brief Numeric attribute function set. 
/*!

  MFnNumericAttribute is the function set for dependency node
  attributes that store one, two or three untyped numeric values. A
  numeric attribute supports both a "hard" maximum/minimum and a soft
  maximum/minimum. The soft maximum/minimum are used by the Attribute
  Editor as the limits on its slider, but values within the hard range
  can be typed in the input box.

  As a convenience, MFnNumericAttribute will set the flags of child
  attributes as well when the flags of the parent attribute are set.
  This is important when creating a numeric pair or triplet.  For
  example, if MFnNumericAttribute is used to create a 3 double
  attribute and the attribute is set to be keyable, then the three
  child attributes will also be marked as keyable.
*/
class OPENMAYA_EXPORT MFnNumericAttribute : public MFnAttribute
{
	declareMFn(MFnNumericAttribute, MFnAttribute);

public:
	MObject 	create( const MString& fullName,
						const MString& briefName,
						MFnNumericData::Type unitType,
						double defaultValue =0,
						MStatus* ReturnStatus = NULL );
	MObject 	createColor( const MString& fullName,
							 const MString& briefName,
							 MStatus* ReturnStatus = NULL );
	MObject 	createPoint( const MString& fullName,
							 const MString& briefName,
							 MStatus* ReturnStatus = NULL );
	MObject 	createAddr( const MString& fullName,
							const MString& briefName,
                            void* defaultValue = NULL,
							MStatus* ReturnStatus = NULL );
	MObject	    create( const MString& fullName,
						const MString& briefName,
						const MObject & child1,
						const MObject & child2,
						const MObject & child3 = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	MFnNumericData::Type	unitType( MStatus* ReturnStatus = NULL ) const;
	MObject 	child( unsigned int index, MStatus* ReturnStatus = NULL ) const;
	bool		hasMin( MStatus* ReturnStatus = NULL) const;
	bool		hasMax( MStatus* ReturnStatus = NULL) const;
	bool        hasSoftMin( MStatus* ReturnStatus = NULL) const;
	bool        hasSoftMax( MStatus* ReturnStatus = NULL) const;
	MStatus		getMin( double& min1 ) const;
	MStatus		getMin( double& min1, double& min2 ) const;
	MStatus		getMin( double& min1, double& min2, double& min3 ) const;
	MStatus		getMax( double& max1 ) const;
	MStatus		getMax( double& max1, double& max2 ) const;
	MStatus		getMax( double& max1, double& max2, double& max3 ) const;
	MStatus		getSoftMin( double& min1 ) const;
	MStatus		getSoftMax( double& max1 ) const;
	MStatus		setMin( double min1 );
	MStatus		setMin( double min1, double min2 );
	MStatus		setMin( double min1, double min2, double min3 );
	MStatus		setMax( double max1 );
	MStatus		setMax( double max1, double max2 );
	MStatus		setMax( double max1, double max2, double max3 );
	MStatus		setSoftMin( double min1 );
	MStatus		setSoftMax( double max1 );
	MStatus		getDefault( bool & def1 );
	MStatus		getDefault( char & def1 );
	MStatus		getDefault( int & def1 );
	MStatus		getDefault( int & def1, int & def2 );
	MStatus		getDefault( int & def1, int & def2, int & def3 );
	MStatus		getDefault( float & def1 );
	MStatus		getDefault( float & def1, float & def2 );
	MStatus		getDefault( float & def1, float & def2, float & def3 );
	MStatus		getDefault( double & def1 );
	MStatus		getDefault( double & def1, double & def2 );
	MStatus		getDefault( double & def1, double & def2, double & def3 );
	MStatus		getDefault( double & def1, double & def2, double & def3, double & def4 );
	MStatus		getDefault( void*& def1 );
	MStatus		setDefault( bool def1 );
	MStatus		setDefault( char def1 );
	MStatus		setDefault( int def1 );
	MStatus		setDefault( int def1, int def2 );
	MStatus		setDefault( int def1, int def2, int def3 );
	MStatus		setDefault( float def1 );
	MStatus		setDefault( float def1, float def2 );
	MStatus		setDefault( float def1, float def2, float def3 );
	MStatus		setDefault( double def1 );
	MStatus		setDefault( double def1, double def2 );
	MStatus		setDefault( double def1, double def2, double def3 );
	MStatus		setDefault( double def1, double def2, double def3, double def4 );
	MStatus		setDefault( void* def1 );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnNumericAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:

	virtual bool objectChanged( MFn::Type, MStatus * );

	MObject 	internalCreate( const MString& fullName,
						const MString& briefName,
						MFnNumericData::Type unitType,
								const char** childSuffixes,
						double defaultValue,
						MStatus* ReturnStatus);
};

#endif /* __cplusplus */
#endif /* _MFnNumericAttribute */
