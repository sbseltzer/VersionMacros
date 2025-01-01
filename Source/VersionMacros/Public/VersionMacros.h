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

#pragma once

// Include file that defines ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION
#include "Runtime/Launch/Resources/Version.h"

// Make sure we have everything we need to implement the version macros.
#if !defined(ENGINE_MAJOR_VERSION) || !defined(ENGINE_MINOR_VERSION) || !defined(ENGINE_PATCH_VERSION)
#error "Engine version macros are not defined!"
#endif

// Major, Minor, and Patch versions must be positive numbers.
#if ENGINE_MAJOR_VERSION < 0 || ENGINE_MINOR_VERSION < 0 || ENGINE_PATCH_VERSION < 0
#error "One of the built-in engine version macros is negative!"
#endif

// Major, Minor, and Patch versions must be between 1 and 3 digits long (inclusive).
#if ENGINE_MAJOR_VERSION > 999 || ENGINE_MINOR_VERSION > 999 || ENGINE_PATCH_VERSION > 999
#error "One of the built-in engine version macros exceeded its maximum range!"
#endif

// Express version as a simple polynomial for easy comparisons.
#define UE_VERSION_TO_INT(major, minor, patch) ((major)*1000000+(minor)*1000+(patch))
// Explicit comparison between two version tuples.
#define UE_VERSION_COMPARE_EXPLICIT(major1, minor1, patch1, op, major2, minor2, patch2) (UE_VERSION_TO_INT(major1, minor1, patch1) op UE_VERSION_TO_INT(major2, minor2, patch2))
// Comparisons against current version with a specified operator.
#define UE_VERSION_COMPARE_CURRENT(op, major, minor) UE_VERSION_COMPARE_EXPLICIT(ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, 0, op, major, minor, 0)
// UE_VERSION_COMPARE_CURRENT_WITH_PATCH is available if you wish to implement your own comparison macros with it.
// Patch versions in Unreal typically do not denote meaningful changes that would affect plugin compatibility, but it's here if you need it.
#define UE_VERSION_COMPARE_CURRENT_WITH_PATCH(op, major, minor, patch) UE_VERSION_COMPARE_EXPLICIT(ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION, op, major, minor, patch)

// Human-readable version comparison macros:
#define UE_VERSION_BELOW(major, minor) UE_VERSION_COMPARE_CURRENT(<, major, minor)
#define UE_VERSION_ABOVE(major, minor) UE_VERSION_COMPARE_CURRENT(>, major, minor)
#define UE_VERSION_EQUAL(major, minor) UE_VERSION_COMPARE_CURRENT(==, major, minor)
#define UE_VERSION_MINIMUM(major, minor) UE_VERSION_COMPARE_CURRENT(>=, major, minor)
#define UE_VERSION_MAXIMUM(major, minor) UE_VERSION_COMPARE_CURRENT(<=, major, minor)
#define UE_VERSION_BETWEEN_INCLUSIVE(major_min, minor_min, major_max, minor_max) (UE_VERSION_MINIMUM(major_min, minor_min) && UE_VERSION_MAXIMUM(major_max, minor_max))
#define UE_VERSION_BETWEEN_EXCLUSIVE(major_min, minor_min, major_max, minor_max) (UE_VERSION_ABOVE(major_min, minor_min) && UE_VERSION_BELOW(major_max, minor_max))