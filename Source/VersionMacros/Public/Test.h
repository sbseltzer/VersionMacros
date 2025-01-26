/**
* Version Macros for supporting projects that span multiple versions of Unreal.
 * https://github.com/sbseltzer/VersionMacros
 * 
 * Copyright Sam Seltzer-Johnston 2025. All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// NOTE: This file is only for testing purposes. Do not copy it to your project!

#pragma once

#include "VersionMacros.h"
#include "Prebuild.h"

// Compile-time sanity tests
#ifndef UE_VERSION_COMPILE_TIME_TESTS
#define UE_VERSION_COMPILE_TIME_TESTS 1
#endif

#if UE_VERSION_COMPILE_TIME_TESTS
#define compile_time_assert(b) static_assert((b), "Compile time assertion failed: "#b)

//==============================================================================================
// Test Prebuild Scripts (fake macro replacements)
// If any of the following asserts fail, it means something is wrong with the prebuild scripts
//==============================================================================================

#if 1 // UE_4_17_OR_LATER
compile_time_assert(UE_4_17_OR_LATER);
#else
compile_time_assert(!UE_4_17_OR_LATER);
#endif
#if 0 // !UE_4_17_OR_LATER
compile_time_assert(!UE_4_17_OR_LATER);
#else
compile_time_assert(UE_4_17_OR_LATER);
#endif

#if 1 // UE_4_18_OR_LATER
compile_time_assert(UE_4_18_OR_LATER);
#else
compile_time_assert(!UE_4_18_OR_LATER);
#endif
#if 0 // !UE_4_18_OR_LATER
compile_time_assert(!UE_4_18_OR_LATER);
#else
compile_time_assert(UE_4_18_OR_LATER);
#endif

#if 1 // UE_4_19_OR_LATER
compile_time_assert(UE_4_19_OR_LATER);
#else
compile_time_assert(!UE_4_19_OR_LATER);
#endif
#if 0 // !UE_4_19_OR_LATER
compile_time_assert(!UE_4_19_OR_LATER);
#else
compile_time_assert(UE_4_19_OR_LATER);
#endif

#if 1 // UE_4_20_OR_LATER
compile_time_assert(UE_4_20_OR_LATER);
#else
compile_time_assert(!UE_4_20_OR_LATER);
#endif
#if 0 // !UE_4_20_OR_LATER
compile_time_assert(!UE_4_20_OR_LATER);
#else
compile_time_assert(UE_4_20_OR_LATER);
#endif

#if 1 // UE_4_21_OR_LATER
compile_time_assert(UE_4_21_OR_LATER);
#else
compile_time_assert(!UE_4_21_OR_LATER);
#endif
#if 0 // !UE_4_21_OR_LATER
compile_time_assert(!UE_4_21_OR_LATER);
#else
compile_time_assert(UE_4_21_OR_LATER);
#endif

#if 1 // UE_4_22_OR_LATER
compile_time_assert(UE_4_22_OR_LATER);
#else
compile_time_assert(!UE_4_22_OR_LATER);
#endif
#if 0 // !UE_4_22_OR_LATER
compile_time_assert(!UE_4_22_OR_LATER);
#else
compile_time_assert(UE_4_22_OR_LATER);
#endif

#if 1 // UE_4_23_OR_LATER
compile_time_assert(UE_4_23_OR_LATER);
#else
compile_time_assert(!UE_4_23_OR_LATER);
#endif
#if 0 // !UE_4_23_OR_LATER
compile_time_assert(!UE_4_23_OR_LATER);
#else
compile_time_assert(UE_4_23_OR_LATER);
#endif

#if 1 // UE_4_24_OR_LATER
compile_time_assert(UE_4_24_OR_LATER);
#else
compile_time_assert(!UE_4_24_OR_LATER);
#endif
#if 0 // !UE_4_24_OR_LATER
compile_time_assert(!UE_4_24_OR_LATER);
#else
compile_time_assert(UE_4_24_OR_LATER);
#endif

#if 1 // UE_4_25_OR_LATER
compile_time_assert(UE_4_25_OR_LATER);
#else
compile_time_assert(!UE_4_25_OR_LATER);
#endif
#if 0 // !UE_4_25_OR_LATER
compile_time_assert(!UE_4_25_OR_LATER);
#else
compile_time_assert(UE_4_25_OR_LATER);
#endif

#if 1 // UE_4_26_OR_LATER
compile_time_assert(UE_4_26_OR_LATER);
#else
compile_time_assert(!UE_4_26_OR_LATER);
#endif
#if 0 // !UE_4_26_OR_LATER
compile_time_assert(!UE_4_26_OR_LATER);
#else
compile_time_assert(UE_4_26_OR_LATER);
#endif

#if 1 // UE_4_27_OR_LATER
compile_time_assert(UE_4_27_OR_LATER);
#else
compile_time_assert(!UE_4_27_OR_LATER);
#endif
#if 0 // !UE_4_27_OR_LATER
compile_time_assert(!UE_4_27_OR_LATER);
#else
compile_time_assert(UE_4_27_OR_LATER);
#endif

#if 1 // UE_5_0_OR_LATER
compile_time_assert(UE_5_0_OR_LATER);
#else
compile_time_assert(!UE_5_0_OR_LATER);
#endif
#if 0 // !UE_5_0_OR_LATER
compile_time_assert(!UE_5_0_OR_LATER);
#else
compile_time_assert(UE_5_0_OR_LATER);
#endif

#if 1 // UE_5_1_OR_LATER
compile_time_assert(UE_5_1_OR_LATER);
#else
compile_time_assert(!UE_5_1_OR_LATER);
#endif
#if 0 // !UE_5_1_OR_LATER
compile_time_assert(!UE_5_1_OR_LATER);
#else
compile_time_assert(UE_5_1_OR_LATER);
#endif

#if 1 // UE_5_2_OR_LATER
compile_time_assert(UE_5_2_OR_LATER);
#else
compile_time_assert(!UE_5_2_OR_LATER);
#endif
#if 0 // !UE_5_2_OR_LATER
compile_time_assert(!UE_5_2_OR_LATER);
#else
compile_time_assert(UE_5_2_OR_LATER);
#endif

#if 1 // UE_5_3_OR_LATER
compile_time_assert(UE_5_3_OR_LATER);
#else
compile_time_assert(!UE_5_3_OR_LATER);
#endif
#if 0 // !UE_5_3_OR_LATER
compile_time_assert(!UE_5_3_OR_LATER);
#else
compile_time_assert(UE_5_3_OR_LATER);
#endif

#if 1 // UE_5_4_OR_LATER
compile_time_assert(UE_5_4_OR_LATER);
#else
compile_time_assert(!UE_5_4_OR_LATER);
#endif
#if 0 // !UE_5_4_OR_LATER
compile_time_assert(!UE_5_4_OR_LATER);
#else
compile_time_assert(UE_5_4_OR_LATER);
#endif

#if 0 // UE_5_5_OR_LATER
compile_time_assert(UE_5_5_OR_LATER);
#else
compile_time_assert(!UE_5_5_OR_LATER);
#endif
#if 1 // !UE_5_5_OR_LATER
compile_time_assert(!UE_5_5_OR_LATER);
#else
compile_time_assert(UE_5_5_OR_LATER);
#endif

#define TEST_MACRO UE_5_0_OR_EARLIER
#if 0 // TEST_MACRO
compile_time_assert(TEST_MACRO);
#else
compile_time_assert(!TEST_MACRO);
#endif
#if 1 // !TEST_MACRO
compile_time_assert(!TEST_MACRO);
#else
compile_time_assert(TEST_MACRO);
#endif

#if 0 // TEST_MACRO_A
compile_time_assert(TEST_MACRO_A);
#else
compile_time_assert(!TEST_MACRO_A);
#endif
#if 1 // !TEST_MACRO_A
compile_time_assert(!TEST_MACRO_A);
#else
compile_time_assert(TEST_MACRO_A);
#endif

#if 0 // UE_VERSION_BELOW(5,0)
compile_time_assert(UE_VERSION_BELOW(5,0));
#else
compile_time_assert(!UE_VERSION_BELOW(5,0));
#endif
#if 1 // !UE_VERSION_BELOW(5,0)
compile_time_assert(!UE_VERSION_BELOW(5,0));
#else
compile_time_assert(UE_VERSION_BELOW(5,0));
#endif

#if 1 // UE_VERSION_ABOVE(5,0)
compile_time_assert(UE_VERSION_ABOVE(5,0));
#else
compile_time_assert(!UE_VERSION_ABOVE(5,0));
#endif
#if 0 // !UE_VERSION_ABOVE(5,0)
compile_time_assert(!UE_VERSION_ABOVE(5,0));
#else
compile_time_assert(UE_VERSION_ABOVE(5,0));
#endif

#if 1 // UE_VERSION_MINIMUM(5,0)
compile_time_assert(UE_VERSION_MINIMUM(5,0));
#else
compile_time_assert(!UE_VERSION_MINIMUM(5,0));
#endif
#if 0 // !UE_VERSION_MINIMUM(5,0)
compile_time_assert(!UE_VERSION_MINIMUM(5,0));
#else
compile_time_assert(UE_VERSION_MINIMUM(5,0));
#endif

#if 0 // UE_VERSION_MAXIMUM(5,0)
compile_time_assert(UE_VERSION_MAXIMUM(5,0));
#else
compile_time_assert(!UE_VERSION_MAXIMUM(5,0));
#endif
#if 1 // !UE_VERSION_MAXIMUM(5,0)
compile_time_assert(!UE_VERSION_MAXIMUM(5,0));
#else
compile_time_assert(UE_VERSION_MAXIMUM(5,0));
#endif

#define TEST_MACRO_CONCISE UE_VERSION_MINIMUM(5,0)
#if 1 // TEST_MACRO_CONCISE
compile_time_assert(TEST_MACRO_CONCISE);
#else
compile_time_assert(!TEST_MACRO_CONCISE);
#endif
#if 0 // !TEST_MACRO_CONCISE
compile_time_assert(!TEST_MACRO_CONCISE);
#else
compile_time_assert(TEST_MACRO_CONCISE);
#endif

#define TEST_MACRO_CONST_STRING 1
#if 1 // TEST_MACRO_CONST_STRING
compile_time_assert(TEST_MACRO_CONST_STRING);
#else
compile_time_assert(!TEST_MACRO_CONST_STRING);
#endif
#if 0 // !TEST_MACRO_CONST_STRING
compile_time_assert(!TEST_MACRO_CONST_STRING);
#else
compile_time_assert(TEST_MACRO_CONST_STRING);
#endif

#define TEST_MACRO_CONST_INT 1
#if 1 // TEST_MACRO_CONST_INT
compile_time_assert(TEST_MACRO_CONST_INT);
#else
compile_time_assert(!TEST_MACRO_CONST_INT);
#endif
#if 0 // !TEST_MACRO_CONST_INT
compile_time_assert(!TEST_MACRO_CONST_INT);
#else
compile_time_assert(TEST_MACRO_CONST_INT);
#endif

#define TEST_MACRO_CONST_BOOL 1
#if 1 // TEST_MACRO_CONST_BOOL
compile_time_assert(TEST_MACRO_CONST_BOOL);
#else
compile_time_assert(!TEST_MACRO_CONST_BOOL);
#endif
#if 0 // !TEST_MACRO_CONST_BOOL
compile_time_assert(!TEST_MACRO_CONST_BOOL);
#else
compile_time_assert(TEST_MACRO_CONST_BOOL);
#endif

#  if   1 // UE_VERSION_WITHIN( 4,26, 5, 4)
compile_time_assert(UE_VERSION_WITHIN( 4,26, 5, 4));
#  else
compile_time_assert(!UE_VERSION_WITHIN( 4,26, 5, 4));
#  endif

#  if   1 // TEST_MACRO_C
compile_time_assert(TEST_MACRO_C);
#  else
compile_time_assert(!TEST_MACRO_C);
#  endif

#  if   1 // TEST_MACRO_C
compile_time_assert(TEST_MACRO_C);
#  elif   1 // !TEST_MACRO_B
compile_time_assert(!TEST_MACRO_B);
#  else
compile_time_assert(!TEST_MACRO_C);
compile_time_assert(TEST_MACRO_B);
#  endif

#if 1 // TEST_MACRO_D
compile_time_assert(TEST_MACRO_D);
#else
compile_time_assert(!TEST_MACRO_D);
#endif

#if 0 // TEST_MACRO_E
compile_time_assert(TEST_MACRO_E);
#else
compile_time_assert(!TEST_MACRO_E);
#endif

#endif

//================================================================================================================
// NOTE: I haven't managed to get any UCLASS or USTRUCT declarations from this file to compile in 4.13 and lower.
//================================================================================================================
#if 1 // UE_VERSION_ABOVE(4, 13)

#if UE_VERSION_MAXIMUM(4, 15)
#include "Core.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Object.h"
#endif

//===================================================================================================================
// NOTE: If you conditionally disable all UCLASS/USTRUCT declarations in a file, the .generated.h file won't exist!
// The simplest solution is to declare an empty placeholder USTRUCT to ensure the .generated.h file always exists.
// If your file only has one UCLASS/USTRUCT, you can simply move the .generated.h include inside the same #if block.
//===================================================================================================================

#include "Test.generated.h"

USTRUCT()
struct FVersionMacrosPlaceholderStruct
{
	GENERATED_BODY()
};

// Test wrapping a USTRUCT
#if 1 // !TEST_MACRO_B
USTRUCT()
struct FVersionMacrosTestStruct
{
	GENERATED_BODY()
	
	// Test wrapping a UPROPERTY
#if 0 // TEST_MACRO_B
	UPROPERTY()
	bool TestProperty;
#endif
};
#endif

// Test wrapping a UCLASS
#if 1 // !TEST_MACRO_A
UCLASS()
class UVersionMacrosTestObject : public UObject
{
public:
	GENERATED_BODY()

	// Test wrapping a UFUNCTION
#if 0 // TEST_MACRO
	UFUNCTION()
	void TestFunction() {}
#endif

	// Test wrapping a UPROPERTY
#if 0 // UE_VERSION_EQUAL(5,5)
	UPROPERTY()
	bool TestProperty;
#endif
};
#endif

#endif