#include "zh_font.h"
#include <cstring>

namespace {

inline bool isWideCodepoint(uint16_t cp) {
    return (cp >= 0x2E80 && cp <= 0x9FFF) || (cp >= 0x3000 && cp <= 0x303F) ||
           (cp >= 0xF900 && cp <= 0xFAFF) || (cp >= 0xFF00 && cp <= 0xFFEF);
}

} // namespace

uint16_t bruceNextCodepoint(const String &s, size_t &i) {
    if (i >= s.length()) return 0;
    uint8_t c = (uint8_t)s[i++];
    if (c < 0x80) return c;
    uint8_t need = 0;
    uint16_t cp = 0;
    if ((c & 0xE0) == 0xC0) {
        need = 1;
        cp = c & 0x1F;
    } else if ((c & 0xF0) == 0xE0) {
        need = 2;
        cp = c & 0x0F;
    } else if ((c & 0xF8) == 0xF0) {
        need = 3;
        cp = c & 0x07;
    } else {
        return '?';
    }
    for (uint8_t n = 0; n < need; ++n) {
        if (i >= s.length()) return '?';
        uint8_t t = (uint8_t)s[i++];
        if ((t & 0xC0) != 0x80) return '?';
        cp = (cp << 6) | (t & 0x3F);
    }
    return cp;
}

bool bruceTextIsWide(const String &s) {
    return bruceTextIsWide(s.c_str());
}

bool bruceTextIsWide(const char *s) {
    if (!s) return false;
    while (*s) {
        uint8_t c = (uint8_t)*s++;
        uint16_t cp = c;
        if (c >= 0x80) {
            uint8_t need = 0;
            if ((c & 0xE0) == 0xC0) {
                need = 1;
                cp = c & 0x1F;
            } else if ((c & 0xF0) == 0xE0) {
                need = 2;
                cp = c & 0x0F;
            } else if ((c & 0xF8) == 0xF0) {
                need = 3;
                cp = c & 0x07;
            } else {
                continue;
            }
            for (uint8_t n = 0; n < need; ++n) {
                if (!*s) return false;
                cp = (cp << 6) | ((uint8_t)*s++ & 0x3F);
            }
        }
        if (isWideCodepoint(cp)) return true;
    }
    return false;
}

#if defined(BRUCE_LANG_ZH)

const uint8_t *zhFontGlyph(uint16_t cp, uint8_t textSize, uint8_t &w, uint8_t &h) {
    int lo = 0, hi = (int)ZH_FONT_COUNT - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        uint16_t v = zh_unicode[mid];
        if (v == cp) {
            if (textSize >= 2) {
                w = h = 16;
                return zh_font16 + (size_t)mid * 32;
            }
            w = h = 12;
            return zh_font12 + (size_t)mid * 24;
        }
        if (v < cp) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}

String bruceTranslate(const String &s) {
    if (s.length() == 0) return s;

    // Keep trailing spaces / newlines (println, padded labels) outside the lookup.
    int end = s.length();
    while (end > 0 && (s[end - 1] == '\n' || s[end - 1] == '\r' || s[end - 1] == ' ')) end--;
    if (end == 0) return s;

    String key = s.substring(0, end);
    const char *zh = bruceLookup(key.c_str());
    if (!zh) return s;

    String out(zh);
    if (end < (int)s.length()) out += s.substring(end);
    return out;
}

const char *bruceLookup(const char *s) {
    if (!s || ZH_STRINGS_COUNT == 0) return nullptr;
    int lo = 0, hi = (int)ZH_STRINGS_COUNT - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int cmp = strcmp(s, ZH_STRINGS[mid].en);
        if (cmp == 0) return ZH_STRINGS[mid].zh;
        if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }

    // Menu code often pads labels with trailing spaces; retry without them.
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t')) len--;
    if (len > 0 && len < (int)strlen(s) && len < 96) {
        char buf[96];
        memcpy(buf, s, len);
        buf[len] = '\0';
        return bruceLookup(buf);
    }
    return nullptr;
}

#else

String bruceTranslate(const String &s) { return s; }
const char *bruceLookup(const char *s) { return nullptr; }

#endif

int16_t bruceTextWidth(const String &s, uint8_t textSize) {
    if (textSize == 0) textSize = 1;
    String text = bruceTranslate(s);
    int32_t width = 0;
    size_t i = 0;
    while (i < text.length()) {
        uint16_t cp = bruceNextCodepoint(text, i);
        if (cp < 0x80) {
            width += 6 * textSize;
        } else {
#if defined(BRUCE_LANG_ZH)
            uint8_t w = 0, h = 0;
            if (zhFontGlyph(cp, textSize, w, h)) width += w;
            else width += 6 * textSize;
#else
            width += 6 * textSize;
#endif
        }
    }
    return (int16_t)width;
}

int16_t bruceTextWidth(const char *s, uint8_t textSize) {
    if (!s) return 0;
    return bruceTextWidth(String(s), textSize);
}

String bruceFitText(const String &s, int16_t maxWidth, uint8_t textSize) {
    if (textSize == 0) textSize = 1;
    if (maxWidth <= 0) return String();
    String text = bruceTranslate(s);
    if (bruceTextWidth(text, textSize) <= maxWidth) return text;

    int16_t dotWidth = 2 * 6 * textSize;
    if (dotWidth + 6 * textSize > maxWidth) dotWidth = 0;
    int16_t limit = maxWidth - dotWidth;

    String out;
    int16_t width = 0;
    size_t i = 0;
    while (i < text.length()) {
        size_t start = i;
        uint16_t cp = bruceNextCodepoint(text, i);
        int16_t cw;
        if (cp < 0x80) {
            cw = 6 * textSize;
        } else {
#if defined(BRUCE_LANG_ZH)
            uint8_t w = 0, h = 0;
            cw = zhFontGlyph(cp, textSize, w, h) ? w : 6 * textSize;
#else
            cw = 6 * textSize;
#endif
        }
        if (width + cw > limit) break;
        out += text.substring(start, i);
        width += cw;
    }
    if (dotWidth) out += "..";
    return out;
}

void bruceWrapText(const String &s, int16_t maxWidth, uint8_t textSize, std::vector<String> &lines) {
    lines.clear();
    if (maxWidth <= 0) return;
    String text = bruceTranslate(s);
    if (text.length() == 0) {
        lines.push_back(String());
        return;
    }

    String line;
    int16_t width = 0;
    size_t i = 0;
    while (i < text.length()) {
        size_t start = i;
        uint16_t cp = bruceNextCodepoint(text, i);
        if (cp == '\n') {
            lines.push_back(line);
            line = "";
            width = 0;
            continue;
        }
        int16_t cw;
        if (cp < 0x80) {
            cw = 6 * textSize;
        } else {
#if defined(BRUCE_LANG_ZH)
            uint8_t w = 0, h = 0;
            cw = zhFontGlyph(cp, textSize, w, h) ? w : 6 * textSize;
#else
            cw = 6 * textSize;
#endif
        }
        if (width + cw > maxWidth && width > 0) {
            lines.push_back(line);
            line = "";
            width = 0;
        }
        line += text.substring(start, i);
        width += cw;
    }
    if (line.length() || lines.empty()) lines.push_back(line);
}
