#include <cstdio>
#include <unistd.h>
#include "erl_comm.h"

/* erl_comm.c */

typedef unsigned char byte;

int read_cmd(byte** buf, int& buffer_size)
{
  int len;

  if (read_exact(*buf, 2) != 2)
    return(-1);
  len = ((*buf)[0] << 8) | (*buf)[1];

  if(len > buffer_size){
    buffer_size = len;
    delete[] *buf;
    *buf = new byte[buffer_size];
  }

  return read_exact(*buf, len);
}

int write_cmd(byte *buf, int len)
{
  byte li;

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);
  
  li = len & 0xff;
  write_exact(&li, 1);

  return write_exact(buf, len);
}

int read_exact(byte *buf, int len)
{
  int i, got=0;

  do {
    if ((i = read(0, buf+got, len-got)) <= 0)
      return(i);
    got += i;
  } while (got<len);

  return(len);
}

int write_exact(byte *buf, int len)
{
  int i, wrote = 0;

  do {
    if ((i = write(1, buf+wrote, len-wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote<len);

  return (len);
}
