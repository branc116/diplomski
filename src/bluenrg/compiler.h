#ifndef COMPILER_H
#define COMPILER_H

#ifdef __GNUC__
#undef __packed
#define __packed
#define PACKED __attribute__((packed))
#else
#define PACKED
#endif /* __GNUC__ */

#endif /* COMPILER_H */

