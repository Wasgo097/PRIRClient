#include "order.h"
#include <QMessageBox>
Order::Order(MainWindow *Window,
             std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
             std::shared_ptr<ThreadingResourcesLight<UserState> > state,
             std::shared_ptr<ThreadingResourcesLight<UserData> > user):
    MyTask{Window,socket,state,user}{
}
void Order::run(){
//    _socket->Resource_mtx.lock();
//    QString current_book_id=Window->get_current_book().Id;
//    std::string mess="ORDER|"+current_book_id.toStdString();
//    _socket->Resource->write(mess.c_str());
//    _socket->Resource->waitForBytesWritten();
//    _socket->Resource->waitForReadyRead();
//    QString read=_socket->Resource->readLine().trimmed();
//    auto list=read.split('|');
//    if(list[0]=="ORDER"&&list[1]=="TRUE"){
//        QMessageBox msg(Window);
//        msg.setIcon(QMessageBox::Information);
//        msg.setText("Udało sie zamówić książkę!");
//        msg.setWindowTitle("Zamówienie");
//        msg.setDetailedText("Miłej Lektury!");
//        msg.setStandardButtons(QMessageBox::Ok);
//        msg.exec();
//    }
//    else{
//        QMessageBox msg(Window);
//        msg.setIcon(QMessageBox::Information);
//        msg.setText("Nie udało sie zamówić książkę!");
//        msg.setWindowTitle("Zamówienie");
//        msg.setStandardButtons(QMessageBox::Ok);
//        msg.exec();
//    }
//    _socket->Resource_mtx.unlock();
}
