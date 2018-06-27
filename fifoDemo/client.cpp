#include <iostream>
#include "NGLMessage.h"

#include <thread>

int main()
{
  ngl::NGLMessage message(ngl::NGLMessage::FromNamedPipe("nccadebug",ngl::NGLMessage::Mode::CLIENT));
  message.startMessageConsumer();

  while(true) ;

}
