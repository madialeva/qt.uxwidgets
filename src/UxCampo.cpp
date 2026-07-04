#include "UxWidgets/UxCampo.h"

#include <QAction>
#include <QApplication>
#include <QColor>
#include <QEvent>
#include <QFocusEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPoint>

namespace {

// Icono por defecto del botón selector: puntos suspensivos "…", el affordance
// convencional de "pulsa para abrir un diálogo y elegir". Se pinta con el color
// de texto de la paleta para que sea visible en tema claro y oscuro.
QIcon iconoElipsisPorDefecto()
{
    QPixmap pm(16, 16);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    const QColor color = QApplication::palette().color(QPalette::Text);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    for (int i = 0; i < 3; ++i)
        p.drawEllipse(QPoint(3 + i * 5, 11), 1, 1);

    return QIcon(pm);
}

} // namespace

UxCampo::UxCampo(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this, &QLineEdit::textChanged, this, [this] { actualizarFondo(); });
}

UxCampo::~UxCampo() = default;

void UxCampo::setRequerido(bool requerido)
{
    m_requerido = requerido;
    actualizarFondo();
}

void UxCampo::asegurarAccion()
{
    if (m_accion)
        return;

    m_accion = new QAction(this);
    m_accion->setIcon(m_icono.isNull() ? iconoElipsisPorDefecto() : m_icono);
    connect(m_accion, &QAction::triggered, this, [this] { alActivarAccion(); });
}

void UxCampo::setMostrarSelector(bool mostrar)
{
    m_mostrarSelector = mostrar;
    if (mostrar) {
        asegurarAccion();
        // Evita duplicar la acción si se activa varias veces.
        removeAction(m_accion);
        addAction(m_accion, QLineEdit::TrailingPosition);
    } else if (m_accion) {
        removeAction(m_accion);
    }
}

void UxCampo::setIcono(const QIcon &icono)
{
    m_icono = icono;
    if (m_accion)
        m_accion->setIcon(icono.isNull() ? iconoElipsisPorDefecto() : icono);
}

void UxCampo::alActivarAccion()
{
    emit seleccionSolicitada();
}

void UxCampo::setColorFocoClaro(const QColor &color)
{
    m_colorFocoClaro = color;
    if (hasFocus())
        actualizarFondo();
}

void UxCampo::setColorFocoOscuro(const QColor &color)
{
    m_colorFocoOscuro = color;
    if (hasFocus())
        actualizarFondo();
}

void UxCampo::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    actualizarFondo();
}

void UxCampo::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    actualizarFondo();
}

void UxCampo::changeEvent(QEvent *event)
{
    // Si el tema de la aplicación cambia en caliente, reevaluar el fondo para
    // que el resaltado de foco use el color del nuevo tema. El guard evita la
    // reentrada provocada por nuestro propio setPalette (PaletteChange).
    if ((event->type() == QEvent::ApplicationPaletteChange
         || event->type() == QEvent::PaletteChange)
        && !m_enActualizacion) {
        actualizarFondo();
    }
    QLineEdit::changeEvent(event);
}

bool UxCampo::esTemaOscuro() const
{
    // Heurística estándar: si el color de texto del tema es claro, el tema es
    // oscuro. Se lee de la paleta (rol Text, que no reasignamos nosotros).
    return palette().color(QPalette::Text).lightness() > 128;
}

void UxCampo::actualizarFondo()
{
    QColor fondo;
    if (hasFocus())
        fondo = esTemaOscuro() ? m_colorFocoOscuro : m_colorFocoClaro;
    else if (m_requerido && text().trimmed().isEmpty())
        fondo = QColor(0xFF, 0xE4, 0xC4); // Bisque
    else
        fondo = QApplication::palette().color(QPalette::Base);

    QPalette pal = palette();
    pal.setColor(QPalette::Base, fondo);

    m_enActualizacion = true;
    setPalette(pal);
    m_enActualizacion = false;
}
