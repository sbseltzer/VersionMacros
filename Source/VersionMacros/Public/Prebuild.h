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

//===============================================================================
// NOTE: This file is only for testing purposes. Do not copy it to your project!
// See PrebuildTemplate.h for more information.
//===============================================================================

#pragma once
#include "VersionMacros.h"

#define TEST_MACRO_A UE_VERSION_MAXIMUM(5, 0)
#define TEST_MACRO_B UE_VERSION_BELOW(4, 27)
#define TEST_MACRO_C UE_VERSION_WITHIN(4, 27, 5,4)

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