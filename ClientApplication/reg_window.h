#ifndef REG_WINDOW_H
#define REG_WINDOW_H

#include <QWidget>

namespace Ui {
class reg_window;
}

class reg_window : public QWidget
{
    Q_OBJECT

public:
    explicit reg_window(QWidget *parent = nullptr);
    ~reg_window();

private slots:
    void on_lineEdit_Name_textEdited(const QString &arg1);

    void on_lineEdit_Password_2_textEdited(const QString &arg1);

    void on_lineEdit_Confirm_3_textEdited(const QString &arg1);

    void on_pushButtonRegister_clicked();

private:
    Ui::reg_window *ui;
};

#endif // REG_WINDOW_H
