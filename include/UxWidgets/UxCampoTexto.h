#pragma once

#include "UxWidgets/UxCampo.h"

/**
 * Campo de texto libre. La longitud máxima se controla con la propiedad
 * maxLength heredada de QLineEdit (setMaxLength). Opcionalmente fuerza que
 * todo lo tecleado y pegado aparezca en mayúsculas.
 */
class UxCampoTexto : public UxCampo
{
    Q_OBJECT
    Q_PROPERTY(bool mayusculas READ mayusculas WRITE setMayusculas)

public:
    explicit UxCampoTexto(QWidget *parent = nullptr);

    bool mayusculas() const { return m_mayusculas; }
    void setMayusculas(bool mayusculas);

private:
    void aplicarValidador();

    bool m_mayusculas = false;
};
