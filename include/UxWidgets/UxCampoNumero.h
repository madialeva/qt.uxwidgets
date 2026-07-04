#pragma once

#include "UxWidgets/UxCampo.h"

/**
 * Campo numérico con precisión configurable: número de dígitos enteros y
 * decimales admitidos, y separador de miles opcional. El contenido se alinea a
 * la derecha. El separador decimal es la coma (uso gallego/español); el de
 * miles, el punto.
 *
 * Con digitosDecimales == 0 se comporta como entero.
 */
class UxCampoNumero : public UxCampo
{
    Q_OBJECT
    Q_PROPERTY(int digitosEnteros READ digitosEnteros WRITE setDigitosEnteros)
    Q_PROPERTY(int digitosDecimales READ digitosDecimales WRITE setDigitosDecimales)
    Q_PROPERTY(bool separadorMiles READ separadorMiles WRITE setSeparadorMiles)

public:
    explicit UxCampoNumero(QWidget *parent = nullptr);

    int digitosEnteros() const { return m_digitosEnteros; }
    void setDigitosEnteros(int digitos);

    int digitosDecimales() const { return m_digitosDecimales; }
    void setDigitosDecimales(int digitos);

    bool separadorMiles() const { return m_separadorMiles; }
    void setSeparadorMiles(bool separador);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void reconstruirValidador();
    void reformatearConMiles();
    void quitarSeparadorMiles();

    int m_digitosEnteros = 9;
    int m_digitosDecimales = 0;
    bool m_separadorMiles = false;
};
