   1              		.arch armv6-m
   2              		.eabi_attribute 20, 1
   3              		.eabi_attribute 21, 1
   4              		.eabi_attribute 23, 3
   5              		.eabi_attribute 24, 1
   6              		.eabi_attribute 25, 1
   7              		.eabi_attribute 26, 1
   8              		.eabi_attribute 30, 6
   9              		.eabi_attribute 34, 0
  10              		.eabi_attribute 18, 4
  11              		.file	"centralenhet.c"
  12              		.text
  13              	.Ltext0:
  14              		.cfi_sections	.debug_frame
  15              		.align	1
  16              		.global	mainInit
  17              		.syntax unified
  18              		.code	16
  19              		.thumb_func
  20              		.fpu softvfp
  22              	mainInit:
  23              	.LFB112:
  24              		.file 1 "C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet/cen
   1:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #include "stm32f4xx_gpio.h"
   2:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #include "stm32f4xx_can.h"
   3:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #include "stm32f4xx_rcc.h"
   4:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
   5:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** // Required pointers for interupts
   6:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define SYSCFG				0x40013800
   7:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define SYSCFG_EXTICR1		((volatile unsigned int *) (SYSCFG+0x8))
   8:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
   9:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI				0x40013C00
  10:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI_IMR			((volatile unsigned int *) (EXTI))
  11:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI_FTSR			((volatile unsigned int *) (EXTI+0xC))
  12:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI_RTSR			((volatile unsigned int *) (EXTI+0x8))
  13:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI_PR				((volatile unsigned int *) (EXTI+0x14))
  14:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  15:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI0_IRQVEC		((void (**)(void)) 0x2001C058)
  16:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  17:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define NVIC_ISER0			((volatile unsigned int *) 0xE000E100)
  18:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define NVIC_EXTI0_IRQ_BPOS	(1 << 6)
  19:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  20:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define EXTI0_IRQ_BPOS		1
  21:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  22:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define SCB_VTOR 			((volatile unsigned long *) 0xE000ED08)
  23:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  24:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define GPIO_Pin_Low               ((uint16_t)0x00FF)  /* All pins selected */
  25:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** #define GPIO_Pin_High              ((uint16_t)0xFF00)  /* All pins selected */
  26:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  27:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  28:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  29:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** void mainInit(void){
  25              		.loc 1 29 0
  26              		.cfi_startproc
  27              		@ args = 0, pretend = 0, frame = 8
  28              		@ frame_needed = 1, uses_anonymous_args = 0
  29 0000 80B5     		push	{r7, lr}
  30              		.cfi_def_cfa_offset 8
  31              		.cfi_offset 7, -8
  32              		.cfi_offset 14, -4
  33 0002 82B0     		sub	sp, sp, #8
  34              		.cfi_def_cfa_offset 16
  35 0004 00AF     		add	r7, sp, #0
  36              		.cfi_def_cfa_register 7
  30:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	GPIO_InitTypeDef init;
  31:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	GPIO_StructInit( &init );
  37              		.loc 1 31 0
  38 0006 3B00     		movs	r3, r7
  39 0008 1800     		movs	r0, r3
  40 000a FFF7FEFF 		bl	GPIO_StructInit
  32:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	
  33:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
  41              		.loc 1 33 0
  42 000e 3B00     		movs	r3, r7
  43 0010 F022     		movs	r2, #240
  44 0012 1201     		lsls	r2, r2, #4
  45 0014 1A60     		str	r2, [r3]
  34:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Mode = GPIO_Mode_IN;
  46              		.loc 1 34 0
  47 0016 3B00     		movs	r3, r7
  48 0018 0022     		movs	r2, #0
  49 001a 1A71     		strb	r2, [r3, #4]
  35:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_OType = GPIO_OType_PP;
  50              		.loc 1 35 0
  51 001c 3B00     		movs	r3, r7
  52 001e 0022     		movs	r2, #0
  53 0020 9A71     		strb	r2, [r3, #6]
  36:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_PuPd = GPIO_PuPd_DOWN;
  54              		.loc 1 36 0
  55 0022 3B00     		movs	r3, r7
  56 0024 0222     		movs	r2, #2
  57 0026 DA71     		strb	r2, [r3, #7]
  37:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Speed = GPIO_Speed_50MHz;
  58              		.loc 1 37 0
  59 0028 3B00     		movs	r3, r7
  60 002a 0222     		movs	r2, #2
  61 002c 5A71     		strb	r2, [r3, #5]
  38:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	GPIO_Init(GPIOD, &init);
  62              		.loc 1 38 0
  63 002e 3B00     		movs	r3, r7
  64 0030 194A     		ldr	r2, .L2
  65 0032 1900     		movs	r1, r3
  66 0034 1000     		movs	r0, r2
  67 0036 FFF7FEFF 		bl	GPIO_Init
  39:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	
  40:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Pin = GPIO_Pin_Low;
  68              		.loc 1 40 0
  69 003a 3B00     		movs	r3, r7
  70 003c FF22     		movs	r2, #255
  71 003e 1A60     		str	r2, [r3]
  41:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Mode = GPIO_Mode_OUT;
  72              		.loc 1 41 0
  73 0040 3B00     		movs	r3, r7
  74 0042 0122     		movs	r2, #1
  75 0044 1A71     		strb	r2, [r3, #4]
  42:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_OType = GPIO_OType_PP;
  76              		.loc 1 42 0
  77 0046 3B00     		movs	r3, r7
  78 0048 0022     		movs	r2, #0
  79 004a 9A71     		strb	r2, [r3, #6]
  43:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  80              		.loc 1 43 0
  81 004c 3B00     		movs	r3, r7
  82 004e 0022     		movs	r2, #0
  83 0050 DA71     		strb	r2, [r3, #7]
  44:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Speed = GPIO_Speed_50MHz;
  84              		.loc 1 44 0
  85 0052 3B00     		movs	r3, r7
  86 0054 0222     		movs	r2, #2
  87 0056 5A71     		strb	r2, [r3, #5]
  45:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	GPIO_Init(GPIOD, &init);
  88              		.loc 1 45 0
  89 0058 3B00     		movs	r3, r7
  90 005a 0F4A     		ldr	r2, .L2
  91 005c 1900     		movs	r1, r3
  92 005e 1000     		movs	r0, r2
  93 0060 FFF7FEFF 		bl	GPIO_Init
  46:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	
  47:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 ;
  94              		.loc 1 47 0
  95 0064 3B00     		movs	r3, r7
  96 0066 F022     		movs	r2, #240
  97 0068 1202     		lsls	r2, r2, #8
  98 006a 1A60     		str	r2, [r3]
  48:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Mode = GPIO_Mode_OUT;
  99              		.loc 1 48 0
 100 006c 3B00     		movs	r3, r7
 101 006e 0122     		movs	r2, #1
 102 0070 1A71     		strb	r2, [r3, #4]
  49:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_OType = GPIO_OType_PP;
 103              		.loc 1 49 0
 104 0072 3B00     		movs	r3, r7
 105 0074 0022     		movs	r2, #0
 106 0076 9A71     		strb	r2, [r3, #6]
  50:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
 107              		.loc 1 50 0
 108 0078 3B00     		movs	r3, r7
 109 007a 0022     		movs	r2, #0
 110 007c DA71     		strb	r2, [r3, #7]
  51:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	init.GPIO_Speed = GPIO_Speed_50MHz;
 111              		.loc 1 51 0
 112 007e 3B00     		movs	r3, r7
 113 0080 0222     		movs	r2, #2
 114 0082 5A71     		strb	r2, [r3, #5]
  52:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	GPIO_Init(GPIOD, &init);
 115              		.loc 1 52 0
 116 0084 3B00     		movs	r3, r7
 117 0086 044A     		ldr	r2, .L2
 118 0088 1900     		movs	r1, r3
 119 008a 1000     		movs	r0, r2
 120 008c FFF7FEFF 		bl	GPIO_Init
  53:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  54:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** }
 121              		.loc 1 54 0
 122 0090 C046     		nop
 123 0092 BD46     		mov	sp, r7
 124 0094 02B0     		add	sp, sp, #8
 125              		@ sp needed
 126 0096 80BD     		pop	{r7, pc}
 127              	.L3:
 128              		.align	2
 129              	.L2:
 130 0098 000C0240 		.word	1073875968
 131              		.cfi_endproc
 132              	.LFE112:
 134              		.align	1
 135              		.global	kbdActivate
 136              		.syntax unified
 137              		.code	16
 138              		.thumb_func
 139              		.fpu softvfp
 141              	kbdActivate:
 142              	.LFB113:
  55:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  56:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** void kbdActivate(unsigned int row) {
 143              		.loc 1 56 0
 144              		.cfi_startproc
 145              		@ args = 0, pretend = 0, frame = 8
 146              		@ frame_needed = 1, uses_anonymous_args = 0
 147 009c 80B5     		push	{r7, lr}
 148              		.cfi_def_cfa_offset 8
 149              		.cfi_offset 7, -8
 150              		.cfi_offset 14, -4
 151 009e 82B0     		sub	sp, sp, #8
 152              		.cfi_def_cfa_offset 16
 153 00a0 00AF     		add	r7, sp, #0
 154              		.cfi_def_cfa_register 7
 155 00a2 7860     		str	r0, [r7, #4]
  57:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     switch(row) {
 156              		.loc 1 57 0
 157 00a4 7B68     		ldr	r3, [r7, #4]
 158 00a6 022B     		cmp	r3, #2
 159 00a8 10D0     		beq	.L6
 160 00aa 02D8     		bhi	.L7
 161 00ac 012B     		cmp	r3, #1
 162 00ae 05D0     		beq	.L8
 163 00b0 24E0     		b	.L5
 164              	.L7:
 165 00b2 032B     		cmp	r3, #3
 166 00b4 12D0     		beq	.L9
 167 00b6 042B     		cmp	r3, #4
 168 00b8 18D0     		beq	.L10
 169 00ba 1FE0     		b	.L5
 170              	.L8:
  58:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         case 1: GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET ); break;
 171              		.loc 1 58 0
 172 00bc 8023     		movs	r3, #128
 173 00be 5B01     		lsls	r3, r3, #5
 174 00c0 1448     		ldr	r0, .L12
 175 00c2 0122     		movs	r2, #1
 176 00c4 1900     		movs	r1, r3
 177 00c6 FFF7FEFF 		bl	GPIO_WriteBit
 178 00ca 1EE0     		b	.L11
 179              	.L6:
  59:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         case 2: GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET ); break;
 180              		.loc 1 59 0
 181 00cc 8023     		movs	r3, #128
 182 00ce 9B01     		lsls	r3, r3, #6
 183 00d0 1048     		ldr	r0, .L12
 184 00d2 0122     		movs	r2, #1
 185 00d4 1900     		movs	r1, r3
 186 00d6 FFF7FEFF 		bl	GPIO_WriteBit
 187 00da 16E0     		b	.L11
 188              	.L9:
  60:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         case 3: GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET ); break;
 189              		.loc 1 60 0
 190 00dc 8023     		movs	r3, #128
 191 00de DB01     		lsls	r3, r3, #7
 192 00e0 0C48     		ldr	r0, .L12
 193 00e2 0122     		movs	r2, #1
 194 00e4 1900     		movs	r1, r3
 195 00e6 FFF7FEFF 		bl	GPIO_WriteBit
 196 00ea 0EE0     		b	.L11
 197              	.L10:
  61:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         case 4: GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET ); break;
 198              		.loc 1 61 0
 199 00ec 8023     		movs	r3, #128
 200 00ee 1B02     		lsls	r3, r3, #8
 201 00f0 0848     		ldr	r0, .L12
 202 00f2 0122     		movs	r2, #1
 203 00f4 1900     		movs	r1, r3
 204 00f6 FFF7FEFF 		bl	GPIO_WriteBit
 205 00fa 06E0     		b	.L11
 206              	.L5:
  62:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         default: GPIO_WriteBit(GPIOD, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12, Bit_RE
 207              		.loc 1 62 0
 208 00fc F023     		movs	r3, #240
 209 00fe 1B02     		lsls	r3, r3, #8
 210 0100 0448     		ldr	r0, .L12
 211 0102 0022     		movs	r2, #0
 212 0104 1900     		movs	r1, r3
 213 0106 FFF7FEFF 		bl	GPIO_WriteBit
 214              	.L11:
  63:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     }
  64:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** }
 215              		.loc 1 64 0
 216 010a C046     		nop
 217 010c BD46     		mov	sp, r7
 218 010e 02B0     		add	sp, sp, #8
 219              		@ sp needed
 220 0110 80BD     		pop	{r7, pc}
 221              	.L13:
 222 0112 C046     		.align	2
 223              	.L12:
 224 0114 000C0240 		.word	1073875968
 225              		.cfi_endproc
 226              	.LFE113:
 228              		.align	1
 229              		.global	kbdGetCol
 230              		.syntax unified
 231              		.code	16
 232              		.thumb_func
 233              		.fpu softvfp
 235              	kbdGetCol:
 236              	.LFB114:
  65:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  66:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** int kbdGetCol(void) {
 237              		.loc 1 66 0
 238              		.cfi_startproc
 239              		@ args = 0, pretend = 0, frame = 8
 240              		@ frame_needed = 1, uses_anonymous_args = 0
 241 0118 90B5     		push	{r4, r7, lr}
 242              		.cfi_def_cfa_offset 12
 243              		.cfi_offset 4, -12
 244              		.cfi_offset 7, -8
 245              		.cfi_offset 14, -4
 246 011a 83B0     		sub	sp, sp, #12
 247              		.cfi_def_cfa_offset 24
 248 011c 00AF     		add	r7, sp, #0
 249              		.cfi_def_cfa_register 7
  67:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     unsigned char c;
  68:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	c = (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))*8 | (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))*4 
 250              		.loc 1 68 0
 251 011e 8023     		movs	r3, #128
 252 0120 1B01     		lsls	r3, r3, #4
 253 0122 284A     		ldr	r2, .L20
 254 0124 1900     		movs	r1, r3
 255 0126 1000     		movs	r0, r2
 256 0128 FFF7FEFF 		bl	GPIO_ReadInputDataBit
 257 012c 0300     		movs	r3, r0
 258 012e DB00     		lsls	r3, r3, #3
 259 0130 DBB2     		uxtb	r3, r3
 260 0132 5CB2     		sxtb	r4, r3
 261 0134 8023     		movs	r3, #128
 262 0136 DB00     		lsls	r3, r3, #3
 263 0138 224A     		ldr	r2, .L20
 264 013a 1900     		movs	r1, r3
 265 013c 1000     		movs	r0, r2
 266 013e FFF7FEFF 		bl	GPIO_ReadInputDataBit
 267 0142 0300     		movs	r3, r0
 268 0144 9B00     		lsls	r3, r3, #2
 269 0146 DBB2     		uxtb	r3, r3
 270 0148 5BB2     		sxtb	r3, r3
 271 014a 2343     		orrs	r3, r4
 272 014c 5CB2     		sxtb	r4, r3
 273 014e 8023     		movs	r3, #128
 274 0150 9B00     		lsls	r3, r3, #2
 275 0152 1C4A     		ldr	r2, .L20
 276 0154 1900     		movs	r1, r3
 277 0156 1000     		movs	r0, r2
 278 0158 FFF7FEFF 		bl	GPIO_ReadInputDataBit
 279 015c 0300     		movs	r3, r0
 280 015e DB18     		adds	r3, r3, r3
 281 0160 DBB2     		uxtb	r3, r3
 282 0162 5BB2     		sxtb	r3, r3
 283 0164 2343     		orrs	r3, r4
 284 0166 5CB2     		sxtb	r4, r3
 285 0168 8023     		movs	r3, #128
 286 016a 5B00     		lsls	r3, r3, #1
 287 016c 154A     		ldr	r2, .L20
 288 016e 1900     		movs	r1, r3
 289 0170 1000     		movs	r0, r2
 290 0172 FFF7FEFF 		bl	GPIO_ReadInputDataBit
 291 0176 0300     		movs	r3, r0
 292 0178 5BB2     		sxtb	r3, r3
 293 017a 2343     		orrs	r3, r4
 294 017c 5AB2     		sxtb	r2, r3
 295 017e FB1D     		adds	r3, r7, #7
 296 0180 1A70     		strb	r2, [r3]
  69:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     if (c & 8) return 4;
 297              		.loc 1 69 0
 298 0182 FB1D     		adds	r3, r7, #7
 299 0184 1B78     		ldrb	r3, [r3]
 300 0186 0822     		movs	r2, #8
 301 0188 1340     		ands	r3, r2
 302 018a 01D0     		beq	.L15
 303              		.loc 1 69 0 is_stmt 0 discriminator 1
 304 018c 0423     		movs	r3, #4
 305 018e 15E0     		b	.L16
 306              	.L15:
  70:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     if (c & 4) return 3;
 307              		.loc 1 70 0 is_stmt 1
 308 0190 FB1D     		adds	r3, r7, #7
 309 0192 1B78     		ldrb	r3, [r3]
 310 0194 0422     		movs	r2, #4
 311 0196 1340     		ands	r3, r2
 312 0198 01D0     		beq	.L17
 313              		.loc 1 70 0 is_stmt 0 discriminator 1
 314 019a 0323     		movs	r3, #3
 315 019c 0EE0     		b	.L16
 316              	.L17:
  71:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     if (c & 2) return 2;
 317              		.loc 1 71 0 is_stmt 1
 318 019e FB1D     		adds	r3, r7, #7
 319 01a0 1B78     		ldrb	r3, [r3]
 320 01a2 0222     		movs	r2, #2
 321 01a4 1340     		ands	r3, r2
 322 01a6 01D0     		beq	.L18
 323              		.loc 1 71 0 is_stmt 0 discriminator 1
 324 01a8 0223     		movs	r3, #2
 325 01aa 07E0     		b	.L16
 326              	.L18:
  72:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     if (c & 1) return 1;
 327              		.loc 1 72 0 is_stmt 1
 328 01ac FB1D     		adds	r3, r7, #7
 329 01ae 1B78     		ldrb	r3, [r3]
 330 01b0 0122     		movs	r2, #1
 331 01b2 1340     		ands	r3, r2
 332 01b4 01D0     		beq	.L19
 333              		.loc 1 72 0 is_stmt 0 discriminator 1
 334 01b6 0123     		movs	r3, #1
 335 01b8 00E0     		b	.L16
 336              	.L19:
  73:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     return 0;
 337              		.loc 1 73 0 is_stmt 1
 338 01ba 0023     		movs	r3, #0
 339              	.L16:
  74:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** }
 340              		.loc 1 74 0
 341 01bc 1800     		movs	r0, r3
 342 01be BD46     		mov	sp, r7
 343 01c0 03B0     		add	sp, sp, #12
 344              		@ sp needed
 345 01c2 90BD     		pop	{r4, r7, pc}
 346              	.L21:
 347              		.align	2
 348              	.L20:
 349 01c4 000C0240 		.word	1073875968
 350              		.cfi_endproc
 351              	.LFE114:
 353              		.section	.rodata
 354              		.align	2
 355              	.LC0:
 356 0000 01       		.byte	1
 357 0001 02       		.byte	2
 358 0002 03       		.byte	3
 359 0003 0A       		.byte	10
 360 0004 04       		.byte	4
 361 0005 05       		.byte	5
 362 0006 06       		.byte	6
 363 0007 0B       		.byte	11
 364 0008 07       		.byte	7
 365 0009 08       		.byte	8
 366 000a 09       		.byte	9
 367 000b 0C       		.byte	12
 368 000c 0E       		.byte	14
 369 000d 00       		.byte	0
 370 000e 0F       		.byte	15
 371 000f 0D       		.byte	13
 372              		.text
 373              		.align	1
 374              		.global	keyb
 375              		.syntax unified
 376              		.code	16
 377              		.thumb_func
 378              		.fpu softvfp
 380              	keyb:
 381              	.LFB115:
  75:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  76:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** unsigned char keyb(void) {
 382              		.loc 1 76 0
 383              		.cfi_startproc
 384              		@ args = 0, pretend = 0, frame = 24
 385              		@ frame_needed = 1, uses_anonymous_args = 0
 386 01c8 90B5     		push	{r4, r7, lr}
 387              		.cfi_def_cfa_offset 12
 388              		.cfi_offset 4, -12
 389              		.cfi_offset 7, -8
 390              		.cfi_offset 14, -4
 391 01ca 87B0     		sub	sp, sp, #28
 392              		.cfi_def_cfa_offset 40
 393 01cc 00AF     		add	r7, sp, #0
 394              		.cfi_def_cfa_register 7
  77:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
 395              		.loc 1 77 0
 396 01ce 3B00     		movs	r3, r7
 397 01d0 164A     		ldr	r2, .L28
 398 01d2 13CA     		ldmia	r2!, {r0, r1, r4}
 399 01d4 13C3     		stmia	r3!, {r0, r1, r4}
 400 01d6 1268     		ldr	r2, [r2]
 401 01d8 1A60     		str	r2, [r3]
  78:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	int row,col;
  79:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	for(row = 1; row <= 4; row++){
 402              		.loc 1 79 0
 403 01da 0123     		movs	r3, #1
 404 01dc 7B61     		str	r3, [r7, #20]
 405 01de 19E0     		b	.L23
 406              	.L26:
  80:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		kbdActivate(row);
 407              		.loc 1 80 0
 408 01e0 7B69     		ldr	r3, [r7, #20]
 409 01e2 1800     		movs	r0, r3
 410 01e4 FFF7FEFF 		bl	kbdActivate
  81:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		if(col = kbdGetCol() ){
 411              		.loc 1 81 0
 412 01e8 FFF7FEFF 		bl	kbdGetCol
 413 01ec 0300     		movs	r3, r0
 414 01ee 3B61     		str	r3, [r7, #16]
 415 01f0 3B69     		ldr	r3, [r7, #16]
 416 01f2 002B     		cmp	r3, #0
 417 01f4 0BD0     		beq	.L24
  82:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 			kbdActivate(0);
 418              		.loc 1 82 0
 419 01f6 0020     		movs	r0, #0
 420 01f8 FFF7FEFF 		bl	kbdActivate
  83:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 			return key[4*(row - 1) + (col - 1)];
 421              		.loc 1 83 0
 422 01fc 7B69     		ldr	r3, [r7, #20]
 423 01fe 013B     		subs	r3, r3, #1
 424 0200 9A00     		lsls	r2, r3, #2
 425 0202 3B69     		ldr	r3, [r7, #16]
 426 0204 013B     		subs	r3, r3, #1
 427 0206 D318     		adds	r3, r2, r3
 428 0208 3A00     		movs	r2, r7
 429 020a D35C     		ldrb	r3, [r2, r3]
 430 020c 09E0     		b	.L27
 431              	.L24:
  79:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		kbdActivate(row);
 432              		.loc 1 79 0 discriminator 2
 433 020e 7B69     		ldr	r3, [r7, #20]
 434 0210 0133     		adds	r3, r3, #1
 435 0212 7B61     		str	r3, [r7, #20]
 436              	.L23:
  79:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		kbdActivate(row);
 437              		.loc 1 79 0 is_stmt 0 discriminator 1
 438 0214 7B69     		ldr	r3, [r7, #20]
 439 0216 042B     		cmp	r3, #4
 440 0218 E2DD     		ble	.L26
  84:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		}
  85:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	}
  86:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	kbdActivate(0);
 441              		.loc 1 86 0 is_stmt 1
 442 021a 0020     		movs	r0, #0
 443 021c FFF7FEFF 		bl	kbdActivate
  87:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	return 0xFF;
 444              		.loc 1 87 0
 445 0220 FF23     		movs	r3, #255
 446              	.L27:
  88:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** }
 447              		.loc 1 88 0 discriminator 1
 448 0222 1800     		movs	r0, r3
 449 0224 BD46     		mov	sp, r7
 450 0226 07B0     		add	sp, sp, #28
 451              		@ sp needed
 452 0228 90BD     		pop	{r4, r7, pc}
 453              	.L29:
 454 022a C046     		.align	2
 455              	.L28:
 456 022c 00000000 		.word	.LC0
 457              		.cfi_endproc
 458              	.LFE115:
 460              		.section	.rodata
 461              		.align	2
 462              	.LC1:
 463 0010 3F       		.byte	63
 464 0011 06       		.byte	6
 465 0012 5B       		.byte	91
 466 0013 4F       		.byte	79
 467 0014 66       		.byte	102
 468 0015 6D       		.byte	109
 469 0016 7D       		.byte	125
 470 0017 07       		.byte	7
 471 0018 7F       		.byte	127
 472 0019 6F       		.byte	111
 473 001a 77       		.byte	119
 474 001b 7C       		.byte	124
 475 001c 39       		.byte	57
 476 001d 5E       		.byte	94
 477 001e 79       		.byte	121
 478 001f 71       		.byte	113
 479              		.text
 480              		.align	1
 481              		.global	out7seg
 482              		.syntax unified
 483              		.code	16
 484              		.thumb_func
 485              		.fpu softvfp
 487              	out7seg:
 488              	.LFB116:
  89:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  90:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** void out7seg(unsigned char c) {
 489              		.loc 1 90 0
 490              		.cfi_startproc
 491              		@ args = 0, pretend = 0, frame = 24
 492              		@ frame_needed = 1, uses_anonymous_args = 0
 493 0230 90B5     		push	{r4, r7, lr}
 494              		.cfi_def_cfa_offset 12
 495              		.cfi_offset 4, -12
 496              		.cfi_offset 7, -8
 497              		.cfi_offset 14, -4
 498 0232 87B0     		sub	sp, sp, #28
 499              		.cfi_def_cfa_offset 40
 500 0234 00AF     		add	r7, sp, #0
 501              		.cfi_def_cfa_register 7
 502 0236 0200     		movs	r2, r0
 503 0238 FB1D     		adds	r3, r7, #7
 504 023a 1A70     		strb	r2, [r3]
  91:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	char segCodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
 505              		.loc 1 91 0
 506 023c 0823     		movs	r3, #8
 507 023e FB18     		adds	r3, r7, r3
 508 0240 0E4A     		ldr	r2, .L34
 509 0242 13CA     		ldmia	r2!, {r0, r1, r4}
 510 0244 13C3     		stmia	r3!, {r0, r1, r4}
 511 0246 1268     		ldr	r2, [r2]
 512 0248 1A60     		str	r2, [r3]
  92:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	if(c != 0xFF){
 513              		.loc 1 92 0
 514 024a FB1D     		adds	r3, r7, #7
 515 024c 1B78     		ldrb	r3, [r3]
 516 024e FF2B     		cmp	r3, #255
 517 0250 0BD0     		beq	.L31
  93:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		GPIO_Write(GPIOD, segCodes[c]);
 518              		.loc 1 93 0
 519 0252 FB1D     		adds	r3, r7, #7
 520 0254 1B78     		ldrb	r3, [r3]
 521 0256 0822     		movs	r2, #8
 522 0258 BA18     		adds	r2, r7, r2
 523 025a D35C     		ldrb	r3, [r2, r3]
 524 025c 9BB2     		uxth	r3, r3
 525 025e 084A     		ldr	r2, .L34+4
 526 0260 1900     		movs	r1, r3
 527 0262 1000     		movs	r0, r2
 528 0264 FFF7FEFF 		bl	GPIO_Write
  94:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	} else {
  95:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		GPIO_Write(GPIOD, 0);
  96:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	}
  97:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** }
 529              		.loc 1 97 0
 530 0268 04E0     		b	.L33
 531              	.L31:
  95:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	}
 532              		.loc 1 95 0
 533 026a 054B     		ldr	r3, .L34+4
 534 026c 0021     		movs	r1, #0
 535 026e 1800     		movs	r0, r3
 536 0270 FFF7FEFF 		bl	GPIO_Write
 537              	.L33:
 538              		.loc 1 97 0
 539 0274 C046     		nop
 540 0276 BD46     		mov	sp, r7
 541 0278 07B0     		add	sp, sp, #28
 542              		@ sp needed
 543 027a 90BD     		pop	{r4, r7, pc}
 544              	.L35:
 545              		.align	2
 546              	.L34:
 547 027c 10000000 		.word	.LC1
 548 0280 000C0240 		.word	1073875968
 549              		.cfi_endproc
 550              	.LFE116:
 552              		.align	1
 553              		.global	think
 554              		.syntax unified
 555              		.code	16
 556              		.thumb_func
 557              		.fpu softvfp
 559              	think:
 560              	.LFB117:
  98:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
  99:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** void think(void) {
 561              		.loc 1 99 0
 562              		.cfi_startproc
 563              		@ args = 0, pretend = 0, frame = 0
 564              		@ frame_needed = 1, uses_anonymous_args = 0
 565 0284 80B5     		push	{r7, lr}
 566              		.cfi_def_cfa_offset 8
 567              		.cfi_offset 7, -8
 568              		.cfi_offset 14, -4
 569 0286 00AF     		add	r7, sp, #0
 570              		.cfi_def_cfa_register 7
 100:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 	mainInit();
 571              		.loc 1 100 0
 572 0288 FFF7FEFF 		bl	mainInit
 573              	.L37:
 101:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 
 102:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****     while (1) {
 103:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c **** 		//GPIO_Write(GPIOD, 0x3F);
 104:C:/Users/freoster/Documents/Datatekniskt-projekt-DAT290/kod/larm-projekt/centralenhet\centralenhet.c ****         out7seg(keyb());
 574              		.loc 1 104 0 discriminator 1
 575 028c FFF7FEFF 		bl	keyb
 576 0290 0300     		movs	r3, r0
 577 0292 1800     		movs	r0, r3
 578 0294 FFF7FEFF 		bl	out7seg
 579 0298 F8E7     		b	.L37
 580              		.cfi_endproc
 581              	.LFE117:
 583              	.Letext0:
 584              		.file 2 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\machine\\_default_types.h"
 585              		.file 3 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\sys\\_stdint.h"
 586              		.file 4 "../stm/inc/core_cm4.h"
 587              		.file 5 "../stm/inc/system_stm32f4xx.h"
 588              		.file 6 "../stm/inc/stm32f4xx.h"
 589              		.file 7 "../stm/inc/stm32f4xx_gpio.h"
