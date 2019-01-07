#ifndef THREAD_TEST_H
#define THREAD_TEST_H

#include <QObject>

class thread_test : public QObject
{
    Q_OBJECT
public:
    explicit thread_test(QObject *parent = nullptr);

signals:

public slots:
    void debug();
};

#endif // THREAD_TEST_H
