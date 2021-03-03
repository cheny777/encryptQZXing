#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QZXing"
#include <string>
#include <QCryptographicHash>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    QString m_strmessage;

    string encryption(string str);
};
#endif // WIDGET_H
