BITS 16
ORG 0x7C00

start:
    jmp short main
    nop

bpb_oem:                db 'NESEOS  '
bpb_bytes_per_sector:   dw 512
bpb_sectors_per_cluster:db 2
bpb_reserved_sectors:   dw 4
bpb_num_fats:           db 2
bpb_root_entries:       dw 112
bpb_total_sectors:      dw 720
bpb_media_descriptor:   db 0xF0
bpb_sectors_per_fat:    dw 2
bpb_sectors_per_track:  dw 9
bpb_num_heads:          dw 2
bpb_hidden_sectors:     dd 0
bpb_large_sectors:      dd 0

boot_drive:             db 0

main:
    mov [boot_drive], dl
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
        mov ax, 0x0203
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]

    mov bx, 0x8000
    int 0x13
    jc disk_error
    jmp 0x0000:0x8000

disk_error:
    mov ah, 0x0E
    mov al, 0x21		
    int 0x10
    mov al, 0x21
    int 0x10   
    mov al, 0x20
    int 0x10    
    mov al, 0x44
    int 0x10   
    mov al, 0x49
    int 0x10
    mov al, 0x53
    int 0x10
    mov al, 0x4b
    int 0x10
    mov al, 0x20
    int 0x10
    mov al, 0x45
    int 0x10
    mov al, 0x52
    int 0x10
    mov al, 0x52
    int 0x10
    mov al, 0x4f
    int 0x10
    mov al, 0x52
    int 0x10
    mov al, 0x20
    int 0x10
    mov al, 0x21
    int 0x10
    mov al, 0x21
    int 0x10
    cli
    hlt

TIMES 510 - ($ - $$) db 0
DW 0xAA55

