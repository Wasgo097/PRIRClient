#ifndef ORDER_H
#define ORDER_H

#include "mytask.h"

class Order : public MyTask
{
    Q_OBJECT
public:
    Order(MainWindow * Window,
          std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
          std::shared_ptr<ThreadingResourcesLight<UserState>> state,
          std::shared_ptr<ThreadingResourcesLight<UserData>> user);
protected:
    virtual void run()override;
};

#endif // ORDER_H
