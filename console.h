#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <QObject>
#include <QString>
#include <string>
#include <iostream>



class Console : public QObject
{
    Q_OBJECT

    public:
        Console() {}
        void operator() const;

    signals:
        void consoleInput(QString& text);
}


#endif
