#ifndef NGLMESSAGE_H_
#define NGLMESSAGE_H_

#include <chrono>
#include <string>
#include <vector>
#include "AbstractMessageConsumer.h"
namespace ngl
{


enum class CommunicationMode{NULLCONSUMER,STDOUT,STDERR,FILE,TCPIP,NAMEDPIPE,SOCKET,SHAREDMEMORY};
enum class DataType{NONE,VEC2,VEC3,VEC4,MAT2,MAT3,MAT4,QUATERNION};

class NGLMessage
{

  public :
    enum class Mode: bool{CLIENT,SERVER};

    bool isActive() const {return m_active;}
    Mode getMode() const {return m_mode;}
    static void addMessage(const std::string &_message,Colours _c=Colours::NORMAL,TimeFormat _timeFormat=TimeFormat::TIME);
    static size_t numMessages()  {return s_messageQueue.size();}
    static void launchMessageConsumer();
    NGLMessage(Mode _mode,CommunicationMode _comMode=CommunicationMode::STDERR);
    static void stopConsuming(){ s_consuming.clear();}
  private :
    bool m_active=false;
    Mode m_mode=Mode::CLIENT;
    CommunicationMode m_comMode=CommunicationMode::STDERR;
    static std::vector <message> s_messageQueue;
    static std::atomic_flag	 s_consuming;
    static std::unique_ptr<AbstractMessageConsumer> m_consumer;

};

}
#endif
