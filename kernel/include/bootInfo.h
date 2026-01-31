#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "types.h"

typedef enum {
	ReservedMemoryType,
	LoaderCode,
	LoaderData,
	BootServicesCode,
	BootServicesData,
	RuntimeServicesCode,
	RuntimeServicesData,
	ConventionalMemory,
	UnusableMemory,
	ACPIReclaimMemory,
	ACPIMemoryNVS,
	MemoryMappedIO,
	MemoryMappedIOPortSpace,
	PalCode,
	PersistentMemory,
	UnacceptedMemoryType,
	MaxMemoryType
} MemoryType;

typedef struct {
	uint32_t		Type;
	uint64_t		PhysicalStart;
	uint64_t		VirtualStart;
	uint64_t		NumberOfPages;
	uint64_t		Attribute;
} MemoryDescriptor;

typedef struct {
    uint32_t Width;
    uint32_t Height;
    uint32_t PixelsPerScanLine;
    uint64_t FrameBufferBase;
} KernelGOPInfo;

typedef struct {
	MemoryDescriptor	*mem_map;
	uint64_t			mem_map_size;
	uint64_t			desc_size;
} KernelMemMapInfo;

typedef struct {
    KernelGOPInfo    *kgi;
	KernelMemMapInfo *kmmi;
} BootInfo;

#endif // BOOT_INFO_H
