#ifndef _MDrawContext
#define _MDrawContext
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
//
// CLASS:    MDrawContext
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MDrawContext)
//
//  MDrawContext allows access to hardware draw context information
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MFrameContext.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVector.h>
#include <maya/MColor.h>
#include <maya/MHWGeometry.h>
#include <maya/MFloatArray.h>
#include <maya/MUintArray.h>
#include <maya/MTextureManager.h>
#include <maya/MRenderTargetManager.h>

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS

class MStateManager;
class MSamplerStateDesc;
class MLightParameterInformationSet;
struct MTextureAssignment;

// ****************************************************************************
// CLASS DECLARATION (MLightParameterInformation)
//! \ingroup OpenMayaRender
//! \brief A class for providing lighting information that may be used with Viewport 2.0
/*!
This class allows for access to various per-light information
accessible via the MDrawContext class in Viewport 2.0.
*/

class OPENMAYARENDER_EXPORT MLightParameterInformation
{
public:
	//! Specifies light parameter data types
	enum ParameterType {
		//! Invalid element type (default value)
		kInvalid,
		//! Boolean
		kBoolean,
		//! Signed 32-bit integer
		kInteger,
		//! IEEE single precision floating point
		kFloat,
		//! IEEE single precision floating point (x2)
		kFloat2,
		//! IEEE single precision floating point (x3)
		kFloat3,
		//! IEEE single precision floating point (x4)
		kFloat4,
		//! IEEE single precision floating point row-major matrix (4x4)
		kFloat4x4Row,
		//! IEEE single precision floating point column-major matrix (4x4)
		kFloat4x4Col,
		//! 2D texture
		kTexture2,
		//! Sampler
		kSampler
	};

	/*!	Specifies semantic meanings for predefined names for common or
		"stock" light parameters.

		The semantic is provided to aid in the determination of the appropriate
		mapping of parameter values to programmable shader or fixed
		function lighting inputs.

		Note that not all parameters exist on all lights so there may
		be no match for a given stock name. For example
		a directional light will have no position parameter.
	*/
	enum StockParameterSemantic {
		//! No semantic
		kNoSemantic,

		// Basic lighting parameters
		//! Light is enabled
		kLightEnabled ,
		//! World space position
		kWorldPosition,
		//! World space direction
		kWorldDirection,
		//! Intensity
		kIntensity,
		//! Color
		kColor,
		//! Emits diffuse lighting
		kEmitsDiffuse,
		//! Emits specular lighting
		kEmitsSpecular,
		//! Decay rate (0=No Decay, 1=Linear, 2=Quadratic, 3=Cubic)
		kDecayRate,
		//! Dropoff
		kDropoff,
		//! Cosine of the cone angle
		kCosConeAngle,

		// Basic shadowing parameters
		//! Incoming irradiance. Also marks the beginning of enums for shadowing
		kStartShadowParameters,
		kIrradianceIn = kStartShadowParameters,
		//! Shadow map
		kShadowMap,
		//! Shadow map sampler
		kShadowSamp,
		//! Shadow map bias
		kShadowBias,
		//! Shadow map size
		kShadowMapSize,
		//! Shadow map view projection matrix
		kShadowViewProj,
		//! Shadow color
		kShadowColor,
		//! Global toggle for whether shadows are enabled or not
		kGlobalShadowOn,
		//! Local toggle per light for whether shadows are enabled or not
		kShadowOn,
		//! Indicates if the contents of the shadow map are out-of-date or uninitialized
		kShadowDirty
	};

	// String based access
	void parameterList(MStringArray& list) const;
	ParameterType parameterType(const MString& parameterName) const;
	StockParameterSemantic parameterSemantic(const MString& parameterName) const;
	void parameterNames(StockParameterSemantic semantic, MStringArray& list) const;
	unsigned int arrayParameterCount(const MString& parameterName) const;

	MStatus getParameter(const MString& parameterName, MIntArray & value );
	MStatus getParameter(const MString& parameterName, MFloatArray & value);
	MStatus getParameter(const MString& parameterName, MMatrix & value );
	MStatus getParameter(const MString& parameterName, MSamplerStateDesc & samplerDesc );
	void * getParameterTextureHandle(const MString& parameterName );
	MStatus getParameter(const MString& parameterName, MTextureAssignment &value );

	MStatus getParameter(StockParameterSemantic semantic, MIntArray & value );
	MStatus getParameter(StockParameterSemantic semantic, MFloatArray & value);
	MStatus getParameter(StockParameterSemantic semantic, MMatrix & value );
	MStatus getParameter(StockParameterSemantic semantic, MSamplerStateDesc & samplerDesc );
	void * getParameterTextureHandle(StockParameterSemantic semantic);
	MStatus getParameter(StockParameterSemantic semantic, MTextureAssignment &value );

	MString lightType() const;
	MDagPath lightPath(MStatus* ReturnStatus=NULL) const;

	static const char* className();

protected:
	~MLightParameterInformation();
private:
	MLightParameterInformation(void* data, void* data2, void *data3);
	void set( void* data, void *data2, void *data3 );

	// Internal parameter helpers
	void buildParamNameMap();
	void* findParameterFromName( const MString &string, MStatus &status ) const;
	void findParametersBySemantic(StockParameterSemantic semantic, void* parameters) const;
	MStatus getParameter(void* vparam, MSamplerStateDesc & samplerDesc );

	void* fData;
	void* fData2;
	void* fData3;
	void* fParamNameMapper;

	friend class MDrawContext;
	friend class MLightParameterInformationSet;
};

// ****************************************************************************
// CLASS DECLARATION (MPassContext)
//
//! \ingroup OpenMayaRender
//! \brief Class to allow access to pass context information.
/*!
MPassContext provides access to current pass context information.
The term "pass context" is used to denote a logical breakdown of the render loop
within a single frame to render.

A "pass" denotes the highest level elements of the breakdown. Each pass
has a unique identifier and a list of semantics.

The identifier can either be an internally specified identifier or
the name of a custom scene or user render operation (MSceneRender, MUserRenderOperation
respectively).

The semantics provide a description of what is being rendered. The list of semantics
is ordered, from first to last element, such that the first element provides the broadest
description and additional elements refine (provides additional details) about the previous
element.

The following is a list of semantics which may be returned as the first element of the
semantic list. Possible refining semantics are also described.
<ul>
<li> A "color pass" semantic implies rendering for color output. Rendering for a color pass
can be further broken down into the follow logical categorizations:
	<ul>
	<li> Color drawing of opaque surfaces. For this, an additional semantic can describe if
	a material override is being used.
	<li> Color drawing of transparent surfaces. For this, a breakdown of whether the drawing
	is occurring with back or front-face culling may be specified.
	<li> Color drawing of the UI for surfaces.
	</ul>
<li> A "shadow pass" semantic implies rendering to create a shadow map.
<li> A "depth" pass semantic implies rendering for depth output only and not color.
<li> A "depth and normal" semantic implies rendering of depth and object normals as output.
<li> A "user pass" semantic implies that a user operation (MUserRenderOperation) is the
current operation being executed for the active render override (MRenderOverride)
</ul>

As an example, the semantic list returned may contain these elements:

[ color pass, opaque surface drawing, material override ]

This indicates that the active pass is drawing to a color output. Also that
the render items being draw are opaque, and that those render items are being
drawing with a material override.

As a second example, the semantic list returned may contain these elements:

[ color pass, transparent surface drawing, back-face culling ]

This means that the active pass is drawing to a color output. Also that the render items
being drawn are transparent, and that those render items are being drawing with back-face
culling enabled.
*/
class OPENMAYARENDER_EXPORT MPassContext
{
public:
	//! Semantic for a color pass
	static const MString kColorPassSemantic;
	//! Semantic for a shadow pass
	static const MString kShadowPassSemantic;
	//! Semantic for a depth pass
	static const MString kDepthPassSemantic;
	//! Semantic for a normal and depth pass
	static const MString kNormalDepthPassSemantic;
	//! Semantic to indicate opaque surfaces are being drawn
	static const MString kOpaqueGeometrySemantic;
	//! Semantic to indicate pre-scene UI is being drawn. E.g. The grid.
	static const MString kPreUIGeometrySemantic;
	//! Semantic to indicate post-scene UI for surfaces is being drawn. e.g. the wireframe or components
	static const MString kPostUIGeometrySemantic;
	//! Semantic to indicate UI for surfaces is being drawn. e.g. the wireframe or components
	static const MString kUIGeometrySemantic;
	//! Semantic for opaque UI
	static const MString kOpaqueUISemantic;
	//! Semantic for transparent UI
	static const MString kTransparentUISemantic;
	//! Semantic for UI in xray mode
	static const MString kXrayUISemantic;
	//! Semantic to indicate transparent surfaces are being drawn
	static const MString kTransparentGeometrySemantic;
	//! Semantic to indicate that back-face culling is being applied
	static const MString kCullBackSemantic;
	//! Semantic to indicate that front-face culling is being applied
	static const MString kCullFrontSemantic;
	//! Semantic to indicate that a material override is being used
	static const MString kMaterialOverrideSemantic;
	//! Semantic to indicate a user defined pass (MUserRenderOperation)
	static const MString kUserPassSemantic;

	const MString & passIdentifier() const;
	const MStringArray & passSemantics() const;
	bool hasShaderOverride() const;
private:
	MPassContext();
	~MPassContext();

	void setPassIdentifier( const MString &val );
	void clearPassSemantics();
	void addPassSemantic( const MString &val );
	void setPassSemantics( const MStringArray & val );
	void setOverrideEffectInstance( void *val );

	MString mPassIdentifier;
	MStringArray mPassSemantics;
	void *mOverrideEffectInstance;

	friend class MDrawContext;
};

// ****************************************************************************
// CLASS DECLARATION (MDrawContext)
//! \ingroup OpenMayaRender
//! \brief Class to allow access to hardware draw context information.
/*!
MDrawContext provides access to current hardware draw context information. It
is read-only and cannot be instantiated by the plugin writer.

The information provided includes various transformation matrices,
camera information, and render target information such as output buffer
size. It is also possible to access and alter GPU state through this class.

There are a few main advantages for using this class:

1. The data is computed and cached as required. This can avoid work for the
plugin writer to extract this information themselves.
2. The plugin writer should not need to directly access the device to extract
this information. Any data extraction from the device has a performance
penalty which can be avoided.
3. The information is device aware, meaning that it will return the correct
results based on the current active device. For example it will return the
appropriate values for DirectX versus an OpenGL device.

MDrawContext is derived from MFrameContext and provides its own implementation
for all virtual methods. The values returned from these methods may differ
slightly between MFrameContext and MDrawContext as MFrameContext retrieves the
values from Maya and MDrawContext retrieves the values from the GPU device
state.

Some information will vary from one draw to the next draw such as the
object-to-world matrix which will change as different objects are being drawn.
*/
class OPENMAYARENDER_EXPORT MDrawContext : public MFrameContext
{
public:
	virtual MMatrix getMatrix(MFrameContext::MatrixType mtype, MStatus* ReturnStatus=NULL) const;
	virtual MDoubleArray getTuple(MFrameContext::TupleType ttype, MStatus* ReturnStatus=NULL) const;
	virtual MStatus getViewportDimensions(int& originX, int& originY, int& width, int& height) const;

	MUint64 getFrameStamp() const;
	MBoundingBox getSceneBox(MStatus* ReturnStatus=NULL) const;
	MBoundingBox getFrustumBox(MStatus* ReturnStatus) const;
	MStatus getRenderTargetSize( int &width, int &height) const;
	MStatus getDepthRange( float &nearVal, float &farVal ) const;
	bool viewDirectionAlongNegZ( MStatus* ReturnStatus ) const;

	//! Options on which lights to return information for from light query methods
	enum LightFilter {
		kFilteredToLightLimit,		//!< Return light information for lights which pass the VP2.0 filter options.
									//!< The number of lights accessible is clamped to the VP2.0 light limit option.
		kFilteredIgnoreLightLimit	//!< Return light information for lights which pass the VP2.0 filter options.
									//!< The number of lights accessible ignores the VP2.0 light limit option.
	};

	unsigned int numberOfActiveLights(
		LightFilter lightFilter=kFilteredToLightLimit,
		MStatus* ReturnStatus=NULL) const;

	MStatus getLightInformation(
		unsigned int lightNumber,
		MFloatPointArray& positions,
		MFloatVector& direction,
		float& intensity,
		MColor& color,
		bool& hasDirection,
		bool& hasPosition,
		LightFilter lightFilter=kFilteredToLightLimit) const;

	MLightParameterInformation* getLightParameterInformation(
		unsigned int lightNumber,
		LightFilter lightFilter=kFilteredToLightLimit) const;

	MHWRender::MStateManager* getStateManager() const;

	const MHWRender::MPassContext& getPassContext() const;

	static const char* className();

private:
	MDrawContext();
	virtual ~MDrawContext();

	void setData(void* data);
	void* fData;
	MStateManager* fStateManager;
	mutable MPassContext mPassContext;

	friend class MShaderInstance;
	friend class MRenderUtilities;

public:
	// Deprecated methods
	unsigned int numberOfActiveLights(MStatus* ReturnStatus) const;
}; /* draw context */

} // namespace MHWRender

#endif /* __cplusplus */
#endif /* _MDrawContext */