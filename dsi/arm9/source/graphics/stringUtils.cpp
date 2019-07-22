#include "stringUtils.hpp"
#include <algorithm>
#include <map>
#include <queue>
#include <vector>

std::u16string StringUtils::UTF8toUTF16(const std::string& src)
{
    std::u16string ret;
    for (size_t i = 0; i < src.size(); i++)
    {
        u16 codepoint = 0xFFFD;
        int iMod      = 0;
        if (src[i] & 0x80 && src[i] & 0x40 && src[i] & 0x20 && !(src[i] & 0x10) && i + 2 < src.size())
        {
            codepoint = src[i] & 0x0F;
            codepoint = codepoint << 6 | (src[i + 1] & 0x3F);
            codepoint = codepoint << 6 | (src[i + 2] & 0x3F);
            iMod      = 2;
        }
        else if (src[i] & 0x80 && src[i] & 0x40 && !(src[i] & 0x20) && i + 1 < src.size())
        {
            codepoint = src[i] & 0x1F;
            codepoint = codepoint << 6 | (src[i + 1] & 0x3F);
            iMod      = 1;
        }
        else if (!(src[i] & 0x80))
        {
            codepoint = src[i];
        }

        ret.push_back((char16_t)codepoint);
        i += iMod;
    }
    return ret;
}

static std::string utf16DataToUtf8(const char16_t* data, size_t size, char16_t delim = 0)
{
    std::string ret;
    char addChar[4] = {0};
    for (size_t i = 0; i < size; i++)
    {
        if (data[i] == delim)
        {
            return ret;
        }
        else if (data[i] < 0x0080)
        {
            addChar[0] = data[i];
            addChar[1] = '\0';
        }
        else if (data[i] < 0x0800)
        {
            addChar[0] = 0xC0 | ((data[i] >> 6) & 0x1F);
            addChar[1] = 0x80 | (data[i] & 0x3F);
            addChar[2] = '\0';
        }
        else
        {
            addChar[0] = 0xE0 | ((data[i] >> 12) & 0x0F);
            addChar[1] = 0x80 | ((data[i] >> 6) & 0x3F);
            addChar[2] = 0x80 | (data[i] & 0x3F);
            addChar[3] = '\0';
        }
        ret.append(addChar);
    }
    return ret;
}

std::string StringUtils::UTF16toUTF8(const std::u16string& src)
{
    return utf16DataToUtf8(src.data(), src.size());
}

std::string StringUtils::getString(const u8* data, int ofs, int len, char16_t term)
{
    return utf16DataToUtf8((char16_t*)(data + ofs), len, term);
}