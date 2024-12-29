# Modify this file to suit your project needs
from re import split

# List of directories to recursively perform text replacements in
# Assumed to be relative to the PluginDir environment variable unless fully-qualified
# That is, "Source" will be converted to "<PluginDir>/Source", while "C:/Exact/Path" will stay as "C:/Exact/Path"
# Being more specific here will improve your prebuild speed if you have a lot of source files
ProcessDirs = [
    "Source"
]

# Custom "fake" macro replacements
# Use this to overcome preprocessor limitations in UnrealHeaderTool
# For example, if you wanted to wrap a UPROPERTY in a preprocessor #if/#endif block
MacroReplacements = {
    # Example: Enable TEST_MACRO for UE Versions <= 5.0
    # - In UE 5.0 or greater, this will search and replace "#if 1 // TEST_MACRO" with "#if 0 // TEST_MACRO"
    # - In UE versions lower than 5.0, this will search and replace "#if 0 // TEST_MACRO" with "#if 1 // TEST_MACRO"
    "TEST_MACRO": {
        "Version": "5.0", # Version of Unreal to compare against
        "Compare": '<=', # Comparison to make (in this case it will evaluate EngineVersion <= 5.0)
        "MatchFiles": [r'.*/VersionMacros.cpp'] # (Optional) Files to restrict replacements to
    },
}

#
# Below is an example of programmatically generating replacement info for a list of Unreal versions.
# Feel free to delete this.
#

# Whether to do text replacements for lines of the form "#if <1 or 0> // UE_<MajorVersion>_<MinorVersion>_OR_LATER"
AllowVersionOrLaterReplacements = True
# Whether to do text replacements for lines of the form "#if <1 or 0> // UE_<MajorVersion>_<MinorVersion>_OR_EARLIER"
AllowVersionOrEarlierReplacements = True
# Default file patterns to perform replacements in
DefaultMatchFiles = [r'.*\.cpp', r'.*\.h', r'.*\.hpp', r'.*\.c', r'.*\.inl']
# Generate fake macro text replacement configurations for UE_*_OR_LATER & UE_*_OR_EARLIER macros
if AllowVersionOrLaterReplacements or AllowVersionOrEarlierReplacements:
    VersionsList = ["4.17", "4.18", "4.19", "4.20", "4.21", "4.22", "4.23", "4.24", "4.25", "4.26", "4.27",
                    "5.0", "5.1", "5.2", "5.3", "5.4", "5.5"]
    for version in VersionsList:
        [major, minor] = split(r'\.', version, 1)
        if AllowVersionOrLaterReplacements:
            MacroReplacements["UE_" + major + "_" + minor + "_OR_LATER"] = {
                "Version": version,
                "Compare": '>=',
                "MatchFiles": DefaultMatchFiles
            }
        if AllowVersionOrEarlierReplacements:
            MacroReplacements["UE_" + major + "_" + minor + "_OR_EARLIER"] = {
                "Version": version,
                "Compare": '<=',
                "MatchFiles": DefaultMatchFiles
            }
