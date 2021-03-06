#ifndef LLARP_H_
#define LLARP_H_
#include <llarp/dht.h>
#include <llarp/ev.h>
#include <llarp/logic.h>
#include <llarp/mem.h>
#include <llarp/nodedb.h>
#include <llarp/router.h>
#include <llarp/version.h>

#ifdef __cplusplus
extern "C" {
#endif

/** llarp application context for C api */
struct llarp_main;

/** initialize application context and load config */
struct llarp_main *
llarp_main_init(const char *fname, bool multiProcess);

/** handle signal for main context */
void
llarp_main_signal(struct llarp_main *ptr, int sig);

/** set custom dht message handler function */
void
llarp_main_set_dht_handler(struct llarp_main *ptr, llarp_dht_msg_handler h);

/** run main context */
int
llarp_main_run(struct llarp_main *ptr);

void
llarp_main_free(struct llarp_main *ptr);

#ifdef __cplusplus
}
#endif
#endif
