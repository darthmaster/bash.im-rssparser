#include "rss.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QTextDocument>
#include <QXmlStreamReader>
#include <iostream>
#include <QRandomGenerator>

void RSS::getRssPage(QString url)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest((QUrl)url));
}

void RSS::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError){
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        Parser(codec->toUnicode(content.data()));
    }
    else {
        std::cout << reply->errorString().toStdString() << std::flush;
    }
    reply->deleteLater();
}

void RSS::Parser(QString data){
    QXmlStreamReader xml(data);
    QList<post> posts;
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement && xml.name() == "item")
        {
            QStringList P;
            while (!(xml.isEndElement() && xml.name() == "item"))
            {
                xml.readNext();
                if (token == QXmlStreamReader::StartElement)
                {
                    auto name = xml.name().toString();
                    auto value = xml.readElementText();
                    if (name == QString("title")){
                        P.insert(0, value);
                        continue;
                    } else
                        if (name == QString("pubDate")){
                            P.insert(1, value);
                            continue;
                        } else
                            if (name == QString("description")){
                                QTextDocument d;
                                d.setHtml(value);
                                P.insert(2, d.toPlainText());
                                continue;
                            } else
                                continue;
                }
            }
            posts.append(post{P[0],P[1],P[2]});
            P.clear();
        }
    }
    if(xml.hasError()){
        std::cout << xml.errorString().toStdString() << std::flush;
    }else{
        auto *rg = QRandomGenerator::global();
        int i = rg->bounded(0, posts.count()-1);
        QString s = QString("%1 | %2\n\n%3")
                .arg(posts[i].title)
                .arg(posts[i].date)
                .arg(posts[i].desc);
        std::cout << s.toStdString() << std::endl;
    }
    xml.clear();
    QCoreApplication::quit();
}
