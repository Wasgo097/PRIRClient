#ifndef GETMYBOOKS_H
#define GETMYBOOKS_H

#include "mytask.h"

class GetMyBooks : public MyTask{
    Q_OBJECT
public:
    GetMyBooks(MainWindow * Window,
               std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
               std::shared_ptr<ThreadingResourcesLight<UserState>> state,
               std::shared_ptr<ThreadingResourcesLight<UserData>> user);
protected:
    virtual void run()override;
};

#endif // GETMYBOOKS_H
