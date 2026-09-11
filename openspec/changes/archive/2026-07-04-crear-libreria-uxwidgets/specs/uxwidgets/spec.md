# Spec: Librería de controles Qt `UxWidgets`

## ADDED Requirements

### Requirement: Librería Qt autocontenida y reutilizable
La librería `UxWidgets` SHALL construirse como una biblioteca CMake independiente, sin dependencias de ninguna aplicación, de modo que pueda usarse desde cualquier proyecto Qt sin cambios. SHALL exponer sus cabeceras públicas bajo `include/UxWidgets/` y ofrecer un target enlazable (`UxWidgets::UxWidgets`) que dependa únicamente de `Qt6::Core`, `Qt6::Gui` y `Qt6::Widgets`.

#### Scenario: Construcción aislada de la librería
- **GIVEN** el repositorio con su `CMakeLists.txt`
- **WHEN** se configura y compila la librería con Qt 6 y CMake
- **THEN** compila sin errores y sin requerir ningún fichero externo a la librería

#### Scenario: Consumo desde otro proyecto
- **GIVEN** un proyecto CMake cualquiera con Qt 6 Widgets
- **WHEN** añade `UxWidgets` (por `add_subdirectory`) y enlaza `UxWidgets::UxWidgets`
- **THEN** puede incluir `<UxWidgets/UxInput.h>` e instanciar los controles sin dependencias externas

### Requirement: Arquitectura en dos capas (campo y compuesto)
La librería SHALL organizar los controles en dos capas: una **capa de campo** cuyas clases heredan de `QLineEdit` (`UxCampo` base y las especializaciones `UxCampoTexto`, `UxCampoNumero`, `UxCampoFecha`), y una **capa compuesta** cuyas clases heredan de `QWidget` (`UxInput` y las conveniencias `UxInputTexto`, `UxInputNumero`, `UxInputFecha`). El comportamiento específico de cada tipo SHALL residir en la capa de campo.

#### Scenario: Uso del campo suelto
- **GIVEN** un formulario que solo necesita el campo sin etiqueta ni icono
- **WHEN** instancia un `UxCampoNumero`
- **THEN** obtiene un `QLineEdit` especializado usable directamente como cualquier `QLineEdit`

#### Scenario: Uso del control compuesto
- **GIVEN** un formulario que necesita etiqueta + campo + icono como una unidad
- **WHEN** instancia un `UxInputTexto` con un texto de etiqueta
- **THEN** obtiene un único widget que contiene la etiqueta, el campo de texto y el icono ya alineados

### Requirement: Campo de texto configurable
`UxCampoTexto` SHALL permitir la entrada de texto libre con una longitud máxima configurable, y SHALL poder forzar que todo lo tecleado y pegado aparezca en mayúsculas cuando se active esa opción, sin admitir minúsculas en ese modo.

#### Scenario: Longitud máxima
- **GIVEN** un `UxCampoTexto` con longitud máxima 30
- **WHEN** el usuario teclea o pega más de 30 caracteres
- **THEN** el campo no admite más allá de 30, en el momento de la entrada (no como validación posterior)

#### Scenario: Mayúsculas forzadas
- **GIVEN** un `UxCampoTexto` con la opción de mayúsculas activada
- **WHEN** el usuario teclea o pega texto en minúsculas
- **THEN** el texto queda en mayúsculas y no se admiten minúsculas

### Requirement: Campo numérico con precisión configurable
`UxCampoNumero` SHALL admitir únicamente la entrada de valores numéricos y SHALL permitir configurar el número de **dígitos enteros** y de **dígitos decimales** admitidos, así como si se muestra **separador de miles**. Con cero dígitos decimales SHALL comportarse como entero. SHALL alinear el contenido a la derecha.

#### Scenario: Rechazo de no numéricos
- **GIVEN** un `UxCampoNumero` configurado con cero decimales
- **WHEN** el usuario intenta teclear letras u otros caracteres no numéricos
- **THEN** el campo no los admite

#### Scenario: Límite de dígitos enteros y decimales
- **GIVEN** un `UxCampoNumero` configurado con 3 dígitos enteros y 2 decimales
- **WHEN** el usuario teclea un valor
- **THEN** no admite más de 3 cifras en la parte entera ni más de 2 en la decimal

#### Scenario: Separador de miles
- **GIVEN** un `UxCampoNumero` con separador de miles activado
- **WHEN** contiene un valor de cuatro o más cifras enteras
- **THEN** el valor se muestra con el separador de miles correspondiente

#### Scenario: Alineación a la derecha
- **GIVEN** un `UxCampoNumero`
- **WHEN** contiene un valor
- **THEN** el valor se muestra alineado a la derecha

### Requirement: Campo de fecha tolerante a vacío
`UxCampoFecha` SHALL gestionar fechas sobre un campo de texto (no `QDateEdit`), con formato **`dd/MM/yyyy`** (separador `/`), admitiendo un valor **vacío** sin señalar error cuando el campo no es obligatorio, y SHALL ofrecer un icono de calendario que, al pulsarlo, despliega un selector (`QCalendarWidget`) y escribe en el campo la fecha elegida ya formateada.

#### Scenario: Fecha vacía no obligatoria
- **GIVEN** un `UxCampoFecha` no obligatorio y vacío
- **WHEN** el foco abandona el campo
- **THEN** no se marca error ni se rellena una fecha por defecto

#### Scenario: Selección por calendario
- **GIVEN** un `UxCampoFecha`
- **WHEN** el usuario pulsa el icono de calendario y elige un día
- **THEN** el campo muestra la fecha seleccionada con el formato y separador definidos

### Requirement: Botón selector integrado en el campo
Los campos SHALL integrar su botón de acción **dentro** del propio campo mediante la posición final del `QLineEdit` (sin un widget hermano). En texto y número el botón SHALL ser opcional y, al pulsarse, SHALL emitir una señal de **solicitud de selección** (`seleccionSolicitada()`) para que el consumidor abra su propio diálogo modal de elección; su icono SHALL ser configurable, con **puntos suspensivos `…`** por defecto (affordance de "abrir para elegir"), evitando la lupa por su connotación de zoom. En fecha el icono de calendario SHALL estar siempre presente y abrir el selector de calendario.

#### Scenario: Botón selector que solicita elección
- **GIVEN** un `UxCampoTexto` con el botón selector activado y conectada su señal de selección
- **WHEN** el usuario pulsa el botón
- **THEN** se emite `seleccionSolicitada()`, dejando que el consumidor abra su diálogo modal y devuelva el valor al campo

#### Scenario: Icono por defecto y configurable
- **GIVEN** un `UxCampoTexto` con el botón selector activado y sin icono personalizado
- **THEN** el botón muestra el icono de puntos suspensivos `…` por defecto, sustituible por otro mediante su propiedad de icono

#### Scenario: Sin botón cuando no se pide
- **GIVEN** un `UxCampoTexto` sin activar el botón selector
- **THEN** el campo no muestra ningún botón al final

### Requirement: Control compuesto con etiqueta reposicionable
`UxInput` SHALL componer una etiqueta (`QLabel`), un campo de la capa de campo y su icono como un único widget, y SHALL permitir situar la etiqueta a la **izquierda** o **arriba** del campo mediante una propiedad, reorganizando su layout interno en consecuencia. `UxInput` SHALL reenviar al campo interno las propiedades relevantes (texto, longitud máxima, requerido) y SHALL reemitir la señal de solicitud de selección (`seleccionSolicitada()`) del campo.

#### Scenario: Etiqueta a la izquierda
- **GIVEN** un `UxInput` con la etiqueta configurada a la izquierda
- **WHEN** se muestra
- **THEN** la etiqueta aparece a la izquierda del campo en la misma fila

#### Scenario: Etiqueta arriba
- **GIVEN** un `UxInput` con la etiqueta configurada arriba
- **WHEN** se muestra
- **THEN** la etiqueta aparece sobre el campo

### Requirement: Campo requerido con indicación visual
Los controles SHALL exponer una propiedad de "requerido" que, cuando esté activa y el campo esté vacío, muestre una indicación visual diferenciada.

#### Scenario: Requerido y vacío
- **GIVEN** un control marcado como requerido y sin valor
- **WHEN** se evalúa su estado
- **THEN** presenta la indicación visual de campo pendiente

#### Scenario: Requerido y relleno
- **GIVEN** un control marcado como requerido con un valor introducido
- **THEN** no presenta la indicación visual de pendiente

### Requirement: Resaltado del campo con el foco adaptado al tema
Los campos SHALL resaltar su fondo mientras tienen el foco, para que el usuario identifique de un vistazo el control activo sin depender del parpadeo del cursor. El color de resaltado SHALL adaptarse al tema: un color claro (azul claro por defecto) en tema claro y un color oscuro (azul oscuro por defecto) en tema oscuro, ambos configurables por separado, de modo que el texto del campo mantenga el contraste. La detección de tema SHALL ser dinámica y el resaltado SHALL actualizarse si el tema de la aplicación cambia en caliente. Al perder el foco SHALL restaurar su fondo según su estado (requerido y vacío, o normal). El resaltado de foco SHALL tener prioridad sobre la indicación de requerido mientras el campo esté activo.

#### Scenario: Campo que recibe el foco en tema claro
- **GIVEN** la aplicación en tema claro y un campo sin foco
- **WHEN** el usuario lo enfoca (con ratón o tabulador)
- **THEN** su fondo pasa al color de foco claro, indicando que es el control activo

#### Scenario: Campo que recibe el foco en tema oscuro
- **GIVEN** la aplicación en tema oscuro y un campo sin foco
- **WHEN** el usuario lo enfoca
- **THEN** su fondo pasa al color de foco oscuro, manteniendo legible el texto claro

#### Scenario: Cambio de tema en caliente
- **GIVEN** un campo enfocado y resaltado
- **WHEN** la aplicación cambia de tema (claro↔oscuro) mientras el campo mantiene el foco
- **THEN** el resaltado se actualiza al color del nuevo tema

#### Scenario: Campo que pierde el foco
- **GIVEN** un campo enfocado y resaltado
- **WHEN** el foco pasa a otro control
- **THEN** el campo restaura su fondo (bisque si es requerido y está vacío, normal en caso contrario)

#### Scenario: Prioridad del foco sobre requerido
- **GIVEN** un campo requerido y vacío (fondo bisque)
- **WHEN** recibe el foco
- **THEN** muestra el color de foco del tema mientras esté activo, y vuelve al bisque al perderlo si sigue vacío

### Requirement: Propiedades expuestas para diseñador futuro
Las propiedades públicas tuneables de los controles (texto de etiqueta, posición de etiqueta, longitud máxima, mayúsculas, requerido, dígitos enteros, dígitos decimales, separador de miles, mostrar selector, icono, color de foco claro, color de foco oscuro) SHALL declararse como `Q_PROPERTY`, y los enumerados públicos con `Q_ENUM`, de forma que un futuro plugin de Qt Designer pueda exponerlas sin rediseñar los controles. La construcción de dicho plugin queda fuera de esta capability.

#### Scenario: Propiedad accesible por el metaobjeto
- **GIVEN** un control de la librería
- **WHEN** se consulta su `QMetaObject`
- **THEN** las propiedades tuneables aparecen como propiedades del metaobjeto (disponibles para un editor de propiedades genérico)
