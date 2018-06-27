#ifndef NGLMESSAGE_H_
#define NGLMESSAGE_H_

#include <chrono>
#include <string>
#include <vector>
#include "AbstractMessageConsumer.h"
namespace ngl
{


enum class CommunicationMode{NULLCONSUMER,STDOUT,STDERR,FILE,TCPIP,NAMEDPIPE,UDP,SHAREDMEMORY};
enum class DataType{NONE,VEC2,VEC3,VEC4,MAT2,MAT3,MAT4,QUATERNION};

class NGLMessage
{

  public :
    enum class Mode  {CLIENT,SERVER,CLIENTSERVER};
    // see here for discussion on why I do this https://stackoverflow.com/questions/5120768/how-to-implement-the-factory-method-pattern-in-c-correctly
    struct FromFilename
    {
      FromFilename(const std::string_view &_name) : m_name(_name){}
      std::string_view m_name;
    };

    struct FromNamedPipe
    {
      FromNamedPipe(const std::string_view &_name) : m_name(_name){}
      std::string_view m_name;
    };
    NGLMessage(Mode _mode,CommunicationMode _comMode=CommunicationMode::STDERR);
    NGLMessage(const FromFilename &_fname);
    NGLMessage(const FromNamedPipe &_fname);

    static bool isActive()  {return m_active;}
    static Mode getMode()   {return m_mode;}
    static void addMessage(const std::string &_message,Colours _c=Colours::NORMAL,TimeFormat _timeFormat=TimeFormat::TIME);
    static size_t numMessages()  {return s_messageQueue.size();}
    static void startMessageConsumer();
    static void stopMessageConsumer(){ s_consuming.clear();}

    static bool startServer();
    static void stopServer(){ s_server.clear();}


    static void clearMessageQueue();
    void setFilename(const std::string_view &_fname);
  private :
    static bool m_active;
    static Mode m_mode;
    CommunicationMode m_comMode=CommunicationMode::STDERR;
    static std::vector <message> s_messageQueue;
    static std::atomic_flag	 s_consuming;
    static std::atomic_flag	 s_server;
    static std::unique_ptr<AbstractMessageConsumer> m_consumer;

};

}
#endif
