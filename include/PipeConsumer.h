#ifndef PIPECONSUMER_H_
#define PIPECONSUMER_H_

#include <fstream>
#include <string_view>
#include "AbstractMessageConsumer.h"

class PipeConsumer : public AbstractMessageConsumer
{
  public :
    PipeConsumer(const std::string_view &_fifoName);
    virtual ~PipeConsumer() override;
    bool setPipeName(const std::string_view &_fifoName);
    virtual void consume(const message &_message) override;
  private :
    std::ofstream m_output;
    std::string m_fifoName;
};




#endif
