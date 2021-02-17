#include <fcntl.h>
#include <libzfs.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int handle_event(libzfs_handle_t *h, int zevent_fd) {
  nvlist_t *nvl;
  int n_dropped;
  int rv;

  rv = zpool_events_next(h, &nvl, &n_dropped,
                         0x0, // ZEVENT_NONE ... not a public const??
                         zevent_fd);

  if ((rv != 0) || !nvl) {
    return errno;
  }

  if (n_dropped > 0) {
    printf("{\"dropped_events\": %d}\n", n_dropped);
    fprintf(stderr, "Missed %d events", n_dropped);
  }

  nvlist_print_json(stdout, nvl);
  printf("\n");
  fflush(stdout);

  nvlist_free(nvl);
  return 0;
}

int main(int argc, char *argv[]) {
  libzfs_handle_t *h;
  h = libzfs_init();

  if (h == NULL) {
    fprintf(stderr, "libzfs_init() failed with errno=%d\n", errno);
    return 1;
  }

  printf("init'd\n");

  int zevent_fd;
  zevent_fd = open(ZFS_DEV, O_RDWR);
  if (zevent_fd < 0) {
    fprintf(stderr, "Failed to open '%s': %s\n", ZFS_DEV, strerror(errno));
    return 1;
  }

  int handler_ret;
  while (1) {
    handler_ret = handle_event(h, zevent_fd);
    if (handler_ret != 0) {
      fprintf(stderr, "Event handler was unhappy with errno %d: %s\n",
              handler_ret, strerror(handler_ret));
      break;
    }
  }

  close(zevent_fd);

  libzfs_fini(h);
  return (0);
}
