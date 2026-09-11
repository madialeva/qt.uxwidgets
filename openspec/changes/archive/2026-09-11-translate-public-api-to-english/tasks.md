## 1. API pública

- [x] 1.1 Renombrar las cabeceras y clases de la capa de campo a inglés, incluidas propiedades, métodos, señales y valores de enumerados; verificar que no queden nombres públicos españoles con `rg`.
- [x] 1.2 Renombrar las cabeceras y clases compuestas a inglés, incluidas propiedades, métodos, señales y valores de enumerados; verificar que las inclusiones y declaraciones anticipadas usen los nuevos nombres.
- [x] 1.3 Actualizar CMake para las nuevas rutas de cabeceras y fuentes; verificar una configuración limpia con CMake y Ninja.

## 2. Implementación y demostración

- [x] 2.1 Traducir los identificadores privados y los comentarios de las implementaciones de la capa de campo; verificar la compilación de la librería.
- [x] 2.2 Traducir los identificadores privados y los comentarios de las implementaciones compuestas; verificar la compilación de la librería.
- [x] 2.3 Actualizar la demo para usar la API inglesa y traducir sus variables, comentarios y textos de interfaz; verificar la compilación de `UxWidgetsDemo`.

## 3. Documentación y validación

- [x] 3.1 Actualizar README para referirse a la API inglesa y conservar los ejemplos compilables; verificar que no haya nombres públicos españoles obsoletos.
- [x] 3.2 Establecer en AGENTS.md el convenio de inglés para código, identificadores, comentarios y textos de demo, con las excepciones de OpenSpec y diálogo; verificar el contenido de la guía.
- [x] 3.3 Configurar y compilar desde un directorio Linux limpio y ejecutar comprobaciones de `rg` para identificar símbolos españoles eliminados; verificar que CMake y Ninja finalicen sin errores.
