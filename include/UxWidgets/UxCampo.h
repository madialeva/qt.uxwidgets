#pragma once

#include <QColor>
#include <QLineEdit>
#include <QIcon>

class QAction;

/**
 * Base de la capa de campo: un QLineEdit especializado con el comportamiento
 * común a todos los sabores (texto, número, fecha):
 *  - Botón de acción integrado al final del campo (addAction) con icono
 *    configurable (por defecto puntos suspensivos), que emite seleccionSolicitada().
 *  - Propiedad "requerido" con indicación visual cuando el campo está vacío.
 *  - Resaltado del fondo mientras el campo tiene el foco, para identificar de un
 *    vistazo el control activo. El color se adapta al tema: azul claro en tema
 *    claro, azul oscuro en tema oscuro (ambos configurables), detectado de forma
 *    dinámica y actualizado si el tema cambia en caliente.
 *
 * El saneado del pegado no requiere un punto de extensión propio: en QLineEdit
 * el validador y maxLength ya se aplican también al texto pegado, de modo que
 * cada sabor (mayúsculas, numérico, fecha) filtra el pegado con su validador.
 */
class UxCampo : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool requerido READ requerido WRITE setRequerido)
    Q_PROPERTY(bool mostrarSelector READ mostrarSelector WRITE setMostrarSelector)
    Q_PROPERTY(QIcon icono READ icono WRITE setIcono)
    Q_PROPERTY(QColor colorFocoClaro READ colorFocoClaro WRITE setColorFocoClaro)
    Q_PROPERTY(QColor colorFocoOscuro READ colorFocoOscuro WRITE setColorFocoOscuro)

public:
    explicit UxCampo(QWidget *parent = nullptr);
    ~UxCampo() override;

    bool requerido() const { return m_requerido; }
    void setRequerido(bool requerido);

    bool mostrarSelector() const { return m_mostrarSelector; }
    void setMostrarSelector(bool mostrar);

    QIcon icono() const { return m_icono; }
    void setIcono(const QIcon &icono);

    /// Color de fondo con el foco en tema claro (azul claro por defecto).
    QColor colorFocoClaro() const { return m_colorFocoClaro; }
    void setColorFocoClaro(const QColor &color);

    /// Color de fondo con el foco en tema oscuro (azul oscuro por defecto).
    QColor colorFocoOscuro() const { return m_colorFocoOscuro; }
    void setColorFocoOscuro(const QColor &color);

signals:
    /// Se solicita abrir un selector (habitualmente un diálogo modal de elección).
    /// El consumidor decide qué abrir y cómo devolver el valor al campo.
    void seleccionSolicitada();

protected:
    /// Punto de extensión: qué ocurre al activar el botón integrado del campo.
    /// Por defecto emite seleccionSolicitada(); la fecha lo redefine para abrir
    /// el calendario.
    virtual void alActivarAccion();

    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent *event) override; // reacciona al cambio de tema

    QAction *accionTrailing() const { return m_accion; }
    void asegurarAccion();

private:
    // Decide el color de fondo según prioridad: foco (según tema) > requerido y
    // vacío (bisque) > normal.
    void actualizarFondo();
    bool esTemaOscuro() const;

    bool m_requerido = false;
    bool m_mostrarSelector = false;
    bool m_enActualizacion = false; // evita reentrada al reasignar la paleta
    QIcon m_icono;
    QColor m_colorFocoClaro{0xCC, 0xE8, 0xFF};  // azul claro (tema claro)
    QColor m_colorFocoOscuro{0x14, 0x37, 0x5A}; // azul oscuro (tema oscuro)
    QAction *m_accion = nullptr;
};
