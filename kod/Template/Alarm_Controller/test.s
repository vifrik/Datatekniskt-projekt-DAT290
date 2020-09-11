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
  11              		.file	"startup.c"
  12              		.text
  13              	.Ltext0:
  14              		.cfi_sections	.debug_frame
  15              		.section	.rodata
  16              		.align	2
  17              	.LC0:
  18 0000 43414E20 		.ascii	"CAN message received: \000"
  18      6D657373 
  18      61676520 
  18      72656365 
  18      69766564 
  19 0017 00       		.align	2
  20              	.LC2:
  21 0018 2A2A2A45 		.ascii	"***Error: Something went wrong :(\000"
  21      72726F72 
  21      3A20536F 
  21      6D657468 
  21      696E6720 
  22              		.text
  23              		.align	1
  24              		.global	receiver
  25              		.syntax unified
  26              		.code	16
  27              		.thumb_func
  28              		.fpu softvfp
  30              	receiver:
  31              	.LFB0:
  32              		.file 1 "C:/Users/nazif/Documents/Ovning_2/Alarm_Controller/startup.c"
   1:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** // A small CAN-application that sends a hello, prints any messages it receives and 
   2:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** //   continually requests words to send via USART. 
   3:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** #include "can.h"
   4:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
   5:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** //
   6:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** // This function should be called only when a CAN message is ready to be received
   7:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** //   currently, it simply prints the message (must be a properly terminated string)
   8:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** //
   9:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** void receiver(void) {
  33              		.loc 1 9 0
  34              		.cfi_startproc
  35              		@ args = 0, pretend = 0, frame = 16
  36              		@ frame_needed = 1, uses_anonymous_args = 0
  37 0000 80B5     		push	{r7, lr}
  38              		.cfi_def_cfa_offset 8
  39              		.cfi_offset 7, -8
  40              		.cfi_offset 14, -4
  41 0002 84B0     		sub	sp, sp, #16
  42              		.cfi_def_cfa_offset 24
  43 0004 00AF     		add	r7, sp, #0
  44              		.cfi_def_cfa_register 7
  10:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	DUMP("CAN message received: ");
  45              		.loc 1 10 0
  46 0006 0C4B     		ldr	r3, .L5
  47 0008 1800     		movs	r0, r3
  48 000a FFF7FEFF 		bl	DUMP
  11:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	
  12:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     CANMsg msg;
  13:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     if (can_receive(&msg))
  49              		.loc 1 13 0
  50 000e 3B1D     		adds	r3, r7, #4
  51 0010 1800     		movs	r0, r3
  52 0012 FFF7FEFF 		bl	can_receive
  53 0016 031E     		subs	r3, r0, #0
  54 0018 05D0     		beq	.L2
  14:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	    DUMP((char*)msg.buff);
  55              		.loc 1 14 0
  56 001a 3B1D     		adds	r3, r7, #4
  57 001c 0333     		adds	r3, r3, #3
  58 001e 1800     		movs	r0, r3
  59 0020 FFF7FEFF 		bl	DUMP
  15:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     else
  16:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****         DUMP("***Error: Something went wrong :(");
  17:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** }
  60              		.loc 1 17 0
  61 0024 03E0     		b	.L4
  62              	.L2:
  16:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** }
  63              		.loc 1 16 0
  64 0026 054B     		ldr	r3, .L5+4
  65 0028 1800     		movs	r0, r3
  66 002a FFF7FEFF 		bl	DUMP
  67              	.L4:
  68              		.loc 1 17 0
  69 002e C046     		nop
  70 0030 BD46     		mov	sp, r7
  71 0032 04B0     		add	sp, sp, #16
  72              		@ sp needed
  73 0034 80BD     		pop	{r7, pc}
  74              	.L6:
  75 0036 C046     		.align	2
  76              	.L5:
  77 0038 00000000 		.word	.LC0
  78 003c 18000000 		.word	.LC2
  79              		.cfi_endproc
  80              	.LFE0:
  82              		.section	.rodata
  83 003a 0000     		.align	2
  84              	.LC4:
  85 003c 0A537461 		.ascii	"\012Starting application\000"
  85      7274696E 
  85      67206170 
  85      706C6963 
  85      6174696F 
  86 0052 0000     		.align	2
  87              	.LC7:
  88 0054 43414E20 		.ascii	"CAN initated\000"
  88      696E6974 
  88      61746564 
  88      00
  89 0061 000000   		.align	2
  90              	.LC9:
  91 0064 48656C6C 		.ascii	"Hello message sent\000"
  91      6F206D65 
  91      73736167 
  91      65207365 
  91      6E7400
  92 0077 00       		.align	2
  93              	.LC11:
  94 0078 456E7465 		.ascii	"Enter a seven letter message\012\000"
  94      72206120 
  94      73657665 
  94      6E206C65 
  94      74746572 
  95 0096 0000     		.align	2
  96              	.LC13:
  97 0098 4D657373 		.ascii	"Message sent: \000"
  97      61676520 
  97      73656E74 
  97      3A2000
  98 00a7 00       		.align	2
  99              	.LC15:
 100 00a8 0A00     		.ascii	"\012\000"
 101              		.text
 102              		.align	1
 103              		.global	main
 104              		.syntax unified
 105              		.code	16
 106              		.thumb_func
 107              		.fpu softvfp
 109              	main:
 110              	.LFB1:
  18:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  19:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  20:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** void main() {
 111              		.loc 1 20 0
 112              		.cfi_startproc
 113              		@ args = 0, pretend = 0, frame = 16
 114              		@ frame_needed = 1, uses_anonymous_args = 0
 115 0040 80B5     		push	{r7, lr}
 116              		.cfi_def_cfa_offset 8
 117              		.cfi_offset 7, -8
 118              		.cfi_offset 14, -4
 119 0042 84B0     		sub	sp, sp, #16
 120              		.cfi_def_cfa_offset 24
 121 0044 00AF     		add	r7, sp, #0
 122              		.cfi_def_cfa_register 7
  21:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	DUMP("\nStarting application");
 123              		.loc 1 21 0
 124 0046 2F4B     		ldr	r3, .L11
 125 0048 1800     		movs	r0, r3
 126 004a FFF7FEFF 		bl	DUMP
  22:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  23:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     can1_init(receiver);
 127              		.loc 1 23 0
 128 004e 2E4B     		ldr	r3, .L11+4
 129 0050 1800     		movs	r0, r3
 130 0052 FFF7FEFF 		bl	can1_init
  24:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	
  25:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	DUMP("CAN initated");
 131              		.loc 1 25 0
 132 0056 2D4B     		ldr	r3, .L11+8
 133 0058 1800     		movs	r0, r3
 134 005a FFF7FEFF 		bl	DUMP
  26:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  27:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	CANMsg msg;
  28:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.msgId = 1;
 135              		.loc 1 28 0
 136 005e 3B00     		movs	r3, r7
 137 0060 0122     		movs	r2, #1
 138 0062 1A70     		strb	r2, [r3]
  29:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.nodeId = 1;
 139              		.loc 1 29 0
 140 0064 3B00     		movs	r3, r7
 141 0066 0122     		movs	r2, #1
 142 0068 5A70     		strb	r2, [r3, #1]
  30:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.length = 6;
 143              		.loc 1 30 0
 144 006a 3B00     		movs	r3, r7
 145 006c 0622     		movs	r2, #6
 146 006e 9A70     		strb	r2, [r3, #2]
  31:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[0] = 'H';
 147              		.loc 1 31 0
 148 0070 3B00     		movs	r3, r7
 149 0072 4822     		movs	r2, #72
 150 0074 DA70     		strb	r2, [r3, #3]
  32:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[1] = 'e';
 151              		.loc 1 32 0
 152 0076 3B00     		movs	r3, r7
 153 0078 6522     		movs	r2, #101
 154 007a 1A71     		strb	r2, [r3, #4]
  33:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[2] = 'l';
 155              		.loc 1 33 0
 156 007c 3B00     		movs	r3, r7
 157 007e 6C22     		movs	r2, #108
 158 0080 5A71     		strb	r2, [r3, #5]
  34:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[3] = 'l';
 159              		.loc 1 34 0
 160 0082 3B00     		movs	r3, r7
 161 0084 6C22     		movs	r2, #108
 162 0086 9A71     		strb	r2, [r3, #6]
  35:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[4] = 'o';
 163              		.loc 1 35 0
 164 0088 3B00     		movs	r3, r7
 165 008a 6F22     		movs	r2, #111
 166 008c DA71     		strb	r2, [r3, #7]
  36:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     msg.buff[5] = 0;
 167              		.loc 1 36 0
 168 008e 3B00     		movs	r3, r7
 169 0090 0022     		movs	r2, #0
 170 0092 1A72     		strb	r2, [r3, #8]
  37:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     can_send(&msg);
 171              		.loc 1 37 0
 172 0094 3B00     		movs	r3, r7
 173 0096 1800     		movs	r0, r3
 174 0098 FFF7FEFF 		bl	can_send
  38:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     
  39:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 	DUMP("Hello message sent");
 175              		.loc 1 39 0
 176 009c 1C4B     		ldr	r3, .L11+12
 177 009e 1800     		movs	r0, r3
 178 00a0 FFF7FEFF 		bl	DUMP
 179              	.L10:
  40:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  41:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     while(1){
  42:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       usart_send("Enter a seven letter message\n");
 180              		.loc 1 42 0
 181 00a4 1B4B     		ldr	r3, .L11+16
 182 00a6 1800     		movs	r0, r3
 183 00a8 FFF7FEFF 		bl	usart_send
 184              	.LBB2:
  43:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       for(int i = 0; i < 7; i++)
 185              		.loc 1 43 0
 186 00ac 0023     		movs	r3, #0
 187 00ae FB60     		str	r3, [r7, #12]
 188 00b0 0CE0     		b	.L8
 189              	.L9:
  44:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****           msg.buff[i] = _getchar();
 190              		.loc 1 44 0 discriminator 3
 191 00b2 FFF7FEFF 		bl	_getchar
 192 00b6 0300     		movs	r3, r0
 193 00b8 1900     		movs	r1, r3
 194 00ba 3A00     		movs	r2, r7
 195 00bc FB68     		ldr	r3, [r7, #12]
 196 00be D318     		adds	r3, r2, r3
 197 00c0 0333     		adds	r3, r3, #3
 198 00c2 0A1C     		adds	r2, r1, #0
 199 00c4 1A70     		strb	r2, [r3]
  43:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       for(int i = 0; i < 7; i++)
 200              		.loc 1 43 0 discriminator 3
 201 00c6 FB68     		ldr	r3, [r7, #12]
 202 00c8 0133     		adds	r3, r3, #1
 203 00ca FB60     		str	r3, [r7, #12]
 204              	.L8:
  43:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       for(int i = 0; i < 7; i++)
 205              		.loc 1 43 0 is_stmt 0 discriminator 1
 206 00cc FB68     		ldr	r3, [r7, #12]
 207 00ce 062B     		cmp	r3, #6
 208 00d0 EFDD     		ble	.L9
 209              	.LBE2:
  45:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       msg.buff[7] = '\0';
 210              		.loc 1 45 0 is_stmt 1
 211 00d2 3B00     		movs	r3, r7
 212 00d4 0022     		movs	r2, #0
 213 00d6 9A72     		strb	r2, [r3, #10]
  46:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       msg.length=8;
 214              		.loc 1 46 0
 215 00d8 3B00     		movs	r3, r7
 216 00da 0822     		movs	r2, #8
 217 00dc 9A70     		strb	r2, [r3, #2]
  47:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       can_send(&msg);
 218              		.loc 1 47 0
 219 00de 3B00     		movs	r3, r7
 220 00e0 1800     		movs	r0, r3
 221 00e2 FFF7FEFF 		bl	can_send
  48:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       usart_send("Message sent: ");
 222              		.loc 1 48 0
 223 00e6 0C4B     		ldr	r3, .L11+20
 224 00e8 1800     		movs	r0, r3
 225 00ea FFF7FEFF 		bl	usart_send
  49:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       usart_send(msg.buff);
 226              		.loc 1 49 0
 227 00ee 3B00     		movs	r3, r7
 228 00f0 0333     		adds	r3, r3, #3
 229 00f2 1800     		movs	r0, r3
 230 00f4 FFF7FEFF 		bl	usart_send
  50:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       usart_send("\n");
 231              		.loc 1 50 0
 232 00f8 084B     		ldr	r3, .L11+24
 233 00fa 1800     		movs	r0, r3
 234 00fc FFF7FEFF 		bl	usart_send
  42:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       for(int i = 0; i < 7; i++)
 235              		.loc 1 42 0
 236 0100 D0E7     		b	.L10
 237              	.L12:
 238 0102 C046     		.align	2
 239              	.L11:
 240 0104 3C000000 		.word	.LC4
 241 0108 00000000 		.word	receiver
 242 010c 54000000 		.word	.LC7
 243 0110 64000000 		.word	.LC9
 244 0114 78000000 		.word	.LC11
 245 0118 98000000 		.word	.LC13
 246 011c A8000000 		.word	.LC15
 247              		.cfi_endproc
 248              	.LFE1:
 250              		.section	.start_section,"ax",%progbits
 251              		.align	1
 252              		.global	startup
 253              		.syntax unified
 254              		.code	16
 255              		.thumb_func
 256              		.fpu softvfp
 258              	startup:
 259              	.LFB2:
  51:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     }
  52:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  53:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     return;
  54:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** }
  55:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  56:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  57:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** // **** Boilerplate startup code ****
  58:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );
  59:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** 
  60:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c **** void startup( void ) {
 260              		.loc 1 60 0
 261              		.cfi_startproc
 262              		@ Naked Function: prologue and epilogue provided by programmer.
 263              		@ args = 0, pretend = 0, frame = 0
 264              		@ frame_needed = 1, uses_anonymous_args = 0
  61:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     __asm volatile(
 265              		.loc 1 61 0
 266              		.syntax divided
 267              	@ 61 "C:/Users/nazif/Documents/Ovning_2/Alarm_Controller/startup.c" 1
 268 0000 0248     		 LDR R0,=0x2001C000
 269 0002 8546     	 MOV SP,R0
 270 0004 FFF7FEFF 	 BL main
 271 0008 FEE7     	 B .
 272              	@ 0 "" 2
 273              		.thumb
 274              		.syntax unified
 275              	.L14:
  62:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       " LDR R0,=0x2001C000\n"		/* set stack */
  63:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       " MOV SP,R0\n" 
  64:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       " BL main\n" 	    /* call main */
  65:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       " B ."
  66:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****       );
  67:C:/Users/nazif/Documents/Ovning_2/Alarm_Controller\startup.c ****     while(1); // should not be needed
 276              		.loc 1 67 0 discriminator 1
 277 000a FEE7     		b	.L14
 278              		.cfi_endproc
 279              	.LFE2:
 281              		.text
 282              	.Letext0:
 283              		.file 2 "../can/inc/can.h"
