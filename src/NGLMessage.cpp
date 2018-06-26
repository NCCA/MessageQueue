#include "NGLMessage.h"
#include <thread>
#include <iostream>
#include "AbstractMessageConsumer.h"
#include "STDERRConsumer.h"
#include "STDOutConsumer.h"
#include "FileConsumer.h"

namespace ngl
{
  std::vector <message> NGLMessage::s_messageQueue;
  std::atomic_flag	 NGLMessage::s_consuming=ATOMIC_FLAG_INIT;
  std::unique_ptr<AbstractMessageConsumer> NGLMessage::m_consumer=std::make_unique<StdErrConsumer>();
  std::mutex g_messageQueueLock;

  NGLMessage::NGLMessage(Mode _mode,CommunicationMode _comMode) : m_mode(_mode),m_comMode(_comMode)
  {
    s_consuming.test_and_set();
    switch (m_comMode)
    {
      case CommunicationMode::STDERR : m_consumer=std::make_unique<StdErrConsumer>(); break;
      case CommunicationMode::STDOUT : m_consumer=std::make_unique<StdOutConsumer>(); break;
      case CommunicationMode::NULLCONSUMER : m_consumer=std::make_unique<NullMessageConsumer>(); break;
      case CommunicationMode::FILE :

      m_consumer=std::make_unique<FileConsumer>("_fname.out"); break;
      break;
    }

  }

  void NGLMessage::addMessage(const std::string &_message, Colours _c, TimeFormat _timeFormat)
  {
    std::lock_guard<std::mutex> lock(g_messageQueueLock);

    // add to front
    s_messageQueue.insert(std::begin(s_messageQueue),{std::chrono::system_clock::now(),_message,_c,_timeFormat});
  }



  void NGLMessage::launchMessageConsumer()
  {
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
    t.detach();

  }



}
