// tcp-reno.cc

#include "tcp-reno.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpReno");
NS_OBJECT_ENSURE_REGISTERED (TcpReno);

TypeId
TcpReno::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpReno")
    .SetParent<TcpCongestionOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpReno> ()
    .SetDocumentation ("TCP Reno congestion control implementation");
  return tid;
}

TcpReno::TcpReno (void)
  : TcpCongestionOps ()
{
  NS_LOG_FUNCTION (this);
}

TcpReno::TcpReno (const TcpReno& sock)
  : TcpCongestionOps (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpReno::~TcpReno (void)
{
}

std::string
TcpReno::GetName () const
{
  return "TcpReno";
}

void
TcpReno::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);
  if (tcb->m_cWnd < tcb->m_ssThresh)
    {
      // Slow Start
      tcb->m_cWnd += tcb->m_segmentSize * segmentsAcked;
    }
  else
    {
      // Congestion Avoidance
      tcb->m_cWnd += tcb->m_segmentSize * tcb->m_segmentSize / tcb->m_cWnd;
    }
}

uint32_t
TcpReno::GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this << tcb << bytesInFlight);
  return std::max (2 * tcb->m_segmentSize, bytesInFlight / 2);
}

Ptr<TcpCongestionOps>
TcpReno::Fork ()
{
  return CreateObject<TcpReno> (*this);
}

} // namespace ns3
