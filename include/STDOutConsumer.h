#ifndef STDOUTCONSUMER_H_
#define STDOUTCONSUMER_H_
#include <iostream>
#include <iomanip>
#include "AbstractMessageConsumer.h"

class StdOutConsumer : public AbstractMessageConsumer
{
  public :
    StdOutConsumer()=default;
    virtual void consume(const message &_message) override;
};


void StdOutConsumer::consume(const message &_message)
{
  std::time_t tm = std::chrono::system_clock::to_time_t(_message.time);
  setTimeFormat(_message.timeFormat);
  std::cout<<getColourString(_message.colour)<<'['<<std::put_time(std::localtime(&tm),m_timeString.c_str())<<"] "<<  _message.message<<'\n';
}



#endif
