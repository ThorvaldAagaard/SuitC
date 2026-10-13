# Equivalent of: sed '/^[cCdils]/s/^/extern /' global.h > extern.h
file(READ "${IN}" content)
string(REGEX REPLACE "\n([cCdils])" "\nextern \\1" content "\n${content}")
string(SUBSTRING "${content}" 1 -1 content)
file(WRITE "${OUT}.tmp" "${content}")
file(COPY_FILE "${OUT}.tmp" "${OUT}" ONLY_IF_DIFFERENT)
file(REMOVE "${OUT}.tmp")
