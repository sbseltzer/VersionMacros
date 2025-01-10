#pragma once
#include "VersionMacros.h"

/**
 * When referenced by CustomPrebuildHeaders in PrebuildConfig.py, the macro definitions in this file will be parsed and
 * evaluated before any compilation takes place. The results are dumped into the MacroReplacements dictionary.
 * Prebuild.py will then use the contents of that dictionary to modify specially annotated #if...#endif blocks.
 *
 * Example Prebuild.h:
 *	#define UE_5_3_OR_LATER UE_VERSION_MINIMUM(5, 3)
 *
 * Example MyClass.h:
 *	UCLASS()
 *	class UMyClass : public UObject
 *	{
 *		GENERATED_BODY()
 *		#if 1 // UE_5_3_OR_LATER
 *		UPROPERTY()
 *		bool TestProperty;
 *		#endif
 *	};
 *	
 * In the above example, the line "#if <0 or 1> // UE_5_3_OR_LATER" will be modified at prebuild time:
 * - Changes to "#if 1 // UE_5_3_OR_LATER" in UE versions 5.3 and later
 * - Changes to "#if 0 // UE_5_3_OR_LATER" in UE versions 5.2 and earlier
 * 
 * In short, this allows you to conditionally disable blocks of code that use Unreal's "magic" macros.
 */

#define TEST_MACRO_A UE_VERSION_MAXIMUM(5, 0)
#define TEST_MACRO_B UE_VERSION_MINIMUM(4, 27)

#define UE_4_17_OR_LATER UE_VERSION_MINIMUM(4, 17)
#define UE_4_18_OR_LATER UE_VERSION_MINIMUM(4, 18)
#define UE_4_19_OR_LATER UE_VERSION_MINIMUM(4, 19)
#define UE_4_20_OR_LATER UE_VERSION_MINIMUM(4, 20)
#define UE_4_21_OR_LATER UE_VERSION_MINIMUM(4, 21)
#define UE_4_22_OR_LATER UE_VERSION_MINIMUM(4, 22)
#define UE_4_23_OR_LATER UE_VERSION_MINIMUM(4, 23)
#define UE_4_24_OR_LATER UE_VERSION_MINIMUM(4, 24)
#define UE_4_25_OR_LATER UE_VERSION_MINIMUM(4, 25)
#define UE_4_26_OR_LATER UE_VERSION_MINIMUM(4, 26)
#define UE_4_27_OR_LATER UE_VERSION_MINIMUM(4, 27)
#define UE_5_0_OR_LATER UE_VERSION_MINIMUM(5, 0)
#define UE_5_1_OR_LATER UE_VERSION_MINIMUM(5, 1)
#define UE_5_2_OR_LATER UE_VERSION_MINIMUM(5, 2)
#define UE_5_3_OR_LATER UE_VERSION_MINIMUM(5, 3)
#define UE_5_4_OR_LATER UE_VERSION_MINIMUM(5, 4)
#define UE_5_5_OR_LATER UE_VERSION_MINIMUM(5, 5)

#define UE_4_17_OR_EARLIER UE_VERSION_MAXIMUM(4, 17)
#define UE_4_18_OR_EARLIER UE_VERSION_MAXIMUM(4, 18)
#define UE_4_19_OR_EARLIER UE_VERSION_MAXIMUM(4, 19)
#define UE_4_20_OR_EARLIER UE_VERSION_MAXIMUM(4, 20)
#define UE_4_21_OR_EARLIER UE_VERSION_MAXIMUM(4, 21)
#define UE_4_22_OR_EARLIER UE_VERSION_MAXIMUM(4, 22)
#define UE_4_23_OR_EARLIER UE_VERSION_MAXIMUM(4, 23)
#define UE_4_24_OR_EARLIER UE_VERSION_MAXIMUM(4, 24)
#define UE_4_25_OR_EARLIER UE_VERSION_MAXIMUM(4, 25)
#define UE_4_26_OR_EARLIER UE_VERSION_MAXIMUM(4, 26)
#define UE_4_27_OR_EARLIER UE_VERSION_MAXIMUM(4, 27)
#define UE_5_0_OR_EARLIER UE_VERSION_MAXIMUM(5, 0)
#define UE_5_1_OR_EARLIER UE_VERSION_MAXIMUM(5, 1)
#define UE_5_2_OR_EARLIER UE_VERSION_MAXIMUM(5, 2)
#define UE_5_3_OR_EARLIER UE_VERSION_MAXIMUM(5, 3)
#define UE_5_4_OR_EARLIER UE_VERSION_MAXIMUM(5, 4)
#define UE_5_5_OR_EARLIER UE_VERSION_MAXIMUM(5, 5)