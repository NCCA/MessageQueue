#ifndef THREAD_H_
#define THREAD_H_

#include <QThread>
#include <QColor>
#include <QString>

class Thread  : public  QThread
{
    Q_OBJECT
  public :
      Thread(const QString &_pipeName,QObject *parent);
      void run();
  private :

    QString m_pipeName;
    bool m_active=false;

  signals:
    void text(QColor _colour,QString _text);
  public slots:
    void stopThread(){m_active=false;}

};

#endif
