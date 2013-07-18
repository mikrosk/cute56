		xdef	_asm_screen_save
		xdef	_asm_screen_restore
		xdef	_asm_screen_setvram

; ------------------------------------------------------
		section	text
; ------------------------------------------------------

; extern void asm_screen_save( void );
;
_asm_screen_save:
		movem.l	d2-d7/a2,-(sp)

		bsr	wait_vbl			; avoid flicking

		lea	$ffff9800.w,a0			; save falcon palette
		lea	save_pal,a1			;
		moveq	#128-1,d7			;
							;
save_loop:	move.l	(a0)+,(a1)+			;
		move.l	(a0)+,(a1)+			;
		dbra	d7,save_loop			;

		movem.l	$ffff8240.w,d0-d7		; save st palette
		movem.l	d0-d7,(a1)			;

		lea	save_video,a0
		move.l	$ffff8200.w,(a0)+		; vidhm
		move.w	$ffff820c.w,(a0)+		; vidl

		move.l	$ffff8282.w,(a0)+		; h-regs
		move.l	$ffff8286.w,(a0)+		;
		move.l	$ffff828a.w,(a0)+		;

		move.l	$ffff82a2.w,(a0)+		; v-regs
		move.l	$ffff82a6.w,(a0)+		;
		move.l	$ffff82aa.w,(a0)+		;

		move.w	$ffff82c0.w,(a0)+		; vco
		move.w	$ffff82c2.w,(a0)+		; c_s

		move.l	$ffff820e.w,(a0)+		; offset+width
		move.w	$ffff820a.w,(a0)+		; sync

		move.b  $ffff8265.w,(a0)+		; p_o

		cmpi.w   #$b0,$ffff8282.w		; st(e) / falcon test
		sle	(a0)+				; it's a falcon resolution

		move.w	$ffff8266.w,(a0)+		; f_s
		move.w	$ffff8260.w,(a0)+		; st_s

		movem.l	(sp)+,d2-d7/a2
		rts

; extern void asm_screen_restore( void );
;
_asm_screen_restore:
		movem.l	d2-d7/a2,-(sp)

		bsr	wait_vbl			; avoid flicking

		lea	save_video,a0

		move.l	(a0)+,$ffff8200.w		; videobase_address:h&m
		move.w	(a0)+,$ffff820c.w		; l

		move.l	(a0)+,$ffff8282.w		; h-regs
		move.l	(a0)+,$ffff8286.w		;
		move.l	(a0)+,$ffff828a.w		;

		move.l	(a0)+,$ffff82a2.w		; v-regs
		move.l	(a0)+,$ffff82a6.w		;
		move.l	(a0)+,$ffff82aa.w		;

		move.w	(a0)+,$ffff82c0.w		; vco
		move.w	(a0)+,$ffff82c2.w		; c_s

		move.l	(a0)+,$ffff820e.w		; offset+width
		move.w	(a0)+,$ffff820a.w		; sync

		move.b  (a0)+,$ffff8265.w		; p_o

		tst.b   (a0)+   			; st(e) compatible mode?
		bne.b   st_comp				; yes

falcon:		move.l	a0,-(sp)
		bsr	wait_vbl			; Patch to avoid
		clr.w   $ffff8266.w			; monochrome sync errors
		bsr	wait_vbl			; (ripped from
		movea.l	(sp)+,a0			; FreeMiNT kernel,
		move.w	(a0),$ffff8266.w		; by Draco/Yescrew)

		bra.b	restored

st_comp:	move.w	(a0)+,$ffff8266.w		; falcon-shift
		move.w  (a0),$ffff8260.w		; st-shift
		lea	save_video,a0
		move.w	32(a0),$ffff82c2.w		; c_s
		move.l	34(a0),$ffff820e.w		; offset+width

restored:	lea	save_pal,a0			; restore falcon palette
		lea	$ffff9800.w,a1			;
		moveq	#128-1,d7			;
							;
restore_loop:	move.l	(a0)+,(a1)+			;
		move.l	(a0)+,(a1)+			;
		dbra	d7,restore_loop			;

		movem.l	(a0),d0-d7			; restore st palette
		movem.l	d0-d7,$ffff8240.w		;

		movem.l	(sp)+,d2-d7/a2
		rts

; extern void asm_screen_setvram( const void* pPixels );
;
_asm_screen_setvram:
		move.l	4(sp),d0

		cmp.l	curr_vram,d0
		beq.b	.skip

		move.l	d0,curr_vram

		move.l	d0,d1				; set screen
		lsr.w	#8,d0				;
		move.l	d0,$ffff8200.w			;
		move.b	d1,$ffff820d.w			;
.skip:		rts

; ======================================================

wait_vbl:	move.w	#$25,-(sp)			; Vsync()
		trap	#14				;
		addq.l	#2,sp				;
		rts

; ------------------------------------------------------
		section	bss
; ------------------------------------------------------

		align	2

curr_vram:	ds.l	1				; current vram

save_pal:	ds.l	256+16/2			; old colours (falcon+st/e)

save_video:	ds.b	32+12+2				; videl save
