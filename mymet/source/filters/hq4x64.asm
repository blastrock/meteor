;hq4x filter
;32bpp output
;----------------------------------------------------------
;Copyright (C) 2003 MaxSt ( maxst@hiend3d.com )
;
;This program is free software; you can rrsistribute it and/or
;modify it under the terms of the GNU General Public License
;as published by the Free Software Foundation; either
;version 2 of the License, or (at your option) any later
;version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program; if not, write to the Free Software
;Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

GLOBAL hq4x

EXTERN LUT16to32
EXTERN RGBtoYUV

SECTION .bss
pixcnt    resd 1
linesleft resd 1
xcounter  resd 1
cross     resd 1
nextline  resd 1
prevline  resd 1
w1        resd 1
w2        resd 1
w3        resd 1
w4        resd 1
w5        resd 1
w6        resd 1
w7        resd 1
w8        resd 1
w9        resd 1
c1        resd 1
c2        resd 1
c3        resd 1
c4        resd 1
c5        resd 1
c6        resd 1
c7        resd 1
c8        resd 1
c9        resd 1

SECTION .data

reg_blank    dd  0,0
const3       dd  0x00030003,0x00000003
const5       dd  0x00050005,0x00000005
const6       dd  0x00060006,0x00000006
const7       dd  0x00070007,0x00000007
threshold    dd  0x00300706,0x00000000

SECTION .text

%macro AUXADDRESS 0
    mov     rcx, rsi
    add     rcx, rbx
    add     rcx, rbx
%endmacro

%macro TestFlags 3
    mov     edx,dword[%1]
    ;cmp     eax,edx
    ;je      %2
    cmp     eax,edx
    je      %2
    movq    mm1,mm5
    movd    mm2,[rbx+rdx*4]
		; on each byte, mm1 = abs(w5 - w2)
    psubusb mm1,mm2
    psubusb mm2,mm5
    por     mm1,mm2
		; compare each byte to the threshold
    psubusb mm1,[threshold]
    movd    edx,mm1
    test    edx,edx
    jz      %2
    or      ecx,%3
%endmacro

%macro TestFlags 4
    mov     edx,dword[%1]
    cmp     eax,edx
    je      %2
    or      dword[cross],%3
    movq    mm1,mm5
    movd    mm2,[rbx+rdx*4]
		; on each byte, mm1 = abs(w5 - w2)
    psubusb mm1,mm2
    psubusb mm2,mm5
    por     mm1,mm2
		; compare each byte to the threshold
    psubusb mm1,[threshold]
    movd    edx,mm1
    test    edx,edx
    jz      %2
    or      ecx,%4
%endmacro

%macro TestDiff 2
    mov     edx,[%1]
    sub     edx,[%2]
    jz      %%end
    mov     edx,dword[%1]
    shl     edx,2
    add     rdx,RGBtoYUV
    movd    mm1,[rdx]
    movq    mm5,mm1
    mov     edx,dword[%2]
    shl     edx,2
    add     rdx,RGBtoYUV
    movd    mm2,[rdx]
    psubusb mm1,mm2
    psubusb mm2,mm5
    por     mm1,mm2
    psubusb mm1,[threshold]
    movd    edx,mm1
%%end:
%endmacro

%macro DiffOrNot 4
   TestDiff %1,%2
   test edx,edx
   jz   %%same
   %3
   jmp %%end
%%same:
   %4
%%end:
%endmacro

%macro DiffOrNot 8
   TestDiff %1,%2
   test edx,edx
   jz   %%same
   %3
   %4
   %5
   jmp %%end
%%same:
   %6
   %7
   %8
%%end:
%endmacro

%macro DiffOrNot 10
   TestDiff %1,%2
   test edx,edx
   jz %%same
   %3
   %4
   %5
   %6
   jmp %%end
%%same:
   %7
   %8
   %9
   %10
%%end:
%endmacro

%macro DiffOrNot 14
   TestDiff %1,%2
   test edx,edx
   jz %%same
   %3
   %4
   %5
   %6
   %7
   %8
   jmp %%end
%%same:
   %9
   %10
   %11
   %12
   %13
   %14
%%end:
%endmacro

%macro Interp1 3
    mov edx,%2
    shl edx,2
    add edx,%3
    sub edx,%2
    shr edx,2
    mov %1,edx
%endmacro

%macro Interp2 4
    mov edx,%2
    shl edx,1
    add edx,%3
    add edx,%4
    shr edx,2
    mov %1,edx
%endmacro

%macro Interp3 2
    movd       mm1, eax
    movd       mm2, %2
    punpcklbw  mm1, [reg_blank]
    punpcklbw  mm2, [reg_blank]
    pmullw     mm1, [const7]
    paddw      mm1, mm2
    psrlw      mm1, 3
    packuswb   mm1, [reg_blank]
    movd       %1, mm1
%endmacro

%macro Interp5 3
    mov edx,%2
    add edx,%3
    shr edx,1
    mov %1,edx
%endmacro

%macro Interp6 3
    movd       mm1, eax
    movd       mm2, %2
    movd       mm3, %3
    punpcklbw  mm1, [reg_blank]
    punpcklbw  mm2, [reg_blank]
    punpcklbw  mm3, [reg_blank]
    pmullw     mm1, [const5]
    psllw      mm2, 1
    paddw      mm1, mm3
    paddw      mm1, mm2
    psrlw      mm1, 3
    packuswb   mm1, [reg_blank]
    movd       %1, mm1
%endmacro

%macro Interp7 3
    movd       mm1, eax
    movd       mm2, %2
    movd       mm3, %3
    punpcklbw  mm1, [reg_blank]
    punpcklbw  mm2, [reg_blank]
    punpcklbw  mm3, [reg_blank]
    pmullw     mm1, [const6]
    paddw      mm2, mm3
    paddw      mm1, mm2
    psrlw      mm1, 3
    packuswb   mm1, [reg_blank]
    movd       %1, mm1
%endmacro

%macro Interp8 3
    movd       mm1, %2
    movd       mm2, %3
    punpcklbw  mm1, [reg_blank]
    punpcklbw  mm2, [reg_blank]
    pmullw     mm1, [const5]
    pmullw     mm2, [const3]
    paddw      mm1, mm2
    psrlw      mm1, 3
    packuswb   mm1, [reg_blank]
    movd       %1, mm1
%endmacro

%macro PIXEL00_0 0
    mov [rsi],eax
%endmacro

%macro PIXEL00_11 0
    Interp1 [rsi],eax,[c4]
%endmacro

%macro PIXEL00_12 0
    Interp1 [rsi],eax,[c2]
%endmacro

%macro PIXEL00_20 0
    Interp2 [rsi],eax,[c2],[c4]
%endmacro

%macro PIXEL00_50 0
    Interp5 [rsi],[c2],[c4]
%endmacro

%macro PIXEL00_80 0
    Interp8 [rsi],eax,[c1]
%endmacro

%macro PIXEL00_81 0
    Interp8 [rsi],eax,[c4]
%endmacro

%macro PIXEL00_82 0
    Interp8 [rsi],eax,[c2]
%endmacro

%macro PIXEL01_0 0
    mov [rsi+4],eax
%endmacro

%macro PIXEL01_10 0
    Interp1 [rsi+4],eax,[c1]
%endmacro

%macro PIXEL01_12 0
    Interp1 [rsi+4],eax,[c2]
%endmacro

%macro PIXEL01_14 0
    Interp1 [rsi+4],[c2],eax
%endmacro

%macro PIXEL01_21 0
    Interp2 [rsi+4],[c2],eax,[c4]
%endmacro

%macro PIXEL01_31 0
    Interp3 [rsi+4],[c4]
%endmacro

%macro PIXEL01_50 0
    Interp5 [rsi+4],[c2],eax
%endmacro

%macro PIXEL01_60 0
    Interp6 [rsi+4],[c2],[c4]
%endmacro

%macro PIXEL01_61 0
    Interp6 [rsi+4],[c2],[c1]
%endmacro

%macro PIXEL01_82 0
    Interp8 [rsi+4],eax,[c2]
%endmacro

%macro PIXEL01_83 0
    Interp8 [rsi+4],[c2],[c4]
%endmacro

%macro PIXEL02_0 0
    mov [rsi+8],eax
%endmacro

%macro PIXEL02_10 0
    Interp1 [rsi+8],eax,[c3]
%endmacro

%macro PIXEL02_11 0
    Interp1 [rsi+8],eax,[c2]
%endmacro

%macro PIXEL02_13 0
    Interp1 [rsi+8],[c2],eax
%endmacro

%macro PIXEL02_21 0
    Interp2 [rsi+8],[c2],eax,[c6]
%endmacro

%macro PIXEL02_32 0
    Interp3 [rsi+8],[c6]
%endmacro

%macro PIXEL02_50 0
    Interp5 [rsi+8],[c2],eax
%endmacro

%macro PIXEL02_60 0
    Interp6 [rsi+8],[c2],[c6]
%endmacro

%macro PIXEL02_61 0
    Interp6 [rsi+8],[c2],[c3]
%endmacro

%macro PIXEL02_81 0
    Interp8 [rsi+8],eax,[c2]
%endmacro

%macro PIXEL02_83 0
    Interp8 [rsi+8],[c2],[c6]
%endmacro

%macro PIXEL03_0 0
    mov [rsi+12],eax
%endmacro

%macro PIXEL03_11 0
    Interp1 [rsi+12],eax,[c2]
%endmacro

%macro PIXEL03_12 0
    Interp1 [rsi+12],eax,[c6]
%endmacro

%macro PIXEL03_20 0
    Interp2 [rsi+12],eax,[c2],[c6]
%endmacro

%macro PIXEL03_50 0
    Interp5 [rsi+12],[c2],[c6]
%endmacro

%macro PIXEL03_80 0
    Interp8 [rsi+12],eax,[c3]
%endmacro

%macro PIXEL03_81 0
    Interp8 [rsi+12],eax,[c2]
%endmacro

%macro PIXEL03_82 0
    Interp8 [rsi+12],eax,[c6]
%endmacro

%macro PIXEL10_0 0
    mov [rsi+rbx],eax
%endmacro

%macro PIXEL10_10 0
    Interp1 [rsi+rbx],eax,[c1]
%endmacro

%macro PIXEL10_11 0
    Interp1 [rsi+rbx],eax,[c4]
%endmacro

%macro PIXEL10_13 0
    Interp1 [rsi+rbx],[c4],eax
%endmacro

%macro PIXEL10_21 0
    Interp2 [rsi+rbx],[c4],eax,[c2]
%endmacro

%macro PIXEL10_32 0
    Interp3 [rsi+rbx],[c2]
%endmacro

%macro PIXEL10_50 0
    Interp5 [rsi+rbx],[c4],eax
%endmacro

%macro PIXEL10_60 0
    Interp6 [rsi+rbx],[c4],[c2]
%endmacro

%macro PIXEL10_61 0
    Interp6 [rsi+rbx],[c4],[c1]
%endmacro

%macro PIXEL10_81 0
    Interp8 [rsi+rbx],eax,[c4]
%endmacro

%macro PIXEL10_83 0
    Interp8 [rsi+rbx],[c4],[c2]
%endmacro

%macro PIXEL11_0 0
    mov [rsi+rbx+4],eax
%endmacro

%macro PIXEL11_30 0
    Interp3 [rsi+rbx+4],[c1]
%endmacro

%macro PIXEL11_31 0
    Interp3 [rsi+rbx+4],[c4]
%endmacro

%macro PIXEL11_32 0
    Interp3 [rsi+rbx+4],[c2]
%endmacro

%macro PIXEL11_70 0
    Interp7 [rsi+rbx+4],[c4],[c2]
%endmacro

%macro PIXEL12_0 0
    mov [rsi+rbx+8],eax
%endmacro

%macro PIXEL12_30 0
    Interp3 [rsi+rbx+8],[c3]
%endmacro

%macro PIXEL12_31 0
    Interp3 [rsi+rbx+8],[c2]
%endmacro

%macro PIXEL12_32 0
    Interp3 [rsi+rbx+8],[c6]
%endmacro

%macro PIXEL12_70 0
    Interp7 [rsi+rbx+8],[c6],[c2]
%endmacro

%macro PIXEL13_0 0
    mov [rsi+rbx+12],eax
%endmacro

%macro PIXEL13_10 0
    Interp1 [rsi+rbx+12],eax,[c3]
%endmacro

%macro PIXEL13_12 0
    Interp1 [rsi+rbx+12],eax,[c6]
%endmacro

%macro PIXEL13_14 0
    Interp1 [rsi+rbx+12],[c6],eax
%endmacro

%macro PIXEL13_21 0
    Interp2 [rsi+rbx+12],[c6],eax,[c2]
%endmacro

%macro PIXEL13_31 0
    Interp3 [rsi+rbx+12],[c2]
%endmacro

%macro PIXEL13_50 0
    Interp5 [rsi+rbx+12],[c6],eax
%endmacro

%macro PIXEL13_60 0
    Interp6 [rsi+rbx+12],[c6],[c2]
%endmacro

%macro PIXEL13_61 0
    Interp6 [rsi+rbx+12],[c6],[c3]
%endmacro

%macro PIXEL13_82 0
    Interp8 [rsi+rbx+12],eax,[c6]
%endmacro

%macro PIXEL13_83 0
    Interp8 [rsi+rbx+12],[c6],[c2]
%endmacro

%macro PIXEL20_0 0
    mov [rcx],eax
%endmacro

%macro PIXEL20_10 0
    Interp1 [rcx],eax,[c7]
%endmacro

%macro PIXEL20_12 0
    Interp1 [rcx],eax,[c4]
%endmacro

%macro PIXEL20_14 0
    Interp1 [rcx],[c4],eax
%endmacro

%macro PIXEL20_21 0
    Interp2 [rcx],[c4],eax,[c8]
%endmacro

%macro PIXEL20_31 0
    Interp3 [rcx],[c8]
%endmacro

%macro PIXEL20_50 0
    Interp5 [rcx],[c4],eax
%endmacro

%macro PIXEL20_60 0
    Interp6 [rcx],[c4],[c8]
%endmacro

%macro PIXEL20_61 0
    Interp6 [rcx],[c4],[c7]
%endmacro

%macro PIXEL20_82 0
    Interp8 [rcx],eax,[c4]
%endmacro

%macro PIXEL20_83 0
    Interp8 [rcx],[c4],[c8]
%endmacro

%macro PIXEL21_0 0
    mov [rcx+4],eax
%endmacro

%macro PIXEL21_30 0
    Interp3 [rcx+4],[c7]
%endmacro

%macro PIXEL21_31 0
    Interp3 [rcx+4],[c8]
%endmacro

%macro PIXEL21_32 0
    Interp3 [rcx+4],[c4]
%endmacro

%macro PIXEL21_70 0
    Interp7 [rcx+4],[c4],[c8]
%endmacro

%macro PIXEL22_0 0
    mov [rcx+8],eax
%endmacro

%macro PIXEL22_30 0
    Interp3 [rcx+8],[c9]
%endmacro

%macro PIXEL22_31 0
    Interp3 [rcx+8],[c6]
%endmacro

%macro PIXEL22_32 0
    Interp3 [rcx+8],[c8]
%endmacro

%macro PIXEL22_70 0
    Interp7 [rcx+8],[c6],[c8]
%endmacro

%macro PIXEL23_0 0
    mov [rcx+12],eax
%endmacro

%macro PIXEL23_10 0
    Interp1 [rcx+12],eax,[c9]
%endmacro

%macro PIXEL23_11 0
    Interp1 [rcx+12],eax,[c6]
%endmacro

%macro PIXEL23_13 0
    Interp1 [rcx+12],[c6],eax
%endmacro

%macro PIXEL23_21 0
    Interp2 [rcx+12],[c6],eax,[c8]
%endmacro

%macro PIXEL23_32 0
    Interp3 [rcx+12],[c8]
%endmacro

%macro PIXEL23_50 0
    Interp5 [rcx+12],[c6],eax
%endmacro

%macro PIXEL23_60 0
    Interp6 [rcx+12],[c6],[c8]
%endmacro

%macro PIXEL23_61 0
    Interp6 [rcx+12],[c6],[c9]
%endmacro

%macro PIXEL23_81 0
    Interp8 [rcx+12],eax,[c6]
%endmacro

%macro PIXEL23_83 0
    Interp8 [rcx+12],[c6],[c8]
%endmacro

%macro PIXEL30_0 0
    mov [rcx+rbx],eax
%endmacro

%macro PIXEL30_11 0
    Interp1 [rcx+rbx],eax,[c8]
%endmacro

%macro PIXEL30_12 0
    Interp1 [rcx+rbx],eax,[c4]
%endmacro

%macro PIXEL30_20 0
    Interp2 [rcx+rbx],eax,[c8],[c4]
%endmacro

%macro PIXEL30_50 0
    Interp5 [rcx+rbx],[c8],[c4]
%endmacro

%macro PIXEL30_80 0
    Interp8 [rcx+rbx],eax,[c7]
%endmacro

%macro PIXEL30_81 0
    Interp8 [rcx+rbx],eax,[c8]
%endmacro

%macro PIXEL30_82 0
    Interp8 [rcx+rbx],eax,[c4]
%endmacro

%macro PIXEL31_0 0
    mov [rcx+rbx+4],eax
%endmacro

%macro PIXEL31_10 0
    Interp1 [rcx+rbx+4],eax,[c7]
%endmacro

%macro PIXEL31_11 0
    Interp1 [rcx+rbx+4],eax,[c8]
%endmacro

%macro PIXEL31_13 0
    Interp1 [rcx+rbx+4],[c8],eax
%endmacro

%macro PIXEL31_21 0
    Interp2 [rcx+rbx+4],[c8],eax,[c4]
%endmacro

%macro PIXEL31_32 0
    Interp3 [rcx+rbx+4],[c4]
%endmacro

%macro PIXEL31_50 0
    Interp5 [rcx+rbx+4],[c8],eax
%endmacro

%macro PIXEL31_60 0
    Interp6 [rcx+rbx+4],[c8],[c4]
%endmacro

%macro PIXEL31_61 0
    Interp6 [rcx+rbx+4],[c8],[c7]
%endmacro

%macro PIXEL31_81 0
    Interp8 [rcx+rbx+4],eax,[c8]
%endmacro

%macro PIXEL31_83 0
    Interp8 [rcx+rbx+4],[c8],[c4]
%endmacro

%macro PIXEL32_0 0
    mov [rcx+rbx+8],eax
%endmacro

%macro PIXEL32_10 0
    Interp1 [rcx+rbx+8],eax,[c9]
%endmacro

%macro PIXEL32_12 0
    Interp1 [rcx+rbx+8],eax,[c8]
%endmacro

%macro PIXEL32_14 0
    Interp1 [rcx+rbx+8],[c8],eax
%endmacro

%macro PIXEL32_21 0
    Interp2 [rcx+rbx+8],[c8],eax,[c6]
%endmacro

%macro PIXEL32_31 0
    Interp3 [rcx+rbx+8],[c6]
%endmacro

%macro PIXEL32_50 0
    Interp5 [rcx+rbx+8],[c8],eax
%endmacro

%macro PIXEL32_60 0
    Interp6 [rcx+rbx+8],[c8],[c6]
%endmacro

%macro PIXEL32_61 0
    Interp6 [rcx+rbx+8],[c8],[c9]
%endmacro

%macro PIXEL32_82 0
    Interp8 [rcx+rbx+8],eax,[c8]
%endmacro

%macro PIXEL32_83 0
    Interp8 [rcx+rbx+8],[c8],[c6]
%endmacro

%macro PIXEL33_0 0
    mov [rcx+rbx+12],eax
%endmacro

%macro PIXEL33_11 0
    Interp1 [rcx+rbx+12],eax,[c6]
%endmacro

%macro PIXEL33_12 0
    Interp1 [rcx+rbx+12],eax,[c8]
%endmacro

%macro PIXEL33_20 0
    Interp2 [rcx+rbx+12],eax,[c8],[c6]
%endmacro

%macro PIXEL33_50 0
    Interp5 [rcx+rbx+12],[c8],[c6]
%endmacro

%macro PIXEL33_80 0
    Interp8 [rcx+rbx+12],eax,[c9]
%endmacro

%macro PIXEL33_81 0
    Interp8 [rcx+rbx+12],eax,[c6]
%endmacro

%macro PIXEL33_82 0
    Interp8 [rcx+rbx+12],eax,[c8]
%endmacro

; the first 8 bytes are for rbx
inbuffer     equ 16
outbuffer    equ 24
Xres         equ 28
Yres         equ 32
pitch        equ 36

hq4x:
    push rbp
    mov rbp,rsp
    push rbx

    mov     qword[rbp-inbuffer], rdi
    mov     qword[rbp-outbuffer], rsi
    mov     dword[rbp-Xres], edx
    mov     dword[rbp-Yres], ecx
    mov     dword[rbp-pitch], r8d

    shl     edx,1
    mov     dword[linesleft],ecx
    mov     dword[prevline],0
    mov     dword[nextline],edx
		mov     dword[pixcnt],0
.loopy:
    mov     ecx,dword[rbp-Xres]
    sub     ecx,2                 ; x={Xres-2, Xres-1} are special cases.
    mov     dword[xcounter],ecx
    ; x=0 - special case
    movsxd  rbx,dword[prevline]
    mov     eax,dword[rdi+rbx]
    mov     ecx,dword[rdi]
    movsxd  rbx,dword[nextline]
    mov     edx,dword[rdi+rbx]
		movzx   r8d, ax
    mov     dword[w1],r8d
    mov     dword[w2],r8d
    shr     eax,16
    mov     dword[w3],eax
		movzx   r8d, cx
    mov     dword[w4],r8d
    mov     dword[w5],r8d
    shr     ecx,16
    mov     dword[w6],ecx
		movzx   r8d, dx
    mov     dword[w7],r8d
    mov     dword[w8],r8d
    shr     edx,16
    mov     dword[w9],edx
    jmp     .flags
.loopx:
    movsxd  rbx,dword[prevline]
    movq    mm5,[rdi+rbx-2]
    movq    mm6,[rdi-2]
    movsxd  rbx,dword[nextline]
    movq    mm7,[rdi+rbx-2]
    movd    eax,mm5
    movzx   edx,ax
    mov     [w1],edx
    shr     eax,16
    mov     [w2],eax
    psrlq   mm5,32
    movd    eax,mm5
    movzx   edx,ax
    mov     [w3],edx
    movd    eax,mm6
    movzx   edx,ax
    mov     [w4],edx
    shr     eax,16
    mov     [w5],eax
    psrlq   mm6,32
    movd    eax,mm6
    movzx   edx,ax
    mov     [w6],edx
    movd    eax,mm7
    movzx   edx,ax
    mov     [w7],edx
    shr     eax,16
    mov     [w8],eax
    psrlq   mm7,32
    movd    eax,mm7
    movzx   edx,ax
    mov     [w9],edx
.flags:
    mov     rbx,RGBtoYUV
    mov     eax,dword[w5]
    xor     rcx,rcx
    movd    mm5,[rbx+rax*4]
    mov     dword[cross],0

		TestFlags w2, .noflag2, 1, 2
.noflag2:
		TestFlags w4, .noflag4, 2, 8
.noflag4:
		TestFlags w6, .noflag6, 4, 16
.noflag6:
		TestFlags w8, .noflag8, 8, 64
.noflag8:
    cmp     dword[cross],0
    jnz     .testflag1
    mov     rbx,LUT16to32
    mov     eax,[rbx+rax*4]
    mov     ebx,dword[rbp-pitch]
    AUXADDRESS
    mov     [rsi],eax
    mov     [rsi+4],eax
    mov     [rsi+8],eax
    mov     [rsi+12],eax
    mov     [rsi+rbx],eax
    mov     [rsi+rbx+4],eax
    mov     [rsi+rbx+8],eax
    mov     [rsi+rbx+12],eax
    mov     [rcx],eax
    mov     [rcx+4],eax
    mov     [rcx+8],eax
    mov     [rcx+12],eax
    mov     [rcx+rbx],eax
    mov     [rcx+rbx+4],eax
    mov     [rcx+rbx+8],eax
    mov     [rcx+rbx+12],eax
    jmp     .loopx_end
.testflag1:
		TestFlags w1, .noflag1, 1
.noflag1:
		TestFlags w3, .noflag3, 4
.noflag3:
		TestFlags w7, .noflag7, 32
.noflag7:
		TestFlags w9, .noflag9, 128
.noflag9:
    mov  rbx,LUT16to32
    mov  eax,[rbx+rax*4]
    mov  edx,dword[w2]
    mov  edx,[rbx+rdx*4]
    mov  [c2],edx
    mov  edx,dword[w4]
    mov  edx,[rbx+rdx*4]
    mov  [c4],edx
    mov  edx,dword[w6]
    mov  edx,[rbx+rdx*4]
    mov  [c6],edx
    mov  edx,dword[w8]
    mov  edx,[rbx+rdx*4]
    mov  [c8],edx
		; if there is no difference between 5 and 2, 4, 6 and 8
    test ecx,0x005A
    jz   .switch
    mov  edx,dword[w1]
    mov  edx,[rbx+rdx*4]
    mov  [c1],edx
    mov  edx,dword[w3]
    mov  edx,[rbx+rdx*4]
    mov  [c3],edx
    mov  edx,dword[w7]
    mov  edx,[rbx+rdx*4]
    mov  [c7],edx
    mov  edx,dword[w9]
    mov  edx,[rbx+rdx*4]
    mov  [c9],edx
.switch:
    mov  ebx,dword[rbp-pitch]
    jmp  [FuncTable+rcx*8]

..@flag0:
..@flag1:
..@flag4:
..@flag32:
..@flag128:
..@flag5:
..@flag132:
..@flag160:
..@flag33:
..@flag129:
..@flag36:
..@flag133:
..@flag164:
..@flag161:
..@flag37:
..@flag165:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag2:
..@flag34:
..@flag130:
..@flag162:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag16:
..@flag17:
..@flag48:
..@flag49:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag64:
..@flag65:
..@flag68:
..@flag69:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag8:
..@flag12:
..@flag136:
..@flag140:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag3:
..@flag35:
..@flag131:
..@flag163:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_61
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag6:
..@flag38:
..@flag134:
..@flag166:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_61
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag20:
..@flag21:
..@flag52:
..@flag53:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    PIXEL03_81
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL13_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag144:
..@flag145:
..@flag176:
..@flag177:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL23_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag192:
..@flag193:
..@flag196:
..@flag197:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_61
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag96:
..@flag97:
..@flag100:
..@flag101:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_61
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag40:
..@flag44:
..@flag168:
..@flag172:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    PIXEL20_31
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL30_81
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag9:
..@flag13:
..@flag137:
..@flag141:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL10_32
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag18:
..@flag50:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_50,PIXEL03_50,PIXEL12_0,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag80:
..@flag81:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_61
    PIXEL21_30
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag72:
..@flag76:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_50,PIXEL21_0,PIXEL30_50,PIXEL31_50
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag10:
..@flag138:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_50,PIXEL01_50,PIXEL10_50,PIXEL11_0
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_61
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag66:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag24:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag7:
..@flag39:
..@flag135:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag148:
..@flag149:
..@flag180:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    PIXEL03_81
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL13_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL23_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag224:
..@flag228:
..@flag225:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag41:
..@flag169:
..@flag45:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL10_32
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL20_31
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL30_81
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag22:
..@flag54:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL12_0
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag208:
..@flag209:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_61
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag104:
..@flag108:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag11:
..@flag139:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_61
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag19:
..@flag51:
    AUXADDRESS
    DiffOrNot w2,w6,PIXEL00_81,PIXEL01_31,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL00_12,PIXEL01_14,PIXEL02_83,PIXEL03_50,PIXEL12_70,PIXEL13_21
    PIXEL10_81
    PIXEL11_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag146:
..@flag178:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL23_32,PIXEL33_82,PIXEL02_21,PIXEL03_50,PIXEL12_70,PIXEL13_83,PIXEL23_13,PIXEL33_11
    PIXEL10_61
    PIXEL11_30
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    jmp .loopx_end
..@flag84:
..@flag85:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    DiffOrNot w6,w8,PIXEL03_81,PIXEL13_31,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL03_12,PIXEL13_14,PIXEL22_70,PIXEL23_83,PIXEL32_21,PIXEL33_50
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL20_61
    PIXEL21_30
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag112:
..@flag113:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL30_82,PIXEL31_32,PIXEL32_10,PIXEL33_80,PIXEL22_70,PIXEL23_21,PIXEL30_11,PIXEL31_13,PIXEL32_83,PIXEL33_50
    jmp .loopx_end
..@flag200:
..@flag204:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL32_31,PIXEL33_81,PIXEL20_21,PIXEL21_70,PIXEL30_50,PIXEL31_83,PIXEL32_14,PIXEL33_12
    PIXEL22_31
    PIXEL23_81
    jmp .loopx_end
..@flag73:
..@flag77:
    AUXADDRESS
    DiffOrNot w8,w4,PIXEL00_82,PIXEL10_32,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL00_11,PIXEL10_13,PIXEL20_83,PIXEL21_70,PIXEL30_50,PIXEL31_21
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag42:
..@flag170:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL20_31,PIXEL30_81,PIXEL00_50,PIXEL01_21,PIXEL10_83,PIXEL11_70,PIXEL20_14,PIXEL30_12
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_61
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag14:
..@flag142:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL02_32,PIXEL03_82,PIXEL10_10,PIXEL11_30,PIXEL00_50,PIXEL01_83,PIXEL02_13,PIXEL03_11,PIXEL10_21,PIXEL11_70
    PIXEL12_32
    PIXEL13_82
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag67:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_61
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag70:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_61
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag28:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag152:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag194:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    PIXEL20_61
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag98:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_61
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag56:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag25:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag26:
..@flag31:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL11_0
    PIXEL12_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag82:
..@flag214:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL12_0
    PIXEL20_61
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag88:
..@flag248:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    jmp .loopx_end
..@flag74:
..@flag107:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_61
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag27:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag86:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL12_0
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag216:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag106:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag30:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_10
    PIXEL11_30
    PIXEL12_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag210:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_61
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag120:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag75:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_61
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag29:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_61
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag198:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_61
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    PIXEL20_61
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag184:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_61
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag99:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_61
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_61
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag57:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag71:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_61
    PIXEL21_30
    PIXEL22_30
    PIXEL23_61
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag156:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag226:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_61
    PIXEL11_30
    PIXEL12_30
    PIXEL13_61
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag60:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag195:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_61
    PIXEL20_61
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag102:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_61
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_61
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag153:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag58:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag83:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL10_81
    PIXEL11_31
    PIXEL20_61
    PIXEL21_30
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag92:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag202:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_61
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_31
    PIXEL23_81
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag78:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    PIXEL02_32
    PIXEL03_82
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag154:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag114:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL10_61
    PIXEL11_30
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    PIXEL30_82
    PIXEL31_32
    jmp .loopx_end
..@flag89:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag90:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag55:
..@flag23:
    AUXADDRESS
    DiffOrNot w2,w6,PIXEL00_81,PIXEL01_31,PIXEL02_0,PIXEL03_0,PIXEL12_0,PIXEL13_0,PIXEL00_12,PIXEL01_14,PIXEL02_83,PIXEL03_50,PIXEL12_70,PIXEL13_21
    PIXEL10_81
    PIXEL11_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_30
    PIXEL23_10
    PIXEL30_20
    PIXEL31_60
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag182:
..@flag150:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL12_0,PIXEL13_0,PIXEL23_32,PIXEL33_82,PIXEL02_21,PIXEL03_50,PIXEL12_70,PIXEL13_83,PIXEL23_13,PIXEL33_11
    PIXEL10_61
    PIXEL11_30
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    jmp .loopx_end
..@flag213:
..@flag212:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    DiffOrNot w6,w8,PIXEL03_81,PIXEL13_31,PIXEL22_0,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL03_12,PIXEL13_14,PIXEL22_70,PIXEL23_83,PIXEL32_21,PIXEL33_50
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL20_61
    PIXEL21_30
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag241:
..@flag240:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_61
    PIXEL03_80
    PIXEL10_60
    PIXEL11_70
    PIXEL12_30
    PIXEL13_10
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_0,PIXEL23_0,PIXEL30_82,PIXEL31_32,PIXEL32_0,PIXEL33_0,PIXEL22_70,PIXEL23_21,PIXEL30_11,PIXEL31_13,PIXEL32_83,PIXEL33_50
    jmp .loopx_end
..@flag236:
..@flag232:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_60
    PIXEL03_20
    PIXEL10_10
    PIXEL11_30
    PIXEL12_70
    PIXEL13_60
    DiffOrNot w8,w4,PIXEL20_0,PIXEL21_0,PIXEL30_0,PIXEL31_0,PIXEL32_31,PIXEL33_81,PIXEL20_21,PIXEL21_70,PIXEL30_50,PIXEL31_83,PIXEL32_14,PIXEL33_12
    PIXEL22_31
    PIXEL23_81
    jmp .loopx_end
..@flag109:
..@flag105:
    AUXADDRESS
    DiffOrNot w8,w4,PIXEL00_82,PIXEL10_32,PIXEL20_0,PIXEL21_0,PIXEL30_0,PIXEL31_0,PIXEL00_11,PIXEL10_13,PIXEL20_83,PIXEL21_70,PIXEL30_50,PIXEL31_21
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag171:
..@flag43:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL11_0,PIXEL20_31,PIXEL30_81,PIXEL00_50,PIXEL01_21,PIXEL10_83,PIXEL11_70,PIXEL20_14,PIXEL30_12
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_61
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag143:
..@flag15:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL02_32,PIXEL03_82,PIXEL10_0,PIXEL11_0,PIXEL00_50,PIXEL01_83,PIXEL02_13,PIXEL03_11,PIXEL10_21,PIXEL11_70
    PIXEL12_32
    PIXEL13_82
    PIXEL20_10
    PIXEL21_30
    PIXEL22_70
    PIXEL23_60
    PIXEL30_80
    PIXEL31_61
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag124:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag203:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_61
    PIXEL20_10
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag62:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_10
    PIXEL11_30
    PIXEL12_0
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag211:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_10
    PIXEL20_61
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag118:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL12_0
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_10
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag217:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag110:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_10
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag155:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag188:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag185:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag61:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag157:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag103:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_61
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag227:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_61
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag230:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_61
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag199:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_61
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag220:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    jmp .loopx_end
..@flag158:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL12_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag234:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_61
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_31
    PIXEL23_81
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag242:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL10_61
    PIXEL11_30
    PIXEL20_82
    PIXEL21_32
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_82
    PIXEL31_32
    jmp .loopx_end
..@flag59:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL11_0
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag121:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag87:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_81
    PIXEL11_31
    PIXEL12_0
    PIXEL20_61
    PIXEL21_30
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag79:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_32
    PIXEL03_82
    PIXEL11_0
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag122:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag94:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL12_0
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag218:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    jmp .loopx_end
..@flag91:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL11_0
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag229:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_60
    PIXEL03_20
    PIXEL10_60
    PIXEL11_70
    PIXEL12_70
    PIXEL13_60
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag167:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_60
    PIXEL21_70
    PIXEL22_70
    PIXEL23_60
    PIXEL30_20
    PIXEL31_60
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag173:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL10_32
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL20_31
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL30_81
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag181:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    PIXEL03_81
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL13_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL23_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag186:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag115:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL10_81
    PIXEL11_31
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    PIXEL30_82
    PIXEL31_32
    jmp .loopx_end
..@flag93:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    jmp .loopx_end
..@flag206:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    PIXEL02_32
    PIXEL03_82
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_31
    PIXEL23_81
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag205:
..@flag201:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL10_32
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    DiffOrNot w8,w4,PIXEL20_10,PIXEL21_30,PIXEL30_80,PIXEL31_10,PIXEL20_12,PIXEL21_0,PIXEL30_20,PIXEL31_11
    PIXEL22_31
    PIXEL23_81
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag174:
..@flag46:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_80,PIXEL01_10,PIXEL10_10,PIXEL11_30,PIXEL00_20,PIXEL01_12,PIXEL10_11,PIXEL11_0
    PIXEL02_32
    PIXEL03_82
    PIXEL12_32
    PIXEL13_82
    PIXEL20_31
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL30_81
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag179:
..@flag147:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    DiffOrNot w2,w6,PIXEL02_10,PIXEL03_80,PIXEL12_30,PIXEL13_10,PIXEL02_11,PIXEL03_20,PIXEL12_0,PIXEL13_12
    PIXEL10_81
    PIXEL11_31
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL23_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag117:
..@flag116:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    PIXEL03_81
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL13_31
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_30,PIXEL23_10,PIXEL32_10,PIXEL33_80,PIXEL22_0,PIXEL23_11,PIXEL32_12,PIXEL33_20
    PIXEL30_82
    PIXEL31_32
    jmp .loopx_end
..@flag189:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag231:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_32
    PIXEL03_82
    PIXEL10_81
    PIXEL11_31
    PIXEL12_32
    PIXEL13_82
    PIXEL20_82
    PIXEL21_32
    PIXEL22_31
    PIXEL23_81
    PIXEL30_82
    PIXEL31_32
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag126:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_10
    PIXEL11_30
    PIXEL12_0
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag219:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_10
    PIXEL20_10
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag125:
    AUXADDRESS
    DiffOrNot w8,w4,PIXEL00_82,PIXEL10_32,PIXEL20_0,PIXEL21_0,PIXEL30_0,PIXEL31_0,PIXEL00_11,PIXEL10_13,PIXEL20_83,PIXEL21_70,PIXEL30_50,PIXEL31_21
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag221:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    DiffOrNot w6,w8,PIXEL03_81,PIXEL13_31,PIXEL22_0,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL03_12,PIXEL13_14,PIXEL22_70,PIXEL23_83,PIXEL32_21,PIXEL33_50
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL20_10
    PIXEL21_30
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag207:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL02_32,PIXEL03_82,PIXEL10_0,PIXEL11_0,PIXEL00_50,PIXEL01_83,PIXEL02_13,PIXEL03_11,PIXEL10_21,PIXEL11_70
    PIXEL12_32
    PIXEL13_82
    PIXEL20_10
    PIXEL21_30
    PIXEL22_31
    PIXEL23_81
    PIXEL30_80
    PIXEL31_10
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag238:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_32
    PIXEL03_82
    PIXEL10_10
    PIXEL11_30
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_0,PIXEL21_0,PIXEL30_0,PIXEL31_0,PIXEL32_31,PIXEL33_81,PIXEL20_21,PIXEL21_70,PIXEL30_50,PIXEL31_83,PIXEL32_14,PIXEL33_12
    PIXEL22_31
    PIXEL23_81
    jmp .loopx_end
..@flag190:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL12_0,PIXEL13_0,PIXEL23_32,PIXEL33_82,PIXEL02_21,PIXEL03_50,PIXEL12_70,PIXEL13_83,PIXEL23_13,PIXEL33_11
    PIXEL10_10
    PIXEL11_30
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    jmp .loopx_end
..@flag187:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL11_0,PIXEL20_31,PIXEL30_81,PIXEL00_50,PIXEL01_21,PIXEL10_83,PIXEL11_70,PIXEL20_14,PIXEL30_12
    PIXEL02_10
    PIXEL03_80
    PIXEL12_30
    PIXEL13_10
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag243:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_10
    PIXEL03_80
    PIXEL10_81
    PIXEL11_31
    PIXEL12_30
    PIXEL13_10
    PIXEL20_82
    PIXEL21_32
    DiffOrNot w6,w8,PIXEL22_0,PIXEL23_0,PIXEL30_82,PIXEL31_32,PIXEL32_0,PIXEL33_0,PIXEL22_70,PIXEL23_21,PIXEL30_11,PIXEL31_13,PIXEL32_83,PIXEL33_50
    jmp .loopx_end
..@flag119:
    AUXADDRESS
    DiffOrNot w2,w6,PIXEL00_81,PIXEL01_31,PIXEL02_0,PIXEL03_0,PIXEL12_0,PIXEL13_0,PIXEL00_12,PIXEL01_14,PIXEL02_83,PIXEL03_50,PIXEL12_70,PIXEL13_21
    PIXEL10_81
    PIXEL11_31
    PIXEL20_82
    PIXEL21_32
    PIXEL22_30
    PIXEL23_10
    PIXEL30_82
    PIXEL31_32
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag237:
..@flag233:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_60
    PIXEL03_20
    PIXEL10_32
    PIXEL11_32
    PIXEL12_70
    PIXEL13_60
    PIXEL20_0
    PIXEL21_0
    PIXEL22_31
    PIXEL23_81
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag175:
..@flag47:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    PIXEL02_32
    PIXEL03_82
    PIXEL10_0
    PIXEL11_0
    PIXEL12_32
    PIXEL13_82
    PIXEL20_31
    PIXEL21_31
    PIXEL22_70
    PIXEL23_60
    PIXEL30_81
    PIXEL31_81
    PIXEL32_60
    PIXEL33_20
    jmp .loopx_end
..@flag183:
..@flag151:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL10_81
    PIXEL11_31
    PIXEL12_0
    PIXEL13_0
    PIXEL20_60
    PIXEL21_70
    PIXEL22_32
    PIXEL23_32
    PIXEL30_20
    PIXEL31_60
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag245:
..@flag244:
    AUXADDRESS
    PIXEL00_20
    PIXEL01_60
    PIXEL02_81
    PIXEL03_81
    PIXEL10_60
    PIXEL11_70
    PIXEL12_31
    PIXEL13_31
    PIXEL20_82
    PIXEL21_32
    PIXEL22_0
    PIXEL23_0
    PIXEL30_82
    PIXEL31_32
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag250:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    PIXEL02_10
    PIXEL03_80
    PIXEL10_10
    PIXEL11_30
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    jmp .loopx_end
..@flag123:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_10
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag95:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL11_0
    PIXEL12_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_30
    PIXEL23_10
    PIXEL30_80
    PIXEL31_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag222:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_10
    PIXEL11_30
    PIXEL12_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag252:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_61
    PIXEL02_81
    PIXEL03_81
    PIXEL10_10
    PIXEL11_30
    PIXEL12_31
    PIXEL13_31
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_0
    PIXEL23_0
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag249:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_61
    PIXEL03_80
    PIXEL10_32
    PIXEL11_32
    PIXEL12_30
    PIXEL13_10
    PIXEL20_0
    PIXEL21_0
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    jmp .loopx_end
..@flag235:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_61
    PIXEL20_0
    PIXEL21_0
    PIXEL22_31
    PIXEL23_81
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag111:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    PIXEL02_32
    PIXEL03_82
    PIXEL10_0
    PIXEL11_0
    PIXEL12_32
    PIXEL13_82
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_61
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag63:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_0
    PIXEL11_0
    PIXEL12_0
    PIXEL20_31
    PIXEL21_31
    PIXEL22_30
    PIXEL23_10
    PIXEL30_81
    PIXEL31_81
    PIXEL32_61
    PIXEL33_80
    jmp .loopx_end
..@flag159:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL11_0
    PIXEL12_0
    PIXEL13_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_32
    PIXEL23_32
    PIXEL30_80
    PIXEL31_61
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag215:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL10_81
    PIXEL11_31
    PIXEL12_0
    PIXEL13_0
    PIXEL20_61
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag246:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_61
    PIXEL11_30
    PIXEL12_0
    PIXEL20_82
    PIXEL21_32
    PIXEL22_0
    PIXEL23_0
    PIXEL30_82
    PIXEL31_32
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag254:
    AUXADDRESS
    PIXEL00_80
    PIXEL01_10
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_10
    PIXEL11_30
    PIXEL12_0
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_0
    PIXEL23_0
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag253:
    AUXADDRESS
    PIXEL00_82
    PIXEL01_82
    PIXEL02_81
    PIXEL03_81
    PIXEL10_32
    PIXEL11_32
    PIXEL12_31
    PIXEL13_31
    PIXEL20_0
    PIXEL21_0
    PIXEL22_0
    PIXEL23_0
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag251:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_10
    PIXEL03_80
    PIXEL11_0
    PIXEL12_30
    PIXEL13_10
    PIXEL20_0
    PIXEL21_0
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    jmp .loopx_end
..@flag239:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    PIXEL02_32
    PIXEL03_82
    PIXEL10_0
    PIXEL11_0
    PIXEL12_32
    PIXEL13_82
    PIXEL20_0
    PIXEL21_0
    PIXEL22_31
    PIXEL23_81
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    PIXEL32_31
    PIXEL33_81
    jmp .loopx_end
..@flag127:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    DiffOrNot w2,w6,PIXEL02_0,PIXEL03_0,PIXEL13_0,PIXEL02_50,PIXEL03_50,PIXEL13_50
    PIXEL10_0
    PIXEL11_0
    PIXEL12_0
    DiffOrNot w8,w4,PIXEL20_0,PIXEL30_0,PIXEL31_0,PIXEL20_50,PIXEL30_50,PIXEL31_50
    PIXEL21_0
    PIXEL22_30
    PIXEL23_10
    PIXEL32_10
    PIXEL33_80
    jmp .loopx_end
..@flag191:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL10_0
    PIXEL11_0
    PIXEL12_0
    PIXEL13_0
    PIXEL20_31
    PIXEL21_31
    PIXEL22_32
    PIXEL23_32
    PIXEL30_81
    PIXEL31_81
    PIXEL32_82
    PIXEL33_82
    jmp .loopx_end
..@flag223:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL01_0,PIXEL10_0,PIXEL00_50,PIXEL01_50,PIXEL10_50
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL11_0
    PIXEL12_0
    PIXEL13_0
    PIXEL20_10
    PIXEL21_30
    PIXEL22_0
    DiffOrNot w6,w8,PIXEL23_0,PIXEL32_0,PIXEL33_0,PIXEL23_50,PIXEL32_50,PIXEL33_50
    PIXEL30_80
    PIXEL31_10
    jmp .loopx_end
..@flag247:
    AUXADDRESS
    PIXEL00_81
    PIXEL01_31
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL10_81
    PIXEL11_31
    PIXEL12_0
    PIXEL13_0
    PIXEL20_82
    PIXEL21_32
    PIXEL22_0
    PIXEL23_0
    PIXEL30_82
    PIXEL31_32
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end
..@flag255:
    AUXADDRESS
    DiffOrNot w4,w2,PIXEL00_0,PIXEL00_20
    PIXEL01_0
    PIXEL02_0
    DiffOrNot w2,w6,PIXEL03_0,PIXEL03_20
    PIXEL10_0
    PIXEL11_0
    PIXEL12_0
    PIXEL13_0
    PIXEL20_0
    PIXEL21_0
    PIXEL22_0
    PIXEL23_0
    DiffOrNot w8,w4,PIXEL30_0,PIXEL30_20
    PIXEL31_0
    PIXEL32_0
    DiffOrNot w6,w8,PIXEL33_0,PIXEL33_20
    jmp .loopx_end

.loopx_end:
		; pixcnt
		; & 0x7
		; if 0
		;		prefetchw rsi+64
		;		& 0x1F
		;		if 0
		;			prefetch rdi+64
		;		endif
		; endif
    mov     eax,dword[pixcnt]
		test    eax, 0x7
		jnz     .noprefetch
		prefetchw [rsi+64]
		test    eax, 0x1F
		jnz     .noprefetch
		prefetch [rdi+64]
.noprefetch:
    add     rdi,2
    add     rsi,16
		inc     dword[pixcnt]
    dec     dword[xcounter]
    jle     .xres_2
    jmp     .loopx
.xres_2:
    ; x=Xres-2 - special case
    jl      .xres_1
    movsxd  rbx,dword[prevline]
    movq    mm5,[rdi+rbx-4]
    movq    mm6,[rdi-4]
    movsxd  rbx,dword[nextline]
    movq    mm7,[rdi+rbx-4]
    psrlq   mm5,16
    psrlq   mm6,16
    psrlq   mm7,16
    movd    eax,mm5
    movzx   edx,ax
    mov     [w1],edx
    shr     eax,16
    mov     [w2],eax
    psrlq   mm5,32
    movd    eax,mm5
    mov     [w3],eax
    movd    eax,mm6
    movzx   edx,ax
    mov     [w4],edx
    shr     eax,16
    mov     [w5],eax
    psrlq   mm6,32
    movd    eax,mm6
    mov     [w6],eax
    movd    eax,mm7
    movzx   edx,ax
    mov     [w7],edx
    shr     eax,16
    mov     [w8],eax
    psrlq   mm7,32
    movd    eax,mm7
    mov     [w9],eax
    jmp     .flags
.xres_1:
    cmp     dword[xcounter],-1
    jl      .nexty
    ; x=Xres-1 - special case
    movsxd  rbx,dword[prevline]
    movq    mm5,[rdi+rbx-6]
    movq    mm6,[rdi-6]
    movsxd  rbx,dword[nextline]
    movq    mm7,[rdi+rbx-6]
    psrlq   mm5,32
    psrlq   mm6,32
    psrlq   mm7,32
    movd    eax,mm5
    movzx   edx,ax
    mov     [w1],edx
    shr     eax,16
    mov     [w2],eax
    mov     [w3],eax
    movd    eax,mm6
    movzx   edx,ax
    mov     [w4],edx
    shr     eax,16
    mov     [w5],eax
    mov     [w6],eax
    movd    eax,mm7
    movzx   edx,ax
    mov     [w7],edx
    shr     eax,16
    mov     [w8],eax
    mov     [w9],eax
    jmp     .flags
.nexty:
		; we add 3 times the pitch
    add     rsi,rbx
    add     rsi,rbx
    add     rsi,rbx
    dec     dword[linesleft]
    jz      .end
    mov     ebx,[rbp-Xres]
    shl     ebx,1
    cmp     dword[linesleft],1
    je      .lastline
    mov     dword[nextline],ebx
    neg     ebx
    mov     dword[prevline],ebx
    jmp     .loopy
.lastline:
    mov     dword[nextline],0
    neg     ebx
    mov     dword[prevline],ebx
    jmp     .loopy
.end:
    emms
		pop rbx
		leave
    ret

SECTION .data
FuncTable:
    dq ..@flag0, ..@flag1, ..@flag2, ..@flag3, ..@flag4, ..@flag5, ..@flag6, ..@flag7
    dq ..@flag8, ..@flag9, ..@flag10, ..@flag11, ..@flag12, ..@flag13, ..@flag14, ..@flag15
    dq ..@flag16, ..@flag17, ..@flag18, ..@flag19, ..@flag20, ..@flag21, ..@flag22, ..@flag23
    dq ..@flag24, ..@flag25, ..@flag26, ..@flag27, ..@flag28, ..@flag29, ..@flag30, ..@flag31
    dq ..@flag32, ..@flag33, ..@flag34, ..@flag35, ..@flag36, ..@flag37, ..@flag38, ..@flag39
    dq ..@flag40, ..@flag41, ..@flag42, ..@flag43, ..@flag44, ..@flag45, ..@flag46, ..@flag47
    dq ..@flag48, ..@flag49, ..@flag50, ..@flag51, ..@flag52, ..@flag53, ..@flag54, ..@flag55
    dq ..@flag56, ..@flag57, ..@flag58, ..@flag59, ..@flag60, ..@flag61, ..@flag62, ..@flag63
    dq ..@flag64, ..@flag65, ..@flag66, ..@flag67, ..@flag68, ..@flag69, ..@flag70, ..@flag71
    dq ..@flag72, ..@flag73, ..@flag74, ..@flag75, ..@flag76, ..@flag77, ..@flag78, ..@flag79
    dq ..@flag80, ..@flag81, ..@flag82, ..@flag83, ..@flag84, ..@flag85, ..@flag86, ..@flag87
    dq ..@flag88, ..@flag89, ..@flag90, ..@flag91, ..@flag92, ..@flag93, ..@flag94, ..@flag95
    dq ..@flag96, ..@flag97, ..@flag98, ..@flag99, ..@flag100, ..@flag101, ..@flag102, ..@flag103
    dq ..@flag104, ..@flag105, ..@flag106, ..@flag107, ..@flag108, ..@flag109, ..@flag110, ..@flag111
    dq ..@flag112, ..@flag113, ..@flag114, ..@flag115, ..@flag116, ..@flag117, ..@flag118, ..@flag119
    dq ..@flag120, ..@flag121, ..@flag122, ..@flag123, ..@flag124, ..@flag125, ..@flag126, ..@flag127
    dq ..@flag128, ..@flag129, ..@flag130, ..@flag131, ..@flag132, ..@flag133, ..@flag134, ..@flag135
    dq ..@flag136, ..@flag137, ..@flag138, ..@flag139, ..@flag140, ..@flag141, ..@flag142, ..@flag143
    dq ..@flag144, ..@flag145, ..@flag146, ..@flag147, ..@flag148, ..@flag149, ..@flag150, ..@flag151
    dq ..@flag152, ..@flag153, ..@flag154, ..@flag155, ..@flag156, ..@flag157, ..@flag158, ..@flag159
    dq ..@flag160, ..@flag161, ..@flag162, ..@flag163, ..@flag164, ..@flag165, ..@flag166, ..@flag167
    dq ..@flag168, ..@flag169, ..@flag170, ..@flag171, ..@flag172, ..@flag173, ..@flag174, ..@flag175
    dq ..@flag176, ..@flag177, ..@flag178, ..@flag179, ..@flag180, ..@flag181, ..@flag182, ..@flag183
    dq ..@flag184, ..@flag185, ..@flag186, ..@flag187, ..@flag188, ..@flag189, ..@flag190, ..@flag191
    dq ..@flag192, ..@flag193, ..@flag194, ..@flag195, ..@flag196, ..@flag197, ..@flag198, ..@flag199
    dq ..@flag200, ..@flag201, ..@flag202, ..@flag203, ..@flag204, ..@flag205, ..@flag206, ..@flag207
    dq ..@flag208, ..@flag209, ..@flag210, ..@flag211, ..@flag212, ..@flag213, ..@flag214, ..@flag215
    dq ..@flag216, ..@flag217, ..@flag218, ..@flag219, ..@flag220, ..@flag221, ..@flag222, ..@flag223
    dq ..@flag224, ..@flag225, ..@flag226, ..@flag227, ..@flag228, ..@flag229, ..@flag230, ..@flag231
    dq ..@flag232, ..@flag233, ..@flag234, ..@flag235, ..@flag236, ..@flag237, ..@flag238, ..@flag239
    dq ..@flag240, ..@flag241, ..@flag242, ..@flag243, ..@flag244, ..@flag245, ..@flag246, ..@flag247
    dq ..@flag248, ..@flag249, ..@flag250, ..@flag251, ..@flag252, ..@flag253, ..@flag254, ..@flag255
