#ifndef LOGGING_WRITER_NANOMSG_H
#define LOGGING_WRITER_NANOMSG_H

#include "logging/WriterBackend.h"
#include "threading/formatters/JSON.h"

namespace logging { namespace writer {

class Nanomsg : public WriterBackend
{

public:
  Nanomsg(WriterFrontend* frontend);
  ~Nanomsg();

  static WriterBackend* Instantiate(WriterFrontend* frontend)
    { return new Nanomsg(frontend); }

protected:
  virtual bool DoInit(const WriterBackend::WriterInfo& info, int num_fields, const threading::Field* const* fields);
  virtual bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals);
  virtual bool DoSetBuf(bool enabled);
  virtual bool DoRotate(const char* rotated_path, double open, double close, bool terminating);
  virtual bool DoFlush(double network_time);
  virtual bool DoFinish(double network_time);
  virtual bool DoHeartbeat(double network_time, double current_time);

private:
  threading::formatter::JSON* formatter;
  string endpoint;
  string socket_type;
  string delimiter;
  int sock;
  const char *channel;
};

}}

#endif
