#ifdef __CYGWIN__
#include <string.h>
//
// Implement inet_ntop on cygwin, which only support inet_ntoa etc
//

static inline
const char *
inet_ntop(int af, const void *src, char *dst, socklen_t cnt)
{
  struct in_addr copy;
  memcpy(&copy, src, sizeof(struct in_addr));
  char *str = inet_ntoa(copy);
  strncpy(dst, str, cnt);
  return dst;
}

static inline void
inet_pton(int af, const void *src, struct in_addr *dst)
{
  inet_aton((const char *) src, dst);
}

#endif
  
