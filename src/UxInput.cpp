#include "UxWidgets/UxInput.h"
#include "UxWidgets/UxCampo.h"

#include <QBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

UxInput::UxInput(UxCampo *campo, QWidget *parent)
    : QWidget(parent)
    , m_etiqueta(new QLabel(this))
    , m_campo(campo)
{
    m_campo->setParent(this);
    connect(m_campo, &UxCampo::seleccionSolicitada,
            this, &UxInput::seleccionSolicitada);
    reconstruirLayout();
}

QString UxInput::labelText() const
{
    return m_etiqueta->text();
}

void UxInput::setLabelText(const QString &texto)
{
    m_etiqueta->setText(texto);
}

void UxInput::setLabelPosition(PosicionEtiqueta posicion)
{
    if (m_posicion == posicion)
        return;
    m_posicion = posicion;
    reconstruirLayout();
}

QString UxInput::text() const
{
    return m_campo->text();
}

void UxInput::setText(const QString &texto)
{
    m_campo->setText(texto);
}

int UxInput::maxLength() const
{
    return m_campo->maxLength();
}

void UxInput::setMaxLength(int longitud)
{
    m_campo->setMaxLength(longitud);
}

bool UxInput::requerido() const
{
    return m_campo->requerido();
}

void UxInput::setRequerido(bool requerido)
{
    m_campo->setRequerido(requerido);
}

void UxInput::reconstruirLayout()
{
    if (QLayout *anterior = layout()) {
        anterior->removeWidget(m_etiqueta);
        anterior->removeWidget(m_campo);
        delete anterior;
    }

    QBoxLayout *disposicion = (m_posicion == Arriba)
        ? static_cast<QBoxLayout *>(new QVBoxLayout)
        : static_cast<QBoxLayout *>(new QHBoxLayout);
    disposicion->setContentsMargins(0, 0, 0, 0);
    disposicion->addWidget(m_etiqueta);
    disposicion->addWidget(m_campo);
    setLayout(disposicion);
}
