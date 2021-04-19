#include "reserve.h"
Reserve::Reserve(MainWindow *Window,
                 std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                 std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                 std::shared_ptr<ThreadingResourcesLight<UserData> > user):
    MyTask{Window,socket,state,user}{
}
void Reserve::run(){
    _socket->Resource_mtx.lock();
    QString current_book_id=Window->get_current_book().Id;
    std::string mess="RESERVE|"+current_book_id.toStdString();
    _socket->Resource->write(mess.c_str());
    _socket->Resource->waitForBytesWritten();
    _socket->Resource_mtx.unlock();
}
