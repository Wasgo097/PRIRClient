#include "getallbooks.h"
#include <QStringList>
GetAllBooks::GetAllBooks(MainWindow *Window,
                         std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                         std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                         std::shared_ptr<ThreadingResourcesLight<UserData> > user):
MyTask{Window,socket,state,user}{

}
void GetAllBooks::run(){
    _socket->Resource_mtx.lock();
    Window->ALLBOOOKS->Resource_mtx.lock();
    Window->clear_all_books();
    Window->ALLBOOOKS->Resource->clear();
    std::string mess="CONTENT|ALLBOOKS";
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
                if(Command[5]=="AVAILABLE")
                    book.State=BookState::Available;
                else
                    book.State=BookState::Unavailable;
                Window->ALLBOOOKS->Resource_mtx.lock();
                Window->ALLBOOOKS->Resource->push_back(book);
                list.append(book.ToQStr());
            }
        }
    }
    Window->fill_all_books(list);
    _socket->Resource_mtx.unlock();
    Window->ALLBOOOKS->Resource_mtx.unlock();
}
