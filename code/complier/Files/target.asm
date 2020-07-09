sseg segment stack
	        Temp db '0000H','$'
	        N dw 0
	        dw 512 dup(?)
sseg ends
cseg segment
	        assume cs:cseg, ds:sseg
	assume ss:sseg

PrintAX proc
     push si
     push cx
     push dx    
     mov si,offset Temp+3;保存存储结果的字符串的最后一个字符偏移地址
     xor cx,cx           ;对cX清零
     mov cl,4            ;设置循环次数为4次
 MainPart: mov DH,AL           ;将Al的内容传送给DH
     shr AX,1
     shr AX,1
     shr AX,1
     shr AX,1
     and dh,0FH
     add dh,30H
     cmp dh,':' 
     ja isLetter 
     jb No       
 isLetter: add dh,7H   
 No: mov [si],dh 
     dec si
 loop MainPart
print:   mov dx,offset Temp 
     mov ah,09
     int 21H
     pop dx
     pop cx
     pop si
     ret
 PrintAX endp

READ10 PROC NEAR
     XOR DX,DX
     MOV BX,10
RD0:
     MOV AH,1
     INT 21H
     CMP AL,13  ;输入回车结束
     JZ RT0
     AND AX,0FH  ;将AH清零
     XCHG AX,[SI]
     MUL BX      ;将前一位数字乘十和下一位数字相加
     ADD AX,[SI]
     XCHG AX,[SI]
     JMP RD0 

RT0:     RET
READ10 ENDP


fa proc near
	        push bp
	        mov bp, sp
	        sub sp, 18
	        mov bx, word ptr [bp]
	        sub bx, 4
	        mov si, bp
	        sub si, 4
	        mov ax, word ptr [bx]
	        mov word ptr [si], ax
	        sub si, 2
	        mov word ptr [si], bp

	        mov bx, word ptr [bp-8]
	        mov ax, word ptr [bx]
	        mov bx, word ptr [bp-10]
	        add ax, word ptr [bx]
	        mov bx, word ptr [bp-8]
	        mov word ptr [bx], ax

	        mov bx, word ptr [bp-8]
	        mov ax, word ptr [bx]
	        mov bx, word ptr [bp-10]
	        sub ax, word ptr [bx]
	        mov bx, word ptr [bp-10]
	        mov word ptr [bx], ax

	        mov sp, bp
	        pop bp
	        ret
fa endp


fb proc near
	        push bp
	        mov bp, sp
	        sub sp, 12
	        mov bx, word ptr [bp]
	        sub bx, 4
	        mov si, bp
	        sub si, 4
	        mov ax, word ptr [bx]
	        mov word ptr [si], ax
	        sub si, 2
	        mov word ptr [si], bp



	        mov bx, word ptr [bp-6]
	        sub bx, 8
	        mov ax, bx
	        mov si, sp
	        mov word ptr [si-12], ax
	        mov bx, word ptr [bp-6]
	        sub bx, 10
	        mov ax, bx
	        mov si, sp
	        mov word ptr [si-14], ax
	        call fa

	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-8]
	        mov bx, word ptr [bp-6]
	        add ax, word ptr [bx-10]
	        mov bx, word ptr [bp-6]
	        mov word ptr [bx-12], ax

	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-12]
	        mov word ptr [bp-2], ax

	        mov sp, bp
	        pop bp
	        ret
fb endp


fc proc near
	        push bp
	        mov bp, sp
	        sub sp, 22
	        mov bx, word ptr [bp]
	        sub bx, 4
	        mov si, bp
	        sub si, 4
	        mov ax, word ptr [bx]
	        mov word ptr [si], ax
	        sub si, 2
	        mov word ptr [si], bp

	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-8]
	        mov bx, word ptr [bp-6]
	        add ax, word ptr [bx-10]
	        mov bx, word ptr [bp-6]
	        mov word ptr [bx-12], ax



	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-8]
	        mov si, sp
	        mov word ptr [si-12], ax
	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-10]
	        mov si, sp
	        mov word ptr [si-14], ax
	        call fb

	        mov bx, word ptr [bp-6]
	        mov si, sp
	        mov ax, word ptr [si-6]
	        mov word ptr [bx-14], ax

	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-12]
	        mov bx, word ptr [bp-6]
	        add ax, word ptr [bx-14]
	        mov bx, word ptr [bp-6]
	        mov word ptr [bx-16], ax

	        mov bx, word ptr [bp-6]
	        mov ax, word ptr [bx-16]
	        mov word ptr [bp-2], ax

	        mov sp, bp
	        pop bp
	        ret
fc endp

start: mov ax, sseg
	        mov ds, ax
	        mov ax, sseg
	        mov ss, ax
	        push bp
	        mov bp, sp
	        sub sp, 14
	        mov word ptr [bp-4], bp

	        mov ax, 0
	        mov N, ax
	        lea si, N
	        call READ10
	        mov ax, N
	        mov bx, word ptr [bp-4]
	        mov word ptr [bx-8], ax

	        mov ax, 0
	        mov N, ax
	        lea si, N
	        call READ10
	        mov ax, N
	        mov bx, word ptr [bp-4]
	        mov word ptr [bx-6], ax



	        mov bx, word ptr [bp-4]
	        mov ax, word ptr [bx-8]
	        mov si, sp
	        mov word ptr [si-12], ax
	        mov bx, word ptr [bp-4]
	        mov ax, word ptr [bx-6]
	        mov si, sp
	        mov word ptr [si-14], ax
	        call fc

	        mov bx, word ptr [bp-4]
	        mov si, sp
	        mov ax, word ptr [si-6]
	        mov word ptr [bx-10], ax

	        xor ax, ax
	        xor dx, dx
	        mov ax, 2
	        mov bx, word ptr [bp-4]
	        mov cx, word ptr [bx-10]
	        imul cx
	        mov bx, word ptr [bp-4]
	        mov word ptr [bx-12], ax

	        mov bx, word ptr [bp-4]
	        mov ax, word ptr [bx-12]
	        mov bx, word ptr [bp-4]
	        add ax, word ptr [bx-8]
	        mov bx, word ptr [bp-4]
	        mov word ptr [bx-6], ax

	        mov bx, word ptr [bp-4]
	        mov ax, word ptr [bx-6]
	        call PrintAx

	        mov ah, 4ch
	        	        int 21h
cseg ends
	        end start

