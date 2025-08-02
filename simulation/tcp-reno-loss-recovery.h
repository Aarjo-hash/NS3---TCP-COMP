// tcp-reno-loss-recovery.h

#ifndef TCP_RENO_LOSS_RECOVERY_H
#define TCP_RENO_LOSS_RECOVERY_H

#include "ns3/tcp-loss-recovery.h"
#include "ns3/tcp-socket-state.h"

namespace ns3 {

/**
 * \ingroup tcp
 * \brief TCP Reno loss recovery algorithm (NewReno-style)
 *
 * This class implements the Reno loss recovery strategy, primarily based
 * on Fast Retransmit and Fast Recovery. It works closely with TcpReno
 * congestion control.
 */
class TcpRenoLossRecovery : public TcpLossRecoveryAlgorithm
{
public:
  static TypeId GetTypeId (void);
  TcpRenoLossRecovery ();
  TcpRenoLossRecovery (const TcpRenoLossRecovery& other);
  virtual ~TcpRenoLossRecovery ();

  /**
   * \brief Called when duplicate ACKs are received (for Fast Retransmit)
   */
  virtual bool DupAck (Ptr<TcpSocketState> tcb, uint32_t count, const TcpHeader& header) override;

  /**
   * \brief Called when a retransmission timeout occurs
   */
  virtual void DoRetransmit (Ptr<TcpSocketState> tcb) override;

  /**
   * \brief Creates a copy of this loss recovery algorithm
   */
  virtual Ptr<TcpLossRecoveryAlgorithm> Fork () override;

protected:
  void EnterFastRecovery (Ptr<TcpSocketState> tcb);
  void ExitFastRecovery (Ptr<TcpSocketState> tcb);

private:
  bool m_inFastRecovery;
};

} // namespace ns3

#endif // TCP_RENO_LOSS_RECOVERY_H
