#pragma once

#include "UxWidgets/UxCampo.h"

/**
 * Campo de fecha sobre un QLineEdit (no QDateEdit), con formato dd/MM/yyyy.
 * Admite valor vacío sin señalar error cuando no es obligatorio, algo que
 * QDateEdit no permite con naturalidad. Muestra siempre un icono de calendario
 * que, al pulsarlo, despliega un QCalendarWidget y escribe la fecha elegida.
 */
class UxCampoFecha : public UxCampo
{
    Q_OBJECT

public:
    /// Formato de fecha usado por el control.
    static constexpr const char *kFormato = "dd/MM/yyyy";

    explicit UxCampoFecha(QWidget *parent = nullptr);

protected:
    void alActivarAccion() override; // abre el calendario en lugar de emitir seleccionSolicitada()

private:
    void mostrarCalendario();
};
