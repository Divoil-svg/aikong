/*
 * PROJECT:   Veil
 * FILE:      Veil.h
 * PURPOSE:   Definition for the Windows Internal API from ntdll.dll,
 *            samlib.dll and winsta.dll
 *
 * LICENSE:   Relicensed under The MIT License from The CC BY 4.0 License
 *
 * DEVELOPER: MiroKaku (50670906+MiroKaku@users.noreply.github.com)
 */

/*
 * PROJECT:   Mouri's Internal NT API Collections (MINT)
 * FILE:      MINT.h
 * PURPOSE:   Definition for the Windows Internal API from ntdll.dll,
 *            samlib.dll and winsta.dll
 *
 * LICENSE:   Relicensed under The MIT License from The CC BY 4.0 License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

/*
 * This file is part of the Process Hacker project - https://processhacker.sf.io/
 *
 * You can redistribute this file and/or modify it under the terms of the
 * Attribution 4.0 International (CC BY 4.0) license.
 *
 * You must give appropriate credit, provide a link to the license, and
 * indicate if changes were made. You may do so in any reasonable manner, but
 * not in any way that suggests the licensor endorses you or your use.
 */

#pragma once

// Warnings which disabled for compiling
#if _MSC_VER >= 1200
#pragma warning(push)
// nonstandard extension used : nameless struct/union
#pragma warning(disable:4201)
// 'struct_name' : structure was padded due to __declspec(align())
#pragma warning(disable:4324)
// 'enumeration': a forward declaration of an unscoped enumeration must have an
// underlying type (int assumed)
#pragma warning(disable:4471)
#endif

VEIL_BEGIN()
#define  SEC_IMAGE_NO_EXECUTE 0x11000000
//
// DLLs
//

typedef BOOLEAN(NTAPI* PLDR_INIT_ROUTINE)(
    _In_ PVOID DllHandle,
    _In_ ULONG Reason,
    _In_opt_ PVOID Context
    );

// symbols
typedef struct _LDR_SERVICE_TAG_RECORD
{
    struct _LDR_SERVICE_TAG_RECORD* Next;
    ULONG ServiceTag;
} LDR_SERVICE_TAG_RECORD, * PLDR_SERVICE_TAG_RECORD;

// symbols
typedef struct _LDRP_CSLIST
{
    PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, * PLDRP_CSLIST;

// symbols
typedef enum _LDR_DDAG_STATE
{
    LdrModulesMerged = -5,
    LdrModulesInitError = -4,
    LdrModulesSnapError = -3,
    LdrModulesUnloaded = -2,
    LdrModulesUnloading = -1,
    LdrModulesPlaceHolder = 0,
    LdrModulesMapping = 1,
    LdrModulesMapped = 2,
    LdrModulesWaitingForDependencies = 3,
    LdrModulesSnapping = 4,
    LdrModulesSnapped = 5,
    LdrModulesCondensed = 6,
    LdrModulesReadyToInit = 7,
    LdrModulesInitializing = 8,
    LdrModulesReadyToRun = 9
} LDR_DDAG_STATE;

// symbols
typedef struct _LDR_DDAG_NODE
{
    LIST_ENTRY Modules;
    PLDR_SERVICE_TAG_RECORD ServiceTagList;
    ULONG LoadCount;
    ULONG LoadWhileUnloadingCount;
    ULONG LowestLink;
    union
    {
        LDRP_CSLIST Dependencies;
        SINGLE_LIST_ENTRY RemovalLink;
    };
    LDRP_CSLIST IncomingDependencies;
    LDR_DDAG_STATE State;
    SINGLE_LIST_ENTRY CondenseLink;
    ULONG PreorderNumber;
} LDR_DDAG_NODE, * PLDR_DDAG_NODE;

// rev
typedef struct _LDR_DEPENDENCY_RECORD
{
    SINGLE_LIST_ENTRY DependencyLink;
    PLDR_DDAG_NODE DependencyNode;
    SINGLE_LIST_ENTRY IncomingDependencyLink;
    PLDR_DDAG_NODE IncomingDependencyNode;
} LDR_DEPENDENCY_RECORD, * PLDR_DEPENDENCY_RECORD;

// symbols
typedef enum _LDR_DLL_LOAD_REASON
{
    LoadReasonStaticDependency,
    LoadReasonStaticForwarderDependency,
    LoadReasonDynamicForwarderDependency,
    LoadReasonDelayloadDependency,
    LoadReasonDynamicLoad,
    LoadReasonAsImageLoad,
    LoadReasonAsDataLoad,
    LoadReasonEnclavePrimary, // since REDSTONE3
    LoadReasonEnclaveDependency,
    LoadReasonPatchImage, // since WIN11
    LoadReasonUnknown = -1
} LDR_DLL_LOAD_REASON, * PLDR_DLL_LOAD_REASON;

typedef enum _LDR_HOT_PATCH_STATE
{
    LdrHotPatchBaseImage,
    LdrHotPatchNotApplied,
    LdrHotPatchAppliedReverse,
    LdrHotPatchAppliedForward,
    LdrHotPatchFailedToPatch,
    LdrHotPatchStateMax,
} LDR_HOT_PATCH_STATE, * PLDR_HOT_PATCH_STATE;

#define LDRP_PACKAGED_BINARY            0x00000001
#define LDRP_STATIC_LINK                0x00000002
#define LDRP_IMAGE_DLL                  0x00000004
#define LDRP_LOAD_IN_PROGRESS           0x00001000
#define LDRP_UNLOAD_IN_PROGRESS         0x00002000
#define LDRP_ENTRY_PROCESSED            0x00004000
#define LDRP_ENTRY_INSERTED             0x00008000
#define LDRP_CURRENT_LOAD               0x00010000
#define LDRP_FAILED_BUILTIN_LOAD        0x00020000
#define LDRP_DONT_CALL_FOR_THREADS      0x00040000
#define LDRP_PROCESS_ATTACH_CALLED      0x00080000
#define LDRP_DEBUG_SYMBOLS_LOADED       0x00100000
#define LDRP_IMAGE_NOT_AT_BASE          0x00200000 // Vista and below
#define LDRP_COR_IMAGE                  0x00400000
#define LDRP_DONT_RELOCATE              0x00800000 // LDR_COR_OWNS_UNMAP
#define LDRP_SYSTEM_MAPPED              0x01000000
#define LDRP_IMAGE_VERIFYING            0x02000000
#define LDRP_DRIVER_DEPENDENT_DLL       0x04000000
#define LDRP_ENTRY_NATIVE               0x08000000
#define LDRP_REDIRECTED                 0x10000000
#define LDRP_NON_PAGED_DEBUG_INFO       0x20000000
#define LDRP_MM_LOADED                  0x40000000
#define LDRP_COMPAT_DATABASE_PROCESSED  0x80000000

#define LDR_DATA_TABLE_ENTRY_SIZE_WINXP FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, DdagNode)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN7 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, BaseNameHashValue)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN8 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, ImplicitPathOptions)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN10 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, SigningLevel)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN11 sizeof(LDR_DATA_TABLE_ENTRY)

// symbols
typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    union
    {
        LIST_ENTRY InInitializationOrderLinks;
        LIST_ENTRY InProgressLinks;
    };
    PVOID DllBase;
    PLDR_INIT_ROUTINE EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    union
    {
        UCHAR FlagGroup[4];
        ULONG Flags;
        struct
        {
            ULONG PackagedBinary : 1;
            ULONG MarkedForRemoval : 1;
            ULONG ImageDll : 1;
            ULONG LoadNotificationsSent : 1;
            ULONG TelemetryEntryProcessed : 1;
            ULONG ProcessStaticImport : 1;
            ULONG InLegacyLists : 1;
            ULONG InIndexes : 1;
            ULONG ShimDll : 1;
            ULONG InExceptionTable : 1;
            ULONG ReservedFlags1 : 2;
            ULONG LoadInProgress : 1;
            ULONG LoadConfigProcessed : 1;
            ULONG EntryProcessed : 1;
            ULONG ProtectDelayLoad : 1;
            ULONG ReservedFlags3 : 2;
            ULONG DontCallForThreads : 1;
            ULONG ProcessAttachCalled : 1;
            ULONG ProcessAttachFailed : 1;
            ULONG CorDeferredValidate : 1;
            ULONG CorImage : 1;
            ULONG DontRelocate : 1;
            ULONG CorILOnly : 1;
            ULONG ChpeImage : 1;
            ULONG ChpeEmulatorImage : 1;
            ULONG ReservedFlags5 : 1;
            ULONG Redirected : 1;
            ULONG ReservedFlags6 : 2;
            ULONG CompatDatabaseProcessed : 1;
        };
    };
    USHORT ObsoleteLoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    ULONG TimeDateStamp;
    struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
    PVOID Lock; // RtlAcquireSRWLockExclusive
    PLDR_DDAG_NODE DdagNode;
    LIST_ENTRY NodeModuleLink;
    struct _LDRP_LOAD_CONTEXT* LoadContext;
    PVOID ParentDllBase;
    PVOID SwitchBackContext;
    RTL_BALANCED_NODE BaseAddressIndexNode;
    RTL_BALANCED_NODE MappingInfoIndexNode;
    ULONG_PTR OriginalBase;
    LARGE_INTEGER LoadTime;
    ULONG BaseNameHashValue;
    LDR_DLL_LOAD_REASON LoadReason;
    ULONG ImplicitPathOptions;
    ULONG ReferenceCount;
    ULONG DependentLoadFlags;
    UCHAR SigningLevel; // since REDSTONE2
    ULONG CheckSum; // since 22H1
    PVOID ActivePatchImageBase;
    LDR_HOT_PATCH_STATE HotPatchState;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

#define LDR_IS_DATAFILE(DllHandle) (((ULONG_PTR)(DllHandle)) & (ULONG_PTR)1)
#define LDR_IS_IMAGEMAPPING(DllHandle) (((ULONG_PTR)(DllHandle)) & (ULONG_PTR)2)
#define LDR_MAPPEDVIEW_TO_DATAFILE(BaseAddress) ((PVOID)(((ULONG_PTR)(BaseAddress)) | (ULONG_PTR)1))
#define LDR_MAPPEDVIEW_TO_IMAGEMAPPING(BaseAddress) ((PVOID)(((ULONG_PTR)(BaseAddress)) | (ULONG_PTR)2))
#define LDR_DATAFILE_TO_MAPPEDVIEW(DllHandle) ((PVOID)(((ULONG_PTR)(DllHandle)) & ~(ULONG_PTR)1))
#define LDR_IMAGEMAPPING_TO_MAPPEDVIEW(DllHandle) ((PVOID)(((ULONG_PTR)(DllHandle)) & ~(ULONG_PTR)2))
#define LDR_IS_RESOURCE(DllHandle) (LDR_IS_IMAGEMAPPING(DllHandle) || LDR_IS_DATAFILE(DllHandle))

#ifndef _KERNEL_MODE
NTSYSAPI
VOID
NTAPI
LdrInitializeThunk(
    _In_ PCONTEXT ContextRecord,
    _In_ PVOID Parameter
);

NTSYSAPI
NTSTATUS
NTAPI
LdrLoadDll(
    _In_opt_ PWSTR DllPath,
    _In_opt_ PULONG DllCharacteristics,
    _In_ PUNICODE_STRING DllName,
    _Out_ PVOID* DllHandle
);

NTSYSAPI
NTSTATUS
NTAPI
LdrUnloadDll(
    _In_ PVOID DllHandle
);

NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllHandle(
    _In_opt_ PWSTR DllPath,
    _In_opt_ PULONG DllCharacteristics,
    _In_ PUNICODE_STRING DllName,
    _Out_ PVOID* DllHandle
);

#define LDR_GET_DLL_HANDLE_EX_UNCHANGED_REFCOUNT 0x00000001
#define LDR_GET_DLL_HANDLE_EX_PIN                0x00000002

NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllHandleEx(
    _In_ ULONG Flags,
    _In_opt_ PWSTR DllPath,
    _In_opt_ PULONG DllCharacteristics,
    _In_ PUNICODE_STRING DllName,
    _Out_opt_ PVOID* DllHandle
);

#if (NTDDI_VERSION >= NTDDI_WIN7)
// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllHandleByMapping(
    _In_ PVOID BaseAddress,
    _Out_ PVOID* DllHandle
);

// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllHandleByName(
    _In_opt_ PUNICODE_STRING BaseDllName,
    _In_opt_ PUNICODE_STRING FullDllName,
    _Out_ PVOID* DllHandle
);
#endif

#if (NTDDI_VERSION >= NTDDI_WIN8)
// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllFullName(
    _In_ PVOID DllHandle,
    _Out_ PUNICODE_STRING FullDllName
);

// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllDirectory(
    _Out_ PUNICODE_STRING DllDirectory
);

// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrSetDllDirectory(
    _In_ PUNICODE_STRING DllDirectory
);
#endif

#define LDR_ADDREF_DLL_PIN 0x00000001

NTSYSAPI
NTSTATUS
NTAPI
LdrAddRefDll(
    _In_ ULONG Flags,
    _In_ PVOID DllHandle
);

NTSYSAPI
NTSTATUS
NTAPI
LdrGetProcedureAddress(
    _In_ PVOID DllHandle,
    _In_opt_ PANSI_STRING ProcedureName,
    _In_opt_ ULONG ProcedureNumber,
    _Out_ PVOID* ProcedureAddress
);

// rev
#define LDR_GET_PROCEDURE_ADDRESS_DONT_RECORD_FORWARDER 0x00000001

#if (NTDDI_VERSION >= NTDDI_VISTA)
// private
NTSYSAPI
NTSTATUS
NTAPI
LdrGetProcedureAddressEx(
    _In_ PVOID DllHandle,
    _In_opt_ PANSI_STRING ProcedureName,
    _In_opt_ ULONG ProcedureNumber,
    _Out_ PVOID* ProcedureAddress,
    _In_ ULONG Flags
);
#endif

NTSYSAPI
NTSTATUS
NTAPI
LdrGetKnownDllSectionHandle(
    _In_ PCWSTR DllName,
    _In_ BOOLEAN KnownDlls32,
    _Out_ PHANDLE Section
);

#if (NTDDI_VERSION >= NTDDI_WIN10)
// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrGetProcedureAddressForCaller(
    _In_ PVOID DllHandle,
    _In_opt_ PANSI_STRING ProcedureName,
    _In_opt_ ULONG ProcedureNumber,
    _Out_ PVOID* ProcedureAddress,
    _In_ ULONG Flags,
    _In_ PVOID* Callback
);
#endif

#define LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS   0x00000001
#define LDR_LOCK_LOADER_LOCK_FLAG_TRY_ONLY          0x00000002

#define LDR_LOCK_LOADER_LOCK_DISPOSITION_INVALID            0
#define LDR_LOCK_LOADER_LOCK_DISPOSITION_LOCK_ACQUIRED      1
#define LDR_LOCK_LOADER_LOCK_DISPOSITION_LOCK_NOT_ACQUIRED  2

NTSYSAPI
NTSTATUS
NTAPI
LdrLockLoaderLock(
    _In_ ULONG Flags,
    _Out_opt_ ULONG* Disposition,
    _Out_ PVOID* Cookie
);

#define LDR_UNLOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS 0x00000001

NTSYSAPI
NTSTATUS
NTAPI
LdrUnlockLoaderLock(
    _In_ ULONG Flags,
    _Inout_ PVOID Cookie
);

NTSYSAPI
NTSTATUS
NTAPI
LdrRelocateImage(
    _In_ PVOID NewBase,
    _In_opt_ PSTR LoaderName,
    _In_ NTSTATUS Success,
    _In_ NTSTATUS Conflict,
    _In_ NTSTATUS Invalid
);

NTSYSAPI
NTSTATUS
NTAPI
LdrRelocateImageWithBias(
    _In_ PVOID NewBase,
    _In_opt_ LONGLONG Bias,
    _In_opt_ PSTR LoaderName,
    _In_ NTSTATUS Success,
    _In_ NTSTATUS Conflict,
    _In_ NTSTATUS Invalid
);

NTSYSAPI
PIMAGE_BASE_RELOCATION
NTAPI
LdrProcessRelocationBlock(
    _In_ ULONG_PTR VA,
    _In_ ULONG SizeOfBlock,
    _In_ PUSHORT NextOffset,
    _In_ LONG_PTR Diff
);

NTSYSAPI
BOOLEAN
NTAPI
LdrVerifyMappedImageMatchesChecksum(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG FileLength
);

typedef VOID(NTAPI* PLDR_IMPORT_MODULE_CALLBACK)(
    _In_ PVOID Parameter,
    _In_ PSTR ModuleName
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrVerifyImageMatchesChecksum(
    _In_ HANDLE ImageFileHandle,
    _In_opt_ PLDR_IMPORT_MODULE_CALLBACK ImportCallbackRoutine,
    _In_ PVOID ImportCallbackParameter,
    _Out_opt_ PUSHORT ImageCharacteristics
);

// private
typedef struct _LDR_IMPORT_CALLBACK_INFO
{
    PLDR_IMPORT_MODULE_CALLBACK ImportCallbackRoutine;
    PVOID ImportCallbackParameter;
} LDR_IMPORT_CALLBACK_INFO, * PLDR_IMPORT_CALLBACK_INFO;

// private
typedef struct _LDR_SECTION_INFO
{
    HANDLE SectionHandle;
    ACCESS_MASK DesiredAccess;
    POBJECT_ATTRIBUTES ObjA;
    ULONG SectionPageProtection;
    ULONG AllocationAttributes;
} LDR_SECTION_INFO, * PLDR_SECTION_INFO;

// private
typedef struct _LDR_VERIFY_IMAGE_INFO
{
    ULONG Size;
    ULONG Flags;
    LDR_IMPORT_CALLBACK_INFO CallbackInfo;
    LDR_SECTION_INFO SectionInfo;
    USHORT ImageCharacteristics;
} LDR_VERIFY_IMAGE_INFO, * PLDR_VERIFY_IMAGE_INFO;

#if (NTDDI_VERSION >= NTDDI_VISTA)
// private
NTSYSAPI
NTSTATUS
NTAPI
LdrVerifyImageMatchesChecksumEx(
    _In_ HANDLE ImageFileHandle,
    _Inout_ PLDR_VERIFY_IMAGE_INFO VerifyInfo
);

// private
NTSYSAPI
NTSTATUS
NTAPI
LdrQueryModuleServiceTags(
    _In_ PVOID DllHandle,
    _Out_writes_(*BufferSize) PULONG ServiceTagBuffer,
    _Inout_ PULONG BufferSize
);
#endif

// begin_msdn:"DLL Load Notification"

#define LDR_DLL_NOTIFICATION_REASON_LOADED   1
#define LDR_DLL_NOTIFICATION_REASON_UNLOADED 2

typedef struct _LDR_DLL_LOADED_NOTIFICATION_DATA
{
    ULONG Flags;
    PUNICODE_STRING FullDllName;
    PUNICODE_STRING BaseDllName;
    PVOID DllBase;
    ULONG SizeOfImage;
} LDR_DLL_LOADED_NOTIFICATION_DATA, * PLDR_DLL_LOADED_NOTIFICATION_DATA;

typedef struct _LDR_DLL_UNLOADED_NOTIFICATION_DATA
{
    ULONG Flags;
    PCUNICODE_STRING FullDllName;
    PCUNICODE_STRING BaseDllName;
    PVOID DllBase;
    ULONG SizeOfImage;
} LDR_DLL_UNLOADED_NOTIFICATION_DATA, * PLDR_DLL_UNLOADED_NOTIFICATION_DATA;

typedef union _LDR_DLL_NOTIFICATION_DATA
{
    LDR_DLL_LOADED_NOTIFICATION_DATA Loaded;
    LDR_DLL_UNLOADED_NOTIFICATION_DATA Unloaded;
} LDR_DLL_NOTIFICATION_DATA, * PLDR_DLL_NOTIFICATION_DATA;

typedef VOID(NTAPI* PLDR_DLL_NOTIFICATION_FUNCTION)(
    _In_ ULONG NotificationReason,
    _In_ PLDR_DLL_NOTIFICATION_DATA NotificationData,
    _In_opt_ PVOID Context
    );

#if (NTDDI_VERSION >= NTDDI_VISTA)
NTSYSAPI
NTSTATUS
NTAPI
LdrRegisterDllNotification(
    _In_ ULONG Flags,
    _In_ PLDR_DLL_NOTIFICATION_FUNCTION NotificationFunction,
    _In_opt_ PVOID Context,
    _Out_ PVOID* Cookie
);

NTSYSAPI
NTSTATUS
NTAPI
LdrUnregisterDllNotification(
    _In_ PVOID Cookie
);
#endif

// end_msdn

// rev
NTSYSAPI
PUNICODE_STRING
NTAPI
LdrStandardizeSystemPath(
    _In_ PUNICODE_STRING SystemPath
);

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
typedef struct _LDR_FAILURE_DATA
{
    NTSTATUS Status;
    WCHAR DllName[0x20];
    WCHAR AdditionalInfo[0x20];
} LDR_FAILURE_DATA, * PLDR_FAILURE_DATA;

NTSYSAPI
PLDR_FAILURE_DATA
NTAPI
LdrGetFailureData(
    VOID
);
#endif

// private
typedef struct _PS_MITIGATION_OPTIONS_MAP
{
    ULONG_PTR Map[3]; // 2 < 20H1
} PS_MITIGATION_OPTIONS_MAP, * PPS_MITIGATION_OPTIONS_MAP;

// private
typedef struct _PS_MITIGATION_AUDIT_OPTIONS_MAP
{
    ULONG_PTR Map[3]; // 2 < 20H1
} PS_MITIGATION_AUDIT_OPTIONS_MAP, * PPS_MITIGATION_AUDIT_OPTIONS_MAP;

// private
typedef struct _PS_SYSTEM_DLL_INIT_BLOCK
{
    ULONG Size;
    ULONG_PTR SystemDllWowRelocation;
    ULONG_PTR SystemDllNativeRelocation;
    ULONG_PTR Wow64SharedInformation[16];
    ULONG RngData;
    union
    {
        ULONG Flags;
        struct
        {
            ULONG CfgOverride : 1;
            ULONG Reserved : 31;
        };
    };
    PS_MITIGATION_OPTIONS_MAP MitigationOptionsMap;
    ULONG_PTR CfgBitMap;
    ULONG_PTR CfgBitMapSize;
    ULONG_PTR Wow64CfgBitMap;
    ULONG_PTR Wow64CfgBitMapSize;
    PS_MITIGATION_AUDIT_OPTIONS_MAP MitigationAuditOptionsMap; // REDSTONE3
} PS_SYSTEM_DLL_INIT_BLOCK, * PPS_SYSTEM_DLL_INIT_BLOCK;

#if (NTDDI_VERSION >= NTDDI_WINTHRESHOLD)
// rev
NTSYSAPI PS_SYSTEM_DLL_INIT_BLOCK LdrSystemDllInitBlock;
#endif
#endif // _KERNEL_MODE

//
// Load as data table
//

#ifndef _KERNEL_MODE
#if (NTDDI_VERSION >= NTDDI_VISTA)
// private
NTSYSAPI
NTSTATUS
NTAPI
LdrAddLoadAsDataTable(
    _In_ PVOID Module,
    _In_ PWSTR FilePath,
    _In_ SIZE_T Size,
    _In_ HANDLE Handle
);

// private
NTSYSAPI
NTSTATUS
NTAPI
LdrRemoveLoadAsDataTable(
    _In_ PVOID InitModule,
    _Out_opt_ PVOID* BaseModule,
    _Out_opt_ PSIZE_T Size,
    _In_ ULONG Flags
);

// private
NTSYSAPI
NTSTATUS
NTAPI
LdrGetFileNameFromLoadAsDataTable(
    _In_ PVOID Module,
    _Out_ PVOID* pFileNamePrt
);
#endif

NTSYSAPI
NTSTATUS
NTAPI
LdrDisableThreadCalloutsForDll(
    _In_ PVOID DllImageBase
);
#endif // _KERNEL_MODE

//
// Resources
//

#ifdef _KERNEL_MODE
#include "Veil.System.VersionResource.h"
#endif

typedef struct _VS_VERSIONINFO_BLOCK
{
    UINT16  TotalLength;
    UINT16  ValueLength;
    UINT16  Type;                /* 1:Text, 0:Binary */
    WCHAR   Key[ANYSIZE_ARRAY];

#if 0   /* variable length structure */
    /* DWORD aligned */
    BYTE    Value[ANYSIZE_ARRAY];
    /* DWORD aligned */
    VS_VERSIONINFO_BLOCK Children[ANYSIZE_ARRAY];
#endif

}VS_VERSIONINFO_BLOCK, *PVS_VERSIONINFO_BLOCK;

#define __VS_VERSIONINFO_Type(block) \
    ((const VS_VERSIONINFO_BLOCK*)(block))->Type

#define __VS_VERSIONINFO_Key(block) \
    ((const VS_VERSIONINFO_BLOCK*)(block))->Key

#define __VS_VERSIONINFO_TotalLength(block) \
    ((const VS_VERSIONINFO_BLOCK*)(block))->TotalLength

#define __VS_VERSIONINFO_ValueLength(block) \
    ((const VS_VERSIONINFO_BLOCK*)(block))->ValueLength

#define __VS_VERSIONINFO_ValueBuffer(block) \
    ((LPBYTE)ROUND_TO_SIZE(__VS_VERSIONINFO_Key(block) + wcslen(__VS_VERSIONINFO_Key(block)) + 1, sizeof UINT32))

#define __VS_VERSIONINFO_ChildrenFirst(block) \
    (const VS_VERSIONINFO_BLOCK*)(__VS_VERSIONINFO_ValueBuffer(block) + \
        ROUND_TO_SIZE(__VS_VERSIONINFO_ValueLength(block) * (__VS_VERSIONINFO_Type(block) ? 2 : 1), sizeof UINT32))

#define __VS_VERSIONINFO_ChildrenNext(block) \
    (const VS_VERSIONINFO_BLOCK*)((LPBYTE)block + ROUND_TO_SIZE(__VS_VERSIONINFO_TotalLength(block), sizeof UINT32))

typedef struct _VS_VERSIONINFO
{
    UINT16  TotalLength;
    UINT16  ValueLength;
    UINT16  Type;               /* always 0 */
    WCHAR   Key[ROUND_TO_SIZE(sizeof("VS_VERSION_INFO"), sizeof(UINT32))];
    VS_FIXEDFILEINFO Value;

}VS_VERSIONINFO, *PVS_VERSIONINFO;

typedef struct _VS_FILEINFO_LANG_CODEPAGE
{
    UINT16 Language;
    UINT16 CodePage;

}VS_FILEINFO_LANG_CODEPAGE, *PVS_FILEINFO_LANG_CODEPAGE;

NTSYSAPI
NTSTATUS
NTAPI
LdrAccessResource(
    _In_ PVOID DllHandle,
    _In_ PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
    _Out_opt_ PVOID* ResourceBuffer,
    _Out_opt_ ULONG* ResourceLength
);

typedef struct _LDR_RESOURCE_INFO
{
    ULONG_PTR Type;
    ULONG_PTR Name;
    ULONG_PTR Language;
} LDR_RESOURCE_INFO, * PLDR_RESOURCE_INFO;

#define LDR_RESOURCE_LEVEL_TYPE     0
#define LDR_RESOURCE_LEVEL_NAME     1
#define LDR_RESOURCE_LEVEL_LANGUAGE 2
#define LDR_RESOURCE_LEVEL_DATA     3

#define LDR_RESOURCE_ID_NAME_MASK   ((~(ULONG_PTR)0) << 16) /* lower 16bits clear */
#define LDR_RESOURCE_ID_NAME_MINVAL (( (ULONG_PTR)1) << 16) /* 17th bit set */

NTSYSAPI
NTSTATUS
NTAPI
LdrFindResource_U(
    _In_ PVOID DllHandle,
    _In_ PLDR_RESOURCE_INFO ResourceInfo,
    _In_ ULONG Level,
    _Out_ PIMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry
);

NTSYSAPI
NTSTATUS
NTAPI
LdrFindResourceEx_U(
    _In_ ULONG Flags,
    _In_ PVOID DllHandle,
    _In_ PLDR_RESOURCE_INFO ResourceInfo,
    _In_ ULONG Level,
    _Out_ PIMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry
);

NTSYSAPI
NTSTATUS
NTAPI
LdrFindResourceDirectory_U(
    _In_ PVOID DllHandle,
    _In_ PLDR_RESOURCE_INFO ResourceInfo,
    _In_ ULONG Level,
    _Out_ PIMAGE_RESOURCE_DIRECTORY* ResourceDirectory
);

// private 
typedef struct _LDR_ENUM_RESOURCE_ENTRY
{
    union
    {
        ULONG_PTR NameOrId;
        PIMAGE_RESOURCE_DIRECTORY_STRING Name;
        struct
        {
            USHORT Id;
            USHORT NameIsPresent;
        };
    } Path[3];
    PVOID Data;
    ULONG Size;
    ULONG Reserved;
} LDR_ENUM_RESOURCE_ENTRY, * PLDR_ENUM_RESOURCE_ENTRY;

#define NAME_FROM_RESOURCE_ENTRY(RootDirectory, Entry) \
    ((Entry)->NameIsString ? (ULONG_PTR)PTR_ADD_OFFSET((RootDirectory), (Entry)->NameOffset) : (Entry)->Id)

NTSYSAPI
NTSTATUS
NTAPI
LdrEnumResources(
    _In_ PVOID DllHandle,
    _In_ PLDR_RESOURCE_INFO ResourceInfo,
    _In_ ULONG Level,
    _Inout_ ULONG* ResourceCount,
    _Out_writes_to_opt_(*ResourceCount, *ResourceCount) PLDR_ENUM_RESOURCE_ENTRY Resources
);

#ifndef _KERNEL_MODE
NTSYSAPI
NTSTATUS
NTAPI
LdrFindEntryForAddress(
    _In_ PVOID DllHandle,
    _Out_ PLDR_DATA_TABLE_ENTRY* Entry
);

NTSYSAPI
NTSTATUS
NTAPI
LdrLoadAlternateResourceModule(
    _In_ PVOID DllHandle,
    _Out_ PVOID* ResourceDllBase,
    _Out_opt_ ULONG_PTR* ResourceOffset,
    _In_ ULONG Flags
);

NTSYSAPI
NTSTATUS
NTAPI
LdrLoadAlternateResourceModuleEx(
    _In_ PVOID DllHandle,
    _In_ LANGID LanguageId,
    _Out_ PVOID* ResourceDllBase,
    _Out_opt_ ULONG_PTR* ResourceOffset,
    _In_ ULONG Flags
);
#endif // if !_KERNEL_MODE

#ifdef _KERNEL_MODE

NTSYSAPI
NTSTATUS
NTAPI
LdrLoadDataFile(
    _In_  PCUNICODE_STRING FileName,
    _Out_ PVOID* ModBase,
    _Out_ SIZE_T* ModSize
);

NTSYSAPI
NTSTATUS
NTAPI
LdrUnloadDataFile(
    _In_ PVOID ModBase
);

NTSYSAPI
NTSTATUS
NTAPI
MmCreateSection(
    _Deref_out_ PVOID* SectionObject,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_ PLARGE_INTEGER InputMaximumSize,
    _In_ ULONG SectionPageProtection,
    _In_ ULONG AllocationAttributes,
    _In_opt_ HANDLE FileHandle,
    _In_opt_ PFILE_OBJECT FileObject
);

inline
NTSTATUS
NTAPI
_VEIL_IMPL_LdrLoadDataFile(
    _In_  PCUNICODE_STRING FileName,
    _Out_ PVOID* ModBase,
    _Out_ SIZE_T* ModSize
)
{
    NTSTATUS Status        = STATUS_SUCCESS;
    HANDLE   FileHandle    = NULL;
    PVOID    SectionObject = NULL;

    do
    {
        *ModBase = NULL;
        *ModSize = 0;

        OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
        InitializeObjectAttributes(
            &ObjectAttributes,
            (PUNICODE_STRING)FileName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL);

        IO_STATUS_BLOCK IoStatusBlock = { 0 };

        Status = ZwOpenFile(
            &FileHandle,
            FILE_GENERIC_READ,
            &ObjectAttributes,
            &IoStatusBlock,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            FILE_NON_DIRECTORY_FILE);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        LARGE_INTEGER MaximumSize = { 0 };

        Status = MmCreateSection(&SectionObject, SECTION_MAP_READ, NULL,
            &MaximumSize, PAGE_READONLY, SEC_IMAGE_NO_EXECUTE, FileHandle, NULL);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        Status = MmMapViewInSystemSpace(SectionObject, ModBase, ModSize);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

    } while (FALSE);

    if (SectionObject)
    {
        ObDereferenceObject(SectionObject);
    }
    if (FileHandle)
    {
        ZwClose(FileHandle);
    }

    return Status;
}

inline
NTSTATUS
NTAPI
_VEIL_IMPL_LdrUnloadDataFile(
    _In_ PVOID ModBase
)
{
    if (ModBase)
    {
        return MmUnmapViewInSystemSpace(ModBase);
    }

    return STATUS_SUCCESS;
}

#if defined _M_IX86

_VEIL_DEFINE_IAT_RAW_SYMBOL(LdrLoadDataFile@12, _VEIL_IMPL_LdrLoadDataFile);
_VEIL_DEFINE_IAT_RAW_SYMBOL(LdrUnloadDataFile@4, _VEIL_IMPL_LdrUnloadDataFile);

#elif defined _M_X64 || defined _M_ARM || defined _M_ARM64

_VEIL_DEFINE_IAT_SYMBOL(LdrLoadDataFile, _VEIL_IMPL_LdrLoadDataFile);
_VEIL_DEFINE_IAT_SYMBOL(LdrUnloadDataFile, _VEIL_IMPL_LdrUnloadDataFile);

#endif

#endif // if _KERNEL_MODE

//
// Module information
//

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

// private
typedef struct _RTL_PROCESS_MODULE_INFORMATION_EX
{
    USHORT NextOffset;
    RTL_PROCESS_MODULE_INFORMATION BaseInfo;
    ULONG ImageChecksum;
    ULONG TimeDateStamp;
    PVOID DefaultBase;
} RTL_PROCESS_MODULE_INFORMATION_EX, * PRTL_PROCESS_MODULE_INFORMATION_EX;

#ifndef _KERNEL_MODE
NTSYSAPI
NTSTATUS
NTAPI
LdrQueryProcessModuleInformation(
    _In_opt_ PRTL_PROCESS_MODULES ModuleInformation,
    _In_opt_ ULONG Size,
    _Out_ PULONG ReturnedSize
);

typedef VOID(NTAPI* PLDR_ENUM_CALLBACK)(
    _In_ PLDR_DATA_TABLE_ENTRY ModuleInformation,
    _In_ PVOID Parameter,
    _Out_ BOOLEAN* Stop
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrEnumerateLoadedModules(
    _In_ BOOLEAN ReservedFlag,
    _In_ PLDR_ENUM_CALLBACK EnumProc,
    _In_ PVOID Context
);

NTSYSAPI
NTSTATUS
NTAPI
LdrOpenImageFileOptionsKey(
    _In_ PUNICODE_STRING SubKey,
    _In_ BOOLEAN Wow64,
    _Out_ PHANDLE NewKeyHandle
);

NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileKeyOption(
    _In_ HANDLE KeyHandle,
    _In_ PCWSTR ValueName,
    _In_ ULONG Type,
    _Out_ PVOID Buffer,
    _In_ ULONG BufferSize,
    _Out_opt_ PULONG ReturnedLength
);

NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileExecutionOptions(
    _In_ PUNICODE_STRING SubKey,
    _In_ PCWSTR ValueName,
    _In_ ULONG ValueSize,
    _Out_ PVOID Buffer,
    _In_ ULONG BufferSize,
    _Out_opt_ PULONG ReturnedLength
);

NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileExecutionOptionsEx(
    _In_ PUNICODE_STRING SubKey,
    _In_ PCWSTR ValueName,
    _In_ ULONG Type,
    _Out_ PVOID Buffer,
    _In_ ULONG BufferSize,
    _Out_opt_ PULONG ReturnedLength,
    _In_ BOOLEAN Wow64
);

// private
typedef struct _DELAYLOAD_PROC_DESCRIPTOR
{
    ULONG ImportDescribedByName;
    union
    {
        PCSTR Name;
        ULONG Ordinal;
    } Description;
} DELAYLOAD_PROC_DESCRIPTOR, * PDELAYLOAD_PROC_DESCRIPTOR;

// private
typedef struct _DELAYLOAD_INFO
{
    ULONG Size;
    PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor;
    PIMAGE_THUNK_DATA ThunkAddress;
    PCSTR TargetDllName;
    DELAYLOAD_PROC_DESCRIPTOR TargetApiDescriptor;
    PVOID TargetModuleBase;
    PVOID Unused;
    ULONG LastError;
} DELAYLOAD_INFO, * PDELAYLOAD_INFO;

// private
typedef PVOID(NTAPI* PDELAYLOAD_FAILURE_DLL_CALLBACK)(
    _In_ ULONG NotificationReason,
    _In_ PDELAYLOAD_INFO DelayloadInfo
    );

// rev
typedef PVOID(NTAPI* PDELAYLOAD_FAILURE_SYSTEM_ROUTINE)(
    _In_ PCSTR DllName,
    _In_ PCSTR ProcName
    );

#if (NTDDI_VERSION >= NTDDI_WIN8)
// rev
NTSYSAPI
PVOID
NTAPI
LdrResolveDelayLoadedAPI(
    _In_ PVOID ParentModuleBase,
    _In_ PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor,
    _In_opt_ PDELAYLOAD_FAILURE_DLL_CALLBACK FailureDllHook,
    _In_opt_ PDELAYLOAD_FAILURE_SYSTEM_ROUTINE FailureSystemHook, // kernel32.DelayLoadFailureHook
    _Out_ PIMAGE_THUNK_DATA ThunkAddress,
    _Reserved_ ULONG Flags
);

// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrResolveDelayLoadsFromDll(
    _In_ PVOID ParentBase,
    _In_ PCSTR TargetDllName,
    _Reserved_ ULONG Flags
);

// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrSetDefaultDllDirectories(
    _In_ ULONG DirectoryFlags
);
#endif

// rev
DECLSPEC_NORETURN
NTSYSAPI
VOID
NTAPI
LdrShutdownProcess(
    VOID
);

// rev
DECLSPEC_NORETURN
NTSYSAPI
VOID
NTAPI
LdrShutdownThread(
    VOID
);

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
// rev
NTSYSAPI
NTSTATUS
NTAPI
LdrSetImplicitPathOptions(
    _In_ ULONG ImplicitPathOptions
);

// rev
NTSYSAPI
BOOLEAN
NTAPI
LdrControlFlowGuardEnforced(
    VOID
);
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)
// rev
NTSYSAPI
BOOLEAN
NTAPI
LdrIsModuleSxsRedirected(
    _In_ PVOID DllHandle
);
#endif
#endif // _KERNEL_MODE

//
// Driver Section
// 

typedef struct _KLDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    PVOID GpValue;
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    union
    {
        union
        {
            struct
            {
                USHORT SignatureLevel : 4;  /* bit position: 0 */
                USHORT SignatureType : 3;   /* bit position: 4 */
                USHORT Frozen : 2;          /* bit position: 7 */
                USHORT Unused : 7;          /* bit position: 9 */
            };
            USHORT EntireField;
        };
    };
    PVOID SectionPointer;
    ULONG CheckSum;
    ULONG CoverageSectionSize;
    PVOID CoverageSection;
    PVOID LoadedImports;
    PVOID Spare;
    ULONG SizeOfImageNotRounded;
    ULONG TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

VEIL_END()

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif
