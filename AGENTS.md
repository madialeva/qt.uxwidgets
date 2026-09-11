# AGENTS.md -- UxWidgets

Guidance for AI agents and contributors working in this repository. It records
project context, design decisions, and non-obvious technical details.

## Language Policy

- Source code, public API identifiers, variables, comments, CMake comments, and
  demo UI text MUST be written in English.
- OpenSpec change artifacts and the agent's conversation with the user remain in
  Spanish.
- Keep public identifiers consistent with the existing English API, such as
  `UxField`, `selectionRequested`, `darkFocusColor`, and `required`.

## Overview

This repository develops **UxWidgets**, a standalone C++17 library of reusable
Qt Widgets data-entry controls. It is a dependency for Qt applications and must
not contain application-specific business logic.

**Two-layer architecture:**

- **Field layer** (`UxField : QLineEdit`) with `UxTextField`, `UxNumberField`,
  and `UxDateField`. It owns validation, icons, focus highlighting, and required
  state.
- **Composite layer** (`UxInput : QWidget`) combines a label, field, and layout;
  `UxTextInput`, `UxNumberInput`, and `UxDateInput` create the appropriate field.

The public catalog, properties, and usage examples are in `README.md`.

## OpenSpec Workflow

The repository uses OpenSpec (`openspec/`): propose, implement, then archive each
functional change. Create `proposal.md`, `design.md`, `tasks.md`, and the required
spec deltas first, then STOP. Do not implement until the user has reviewed and
approved the proposal and design. After implementation, leave the project compiling
for the user's verification. Archive only after user confirmation.

Baseline specs live in `openspec/specs/`; archived changes are in
`openspec/changes/archive/`.

## Project Layout

```
.
├── CMakeLists.txt          (static UxWidgets library and UxWidgets::UxWidgets alias;
│                            UXWIDGETS_BUILD_DEMO is ON by default)
├── README.md               (control catalog, properties, consumption, and build guide)
├── include/UxWidgets/      (public headers, included as <UxWidgets/UxInput.h>)
├── src/                    (implementations)
└── demo/                   (UxWidgetsDemo exercises the three controls; on Windows,
                             POST_BUILD uses windeployqt for self-contained deployment)
```

## Control Design Conventions

- **UI:** Qt **Widgets**, not QML. C++17.
- **Compiler:** GCC with C++17. On Windows, use the **Qt MinGW** kit for consistency
  with target applications and `windeployqt`; on Linux, use the distribution's GCC.
  Build with **CMake + Ninja**.
- **Two layers:** specialize by type in the `QLineEdit`-based field layer; keep the
  composite `QWidget` layer mostly generic.
- **Date field:** use `QLineEdit`, not `QDateEdit`, to support empty optional dates.
  Use `dd/MM/yyyy`; its calendar icon opens a popup `QCalendarWidget`.
- **Integrated action icon:** use `QLineEdit::addAction(TrailingPosition)`, not a
  sibling widget. Text and number fields expose an opt-in selector button with an
  ellipsis icon that emits `selectionRequested()`; consumers open their own dialog.
- **Properties:** declare every configurable setting with `Q_PROPERTY` and public
  enums with `Q_ENUM`, keeping a future Qt Designer plugin possible without redesign.
  Do not add dependencies when Qt already provides the needed mechanism.

## Non-obvious Technical Notes

- `QLineEdit` has no `insertFromMimeData` (that belongs to `QTextEdit` and
  `QPlainTextEdit`). Its validator and `maxLength` already apply to pasted text.
  Uppercase conversion belongs in a `QValidator::validate()` implementation, which
  applies to typing and pasting without moving the cursor.
- Focus highlighting uses `QPalette::Base`, with separate light and dark theme
  colors (`lightFocusColor` and `darkFocusColor`). Detect dark themes from
  `QPalette::Text` lightness (`> 128`). React to `ApplicationPaletteChange` and
  `PaletteChange` in `changeEvent` with a reentrancy guard because `setPalette`
  triggers `PaletteChange`. Priority is focus, then required-and-empty (bisque),
  then normal. A consuming application's global style sheet can override palettes.
- Field variants that override focus handling, such as `UxNumberField` for the
  thousands separator, must chain to `UxField::focus*Event`, not `QLineEdit::`, to
  preserve focus highlighting.
- On Windows, `demo/CMakeLists.txt` runs `windeployqt --compiler-runtime` and copies
  the MinGW runtimes (`libgcc_s_seh-1`, `libstdc++-6`, `libwinpthread-1`) next to the
  executable. Build directories are ignored by Git and should be regenerated.

## Supported Development Environments

Install **Qt 6** with Core, Gui, and Widgets; a C++17-capable compiler; CMake 3.21
or later; and Ninja. When Qt is not in a location CMake discovers automatically,
provide its prefix with `CMAKE_PREFIX_PATH`.

### Windows

Use a Qt 6 MinGW kit and make the Qt, MinGW, CMake, and Ninja binaries available in
`PATH`. `windeployqt --compiler-runtime` deploys Qt DLLs and MinGW runtimes beside
the demo executable.

```powershell
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=<path-to-Qt>
cmake --build build
.\build\demo\UxWidgetsDemo.exe
```

### Linux

Install Qt 6 development packages, GCC/G++ (or another C++17 compiler), CMake, and
Ninja from the distribution. On DNF-based systems:

```bash
sudo dnf install qt6-qtbase-devel qt6-qttools cmake ninja-build gcc-c++
```

Build from the repository root:

```bash
cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux
./build-linux/demo/UxWidgetsDemo
```

Distribution-provided Qt is detected automatically, so do not set
`CMAKE_PREFIX_PATH` unless using an external Qt installation. The project does not
package the Linux demo; that belongs to the consuming application's distribution
format.

## Status and Ideas

Completed:

- Two layers and three variants: text, number, and date.
- Text has `maxLength` and forced uppercase. Number has `integerDigits`,
  `decimalDigits`, and `thousandsSeparator`, and is right-aligned. Date uses
  `dd/MM/yyyy`, tolerates empty values, and offers a popup calendar.
- An ellipsis selector button emits `selectionRequested()`, labels can be moved,
  required empty fields use bisque feedback, and focus highlighting adapts to themes.
- Configurable settings use `Q_PROPERTY`; the Windows demo deploys with
  `windeployqt`.

Future ideas:

- A **Qt Designer** plugin exposing controls and their `Q_PROPERTY` values.
- A theme-aware required-field warning; bisque is currently a fixed light color.
- Additional controls such as combo boxes and check boxes, following the two-layer
  pattern.
- An installable CMake package (`find_package(UxWidgets)`) in addition to
  `add_subdirectory`.
