#include "help.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "SensorTile.h"


//#undef errno
//extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

//register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;/* Functions */
void initialise_monitor_handles(void)
{
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
  UNUSED(pid);
  UNUSED(sig);
  errno = EINVAL;
  return -1;
}

void _exit (int status)
{
  _kill(status, -1);
  while (1) {}    /* Make sure we hang here */
}

int _read (int file, char *ptr, int len)
{
  UNUSED(file);
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    *ptr++ = (char)__io_getchar();
  }

  return len;
}

int _write(int file, char *ptr, int len)
{
  UNUSED(file);
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}


int _close(int file)
{
  UNUSED(file);
  return -1;
}

int _fstat(int file, struct stat *st)
{
  UNUSED(file);
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  UNUSED(file);
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  UNUSED(file); UNUSED(ptr); UNUSED(dir);
  return 0;
}

int _open(char *path, int flags, ...)
{
  UNUSED(path); UNUSED(flags);
  /* Pretend like we always fail */
  return -1;
}

int _wait(int *status)
{
  UNUSED(status);
  errno = ECHILD;
  return -1;
}

int _unlink(char *name)
{
  UNUSED(name);
  errno = ENOENT;
  return -1;
}

int _times(struct tms *buf)
{
  UNUSED(buf);
  return -1;
}

int _stat(char *file, struct stat *st)
{
  UNUSED(file); UNUSED(st);

  st->st_mode = S_IFCHR;
  return 0;
}

int _link(char *old, char *new)
{
  UNUSED(old); UNUSED(new);
  errno = EMLINK;
  return -1;
}

int _fork(void)
{
  errno = EAGAIN;
  return -1;
}

int _execve(char *name, char **argv, char **env)
{
  UNUSED(name); UNUSED(argv); UNUSED(env);
  errno = ENOMEM;
  return -1;
}

void Error_Handler(void) {
  volatile int i = 0;
  while(1) {
    ++i;
  }
}

extern PCD_HandleTypeDef hpcd;
extern TIM_HandleTypeDef TimHandle;

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xxxx.s)                                              */
/******************************************************************************/

/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  */
void OTG_FS_IRQHandler(void)
{
  //HAL_PCD_IRQHandler(&hpcd);
}

/**
  * @brief  This function handles TIM interrupt request
  */
void TIM3_IRQHandler(void)
{
  //HAL_TIM_IRQHandler(&TimHandle);
}

