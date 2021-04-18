#include "returnbook.h"
#include <QMessageBox>
ReturnBook::ReturnBook(MainWindow *Window,
                        std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                        std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                        std::shared_ptr<ThreadingResourcesLight<UserData> > user):
    MyTask{Window,socket,state,user}{
}
void ReturnBook::run(){
    _socket->Resource_mtx.lock();
    QString current_my_book_id=Window->get_current_my_book().Id;
    std::string mess="RETURN|"+current_my_book_id.toStdString()+"\r\n";
    _socket->Resource->write(mess.c_str());
    _socket->Resource->waitForBytesWritten();
    _socket->Resource->waitForReadyRead();
    QString read=_socket->Resource->readLine().trimmed();
    auto list=read.split('|');
    if(list[0]=="RETURN"&&list[1]=="TRUE"){
        QMessageBox msg(Window);
        msg.setIcon(QMessageBox::Information);
        msg.setText("Udało sie zwrócić książkę!");
        msg.setWindowTitle("Zwrot");
        msg.setDetailedText("Zapraszamy do skorzystania z innych książek!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    else{
        QMessageBox msg(Window);
        msg.setIcon(QMessageBox::Information);
        msg.setText("Nie udało sie zwrócić książki!");
        msg.setWindowTitle("Zwrot");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    _socket->Resource_mtx.unlock();
}
