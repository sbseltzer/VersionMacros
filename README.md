# What this is

This repository has 2 things that help with multi-version support in Unreal code plugins:
- The [`VersionMacros.h`](Source/VersionMacros/Public/VersionMacros.h) header file
- [Prebuild scripts](Resources/BuildScripts) that work around preprocessor limitations in UnrealHeaderTool

Both are optional and independent of one another.

This was originally designed for use on my personal collection of plugins, which I use in various versions of UE4 and UE5. I sometimes needed to backport modern UE features, which normally is not possible with preprocessor macros alone.

## Installation

The VersionMacros plugin is not meant to be installed in your project. Instead, you should copy the parts you need to the plugins you wish to use it on. This is explained in greater detail below.

# Usage

The `VersionMacros.h` header file is located in `Source/VersionMacros/Public/` and is meant to be copied to your plugin `Source/<PluginName>/Public/` folder.

The prebuild scripts are located in the `Resources/BuildScripts/` folder, which is meant to be copied to your plugin `Resources/` folder.

The `VersionMacros.uplugin` file has an example of using the prebuild scripts, which can be copied to your own `.uplugin` files if you choose to use the build scripts.

## Macros

`VersionMacros.h` provides the following macros for arbitrary engine version comparisons.

```
UE_VERSION_BELOW(major, minor)
UE_VERSION_ABOVE(major, minor)
UE_VERSION_EQUAL(major, minor)
UE_VERSION_MINIMUM(major, minor)
UE_VERSION_MAXIMUM(major, minor)
UE_VERSION_BETWEEN_INCLUSIVE(major_min, minor_min, major_max, minor_max)
UE_VERSION_BETWEEN_EXCLUSIVE(major_min, minor_min, major_max, minor_max)
```
My all-time favorites are `UE_VERSION_MINIMUM` and `UE_VERSION_MAXIMUM`, but I've used all of them at some point or another.

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
#if UE_VERSION_BETWEEN_INCLUSIVE(4,24, 4,27)
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

## PreBuild Scripts

A notable limitation of preprocessor macros in Unreal is you can't wrap Unreal "magic" macros in preprocessor logic.

If you're not sure what that means, you probably don't need these prebuild scripts for your project.

To add the build scripts to your own plugin:
1. Copy the `Resources/BuildScripts/` folder to your plugin `Resources/` folder.
2. Copy the `"PreBuildSteps"` section from `VersionMacros.uplugin` to your `.uplugin` file.
3. Modify the prebuild scripts as needed for your plugin. Use the `PrebuildConfig.py` file to customize to your project needs.

The benefit of using `PreBuildSteps` for this is your plugin can safely be copy/pasted from a newer version of Unreal to an older one and still compile! At least as long as you're diligent about `#if`ing out newer dependency references in your `.Build.cs` files and using the `Optional` field for newer dependencies in your `.uplugin` file `"Plugins"` section.

We'll review some specific use-cases for these prebuild scripts next.

### Example 1: Disabling a UPROPERTY on certain versions of Unreal

Let's say I've defined `SHOULD_MY_PROPERTY_EXIST` as `UE_VERSION_MINIMUM(5,5)` and want to hide a specific `UPROPERTY` on earlier engine versions.

The following would give a compile error:
```c++
#if SHOULD_MY_PROPERTY_EXIST
UPROPERTY()
bool MyProperty = true;
#endif
```
Sadly, UnrealHeaderTool forbids it. I'm not sure why exactly, but I'm sure Epic had their reasons.

To overcome this limitation when supporting plugins that span many versions of Unreal, I do something like this instead:
```c++
#if 1 // SHOULD_MY_PROPERTY_EXIST
UPROPERTY()
bool MyProperty = true;
#endif
```
The above code is allowed! But for it to have a meaningful effect, you need to use a prebuild script to ensure it changes that `1` to a `0` and vice versa depending on your version of Unreal.

Example `MacroReplacements` in `PrebuildConfig.py`:
```python
MacroReplacements = {
    "SHOULD_MY_PROPERTY_EXIST": {
        "Version": "5.5",
        "Compare": '>='
    },
    # More of these "fake" macro configurations can be added
}
```

### Example 2: Backporting a modern UCLASS

`UTickableWorldSubsystem` didn't get introduced until UE 4.27, so I created a backport of it for earlier versions of Unreal. The problem is `UObject` types need a `UCLASS()` macro, so I couldn't do the following:
```c++
#define COMPAT_TICKABLE_WORLD_SUBSYSTEM UE_VERSION_MAXIMUM(4,26)
#if COMPAT_TICKABLE_WORLD_SUBSYSTEM
UCLASS()
class UTickableWorldSubsystem : public UDynamicSubsystem
{
    // backported declarations
};
#endif
```

Prebuild scripts helped me work around this.

Example `WorldSubsystemCompat.h` file:
```c++
#define COMPAT_TICKABLE_WORLD_SUBSYSTEM UE_VERSION_MAXIMUM(4,26)
#if 0 // COMPAT_TICKABLE_WORLD_SUBSYSTEM
UCLASS()
class UTickableWorldSubsystem : public UDynamicSubsystem
{
    // backported declarations
};
#endif
```
Example `MacroReplacements` in `PrebuildConfig.py`:
```python
MacroReplacements = {
    "COMPAT_TICKABLE_WORLD_SUBSYSTEM": {
        "Version": "4.26",
        "Compare": '<=',
        "MatchFiles": [r'.*/WorldSubsystemCompat.h']
    },
    # More of these "fake" macro configurations can be added
}
```
The above configuration will do the following:
- Automatically replace `#if 0 // COMPAT_TICKABLE_WORLD_SUBSYSTEM` with `#if 1 // COMPAT_TICKABLE_WORLD_SUBSYSTEM` inside the `WorldSubsystemCompat.h` file anytime I start a build on Unreal 4.26 and lower.
- Automatically replace `#if 1 // COMPAT_TICKABLE_WORLD_SUBSYSTEM` with `#if 0 // COMPAT_TICKABLE_WORLD_SUBSYSTEM` inside the `WorldSubsystemCompat.h` file anytime I start a build on Unreal 4.27 and greater.

Some other possible use-cases:
- Adding version-specific `UFUNCTION`/`USTRUCT` declarations
- Changing how a `UPROPERTY` is declared, such as whether to use `TObjectPtr<>` (introduced in UE 5.0) vs a raw pointer

### Technical Notes for PrebuildConfig.py

`ProcessDirs` is a list of directories to recursively perform replacements in. The more specific you are here, the faster the prebuild script will complete. By default, it does replacements in every file under the plugin `Source` directory. It's not a bad idea to replace that with more specific directories with files you care about.

`MacroReplacements` is a dictionary of macro replacement configurations, which we've already covered in examples.

There are some options at the bottom that generate some helpful macro replacements.
- `AllowVersionOrLaterReplacements` adds `UE_<MajorVersion>_<MinorVersion>_OR_LATER`.
- `AllowVersionOrEarlierReplacements` adds `UE_<MajorVersion>_<MinorVersion>_OR_EARLIER`.
- `DefaultMatchFiles` are file patterns to match for the generated macro replacements.

# Platform Support

`VersionMacros.h` should work on all platforms.

Prebuild script platforms tested so far:

- [x] Win64
- [ ] Mac
- [ ] Linux

Please file an issue if you run into a platform where this doesn't work.

# Engine Version Support

I've tested this in UE versions 4.22 to 5.x, but it should work in lower versions as well.

Please file an issue if you run into a version of Unreal where this doesn't work.

# Areas for improvement

1. Make `Prebuild.py` recognize and evaluate the same `UE_VERSION_*` macros defined in `VersionMacros.h`.
2. Add a script to automate installing `VersionMacros.h` and/or `BuildScripts` to another plugin.

# Closing Remarks

If this was helpful for one of your projects, consider leaving a thank-you note in the issues. :)

My website: [sbseltzer.net](https://sbseltzer.net/)
