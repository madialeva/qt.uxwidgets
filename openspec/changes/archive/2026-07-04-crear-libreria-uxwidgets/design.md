## Context

Los formularios densos necesitan muchos campos de entrada tipados (texto, número, fecha), cada uno con su etiqueta, su validación y a veces un icono de acción. Montarlos a mano (un `QLineEdit` + un `QLabel` + un validador por campo) no escala ni se reutiliza. Este cambio crea una librería que encapsula ese patrón en un control compuesto configurable.

## Goals / Non-Goals

**Goals:**
- Ofrecer un control compuesto configurable (etiqueta + campo tipado + icono) para no crear tres widgets por campo.
- Tres sabores de campo: texto, número, fecha, especializados en la capa de campo.
- Empaquetar todo como librería Qt reutilizable y autocontenida.
- Admitir fecha vacía cuando no es obligatoria, algo que `QDateEdit` no permite con naturalidad.
- Dejar el terreno preparado para un futuro plugin de Qt Designer (`Q_PROPERTY`), sin construirlo ahora.

**Non-Goals:**
- No se construye el plugin de Qt Designer en este cambio (solo se deja la puerta abierta con `Q_PROPERTY`).
- No se incluyen controles más allá de texto/número/fecha (ampliable después).

## Decisions

### 1. Arquitectura en dos capas (campo `QLineEdit` + compuesto `QWidget`)

```
              QLineEdit                              QWidget
                  │                                     │
            ┌─────┴─────┐                        ┌──────┴──────┐
            │  UxCampo  │  (común)               │   UxInput   │  (compuesto)
            └─────┬─────┘                        │ QLabel +    │
      ┌───────────┼───────────┐                  │ UxCampo* +  │
 UxCampoTexto UxCampoNumero UxCampoFecha          │ layout      │
                                                  └──────┬──────┘
                                            ┌────────────┼────────────┐
                                      UxInputTexto  UxInputNumero  UxInputFecha
                                          (conveniencias delgadas que crean el UxCampo* que toca)
```

- **La especialización de comportamiento vive en la capa de campo** (`UxCampo*`), que es donde de verdad cambia (validador, icono, alineación).
- **El compuesto `UxInput` es casi genérico**: aloja una etiqueta y un `UxCampo`, gestiona la posición de la etiqueta y reenvía propiedades. Las tres subclases compuestas (`UxInputTexto/Numero/Fecha`) son fábricas finas para dar buen sitio de llamada con muchos campos (`new UxInputFecha(tr("Fecha"))`).
- *Por qué no un único `UxInput` con enum `tipo` y `switch` interno:* la herencia sustituye el `switch` por polimorfismo y mantiene cada sabor pequeño.

### 2. La fecha se implementa sobre `QLineEdit`, no `QDateEdit`

- `QDateEdit`/`QAbstractSpinBox` **no tiene un estado vacío/nulo natural**: siempre contiene una fecha válida. El requisito de admitir **fecha vacía cuando no es obligatoria** choca de frente con él.
- Las validaciones a medida (separador `/`, entrada parcial, tolerancia) son libres sobre un `QLineEdit` y rígidas en `QDateEdit`.
- **Uniformidad**: los tres campos comparten `QLineEdit` como base, por lo que el reenvío de propiedades del compuesto es idéntico y el icono por `addAction` encaja igual en los tres.
- *Coste asumido:* el popup de calendario se implementa a mano (un `QCalendarWidget` en un contenedor emergente colgado del icono), en vez de venir gratis con `QDateEdit`.

### 3. Botón selector integrado en el campo (`addAction`)

- Se usa `QLineEdit::addAction(icono, QLineEdit::TrailingPosition)` para incrustar el icono **dentro** del campo, evitando un widget hermano (`QToolButton`) y su alineación.
- **Semántica "abrir selector", no "buscar"**: la acción habitual es abrir un diálogo modal de elección, no una búsqueda incremental. Por eso:
  - La señal se llama `seleccionSolicitada()`.
  - El icono por defecto es **puntos suspensivos `…`**, el affordance convencional de "pulsa para abrir un diálogo y elegir". Se descarta la lupa por su connotación de zoom.
  - El icono es **configurable** (propiedad `icono`): distintos campos pueden mostrar uno propio manteniendo la misma mecánica.
- Es opt-in en texto/número (propiedad `mostrarSelector`); en fecha el icono de calendario está siempre presente y abre el `QCalendarWidget`.

### 3b. Precisión del campo numérico y formato de fecha

- **Número**: propiedades `digitosEnteros` y `digitosDecimales` (cuántas cifras a cada lado del separador decimal) y `separadorMiles` (bool). El validador se construye a partir de esas cifras; el contenido se alinea a la derecha. Con `digitosDecimales = 0` se comporta como entero.
- **Fecha**: formato fijo **`dd/MM/yyyy`** con separador `/`.

### 4. Etiqueta reposicionable

- Propiedad `labelPosition` (`Izquierda` | `Arriba`). Al cambiarla, el compuesto reconstruye su layout interno (`QHBoxLayout` para izquierda, `QVBoxLayout` para arriba).

### 5. Empaquetado como librería estática reutilizable

- Estructura en la raíz del repo: `include/UxWidgets/*.h`, `src/*.cpp`, `CMakeLists.txt`.
- Biblioteca **estática** CMake con target `UxWidgets` y alias `UxWidgets::UxWidgets`, `Qt6::Core/Gui/Widgets`, `CMAKE_AUTOMOC ON`, C++17.
- **Sin dependencias de código externo**: la librería no conoce ninguna aplicación. Se consume con `add_subdirectory(<ruta>)` + `target_link_libraries(MiApp PRIVATE UxWidgets::UxWidgets)`.

### 6. `Q_PROPERTY` desde el día uno

- Aunque hoy se use en C++ directo, todas las propiedades tuneables públicas (`labelText`, `labelPosition`, `text`, `maxLength`, `mayusculas`, `requerido`, `digitosEnteros`, `digitosDecimales`, `separadorMiles`, `mostrarSelector`, `icono`, `colorFocoClaro`, `colorFocoOscuro`…) se declaran con `Q_PROPERTY` + `Q_ENUM` para los enums. Coste casi nulo y habilita un futuro plugin de Qt Designer que las muestre en el panel de propiedades.

### 7. Resaltado del campo con el foco, adaptado al tema

- El campo activo resalta su fondo para identificarlo de un vistazo sin depender del parpadeo del cursor.
- Vive en la **base `UxCampo`** (lo heredan los tres sabores), sobreescribiendo `focusInEvent`/`focusOutEvent`.
- **Dos colores según tema**, configurables por separado: `colorFocoClaro` (por defecto `#CCE8FF`, azul claro para tema claro con texto negro) y `colorFocoOscuro` (por defecto `#14375A`, azul oscuro para tema oscuro con texto blanco). Un azul claro con letras blancas sería ilegible, de ahí los dos.
- **Detección de tema dinámica** por heurística estándar: si el color `QPalette::Text` es claro (`lightness() > 128`), el tema es oscuro. Se lee de la paleta heredada (rol `Text`, que no reasignamos).
- **Cambio de tema en caliente**: se sobreescribe `changeEvent` para reaccionar a `ApplicationPaletteChange`/`PaletteChange` y reevaluar el fondo. Un guard (`m_enActualizacion`) evita la reentrada que provocaría nuestro propio `setPalette`.
- **Prioridad de color unificada** en un único `actualizarFondo()`: foco (color del tema) > requerido y vacío (bisque) > normal. Al perder el foco el campo recupera el estado que le corresponda.
- Los sabores que ya sobreescriben el foco (p. ej. `UxCampoNumero`, por el separador de miles) encadenan a `UxCampo::focus*Event` en vez de a `QLineEdit::`, para no perder el resaltado.
- *Implementación por paleta* (`QPalette::Base`): sencilla y sin stylesheet. En una app consumidora con hoja de estilos global el color podría no aplicarse; se revisaría pasando a stylesheet. (El aviso de requerido `bisque` es un color fijo claro; si se usa tema oscuro con campos requeridos, convendría darle el mismo tratamiento por tema — pendiente si surge la necesidad.)

## Risks / Trade-offs

- **Popup de calendario a mano** (por descartar `QDateEdit`) → más código que la opción nativa, pero es la única forma de cumplir el requisito de fecha vacía/opcional. Mitigación: encapsularlo una sola vez en `UxCampoFecha`.
- **Dos capas + subclases finas** puede parecer sobre-ingeniería para tres tipos → se acepta porque mantiene cada clase pequeña y es la que mejor escala a muchos campos y a reutilización externa.
- **`setMaxLength` cuenta en `QChar` (UTF-16)** → para textos latinos coincide con lo esperado; no se prevén caracteres fuera del BMP.
