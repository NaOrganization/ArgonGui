# ⚡ ArgonGui Update Log

> *A lightweight, cross-platform, untrammeled GUI framework that doesn't ask for much—just your heart.* ❤️

### v1.1.1 (2025/5/9)

#### [Improved]

1. [Context] Improved memory access safety by modifying interfaces destructions and no longer needs to destroy interfaces manually.
1. [Graphic] Add unawake children queue in to ArGraphicElement in order to avoid iterators error occurred during the children traversal.
1. [Architecture Design] Mixed ArTextureManager into ArRenderManager.

#### [Fixed]

1. [Context] Fixed glyph parser interface that was forgotten to be destroyed when context shutdown.
2. [Input] Fixed mouse clicked action logic.
3. [Render] Fixed burrs during rendering text.
4. [Example] Fixed unable to unload module problem in Dll examples.
5. [Code Format] Delete useless functions like non-params OnDestroy.
6. [Graphic] Fixed the issue that the global focus is not unique

#### [News]

1. [Render] Add ArImage for convenient using image texture in rendering image
2. [Template] Add new flags operators
3. [Graphic] Add box model design into ArGraphicElement.
4. [Graphic] Add basic components.
   - ArDroppableComp allows the element to be dragged and dropped.
   - ArFlexItemComp is used to set the flex properties of the element.
   - ArFlexLayoutComp is used to set the layout rules for the element.
5. [Graphic] Add new callbacks in ArGraphicElement.
6. [Backend] Add DirectX 9 support.
7. [Example] Add Dll_Dx9_Win32.proj and Exe_Dx9_Win32.proj

---

**© 2025 by [NaOrganization](https://github.com/NaOrganization) & All ArgonGui Contributors **
