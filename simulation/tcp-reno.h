// tcp-reno.h

#ifndef TCP_RENO_H
#define TCP_RENO_H

#include "tcp-congestion-ops.h"
#include "ns3/tcp-socket-state.h"

namespace ns3 {

/**
 * \ingroup tcp
 * \brief Implementation of TCP Reno congestion control
 */
class TcpReno : public TcpCongestionOps
{
public:
  static TypeId GetTypeId (void);
  TcpReno (void);
  TcpReno (const TcpReno& sock);
  virtual ~TcpReno (void);

  virtual std::string GetName () const override;

  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) override;

  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight) override;

  virtual Ptr<TcpCongestionOps> Fork () override;
};

} // namespace ns3

#endif // TCP_RENO_H
