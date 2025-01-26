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

// NOTE: This file is only for module compilation & testing purposes. Do not copy it to your project!

#include "VersionMacros.h"
#include "Test.h"
#include "Modules/ModuleManager.h"

#if UE_VERSION_COMPILE_TIME_TESTS

//==============================================================================================
// Test VersionMacros.h
// If any of the following asserts fail, it means something is wrong with VersionMacros.h
//==============================================================================================

// EQ Comparisons
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,5,1, ==, 5,5,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, ==, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, ==, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, ==, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, ==, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, ==, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, ==, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, ==, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, ==, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, ==, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, ==, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, ==, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, ==, 4,21,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,20,3, ==, 4,20,3));

// GTE Comparisons (Equal)
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,5,1, >=, 5,5,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, >=, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, >=, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, >=, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, >=, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, >=, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, >=, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, >=, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, >=, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, >=, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, >=, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, >=, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, >=, 4,21,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,20,3, >=, 4,20,3));

// LTE Comparisons (Equal)
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,5,1, <=, 5,5,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, <=, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, <=, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, <=, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, <=, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, <=, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, <=, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, <=, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, <=, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, <=, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, <=, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, <=, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, <=, 4,21,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,20,3, <=, 4,20,3));

// GT Comparisons
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,5,1, >, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, >, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, >, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, >, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, >, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, >, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, >, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, >, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, >, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, >, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, >, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, >, 4,21,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, >, 4,20,3));

// GTE Comparisons (Greater)
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,5,1, >=, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, >=, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, >=, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, >=, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, >=, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, >=, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, >=, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, >=, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, >=, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, >=, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, >=, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, >=, 4,21,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, >=, 4,20,3));

// LT Comparisons
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, <, 5,5,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, <, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, <, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, <, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, <, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, <, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, <, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, <, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, <, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, <, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, <, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, <, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,20,3, <, 4,21,2));

// LTE Comparisons (Lesser)
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,4,4, <=, 5,5,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,3,2, <=, 5,4,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,2,1, <=, 5,3,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,1, <=, 5,2,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,3, <=, 5,1,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,2, <=, 5,0,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,26,2, <=, 4,27,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,25,4, <=, 4,26,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,24,3, <=, 4,25,4));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,23,1, <=, 4,24,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,22,3, <=, 4,23,1));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,21,2, <=, 4,22,3));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,20,3, <=, 4,21,2));

//==============================================================================================
// Test Prebuild Scripts (TObjectPtr replacements)
// If the following fails to compile, it means something is wrong with the prebuild scripts
//==============================================================================================

// Verify it works on static variable declarations
static TObjectPtr<UObject> TestVariable1;
static TObjectPtr<UObject> TestVariable2;
static const TObjectPtr<UObject> TestVariable3;
// Verify it works inside function declarations
static TObjectPtr<UObject> FuncTest(TObjectPtr<UObject>& Arg1, const TObjectPtr<UObject> Arg2) { return Arg1; }
static const TObjectPtr<UObject> FuncTest2(TObjectPtr<UObject> Arg1, const TObjectPtr<UObject>& Arg2) { return Arg2; }
// Verify it works inside delegate macros
#include "Delegates/DelegateCombinations.h"
DECLARE_MULTICAST_DELEGATE_TwoParams(FTestObjectPtrDelegate1, const TObjectPtr<UObject>, TObjectPtr<UObject>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FTestObjectPtrDelegate2, const TObjectPtr<UObject>, TObjectPtr<UObject>);
// NOTE: DECLARE_DYNAMIC_MULTICAST_DELEGATE is not relevant to test as it's not allowed to take TObjectPtr as a parameter type

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

#define TEST_MACRO_CONCISE_WITH_FILES UE_VERSION_MINIMUM(5,0)
#if 1 // TEST_MACRO_CONCISE_WITH_FILES
compile_time_assert(TEST_MACRO_CONCISE_WITH_FILES);
#else
compile_time_assert(!TEST_MACRO_CONCISE_WITH_FILES);
#endif
#if 0 // !TEST_MACRO_CONCISE_WITH_FILES
compile_time_assert(!TEST_MACRO_CONCISE_WITH_FILES);
#else
compile_time_assert(TEST_MACRO_CONCISE_WITH_FILES);
#endif

#endif

// Bare minimum code plugin
class FVersionMacrosModule : public IModuleInterface {};
IMPLEMENT_MODULE(FVersionMacrosModule, VersionMacros);
