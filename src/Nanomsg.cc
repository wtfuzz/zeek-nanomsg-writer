#include <string>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "Nanomsg.h"
#include "nanomsg.bif.h"

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

using namespace logging::writer;
using namespace threading;

Nanomsg::Nanomsg(WriterFrontend* frontend): WriterBackend(frontend), formatter(nullptr)
{
  endpoint.assign((const char *)BifConst::LogNanomsg::endpoint->Bytes(),
                  BifConst::LogNanomsg::endpoint->Len());
  socket_type.assign((const char *)BifConst::LogNanomsg::socket_type->Bytes(),
                     BifConst::LogNanomsg::socket_type->Len());
  delimiter.assign((const char *)BifConst::LogNanomsg::delimiter->Bytes(),
                   BifConst::LogNanomsg::delimiter->Len());
  prefix.assign((const char *)BifConst::LogNanomsg::prefix->Bytes(),
                   BifConst::LogNanomsg::prefix->Len());
}

Nanomsg::~Nanomsg()
{

}

bool Nanomsg::DoInit(const WriterInfo& info, int num_fields, const Field* const * fields)
{
  channel = info.path;
  formatter = new threading::formatter::JSON(this, threading::formatter::JSON::TS_EPOCH);

  sock = nn_socket(AF_SP, NN_PUSH);

  if(nn_connect(sock, endpoint.c_str()) < 0)
  {
    Error(Fmt("nn_connect() failed: %s", strerror(errno)));
    return false;
  }

  return true;
}

bool Nanomsg::DoFinish(double network_time)
{
  nn_close(sock);
  delete formatter;
  return true;
}

bool Nanomsg::DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals)
{
  struct nn_msghdr header;
  struct nn_iovec iov[5];

  ODesc buffer;
  buffer.Clear();

  formatter->Describe(&buffer, num_fields, fields, vals);

  if(prefix.length() == 0)
  {
    iov[0].iov_base = (void *)channel;
    iov[0].iov_len = strlen(channel);
    iov[1].iov_base = (void *)delimiter.c_str();
    iov[1].iov_len = delimiter.length();
    iov[2].iov_base = (void *)buffer.Bytes();
    iov[2].iov_len = buffer.Len();
    memset(&header, 0, sizeof(header));
    header.msg_iov = iov;
    header.msg_iovlen = 3;
  }
  else
  {
    iov[0].iov_base = (void *)prefix.c_str();
    iov[0].iov_len = prefix.length();
    iov[1].iov_base = (void *)delimiter.c_str();
    iov[1].iov_len = delimiter.length();
    iov[2].iov_base = (void *)channel;
    iov[2].iov_len = strlen(channel);
    iov[3].iov_base = (void *)delimiter.c_str();
    iov[3].iov_len = delimiter.length();
    iov[4].iov_base = (void *)buffer.Bytes();
    iov[4].iov_len = buffer.Len();
    memset(&header, 0, sizeof(header));
    header.msg_iov = iov;
    header.msg_iovlen = 5;
  }

  if(nn_sendmsg(sock, &header, 0) < 0)
  {
    Error(Fmt("nn_sendmsg() failed: %s", strerror(errno)));
    return false;
  }

  return true;
}

bool Nanomsg::DoSetBuf(bool enabled)
{
    return true;
}

bool Nanomsg::DoFlush(double network_time)
{
    return true;
}

bool Nanomsg::DoRotate(const char* rotated_path, double open, double close, bool terminating)
{
  return FinishedRotation();
}

bool Nanomsg::DoHeartbeat(double network_time, double current_time)
{
  return true;
}
