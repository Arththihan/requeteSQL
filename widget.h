#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget; // déclaration de la classe Widget
}

class Widget : public QWidget // héritage de QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0); // constructeur de la classe Widget
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif //WIDGET_H
