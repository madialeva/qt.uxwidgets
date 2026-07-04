#include "UxWidgets/UxCampoTexto.h"

#include <QValidator>

namespace {

// Validador que convierte a mayúsculas cuanto se teclea o pega, preservando la
// posición del cursor (validate() puede modificar el texto de entrada). Es la
// forma idiomática en Qt de forzar mayúsculas sin recolocar el cursor al final.
class ValidadorMayusculas : public QValidator
{
public:
    using QValidator::QValidator;

    State validate(QString &input, int & /*pos*/) const override
    {
        input = input.toUpper();
        return Acceptable;
    }
};

} // namespace

UxCampoTexto::UxCampoTexto(QWidget *parent)
    : UxCampo(parent)
{
}

void UxCampoTexto::setMayusculas(bool mayusculas)
{
    m_mayusculas = mayusculas;
    aplicarValidador();
}

void UxCampoTexto::aplicarValidador()
{
    if (m_mayusculas) {
        setValidator(new ValidadorMayusculas(this));
        // Convierte también lo que ya hubiera escrito.
        setText(text().toUpper());
    } else {
        setValidator(nullptr);
    }
}
