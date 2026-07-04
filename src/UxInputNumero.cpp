#include "UxWidgets/UxInputNumero.h"
#include "UxWidgets/UxCampoNumero.h"

UxInputNumero::UxInputNumero(const QString &etiqueta, QWidget *parent)
    : UxInput(new UxCampoNumero, parent)
{
    setLabelText(etiqueta);
}

UxCampoNumero *UxInputNumero::campoNumero() const
{
    return static_cast<UxCampoNumero *>(campo());
}
