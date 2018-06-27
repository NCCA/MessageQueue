#include <iostream>
#include "NGLMessage.h"

#include <thread>

int main()
{
  ngl::NGLMessage message(ngl::NGLMessage::FromNamedPipe("nccadebug",ngl::NGLMessage::Mode::SERVER));
  message.startServer();
  while(1)
  {
  for(size_t i=97; i<97+26; ++i)
  {
    std::string msg="test message ";
    msg+=std::to_string(i);
    msg+='\n';
    message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
    message.addMessage(msg,Colours::RED);
    message.addMessage(msg,Colours::GREEN);
    message.addMessage(msg,Colours::YELLOW,TimeFormat::TIMEDATE);
    message.addMessage(msg,Colours::BLUE);

    message.addMessage(msg,Colours::MAGENTA,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::CYAN);
    message.addMessage(msg,Colours::WHITE,TimeFormat::TIMEDATEDAY);
    message.addMessage(msg,Colours::RESET);

  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

}
