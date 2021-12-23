#pragma once
#include <Windows.h>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

namespace shared::nt {
	constexpr auto STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;

	constexpr auto SystemModuleInformation = 11;

	typedef NTSTATUS( *NtLoadDriver )( PUNICODE_STRING DriverServiceName );
	typedef NTSTATUS( *NtUnloadDriver )( PUNICODE_STRING DriverServiceName );

	typedef struct _RTL_PROCESS_MODULE_INFORMATION {
		HANDLE Section;
		PVOID MappedBase;
		PVOID ImageBase;
		ULONG ImageSize;
		ULONG Flags;
		USHORT LoadOrderIndex;
		USHORT InitOrderIndex;
		USHORT LoadCount;
		USHORT OffsetToFileName;
		UCHAR FullPathName[ 256 ];
	} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

	typedef struct _RTL_PROCESS_MODULES {
		ULONG NumberOfModules;
		RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
	} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;
}
