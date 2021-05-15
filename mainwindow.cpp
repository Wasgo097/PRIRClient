#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTcpSocket>
typedef  std::string string;
//#define test
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    _state=std::shared_ptr<ThreadingResourcesLight<UserState>>(new ThreadingResourcesLight<UserState>);
    _socket=std::shared_ptr<ThreadingResources<QTcpSocket>>(new ThreadingResources<QTcpSocket>);
    _socket->Resource=std::shared_ptr<QTcpSocket>(new QTcpSocket(this));
    _socket->Resource->connectToHost(QString("127.0.0.1"),7654);
    if(_socket->Resource->waitForConnected()){
        _state->Resource=UserState::Connected;
        //qDebug()<<"Polaczono";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setText("Udało sie połączyć z serwerem");
        msg.setWindowTitle("Stan połączenia");
        //msg.setDetailedText("Wyłącz aplikacje i upewnij się że serwer działa");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ALLBOOOKS=std::shared_ptr<ThreadingResources<std::vector<Book>>>(new ThreadingResources<std::vector<Book>>);
        ALLBOOOKS->Resource=std::shared_ptr<std::vector<Book>>(new std::vector<Book>);
        MYBOOKS=std::shared_ptr<ThreadingResources<std::vector<Book>>>(new ThreadingResources<std::vector<Book>>);
        MYBOOKS->Resource=std::shared_ptr<std::vector<Book>>(new std::vector<Book>);
#ifdef test
        string fullmess="LOG|kpaluch|qwerty";
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
        }
#endif
    }
    else{
        _state->Resource=UserState::Unconnected;        
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Nie udało sie połączyć z serwerem");
        msg.setWindowTitle("Stan połączenia");
        msg.setDetailedText("Wyłącz aplikacje i upewnij się że serwer działa");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
MainWindow::~MainWindow(){
    _socket->Resource->write("EXIT");
    _socket->Resource->waitForBytesWritten();
    delete ui;
}
void MainWindow::clear_all_books(){
    //ui_mtx.lock();
    ui->allbooks->clear();
    //ui_mtx.unlock();
}
void MainWindow::clear_my_books(){
    //ui_mtx.lock();
    ui->mybooks->clear();
    //ui_mtx.unlock();
}
void MainWindow::fill_all_books(const QStringList &List){
    //ui_mtx.lock();
    ui->allbooks->addItems(List);
    //ui_mtx.unlock();
}
void MainWindow::fill_my_books(const QStringList &List){
    //ui_mtx.lock();
    ui->mybooks->addItems(List);
    //ui_mtx.unlock();
}
void MainWindow::on_login_clicked(){
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
                ui->loginname->clear();
                ui->loginpass->clear();
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
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Nie udało sie połączyć z serwerem");
        msg.setWindowTitle("Błąd połączenia");
        msg.setDetailedText("Wyłącz aplikacje i upewnij się że serwer działa");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
void MainWindow::on_reg_clicked(){
    if(_state->Resource==UserState::Connected){
        string login=ui->regname->text().toStdString();
        string password=ui->regpass->text().toStdString();
        string fullmess="REG|"+login+"|"+password;
        _socket->Resource->write(fullmess.c_str());
        _socket->Resource->waitForBytesWritten();
        if(_socket->Resource->waitForReadyRead()){
            QString read=_socket->Resource->readLine().trimmed();
            auto list=read.split('|');
            if(list.count()==2&&list[0]=="REG"&&list[1]=="TRUE"){
                ui->regname->clear();
                ui->regpass->clear();
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
    else if(_state->Resource==UserState::Logged){
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Jesteś zalogowany");
        msg.setWindowTitle("Błąd rejestracji");
        msg.setDetailedText("Wyloguj się");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    else{
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Nie udało sie połączyć z serwerem");
        msg.setWindowTitle("Błąd połączenia");
        msg.setDetailedText("Wyłącz aplikacje i upewnij się że serwer działa");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
void MainWindow::on_allbooks_currentRowChanged(int currentRow){
    if(currentRow>=0){
        qDebug()<<"Zmiana all books "<<currentRow<<" "<<(*ALLBOOOKS->Resource)[currentRow].ToQStr();
        //ALLBOOOKS->Resource_mtx.lock();
        _selected_book=&(*ALLBOOOKS->Resource)[currentRow];
        //ALLBOOOKS->Resource_mtx.unlock();
    }
    else
        _selected_book=nullptr;
}
void MainWindow::on_mybooks_currentRowChanged(int currentRow){
    if(currentRow>=0){
        qDebug()<<"Zmiana my books "<<currentRow<<" "<<(*MYBOOKS->Resource)[currentRow].ToQStr();
        //MYBOOKS->Resource_mtx.lock();
        _selected_my_book=&(*MYBOOKS->Resource)[currentRow];
        //MYBOOKS->Resource_mtx.unlock();
    }
    else
        _selected_my_book=nullptr;
}
void MainWindow::on_tabWidget_currentChanged(int index){
    if(_state->Resource==UserState::Logged){
        //qDebug()<<"Zmiana";
        _selected_book=nullptr;
        _selected_my_book=nullptr;
        ALLBOOOKS->Resource->clear();
        MYBOOKS->Resource->clear();
        clear_all_books();
        clear_my_books();
        if(index==1){
            std::string mess="CONTENT|ALLBOOKS";
            _socket->Resource->write(mess.c_str());
            _socket->Resource->waitForBytesWritten();
            QStringList list;
            while(_socket->Resource->waitForReadyRead(2000)){
                QString Line=_socket->Resource->readLine().trimmed();
                auto Command=Line.split('|');
                if(Command[0]=="CONTENT"){
                    if(Command[1]=="END"){
                       break;
                    }
                    else{
                        Book book;
                        book.Id=Command[1];
                        book.Name=Command[2];
                        book.Author=Command[3];
                        book.Date=Command[4];
                        if(Command[5]=="TRUE")
                            book.State=BookState::Available;
                        else
                            book.State=BookState::Unavailable;
                        ALLBOOOKS->Resource->push_back(book);
                        list.append(book.ToQStrPl());
                    }
                }
                else
                    break;
            }
            fill_all_books(list);
        }
        else if (index==2){
            std::string mess="CONTENT|MYBOOKS";
            _socket->Resource->write(mess.c_str());
            _socket->Resource->waitForBytesWritten();
            QStringList list;
            while(_socket->Resource->waitForReadyRead(2000)){
                QString Line=_socket->Resource->readLine().trimmed();
                auto Command=Line.split('|');
                if(Command[0]=="CONTENT"){
                    if(Command[1]=="END"){
                        break;
                    }
                    else{
                        Book book;
                        book.Id=Command[1];
                        book.Name=Command[2];
                        book.Author=Command[3];
                        book.Date=Command[4];
                        MYBOOKS->Resource->push_back(book);
                        list.append(book.ToQStr());
                    }
                }
                else
                    break;
            }
            fill_my_books(list);
        }
    }
}
//void MainWindow::sent_data(QString data){
//    _socket->Resource->write(data.toStdString().c_str());
//    _socket->Resource->waitForBytesWritten();
//}
void MainWindow::on_btn_return_clicked(){
    if(_selected_my_book!=nullptr){
        std::string mess="RETURN|"+_selected_my_book->Id.toStdString();
        _socket->Resource->write(mess.c_str());
        _socket->Resource->waitForBytesWritten();
        if(_socket->Resource->waitForReadyRead()){
            QString line=_socket->Resource->readLine().trimmed();
            auto list=line.split('|');
            if(list[0]=="RETURN"){
                if(list[1]=="TRUE"){
                    //qDebug()<<"Udalo sie oddac ksiazke, odsiwez zakladki";
                    QMessageBox msg(this);
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Udalo sie oddac ksiazke, odśwież zakladki");
                    msg.setWindowTitle("Oddanie książki");
                    //msg.setDetailedText("Przed dokonaniem operacji musisz");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                //false or other
                else{
                    //qDebug()<<"Nie udalo sie oddac, odswiez zakladki";
                    QMessageBox msg(this);
                    msg.setIcon(QMessageBox::Warning);
                    msg.setText("Nie udalo sie oddac, odswiez zakladki");
                    msg.setWindowTitle("Oddanie książki");
                    //msg.setDetailedText("Przed dokonaniem operacji musisz");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
            }
            //first element isnt return
            else{
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Serwer nie mógł przetworzyć zapytania");
                msg.setWindowTitle("Błąd połączenia");
                msg.setDetailedText("Zresetuj aplikacje");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        //socket cant read
        else{
            //qDebug()<<"serwer nie odpowiada";
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Serwer nie odpowiada");
            msg.setWindowTitle("Błąd połączenia");
            msg.setDetailedText("Zresetuj aplikacje");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
    else{
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setText("Wybierz ksiażkę do oddania");
        msg.setWindowTitle("Brak wybranej książki");
        msg.setDetailedText("Przed dokonaniem operacji musisz wybrać ksiazke");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
void MainWindow::on_btn_orderorreserve_clicked(){
    if(_selected_book!=nullptr){
        if(_selected_book->State==BookState::Available){
            std::string mess="ORDER|"+_selected_book->Id.toStdString();
            _socket->Resource->write(mess.c_str());
            _socket->Resource->waitForBytesWritten();
            if(_socket->Resource->waitForReadyRead()){
                QString line=_socket->Resource->readLine().trimmed();
                auto list=line.split('|');
                if(list[0]=="ORDER"){
                    if(list[1]=="TRUE"){
                        //qDebug()<<"Udalo sie zamowic ksiazke, odsiez zakladki";
                        QMessageBox msg(this);
                        msg.setIcon(QMessageBox::Information);
                        msg.setText("Udalo sie zamowic ksiazke, odśwież zakladki");
                        msg.setWindowTitle("Zamawianie książki");
                        //msg.setDetailedText("Przed dokonaniem operacji musisz");
                        msg.setStandardButtons(QMessageBox::Ok);
                        msg.exec();
                    }
                    //false or other
                    else{
                        //qDebug()<<"Nie udalo sie zamowic ksiazke,odswiez zakladki";
                        QMessageBox msg(this);
                        msg.setIcon(QMessageBox::Information);
                        msg.setText("Nie udalo sie zamowic ksiazki, odsiwez zakladki");
                        msg.setWindowTitle("Zamawianie książki");
                        //msg.setDetailedText("Przed dokonaniem operacji musisz");
                        msg.setStandardButtons(QMessageBox::Ok);
                        msg.exec();
                    }
                }
                //first element isnt order
                else{
                    //qDebug()<<"Blad podczas zamawiania";
                    QMessageBox msg(this);
                    msg.setIcon(QMessageBox::Warning);
                    msg.setText("Serwer nie mógł przetworzyć zapytania");
                    msg.setWindowTitle("Błąd połączenia");
                    msg.setDetailedText("Zresetuj aplikacje");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
            }
            //socket cant read
            else{
                //qDebug()<<"serwer nie odpowiada";
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Serwer nie odpowiada");
                msg.setWindowTitle("Błąd połączenia");
                msg.setDetailedText("Zresetuj aplikacje");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        //unavailable
        else{
            std::string mess="RESERVE|"+_selected_book->Id.toStdString();
            _socket->Resource->write(mess.c_str());
            _socket->Resource->waitForBytesWritten();
            //qDebug()<<"Wyslano żądanie rezerwacji, poczekaj na swoja kolej";
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setText("Wyslano żądanie rezerwacji, poczekaj na swoja kolej");
            msg.setWindowTitle("Rezerwacja książki książki");
            //msg.setDetailedText("Przed dokonaniem operacji musisz");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
    //selectedbook==nulltptr
    else{
        //qDebug()<<"Wybierz ksiazke do zamowienia/rezerwacji";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setText("Wybierz ksiazke do zamowienia/rezerwacji");
        msg.setWindowTitle("Brak wybranej książki");
        msg.setDetailedText("Przed dokonaniem operacji musisz wybrać ksiazke");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
