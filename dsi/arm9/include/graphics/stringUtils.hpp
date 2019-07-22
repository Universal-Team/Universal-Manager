#include <nds/ndstypes.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <optional>
#include <stdarg.h>
#include <string.h>
#include <string>

namespace StringUtils
{
    std::u16string UTF8toUTF16(const std::string& src);
    std::string UTF16toUTF8(const std::u16string& src);
    std::string getString(const u8* data, int ofs, int len, char16_t term = 0);
}