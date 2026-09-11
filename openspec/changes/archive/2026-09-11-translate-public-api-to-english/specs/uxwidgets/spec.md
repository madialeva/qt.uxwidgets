## ADDED Requirements

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
