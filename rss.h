#ifndef RSS_H
#define RSS_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QCoreApplication>

class RSS : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *manager;
public:
    //explicit RSS(QObject *parent);
    void getRssPage(QString);
    typedef struct{
        QString title;
        QString date;
        QString desc;
    } post;
private:
    void Parser(QString);

private slots:
    void replyFinished(QNetworkReply *);
};

#endif // RSS_H
