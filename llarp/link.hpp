#ifndef LLARP_LINK_HPP
#define LLARP_LINK_HPP
#include <netinet/in.h>
#include <llarp/crypto.h>
#include <cstdint>
#include <map>
#include <memory>
#include <functional>

#include <llarp/ev.h>
#include <llarp/router_contact.h>

namespace llarp
{
  struct Link;
  
  struct PeerSession
  {
    sockaddr_in6 remoteAddr;
    llarp_rc rc;
    llarp_sharedkey_t sessionKey;

    uint64_t lastRX;

    llarp_crypto * _crypto;
    
    enum State
    {
      eStateNULL,
      eHandshakeInboundInit,
      eHandshakeOutboundInit,
      eHandshakeInboundRepliedInit,
      eHandshakeOutboundGotReply,
      eHandshakeInboundGotAck,
      eHandshakeOutboundGotAck,
      eEstablished,
      eTimeout
    };

    State state;

    /** inbound session */
    PeerSession(llarp_crypto * crypto, sockaddr_in6 remote);
    /** outbound session */
    PeerSession(llarp_crypto * crypto, llarp_rc rc);
    
    void SendTo(Link * link, const char * buff, std::size_t sz);

    void RecvFrom(const char * buff, ssize_t sz);


    typedef std::shared_ptr<PeerSession> Ptr;
  };
  
  struct Link
  {
    typedef std::map<sockaddr_in6, PeerSession::Ptr> Sessions;
 
    Sessions sessions;
    llarp_seckey_t transportSecKey;
    llarp_pubkey_t transportPubKey;

    llarp_crypto * _crypto;

    Link(llarp_crypto * crypto);
    
    llarp_udp_listener _listener;

    llarp_udp_listener * Listener() { return &_listener; }
    
  };
}

#endif
