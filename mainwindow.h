#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "Utilities.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QTcpSocket;
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clear_all_books();
    void clear_my_books();
private slots:
    void on_login_clicked();
    void on_reg_clicked();
private:
    std::mutex ui_mtx;
    Ui::MainWindow *ui;
    std::shared_ptr<ThreadingResources<QTcpSocket>> _socket;
    std::shared_ptr<ThreadingResourcesLight<UserState>> _state;
    std::shared_ptr<ThreadingResourcesLight<UserData>> _user;
};
#endif // MAINWINDOW_H
