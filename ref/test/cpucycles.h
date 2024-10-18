#ifndef CPUCYCLES_H
#define CPUCYCLES_H

#include <stdint.h>

#ifdef USE_RDPMC  /* Needs echo 2 > /sys/devices/cpu/rdpmc */

static inline uint64_t cpucycles(void) {
  const uint32_t ecx = (1U << 30) + 1;
  uint64_t result;

  __asm__ volatile ("rdpmc; shlq $32,%%rdx; orq %%rdx,%%rax"
    : "=a" (result) : "c" (ecx) : "rdx");

  return result;
}

#elif defined (__x86_64__)  /* Available on all x86_64 cpus */

static inline uint64_t cpucycles(void) {
  uint64_t result;

  __asm__ volatile ("rdtsc; shlq $32,%%rdx; orq %%rdx,%%rax"
    : "=a" (result) : : "%rdx");

  return result;
}

// sudo insmod pmu_el0_cycle_counter.ko
#elif defined(__aarch64__)

static inline uint64_t cpucycles(void) {
  uint64_t result;

  asm volatile("mrs %0, pmccntr_el0"
               : "=r"(result));

  return result;
}

#else

static inline uint64_t cpucycles(void) {
  fprintf(stderr, "cpucycles not implemented for this architecture\n");
  exit(1);
}

#endif

uint64_t cpucycles_overhead(void);

#endif
