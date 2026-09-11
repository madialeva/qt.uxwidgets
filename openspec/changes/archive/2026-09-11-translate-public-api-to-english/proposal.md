## Why

La API pública y el código fuente usan identificadores en español, lo que dificulta la adopción y el mantenimiento por desarrolladores internacionales. El repositorio se va a publicar con documentación técnica en inglés y necesita una convención única y coherente para la API, las implementaciones y la demo.

## What Changes

- **BREAKING** Renombrar al inglés todas las clases públicas, cabeceras, métodos, señales, propiedades `Q_PROPERTY`, enumerados y valores de enumerados de UxWidgets.
- **BREAKING** Actualizar la demo y las instrucciones de consumo para que usen exclusivamente la API renombrada.
- Traducir al inglés las variables y los comentarios del código fuente, incluidos los textos de la demo.
- Establecer en `AGENTS.md` que el código fuente, sus identificadores y comentarios se escriben en inglés; OpenSpec y el diálogo con el agente permanecen en español.

## Capabilities

### New Capabilities

Ninguna.

### Modified Capabilities

- `uxwidgets`: Sustituir la nomenclatura española de la API pública por su equivalente en inglés sin alterar el comportamiento de los controles.

## Impact

- Cabeceras públicas en `include/UxWidgets/`, implementaciones en `src/` y la demo.
- Todas las inclusiones y los consumidores existentes deberán migrar a los nuevos nombres; no se conservarán alias de compatibilidad.
- `CMakeLists.txt`, `README.md` y `AGENTS.md` se actualizarán para reflejar la nomenclatura y la convención de idioma.
- No se añaden dependencias ni se modifica el comportamiento de validación, foco, requerido, selector o calendario.
