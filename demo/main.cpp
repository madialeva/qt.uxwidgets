// Demostración de UxWidgets: instancia los tres controles compuestos con la
// etiqueta a la izquierda y arriba, un campo requerido, y un botón selector
// conectado a un diálogo modal de ejemplo (elegir un analgésico).
//
// Sin argumentos usa el tema del sistema. Con --dark o --light fuerza una paleta
// oscura o clara (Fusion), útil para ver el resaltado de foco adaptado al tema.

#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QPalette>
#include <QVBoxLayout>
#include <QWidget>

#include "UxWidgets/UxCampoNumero.h"
#include "UxWidgets/UxCampoTexto.h"
#include "UxWidgets/UxInputFecha.h"
#include "UxWidgets/UxInputNumero.h"
#include "UxWidgets/UxInputTexto.h"

// Aplica una paleta oscura estándar sobre el estilo Fusion.
static void aplicarTemaOscuro(QApplication &app)
{
    app.setStyle(QStringLiteral("Fusion"));
    QPalette p;
    p.setColor(QPalette::Window, QColor(53, 53, 53));
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Base, QColor(35, 35, 35));
    p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::Highlight, QColor(42, 130, 218));
    p.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(p);
}

// Aplica una paleta clara estándar sobre el estilo Fusion.
static void aplicarTemaClaro(QApplication &app)
{
    app.setStyle(QStringLiteral("Fusion"));
    QPalette p;
    p.setColor(QPalette::Window, QColor(0xF0, 0xF0, 0xF0));
    p.setColor(QPalette::WindowText, Qt::black);
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::AlternateBase, QColor(0xF7, 0xF7, 0xF7));
    p.setColor(QPalette::ToolTipBase, Qt::black);
    p.setColor(QPalette::ToolTipText, Qt::black);
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Button, QColor(0xF0, 0xF0, 0xF0));
    p.setColor(QPalette::ButtonText, Qt::black);
    p.setColor(QPalette::Highlight, QColor(0x30, 0x8C, 0xC6));
    p.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(p);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (app.arguments().contains(QStringLiteral("--dark")))
        aplicarTemaOscuro(app);
    else if (app.arguments().contains(QStringLiteral("--light")))
        aplicarTemaClaro(app);

    QWidget ventana;
    ventana.setWindowTitle(QStringLiteral("Demo UxWidgets"));
    auto *disposicion = new QVBoxLayout(&ventana);

    // --- Etiqueta a la izquierda -------------------------------------------
    auto *grupoIzq = new QGroupBox(QStringLiteral("Etiqueta a la izquierda"), &ventana);
    auto *formIzq = new QVBoxLayout(grupoIzq);

    auto *nombre = new UxInputTexto(QStringLiteral("Nombre"));
    nombre->campoTexto()->setMayusculas(true);
    nombre->setMaxLength(20);
    nombre->setRequerido(true); // requerido: se resalta al estar vacío
    formIzq->addWidget(nombre);

    auto *importe = new UxInputNumero(QStringLiteral("Importe"));
    importe->campoNumero()->setDigitosEnteros(6);
    importe->campoNumero()->setDigitosDecimales(2);
    importe->campoNumero()->setSeparadorMiles(true);
    formIzq->addWidget(importe);

    auto *analgesico = new UxInputTexto(QStringLiteral("Analgésico"));
    analgesico->setRequerido(true); // requerido y sin foco al abrir => fondo bisque visible
    analgesico->campoTexto()->setMostrarSelector(true); // botón selector "…"
    QObject::connect(analgesico, &UxInput::seleccionSolicitada, &ventana, [&ventana, analgesico] {
        const int r = QMessageBox::question(
            &ventana, QStringLiteral("Seleccionar analgésico"),
            QStringLiteral("Aquí se abriría el diálogo de elección.\n¿Elegir «Paracetamol»?"));
        if (r == QMessageBox::Yes)
            analgesico->setText(QStringLiteral("Paracetamol"));
    });
    formIzq->addWidget(analgesico);

    disposicion->addWidget(grupoIzq);

    // --- Etiqueta arriba ----------------------------------------------------
    auto *grupoArriba = new QGroupBox(QStringLiteral("Etiqueta arriba"), &ventana);
    auto *formArriba = new QVBoxLayout(grupoArriba);

    auto *fecha = new UxInputFecha(QStringLiteral("Fecha de nacimiento"));
    fecha->setLabelPosition(UxInput::Arriba); // fecha vacía admitida (no obligatoria)
    formArriba->addWidget(fecha);

    disposicion->addWidget(grupoArriba);

    ventana.resize(360, 260);
    ventana.show();
    return app.exec();
}
