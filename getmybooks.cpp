#include "getmybooks.h"


GetMyBooks::GetMyBooks(MainWindow *Window,
                       std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                       std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                       std::shared_ptr<ThreadingResourcesLight<UserData> > user):
    MyTask{Window,socket,state,user}{
}
void GetMyBooks::run(){    
    _socket->Resource_mtx.lock();
    Window->MYBOOKS->Resource_mtx.lock();
    Window->clear_my_books();
    Window->MYBOOKS->Resource->clear();
    std::string mess="CONTENT|MYBOOKS";
    _socket->Resource->write(mess.c_str());
    _socket->Resource->waitForBytesWritten();
    bool end=false;
    QStringList list;
    while(!end){
        _socket->Resource->waitForReadyRead();
        QString Line=_socket->Resource->readLine().trimmed();
        auto Command=Line.split('|');
        if(Command[0]=="CONTENT"){
            if(Command[1]=="END"){
                end=true;
            }
            else{
                Book book;
                book.Id=Command[1];
                book.Name=Command[2];
                book.Author=Command[3];
                book.Date=Command[4];
                Window->MYBOOKS->Resource->push_back(book);
                list.append(book.ToQStr());
            }
        }
    }
    Window->fill_my_books(list);
    _socket->Resource_mtx.unlock();
    Window->MYBOOKS->Resource_mtx.unlock();
}
