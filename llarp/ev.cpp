#include <llarp/ev.h>
#include <llarp/logic.h>
#include "mem.hpp"

#ifdef __linux__
#include "ev_epoll.hpp"
#endif
#if(__APPLE__ && __MACH__)
#include "ev_kqueue.hpp"
#endif
#ifdef __FreeBSD__
#include "ev_kqueue.hpp"
#endif

extern "C" {

void
llarp_ev_loop_alloc(struct llarp_ev_loop **ev)
{
#ifdef __linux__
  *ev = new llarp_epoll_loop;
#endif
#if(__APPLE__ && __MACH__)
  *ev = new llarp_kqueue_loop;
#endif
#ifdef __FreeBSD__
  *ev = new llarp_kqueue_loop;
#endif
  (*ev)->init();
}

void
llarp_ev_loop_free(struct llarp_ev_loop **ev)
{
  delete *ev;
  *ev = nullptr;
}

int
llarp_ev_loop_run(struct llarp_ev_loop *ev)
{
  return ev->run();
}

void
llarp_ev_loop_run_single_process(struct llarp_ev_loop *ev,
                                 struct llarp_threadpool *tp,
                                 struct llarp_logic *logic)
{
  while(true)
  {
    if(ev->tick(10) == -1)
      return;
    llarp_logic_tick(logic);
    llarp_threadpool_tick(tp);
  }
}

int
llarp_ev_add_udp(struct llarp_ev_loop *ev, struct llarp_udp_io *udp,
                 const struct sockaddr *src)
{
  udp->parent = ev;
  if(ev->udp_listen(udp, src))
    return 0;
  return -1;
}

int
llarp_ev_close_udp(struct llarp_udp_io *udp)
{
  if(udp->parent->udp_close(udp))
    return 0;
  return -1;
}

void
llarp_ev_loop_stop(struct llarp_ev_loop *loop)
{
  loop->stop();
}

int
llarp_ev_udp_sendto(struct llarp_udp_io *udp, const sockaddr *to,
                    const void *buf, size_t sz)
{
  return static_cast< llarp::ev_io * >(udp->impl)->sendto(to, buf, sz);
}
}
