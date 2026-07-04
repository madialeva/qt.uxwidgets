#include "UxWidgets/UxCampoNumero.h"

#include <QFocusEvent>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>

UxCampoNumero::UxCampoNumero(QWidget *parent)
    : UxCampo(parent)
{
    setAlignment(Qt::AlignRight);
    reconstruirValidador();
}

void UxCampoNumero::setDigitosEnteros(int digitos)
{
    m_digitosEnteros = qMax(1, digitos);
    reconstruirValidador();
}

void UxCampoNumero::setDigitosDecimales(int digitos)
{
    m_digitosDecimales = qMax(0, digitos);
    reconstruirValidador();
}

void UxCampoNumero::setSeparadorMiles(bool separador)
{
    m_separadorMiles = separador;
    if (!separador)
        quitarSeparadorMiles();
    else if (!hasFocus())
        reformatearConMiles();
}

void UxCampoNumero::reconstruirValidador()
{
    // La coma es el separador decimal. El separador de miles (punto) no se
    // teclea: se aplica al perder el foco, por eso no entra en el validador.
    QString patron;
    if (m_digitosDecimales > 0)
        patron = QStringLiteral("^\\d{0,%1}([,]\\d{0,%2})?$")
                     .arg(m_digitosEnteros)
                     .arg(m_digitosDecimales);
    else
        patron = QStringLiteral("^\\d{0,%1}$").arg(m_digitosEnteros);

    // La cadena vacía casa con el patrón (0 dígitos) => se permite vacío.
    setValidator(new QRegularExpressionValidator(QRegularExpression(patron), this));
}

void UxCampoNumero::quitarSeparadorMiles()
{
    QString t = text();
    if (!t.contains(QLatin1Char('.')))
        return;
    t.remove(QLatin1Char('.'));
    QSignalBlocker bloqueo(this);
    setText(t);
}

void UxCampoNumero::reformatearConMiles()
{
    if (!m_separadorMiles)
        return;

    const QString t = text().trimmed();
    if (t.isEmpty())
        return;

    const int coma = t.indexOf(QLatin1Char(','));
    QString parteEntera = (coma >= 0) ? t.left(coma) : t;
    const QString parteDecimal = (coma >= 0) ? t.mid(coma + 1) : QString();
    parteEntera.remove(QLatin1Char('.'));

    QString agrupada;
    int cuenta = 0;
    for (int i = parteEntera.size() - 1; i >= 0; --i) {
        agrupada.prepend(parteEntera.at(i));
        if (++cuenta % 3 == 0 && i > 0)
            agrupada.prepend(QLatin1Char('.'));
    }

    QString resultado = agrupada;
    if (coma >= 0)
        resultado += QLatin1Char(',') + parteDecimal;

    QSignalBlocker bloqueo(this);
    setText(resultado);
}

void UxCampoNumero::focusInEvent(QFocusEvent *event)
{
    // Al editar se quita el separador de miles para no estorbar al validador.
    if (m_separadorMiles)
        quitarSeparadorMiles();
    // Encadena a UxCampo (no a QLineEdit) para que aplique el resaltado de foco.
    UxCampo::focusInEvent(event);
}

void UxCampoNumero::focusOutEvent(QFocusEvent *event)
{
    reformatearConMiles();
    UxCampo::focusOutEvent(event);
}
