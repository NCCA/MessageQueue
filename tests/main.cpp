#include <gtest/gtest.h>
#include <array>
#include <string>
#include <thread>
#include "NGLMessage.h"
class Environment : public  ::testing::Environment
{
 public:
  virtual ~Environment() {}
  // Override this to define how to set up the environment.
  virtual void SetUp()
  {
      std::cout<<"Setup Test Environment\n";
  }
  // Override this to define how to tear down the environment.
  virtual void TearDown()
  {
    std::cerr<<"Teardown removing files\n";
  }
};


TEST(NGLMessage,defaultctor)
{
  {
    ngl::NGLMessage message(ngl::NGLMessage::Mode::CLIENT);
    EXPECT_TRUE(message.isActive()==false);
    EXPECT_TRUE(message.getMode()==ngl::NGLMessage::Mode::CLIENT);
  }
  {
    ngl::NGLMessage message(ngl::NGLMessage::Mode::SERVER);
    EXPECT_TRUE(message.getMode()==ngl::NGLMessage::Mode::SERVER);
  }
}

TEST(NGLMessage,addMessage)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::SERVER);
  message.addMessage("test message");
  EXPECT_TRUE(message.numMessages()==1);
}

TEST(NGLMessage,stdOut)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::SERVER,ngl::CommunicationMode::STDOUT);
  message.addMessage("test message to std out");
  EXPECT_TRUE(message.numMessages()==1);
  ngl::NGLMessage::launchMessageConsumer();
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);

}


TEST(NGLMessage,writeOutput)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::CLIENT);

  ngl::NGLMessage::launchMessageConsumer();
  for(size_t i=97; i<97+26; ++i)
  {
    std::string msg="test message ";
    msg+=(int)i;
    message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
    message.addMessage(msg,Colours::RED);
    message.addMessage(msg,Colours::GREEN);
    message.addMessage(msg,Colours::YELLOW,TimeFormat::TIMEDATE);
    message.addMessage(msg,Colours::BLUE);

    message.addMessage(msg,Colours::MAGENTA,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::CYAN);
    message.addMessage(msg,Colours::WHITE);
    message.addMessage(msg,Colours::RESET);

  }
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);

}

TEST(NGLMessage,nullConsumer)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::SERVER,ngl::CommunicationMode::NULLCONSUMER);
  ngl::NGLMessage::launchMessageConsumer();
  for(size_t i=97; i<97+26; ++i)
  {
    std::string msg="test message ";
    msg+=(int)i;
    message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
    message.addMessage(msg,Colours::RED);
    message.addMessage(msg,Colours::GREEN);
    message.addMessage(msg,Colours::YELLOW,TimeFormat::TIMEDATE);
    message.addMessage(msg,Colours::BLUE);

    message.addMessage(msg,Colours::MAGENTA,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::CYAN);
    message.addMessage(msg,Colours::WHITE);
    message.addMessage(msg,Colours::RESET);

  }
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);
}


TEST(NGLMessage,fileConsumer)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::SERVER,ngl::CommunicationMode::FILE);
  ngl::NGLMessage::launchMessageConsumer();
  for(size_t i=97; i<97+26; ++i)
  {
    std::string msg="test message ";
    msg+=(int)i;
    message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
    message.addMessage(msg,Colours::RED);
    message.addMessage(msg,Colours::GREEN);
    message.addMessage(msg,Colours::YELLOW,TimeFormat::TIMEDATE);
    message.addMessage(msg,Colours::BLUE);

    message.addMessage(msg,Colours::MAGENTA,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::CYAN);
    message.addMessage(msg,Colours::WHITE);
    message.addMessage(msg,Colours::RESET);

  }
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);
}


TEST(NGLMessage,fileConsumerFromFileName)
{
  ngl::NGLMessage message(ngl::NGLMessage::FromFilename("testFromFilename.out"));
  ngl::NGLMessage::launchMessageConsumer();
  for(size_t i=97; i<97+26; ++i)
  {
    std::string msg="test message ";
    msg+=(int)i;
    message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
    message.addMessage(msg,Colours::RED);
    message.addMessage(msg,Colours::GREEN);
    message.addMessage(msg,Colours::YELLOW,TimeFormat::TIMEDATE);
    message.addMessage(msg,Colours::BLUE);

    message.addMessage(msg,Colours::MAGENTA,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::CYAN);
    message.addMessage(msg,Colours::WHITE);
    message.addMessage(msg,Colours::RESET);

  }
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);
}



TEST(NGLMessage,testMultiThread)
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::CLIENT);
  auto func=[]()
  {
    for(size_t i=0; i<20; ++i)
    {
      ngl::NGLMessage::addMessage("test threads",Colours::CYAN);
    }
    };
  std::array<std::thread,10> threads;
  for (auto &t : threads)
  {
    t=std::thread(func);
  }
  for (auto &t : threads)
  {
    t.join();
  }
  ASSERT_TRUE(ngl::NGLMessage::numMessages() == 20*10);
  ngl::NGLMessage::launchMessageConsumer();
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
  EXPECT_TRUE(message.numMessages()==0);
}



int main(int argc, char **argv)
{
  ::testing::AddGlobalTestEnvironment(new Environment);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
