;; writes pink to the display through mmio at
;; 0x1000
;; colour is #ffbcf8

ldli $1, 0x1000
ldli $3, 0x3
ldli $4, 0x14 
ldli $2, 0xf800
ldhi $2, 0xffbc
str $2, $1
add $1, $3, $1
jaleq $0, $4, $0, $0

;;simplified C code below
;;int* count = 0x1000;
;;while (1) {
;;    *count = 0xffbcf800
;;    count += 3;
;;}