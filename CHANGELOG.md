# 更新记录

## 1.17-zh — 2026-11-18

基于上游 Bruce 1.17 重新汉化。

- 同步上游 1.17：新增 Wire 工具、SDR、WiFite、双频分析、逻辑分析仪等功能。
- 移植中文语言层与内嵌字库，修复中英混排、按像素宽度截断/换行。
- 中文颜色跟随原版主题（priColor / secColor / 选中反色）。
- 翻译表扩充到 412 条，新增 1.17 菜单条目：I2C/SPI 总线、JTAG 扫描、逻辑分析仪、WiFite、SDR 等。
- 上游已自带 FastLED 3.10.3 固定和统一工具链 patch.py，移除本地重复补丁。
- 新增 GitHub Actions 子模块检出与 Cardputer 中文构建。
## 1.16.1-zh — 2026-10-30

首个可发布版本。

- 新增简体中文语言层：在 `tft_logger` 中拦截文字绘制，支持中英混排、按像素宽度截断与换行。
- 内置 12px / 16px 两级中文点阵字库（GB2312 全集，7445 字），来源于 Noto Sans SC。
- 中文使用原版主题的 `priColor` / `secColor` / 选中反色，不再硬编码颜色。
- 修复中英混排时 `drawChar` 参数顺序错误导致的英文/数字乱码。
- 中文点阵改为逐像素绘制，避免不同 TFT_eSPI 配置下的字节序问题。
- 翻译表扩充到 396 条，覆盖主菜单、设置、WiFi、BLE、RFID、红外、文件管理等常用界面。
- 修正术语：`BLE Spam` → `BLE 轰炸`，`BLE Suite` → `BLE 工具箱`；保留 WiFi、NFC、RF、FM、BLE、RFID、GPS、USB 等常用英文。
- 固定 FastLED 3.10.3，修复 `FP` 宏冲突；`patch.py` 适配 pioarduino 55.x 的统一工具链。
- 新增 GitHub Actions 自动构建工作流与预编译固件。
