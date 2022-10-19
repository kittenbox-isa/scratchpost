; multiply function
; does an integer n * m, pass in
; r1 = n
; r2 = m

ldli $3, 0x01 ; sub for loop
ldli $5, 0x00 ; clear iterator
ldli $4, 0x00 ; clear variable
ldli $6, 0x10
add $4, $2, $4
add $3, $5, %5 ; i++
jall $0, $6, $5, $1

; simplified form below
; for (int i = 0; i < n; i++) $4 += m

ldl $3, 0x01 ; sub for loop
ldl $5, 0x00 ; clear iterator
ldl $4, 0x00 ; clear variable
loop:
add $4, $2, $4
add $3, $5, %5 ; i++
jall $0, loop, $5, $1