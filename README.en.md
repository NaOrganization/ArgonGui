Here is the English version of your README for **ArgonGui**:

# ⚡ ArgonGui

> *A lightweight, cross-platform, untrammeled GUI framework that doesn't ask for much—just your heart.* ❤️

**ArgonGui** is a minimalist and highly flexible GUI framework designed for modern C++17 developers.

[![Stars](https://img.shields.io/github/stars/NaOrganization/ArgonGui?style=flat)](https://github.com/NaOrganization/ArgonGui/issues)
[![Forks](https://img.shields.io/github/forks/NaOrganization/ArgonGui?style=flat)](https://github.com/NaOrganization/ArgonGui/forks)
[![Issues](https://img.shields.io/github/issues/NaOrganization/ArgonGui?style=flat)](https://github.com/NaOrganization/ArgonGui/issues)
[![License](https://img.shields.io/github/license/NaOrganization/ArgonGui?style=flat)](./LICENSE)
[![Wiki](https://img.shields.io/badge/Wiki-Available-blue?style=flat&logo=github&link=https://github.com/NaOrganization/ArgonGui/wiki)](https://github.com/NaOrganization/ArgonGui/wiki)

## ✨ Features

- **Robust Layered Architecture** — Clean separation of context and subsystems ensures low coupling and high efficiency.
- **Graphic Element System** — Layered focus and hover management makes UI control straightforward.
- **Component-Free Philosophy** — You're in full control of your widgets. ArgonGui is your tool, not your boss.
- **Cross-Platform Support** — One unified interface across multiple platforms makes porting easy.

## 🌏 Demo

![](./Images/0B4A5F412A43EC9609683336DFF56C22.png)

## 📚 Dependencies

Some modules in ArgonGui rely on the following open-source libraries:

- [FreeType](https://freetype.org) — For font parsing and glyph generation (via `ArgonFreeTypeGlyphParser`)
- [fmt](https://github.com/fmtlib/fmt) — A high-performance formatting library (used in `Example\Exe_Dx11_Win32\Main`)

We recommend using  [`vcpkg`](https://github.com/microsoft/vcpkg) to install dependencies:

```
vcpkg install fmt:x64-windows-static
vcpkg install freetype:x64-windows-static
vcpkg install fmt:x86-windows-static
vcpkg install freetype:x86-windows-static
```

## 📦 Installation

Package manager integration (e.g., vcpkg/conan) is not yet available, but you can add ArgonGui directly as a submodule:

```
git submodule add https://github.com/NaOrganization/ArgonGui
```

**Build Requirements:**

- C++17 compatible compiler
- Supported platform: Windows

### ⭐ Star History

[![Star History Chart](https://api.star-history.com/svg?repos=NaOrganization/ArgonGui&type=Date)](https://www.star-history.com/#NaOrganization/ArgonGui&Date)

## 🔒 License

ArgonGui is licensed under the Apache License 2.0.

**© 2025 by [NaOrganization](https://github.com/NaOrganization) & All ArgonGui Contributors**