#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpSocket>

#include "Utilities.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_login_clicked();
    void on_reg_clicked();
private:
    Ui::MainWindow *ui;
    ThreadingResources<QTcpSocket> _socket;
    ThreadingResourcesLight<UserState> _state;
    ThreadingResourcesLight<UserData> _user;
private:
    void connect();
    void unconnect();
};
#endif // MAINWINDOW_H
