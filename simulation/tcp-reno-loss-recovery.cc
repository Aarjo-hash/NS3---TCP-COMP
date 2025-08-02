// tcp-reno-loss-recovery.cc

#include "tcp-reno-loss-recovery.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRenoLossRecovery");
NS_OBJECT_ENSURE_REGISTERED (TcpRenoLossRecovery);

TypeId
TcpRenoLossRecovery::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRenoLossRecovery")
    .SetParent<TcpLossRecoveryAlgorithm> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpRenoLossRecovery> ()
    ;
  return tid;
}

TcpRenoLossRecovery::TcpRenoLossRecovery ()
  : m_inFastRecovery (false)
{
  NS_LOG_FUNCTION (this);
}

TcpRenoLossRecovery::TcpRenoLossRecovery (const TcpRenoLossRecovery& other)
  : TcpLossRecoveryAlgorithm (other),
    m_inFastRecovery (other.m_inFastRecovery)
{
  NS_LOG_FUNCTION (this);
}

TcpRenoLossRecovery::~TcpRenoLossRecovery ()
{
  NS_LOG_FUNCTION (this);
}

Ptr<TcpLossRecoveryAlgorithm>
TcpRenoLossRecovery::Fork ()
{
  return CopyObject<TcpRenoLossRecovery> (this);
}

bool
TcpRenoLossRecovery::DupAck (Ptr<TcpSocketState> tcb, uint32_t count, const TcpHeader& header)
{
  NS_LOG_FUNCTION (this << tcb << count << header);

  if (count == tcb->m_retxThresh && !m_inFastRecovery)
    {
      NS_LOG_INFO ("Fast Retransmit triggered");
      EnterFastRecovery (tcb);
      return true; // Retransmit needed
    }

  if (m_inFastRecovery)
    {
      NS_LOG_INFO ("In fast recovery, inflating congestion window");
      tcb->m_cWnd += tcb->m_segmentSize;
    }

  return false; // No retransmit required
}

void
TcpRenoLossRecovery::DoRetransmit (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  NS_LOG_INFO ("Timeout occurred: resetting cWnd and ssthresh");
  tcb->m_ssthresh = std::max (2 * tcb->m_segmentSize, tcb->m_cWnd / 2);
  tcb->m_cWnd = tcb->m_segmentSize;
  ExitFastRecovery (tcb);
}

void
TcpRenoLossRecovery::EnterFastRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  m_inFastRecovery = true;
  tcb->m_ssthresh = std::max (2 * tcb->m_segmentSize, tcb->m_cWnd / 2);
  tcb->m_cWnd = tcb->m_ssthresh + 3 * tcb->m_segmentSize;
  tcb->m_nextTxSequence = tcb->m_highTxMark;
}

void
TcpRenoLossRecovery::ExitFastRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  m_inFastRecovery = false;
  tcb->m_cWnd = tcb->m_ssthresh;
}

} // namespace ns3
