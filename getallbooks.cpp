#include "getallbooks.h"
#include <QStringList>
GetAllBooks::GetAllBooks(MainWindow *Window,
                         std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
                         std::shared_ptr<ThreadingResourcesLight<UserState>> state,
                         std::shared_ptr<ThreadingResourcesLight<UserData>> user):
MyTask{Window,socket,state,user}{
    connect(this,&GetAllBooks::sent_data,Window,&MainWindow::sent_data);
}
GetAllBooks::~GetAllBooks(){
    disconnect(this,&GetAllBooks::sent_data,Window,&MainWindow::sent_data);
}
void GetAllBooks::run(){
    Window->ALLBOOOKS->Resource_mtx.lock();
    Window->ALLBOOOKS->Resource->clear();
    Window->ALLBOOOKS->Resource_mtx.unlock();
    Window->clear_all_books();
    //_socket->Resource_mtx.lock();
    QString mess="CONTENT|ALLBOOKS";
   // _socket->Resource->write(mess.c_str());
    //_socket->Resource->waitForBytesWritten();
    emit sent_data(mess);
    connect(_socket->Resource.get(),&QTcpSocket::readyRead,Window,&MainWindow::read_allbooks);
    //_socket->Resource_mtx.unlock();
    this->quit();
}
