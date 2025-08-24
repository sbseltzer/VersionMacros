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

# GENERAL MAINTENANCE NOTES:
# Avoid importing any nonstandard Python libraries.
# All Python code must be forward/backward compatible between Python versions that ship with each Unreal Engine version.
# Version Reference:
#   UE 5.4-5.6 = Python 3.11.8
#   UE 5.0-5.3 = Python 3.9.7
#   UE 4.26-4.27 = Python 3.7.7
#   UE 4.19-4.25 = Python 2.7.14
#   UE 4.9-4.18 = Python 2.7.5
# We can only support bundled Python versions down to UE 4.9, before which the engine did not ship with Python.
# People compiling on Windows for versions of UE lower than that are expected to have Python in their PATH.
# On POSIX conforming platforms (MacOS/Linux) we use the system Python install, which will generally be Python 3.x

import os
import json
import re
import io
import traceback

from PrebuildConst import *
import PrebuildConfig

# Ensure MacroPrefixName is up to date with custom user prefix name
if PrebuildConfig.MacroPrefixName != None:
    MacroPrefixName = PrebuildConfig.MacroPrefixName
# Ensure MacroCommonName is up to date with custom user common name
if PrebuildConfig.MacroCommonName != None:
    MacroCommonName = PrebuildConfig.MacroCommonName
# Ensure OperatorStringToID table is up to date with custom user suffix names
if PrebuildConfig.MacroSuffixNames != None:
    for i in range(1, len(PrebuildConfig.MacroSuffixNames)):
        OperatorStringToID[MacroSuffixNames[i]] = i

# Environment Variables (set by PreBuildSteps in .uplugin file)
# PluginDir is used for source file path construction. Will assume current working directory if not set.
PluginDir = os.environ['PluginDir'] or os.path.curdir
# You must either set EngineDir or UEMajorVersion/UEMinorVersion
# EngineDir is used to deduce engine version when it isn't explicitly provided by UEMajorVersion/UEMinorVersion
EngineDir = os.environ['EngineDir']
MajorVersion = os.environ['UEMajorVersion']
MinorVersion = os.environ['UEMinorVersion']

def print_error_and_exit(message, source_file=None, line_num=None, exception=None):
    print_str = ""
    if (source_file != None):
        print_str += source_file
    if (line_num != None):
        print_str += ":" + str(line_num)
    if print_str != "":
        print_str += " : "
    print_str += message
    print("ERROR: " + print_str)
    if exception != None:
        traceback.print_exception(type(exception), exception, None, 10)
    exit(1)

# Deduce engine version if not already provided by environment
if MajorVersion == None or MinorVersion == None:
    if EngineDir == None:
        print_error_and_exit("EngineDir environment variable is required to find Build.version file!")
    VersionFilePath = EngineDir + "/Build/Build.version"
    if not os.path.exists(VersionFilePath):
        print_error_and_exit("Failed to find engine version file!")
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
        print_error_and_exit("Invalid comparison operator '" + compare + "'!")
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
    print_error_and_exit("Unhandled comparison operator: " + compare)
    return False

def parse_prebuild_header_line(line, file_path, line_num):
    match = re.search(r'#define\s+([\w_\d]+)\s+((!?)\s*' + MacroPrefixName + MacroCommonName + r'(\w+)\s*\(([\s\d,]+))', line)
    if match:
        macro_name = match.group(1)
        is_negated = match.group(3) == '!'
        comparison_name = match.group(4)
        args_string = match.group(5)
        args = re.split(r',', args_string, 3)
        num_args = len(args)
        version_matches = False
        if num_args == 2:
            [major, minor] = args
            version = str(int(major)) + "." + str(int(minor))
            version_matches = (do_comparison(version, comparison_name) != is_negated)
        elif num_args == 4 and (OperatorStringToID.get(comparison_name) == WITHIN):
            [min_major, min_minor, max_major, max_minor] = args
            min_version = str(int(min_major)) + "." + str(int(min_minor))
            max_version = str(int(max_major)) + "." + str(int(max_minor))
            version_matches = ((do_comparison(min_version, MINIMUM) and do_comparison(max_version, MAXIMUM)) != is_negated)
        else:
            print_error_and_exit("Invalid number of arguments (" + str(num_args) + ") for " + macro_name, file_path, line_num)
        PrebuildConfig.MacroReplacements[macro_name] = {
            "MatchFiles": type(PrebuildConfig.MacroReplacements.get(macro_name)) == dict and PrebuildConfig.MacroReplacements[macro_name].get("MatchFiles") or PrebuildConfig.DefaultMacroReplacementFiles,
            "EvaluatedTo": version_matches
        }
        # print("Registered Macro Replacement: " + macro_name + " = " + str(version_matches) + " | " + line + "\t" + str(PrebuildConfig.MacroReplacements[macro_name]))
    else:
        match = re.search(r'#define\s+([\w_\d]+)\s+([01])', line)
        if match:
            macro_name = match.group(1)
            constant_value = match.group(2)
            PrebuildConfig.MacroReplacements[macro_name] = {
                "MatchFiles": type(PrebuildConfig.MacroReplacements.get(macro_name)) == dict and PrebuildConfig.MacroReplacements[macro_name].get("MatchFiles") or PrebuildConfig.DefaultMacroReplacementFiles,
                "EvaluatedTo": bool(int(constant_value))
            }
            # print("Registered Macro Replacement: " + macro_name + " = " + str(bool(int(constant_value))) + " | " + line + "\t" + str(PrebuildConfig.MacroReplacements[macro_name]))

def parse_prebuild_header(path):
    try:
        header_file = io.open(path, 'r', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
        line_num = 1
        for line in header_file:
            try:
                parse_prebuild_header_line(line, path, line_num)
            except Exception as e:
                print_error_and_exit("Failed to parse prebuild header line: " + line, path, line_num, e)
            finally:
                line_num = line_num + 1
        header_file.close()
    except Exception as e:
        print_error_and_exit("Failed to open prebuild header file - Check CustomPrebuildHeaders in PrebuildConfig.py and make you've entered a valid file path relative to this plugin.", path, None, e)

def handle_object_ptr_replacement(line, file_path, line_num, was_prev_line_uproperty):
    new_line = line
    changed = False
    should_replace = False
    try:
        for pattern in PrebuildConfig.MatchAllSourceFiles:
            if (re.match(pattern, file_path)):
                should_replace = True
                break
        if should_replace:
            if do_comparison("5.0", BELOW):
                # TObjectPtr backward-portability
                object_ptr_match = re.search(r'TObjectPtr<([\s\w_:]+)>', new_line)
                if object_ptr_match:
                    if was_prev_line_uproperty:
                        # NOTE: The TObjectPtr rules for UPROPERTY are unambiguous, so we don't add an inline annotation for these conversions
                        new_line = re.sub(r'TObjectPtr<([\s\w_:]+)>', r'\1*', new_line)
                        changed = True
                    else:
                        new_line = re.sub(r'TObjectPtr<([\s\w_:]+)>', r'\1* /* TObjectPtr */', new_line)
                        changed = True
            else:
                # TObjectPtr forward-portability
                if was_prev_line_uproperty:
                    # NOTE: The TObjectPtr rules for UPROPERTY are unambiguous, so we don't require an inline annotation for these conversions
                    raw_fwd_object_ptr_match = re.search(r'class(\s+)(U[\w_:]+)\s*\*\s*', new_line)
                    if raw_fwd_object_ptr_match:
                        new_line = re.sub(r'class(\s+)(U[\w_:]+)\s*\*(\s*)', r'TObjectPtr<class\1\2>\3', new_line)
                        changed = True
                    if not changed:
                        raw_object_ptr_match = re.search(r'(U[\w_:]+)\s*\*\s*', new_line)
                        if raw_object_ptr_match:
                            new_line = re.sub(r'(U[\w_:]+)\s*\*(\s*)', r'TObjectPtr<\1>\2', new_line)
                            changed = True
                if not changed:
                    raw_fwd_object_ptr_match = re.search(r'class(\s+)([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', new_line)
                    if raw_fwd_object_ptr_match:
                        new_line = re.sub(r'class(\s+)([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', r'TObjectPtr<class\1\2>', new_line)
                        changed = True
                if not changed:
                    raw_object_ptr_match = re.search(r'([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', new_line)
                    if raw_object_ptr_match:
                        new_line = re.sub(r'([\w_:]+)\s*\*\s*/\*\s*TObjectPtr\s*\*/', r'TObjectPtr<\1>', new_line)
                        changed = True
    except Exception as e:
        print_error_and_exit("Failed to handle TObjectPtr replacements for line `" + line + "`", file_path, line_num, e)
    return new_line, changed

def handle_dynamic_fake_macro_replacement(line, file_path, line_num):
    new_line = line
    changed = False
    is_dynamic_macro_replacement = False
    should_replace = False
    # Only header files benefit from this kind of fake macro replacement
    for pattern in PrebuildConfig.DefaultMacroReplacementFiles:
        if (re.match(pattern, file_path)):
            should_replace = True
            break
    if should_replace:
        match = re.search(r'^\s*#\s*(el)?if\s+(\d)\s*//\s*(!?)' + MacroPrefixName + MacroCommonName + r'(\w+)\s*\(([\s\d,]+)\)', new_line)
        if match:
            elif_prefix = match.group(1)
            is_dynamic_macro_replacement = True
            current_literal_expression = int(match.group(2))
            is_negated = match.group(3) == '!'
            comparison_name = match.group(4)
            args_string = match.group(5)
            args = re.split(r',', args_string, 3)
            num_args = len(args)
            version_matches = False
            if num_args == 2:
                [major, minor] = args
                version = str(int(major)) + "." + str(int(minor))
                version_matches = (do_comparison(version, comparison_name) != is_negated)
            elif num_args == 4 and (OperatorStringToID.get(comparison_name) == WITHIN):
                [min_major, min_minor, max_major, max_minor] = args
                min_version = str(int(min_major)) + "." + str(int(min_minor))
                max_version = str(int(max_major)) + "." + str(int(max_minor))
                version_matches = ((do_comparison(min_version, MINIMUM) and do_comparison(max_version, MAXIMUM)) != is_negated)
            else:
                print_error_and_exit("Invalid number of arguments (" + str(num_args) + ")", file_path, line_num)
            if version_matches:
                if (current_literal_expression == 0):
                    if elif_prefix:
                        new_line = re.sub(r'#(\s*)elif(\s+)0', r'#\1elif\g<2>1', new_line)
                    else:
                        new_line = re.sub(r'#(\s*)if(\s+)0', r'#\1if\g<2>1', new_line)
                    changed = True
            else:
                if (current_literal_expression == 1):
                    if elif_prefix:
                        new_line = re.sub(r'#(\s*)elif(\s+)1', r'#\1elif\g<2>0', new_line)
                    else:
                        new_line = re.sub(r'#(\s*)if(\s+)1', r'#\1if\g<2>0', new_line)
                    changed = True
    return new_line, changed, is_dynamic_macro_replacement

def handle_fake_macro_replacement(line, file_path, line_num):
    new_line = line
    changed = False
    match = re.search(r'^\s*#\s*(el)?if\s+(\d)\s*//\s*(!?)(\w[\w\d_]+)', new_line)
    # Search the dictionary of user-defined macros that are associated with a version and comparison
    if match:
        elif_prefix = match.group(1)
        current_literal_expression = int(match.group(2))
        is_negated = match.group(3) == '!'
        macro_text = match.group(4)
        replacement_info = PrebuildConfig.MacroReplacements.get(macro_text)
        if replacement_info != None:
            if type(replacement_info) == str:
                if replacement_info.isdigit():
                    PrebuildConfig.MacroReplacements[macro_text] = {"EvaluatedTo": bool(int(replacement_info))}
                else:
                    parse_prebuild_header_line("#define " + macro_text + " " + replacement_info, file_path, line_num)
            elif type(replacement_info) == bool:
                PrebuildConfig.MacroReplacements[macro_text] = {"EvaluatedTo": replacement_info}
            elif type(replacement_info) == int:
                PrebuildConfig.MacroReplacements[macro_text] = {"EvaluatedTo": bool(replacement_info)}
            else:
                replacement_value = type(replacement_info) == dict and replacement_info.get("Value") or replacement_info
                if replacement_value != None:
                    if type(replacement_value) == str:
                        if replacement_value.isdigit() and int(replacement_value) != 0:
                            PrebuildConfig.MacroReplacements[macro_text] = {"EvaluatedTo": True}
                        else:
                            parse_prebuild_header_line("#define " + macro_text + " " + replacement_value, file_path, line_num)
                    elif type(replacement_value) == int and int(replacement_value) != 0:
                        PrebuildConfig.MacroReplacements[macro_text] = {"EvaluatedTo": True}
        replacement_info = PrebuildConfig.MacroReplacements.get(macro_text)
        match_files = type(replacement_info) == dict and replacement_info.get('MatchFiles') or PrebuildConfig.DefaultMacroReplacementFiles
        should_replace = replacement_info != None
        if (should_replace and match_files and len(match_files) > 0):
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
                    print_error_and_exit("Macro Replacement " + macro_text + " is missing 'Version' value!", file_path, line_num)
                compare_type = replacement_info.get('Compare')
                if not compare_type:
                    print_error_and_exit("Macro Replacement " + macro_text + " is missing 'Compare' value!", file_path, line_num)
                cached_comparison = do_comparison(compare_version, compare_type)
                replacement_info["EvaluatedTo"] = cached_comparison
            if (cached_comparison != is_negated):
                if (current_literal_expression == 0):
                    if elif_prefix:
                        new_line = re.sub(r'#(\s*)elif(\s+)0', r'#\1elif\g<2>1', new_line)
                    else:
                        new_line = re.sub(r'#(\s*)if(\s+)0', r'#\1if\g<2>1', new_line)
                    changed = True
            else:
                if (current_literal_expression == 1):
                    if elif_prefix:
                        new_line = re.sub(r'#(\s*)elif(\s+)1', r'#\1elif\g<2>0', new_line)
                    else:
                        new_line = re.sub(r'#(\s*)if(\s+)1', r'#\1if\g<2>0', new_line)
                    changed = True
        if not replacement_info:
            print("Warning: Failed to find Macro Replacement Info for " + macro_text + " " + file_path + ":" + str(line_num))
    return new_line, changed

def replace_line_in_file(file_path, line_num, line, was_prev_line_uproperty):
    changed = False
    new_line = line
    is_dynamic_macro_replacement = False
    try:
        # TObjectPtr replacement
        if PrebuildConfig.AllowObjectPtrReplacements:
            [new_line, obj_ptr_changed] = handle_object_ptr_replacement(new_line, file_path, line_num, was_prev_line_uproperty)
            changed = changed or obj_ptr_changed
        # Fake macro replacement (UE_VERSION_* form)
        if PrebuildConfig.AllowDynamicVersionMacroReplacements:
            [new_line, dyn_macro_changed, is_dynamic_macro_replacement] = handle_dynamic_fake_macro_replacement(new_line, file_path, line_num)
            changed = changed or dyn_macro_changed
        # Fake macro replacement (user-defined form)
        if not is_dynamic_macro_replacement:
            [new_line, fake_macro_changed] = handle_fake_macro_replacement(new_line, file_path, line_num)
            changed = changed or fake_macro_changed
        if (changed):
            print(file_path + ":" + str(line_num) + "\nChanged:\n  " + line + "To:\n  " + new_line)
    except Exception as e:
        print_error_and_exit("Exception while processing line `" + line + "`", file_path, line_num, e)
    return new_line, changed

def replace_in_file(file_path):
    input_file = io.open(file_path, 'r', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
    output_file = io.open(file_path + ".new", 'w', encoding=PrebuildConfig.SourceFileEncoding, errors=PrebuildConfig.EncodingErrorHandling)
    any_replaced = False
    line_num = 0
    was_prev_line_uproperty = False
    for line in input_file:
        line_num += 1
        new_line, changed = replace_line_in_file(file_path, line_num, line, was_prev_line_uproperty)
        was_prev_line_uproperty = re.match(r'\s*UPROPERTY\s*\(.*', new_line) != None
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

PluginName = os.path.basename(PluginDir)
for path in PrebuildConfig.CustomPrebuildHeaders:
    path = path.replace("{PluginName}", PluginName)
    parse_prebuild_header(path)

for dir in PrebuildConfig.ProcessDirs:
    dir = dir.replace("{PluginName}", PluginName)
    do_replacements_in_directory_recursive(os.path.join(PluginDir, dir))