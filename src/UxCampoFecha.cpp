#include "UxWidgets/UxCampoFecha.h"

#include <QApplication>
#include <QCalendarWidget>
#include <QColor>
#include <QDate>
#include <QIcon>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPoint>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QWidget>

namespace {

// Icono de calendario dibujado a mano (una hoja con cabecera), con el color de
// texto de la paleta para ser visible en tema claro y oscuro.
QIcon iconoCalendario()
{
    QPixmap pm(16, 16);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    const QColor color = QApplication::palette().color(QPalette::Text);
    QPen lapiz(color);
    lapiz.setWidthF(1.2);
    p.setPen(lapiz);
    p.setBrush(Qt::NoBrush);

    const QRect cuerpo(2, 3, 12, 11);
    p.drawRoundedRect(cuerpo, 1.5, 1.5);
    // Cabecera rellena.
    p.setBrush(color);
    p.drawRect(QRect(2, 3, 12, 3));
    // Anillas.
    p.setPen(lapiz);
    p.drawLine(5, 1, 5, 4);
    p.drawLine(11, 1, 11, 4);

    return QIcon(pm);
}

} // namespace

UxCampoFecha::UxCampoFecha(QWidget *parent)
    : UxCampo(parent)
{
    setPlaceholderText(QStringLiteral("dd/mm/aaaa"));

    // Validador tolerante que permite el tecleo progresivo y el valor vacío:
    // "", "1", "12", "12/", "12/0", "12/03/", "12/03/2024".
    const QString patron =
        QStringLiteral("^(\\d{0,2})(/(\\d{0,2})(/(\\d{0,4})?)?)?$");
    setValidator(new QRegularExpressionValidator(QRegularExpression(patron), this));

    // Icono de calendario siempre presente; al activarlo abre el popup.
    setIcono(iconoCalendario());
    setMostrarSelector(true);
}

void UxCampoFecha::alActivarAccion()
{
    mostrarCalendario();
}

void UxCampoFecha::mostrarCalendario()
{
    auto *popup = new QWidget(this, Qt::Popup);
    auto *disposicion = new QVBoxLayout(popup);
    disposicion->setContentsMargins(0, 0, 0, 0);

    auto *calendario = new QCalendarWidget(popup);
    const QDate actual = QDate::fromString(text(), QString::fromLatin1(kFormato));
    if (actual.isValid())
        calendario->setSelectedDate(actual);
    disposicion->addWidget(calendario);

    connect(calendario, &QCalendarWidget::clicked, this,
            [this, popup](const QDate &fecha) {
                setText(fecha.toString(QString::fromLatin1(kFormato)));
                popup->close();
                popup->deleteLater();
            });

    popup->move(mapToGlobal(QPoint(0, height())));
    popup->show();
}
