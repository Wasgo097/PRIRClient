#ifndef GETALLBOOKS_H
#define GETALLBOOKS_H

#include "mytask.h"

class GetAllBooks : public MyTask
{
    Q_OBJECT
public:
    GetAllBooks(MainWindow * Window,
                std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
                std::shared_ptr<ThreadingResourcesLight<UserState>> state,
                std::shared_ptr<ThreadingResourcesLight<UserData>> user);
protected:
    virtual void run()override;
};

#endif // GETALLBOOKS_H
