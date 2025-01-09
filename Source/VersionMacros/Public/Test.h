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

// Compile-time sanity tests
#ifndef UE_VERSION_COMPILE_TIME_TESTS
#define UE_VERSION_COMPILE_TIME_TESTS 1
#endif

#if UE_VERSION_MAXIMUM(4, 15)
#include "Core.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Object.h"
#endif

#if UE_VERSION_COMPILE_TIME_TESTS
#define compile_time_assert(b) static_assert((b), "Compile time assertion failed: "#b)

//==============================================================================================
// Test Prebuild Scripts (fake macro replacements)
// If any of the following asserts fail, it means something is wrong with the prebuild scripts
//==============================================================================================

#define UE_5_0_OR_LATER UE_VERSION_MINIMUM(5, 0)
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

#define UE_5_4_OR_LATER UE_VERSION_MINIMUM(5, 4)
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
#define TEST_MACRO UE_VERSION_MAXIMUM(5, 0)
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
#endif

#if 1 // !TEST_MACRO
#include "Test.generated.h"

UCLASS()
class UVersionMacrosTestObject : public UObject
{
public:
 GENERATED_BODY()

#if 0 // !UE_VERSION_EQUAL(5,5)
 UPROPERTY()
 bool TestProperty;
#endif
};
#endif