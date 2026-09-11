## Context

La librería expone en sus cabeceras y en la demo identificadores españoles. La migración es transversal: incluye nombres de archivos, clases, API Qt metaobjeto, implementaciones, demo, CMake y documentación. El motivo y el alcance funcional están definidos en `proposal.md` y `specs/uxwidgets/spec.md`.

## Goals / Non-Goals

**Goals:**

- Ofrecer una única API pública escrita en inglés.
- Traducir los identificadores privados, comentarios y textos de la demo al inglés.
- Mantener exactamente el comportamiento de los controles y su integración CMake.
- Dejar explícito el convenio de idioma para futuras contribuciones.

**Non-Goals:**

- Mantener cabeceras, clases o métodos españoles como alias de compatibilidad.
- Cambiar validadores, diseño visual, distribución, dependencias o arquitectura.
- Traducir los artefactos de OpenSpec ni el diálogo con el agente.

## Decisions

### Renombrado incompatible y uniforme

Se eliminará la API española en lugar de introducir aliases obsoletos. Mantener ambas variantes duplicaría señales, propiedades de metaobjeto y superficie pública sin aportar valor en una librería todavía en desarrollo.

Mapa principal de tipos:

| Actual | Nuevo |
|---|---|
| `UxCampo` | `UxField` |
| `UxCampoTexto` | `UxTextField` |
| `UxCampoNumero` | `UxNumberField` |
| `UxCampoFecha` | `UxDateField` |
| `UxInput` | `UxInput` |
| `UxInputTexto` | `UxTextInput` |
| `UxInputNumero` | `UxNumberInput` |
| `UxInputFecha` | `UxDateInput` |

Las cabeceras, archivos fuente y declaraciones anticipadas seguirán el mismo mapa. Los miembros públicos y las propiedades se traducirán literalmente, por ejemplo: `requerido` a `required`, `mostrarSelector` a `showSelector`, `seleccionSolicitada` a `selectionRequested`, `campo` a `field`, `campoTexto` a `textField`, `campoNumero` a `numberField`, `campoFecha` a `dateField`, `Izquierda` a `Left` y `Arriba` a `Above`.

### Renombrado mecánico por capas

Se actualizarán primero cabeceras y archivos de la capa de campo, después la capa compuesta y sus conveniencias, y por último la demo, CMake, README y AGENTS. Cada capa se compilará antes de continuar para descubrir inclusiones o símbolos omitidos.

Se usará el renombrado semántico cuando el lenguaje lo permita y búsquedas dirigidas para nombres de archivos, `Q_PROPERTY`, `Q_ENUM`, conexiones de señales y documentación.

### Convenio de idioma documentado

`AGENTS.md` establecerá que el código fuente, sus identificadores, comentarios y textos de demostración se escriben en inglés. La excepción se limita a changes de OpenSpec y a la conversación con el agente, que se mantienen en español.

## Risks / Trade-offs

- [Rotura de consumidores existentes] → Se declara el cambio como incompatible, se actualiza el README con los nuevos nombres y se omiten aliases deliberadamente.
- [Símbolos de metaobjeto o señales sin renombrar] → Se revisan específicamente `Q_PROPERTY`, `Q_ENUM`, `signals` y las conexiones antes de compilar.
- [Inclusiones o listas CMake desactualizadas] → Se recompila desde un directorio de build limpio tras renombrar archivos.
- [Cambio accidental de comportamiento] → Se limita la edición a nombres y textos; se valida la demo y la compilación completa en Linux.

## Migration Plan

1. Actualizar los consumidores para incluir las nuevas cabeceras y usar la tabla de nombres inglesa.
2. Configurar y compilar el proyecto desde un directorio de build limpio.
3. Ejecutar la demo para comprobar los tres controles y el selector de calendario.
4. No existe rollback binario: para volver atrás debe restaurarse la revisión previa con la API española.
