#pragma once

#include <QWidget>
#include <QString>

class QLabel;
class UxCampo;

/**
 * Control compuesto: etiqueta (QLabel) + un campo de la capa de campo (UxCampo)
 * como una sola unidad, para no crear y alinear tres widgets por campo. La
 * etiqueta puede situarse a la izquierda o arriba. Reenvía al campo interno las
 * propiedades relevantes y reemite su señal de selección.
 */
class UxInput : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)
    Q_PROPERTY(PosicionEtiqueta labelPosition READ labelPosition WRITE setLabelPosition)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(bool requerido READ requerido WRITE setRequerido)

public:
    enum PosicionEtiqueta { Izquierda, Arriba };
    Q_ENUM(PosicionEtiqueta)

    /// Toma posesión de `campo` (lo reparenta dentro del compuesto).
    explicit UxInput(UxCampo *campo, QWidget *parent = nullptr);

    UxCampo *campo() const { return m_campo; }

    QString labelText() const;
    void setLabelText(const QString &texto);

    PosicionEtiqueta labelPosition() const { return m_posicion; }
    void setLabelPosition(PosicionEtiqueta posicion);

    // Reenvíos al campo interno.
    QString text() const;
    void setText(const QString &texto);
    int maxLength() const;
    void setMaxLength(int longitud);
    bool requerido() const;
    void setRequerido(bool requerido);

signals:
    void seleccionSolicitada();

private:
    void reconstruirLayout();

    QLabel *m_etiqueta = nullptr;
    UxCampo *m_campo = nullptr;
    PosicionEtiqueta m_posicion = Izquierda;
};
