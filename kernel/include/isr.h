#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

typedef struct registers_struct {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t);
void set_isr_callback(uint8_t n, isr_t handler);

#endif
