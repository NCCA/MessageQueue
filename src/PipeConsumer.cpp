#include "PipeConsumer.h"
#include <iostream>
#include <iomanip>

PipeConsumer::PipeConsumer(const std::string_view &_fifoName)
{

}

PipeConsumer::~PipeConsumer()
{

}

bool PipeConsumer::setPipeName(const std::string_view &_fifoName)
{

}

void PipeConsumer::consume(const message &_message)
{
  std::time_t tm = std::chrono::system_clock::to_time_t(_message.time);
  setTimeFormat(_message.timeFormat);
  //m_output<<'['<<std::put_time(std::localtime(&tm),m_timeString.c_str())<<"] "<<  _message.message<<'\n';

}

