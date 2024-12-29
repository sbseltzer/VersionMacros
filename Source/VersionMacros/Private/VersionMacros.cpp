#include "VersionMacros.h"
#include "Modules/ModuleManager.h"

// NOTE: This file is only for testing purposes. Do not copy it to your project!

// Compile-time sanity tests
#ifndef UE_VERSION_COMPILE_TIME_TESTS
#define UE_VERSION_COMPILE_TIME_TESTS 0
#endif

#if UE_VERSION_COMPILE_TIME_TESTS
#define compile_time_assert(b) static_assert((b), "Compile time assertion failed: "#b)

// Test VersionMacros.h
// If any of the following asserts fail, it means something is wrong with VersionMacros.h
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,0, ==, 5,0,0));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,0,0, <, 5,0,0));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,0,0, >=, 4,0,0));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,1,20, <, 4,2,0));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(5,1,3, >, 5,1,2));
compile_time_assert(UE_VERSION_COMPARE_EXPLICIT(4,27,3, <, 5,0,2));

// Test Prebuild Scripts
// If any of the following asserts fail, it means something is wrong with the prebuild scripts
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

#undef compile_time_assert
#undef UE_VERSION_COMPILE_TIME_TESTS
#endif

// Bare minimum code plugin
class FVersionMacrosModule : public IModuleInterface {};
IMPLEMENT_MODULE(FVersionMacrosModule, VersionMacros);
