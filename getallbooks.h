#ifndef GETALLBOOKS_H
#define GETALLBOOKS_H

#include "mytask.h"
#include <QTimer>
class GetAllBooks : public MyTask
{
    Q_OBJECT
public:
    GetAllBooks(MainWindow * Window,
                std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
                std::shared_ptr<ThreadingResourcesLight<UserState>> state,
                std::shared_ptr<ThreadingResourcesLight<UserData>> user);
    ~GetAllBooks();
protected:
    virtual void run()override;
signals:
    void sent_data(QString data);
};

#endif // GETALLBOOKS_H
