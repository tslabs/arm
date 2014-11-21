
            area    |.text|, code, readonly
            preserve8
            import WAddr
            import RReg
            import WReg
            export TIM3_IRQHandler

TIM3_IRQHandler proc
            ldr r2, =0x40020400
            ldr r1, [r2, #0x10]     ; GPIOB->IDR
            mov r0, r1
            and r1, r1, #3
            cmp r1, #2
            bne l01

; read register (in #FFFD)
            ; assert data on d-bus
            mov r1, #0x5555
            strh r1, [r2, #0x02]    ; GPIOB->MODER[15:8]
            movs r3, #0x0000

wait_bus_release
            ldr r1, [r2, #0x10]     ; GPIOB->IDR
            tst r1, #3
            bne wait_bus_release

            ; release d-bus
            strh r3, [r2, #0x02]    ; GPIOB->MODER[15:8]
            b exit

l01
            ubfx r0, r0, #8, #8     ; GPIOB->IDR >>= 8
            cmp r1, #1
            bne l02

; write register (out #BFFD)
            push {lr, r2}
            bl WReg
            b l03

l02
            cmp r1, #3
            bne exit

; write address (out #FFFD)
            push {lr, r2}
			bl WAddr
l03
            bl RReg
            pop {lr, r2}
            strb r0, [r2, #0x15]    ; GPIOB->ODR[15:0]

exit
            ; clear interrupt flag by reading CCRx regs
            ldr r0, =0x40000400
            ldr r1, [r0, #0x3c]     ; TIM3->CCR3
            ldr r1, [r0, #0x40]     ; TIM3->CCR4

            bx lr

            endp
            align
            end
