#include <iostream>
#include "NGLMessage.h"
#include <ngl/fmt/format.h>
#include <thread>

int main()
{
  ngl::NGLMessage message(ngl::NGLMessage::FromNamedPipe("nccadebug",ngl::NGLMessage::Mode::SERVER));
  message.startServer();
  while(1)
  {
    for(size_t i=0; i<4; ++i)
    {
      std::string msg="Basic Message Generation ";
      msg+=std::to_string(i);
      msg+='\n';
      message.addMessage(msg,Colours::NORMAL,TimeFormat::TIME);
      message.addMessage(4,20,fmt::format("Test format {0} {1} {0}\n",1,i,3),Colours::RED);
    }
    message.sendClearTerminal();
  //std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

}
