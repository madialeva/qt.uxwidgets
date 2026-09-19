# Spec: Librería de controles Qt `UxWidgets`

## Purpose

Define la librería `UxWidgets`: un conjunto de controles Qt reutilizables de entrada
y presentación de datos, autocontenido e independiente de cualquier aplicación.
Ofrece controles de entrada compuestos y configurables (texto, número o fecha)
organizados en dos capas —un campo tipado que hereda de `QLineEdit` y un control
compuesto etiqueta/campo/icono que hereda de `QWidget`—, además de una etiqueta no
editable (`UxLabel`), un compuesto de selección (`UxComboInput`) y una casilla de
verificación (`UxCheck`), para no tener que
crear y alinear varios widgets por cada campo en formularios densos.

## Requirements

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

### Requirement: Arquitectura en capas (campo, compuesto e independientes)
La librería SHALL organizar los controles de entrada en dos capas: una **capa de campo** cuyas clases heredan de `QLineEdit` (`UxField` base y las especializaciones `UxTextField`, `UxNumberField`, `UxDateField`), y una **capa compuesta** cuyas clases heredan de `QWidget` (`UxInput` y las conveniencias `UxTextInput`, `UxNumberInput`, `UxDateInput`). El comportamiento específico de cada tipo SHALL residir en la capa de campo. Fuera de la capa de campo, la librería SHALL ofrecer además controles independientes: la etiqueta no editable `UxLabel` (que hereda de `QLabel`), el compuesto de selección `UxComboInput` (que hereda de `QWidget`) y la casilla de verificación `UxCheck` (que hereda de `QCheckBox`).

#### Scenario: Uso del campo suelto
- **GIVEN** un formulario que solo necesita el campo sin etiqueta ni icono
- **WHEN** instancia un `UxNumberField`
- **THEN** obtiene un `QLineEdit` especializado usable directamente como cualquier `QLineEdit`

#### Scenario: Uso del control compuesto
- **GIVEN** un formulario que necesita etiqueta + campo + icono como una unidad
- **WHEN** instancia un `UxTextInput` con un texto de etiqueta
- **THEN** obtiene un único widget que contiene la etiqueta, el campo de texto y el icono ya alineados

#### Scenario: Uso de los controles independientes
- **GIVEN** un formulario que necesita mostrar un valor de solo lectura, elegir de una lista o marcar una opción
- **WHEN** instancia un `UxLabel`, un `UxComboInput` o un `UxCheck`
- **THEN** obtiene respectivamente una etiqueta no editable, un compuesto etiqueta + desplegable o una casilla de verificación, sin depender de la capa de campo

### Requirement: Campo de texto configurable
`UxTextField` SHALL permitir la entrada de texto libre con una longitud máxima configurable, y SHALL poder forzar que todo lo tecleado y pegado aparezca en mayúsculas cuando se active esa opción, sin admitir minúsculas en ese modo.

#### Scenario: Longitud máxima
- **GIVEN** un `UxTextField` con longitud máxima 30
- **WHEN** el usuario teclea o pega más de 30 caracteres
- **THEN** el campo no admite más allá de 30, en el momento de la entrada (no como validación posterior)

#### Scenario: Mayúsculas forzadas
- **GIVEN** un `UxTextField` con la opción de mayúsculas activada
- **WHEN** el usuario teclea o pega texto en minúsculas
- **THEN** el texto queda en mayúsculas y no se admiten minúsculas

### Requirement: Campo numérico con precisión configurable
`UxNumberField` SHALL admitir únicamente la entrada de valores numéricos y SHALL permitir configurar el número de **dígitos enteros** y de **dígitos decimales** admitidos, así como si se muestra **separador de miles**. Con cero dígitos decimales SHALL comportarse como entero. SHALL alinear el contenido a la derecha.

#### Scenario: Rechazo de no numéricos
- **GIVEN** un `UxNumberField` configurado con cero decimales
- **WHEN** el usuario intenta teclear letras u otros caracteres no numéricos
- **THEN** el campo no los admite

#### Scenario: Límite de dígitos enteros y decimales
- **GIVEN** un `UxNumberField` configurado con 3 dígitos enteros y 2 decimales
- **WHEN** el usuario teclea un valor
- **THEN** no admite más de 3 cifras en la parte entera ni más de 2 en la decimal

#### Scenario: Separador de miles
- **GIVEN** un `UxNumberField` con separador de miles activado
- **WHEN** contiene un valor de cuatro o más cifras enteras
- **THEN** el valor se muestra con el separador de miles correspondiente

#### Scenario: Alineación a la derecha
- **GIVEN** un `UxNumberField`
- **WHEN** contiene un valor
- **THEN** el valor se muestra alineado a la derecha

### Requirement: Campo de fecha tolerante a vacío
`UxDateField` SHALL gestionar fechas sobre un campo de texto (no `QDateEdit`), con formato **`dd/MM/yyyy`** (separador `/`), admitiendo un valor **vacío** sin señalar error cuando el campo no es obligatorio, y SHALL ofrecer un icono de calendario que, al pulsarlo, despliega un selector (`QCalendarWidget`) y escribe en el campo la fecha elegida ya formateada.

#### Scenario: Fecha vacía no obligatoria
- **GIVEN** un `UxDateField` no obligatorio y vacío
- **WHEN** el foco abandona el campo
- **THEN** no se marca error ni se rellena una fecha por defecto

#### Scenario: Selección por calendario
- **GIVEN** un `UxDateField`
- **WHEN** el usuario pulsa el icono de calendario y elige un día
- **THEN** el campo muestra la fecha seleccionada con el formato y separador definidos

### Requirement: Botón selector integrado en el campo
Los campos SHALL integrar su botón de acción **dentro** del propio campo mediante la posición final del `QLineEdit` (sin un widget hermano). En texto y número el botón SHALL ser opcional y, al pulsarse, SHALL emitir una señal de **solicitud de selección** (`selectionRequested()`) para que el consumidor abra su propio diálogo modal de elección; su icono SHALL ser configurable, con **puntos suspensivos `…`** por defecto (affordance de "abrir para elegir"), evitando la lupa por su connotación de zoom. En fecha el icono de calendario SHALL estar siempre presente y abrir el selector de calendario.

#### Scenario: Botón selector que solicita elección
- **GIVEN** un `UxTextField` con el botón selector activado y conectada su señal de selección
- **WHEN** el usuario pulsa el botón
- **THEN** se emite `selectionRequested()`, dejando que el consumidor abra su diálogo modal y devuelva el valor al campo

#### Scenario: Icono por defecto y configurable
- **GIVEN** un `UxTextField` con el botón selector activado y sin icono personalizado
- **THEN** el botón muestra el icono de puntos suspensivos `…` por defecto, sustituible por otro mediante su propiedad de icono

#### Scenario: Sin botón cuando no se pide
- **GIVEN** un `UxTextField` sin activar el botón selector
- **THEN** el campo no muestra ningún botón al final

### Requirement: Control compuesto con etiqueta reposicionable
`UxInput` SHALL componer una etiqueta (`QLabel`), un campo de la capa de campo y su icono como un único widget, y SHALL permitir situar la etiqueta a la **izquierda** o **arriba** del campo mediante una propiedad, reorganizando su layout interno en consecuencia. `UxInput` SHALL reenviar al campo interno las propiedades relevantes (texto, longitud máxima, requerido) y SHALL reemitir la señal de solicitud de selección (`selectionRequested()`) del campo.

#### Scenario: Etiqueta a la izquierda
- **GIVEN** un `UxInput` con la etiqueta configurada a la izquierda
- **WHEN** se muestra
- **THEN** la etiqueta aparece a la izquierda del campo en la misma fila

#### Scenario: Etiqueta arriba
- **GIVEN** un `UxInput` con la etiqueta configurada arriba
- **WHEN** se muestra
- **THEN** la etiqueta aparece sobre el campo

### Requirement: Compuesto de selección con etiqueta integrada
`UxComboInput` SHALL combinar una etiqueta (`QLabel`) y un desplegable (`QComboBox`) como un único widget, y SHALL permitir situar la etiqueta a la **izquierda** o **arriba** del desplegable mediante una propiedad, reorganizando su layout interno en consecuencia. SHALL exponer el desplegable para añadir elementos, consultar y fijar el elemento seleccionado y leer el dato asociado a cada elemento. Al bloquear el control SHALL deshabilitar únicamente el desplegable, manteniendo la etiqueta con su aspecto normal.

#### Scenario: Etiqueta integrada
- **GIVEN** un `UxComboInput` visible
- **WHEN** se cambia el idioma de la interfaz
- **THEN** la etiqueta del control se muestra traducida, junto a su desplegable

#### Scenario: Posición de la etiqueta
- **GIVEN** un `UxComboInput`
- **WHEN** se configura la etiqueta a la izquierda o arriba
- **THEN** el control reorganiza la etiqueta y el desplegable según la posición elegida

#### Scenario: Poblado y selección
- **GIVEN** un `UxComboInput` con varios elementos añadidos
- **WHEN** se fija el elemento seleccionado por su dato asociado
- **THEN** el desplegable muestra el elemento correspondiente y permite leer su dato

#### Scenario: Bloqueo del control
- **GIVEN** un `UxComboInput` que se deshabilita
- **WHEN** el control queda bloqueado
- **THEN** el desplegable no permite cambiar la selección y la etiqueta conserva su color normal (no deshabilitado)

### Requirement: Etiqueta no editable
`UxLabel` SHALL mostrar texto **no editable** con alineación configurable, SHALL admitir modo **multilínea** con ajuste de palabra, SHALL poder mostrar una **imagen** opcional a la izquierda del texto y SHALL poder **rellenar con puntos** el espacio libre hasta el ancho del control cuando no sea multilínea. SHALL ofrecer un **resaltado al pasar el ratón** por encima (color configurable, tipografía subrayada y cursor de mano), activo solo cuando la propiedad de resalte está activada y el control está habilitado, y SHALL emitir una señal `clicked()` en la pulsación izquierda mientras el resalte está activo. El color del texto SHALL adaptarse al estado habilitado/deshabilitado del control.

#### Scenario: Solo lectura
- **GIVEN** una etiqueta no editable mostrando un valor
- **WHEN** el usuario intenta teclear o modificar el texto
- **THEN** el texto no cambia, porque el control no admite edición

#### Scenario: Resaltado al pasar el ratón
- **GIVEN** una etiqueta no editable con el resalte activado y habilitada
- **WHEN** el ratón entra en el control
- **THEN** el texto se muestra subrayado y con el color de resalte, y el cursor cambia a mano; al salir, recupera su aspecto original

#### Scenario: Resalte deshabilitado
- **GIVEN** una etiqueta no editable
- **WHEN** el resalte está desactivado y el ratón pasa por encima
- **THEN** el aspecto del texto no cambia

#### Scenario: Texto multilínea
- **GIVEN** una etiqueta no editable en modo multilínea
- **WHEN** el texto es más ancho que el control
- **THEN** el texto se ajusta en varias líneas dentro del ancho disponible

#### Scenario: Imagen a la izquierda
- **GIVEN** una etiqueta no editable con una imagen asignada
- **WHEN** se muestra el control
- **THEN** la imagen aparece a la izquierda del texto y el texto se desplaza para no solaparse con ella

#### Scenario: Relleno con puntos
- **GIVEN** una etiqueta no editable, no multilínea, con el relleno con puntos activado
- **WHEN** el texto es más estrecho que el ancho del control
- **THEN** se completa con puntos el espacio libre hasta el borde del control

### Requirement: Casilla de verificación con resaltado y modo de solo lectura
`UxCheck` SHALL ofrecer una casilla de verificación (`QCheckBox`) con resaltado de fondo mientras tiene el foco de teclado, con un color adaptado al tema (claro/oscuro) que se actualiza si el tema de la aplicación cambia en caliente. SHALL mostrar el texto en un color diferenciado mientras la casilla esté marcada, también adaptado al tema. SHALL exponer un modo de solo lectura (`readOnly`) que ignore los cambios del usuario (ratón y teclado) manteniendo el aspecto normal (sin el aspecto de deshabilitado) y sin poder recibir el foco.

#### Scenario: Casilla que recibe el foco en tema claro
- **WHEN** el usuario enfoca con ratón o tabulador una casilla `UxCheck` editable en tema claro
- **THEN** su fondo pasa al color de foco claro, indicando que es el control activo

#### Scenario: Casilla que recibe el foco en tema oscuro
- **WHEN** el usuario enfoca una casilla `UxCheck` editable en tema oscuro
- **THEN** su fondo pasa al color de foco oscuro, manteniendo legible el texto claro

#### Scenario: Texto diferenciado en estado marcado
- **WHEN** la casilla está marcada
- **THEN** su texto se muestra en el color de marcado del tema activo (granate en tema claro, rosa pálido en tema oscuro)

#### Scenario: Cambio de tema en caliente
- **WHEN** la aplicación cambia de tema (claro↔oscuro) mientras la casilla mantiene el foco o está marcada
- **THEN** el resaltado de foco y el color de marcado se actualizan al color del nuevo tema

#### Scenario: Casilla que pierde el foco
- **WHEN** el foco pasa a otro control
- **THEN** la casilla restaura su fondo normal, conservando el color de marcado si sigue marcada

#### Scenario: Solo lectura ignora el ratón
- **WHEN** una casilla `UxCheck` está en modo de solo lectura y el usuario pulsa sobre ella con el ratón
- **THEN** su estado no cambia y conserva su aspecto normal (no aparece deshabilitada)

#### Scenario: Solo lectura ignora el teclado y no recibe el foco
- **WHEN** una casilla `UxCheck` está en modo de solo lectura
- **THEN** no puede recibir el foco por tabulador y las teclas de activación (espacio) no cambian su estado, aunque el programa sí puede fijarlo con `setChecked`

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
Las propiedades públicas tuneables de los controles (texto de etiqueta, posición de etiqueta, longitud máxima, mayúsculas, requerido, dígitos enteros, dígitos decimales, separador de miles, mostrar selector, icono, color de foco claro, color de foco oscuro, multilínea, resalte, color de resalte, relleno con puntos, imagen y solo lectura) SHALL declararse como `Q_PROPERTY`, y los enumerados públicos con `Q_ENUM`, de forma que un futuro plugin de Qt Designer pueda exponerlas sin rediseñar los controles. La construcción de dicho plugin queda fuera de esta capability.

#### Scenario: Propiedad accesible por el metaobjeto
- **GIVEN** un control de la librería
- **WHEN** se consulta su `QMetaObject`
- **THEN** las propiedades tuneables aparecen como propiedades del metaobjeto (disponibles para un editor de propiedades genérico)

### Requirement: Nomenclatura inglesa de la API y del código fuente
La librería SHALL exponer exclusivamente una API pública en inglés: nombres de cabeceras, clases, constructores, métodos, señales, propiedades `Q_PROPERTY`, enumerados y valores de enumerados. El código fuente SHALL usar identificadores, comentarios y textos de interfaz en inglés. Los nombres públicos españoles anteriores SHALL dejar de estar disponibles.

#### Scenario: Consumo con la API inglesa
- **WHEN** una aplicación incluye las cabeceras públicas y crea cualquiera de los controles de UxWidgets
- **THEN** utiliza exclusivamente nombres de clases, métodos, señales, propiedades y enumerados en inglés

#### Scenario: Rechazo de la API española eliminada
- **WHEN** una aplicación consumidora intenta incluir una cabecera o usar un identificador público español anterior
- **THEN** la compilación falla al no existir ese símbolo o cabecera

#### Scenario: Comportamiento conservado tras la migración
- **WHEN** una aplicación configura los controles con la API inglesa equivalente
- **THEN** conserva el comportamiento de validación, selector, calendario, requerido y resaltado de foco definido para UxWidgets
