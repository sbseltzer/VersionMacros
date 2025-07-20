# Modify PrebuildConfig.py to suit your project needs

# Import constants
from PrebuildConst import *

# List of directories to recursively perform text replacements in
# Assumed to be relative to the PluginDir environment variable unless fully-qualified
# That is, "Source" will be converted to "<PluginDir>/Source", while "C:/Exact/Path" will stay as "C:/Exact/Path"
# Being more specific here will improve your prebuild speed if you have a lot of source files
ProcessDirs = [
    "Source"
]

# Optional list of header file paths to auto-generate `MacroReplacements` for.
# It supports simple #define directives that use the UE_VERSION_* macros from VersionMacros.h
# It also supports constant (1 or 0) values.
# It does not actually compile the header file, so other includes and arithmatic/logical operators will be ignored.
CustomPrebuildHeaders = [
    "Source/{PluginName}/Public/Prebuild.h"
]

# If you've modified the UE_VERSION_* macros to have a custom prefix, set it here
# This may be a good idea if you expect gameplay code to include header files from your plugin that use these macros
# MacroPrefixName = ""

# If you've modified the common part of the version macro names to something other than "UE_VERSION_", set it here
# You might wish to do this if you have a particular naming convention you want to follow, or to make them more concise
# MacroCommonName = "UE_VERSION_"

# If you've modified any of the UE_VERSION_* macros to have a different suffix name, set them here
# MacroSuffixNames[ABOVE] = "ABOVE"
# MacroSuffixNames[BELOW] = "BELOW"
# MacroSuffixNames[EQUAL] = "EQUAL"
# MacroSuffixNames[MINIMUM] = "MINIMUM"
# MacroSuffixNames[MAXIMUM] = "MAXIMUM"
# MacroSuffixNames[WITHIN] = "WITHIN"

# Whether to interpret macro replacements of the form `#if <0 or 1> // UE_VERSION_*(major,minor)`
AllowDynamicVersionMacroReplacements = True

# Whether to replace TObjectPtr<T> with T* on UE4 builds (includes annotation for reversibility)
AllowObjectPtrReplacements = True

# Default file patterns to perform replacements in
MatchHeaderFiles = [r'.*\.h', r'.*\.hpp']
MatchImplementationFiles = [r'.*\.cpp', r'.*\.inl', r'.*\.c']
MatchAllSourceFiles = MatchHeaderFiles + MatchImplementationFiles

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
        "MatchFiles": [r'.*/VersionMacros.cpp', r'.*/Test.h'] # (Optional) Files to restrict replacements to
    },
    # Here are more concise ways you could specify a fake macro for >= 5.0
    "TEST_MACRO_CONCISE": "UE_VERSION_MINIMUM(5,0)",
    "TEST_MACRO_CONCISE_WITH_FILES": {
        "Value": "UE_VERSION_MINIMUM(5,0)",
        "MatchFiles": MatchAllSourceFiles
    },
    # Here are ways you can specify a constant value
    "TEST_MACRO_CONST_STRING": "1",
    "TEST_MACRO_CONST_INT": 1,
    "TEST_MACRO_CONST_BOOL": True,
}
DefaultMacroReplacementFiles = MatchHeaderFiles

# Passed to io.open() as the encoding option
# Modify this value to match the encoding your project uses if you run into Python errors that mention decoding
# See: https://docs.python.org/2.7/library/io.html?highlight=encoding#io.open
SourceFileEncoding = None # 'utf-8', 'utf-16', 'iso-8859-1', etc.

# Passed to io.open() as the errors option
# It's usually safe to set this to 'ignore' but you should use version control to see whether anything important changed
# See: https://docs.python.org/2.7/library/io.html?highlight=errors#io.open
EncodingErrorHandling = None # 'strict', 'ignore', 'replace', etc.
