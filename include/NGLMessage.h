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
    // see here for discussion on why I do this https://stackoverflow.com/questions/5120768/how-to-implement-the-factory-method-pattern-in-c-correctly
    struct FromFilename
    {
      FromFilename(const std::string_view &_name) : m_name(_name){}
      std::string_view m_name;
    };

    bool isActive() const {return m_active;}
    Mode getMode() const {return m_mode;}
    static void addMessage(const std::string &_message,Colours _c=Colours::NORMAL,TimeFormat _timeFormat=TimeFormat::TIME);
    static size_t numMessages()  {return s_messageQueue.size();}
    static void launchMessageConsumer();
    NGLMessage(Mode _mode,CommunicationMode _comMode=CommunicationMode::STDERR);
    NGLMessage(const FromFilename &_fname);
    void setFilename(const std::string_view &_fname);
    static void clearMessageQueue();
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
