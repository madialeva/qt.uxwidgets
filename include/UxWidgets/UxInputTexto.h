#pragma once

#include "UxWidgets/UxInput.h"

class UxCampoTexto;

/// Conveniencia: control compuesto con un campo de texto.
class UxInputTexto : public UxInput
{
    Q_OBJECT

public:
    explicit UxInputTexto(const QString &etiqueta = QString(), QWidget *parent = nullptr);

    /// Acceso tipado al campo para configurar mayúsculas, selector, etc.
    UxCampoTexto *campoTexto() const;
};
