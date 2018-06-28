#include "Thread.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <QColor>
#include <QtDebug>

Thread::Thread(const QString &_pipeName,QObject *parent) : QThread (parent)
{
  m_pipeName=_pipeName;
}
void Thread::run()
{
  m_active=true;

  auto fifoID = open(m_pipeName.toStdString().c_str(), O_RDONLY );
  char data[2048];
  while(m_active)
  {
      auto size=read(fifoID, data, 2048);
      data[size]='\0';
      if(size !=0)
      {
        QString message(data);
        auto index=message.indexOf("m");
        if (message.contains("\033[2J\033[1;1H") )
        {
          emit clearText();
        }
        else
        {
        QColor colour;
        QString colourString=message;
        colourString.truncate(index);

        if(colourString == "\u001B[0")
          colour=Qt::GlobalColor::black;
        else if(colourString == "\u001B[31")
          colour=Qt::GlobalColor::red;
        else if(colourString == "\u001B[32")
          colour=Qt::GlobalColor::green;
        else if(colourString == "\u001B[33")
          colour=Qt::GlobalColor::yellow;

        else if(colourString == "\u001B[34" )
          colour=Qt::GlobalColor::blue;

        else if(colourString == "\u001B[35")
          colour=Qt::GlobalColor::magenta;

        else if(colourString == "\u001B[36")
          colour=Qt::GlobalColor::cyan;
        else if(colourString == "\u001B[37")
                colour=Qt::GlobalColor::white;

        emit text(colour,QString(&data[index+1]).trimmed());
        }
      }
      }
  close(fifoID);

}
