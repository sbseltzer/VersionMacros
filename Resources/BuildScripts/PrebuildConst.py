# Operator IDs
ABOVE = 1
BELOW = 2
EQUAL = 3
MINIMUM = 4
MAXIMUM = 5
WITHIN = 6
# Version Macro Prefix
MacroPrefixName = ""
# Version Macro Common
MacroCommonName = "UE_VERSION_"
# Operator IDs to suffix names
MacroSuffixNames = [
    None,
    'ABOVE',
    'BELOW',
    'EQUAL',
    'MINIMUM',
    'MAXIMUM',
    'WITHIN'
]
# Operator Names/Symbols to ID
OperatorStringToID = {
    '>': ABOVE,
    '<': BELOW,
    '==': EQUAL,
    '>=': MINIMUM,
    '<=': MAXIMUM,
    '[]': WITHIN
}