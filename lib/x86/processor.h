#ifndef LIBCFLAT_PROCESSOR_H
#define LIBCFLAT_PROCESSOR_H

#include "libcflat.h"
#include <stdint.h>

struct descriptor_table_ptr {
    u16 limit;
    ulong base;
} __attribute__((packed));

static inline void barrier(void)
{
    asm volatile ("" : : : "memory");
}

static inline u16 read_cs(void)
{
    unsigned val;

    asm ("mov %%cs, %0" : "=mr"(val));
    return val;
}

static inline u16 read_ds(void)
{
    unsigned val;

    asm ("mov %%ds, %0" : "=mr"(val));
    return val;
}

static inline u16 read_es(void)
{
    unsigned val;

    asm ("mov %%es, %0" : "=mr"(val));
    return val;
}

static inline u16 read_ss(void)
{
    unsigned val;

    asm ("mov %%ss, %0" : "=mr"(val));
    return val;
}

static inline u16 read_fs(void)
{
    unsigned val;

    asm ("mov %%fs, %0" : "=mr"(val));
    return val;
}

static inline u16 read_gs(void)
{
    unsigned val;

    asm ("mov %%gs, %0" : "=mr"(val));
    return val;
}

static inline void write_ds(unsigned val)
{
    asm ("mov %0, %%ds" : : "rm"(val) : "memory");
}

static inline void write_es(unsigned val)
{
    asm ("mov %0, %%es" : : "rm"(val) : "memory");
}

static inline void write_ss(unsigned val)
{
    asm ("mov %0, %%ss" : : "rm"(val) : "memory");
}

static inline void write_fs(unsigned val)
{
    asm ("mov %0, %%fs" : : "rm"(val) : "memory");
}

static inline void write_gs(unsigned val)
{
    asm ("mov %0, %%gs" : : "rm"(val) : "memory");
}

static inline u64 rdmsr(u32 index)
{
    u32 a, d;
    asm volatile ("rdmsr" : "=a"(a), "=d"(d) : "c"(index) : "memory");
    return a | ((u64)d << 32);
}

static inline void wrmsr(u32 index, u64 val)
{
    u32 a = val, d = val >> 32;
    asm volatile ("wrmsr" : : "a"(a), "d"(d), "c"(index) : "memory");
}

static inline uint64_t rdpmc(uint32_t index)
{
    uint32_t a, d;
    asm volatile ("rdpmc" : "=a"(a), "=d"(d) : "c"(index));
    return a | ((uint64_t)d << 32);
}

static inline void write_cr0(ulong val)
{
    asm volatile ("mov %0, %%cr0" : : "r"(val) : "memory");
}

static inline ulong read_cr0(void)
{
    ulong val;
    asm volatile ("mov %%cr0, %0" : "=r"(val) : : "memory");
    return val;
}

static inline void write_cr2(ulong val)
{
    asm volatile ("mov %0, %%cr2" : : "r"(val) : "memory");
}

static inline ulong read_cr2(void)
{
    ulong val;
    asm volatile ("mov %%cr2, %0" : "=r"(val) : : "memory");
    return val;
}

static inline void write_cr3(ulong val)
{
    asm volatile ("mov %0, %%cr3" : : "r"(val) : "memory");
}

static inline ulong read_cr3(void)
{
    ulong val;
    asm volatile ("mov %%cr3, %0" : "=r"(val) : : "memory");
    return val;
}

static inline void write_cr4(ulong val)
{
    asm volatile ("mov %0, %%cr4" : : "r"(val) : "memory");
}

static inline ulong read_cr4(void)
{
    ulong val;
    asm volatile ("mov %%cr4, %0" : "=r"(val) : : "memory");
    return val;
}

static inline void write_cr8(ulong val)
{
    asm volatile ("mov %0, %%cr8" : : "r"(val) : "memory");
}

static inline ulong read_cr8(void)
{
    ulong val;
    asm volatile ("mov %%cr8, %0" : "=r"(val) : : "memory");
    return val;
}

static inline void lgdt(const struct descriptor_table_ptr *ptr)
{
    asm volatile ("lgdt %0" : : "m"(*ptr));
}

static inline void sgdt(struct descriptor_table_ptr *ptr)
{
    asm volatile ("sgdt %0" : "=m"(*ptr));
}

static inline void lidt(const struct descriptor_table_ptr *ptr)
{
    asm volatile ("lidt %0" : : "m"(*ptr));
}

static inline void sidt(struct descriptor_table_ptr *ptr)
{
    asm volatile ("sidt %0" : "=m"(*ptr));
}

static inline void lldt(unsigned val)
{
    asm volatile ("lldt %0" : : "rm"(val));
}

static inline u16 sldt(void)
{
    u16 val;
    asm volatile ("sldt %0" : "=rm"(val));
    return val;
}

static inline void ltr(u16 val)
{
    asm volatile ("ltr %0" : : "rm"(val));
}

static inline u16 str(void)
{
    u16 val;
    asm volatile ("str %0" : "=rm"(val));
    return val;
}

static inline void write_dr6(ulong val)
{
    asm volatile ("mov %0, %%dr6" : : "r"(val) : "memory");
}

static inline ulong read_dr6(void)
{
    ulong val;
    asm volatile ("mov %%dr6, %0" : "=r"(val));
    return val;
}

static inline void write_dr7(ulong val)
{
    asm volatile ("mov %0, %%dr7" : : "r"(val) : "memory");
}

static inline ulong read_dr7(void)
{
    ulong val;
    asm volatile ("mov %%dr7, %0" : "=r"(val));
    return val;
}

struct cpuid { u32 a, b, c, d; };

static inline struct cpuid cpuid_indexed(u32 function, u32 index)
{
    struct cpuid r;
    asm volatile ("cpuid"
                  : "=a"(r.a), "=b"(r.b), "=c"(r.c), "=d"(r.d)
                  : "0"(function), "2"(index));
    return r;
}

static inline struct cpuid cpuid(u32 function)
{
    return cpuid_indexed(function, 0);
}

static inline void pause(void)
{
    asm volatile ("pause");
}

static inline void cli(void)
{
    asm volatile ("cli");
}

static inline void sti(void)
{
    asm volatile ("sti");
}

static inline unsigned long long rdtsc()
{
	long long r;

#ifdef __x86_64__
	unsigned a, d;

	asm volatile ("rdtsc" : "=a"(a), "=d"(d));
	r = a | ((long long)d << 32);
#else
	asm volatile ("rdtsc" : "=A"(r));
#endif
	return r;
}

static inline void wrtsc(u64 tsc)
{
	unsigned a = tsc, d = tsc >> 32;

	asm volatile("wrmsr" : : "a"(a), "d"(d), "c"(0x10));
}

static inline void irq_disable(void)
{
    asm volatile("cli");
}

static inline void irq_enable(void)
{
    asm volatile("sti");
}

static inline void invlpg(void *va)
{
	asm volatile("invlpg (%0)" ::"r" (va) : "memory");
}

static inline void safe_halt(void)
{
	asm volatile("sti; hlt");
}

#ifdef __x86_64__
static inline u64 read_rflags(void)
{
	u64 r;
	asm volatile("pushf; pop %0\n\t" : "=q"(r) : : "cc");
	return r;
}

static inline void write_rflags(u64 r)
{
	asm volatile("push %0; popf\n\t" : : "q"(r) : "cc");
}
#endif

#endif
