#pragma once

#include "UxWidgets/UxInput.h"

class UxCampoFecha;

/// Conveniencia: control compuesto con un campo de fecha.
class UxInputFecha : public UxInput
{
    Q_OBJECT

public:
    explicit UxInputFecha(const QString &etiqueta = QString(), QWidget *parent = nullptr);

    /// Acceso tipado al campo de fecha.
    UxCampoFecha *campoFecha() const;
};
