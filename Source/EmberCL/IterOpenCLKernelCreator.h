#pragma once

#include "EmberCLPch.h"
#include "EmberCLStructs.h"
#include "EmberCLFunctions.h"

/// <summary>
/// IterOpenCLKernelCreator class.
/// </summary>

namespace EmberCLns
{
/// <summary>
/// Class for creating the main iteration code in OpenCL.
/// It uses the Cuburn method of iterating where all conditionals
/// are stripped out and a specific kernel is compiled at run-time.
/// It uses a very sophisticated method for randomization that avoids
/// the problem of warp/wavefront divergence that would occur if every
/// thread selected a random xform to apply.
/// This only works with embers of type float, double is not supported.
/// </summary>
template <typename T>
class EMBERCL_API IterOpenCLKernelCreator
{
public:
	IterOpenCLKernelCreator();
	const string& ZeroizeKernel() const;
	const string& ZeroizeEntryPoint() const;
	const string& SumHistKernel() const;
	const string& SumHistEntryPoint() const;
	const string& IterEntryPoint() const;
	string CreateIterKernelString(Ember<T>& ember, string& parVarDefines, bool lockAccum = false, bool doAccum = true);
	static void ParVarIndexDefines(Ember<T>& ember, pair<string, vector<T>>& params, bool doVals = true, bool doString = true);
	static bool IsBuildRequired(Ember<T>& ember1, Ember<T>& ember2);

private:
	string CreateZeroizeKernelString();
	string CreateSumHistKernelString();
	string CreateProjectionString(Ember<T>& ember);

	string m_IterEntryPoint;
	string m_ZeroizeKernel;
	string m_ZeroizeEntryPoint;
	string m_SumHistKernel;
	string m_SumHistEntryPoint;
};

#ifdef OPEN_CL_TEST_AREA
typedef void (*KernelFuncPointer) (size_t gridWidth, size_t gridHeight, size_t blockWidth, size_t blockHeight,
								   size_t BLOCK_ID_X, size_t BLOCK_ID_Y, size_t THREAD_ID_X, size_t THREAD_ID_Y);

static void OpenCLSim(size_t gridWidth, size_t gridHeight, size_t blockWidth, size_t blockHeight, KernelFuncPointer func)
{
	cout << "OpenCLSim(): " << endl;
	cout << "	Params: " << endl;
	cout << "		gridW: " << gridWidth << endl;
	cout << "		gridH: " << gridHeight << endl;
	cout << "		blockW: " << blockWidth << endl;
	cout << "		blockH: " << blockHeight << endl;

	for (size_t i = 0; i < gridHeight; i += blockHeight)
	{
		for (size_t j = 0; j < gridWidth; j += blockWidth)
		{
			for (size_t k = 0; k < blockHeight; k++)
			{
				for (size_t l = 0; l < blockWidth; l++)
				{
					func(gridWidth, gridHeight, blockWidth, blockHeight, j / blockWidth, i / blockHeight, l, k);
				}
			}
		}
	}
}
#endif
}
