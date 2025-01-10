# Modify this file to suit your project needs
from PrebuildConst import *

# List of directories to recursively perform text replacements in
# Assumed to be relative to the PluginDir environment variable unless fully-qualified
# That is, "Source" will be converted to "<PluginDir>/Source", while "C:/Exact/Path" will stay as "C:/Exact/Path"
# Being more specific here will improve your prebuild speed if you have a lot of source files
ProcessDirs = [
    "Source"
]

# Optional list of header file paths to auto-generate `MacroReplacements` for.
# It will only consider simple #define directives that use the UE_VERSION_* macros from VersionMacros.h
# It does not handle the UE_VERSION_WITHIN macro.
# It does not actually compile the header file, so other includes and arithmatic/logical operators will be ignored.
CustomPrebuildHeaders = [
    # Replace this with your own prebuild header file (or remove it if you don't plan to use one)
    "Source/VersionMacros/Public/Prebuild.h"
]

# Whether to interpret macro replacements of the form `#if <0 or 1> // UE_VERSION_*(major,minor)`
# It does not handle the UE_VERSION_WITHIN macro.
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
}

# Passed to io.open() as the encoding option
# Modify this value to match the encoding your project uses if you run into Python errors that mention decoding
# See: https://docs.python.org/2.7/library/io.html?highlight=encoding#io.open
SourceFileEncoding = None # 'utf-8', 'utf-16', 'iso-8859-1', etc.

# Passed to io.open() as the errors option
# It's usually safe to set this to 'ignore' but you should use version control to see whether anything important changed
# See: https://docs.python.org/2.7/library/io.html?highlight=errors#io.open
EncodingErrorHandling = None # 'strict', 'ignore', 'replace', etc.
