#include "NGLMessage.h"
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <strstream>

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
  NGLMessage::Mode NGLMessage::s_mode=Mode::CLIENT;
  std::mutex g_messageQueueLock;
  std::mutex g_serverLock;
  bool NGLMessage::s_active=true;
  CommunicationMode NGLMessage::s_comMode=CommunicationMode::STDERR;

  static std::string s_fifoName="/tmp/nccadebug";
  static int s_fifoID=0;

  NGLMessage::NGLMessage(Mode _mode,CommunicationMode _comMode)
  {
    s_mode=_mode;
    s_comMode=_comMode;
    s_consuming.test_and_set();
    s_server.test_and_set();
    switch (s_comMode)
    {
      case CommunicationMode::STDERR : m_consumer=std::make_unique<StdErrConsumer>(); s_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::STDOUT : m_consumer=std::make_unique<StdOutConsumer>(); s_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::NULLCONSUMER : m_consumer=std::make_unique<NullMessageConsumer>(); s_mode=Mode::CLIENTSERVER; break;
      case CommunicationMode::FILE :  m_consumer=std::make_unique<FileConsumer>("NGLMessageDebug.out"); s_mode=Mode::CLIENTSERVER; break;
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


  NGLMessage::~NGLMessage()
  {

  }

  void NGLMessage::setFilename(const std::string_view &_fname)
  {
    FileConsumer *f=reinterpret_cast<FileConsumer *>(m_consumer.get());
    if(f!=nullptr)
      f->setFilename(_fname);
  }

  NGLMessage::NGLMessage(const FromFilename &_f)
  {
    s_comMode=CommunicationMode::FILE;
    s_mode=Mode::CLIENTSERVER;
    m_consumer=std::make_unique<FileConsumer>(_f.m_name);

  }

  NGLMessage::NGLMessage(const FromNamedPipe &_f)
  {
    s_comMode=CommunicationMode::NAMEDPIPE;
    s_mode=_f.m_mode;
    s_fifoName=_f.m_name;
    if(s_mode == Mode::CLIENT)
    {
      m_consumer=std::make_unique<PipeConsumer>(_f.m_name);
    }
    else if(s_mode == Mode::SERVER)
    {
      createFiFo();
      s_server.test_and_set();
    }
  }

  void NGLMessage::addMessage(const std::string &_message, Colours _c, TimeFormat _timeFormat)
  {
    std::lock_guard<std::mutex> lock(g_messageQueueLock);

    // add to front
    s_messageQueue.insert(std::begin(s_messageQueue),{std::chrono::system_clock::now(),_message,_c,_timeFormat});
  }



  void NGLMessage::startMessageConsumer()
  {
    if(s_mode == Mode::SERVER)
    {
      std::cerr<<"Trying to launch consumer on Server \n";
      return;
    }
    if( s_comMode == CommunicationMode::NULLCONSUMER ||
        s_comMode == CommunicationMode::FILE ||
        s_comMode == CommunicationMode::STDERR ||
        s_comMode == CommunicationMode::STDOUT)
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
      s_active=true;
      t.detach();
     } // client server in one versions
    else if(s_comMode == CommunicationMode::NAMEDPIPE)
    {
      std::thread t([]()
      {
        char str1[2048];
        while(true)
        {
          s_fifoID = open(s_fifoName.c_str(), O_RDONLY );
          auto size=read(s_fifoID, str1, 2048);
          str1[size]='\0';
          std::cerr<<str1;
          close(s_fifoID);
        }
        });
      s_active=true;
      t.detach();
    }


  }


  bool NGLMessage::startServer()
  {
    std::cerr<<"starting server\n";
    if(s_mode !=Mode::SERVER)
    {
      std::cerr<<"Error attempting to start server in Client Mode \n";
      return false;
    }
    if(s_comMode == CommunicationMode::NAMEDPIPE)
    {
       createFiFo();
       std::thread t([]()
       {
         while(s_server.test_and_set())
         {
         std::lock_guard<std::mutex> lock(g_serverLock);
         if(s_messageQueue.size() !=0)
         {
           auto msg=s_messageQueue.back();
           s_messageQueue.pop_back();
           std::strstream message;
           message<<AbstractMessageConsumer::getColourString(msg.colour);

           // put_time returns a " " if time string is empty which is annoying!
           if(msg.timeFormat !=TimeFormat::NONE)
           {
             std::string fmt=AbstractMessageConsumer::getTimeString(msg.timeFormat);
             std::time_t tm = std::chrono::system_clock::to_time_t(msg.time);
             message<<std::put_time(std::localtime(&tm),fmt.c_str())<<' ';
           }


           message<<msg.message;
           write(s_fifoID,message.str(),message.pcount());

         }
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
         }

       });
       s_active=true;
       t.detach();
       return true;
    }
    else return false;
  }

  bool NGLMessage::createFiFo()
  {
    if(s_fifoName !="")
    {
      std::cerr<<"Making FIFO \n";
      mkfifo(s_fifoName.c_str(), 0666);
      s_fifoID = open(s_fifoName.c_str(), O_RDWR | O_NONBLOCK);
      perror("make fifo");
      std::cout<<"ID "<<s_fifoID<<" name "<<s_fifoName<<'\n';
    }
    return  (s_fifoID == -1) ? true : false;

  }



  bool NGLMessage::createSocket()
  {
    return false;
  }
  bool NGLMessage::createSharedMemory()
  {
    return false;
  }



}
