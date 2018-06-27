#include "NGLMessage.h"
#include <thread>
#include <iostream>
#include "AbstractMessageConsumer.h"
#include "STDERRConsumer.h"
#include "STDOutConsumer.h"
#include "FileConsumer.h"
#include "PipeConsumer.h"
namespace ngl
{
  std::vector <message> NGLMessage::s_messageQueue;
  std::atomic_flag NGLMessage::s_consuming=ATOMIC_FLAG_INIT;
  std::atomic_flag NGLMessage::s_server=ATOMIC_FLAG_INIT;
  std::unique_ptr<AbstractMessageConsumer> NGLMessage::m_consumer=std::make_unique<StdErrConsumer>();
  NGLMessage::Mode NGLMessage::m_mode=Mode::CLIENT;
  std::mutex g_messageQueueLock;
  bool NGLMessage::m_active=true;



  NGLMessage::NGLMessage(Mode _mode,CommunicationMode _comMode) : m_comMode(_comMode)
  {
    m_mode=_mode;
    s_consuming.test_and_set();
    switch (m_comMode)
    {
      case CommunicationMode::STDERR : m_consumer=std::make_unique<StdErrConsumer>(); m_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::STDOUT : m_consumer=std::make_unique<StdOutConsumer>(); m_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::NULLCONSUMER : m_consumer=std::make_unique<NullMessageConsumer>(); m_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::FILE :  m_consumer=std::make_unique<FileConsumer>("NGLMessageDebug.out"); m_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::NAMEDPIPE : m_consumer=std::make_unique<PipeConsumer>("/tmp/NCCADebug"); break;
      case CommunicationMode::TCPIP : break;
      case CommunicationMode::UDP : break;
      case CommunicationMode::SHAREDMEMORY : break;
    }

  }

  void NGLMessage::clearMessageQueue()
  {
    s_messageQueue.clear();
  }

  void NGLMessage::setFilename(const std::string_view &_fname)
  {
    FileConsumer *f=reinterpret_cast<FileConsumer *>(m_consumer.get());
    if(f!=nullptr)
      f->setFilename(_fname);
  }

  NGLMessage::NGLMessage(const FromFilename &_f) :  m_comMode(CommunicationMode::FILE)
  {
    m_mode=Mode::CLIENTSERVER;
    m_consumer=std::make_unique<FileConsumer>(_f.m_name);

  }

  NGLMessage::NGLMessage(const FromNamedPipe &_f) : m_comMode(CommunicationMode::NAMEDPIPE)
  {
     m_mode=Mode::SERVER;
    m_consumer=std::make_unique<PipeConsumer>(_f.m_name);

  }

  void NGLMessage::addMessage(const std::string &_message, Colours _c, TimeFormat _timeFormat)
  {
    std::lock_guard<std::mutex> lock(g_messageQueueLock);

    // add to front
    s_messageQueue.insert(std::begin(s_messageQueue),{std::chrono::system_clock::now(),_message,_c,_timeFormat});
  }



  void NGLMessage::startMessageConsumer()
  {
    if(m_mode == Mode::SERVER)
    {
      std::cerr<<"Trying to launch consumer on Server \n";
      return;
    }
    std::thread t([]()
    {
      while(s_consuming.test_and_set())
      {
      std::lock_guard<std::mutex> lock(g_messageQueueLock);
      if(s_messageQueue.size() !=0)
      {
        auto msg=s_messageQueue.back();
        s_messageQueue.pop_back();
        m_consumer->consume(msg);
      }
//      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      }

    });
    m_active=true;
    t.detach();
  }


  bool NGLMessage::startServer()
  {
    if(m_mode !=Mode::SERVER)
    {
      std::cerr<<"Error attempting to start server in Client Mode \n";
      return false;
    }

    else return true;
  }



}
