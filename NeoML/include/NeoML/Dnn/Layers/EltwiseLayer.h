/* Copyright © 2017-2020 ABBYY Production LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
--------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <NeoML/NeoMLDefs.h>
#include <NeoML/Dnn/Dnn.h>

namespace NeoML {

// CEltwiseBaseLayer is the base class for layers performing elementwise operations
class NEOML_API CEltwiseBaseLayer : public CBaseLayer {
public:
	virtual void Serialize( CArchive& archive ) override;

protected:
	explicit CEltwiseBaseLayer( IMathEngine& mathEngine ) : CBaseLayer( mathEngine, "CCnnEltwiseBaseLayer", false ) {}
	virtual void Reshape() override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseSumLayer implements a layer that adds its inputs element by element
class NEOML_API CEltwiseSumLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseSumLayer )
public:
	explicit CEltwiseSumLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine ) { SetName( "CCnnEltwiseSumLayer" ); }

	virtual void Serialize( CArchive& archive ) override;

protected:
	virtual void RunOnce() override;
	virtual void BackwardOnce() override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseMulLayer implements a layer that multiplies its inputs element by element
class NEOML_API CEltwiseMulLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseMulLayer )
public:
	explicit CEltwiseMulLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine ) { SetName( "CCnnEltwiseMulLayer" ); }

	virtual void Serialize( CArchive& archive ) override;

protected:
	virtual void RunOnce() override;
	virtual void BackwardOnce() override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseNegMulLayer implements a layer that calculates 
// the product of `1 - firstInput[i]` and input[i] for all other inputs
class NEOML_API CEltwiseNegMulLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseNegMulLayer )
public:
	explicit CEltwiseNegMulLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine ) { SetName( "CCnnEltwiseNegMulLayer" ); }

	virtual void Serialize( CArchive& archive ) override;

protected:
	virtual void Reshape() override;
	virtual void RunOnce() override;
	virtual void BackwardOnce() override;

private:
	CPtr<CDnnBlob> oneVector;
	CPtr<CDnnBlob> negInputBlob;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseMaxLayer implements a layer that finds the maximum among the elements that are at the same position in all input blobs
class NEOML_API CEltwiseMaxLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseMaxLayer )
public:
	explicit CEltwiseMaxLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine ) { SetName("CCnnEltwiseMaxLayer"); }

	virtual void Serialize( CArchive& archive ) override;

protected:
	CArray< CArray<CConstFloatHandle> > vectorsArray;
	CArray< CArray<CFloatHandle> > diffVectorsArray;

	virtual void Reshape() override;
	virtual void RunOnce() override;
	virtual void BackwardOnce() override;

private:
	CPtr<CDnnBlob> maxIndices; // the indices of the inputs that had the largest elements
};

} // namespace NeoML
