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

#include <common.h>
#pragma hdrstop

#include <NeoMathEngine/NeoMathEngineDefs.h>

#ifdef NEOML_USE_OWN_BLAS

#include <CPUInfo.h>
#include <MatrixMultiplyingInterleavedCommon/MatrixMultiplying.h>
#include <MatrixMultiplyingInterleavedCommon/CpuMemoryHelper.h>
#include <MathEngineCommon.h>

// There is no way to find out the cache size for ARM
// Use a set value
// These constants helped speed up performance of some of the testing devices
static constexpr CCPUInfo CpuInfo(0x8000, 0x20000);

namespace NeoML {

void CCpuMathEngine::multiplyMatrixByMatrix(const CConstFloatHandle& firstHandle, int firstHeight,
	int firstWidth, int firstRowSize, const CConstFloatHandle& secondHandle, int secondWidth, int secondRowSize,
	const CFloatHandle& resultHandle, int resultRowSize, int resultBufferSize)
{
	const float* first = GetRaw(firstHandle);
	const float* second = GetRaw(secondHandle);
	float* result = GetRaw(resultHandle);

	ASSERT_EXPR(firstWidth <= firstRowSize);
	ASSERT_EXPR(secondWidth <= secondRowSize);
	ASSERT_EXPR(secondWidth <= resultRowSize);
	ASSERT_EXPR((firstHeight - 1) * resultRowSize + secondWidth <= resultBufferSize);

	nullify(result, firstHeight, secondWidth, resultRowSize);
	MultiplyMatrix<false, false, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstHeight, secondWidth, firstWidth);
}

void CCpuMathEngine::multiplyMatrixByMatrixAndAdd(const CConstFloatHandle& firstHandle, int firstHeight,
	int firstWidth, int firstRowSize, const CConstFloatHandle& secondHandle, int secondWidth, int secondRowSize,
	const CFloatHandle& resultHandle, int resultRowSize, int resultBufferSize)
{
	const float* first = GetRaw(firstHandle);
	const float* second = GetRaw(secondHandle);
	float* result = GetRaw(resultHandle);

	ASSERT_EXPR(firstWidth <= firstRowSize);
	ASSERT_EXPR(secondWidth <= resultRowSize);
	ASSERT_EXPR((firstHeight - 1) * resultRowSize + secondWidth <= resultBufferSize);

	MultiplyMatrix<false, false, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstHeight, secondWidth, firstWidth);
}

void CCpuMathEngine::multiplyMatrixByTransposedMatrix(const CConstFloatHandle& firstHandle, int firstHeight,
	int firstWidth, int firstRowSize, const CConstFloatHandle& secondHandle, int secondHeight, int secondRowSize,
	const CFloatHandle& resultHandle, int resultRowSize, int resultBufferSize)
{
	const float* first = GetRaw(firstHandle);
	const float* second = GetRaw(secondHandle);
	float* result = GetRaw(resultHandle);

	ASSERT_EXPR(firstWidth <= firstRowSize);
	ASSERT_EXPR(firstWidth <= secondRowSize);
	ASSERT_EXPR(secondHeight <= resultRowSize);
	ASSERT_EXPR((firstHeight - 1) * resultRowSize + secondHeight <= resultBufferSize);

	nullify(result, firstHeight, secondHeight, resultRowSize);
	MultiplyMatrix<false, true, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstHeight, secondHeight, firstWidth);
}

void CCpuMathEngine::multiplyMatrixByTransposedMatrixAndAdd( const float* first, int firstHeight, int firstWidth, int firstRowSize,
	const float* second, int secondHeight, int secondRowSize, float* result, int resultRowSize )
{
	MultiplyMatrix<false, true, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstHeight, secondHeight, firstWidth);
}

void CCpuMathEngine::multiplyTransposedMatrixByMatrix(const CConstFloatHandle& firstHandle, int firstHeight,
	int firstWidth, const CConstFloatHandle& secondHandle, int secondWidth,
	const CFloatHandle& resultHandle, int resultBufferSize)
{
	const float* first = GetRaw(firstHandle);
	const float* second = GetRaw(secondHandle);
	float* result = GetRaw(resultHandle);

	ASSERT_EXPR((firstWidth - 1) * secondWidth + secondWidth <= resultBufferSize);

	auto firstRowSize = firstWidth;
	auto secondRowSize = secondWidth;
	auto resultRowSize = secondWidth;
	nullify(result, firstWidth, secondWidth);
	MultiplyMatrix<true, false, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstWidth, secondWidth, firstHeight);
}

void CCpuMathEngine::multiplyTransposedMatrixByMatrixAndAdd(const CConstFloatHandle& firstHandle, int firstHeight,
	int firstWidth, int firstRowSize, const CConstFloatHandle& secondHandle, int secondWidth, int secondRowSize,
	const CFloatHandle& resultHandle, int resultRowSize, int resultBufferSize)
{
	const float* first = GetRaw(firstHandle);
	const float* second = GetRaw(secondHandle);
	float* result = GetRaw(resultHandle);

	ASSERT_EXPR(firstWidth <= firstRowSize);
	ASSERT_EXPR(secondWidth <= secondRowSize);
	ASSERT_EXPR(secondWidth <= resultRowSize);
	ASSERT_EXPR((firstWidth - 1) * resultRowSize + secondWidth <= resultBufferSize);

	MultiplyMatrix<true, false, CTmpMemoryHandler>(this, CpuInfo, first, firstRowSize, second, secondRowSize,
		result, resultRowSize, firstWidth, secondWidth, firstHeight);
}

} // namespace NeoML

#endif // NEOML_USE_OWN_BLAS
