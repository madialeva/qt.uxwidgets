#include "UxWidgets/UxInputFecha.h"
#include "UxWidgets/UxCampoFecha.h"

UxInputFecha::UxInputFecha(const QString &etiqueta, QWidget *parent)
    : UxInput(new UxCampoFecha, parent)
{
    setLabelText(etiqueta);
}

UxCampoFecha *UxInputFecha::campoFecha() const
{
    return static_cast<UxCampoFecha *>(campo());
}
