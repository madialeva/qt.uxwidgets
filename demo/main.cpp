// UxWidgets demo: instantiates the three composite controls with labels on the
// left and above, a required field, and a selection button connected to an
// example modal dialog.
//
// It uses the system theme by default. --dark and --light force Fusion palettes
// to demonstrate theme-aware focus highlighting.

#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QPalette>
#include <QVBoxLayout>
#include <QWidget>

#include "UxWidgets/UxDateInput.h"
#include "UxWidgets/UxNumberField.h"
#include "UxWidgets/UxNumberInput.h"
#include "UxWidgets/UxTextField.h"
#include "UxWidgets/UxTextInput.h"

static void applyDarkTheme(QApplication &app)
{
    app.setStyle(QStringLiteral("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(35, 35, 35));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(palette);
}

static void applyLightTheme(QApplication &app)
{
    app.setStyle(QStringLiteral("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0xF0, 0xF0, 0xF0));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, QColor(0xF7, 0xF7, 0xF7));
    palette.setColor(QPalette::ToolTipBase, Qt::black);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(0xF0, 0xF0, 0xF0));
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::Highlight, QColor(0x30, 0x8C, 0xC6));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(palette);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (app.arguments().contains(QStringLiteral("--dark")))
        applyDarkTheme(app);
    else if (app.arguments().contains(QStringLiteral("--light")))
        applyLightTheme(app);

    QWidget window;
    window.setWindowTitle(QStringLiteral("UxWidgets Demo"));
    auto *layout = new QVBoxLayout(&window);

    auto *leftGroup = new QGroupBox(QStringLiteral("Label on the left"), &window);
    auto *leftForm = new QVBoxLayout(leftGroup);

    auto *name = new UxTextInput(QStringLiteral("Name"));
    name->textField()->setUppercase(true);
    name->setMaxLength(20);
    name->setRequired(true);
    leftForm->addWidget(name);

    auto *amount = new UxNumberInput(QStringLiteral("Amount"));
    amount->numberField()->setIntegerDigits(6);
    amount->numberField()->setDecimalDigits(2);
    amount->numberField()->setThousandsSeparator(true);
    leftForm->addWidget(amount);

    auto *analgesic = new UxTextInput(QStringLiteral("Analgesic"));
    analgesic->setRequired(true);
    analgesic->textField()->setShowSelector(true);
    QObject::connect(analgesic, &UxInput::selectionRequested, &window, [&window, analgesic] {
        const int response = QMessageBox::question(
            &window, QStringLiteral("Select analgesic"),
            QStringLiteral("The selection dialog would open here.\nSelect \"Paracetamol\"?"));
        if (response == QMessageBox::Yes)
            analgesic->setText(QStringLiteral("Paracetamol"));
    });
    leftForm->addWidget(analgesic);

    layout->addWidget(leftGroup);

    auto *aboveGroup = new QGroupBox(QStringLiteral("Label above"), &window);
    auto *aboveForm = new QVBoxLayout(aboveGroup);

    auto *date = new UxDateInput(QStringLiteral("Date of birth"));
    date->setLabelPosition(UxInput::Above);
    aboveForm->addWidget(date);

    layout->addWidget(aboveGroup);

    window.resize(360, 260);
    window.show();
    return app.exec();
}
