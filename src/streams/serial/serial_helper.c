﻿/**
 * File:   serial_helper.c
 * Author: AWTK Develop Team
 * Brief:  serial helper functions
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-09-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

/*
 *the following code adapted from: https://github.com/wjwwood/serial/tree/master/src/impl
 *
 * Copyright 2012 William Woodall and John Harrison
 *
 * Additional Contributors: Christopher Baker @bakercp
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/wstr.h"
#include "tkc/thread.h"
#include "tkc/time_now.h"
#include "tkc/socket_pair.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

#ifdef WIN32
#define prefix L"\\\\.\\"

static ret_t serial_wait_for_data_impl(serial_handle_t handle, uint32_t timeout_ms);

int serial_handle_get_fd(serial_handle_t handle) {
  return handle->client_fd;
}

serial_dev_t serial_handle_get_dev(serial_handle_t handle) {
  return handle->dev;
}

static ret_t serial_cond_var_wait(serial_handle_t handle, uint32_t timeout_ms) {
  ret_t ret = RET_OK;

  tk_mutex_lock(handle->mutex);
  if (!handle->has_signal) {
    ret = tk_cond_wait_timeout(handle->cond, handle->mutex, timeout_ms);
  }
  handle->has_signal = FALSE;
  tk_mutex_unlock(handle->mutex);

  return ret;
}

static ret_t serial_cond_var_awake(serial_handle_t handle) {
  return_value_if_fail(handle->cond != NULL && handle->mutex, RET_BAD_PARAMS);

  tk_mutex_lock(handle->mutex);
  handle->has_signal = TRUE;
  tk_mutex_unlock(handle->mutex);
  tk_cond_signal(handle->cond);

  return RET_OK;
}

static void* serial_thread_entry(void* args) {
  char buff[1] = {0};
  serial_handle_t handle = (serial_handle_t)args;

  int fd = handle->server_fd;

  while (!handle->closed) {
    if (serial_wait_for_data_impl(handle, 100) == RET_OK) {
      while (send(fd, buff, sizeof(buff), 0) <= 0) {
        log_warn("send fb buff fail \r\n");
      }
      serial_cond_var_wait(handle, 0xFFFFFFFF);
    }
  }

  return NULL;
}

serial_handle_t serial_open(const char* port) {
  wstr_t str;
  serial_dev_t dev = 0;
  int socks[2] = {0, 0};
  serial_handle_t handle = NULL;
  return_value_if_fail(port != NULL && *port != '\0', NULL);

  wstr_init(&str, 256);
  wstr_set_utf8(&str, port);
  if (strstr(port, "\\\\.\\") != port) {
    wstr_insert(&str, 0, prefix, wcslen(prefix));
  }

  dev = CreateFileW(str.str, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
  wstr_reset(&str);
  return_value_if_fail(dev != INVALID_HANDLE_VALUE, NULL);

  handle = TKMEM_ZALLOC(serial_info_t);
  return_value_if_fail(handle != NULL, NULL);
  handle->dev = dev;

  ZeroMemory(&handle->read_overlapped, sizeof(OVERLAPPED));
  ZeroMemory(&handle->write_overlapped, sizeof(OVERLAPPED));

  handle->read_overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  handle->write_overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  handle->cond = tk_cond_create();
  handle->mutex = tk_mutex_create();
  handle->thread = tk_thread_create(serial_thread_entry, handle);

  if (!SetCommMask(dev, EV_RXCHAR) || handle->cond == NULL || handle->mutex == NULL ||
      handle->thread == NULL || tk_socketpair(socks) < 0) {
    serial_close(handle);
    return NULL;
  }

  handle->client_fd = socks[0];
  handle->server_fd = socks[1];
  tk_thread_start(handle->thread);

  return handle;
}

ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity) {
  DCB dcbSerialParams = {0};
  serial_dev_t dev = serial_handle_get_dev(handle);
  return_value_if_fail(handle != NULL, RET_BAD_PARAMS);

  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if (!GetCommState(dev, &dcbSerialParams)) {
    // error getting state
    log_debug("Error getting the serial port state.");
    return RET_FAIL;
  }

  // setup baud rate
  switch (baudrate) {
#ifdef CBR_0
    case 0:
      dcbSerialParams.BaudRate = CBR_0;
      break;
#endif
#ifdef CBR_50
    case 50:
      dcbSerialParams.BaudRate = CBR_50;
      break;
#endif
#ifdef CBR_75
    case 75:
      dcbSerialParams.BaudRate = CBR_75;
      break;
#endif
#ifdef CBR_110
    case 110:
      dcbSerialParams.BaudRate = CBR_110;
      break;
#endif
#ifdef CBR_134
    case 134:
      dcbSerialParams.BaudRate = CBR_134;
      break;
#endif
#ifdef CBR_150
    case 150:
      dcbSerialParams.BaudRate = CBR_150;
      break;
#endif
#ifdef CBR_200
    case 200:
      dcbSerialParams.BaudRate = CBR_200;
      break;
#endif
#ifdef CBR_300
    case 300:
      dcbSerialParams.BaudRate = CBR_300;
      break;
#endif
#ifdef CBR_600
    case 600:
      dcbSerialParams.BaudRate = CBR_600;
      break;
#endif
#ifdef CBR_1200
    case 1200:
      dcbSerialParams.BaudRate = CBR_1200;
      break;
#endif
#ifdef CBR_1800
    case 1800:
      dcbSerialParams.BaudRate = CBR_1800;
      break;
#endif
#ifdef CBR_2400
    case 2400:
      dcbSerialParams.BaudRate = CBR_2400;
      break;
#endif
#ifdef CBR_4800
    case 4800:
      dcbSerialParams.BaudRate = CBR_4800;
      break;
#endif
#ifdef CBR_7200
    case 7200:
      dcbSerialParams.BaudRate = CBR_7200;
      break;
#endif
#ifdef CBR_9600
    case 9600:
      dcbSerialParams.BaudRate = CBR_9600;
      break;
#endif
#ifdef CBR_14400
    case 14400:
      dcbSerialParams.BaudRate = CBR_14400;
      break;
#endif
#ifdef CBR_19200
    case 19200:
      dcbSerialParams.BaudRate = CBR_19200;
      break;
#endif
#ifdef CBR_28800
    case 28800:
      dcbSerialParams.BaudRate = CBR_28800;
      break;
#endif
#ifdef CBR_57600
    case 57600:
      dcbSerialParams.BaudRate = CBR_57600;
      break;
#endif
#ifdef CBR_76800
    case 76800:
      dcbSerialParams.BaudRate = CBR_76800;
      break;
#endif
#ifdef CBR_38400
    case 38400:
      dcbSerialParams.BaudRate = CBR_38400;
      break;
#endif
#ifdef CBR_115200
    case 115200:
      dcbSerialParams.BaudRate = CBR_115200;
      break;
#endif
#ifdef CBR_128000
    case 128000:
      dcbSerialParams.BaudRate = CBR_128000;
      break;
#endif
#ifdef CBR_153600
    case 153600:
      dcbSerialParams.BaudRate = CBR_153600;
      break;
#endif
#ifdef CBR_230400
    case 230400:
      dcbSerialParams.BaudRate = CBR_230400;
      break;
#endif
#ifdef CBR_256000
    case 256000:
      dcbSerialParams.BaudRate = CBR_256000;
      break;
#endif
#ifdef CBR_460800
    case 460800:
      dcbSerialParams.BaudRate = CBR_460800;
      break;
#endif
#ifdef CBR_921600
    case 921600:
      dcbSerialParams.BaudRate = CBR_921600;
      break;
#endif
    default:
      // Try to blindly assign it
      dcbSerialParams.BaudRate = baudrate;
  }

  // setup char len
  if (bytesize == eightbits)
    dcbSerialParams.ByteSize = 8;
  else if (bytesize == sevenbits)
    dcbSerialParams.ByteSize = 7;
  else if (bytesize == sixbits)
    dcbSerialParams.ByteSize = 6;
  else if (bytesize == fivebits)
    dcbSerialParams.ByteSize = 5;
  else
    log_debug("invalid char len");

  // setup stopbits
  if (stopbits == stopbits_one)
    dcbSerialParams.StopBits = ONESTOPBIT;
  else if (stopbits == stopbits_one_point_five)
    dcbSerialParams.StopBits = ONE5STOPBITS;
  else if (stopbits == stopbits_two)
    dcbSerialParams.StopBits = TWOSTOPBITS;
  else
    log_debug("invalid stop bit");

  // setup parity
  if (parity == parity_none) {
    dcbSerialParams.Parity = NOPARITY;
  } else if (parity == parity_even) {
    dcbSerialParams.Parity = EVENPARITY;
  } else if (parity == parity_odd) {
    dcbSerialParams.Parity = ODDPARITY;
  } else if (parity == parity_mark) {
    dcbSerialParams.Parity = MARKPARITY;
  } else if (parity == parity_space) {
    dcbSerialParams.Parity = SPACEPARITY;
  } else {
    log_debug("invalid parity");
  }

  // setup flowcontrol
  if (flowcontrol == flowcontrol_none) {
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fOutX = FALSE;
    dcbSerialParams.fInX = FALSE;
  }
  if (flowcontrol == flowcontrol_software) {
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fOutX = TRUE;
    dcbSerialParams.fInX = TRUE;
  }
  if (flowcontrol == flowcontrol_hardware) {
    dcbSerialParams.fOutxCtsFlow = TRUE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_HANDSHAKE;
    dcbSerialParams.fOutX = FALSE;
    dcbSerialParams.fInX = FALSE;
  }

  // activate settings
  if (!SetCommState(dev, &dcbSerialParams)) {
    log_debug("Error setting serial port settings.");
    return RET_FAIL;
  }

  // Setup timeouts
  COMMTIMEOUTS timeouts = {0};
  timeouts.ReadIntervalTimeout = 10;
  timeouts.ReadTotalTimeoutConstant = 100;
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant = 100;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if (!SetCommTimeouts(dev, &timeouts)) {
    log_debug("Error setting timeouts.");
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t serial_iflush(serial_handle_t handle) {
  serial_dev_t dev = serial_handle_get_dev(handle);

  PurgeComm(dev, PURGE_RXCLEAR);

  return RET_OK;
}

ret_t serial_oflush(serial_handle_t handle) {
  serial_dev_t dev = serial_handle_get_dev(handle);

  PurgeComm(dev, PURGE_TXCLEAR);

  return RET_OK;
}

int serial_close(serial_handle_t handle) {
  if (handle != NULL) {
    serial_dev_t dev = serial_handle_get_dev(handle);

    serial_iflush(handle);
    serial_oflush(handle);

    if (dev != INVALID_HANDLE_VALUE) {
      CancelIo(dev);
      CloseHandle(dev);
    }
    if (handle->read_overlapped.hEvent != INVALID_HANDLE_VALUE) {
      CloseHandle(handle->read_overlapped.hEvent);
    }
    if (handle->write_overlapped.hEvent != INVALID_HANDLE_VALUE) {
      CloseHandle(handle->write_overlapped.hEvent);
    }

    handle->closed = TRUE;
    serial_cond_var_awake(handle);
    if (handle->thread != NULL) {
      tk_thread_join(handle->thread);
      tk_thread_destroy(handle->thread);
    }

    tk_socket_close(handle->client_fd);
    tk_socket_close(handle->server_fd);

    if (handle->mutex != NULL) {
      tk_mutex_destroy(handle->mutex);
    }

    if (handle->cond != NULL) {
      tk_cond_destroy(handle->cond);
    }
    memset(handle, 0x0, sizeof(*handle));
    TKMEM_FREE(handle);
  }
  return 0;
}

int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size) {
  DWORD err = 0;
  DWORD bytes = 0;
  char tmp_buff[1] = {0};
  bool_t has_signal = FALSE;
  serial_dev_t dev = serial_handle_get_dev(handle);

  if (serial_wait_for_data(handle, 0) != RET_OK) {
    return 0;
  }

  if (!ReadFile(dev, buff, max_size, &bytes, &handle->read_overlapped)) {
    err = GetLastError();
    if (err == ERROR_IO_PENDING) {
      GetOverlappedResult(dev, &handle->read_overlapped, &bytes, TRUE);
    } else {
      bytes = 0;
    }
  }
  tk_mutex_lock(handle->mutex);
  if (!handle->has_signal) {
    if (recv(handle->client_fd, tmp_buff, sizeof(tmp_buff), 0) > 0) {
      handle->has_signal = has_signal = TRUE;
    }
  }
  tk_mutex_unlock(handle->mutex);
  if (has_signal) {
    tk_cond_signal(handle->cond);
  }
  return bytes;
}

int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  DWORD err = 0;
  DWORD real_send = 0;
  serial_dev_t dev = serial_handle_get_dev(handle);
  if (!WriteFile(dev, buff, max_size, &real_send, &handle->write_overlapped)) {
    err = GetLastError();
    if (err == ERROR_IO_PENDING) {
      GetOverlappedResult(dev, &handle->write_overlapped, &real_send, TRUE);
    } else {
      real_send = 0;
    }
  }
  ClearCommError(dev, &err, NULL);
  return real_send;
}

static ret_t serial_wait_for_data_impl(serial_handle_t handle, uint32_t timeout_ms) {
  DWORD err = 0;
  DWORD mask = 0;
  COMSTAT cstate = {0};
  ret_t ret = RET_FAIL;
  serial_dev_t dev = serial_handle_get_dev(handle);
  if (!WaitCommEvent(dev, &mask, &handle->read_overlapped)) {
    err = GetLastError();
    if (err == ERROR_IO_PENDING) {
      err = WaitForSingleObject(handle->read_overlapped.hEvent, timeout_ms);
      if (err == WAIT_OBJECT_0) {
        ret = RET_OK;
      } else if (err == WAIT_TIMEOUT) {
        ret = RET_TIMEOUT;
      } else {
        ret = RET_FAIL;
      }
    }
  }
  /* 清除可能的异常以及其事件，同时获取缓冲区的数据个数 */
  ClearCommError(dev, &err, &cstate);

  if (cstate.cbInQue > 0) {
    /* 如果缓冲区有数据的话，应该触发 serial_wait_for_data 让其他取缓冲区的数据。*/
    ret = RET_OK;
  } else if (ret == RET_OK) {
    sleep_ms(timeout_ms);
    /* 如果缓冲区没有数据的话，但是却触发了 WaitForSingleObject 触发成功了，说明串口通信有异常，则应该返回 RET_FAIL。*/
    ret = RET_FAIL;
  }
  return ret;
}

ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms) {
  int fd = serial_handle_get_fd(handle);
  return tk_socket_wait_for_data(fd, timeout_ms);
}

#else

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <errno.h>
#include <paths.h>
#include <sysexits.h>
#include <termios.h>
#include <sys/param.h>
#include <pthread.h>

#if defined(__linux__)
#include <linux/serial.h>
#endif

#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

#ifdef __MACH__
#include <AvailabilityMacros.h>
#endif

#ifndef TIOCINQ
#ifdef FIONREAD
#define TIOCINQ FIONREAD
#else
#define TIOCINQ 0x541B
#endif
#endif

#if defined(MAC_OS_X_VERSION_10_3) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_3)
#if defined(IOS)
#include <sys/termios.h>
#include <sys/ttycom.h>
#define IOSSIOSPEED _IOW('T', 2, speed_t)
#else
#include <IOKit/serial/ioss.h>
#endif /*IOS*/
#endif

serial_handle_t serial_open(const char* port) {
  serial_handle_t handle = TKMEM_ZALLOC(serial_info_t);
  return_value_if_fail(handle != NULL && port != NULL && *port != '\0', NULL);
  handle->dev = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (handle->dev <= 0) {
    TKMEM_FREE(handle);
    return NULL;
  }

  return handle;
}

ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity) {
  bool_t xonxoff = FALSE;
  bool_t rtscts = FALSE;
  int32_t byte_time_ns = 0;
  struct termios options;
  serial_dev_t dev = serial_handle_get_dev(handle);
  return_value_if_fail(dev >= 0, RET_BAD_PARAMS);

  return_value_if_fail(tcgetattr(dev, &options) >= 0, RET_BAD_PARAMS);

  options.c_cflag |= (tcflag_t)(CLOCAL | CREAD);
  options.c_lflag &=
      (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN);  //|ECHOPRT

  options.c_oflag &= (tcflag_t) ~(OPOST);
  options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);

#ifdef IUCLC
  options.c_iflag &= (tcflag_t)~IUCLC;
#endif
#ifdef PARMRK
  options.c_iflag &= (tcflag_t)~PARMRK;
#endif

  // setup baud rate
  bool_t custom_baud = FALSE;
  speed_t baud;
  switch (baudrate) {
#ifdef B0
    case 0:
      baud = B0;
      break;
#endif
#ifdef B50
    case 50:
      baud = B50;
      break;
#endif
#ifdef B75
    case 75:
      baud = B75;
      break;
#endif
#ifdef B110
    case 110:
      baud = B110;
      break;
#endif
#ifdef B134
    case 134:
      baud = B134;
      break;
#endif
#ifdef B150
    case 150:
      baud = B150;
      break;
#endif
#ifdef B200
    case 200:
      baud = B200;
      break;
#endif
#ifdef B300
    case 300:
      baud = B300;
      break;
#endif
#ifdef B600
    case 600:
      baud = B600;
      break;
#endif
#ifdef B1200
    case 1200:
      baud = B1200;
      break;
#endif
#ifdef B1800
    case 1800:
      baud = B1800;
      break;
#endif
#ifdef B2400
    case 2400:
      baud = B2400;
      break;
#endif
#ifdef B4800
    case 4800:
      baud = B4800;
      break;
#endif
#ifdef B7200
    case 7200:
      baud = B7200;
      break;
#endif
#ifdef B9600
    case 9600:
      baud = B9600;
      break;
#endif
#ifdef B14400
    case 14400:
      baud = B14400;
      break;
#endif
#ifdef B19200
    case 19200:
      baud = B19200;
      break;
#endif
#ifdef B28800
    case 28800:
      baud = B28800;
      break;
#endif
#ifdef B57600
    case 57600:
      baud = B57600;
      break;
#endif
#ifdef B76800
    case 76800:
      baud = B76800;
      break;
#endif
#ifdef B38400
    case 38400:
      baud = B38400;
      break;
#endif
#ifdef B115200
    case 115200:
      baud = B115200;
      break;
#endif
#ifdef B128000
    case 128000:
      baud = B128000;
      break;
#endif
#ifdef B153600
    case 153600:
      baud = B153600;
      break;
#endif
#ifdef B230400
    case 230400:
      baud = B230400;
      break;
#endif
#ifdef B256000
    case 256000:
      baud = B256000;
      break;
#endif
#ifdef B460800
    case 460800:
      baud = B460800;
      break;
#endif
#ifdef B500000
    case 500000:
      baud = B500000;
      break;
#endif
#ifdef B576000
    case 576000:
      baud = B576000;
      break;
#endif
#ifdef B921600
    case 921600:
      baud = B921600;
      break;
#endif
#ifdef B1000000
    case 1000000:
      baud = B1000000;
      break;
#endif
#ifdef B1152000
    case 1152000:
      baud = B1152000;
      break;
#endif
#ifdef B1500000
    case 1500000:
      baud = B1500000;
      break;
#endif
#ifdef B2000000
    case 2000000:
      baud = B2000000;
      break;
#endif
#ifdef B2500000
    case 2500000:
      baud = B2500000;
      break;
#endif
#ifdef B3000000
    case 3000000:
      baud = B3000000;
      break;
#endif
#ifdef B3500000
    case 3500000:
      baud = B3500000;
      break;
#endif
#ifdef B4000000
    case 4000000:
      baud = B4000000;
      break;
#endif
    default:
      custom_baud = TRUE;
      // OS X support
#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
      // Starting with Tiger, the IOSSIOSPEED ioctl can be used to set arbitrary baud rates
      // other than those specified by POSIX. The driver for the underlying serial hardware
      // ultimately determines which baud rates can be used. This ioctl sets both the input
      // and output speed.
      speed_t new_baud = (speed_t)(baudrate);
      return_value_if_fail(ioctl(dev, IOSSIOSPEED, &new_baud, 1) >= 0, RET_FAIL);
      // Linux Support
#elif defined(__linux__) && defined(TIOCSSERIAL)
      struct serial_struct ser;

      return_value_if_fail(ioctl(dev, TIOCGSERIAL, &ser) >= 0, RET_FAIL);

      // set custom divisor
      ser.custom_divisor = ser.baud_base / (int)(baudrate);
      // update flags
      ser.flags &= ~ASYNC_SPD_MASK;
      ser.flags |= ASYNC_SPD_CUST;

      return_value_if_fail(ioctl(dev, TIOCSSERIAL, &ser) >= 0, RET_FAIL);
#else
      log_debug("OS does not currently support custom bauds");
      return RET_FAIL;
#endif
  }
  if (custom_baud == FALSE) {
#ifdef _BSD_SOURCE
    cfsetspeed(&options, baud);
#else
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
#endif
  }

  // setup char len
  options.c_cflag &= (tcflag_t)~CSIZE;
  if (bytesize == eightbits)
    options.c_cflag |= CS8;
  else if (bytesize == sevenbits)
    options.c_cflag |= CS7;
  else if (bytesize == sixbits)
    options.c_cflag |= CS6;
  else if (bytesize == fivebits)
    options.c_cflag |= CS5;
  else
    log_debug("invalid char len");
  // setup stopbits
  if (stopbits == stopbits_one)
    options.c_cflag &= (tcflag_t) ~(CSTOPB);
  else if (stopbits == stopbits_one_point_five)
    // ONE POINT FIVE same as TWO.. there is no POSIX support for 1.5
    options.c_cflag |= (CSTOPB);
  else if (stopbits == stopbits_two)
    options.c_cflag |= (CSTOPB);
  else
    log_debug("invalid stop bit");
  // setup parity
  options.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP);
  if (parity == parity_none) {
    options.c_cflag &= (tcflag_t) ~(PARENB | PARODD);
  } else if (parity == parity_even) {
    options.c_cflag &= (tcflag_t) ~(PARODD);
    options.c_cflag |= (PARENB);
  } else if (parity == parity_odd) {
    options.c_cflag |= (PARENB | PARODD);
  }
#ifdef CMSPAR
  else if (parity == parity_mark) {
    options.c_cflag |= (PARENB | CMSPAR | PARODD);
  } else if (parity == parity_space) {
    options.c_cflag |= (PARENB | CMSPAR);
    options.c_cflag &= (tcflag_t) ~(PARODD);
  }
#else
  // CMSPAR is not defined on OSX. So do not support mark or space parity.
  else if (parity == parity_mark || parity == parity_space) {
    log_debug("OS does not support mark or space parity");
  }
#endif  // ifdef CMSPAR
  else {
    log_debug("invalid parity");
  }
  // setup flow control
  if (flowcontrol == flowcontrol_none) {
    xonxoff = FALSE;
    rtscts = FALSE;
  }
  if (flowcontrol == flowcontrol_software) {
    xonxoff = TRUE;
    rtscts = FALSE;
  }
  if (flowcontrol == flowcontrol_hardware) {
    xonxoff = FALSE;
    rtscts = TRUE;
  }
  // xonxoff
#ifdef IXANY
  if (xonxoff)
    options.c_iflag |= (IXON | IXOFF);  //|IXANY)
  else
    options.c_iflag &= (tcflag_t) ~(IXON | IXOFF | IXANY);
#else
  if (xonxoff)
    options.c_iflag |= (IXON | IXOFF);
  else
    options.c_iflag &= (tcflag_t) ~(IXON | IXOFF);
#endif
    // rtscts
#ifdef CRTSCTS
  if (rtscts)
    options.c_cflag |= (CRTSCTS);
  else
    options.c_cflag &= (unsigned long)~(CRTSCTS);
#elif defined CNEW_RTSCTS
  if (rtscts)
    options.c_cflag |= (CNEW_RTSCTS);
  else
    options.c_cflag &= (unsigned long)~(CNEW_RTSCTS);
#else
#error "OS Support seems wrong."
#endif

  // http://www.unixwiz.net/techtips/termios-vmin-vtime.html
  // this basically sets the read call up to be a polling read,
  // but we are using select to ensure there is data available
  // to read before each call, so we should never needlessly poll
  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 0;

  // activate settings
  tcsetattr(dev, TCSANOW, &options);

  // Update byte_time based on the new settings.
  uint32_t bit_time_ns = 1e9 / baudrate;
  byte_time_ns = bit_time_ns * (1 + bytesize + parity + stopbits);

  // Compensate for the stopbits_one_point_five enum being equal to int 3,
  // and not 1.5.
  if (stopbits == stopbits_one_point_five) {
    byte_time_ns += ((1.5 - stopbits_one_point_five) * bit_time_ns);
  }

  return RET_OK;
}

ret_t serial_iflush(serial_handle_t handle) {
  serial_dev_t dev = serial_handle_get_dev(handle);
  return tcflush(dev, TCIFLUSH) == 0 ? RET_OK : RET_FAIL;
}

ret_t serial_oflush(serial_handle_t handle) {
  serial_dev_t dev = serial_handle_get_dev(handle);
  return tcflush(dev, TCOFLUSH) == 0 ? RET_OK : RET_FAIL;
}

int serial_close(serial_handle_t handle) {
  serial_dev_t dev = serial_handle_get_dev(handle);

  serial_iflush(handle);
  serial_oflush(handle);

  close(dev);
  memset(handle, 0x0, sizeof(*handle));
  TKMEM_FREE(handle);

  return 0;
}

int serial_handle_get_fd(serial_handle_t handle) {
  return handle->dev;
}

serial_dev_t serial_handle_get_dev(serial_handle_t handle) {
  return handle->dev;
}

int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);

  return read(fd, buff, max_size);
}

int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);

  return write(fd, buff, max_size);
}

ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms) {
  int fd = serial_handle_get_fd(handle);

  return tk_socket_wait_for_data(fd, timeout_ms);
}

ret_t serial_timeout_set(serial_handle_t handle, serial_timeout_t* timeout) {
  return RET_NOT_IMPL;
}

ret_t serial_timeout_get(serial_handle_t handle, serial_timeout_t* timeout) {
  return RET_NOT_IMPL;
}

#endif /*WIN32*/

stopbits_t serial_stopbits_from_str(const char* str) {
  if (str == NULL) {
    return stopbits_one;
  }

  if (strstr(str, "two") != NULL || strchr(str, '2') != NULL) {
    return stopbits_two;
  } else if (strstr(str, "five") != NULL || strchr(str, '5') != NULL) {
    return stopbits_one_point_five;
  } else {
    return stopbits_one;
  }
}

flowcontrol_t serial_flowcontrol_from_str(const char* str) {
  if (str == NULL) {
    return flowcontrol_none;
  }

  if (strstr(str, "soft") != NULL) {
    return flowcontrol_software;
  } else if (strstr(str, "hard") != NULL) {
    return flowcontrol_hardware;
  } else {
    return flowcontrol_none;
  }
}

parity_t serial_parity_from_str(const char* str) {
  if (str == NULL) {
    return parity_none;
  }

  if (strstr(str, "odd") != NULL) {
    return parity_odd;
  } else if (strstr(str, "even") != NULL) {
    return parity_even;
  } else if (strstr(str, "mark") != NULL) {
    return parity_mark;
  } else if (strstr(str, "space") != NULL) {
    return parity_space;
  } else {
    return parity_none;
  }
}

bytesize_t serial_bytesize_from_str(const char* str) {
  if (str == NULL) {
    return eightbits;
  }

  if (strstr(str, "eight") != NULL || strchr(str, '8') != NULL) {
    return eightbits;
  } else if (strstr(str, "seven") != NULL || strchr(str, '7') != NULL) {
    return sevenbits;
  } else if (strstr(str, "six") != NULL || strchr(str, '6') != NULL) {
    return sixbits;
  } else if (strstr(str, "five") != NULL || strchr(str, '5') != NULL) {
    return fivebits;
  }

  return eightbits;
}
