#ifndef BRUCE_LANG_ZH_FONT_H
#define BRUCE_LANG_ZH_FONT_H

#include <Arduino.h>
#include <stdint.h>
#include <vector>

struct ZhEntry {
    const char *en;
    const char *zh;
};

// Compiled in only for the Chinese build (-DBRUCE_LANG_ZH).
#if defined(BRUCE_LANG_ZH)
extern const uint16_t ZH_FONT_COUNT;
extern const uint16_t zh_unicode[];
extern const uint8_t zh_font12[];
extern const uint8_t zh_font16[];
extern const uint16_t ZH_STRINGS_COUNT;
extern const ZhEntry ZH_STRINGS[];

// Returns a 1bpp glyph bitmap (rows of `h`, `w` bits per row, MSB first),
// or nullptr when the codepoint is not part of the embedded font.
const uint8_t *zhFontGlyph(uint16_t cp, uint8_t textSize, uint8_t &w, uint8_t &h);
#endif

// Decode one UTF-8 codepoint starting at `i` and advance `i`.
uint16_t bruceNextCodepoint(const String &s, size_t &i);

// True when the string contains CJK / full-width codepoints.
bool bruceTextIsWide(const String &s);
bool bruceTextIsWide(const char *s);

// nullptr when there is no translation for this exact string.
const char *bruceLookup(const char *s);

// Width in pixels for the current font size (ASCII 6*size, CJK 12 or 16).
int16_t bruceTextWidth(const String &s, uint8_t textSize);
int16_t bruceTextWidth(const char *s, uint8_t textSize);

// Replace a fully matching UI string with its Chinese translation.
String bruceTranslate(const String &s);

// Truncate on a codepoint boundary and append ".." when it does not fit.
String bruceFitText(const String &s, int16_t maxWidth, uint8_t textSize);

// Greedy width based wrapping, used by the status stripe/dialog helpers.
void bruceWrapText(const String &s, int16_t maxWidth, uint8_t textSize, std::vector<String> &lines);

#endif
