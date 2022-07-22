
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/async.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/socket_helper.h"

int main(int argc, char* argv[]) {
  platform_prepare();
  tk_socket_init();

  if (argc < 4) {
    printf("Usage: %s host port timeout\n", argv[0]);
  } else {
    const char* host = argv[1];
    int port = tk_atoi(argv[2]);
    int timeout = tk_atoi(argv[3]);
    int sock = tk_tcp_connect_ex(host, port, timeout);

    if (sock >= 0) {
      if (timeout == 0) {
        if (tk_socket_wait_for_connected(sock, 1000) == RET_OK) {
          log_debug("connect to %s %d ok\n", host, port);
        } else {
          log_debug("connect to %s %d fail\n", host, port);
        }
      } else {
        log_debug("connect to %s %d ok\n", host, port);
      }
      tk_socket_close(sock);
    } else {
      log_debug("connect to %s %d fail\n", host, port);
    }
  }

  return 0;
}
