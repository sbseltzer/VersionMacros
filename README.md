# What this is

This repository has 2 things that help with multi-version support in Unreal Engine code plugins:
- The [`VersionMacros.h`](Source/VersionMacros/Public/VersionMacros.h) header file
- Automated [Prebuild scripts](Resources/BuildScripts) that work around preprocessor limitations in UnrealHeaderTool

Both are optional and independent of one another. [Installing this to your own plugins](#installation) involves copying the relevant files over and modifying them. The VersionMacros plugin itself is for testing/documentation purposes.

I use these macros and prebuild scripts on my own plugins across various versions of UE4 and UE5. Sometimes I needed to backport modern UE features without breaking forward-compatibility, which is usually not possible with preprocessor macros alone.

## VersionMacros.h

[`VersionMacros.h`](Source/VersionMacros/Public/VersionMacros.h) provides the following macros for engine version comparisons.

```
UE_VERSION_BELOW(major, minor)
UE_VERSION_ABOVE(major, minor)
UE_VERSION_EQUAL(major, minor)
UE_VERSION_MINIMUM(major, minor)
UE_VERSION_MAXIMUM(major, minor)
UE_VERSION_WITHIN(major_min, minor_min, major_max, minor_max)
```
My all-time favorites are `UE_VERSION_MINIMUM` and `UE_VERSION_MAXIMUM`, but I've used all of them at some point or another.

**Technical Note:** When more than one plugin has a `VersionMacros.h` file, attempting to include it in a dependent project will use the first one found among dependencies listed in the `.Build.cs` file. You can disambiguate between them by specifying the plugin in the path when including it (i.e. `MyPlugin/Public/VersionMacros.h`), or by simply renaming the header file with a prefix related to your plugin (i.e. `MyPluginVersionMacros.h`). In any case, you shouldn't have to worry about multiple plugins conflicting with each other.

[Jump to macro examples](#using-versionmacrosh)

## Prebuild Scripts

Some version portability problems can't be solved with preprocessor macros alone.
1. A notable limitation of preprocessor macros in Unreal is you can't wrap "magic" macros (`UCLASS`, `USTRUCT`, `UPROPERTY`, `UFUNCTION`) in preprocessor logic. However, UnrealHeaderTool allows `#if 1` and `#if 0` to wrap their "magic" macros. Prebuild scripts take advantage of that to work around those limitations by annotating the `#if` directives with "fake macros" in specially formatted comments.
2. Another common compatibility issue between UE4/UE5 is `TObjectPtr`. The prebuild scripts have a feature to automatically convert those to raw pointers in UE4 builds, and add an inline annotation to allow forward-compatibility when a user upgrades the engine version of their project.

When configured in your `.uplugin` file, prebuild scripts will automatically execute whenever you compile.

[Jump to prebuild examples](#using-prebuild-scripts)

### Prebuild Features

[`PrebuildConfig.py`](Resources/BuildScripts/PrebuildConfig.py) gives you options to take advantage of this, plus some other goodies:

- `MacroReplacements` is a dictionary where you can configure "fake" macros of the form `#if <0 or 1> // MY_CUSTOM_MACRO`. The prebuild script will automatically change matching code lines between `0` and `1` depending on your engine version or a constant value you've specified.
- `CustomPrebuildHeaders` is a list of header file paths to auto-generate `MacroReplacements` for. It will only consider `#define` directives that use a constant (`1` or `0`) value, or the `UE_VERSION_*` macros seen in `VersionMacros.h`. It does not actually compile the header file, so complex macros that use arithmatic/logical operators or that depend on other headers (besides `VersionMacros.h`) will be ignored.
- `AllowDynamicVersionMacroReplacements` will interpret lines of the form `#if <0 or 1> // UE_VERSION_*(major,minor)` to match the macros defined in `VersionMacros.h` and change between `1` and `0` according to your engine version.
- `AllowObjectPtrReplacements` provides backward/forward compatibility with `TObjectPtr`, which is a common issue for UE4/UE5 cross-compatibility. In UE4 it will replace all `TObjectPtr<T>` with `T* /* TObjectPtr */`. In UE5 it will replace all `T* /* TObjectPtr */`with `TObjectPtr<T>`.

By default, all features are enabled. Disable the ones you don't need to speed up the prebuild phase.

### Prebuild Implementation

Here's how it works:
1. When you start a build, Unreal parses your `.uplugin` file and executes its `"PreBuildSteps"` in your host platform shell.
2. `PreBuildSteps` exports variables from Unreal to the host shell environment so scripts can access them. The relevant variables are `EngineDir` and `PluginDir`.
3. `PreBuildSteps` executes the shim script contained in `Resources/BuildScripts/<HostPlatform>/`. On Windows this is a Powershell script. On Mac/Linux it's a Bash script.
4. The shim script first deduces your Unreal Engine version using the `Build.version` file in your engine directory.
5. The shim script then deduces a reliable Python executable location. On Windows, it will use the `python.exe` that's bundled with Unreal according to your engine version. If that fails (i.e. UE 4.8 or lower), it will search your environment `PATH` for `python.exe` or `python3.exe` (in that order), with some special handling for the fake Windows `python3` shim. On Mac/Linux, it will search for an executable named `python3` or `python` (in that order) using your environment `PATH`.
6. The shim script then executes [`Prebuild.py`](Resources/BuildScripts/Prebuild.py).
7. [`Prebuild.py`](Resources/BuildScripts/Prebuild.py) performs text replacements in your plugin source files according to your engine version and your settings in [`PrebuildConfig.py`](Resources/BuildScripts/PrebuildConfig.py).
8. The prebuild script will only modify source files that actually require changes, which makes it friendly with incremental builds. It also won't modify a source file if the script fails part-way through for some reason, meaning you don't need to worry about data loss if the script blows up. That shouldn't happen anyway, but there's a safeguard against it just in case.

The benefit of using `PreBuildSteps` is your plugin can safely be copy/pasted from a newer version of Unreal to an older one (and vice versa) and still compile! At least as long as you're diligent about `#if`ing out newer dependency references in your `.Build.cs` files and using the `Optional` field for newer dependencies in the `"Plugins"` section of your `.uplugin` file

### Technical Notes for PrebuildConfig.py

- `SourceFileEncoding` is passed to `io.open` as the `encoding` option when reading/writing source files.
- `EncodingErrorHandling` is passed to `io.open` as the `errors` option when reading/writing source files.
- `ProcessDirs` is a list of directories to recursively perform replacements in. The more specific you are here, the faster the prebuild script will complete. By default, it does replacements in every file under the plugin `Source` directory. It's not a bad idea to replace that with more specific directories with files you care about.
- `MatchHeaderFiles` is a regex pattern list for header files (`.h`). These are used to determine which files to perform "fake" macro replacements in by default.
- `MatchImplementationFiles` is a regex pattern list for implementation files (`.cpp`). These are used in conjunction with `MatchHeaderFiles` to determine which files to perform `TObjectPtr` replacements in.
- `MatchAllSourceFiles` is the combination of `MatchHeaderFiles` and `MatchImplementationFiles`.

# Installation

The VersionMacros plugin is not meant to be installed in your project. Instead, you should copy the parts you need to the plugins you wish to use it on. This is explained in greater detail below.

## Installing VersionMacros.h

The [`VersionMacros.h`](Source/VersionMacros/Public/VersionMacros.h) header file is located in [`Source/VersionMacros/Public/`](Source/VersionMacros/Public/) and is meant to be copied to your plugin `Source/<PluginName>/Public/` folder.

## Installing Prebuild Scripts

To add the prebuild scripts to your own plugin:
1. Copy the [`Resources/BuildScripts/`](Resources/BuildScripts/) folder to your plugin `Resources/` folder.
2. Copy the `"PreBuildSteps"` section from [`VersionMacros.uplugin`](VersionMacros.uplugin) to your `.uplugin` file.
3. Modify the prebuild scripts as needed for your plugin. Use the [`PrebuildConfig.py`](Resources/BuildScripts/PrebuildConfig.py) file to customize to your project needs. Disable the features you don't need to improve prebuild performance.
4. (Optional) Add a `Prebuild.h` file to your plugin and update [`PrebuildConfig.py`](Resources/BuildScripts/PrebuildConfig.py) to point to it in the `CustomPrebuildHeaders` list. This plugin comes with a sample [`PrebuildTemplate.h`](Source/VersionMacros/Public/PrebuildTemplate.h) file.

# Using VersionMacros.h

### Example 1: Version-specific header files

Starting in UE 5.4, the declaration of `FHitResult` was moved to a new header file.
```c++
#if UE_VERSION_MINIMUM(5,4)
#include "Engine/HitResult.h"
#endif
```

### Example 2: Version-specific function signatures
Starting in UE 5.3, `FMessageDialog::Open` had a signature change that broke my build.
```c++
#if UE_VERSION_MINIMUM(5,3)
	const EAppReturnType::Type Result = FMessageDialog::Open(EAppMsgType::YesNo, Message, Title);
#else
	const EAppReturnType::Type Result = FMessageDialog::Open(EAppMsgType::YesNo, Message, &Title);
#endif
```

### Example 3: Version-specific coding practices

Before UE 4.24, `bReplicates` was the way to inform a component/actor that it should replicate. This was later replaced with `SetIsReplicatedByDefault`.
```c++
#if UE_VERSION_BELOW(4,24)
	bReplicates = true;
#else
	SetIsReplicatedByDefault(true);
#endif
```

### Example 4: Working around missing operator overloads
There was a concatenation operator missing for `FString`/`FStringView` until UE 4.27, and the necessary implementation for it also differed in UE 4.24. `FStringView` didn't exist until 4.24 so I was able to build this overload into my backported implementation in 4.23 and lower.
```c++
#if UE_VERSION_WITHIN(4,24, 4,27)
FString operator+(const FStringView& Lhs, const FString& RHS)
{
#if UE_VERSION_EQUAL(4,24)
	return Lhs.ToString() + RHS;
#else
	return FString(Lhs.GetData()) + RHS;
#endif
}
#endif
```

### Example 5: Making portable utility functions
UE 4.24 was the last version you could access `APlayerState::bIsABot` directly, after which it was deprecated and replaced with `APlayerState::IsABot()`, so I made this utility function to wrap that.
```c++
static bool IsABot(APlayerState* PS)
{
#if UE_VERSION_MAXIMUM(4,24)
	return IsValid(PS) && PS->bIsABot;
#else
	return IsValid(PS) && PS->IsABot();
#endif
}
```

# Using Prebuild Scripts

### Example 1: Disabling a UPROPERTY on certain versions of Unreal

Let's say I have a property that should only exist for Unreal 5.3 and later.

The following would give a compile error:
```c++
#if UE_VERSION_MINIMUM(5,3)
UPROPERTY()
bool MyProperty = true;
#endif
```
Sadly, UnrealHeaderTool forbids it. I'm not sure why exactly, but I'm sure Epic had their reasons.

To overcome this limitation when supporting plugins that span many versions of Unreal, I do something like this instead:
```c++
#if 1 // UE_VERSION_MINIMUM(5,3)
UPROPERTY()
bool MyProperty = true;
#endif
```
The above code is allowed!

When the prebuild script sees a line of the form `#if <0 or 1> // UE_VERSION_*(major,minor)` it will automatically switch between `1` (enabled) and `0` (disabled) depending on the macro you used.

If you wanted to use a custom macro name for it (which is more efficient) the easiest way to do it is to add a `Prebuild.h` file to your plugin's `Public` source files.

Example `Prebuild.h`:
```c++
#pragma once
#include "VersionMacros.h"

#define SHOULD_MY_PROPERTY_EXIST UE_VERSION_MINIMUM(5,3)
```
Example of using it on the `UPROPERTY`:
```c++
#if 1 // SHOULD_MY_PROPERTY_EXIST
UPROPERTY()
bool MyProperty = true;
#endif
```
If you need to use a custom prebuild header path you can modify `CustomPrebuildHeaders` in [`PrebuildConfig.py`](Resources/BuildScripts/PrebuildConfig.py). The default is `Source/{PluginName}/Public/Prebuild.h`. You can see examples of its use in [Test.h](Source/VersionMacros/Public/Test.h) and [Prebuild.h](Source/VersionMacros/Public/Prebuild.h).

### Example 2: Backporting a modern UFUNCTION

Forward-compatibility on Blueprint Libraries is pretty neat. Here's an example of backporting the `IsA ( soft )` node to UE 5.4 and lower in a way that's forward-compatible.

Example `Prebuild.h`:
```c++
#pragma once
#include "VersionMacros.h"

#define BACKPORT_ISA_SOFT UE_VERSION_MAXIMUM(5,4)
```

Example `MyBackportedKismetSystemLibrary.h`:
```c++
UCLASS()
class UMyBackportedKismetSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
#if 0 // BACKPORT_ISA_SOFT
	/** Returns true if Object is of type SoftClass - either an instance of the class or child class, or implements the interface. Alternative to Cast - slower but without adding a hard reference. */
	UFUNCTION(BlueprintCallable, Category = "Utilities", meta = (ExpandEnumAsExecs = ReturnValue, DisplayName = "IsA ( soft )"))
	static bool IsObjectOfSoftClass(const UObject* Object, TSoftClassPtr<UObject> SoftClass);
#endif
};
```

Example `MyBackportedKismetSystemLibrary.cpp`:
```c++
#include "MyBackportedKismetSystemLibrary.h"
#include "Prebuild.h"

#if BACKPORT_ISA_SOFT
bool UMyBackportedKismetSystemLibrary::IsObjectOfSoftClass(const UObject* Object, TSoftClassPtr<UObject> SoftClass)
{
	if (!Object)
	{
		return false;
	}

	TSubclassOf<UObject> ObjectClass = SoftClass.Get();
	if (!ObjectClass)
	{
		return false;
	}

	TSubclassOf<UInterface> InterfaceClass = ObjectClass.Get();
	if (InterfaceClass)
	{
		check(Object->GetClass());
		return Object->GetClass()->ImplementsInterface(InterfaceClass);
	}

	return Object->IsA(ObjectClass);
}
#endif
```

If you import a Blueprint using that node from UE 5.4 to UE 5.5 it will actually replace it with the built-in UE 5.5 node!

### Example 3: Backporting a modern UCLASS

`UTickableWorldSubsystem` didn't get introduced until UE 4.27, so I created a backport of it for my plugin in earlier versions of Unreal. The problem is `UObject` types need a `UCLASS()` macro, so I couldn't do the following:
```c++
#if UE_VERSION_MAXIMUM(4,26)
#include "Subsystems/Subsystem.h
#else
#include "Subsystems/WorldSubsystem.h
#endif

#include "MyBackportedTickableWorldSubsystem.generated.h"

#if UE_VERSION_MAXIMUM(4,26) // THIS LINE WILL FAIL TO COMPILE
UCLASS()
class UMyBackportedTickableWorldSubsystem : public UDynamicSubsystem
{
    GENERATED_BODY()
    // backported declarations
};
#else
class UMyBackportedTickableWorldSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()
    // no declarations, it's all inherited from UTickableWorldSubsystem
};
#endif
```

Prebuild scripts helped me work around this:
```c++
#if UE_VERSION_MAXIMUM(4,26)
#include "Subsystems/Subsystem.h
#else
#include "Subsystems/WorldSubsystem.h
#endif

#include "MyBackportedTickableWorldSubsystem.generated.h"

#if 0 // UE_VERSION_MAXIMUM(4,26)
UCLASS()
class UMyBackportedTickableWorldSubsystem : public UDynamicSubsystem
{
    GENERATED_BODY()
    // backported declarations
};
#else
UCLASS()
class UMyBackportedTickableWorldSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()
    // no declarations, it's all inherited from UTickableWorldSubsystem
};
#endif
```

With the above code in place, I can use `UMyBackportedTickableWorldSubsystem` instead of `UTickableWorldSubsystem` throughout my plugin without needing to maintain a separate version of my plugin in UE 4.26 and lower.

# Platform Support

`VersionMacros.h` should work on all platforms.

Prebuild scripts should work on all Windows or POSIX-compliant (Mac/Linux) systems that support Unreal Engine.

Please file an issue if you run into a platform where this doesn't work.

# Engine Version Support

I've tested this in UE versions 4.12 to 5.x, but it should work in lower versions as well.

Support Notes:
- UE 4.14 sometimes has problems with wrapping entire `UCLASS` declarations in `#if 0` / `#if 1` preprocessor blocks, but can usually be resolved with a clean rebuild.
- UE 4.12 and 4.13 wouldn't compile any `UCLASS`/`USTRUCT` declarations for me, so I have certain test cases disabled on those versions of Unreal until I find out why.
- I'm unable to test UE 4.10 and 4.11 until I can find a Visual Studio 2015 installer that doesn't include Update 3, which breaks builds on those versions of UE.
- I'm unable to test UE 4.9 and lower. I was unable to install Visual Studio 2013 on my system due to an unspecified conflict.
- UE 4.8 and lower do not bundle a Python executable on Windows, so you'll need Python installed and in your environment `PATH` in order to run the prebuild scripts.

Please file an issue if you run into a version of Unreal where this doesn't work.

# Closing Remarks

If this was helpful for one of your projects, consider leaving a thank-you note in [Discussions](https://github.com/sbseltzer/VersionMacros/discussions). :)

My website: [sbseltzer.net](https://sbseltzer.net/)
