#include "UxWidgets/UxInputTexto.h"
#include "UxWidgets/UxCampoTexto.h"

UxInputTexto::UxInputTexto(const QString &etiqueta, QWidget *parent)
    : UxInput(new UxCampoTexto, parent)
{
    setLabelText(etiqueta);
}

UxCampoTexto *UxInputTexto::campoTexto() const
{
    return static_cast<UxCampoTexto *>(campo());
}
