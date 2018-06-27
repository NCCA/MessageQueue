#include "PipeConsumer.h"
#include <iostream>
#include <iomanip>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

PipeConsumer::PipeConsumer(const std::string_view &_fifoName) :m_fifoName(_fifoName)
{
//  if(m_fifoName !="")
//  {
//    std::cerr<<"Making FIFO \n";
//    mkfifo(m_fifoName.c_str(), 0666);
    m_fifoID = open(m_fifoName.c_str(), O_RDWR | O_NONBLOCK);
//    perror("make fifo");
    if(m_fifoID == -1)
    {
      std::cerr<<"unable to open FIFO\n";
    }
}

PipeConsumer::~PipeConsumer()
{

}

bool PipeConsumer::setPipeName(const std::string_view &_fifoName)
{
  m_fifoName=_fifoName;
}

void PipeConsumer::consume(const message &_message)
{
}

