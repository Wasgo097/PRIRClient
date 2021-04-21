#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <vector>
#include <QMainWindow>
#include <QTimer>
#include "Utilities.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QTcpSocket;
class GetAllBooks;
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clear_all_books();
    void clear_my_books();
    void fill_all_books(const QStringList & List);
    void fill_my_books(const QStringList & List);
public:
    std::shared_ptr<ThreadingResources<std::vector<Book>>> MYBOOKS;
    std::shared_ptr<ThreadingResources<std::vector<Book>>> ALLBOOOKS;
private slots:
    void on_login_clicked();
    void on_reg_clicked();
    void on_allbooks_currentRowChanged(int currentRow);
    void on_mybooks_currentRowChanged(int currentRow);
    void on_tabWidget_currentChanged(int index);
    void on_btn_return_clicked();
    void on_btn_orderorreserve_clicked();
public slots:
    void sent_data(QString data);
private:
    std::mutex ui_mtx;
    Ui::MainWindow *ui;
    QTimer _get_all_books_timer;
    std::shared_ptr<ThreadingResources<QTcpSocket>> _socket;
    std::shared_ptr<ThreadingResourcesLight<UserState>> _state;
    std::shared_ptr<ThreadingResourcesLight<UserData>> _user;
    Book * _selected_book=nullptr;
    Book *_selected_my_book=nullptr;
};
#endif // MAINWINDOW_H
