#ifndef _MUniformParameter
#define _MUniformParameter
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
// The Data may not be disclosed or MGeometryList to third parties or be
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
// CLASS:    MUniformParameter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MPlug.h>
class MUniform;
class MGeometryList;

namespace MHWRender {
	class MDrawContext;
};

// ****************************************************************************
// CLASS DECLARATION (MUniformParameter)

//! \ingroup OpenMayaRender
//! Uniform parameter.
/*!
The MUniformParameter class provides a high-level interface to
hardware shader uniform parameters. By defining your shader's uniform
parameters through this class, you allow Maya to handle the
attributes, editing, serialisation, and caching for you in a standard
way that ensure you'll be able to leverage future performance and
functionlity improvements.

At setup time (either initial load or when the effect/technique is
changed), your shader simply creates the list of parameters it
requires, specifying the name, type, semantic of the parameters. At
render time, you can then use the parameters to directly access the
appropriate buffers for that surface data.

If you include a custom Attribute Editor template for your shader
node, you can include these surface parameters by calling the
AEhwUniformTemplateParameters script function. The following sample
code provides a basic template you can modify - however your AE
template can use as much or as little of this as you like:

\code
global int $yourUniformNameTemplateInitialised = 0;

global proc AEyourUniformNameTemplate ( string $node )
{
	global int $yourUniformNameTemplateInitialised;
	if( $yourUniformNameTemplateInitialised == 0)
	{
		source "AEhwUniformTemplate.mel";
		$yourUniformNameTemplateInitialised = 1;
	}

	AEhwUniformTemplateHeader( $node);

	// Insert custom attributes between the swatch and the parameters here

	AEhwUniformTemplateParameters( $node);

	// Insert custom attributes after the parameters here

	AEhwUniformTemplateFooter( $node);
}

\endcode
*/
class OPENMAYARENDER_EXPORT MUniformParameter
{
public:
	//! Parameter data types.
	typedef enum
	{
		kTypeUnknown,		//!< \nop
		kTypeBool,		//!< \nop
		kTypeInt,		//!< \nop
		kTypeFloat,		//!< \nop
		kType1DTexture,		//!< \nop
		kType2DTexture,		//!< \nop
		kType3DTexture,		//!< \nop
		kTypeCubeTexture,	//!< \nop
		kTypeEnvTexture,	//!< \nop
		kTypeString,		//!< \nop
		kTypeEnum,		    //!< \nop
		// NOTE - when editing this, be sure to update the internal equivalent
	} DataType;


	//! Parameter semantics (i.e. what the parameter represents).
	typedef enum
	{
		kSemanticUnknown,				//!< \nop
		kSemanticObjectDir,				//!< \nop
		kSemanticWorldDir,				//!< \nop
		kSemanticViewDir,				//!< \nop
		kSemanticProjectionDir,				//!< \nop
		kSemanticObjectPos,				//!< \nop
		kSemanticWorldPos,				//!< \nop
		kSemanticViewPos,				//!< \nop
		kSemanticProjectionPos,				//!< \nop
		kSemanticColor,					//!< \nop
		kSemanticNormal,				//!< \nop
		kSemanticBump,					//!< \nop
		kSemanticEnvironment,				//!< \nop
		kSemanticWorldMatrix,				//!< \nop
		kSemanticWorldInverseMatrix,			//!< \nop
		kSemanticWorldInverseTransposeMatrix,		//!< \nop
		kSemanticViewMatrix,				//!< \nop
		kSemanticViewInverseMatrix,			//!< \nop
		kSemanticViewInverseTransposeMatrix,		//!< \nop
		kSemanticProjectionMatrix,			//!< \nop
		kSemanticProjectionInverseMatrix,		//!< \nop
		kSemanticProjectionInverseTransposeMatrix,	//!< \nop
		kSemanticWorldViewMatrix,			//!< \nop
		kSemanticWorldViewInverseMatrix,		//!< \nop
		kSemanticWorldViewInverseTransposeMatrix,	//!< \nop
		kSemanticWorldViewProjectionMatrix,		//!< \nop
		kSemanticWorldViewProjectionInverseMatrix,	//!< \nop
		kSemanticWorldViewProjectionInverseTransposeMatrix, //!< \nop
		kSemanticColorTexture,				//!< \nop
		kSemanticNormalTexture,				//!< \nop
		kSemanticBumpTexture,				//!< \nop
		kSemanticNormalizationTexture,			//!< \nop
		kSemanticTime,					//!< \nop

		kSemanticWorldTransposeMatrix,				//!< \nop
		kSemanticViewTransposeMatrix,				//!< \nop
		kSemanticProjectionTransposeMatrix,				//!< \nop
		kSemanticWorldViewTransposeMatrix,				//!< \nop
		kSemanticWorldViewProjectionTransposeMatrix,		//!< \nop

		kSemanticViewProjectionMatrix,		//!< \nop
		kSemanticViewProjectionInverseMatrix,	//!< \nop
		kSemanticViewProjectionTransposeMatrix,	//!< \nop
		kSemanticViewProjectionInverseTransposeMatrix, //!< \nop

		kSemanticLocalViewer,					//!< \nop
		kSemanticViewportPixelSize,				//!< \nop
		kSemanticBackgroundColor,				//!< \nop
		kSemanticFrameNumber					//!< \nop

		// NOTE - when editing this, be sure to update the internal equivalent
	} DataSemantic;

	MUniformParameter();

	MUniformParameter( const MString& name,
						DataType type,
						DataSemantic semantic,
						unsigned int numRows = 1,
						unsigned int numColumns = 1,
						void* userData = NULL);

	~MUniformParameter();

	const MUniformParameter& operator=( const MUniformParameter& other);

	MString name() const;

	MUniformParameter::DataType type() const;

	MUniformParameter::DataSemantic semantic() const;

	unsigned int numRows() const;

	unsigned int numColumns() const;

	unsigned int numElements() const;

	void* userData() const;

	void setRangeMin(double rangeMin) const;
	void setRangeMax(double rangeMax) const;
	void setSoftRangeMin(double softRangeMin) const;
	void setSoftRangeMax(double softRangeMax) const;

	void setUIHidden(bool hiddenState) const;
	bool UIHidden() const;

	void setEnumFieldNames(const MString& fieldNames);

	void setKeyable(bool keyable) const;
	bool keyable() const;

	bool isATexture() const;

	bool hasChanged( const MGeometryList& iterator) const;
	bool hasChanged( const MHWRender::MDrawContext& context) const;

	const float* getAsFloatArray( const MGeometryList& iterator) const;
	const float* getAsFloatArray( const MHWRender::MDrawContext& context) const;

	void setAsFloatArray( const float* value, unsigned int maxElements) const;

	float getAsFloat( const MGeometryList& iterator) const;
	float getAsFloat( const MHWRender::MDrawContext& context ) const;

	void setAsFloat( float value) const;

	MString getAsString( const MGeometryList& iterator) const;
	MString getAsString( const MHWRender::MDrawContext& context ) const;

	void setAsString( const MString& value) const;

	bool getAsBool( const MGeometryList& iterator) const;
	bool getAsBool( const MHWRender::MDrawContext& context  ) const;

	void setAsBool( bool value) const;

	int getAsInt( const MGeometryList& iterator) const;
	int getAsInt( const MHWRender::MDrawContext& context ) const;

	void setAsInt( int value) const;

	void setUINiceName(const MString& name);

	MPlug getPlug() const;
	MPlug getSource() const;

	void setDirty();

protected:

	MUniformParameter( void* ptr);

	void*			_ptr;

private:
// No private members

	friend class MUniformParameterList;
};

#endif
#endif // _MUniformParameter
