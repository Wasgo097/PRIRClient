#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTcpSocket>
#include "getallbooks.h"
#include "getmybooks.h"
#include "order.h"
#include "reserve.h"
#include "returnbook.h"
typedef  std::string string;
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    _state=std::shared_ptr<ThreadingResourcesLight<UserState>>(new ThreadingResourcesLight<UserState>);
    _socket=std::shared_ptr<ThreadingResources<QTcpSocket>>(new ThreadingResources<QTcpSocket>);
    _socket->Resource=std::shared_ptr<QTcpSocket>(new QTcpSocket(this));
    _socket->Resource->connectToHost(QString("127.0.0.1"),7654);
    if(_socket->Resource->waitForConnected()){
        _state->Resource=UserState::Connected;
        qDebug()<<"Polaczono";
        ALLBOOOKS=std::shared_ptr<ThreadingResources<std::vector<Book>>>(new ThreadingResources<std::vector<Book>>);
        ALLBOOOKS->Resource=std::shared_ptr<std::vector<Book>>(new std::vector<Book>);
        MYBOOKS=std::shared_ptr<ThreadingResources<std::vector<Book>>>(new ThreadingResources<std::vector<Book>>);
        MYBOOKS->Resource=std::shared_ptr<std::vector<Book>>(new std::vector<Book>);
    }
    else{
        _state->Resource=UserState::Unconnected;
        qDebug()<<"Nie polaczono";
    }
}
MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::clear_all_books(){
    ui_mtx.lock();
    ui->allbooks->clear();
    ui_mtx.unlock();
}
void MainWindow::clear_my_books(){
    ui_mtx.lock();
    ui->mybooks->clear();
    ui_mtx.unlock();
}
void MainWindow::fill_all_books(const QStringList &List){
    ui_mtx.lock();
    ui->allbooks->addItems(List);
    ui_mtx.unlock();
}
void MainWindow::fill_my_books(const QStringList &List){
    ui_mtx.lock();
    ui->mybooks->addItems(List);
    ui_mtx.unlock();
}
const Book MainWindow::get_current_book()const{
    return _current_book;
}
const Book MainWindow::get_current_my_book() const{
    return _my_current_book;
}
void MainWindow::on_login_clicked(){
//    _state->Resourc_mtx.lock();
//    _socket->Resource_mtx.lock();
    if(_state->Resource==UserState::Logged){
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Jesteś już zalogowany");
        msg.setWindowTitle("Błąd logowania");
        msg.setDetailedText("Wyloguj się");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    else if(_state->Resource==UserState::Connected){
        string login=ui->loginname->text().toStdString();
        string password=ui->loginpass->text().toStdString();
        string fullmess="LOG|"+login+"|"+password;
        _socket->Resource->write(fullmess.c_str());
        _socket->Resource->waitForBytesWritten();
        if(_socket->Resource->waitForReadyRead()){
            QString read=_socket->Resource->readLine().trimmed();
            auto list=read.split('|');
            if(list.count()==2&&list[0]=="LOG"&&list[1]=="TRUE"){
                _state->Resource=UserState::Logged;
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Information);
                msg.setText("Poprawnie zalogowano");
                msg.setWindowTitle("Proces logowania");
                msg.setDetailedText("Witamy w systemie bibliotecznym firmy XYZ");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
            else{
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Information);
                msg.setText("Nie udalo sie zalogowac, blad danych logowania");
                msg.setWindowTitle("Problem z logowaniem");
                msg.setDetailedText("Zresetuj aplikacje oraz wpisz poprawne dane");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        else{
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setText("Nie udalo sie zalogowac, serwer nie odeslal danych");
            msg.setWindowTitle("Problem z logowaniem");
            msg.setDetailedText("Zresetuj aplikacje/upewnic sie ze serwer dziala");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
    else{
        qDebug()<<"Socket nie polaczony";
    }
//    _state->Resourc_mtx.unlock();
//    _socket->Resource_mtx.unlock();
}
void MainWindow::on_reg_clicked(){
    _state->Resourc_mtx.lock();
    _socket->Resource_mtx.lock();
    if(_state->Resource==UserState::Unconnected){
        qDebug()<<"Socket nie polaczony";
    }
    else if(_state->Resource==UserState::Connected){
        string login=ui->regname->text().toStdString();
        string password=ui->regpass->text().toStdString();
        string fullmess="REG|"+login+"|"+password;
        _socket->Resource->write(fullmess.c_str());
        _socket->Resource->waitForBytesWritten();
        if(_socket->Resource->waitForReadyRead()){
            QString read=_socket->Resource->readLine().trimmed();
            auto list=read.split('|');
            if(list.count()==2&&list[0]=="REG"&&list[1]=="TRUE"){
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Information);
                msg.setText("Poprawnie zarejestrowano");
                msg.setWindowTitle("Proces rejestracji");
                msg.setDetailedText("Witamy w systemie bibliotecznym firmy XYZ");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
            else{
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Information);
                msg.setText("Nie udalo sie zalogowac, blad danych logowania");
                msg.setWindowTitle("Problem z logowaniem");
                msg.setDetailedText("Zresetuj aplikacje oraz wpisz poprawne dane");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        else{
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setText("Nie udalo sie zarejestrowac, serwer nie odeslal danych");
            msg.setWindowTitle("Problem z rejestracja");
            msg.setDetailedText("Zresetuj aplikacje/upewnic sie ze serwer dziala");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
    else{
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Jesteś zalogowany");
        msg.setWindowTitle("Błąd rejestracji");
        msg.setDetailedText("Wyloguj się");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    _state->Resourc_mtx.unlock();
    _socket->Resource_mtx.unlock();
}
void MainWindow::on_allbooks_currentRowChanged(int currentRow){
    ALLBOOOKS->Resource_mtx.lock();
    _current_book=(*ALLBOOOKS->Resource)[currentRow];
    ALLBOOOKS->Resource_mtx.unlock();
}
void MainWindow::on_mybooks_currentRowChanged(int currentRow){
    MYBOOKS->Resource_mtx.lock();
    _my_current_book=(*MYBOOKS->Resource)[currentRow];
    MYBOOKS->Resource_mtx.unlock();
}
void MainWindow::on_tabWidget_currentChanged(int index){
    qDebug()<<"Zmiana";
    if(index==1){
        //_socket->Resource_mtx.lock();
       // Window->ALLBOOOKS->Resource_mtx.lock();
        this->clear_all_books();
        this->ALLBOOOKS->Resource->clear();
        std::string mess="CONTENT|ALLBOOKS";
        _socket->Resource->write(mess.c_str());
        _socket->Resource->waitForBytesWritten();
        _socket->Resource->waitForReadyRead();
        bool end=false;
        QStringList list;
        while(!end){
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
                    this->ALLBOOOKS->Resource->push_back(book);
                    list.append(book.ToQStr());
                }
            }
        }
        this->fill_all_books(list);
        //_socket->Resource_mtx.unlock();
        //Window->ALLBOOOKS->Resource_mtx.unlock();
    }
    else if (index==2){

    }
}
