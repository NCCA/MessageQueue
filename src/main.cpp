#include "NGLMessage.h"
#include <string>
#include <thread>

int main()
{
  ngl::NGLMessage message(ngl::NGLMessage::Mode::CLIENT);

  ngl::NGLMessage::launchMessageConsumer();
  for(size_t i=0; i<100; ++i)
  {
    std::string msg="test message ";
    msg+=(int)i;
    message.addMessage(msg,Colours::RED);
  }
  while(ngl::NGLMessage::numMessages() !=0)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  ngl::NGLMessage::stopConsuming();
}
