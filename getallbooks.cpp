//#include <string>
#include "getallbooks.h"
GetAllBooks::GetAllBooks(MainWindow *Window,
                         std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                         std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                         std::shared_ptr<ThreadingResourcesLight<UserData> > user):
MyTask{Window,socket,state,user}{

}
void GetAllBooks::run(){
    _socket->Resource_mtx.lock();
    std::string mess="CONTENT|ALLBOOKS";
    _socket->Resource->write(mess.c_str());
    _socket->Resource->waitForBytesWritten();
    bool end=false;
    while(!end){
        _socket->Resource->waitForReadyRead();
        QString Line=_socket->Resource->readLine().trimmed();
        auto Command=Line.split('|');
        if(Command[0]=="CONTENT"){
            Book book;
            book.Id=Command[1];
            book.Name=Command[2];
            book.Author=Command[3];
            book.Date=Command[4];
            if(Command[5]=="AVAILABLE")
                book.State=BookState::Available;
            else
                book.State=BookState::Unavailable;
        }
    }
}
