# Crear la librería de controles Qt reutilizables `UxWidgets`

## Why

Una aplicación Qt con formularios densos necesita **muchos campos de entrada tipados** (texto, número, fecha), cada uno con su validación (longitud máxima, solo dígitos, mayúsculas, formato de fecha) y, a menudo, una etiqueta y un icono de acción asociados. Resolver esto **campo a campo, a mano** (un `QLineEdit` + un `QLabel` + un validador por cada dato) no escala a cientos de campos ni se puede reutilizar entre proyectos.

Se quiere un control de entrada **compuesto y configurable** —etiqueta + campo tipado + icono en una sola unidad— empaquetado como **librería Qt reutilizable e independiente**, de modo que cualquier aplicación pueda usarlo como dependencia.

## What Changes

- **Nueva librería `UxWidgets`**, autocontenida y **sin dependencias de ninguna aplicación**, consumible desde cualquier proyecto Qt con `add_subdirectory` + `target_link_libraries(... UxWidgets::UxWidgets)`.
- **Arquitectura en dos capas**:
  - **Capa de campo** — hereda de `QLineEdit`: una base `UxCampo` con el comportamiento común (icono de acción integrado, campo requerido, resaltado de foco) y tres especializaciones `UxCampoTexto`, `UxCampoNumero`, `UxCampoFecha`.
  - **Capa compuesta** — hereda de `QWidget`: `UxInput` = etiqueta (`QLabel`) + un `UxCampo` + layout, con la etiqueta reposicionable (izquierda o arriba) y reenvío de propiedades al campo. Tres conveniencias delgadas (`UxInputTexto`, `UxInputNumero`, `UxInputFecha`) para el sitio de llamada.
- **Fecha implementada sobre `QLineEdit`** (no `QDateEdit`), para poder admitir **fecha vacía cuando no es obligatoria** sin protestar y validaciones a medida, con formato **`dd/MM/yyyy`** (separador `/`) y un icono de **calendario** que despliega un `QCalendarWidget` en popup y escribe la fecha formateada.
- **Número con precisión configurable**: se indica el número de **dígitos enteros** y de **dígitos decimales** admitidos, y si se muestra **separador de miles**; el contenido se alinea a la derecha.
- **Botón selector integrado dentro del campo** vía `QLineEdit::addAction(..., TrailingPosition)` (sin un widget hermano): en texto/número un icono (por defecto **puntos suspensivos `…`**, configurable) que al pulsarlo emite `seleccionSolicitada()` para que el consumidor **abra un diálogo modal de elección**; en fecha, el icono de calendario.
- **Resaltado del campo activo con el foco, adaptado al tema**: el campo enfocado tiñe su fondo para identificar de un vistazo el control activo sin depender del parpadeo del cursor. Usa **dos colores configurables** —azul claro en tema claro, azul oscuro en tema oscuro— detectados dinámicamente (y actualizados si el tema cambia en caliente) para que el texto mantenga el contraste. Tiene prioridad sobre el aviso de requerido mientras el campo está activo.
- **Propiedades públicas expuestas como `Q_PROPERTY`** desde el principio, de modo que —aunque hoy se use en C++ directo sin diseñador— quede la puerta abierta a un **plugin de Qt Designer** en el futuro sin rediseñar los controles.
- **Objetivo de validación**: un pequeño ejecutable de demostración dentro de la propia librería que instancia los tres controles.

## Capabilities

### New Capabilities
- `uxwidgets`: librería de controles Qt reutilizables de entrada de datos (campo tipado + control compuesto etiqueta/campo/icono), independiente de cualquier aplicación.

### Modified Capabilities
<!-- Ninguna. -->

## Impact

- **Nuevo código**: cabeceras en `include/UxWidgets/`, fuentes en `src/`, `CMakeLists.txt` en la raíz y demo opcional en `demo/`.
- **Build**: la librería se construye como biblioteca estática CMake con `Qt6::Core/Gui/Widgets` y `CMAKE_AUTOMOC`. Cualquier proyecto la enlaza mediante `add_subdirectory(<ruta>) ` + `target_link_libraries(... UxWidgets::UxWidgets)`.
- **Sin dependencias externas** más allá de Qt 6 y un compilador C++17.
