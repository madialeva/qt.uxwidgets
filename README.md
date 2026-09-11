<div align="center">

# UxWidgets

**Reusable Qt Widgets controls for data-entry forms.**

A configurable composite input control -- label + typed field + icon -- in a
single line of code, with validation, active-field highlighting, and properties
ready for Designer.

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?logo=cplusplus&logoColor=white)
![Qt 6](https://img.shields.io/badge/Qt-6-41CD52.svg?logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.21%2B-064F8C.svg?logo=cmake&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

</div>

<div align="center">

| Light theme | Dark theme |
|:---:|:---:|
| ![UxWidgets demo in light theme](assets/demo-claro.png) | ![UxWidgets demo in dark theme](assets/demo-oscuro.png) |

<sub>The focused field is highlighted in blue (light or dark depending on the theme); required empty fields are highlighted in bisque.</sub>

</div>

---

`UxWidgets` packages the "label + field + icon" pattern repeated throughout desktop
application forms. Instead of creating, aligning, and validating three widgets for
each value, place **one** control that already knows how to behave as text, a number,
or a date.

## ✨ Features

- 🧩 **Composite control** -- label, field, and icon as a single unit; the label can
  be placed to the left or above.
- 🔤 **Three typed variants** -- text (with maximum length and uppercase), number
  (integer/decimal digits and thousands separator), and date (`dd/MM/yyyy`).
- 📅 **Empty-tolerant date** -- built on `QLineEdit`, it accepts an empty field when
  not required (something `QDateEdit` does not naturally allow) and provides a popup
  calendar.
- 🔎 **Built-in selection button** -- an icon inside the field opens your selection
  dialog; you decide what to show and what to return.
- 🎯 **Active-field highlighting** -- the focused field tints its background with a
  **theme-aware** color (light/dark), updated when the theme changes at runtime.
- ⚠️ **Required-field warning** -- visual feedback when a mandatory field is empty.
- 🎛️ **Fully configurable with `Q_PROPERTY`** -- ready for a future Qt Designer
  plugin without redesigning the controls.
- 📦 **No dependencies** beyond Qt 6 and C++17.

## 🧱 Control Anatomy

```
   ┌──────────────────────────────────────────────┐
  │  UxInput  (a single control to place)        │
   │  ┌─────────┐ ┌──────────────────┐ ┌────────┐ │
  │  │  Label  │ │    text field    │ │   …    │ │
   │  └─────────┘ └──────────────────┘ └────────┘ │
  │    QLabel        UxField            selector │
   └──────────────────────────────────────────────┘
```

Two-layer architecture: controls inherit from standard Qt widgets.

```
              QLineEdit                              QWidget
                  │                                     │
            ┌─────┴─────┐                        ┌──────┴──────┐
            │  UxField  │  (common)              │   UxInput   │  (composite)
            └─────┬─────┘                        │ QLabel +    │
       ┌───────────┼───────────┐                  │ UxField* +  │
     UxTextField UxNumberField UxDateField           │ layout      │
                                                 └──────┬──────┘
                                           ┌────────────┼────────────┐
                                      UxTextInput  UxNumberInput  UxDateInput
```

- **Field layer** (`UxField : QLineEdit`): shared behavior (selection button,
  required state, focus highlighting) plus validation for each type.
- **Composite layer** (`UxInput : QWidget`): label + field + layout and property
  forwarding. The `Ux{Text,Number,Date}Input` convenience classes create the
  appropriate field type.

## 📋 Requirements

- **Qt 6** (Core, Gui, Widgets)
- **C++17**
- **CMake ≥ 3.21**
- **Ninja** (recomendado como generador)

Tested on Windows with Qt 6 and MinGW, and on Linux with Qt 6, GCC, CMake, and
Ninja. Self-contained demo deployment with `windeployqt` is Windows-specific.

On Fedora, RHEL, CentOS Stream, and Oracle Linux distributions, install the
development environment with:

```bash
sudo dnf install qt6-qtbase-devel qt6-qttools cmake ninja-build gcc-c++
```

## 🚀 Quick Start

Add the library to your CMake project and link its target:

```cmake
add_subdirectory(path/to/UxWidgets)
target_link_libraries(MyApp PRIVATE UxWidgets::UxWidgets)
```

```cpp
#include <UxWidgets/UxTextInput.h>
#include <UxWidgets/UxNumberInput.h>
#include <UxWidgets/UxDateInput.h>
```

When using it as a dependency, disable the demo:

```cmake
set(UXWIDGETS_BUILD_DEMO OFF)
add_subdirectory(path/to/UxWidgets)
```

## 💡 Examples

```cpp
// Uppercase text, maximum length, and a required field
auto *name = new UxTextInput("Name");
name->textField()->setUppercase(true);
name->setMaxLength(30);
name->setRequired(true);

// Amount: 6 integer digits, 2 decimal digits, thousands separator
auto *amount = new UxNumberInput("Amount");
amount->numberField()->setIntegerDigits(6);
amount->numberField()->setDecimalDigits(2);
amount->numberField()->setThousandsSeparator(true);

// Date with the label above (allows empty values)
auto *date = new UxDateInput("Date");
date->setLabelPosition(UxInput::Above);
```

## 🧭 Control Catalog

| Control | Inherits | Purpose |
|---|---|---|
| `UxTextField` | `UxField` | Free text. `maxLength` and `uppercase`. |
| `UxNumberField` | `UxField` | Numeric only. `integerDigits`, `decimalDigits`, `thousandsSeparator`; right-aligned. |
| `UxDateField` | `UxField` | `dd/MM/yyyy` date; allows empty values; popup calendar. |
| `UxInput` | `QWidget` | Composite label + field + icon. `labelText`, `labelPosition`. |
| `UxTextInput` · `UxNumberInput` · `UxDateInput` | `UxInput` | Convenience classes that create the corresponding field type. |

<details>
<summary><b>Properties (<code>Q_PROPERTY</code>)</b></summary>

All configurable properties are declared as `Q_PROPERTY` (and enums with `Q_ENUM`),
so that a future Qt Designer plugin can expose them.

| Class | Properties |
|---|---|
| `UxField` | `required`, `showSelector`, `icon`, `lightFocusColor`, `darkFocusColor` |
| `UxTextField` | `uppercase` (+ `maxLength` inherited from `QLineEdit`) |
| `UxNumberField` | `integerDigits`, `decimalDigits`, `thousandsSeparator` |
| `UxInput` | `labelText`, `labelPosition` (`Left` \| `Above`), `text`, `maxLength`, `required` |

</details>

## 🔎 The Selection Button

The icon at the end of the field **does not search or select on its own**: clicking
it emits `selectionRequested()`. Your code decides what to open (usually a modal
selection dialog) and what to return to the field:

```cpp
auto *article = new UxTextInput("Article");
article->textField()->setShowSelector(true);   // trailing "..." icon
connect(article, &UxInput::selectionRequested, this, [=] {
  // Open your selection dialog and, upon acceptance:
    article->setText(selectedValue);
});
```

In `UxDateField`, the always-visible icon is a calendar that opens a
`QCalendarWidget` and writes the selected date.

## 🛠️ Build the Library and Demo

### Linux

Qt installed from distribution packages is detected automatically:

```bash
cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux
./build-linux/demo/UxWidgetsDemo
```

In a graphical session, the final command starts the demo. To distribute a Qt
application on Linux, use the distribution's packaging mechanism or a compatible
deployment tool, since this project does not package it.

### Windows

```bash
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=<qt-path>/mingw_64
cmake --build build
./build/demo/UxWidgetsDemo
```

The `UXWIDGETS_BUILD_DEMO` option (ON by default) builds `UxWidgetsDemo`, an app
that exercises all three controls. On Windows, a `POST_BUILD` step runs
`windeployqt` and copies Qt DLLs and MinGW runtimes next to the executable, so it
can launch by double-clicking without Qt in `PATH`.

## 🗺️ Roadmap

- [ ] **Qt Designer** plugin that exposes controls and their properties in the palette.
- [ ] **Theme-aware** required-field warning (currently uses a fixed light color).
- [ ] New controls following the same pattern (combo box, check box, etc.).
- [ ] Installable package (`find_package(UxWidgets)`) in addition to `add_subdirectory`.

## 📄 License

UxWidgets is distributed under the **MIT** license (see [`LICENSE`](LICENSE)): you
may use, copy, modify, and redistribute it freely while retaining the copyright
notice.

This library **depends on Qt** but does not include it. Qt is distributed under its
own license (**LGPLv3** in its open-source edition, or a commercial license). If you
build and **distribute a binary** that links Qt, you must comply with the Qt LGPL:
link Qt dynamically (as the demo does with Qt DLLs), include the Qt license text, and
state where its source code can be obtained. That compliance is the responsibility of
the person building and distributing the final application, not this source-code
repository. More information at <https://www.qt.io/licensing>.
