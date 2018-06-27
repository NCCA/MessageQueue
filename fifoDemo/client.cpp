#include <iostream>
#include "NGLMessage.h"

#include <thread>

int main()
{
  ngl::NGLMessage message(ngl::NGLMessage::FromNamedPipe("nccadebug",ngl::NGLMessage::Mode::CLIENT));
  message.startMessageConsumer();
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));


}
