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

#pragma once
#include "VersionMacros.h"

// Some example defines
#define UE_4_27_OR_EARLIER UE_VERSION_MAXIMUM(4,27)
#define UE_5_0_OR_LATER UE_VERSION_MINIMUM(5,0)