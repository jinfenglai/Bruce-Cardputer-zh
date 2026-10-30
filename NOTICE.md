# NOTICE

This repository is a **Simplified Chinese localization** of the Bruce firmware
for the M5Stack Cardputer.

## Upstream

- Bruce firmware — https://github.com/BruceDevices/firmware
- Copyright held by BruceDevices and the Bruce contributors.
- Licensed under the GNU General Public License v3.0; see `LICENSE`.
- The original upstream source is included in this repository. All local
  modifications are released under the same GPL-3.0 terms.

## Chinese font

The embedded 12 px / 16 px bitmap fonts (`src/core/lang/zh_font12.cpp`,
`src/core/lang/zh_font16.cpp`, `src/core/lang/zh_font_unicode.cpp`) were
rasterized from **Noto Sans SC**:

- Noto Sans SC — https://fonts.google.com/noto/specimen/Noto+Sans+SC
- Copyright 2014-2021 Adobe (http://www.adobe.com/), with Reserved Font Name
  "Source".
- Licensed under the **SIL Open Font License, Version 1.1**.
- The full license text is included in `licenses/OFL-NotoSansSC.txt`.

The bitmap glyph data is a derived work of the font and is distributed under
the same OFL 1.1 terms. The font itself is **not** redistributed as a font
file; only rasterized glyph bitmaps are embedded in the firmware.

## Third-party libraries

All third-party libraries are fetched by PlatformIO according to
`platformio.ini` and keep their own licenses. See `THIRD_PARTY.md` and the
license files inside the respective library directories.

Notable build-time pin:

- **FastLED 3.10.3** — later 3.10.x releases use `FP` as a template parameter,
  which collides with Bruce's `FP` font-size macro. The dependency is pinned
  to 3.10.3 so the Cardputer build keeps compiling.

## Net80211 patch

`patch.py` weakens `ieee80211_raw_frame_sanity_check` in the ESP32-S3
`libnet80211.a` so Bruce's own implementation is used. This is the same patch
performed by upstream Bruce, adapted for the unified `toolchain-xtensa-esp-elf`
package shipped with pioarduino platform 55.x.
