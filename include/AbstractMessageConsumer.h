#ifndef ABSTRACTMESSAGECONSUMER_H_
#define ABSTRACTMESSAGECONSUMER_H_
#include <string>
#include <chrono>

enum class Colours{NORMAL,RED,GREEN ,YELLOW,BLUE,MAGENTA,CYAN,WHITE,RESET};
enum TimeFormat{TIME,TIMEDATE,TIMEDATEDAY};

struct message
{
  std::chrono::system_clock::time_point time;
  std::string message;
  Colours colour;
  TimeFormat timeFormat;
};


class AbstractMessageConsumer
{
  public :

    AbstractMessageConsumer()=default;
    virtual void consume(const message &_message)=0;
    virtual ~AbstractMessageConsumer()=default;
    AbstractMessageConsumer(const AbstractMessageConsumer &)=delete;
    AbstractMessageConsumer &operator=(AbstractMessageConsumer &)=delete;
    void setTimeFormat(TimeFormat _f);
    std::string getColourString(const Colours &_colour) const;

  protected :
    std::string m_timeString="%I:%M%p";
    Colours m_activeColour=Colours::NORMAL;

};

class NullMessageConsumer : public AbstractMessageConsumer
{
  public :
    NullMessageConsumer()=default;

    virtual ~NullMessageConsumer()=default;
    virtual void consume(const message &){;}

};

#endif
