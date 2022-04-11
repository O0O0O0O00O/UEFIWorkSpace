;------------------------------------------------------------------------------
;
; Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   LongJump.Asm
;
; Abstract:
;
;   Implementation of _LongJump() on IA-32.
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;
; Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Abstract:
;
;   This file provides macro definitions for NASM files.
;
;------------------------------------------------------------------------------

%macro SAVEPREVSSP     0
    DB 0xF3, 0x0F, 0x01, 0xEA
%endmacro

%macro CLRSSBSY_EAX    0
    DB 0x67, 0xF3, 0x0F, 0xAE, 0x30
%endmacro

%macro RSTORSSP_EAX    0
    DB 0x67, 0xF3, 0x0F, 0x01, 0x28
%endmacro

%macro SETSSBSY        0
    DB 0xF3, 0x0F, 0x01, 0xE8
%endmacro

%macro READSSP_EAX     0
    DB 0xF3, 0x0F, 0x1E, 0xC8
%endmacro

%macro INCSSP_EAX      0
    DB 0xF3, 0x0F, 0xAE, 0xE8
%endmacro

; NASM provides built-in macros STRUC and ENDSTRUC for structure definition.
; For example, to define a structure called mytype containing a longword,
; a word, a byte and a string of bytes, you might code
;
; struc   mytype
;
;  mt_long:      resd    1
;  mt_word:      resw    1
;  mt_byte:      resb    1
;  mt_str:       resb    32
;
; endstruc
;
; Below macros are help to map the C types and the RESB family of pseudo-instructions.
; So that the above structure definition can be coded as
;
; struc   mytype
;
;  mt_long:      CTYPE_UINT32    1
;  mt_word:      CTYPE_UINT16    1
;  mt_byte:      CTYPE_UINT8     1
;  mt_str:       CTYPE_CHAR8     32
;
; endstruc
%define CTYPE_UINT64    resq
%define CTYPE_INT64     resq
%define CTYPE_UINT32    resd
%define CTYPE_INT32     resd
%define CTYPE_UINT16    resw
%define CTYPE_INT16     resw
%define CTYPE_BOOLEAN   resb
%define CTYPE_UINT8     resb
%define CTYPE_CHAR8     resb
%define CTYPE_INT8      resb

%define CTYPE_UINTN     resd
%define CTYPE_INTN      resd


    SECTION .text

extern ASM_PFX(PcdGet32 (PcdControlFlowEnforcementPropertyMask))

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; InternalLongJump (
;   IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer,
;   IN      UINTN                     Value
;   );
;------------------------------------------------------------------------------
global ASM_PFX(InternalLongJump)
ASM_PFX(InternalLongJump):

    mov     eax, [ASM_PFX(PcdGet32 (PcdControlFlowEnforcementPropertyMask))]
    test    eax, eax
    jz      CetDone
    mov     eax, cr4
    bt      eax, 23                ; check if CET is enabled
    jnc     CetDone

    mov     edx, [esp + 4]         ; edx = JumpBuffer
    mov     edx, [edx + 24]        ; edx = target SSP
    READSSP_EAX
    sub     edx, eax               ; edx = delta
    mov     eax, edx               ; eax = delta

    shr     eax, 2                 ; eax = delta/sizeof(UINT32)
    INCSSP_EAX

CetDone:

    pop     eax                         ; skip return address
    pop     edx                         ; edx <- JumpBuffer
    pop     eax                         ; eax <- Value
    mov     ebx, [edx]
    mov     esi, [edx + 4]
    mov     edi, [edx + 8]
    mov     ebp, [edx + 12]
    mov     esp, [edx + 16]
    jmp     dword [edx + 20]        ; restore "eip"

