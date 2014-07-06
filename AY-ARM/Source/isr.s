
            area    |.text|, code, readonly

TIM3_IRQHandler proc
            export TIM3_IRQHandler

            ldr r2, =0x40020400
            ldr r0, [r2, #0x10]     ; GPIOB->IDR
            mov r1, r0
            and r0, r0, #3
            cmp r0, #2
            bne l01

; in #FFFD
            ; set data on d-bus
            mov r0, #0x5555
            strh r0, [r2, #0x02]    ; GPIOB->MODER[15:8]
            movs r3, #0x0000

wait_bus_release
            ldr r0, [r2, #0x10]     ; GPIOB->IDR
            tst r0, #3
            bne wait_bus_release

            ; release d-bus
            strh r3, [r2, #0x02]    ; GPIOB->MODER[15:8]
            b exit

l01
            ubfx r1, r1, #8, #8     ; GPIOB->IDR >>= 8
            cmp r0, #1
            bne l02

; out #BFFD
            strb r1, [r2, #0x15]    ; GPIOB->ODR[15:0]
            b exit

l02
            cmp r0, #3
            bne exit

; out #FFFD
            strb r1, [r2, #0x15]    ; GPIOB->ODR[15:0]
            
exit
            ldr r1, =0x40000400
            ldr r0, [r1, #0x3c]     ; TIM3->CCR3
            ldr r0, [r1, #0x40]     ; TIM3->CCR4
            
            bx lr

            align

            endp

            end
