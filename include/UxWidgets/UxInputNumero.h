#pragma once

#include "UxWidgets/UxInput.h"

class UxCampoNumero;

/// Conveniencia: control compuesto con un campo numérico.
class UxInputNumero : public UxInput
{
    Q_OBJECT

public:
    explicit UxInputNumero(const QString &etiqueta = QString(), QWidget *parent = nullptr);

    /// Acceso tipado al campo para configurar precisión, separador de miles, etc.
    UxCampoNumero *campoNumero() const;
};
