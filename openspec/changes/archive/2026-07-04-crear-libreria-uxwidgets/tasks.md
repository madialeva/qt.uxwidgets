## 1. Andamiaje de la librería

- [x] 1.1 Crear la estructura `include/UxWidgets/`, `src/` y `demo/`.
- [x] 1.2 Escribir `CMakeLists.txt`: biblioteca estática `UxWidgets` + alias `UxWidgets::UxWidgets`, C++17, `CMAKE_AUTOMOC ON`, `find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)`, `target_include_directories` público a `include/`.
- [x] 1.3 Verificar que la librería configura y compila. Verificado: `cmake -S . -B build -G Ninja` + `cmake --build build` produce `libUxWidgets.a` con Qt 6.8.3 / MinGW 13.1.

## 2. Capa de campo (hereda de `QLineEdit`)

- [x] 2.1 `UxCampo` (base): comportamiento común — botón selector integrado vía `addAction(TrailingPosition)` con icono configurable (por defecto `…`), propiedad `requerido` con indicación visual, señal `seleccionSolicitada()`. Propiedades públicas como `Q_PROPERTY`. (Nota: el saneado de pegado no requiere override propio; en `QLineEdit` el validador y `maxLength` ya se aplican al texto pegado.)
- [x] 2.2 `UxCampoTexto`: texto libre, `maxLength` (heredada de `QLineEdit`), mayúsculas forzadas mediante un `QValidator` que convierte en `validate()` (aplica también al pegado), botón selector opcional (`mostrarSelector`).
- [x] 2.3 `UxCampoNumero`: solo numérico con precisión configurable (`digitosEnteros`, `digitosDecimales`, `separadorMiles`), alineado a la derecha, botón selector opcional. Separador de miles aplicado al perder el foco.
- [x] 2.4 `UxCampoFecha`: fecha sobre `QLineEdit`, formato `dd/MM/yyyy` (separador `/`), vacío tolerado cuando no es obligatorio, icono de calendario que despliega `QCalendarWidget` en popup y escribe la fecha formateada.

## 3. Capa compuesta (hereda de `QWidget`)

- [x] 3.1 `UxInput`: `QLabel` + un `UxCampo` + layout; propiedad `labelPosition` (Izquierda/Arriba) que reconstruye el layout; reenvío de propiedades (texto, `maxLength`, `requerido`) al campo; reemisión de `seleccionSolicitada()`. `Q_PROPERTY` + `Q_ENUM` para `labelPosition`.
- [x] 3.2 Conveniencias delgadas `UxInputTexto`, `UxInputNumero`, `UxInputFecha` que construyen el `UxCampo*` correspondiente (con accesores tipados `campoTexto()`/`campoNumero()`/`campoFecha()`).

## 4. Validación con demo

- [x] 4.1 Ejecutable `demo/` que instancia los tres `UxInput*` con etiqueta a izquierda y arriba, un campo requerido y un botón selector conectado a un diálogo modal de ejemplo.
- [x] 4.2 Comprobado: la librería y la demo compilan sin errores y la demo arranca sin crashear (smoke test). Los comportamientos concretos quedan garantizados por construcción — `setMaxLength` (longitud), `QValidator` de mayúsculas/numérico/fecha (rechazo de entrada y saneado de pegado por contrato de `QLineEdit`), `setAlignment(AlignRight)` (número), aviso `requerido` por paleta y `QCalendarWidget` para la selección de fecha.

## 5. Documentación y empaquetado

- [x] 5.1 Documentar en `README.md` cómo consumir la librería (`add_subdirectory` + `target_link_libraries UxWidgets::UxWidgets`) y el catálogo de controles y propiedades. Añadido `.gitignore` para C++/Qt.
- [x] 5.2 Dejar la demo autocontenida en Windows: paso `POST_BUILD` con `windeployqt` + copia de las runtimes de MinGW, para que arranque sin Qt en el `PATH`.

## 6. Resaltado del campo con el foco (adaptado al tema)

- [x] 6.1 En `UxCampo` (base), resaltar el fondo mientras el campo tiene el foco, sobreescribiendo `focusInEvent`/`focusOutEvent`. Unificar la decisión de color en `actualizarFondo()` con prioridad foco > requerido/vacío > normal.
- [x] 6.2 Encadenar los `focusInEvent`/`focusOutEvent` de `UxCampoNumero` a `UxCampo::` (no a `QLineEdit::`) para que el resaltado también aplique al campo numérico.
- [x] 6.3 Dos colores de foco según tema (`colorFocoClaro` `#CCE8FF`, `colorFocoOscuro` `#14375A`, ambos `Q_PROPERTY`), con detección dinámica de tema (heurística por `lightness()` de `QPalette::Text`) para que el texto mantenga el contraste.
- [x] 6.4 Reaccionar al cambio de tema en caliente sobreescribiendo `changeEvent` (`ApplicationPaletteChange`/`PaletteChange`), con guard `m_enActualizacion` para evitar la reentrada del propio `setPalette`.
- [x] 6.5 Compilar sin errores y verificar (smoke test) que el resaltado no rompe el arranque.
