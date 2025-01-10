# Version Macros for supporting projects that span multiple versions of Unreal.
# https://github.com/sbseltzer/VersionMacros
# 
# Copyright Sam Seltzer-Johnston 2025. All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
# Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import os
import json
import re
import io
import PrebuildConfig
from PrebuildConst import *

# Environment Variables
# PluginDir is used for source file path construction. Will assume current working directory if not set.
PluginDir = os.environ['PluginDir'] or "."
# You must either set EngineDir or UEMajorVersion/UEMinorVersion
# EngineDir is used to deduce engine version when it isn't explicitly provided by UEMajorVersion/UEMinorVersion
EngineDir = os.environ['EngineDir']
MajorVersion = os.environ['UEMajorVersion']
MinorVersion = os.environ['UEMinorVersion']

# Deduce engine version if not already provided by environment
if MajorVersion == None or MinorVersion == None:
    if EngineDir == None:
        print("EngineDir environment variable is required to find Build.version file!")
        exit(1)
    VersionFilePath = EngineDir + "/Build/Build.version"
    if not os.path.exists(VersionFilePath):
        print("Failed to find engine version file!")
        exit(1)
    with open(VersionFilePath) as f:
        BuildVersion = json.load(f)
        MajorVersion = str(BuildVersion['MajorVersion'])
        MinorVersion = str(BuildVersion['MinorVersion'])

EngineVersion = MajorVersion + "." + MinorVersion

def version_to_int(major, minor, patch=0):
    return int(major)*1000000+int(minor)*1000+int(patch)

EngineVersionAsInt = version_to_int(MajorVersion, MinorVersion)

def do_comparison(version, compare):
    # If version is a non-string (i.e. float or decimal) convert to string so we can separate the major/minor versions
    if type(version) != str:
        version = str(version)
    if '.' not in version:
        version += ".0"
    # Split version into major/minor integers so we can do a polynomial comparison
    # This works around an edge case in UE4 where the minor version exceeded 9
    # For example UE 4.9 is a lower version than UE 4.27, but a numeric comparison would evaluate to the opposite!
    [major, minor] = re.split(r'\.', version, 1)
    version_as_int = version_to_int(major, minor)
    compare_id = (type(compare) == int) and compare or OperatorStringToID.get(compare)
    if compare_id == None:
        print("Error: Invalid comparison operator '" + compare + "'!")
        exit(1)
    # Old versions of Unreal use Python 2, which doesn't have match statements, so we use if-else here
    if compare_id == EQUAL:
        return EngineVersionAsInt == version_as_int
    elif compare_id == BELOW:
        return EngineVersionAsInt < version_as_int
    elif compare_id == MAXIMUM:
        return EngineVersionAsInt <= version_as_int
    elif compare_id == ABOVE:
        return EngineVersionAsInt > version_as_int
    elif compare_id == MINIMUM:
        return EngineVersionAsInt >= version_as_int
    print("Error: Unhandled comparison operator: " + compare)
    exit(1)

def parse_prebuild_header_line(line):
    match = re.search(r'#define\s+([\w_\d]+)\s+((!?)\s*UE_VERSION_(\w+)\s*\(\s*(\d+)\s*,\s*(\d+)\s*\))', line)
    if match:
        macro_name = match.group(1)
        is_negated = match.group(3) == '!'
        comparison_string = match.group(4)
        version = match.group(5) + "." + match.group(6)
        PrebuildConfig.MacroReplacements[macro_name] = {
            "Version": version,
            "MatchFiles": PrebuildConfig.MatchHeaderFiles,
            "EvaluatedTo": is_negated != do_comparison(version, comparison_string)
        }

def parse_prebuild_header(path):
    header_file = io.open(path, 'r', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
    for line in header_file:
        parse_prebuild_header_line(line);
    header_file.close()

def handle_object_ptr_replacement(file_path, line):
    new_line = line
    changed = False
    should_replace = False
    for pattern in PrebuildConfig.MatchAllSourceFiles:
        if (re.match(pattern, file_path)):
            should_replace = True
            break
    if should_replace:
        if do_comparison("5.0", BELOW):
            object_ptr_match = re.search(r'TObjectPtr<([\s\w_:]+)>', new_line)
            if object_ptr_match:
                new_line = re.sub(r'TObjectPtr<([\s\w_:]+)>', r'\1* /* TObjectPtr */', new_line)
                changed = True
        else:
            raw_object_ptr_match = re.search(r'([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', new_line)
            if raw_object_ptr_match:
                new_line = re.sub(r'([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', r'TObjectPtr<\1>', new_line)
                changed = True
    return new_line, changed

def handle_dynamic_fake_macro_replacement(file_path, line):
    new_line = line
    changed = False
    is_dynamic_macro_replacement = False
    should_replace = False
    # Only header files benefit from this kind of fake macro replacement
    for pattern in PrebuildConfig.MatchHeaderFiles:
        if (re.match(pattern, file_path)):
            should_replace = True
            break
    if should_replace:
        match = re.search(r'^\s*#if (\d)\s*//\s*(!?)UE_VERSION_(\w+)\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)', new_line)
        if match:
            current_literal_expression = int(match.group(1))
            is_negated = match.group(2) == '!'
            comparison_name = match.group(3)
            is_dynamic_macro_replacement = True
            version = match.group(4) + "." + match.group(5)
            if (do_comparison(version, comparison_name) != is_negated):
                if (current_literal_expression == 0):
                    new_line = re.sub(r'#if 0(\s*)//(\s*)(!?UE_VERSION_\w+\s*\(\s*\d+\s*,\s*\d+\s*\))', r'#if 1\1//\2\3', new_line)
                    changed = True
            else:
                if (current_literal_expression == 1):
                    new_line = re.sub(r'#if 1(\s*)//(\s*)(!?UE_VERSION_\w+\s*\(\s*\d+\s*,\s*\d+\s*\))', r'#if 0\1//\2\3', new_line)
                    changed = True
    return new_line, changed, is_dynamic_macro_replacement
                    
def handle_fake_macro_replacement(file_path, line):
    new_line = line
    changed = False
    match = re.search(r'^\s*#if (\d)\s*//\s*(!?)(\w[\w\d_]+)', new_line)
    # Search the dictionary of user-defined macros that are associated with a version and comparison
    if match:
        current_literal_expression = int(match.group(1))
        is_negated = match.group(2) == '!'
        macro_text = match.group(3)
        replacement_info = PrebuildConfig.MacroReplacements.get(macro_text)
        match_files = replacement_info and replacement_info.get('MatchFiles') or PrebuildConfig.MatchHeaderFiles
        should_replace = replacement_info != None
        if (match_files and len(match_files) > 0):
            should_replace = False
            for pattern in match_files:
                if (re.match(pattern, file_path)):
                    should_replace = True
                    break
        if (should_replace):
            cached_comparison = replacement_info.get("EvaluatedTo")
            if cached_comparison == None:
                compare_version = replacement_info.get('Version')
                if not compare_version:
                    print("ERROR: Macro Replacement " + macro_text + " is missing 'Version' value!")
                    exit(1)
                compare_type = replacement_info.get('Compare')
                if not compare_type:
                    print("ERROR: Macro Replacement " + macro_text + " is missing 'Compare' value!")
                    exit(1)
                cached_comparison = do_comparison(compare_version, compare_type)
                replacement_info["EvaluatedTo"] = cached_comparison
            if (cached_comparison != is_negated):
                if (current_literal_expression == 0):
                    new_line = re.sub(r'#if 0(\s*)//(\s*)(!?\w+)', r'#if 1\1//\2\3', new_line)
                    changed = True
            else:
                if (current_literal_expression == 1):
                    new_line = re.sub(r'#if 1(\s*)//(\s*)(!?\w+)', r'#if 0\1//\2\3', new_line)
                    changed = True
        if not replacement_info:
            print("Failed to find Macro Replacement Info for " + macro_text)
    return new_line, changed
    
def replace_line_in_file(file_path, line):
    changed = False
    new_line = line
    is_dynamic_macro_replacement = False
    # TObjectPtr replacement
    if PrebuildConfig.AllowObjectPtrReplacements:
        [new_line, changed] = handle_object_ptr_replacement(file_path, new_line)
    # Fake macro replacement (UE_VERSION_* form)
    if PrebuildConfig.AllowDynamicVersionMacroReplacements:
        [new_line, changed, is_dynamic_macro_replacement] = handle_dynamic_fake_macro_replacement(file_path, new_line)
    # Fake macro replacement (user-defined form)
    if not is_dynamic_macro_replacement:
        [new_line, changed] = handle_fake_macro_replacement(file_path, new_line)
    if (changed):
        print("Changed:\n  " + line + "To:\n  " + new_line)
    return new_line, changed

def replace_in_file(file_path):
    input_file = io.open(file_path, 'r', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
    output_file = io.open(file_path + ".new", 'w', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
    any_replaced = False
    for line in input_file:
        new_line, changed = replace_line_in_file(file_path, line)
        any_replaced = any_replaced or changed
        output_file.write(new_line)
    input_file.close()
    output_file.close()
    # Don't overwrite the source file if nothing changed!
    # This prevents file timestamps from updating unnecessarily, which would trigger a rebuild of those source files
    if any_replaced:
        os.rename(file_path, file_path + ".old")
        os.rename(file_path + ".new", file_path)
        os.remove(file_path + ".old")
    else:
        os.remove(file_path + ".new")

def do_replacements_in_directory_recursive(directory):
    for item in os.listdir(directory):
        # Always use / path separators to simplify file matching regex
        path = os.path.join(directory, item).replace("\\", "/")
        if os.path.isdir(path):
            do_replacements_in_directory_recursive(path)
        elif os.path.isfile(path):
            replace_in_file(path)

for path in PrebuildConfig.CustomPrebuildHeaders:
    if os.path.exists(path):
        parse_prebuild_header(path)

for dir in PrebuildConfig.ProcessDirs:
    do_replacements_in_directory_recursive(os.path.join(PluginDir, dir))