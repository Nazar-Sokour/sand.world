
#ifndef __NS_ERR_H__
# define __NS_ERR_H__

# if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#  include <err.h>
# else
#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>

#  define err(status, ...)          fprintf(stderr, __VA_ARGS__), fprintf(stderr, ": %s\n", strerror(errno)), exit(status)
#  define errx(status, ...)         fprintf(stderr, __VA_ARGS__), exit(status)
#  define warnx(...)                fprintf(stderr, __VA_ARGS__)
# endif

#endif /* !defined(__NS_ERR_H__) */
