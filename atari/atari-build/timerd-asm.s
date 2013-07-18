		xdef	_asm_timerd_init
		xdef	_asm_timerd_deinit
		xref	_TimerDCallback

; ------------------------------------------------------
		section	text
; ------------------------------------------------------

_asm_timerd_init:
		move	sr,d0				; save sr
		ori	#$0700,sr			; ints off

		lea	save_mfp,a0			; save mfp
		move.b	$fffffa09.w,(a0)+		;
		move.b	$fffffa15.w,(a0)+		;
		move.b	$fffffa1d.w,(a0)+		;
		move.b	$fffffa25.w,(a0)+		;

		bset	#4,$fffffa09.w			; timer-d
		bset	#4,$fffffa15.w			;
		or.b	#%101,$fffffa1d.w		; %101 = divide by 64
		move.b	#128,$fffffa25.w		; 2457600/64/128 = 300 Hz

		move.l	$110.w,save_timer		; save timer-d vector
		move.l	#timer,$110.w			; own timer-d

		move	d0,sr				; ints back
		rts

_asm_timerd_deinit:
		move	sr,d0				; save sr
		ori	#$0700,sr			; ints off

		lea	save_mfp,a0			; restore mfp
		move.b	(a0)+,$fffffa09.w		;
		move.b	(a0)+,$fffffa15.w		;
		move.b	(a0)+,$fffffa1d.w		;
		move.b	(a0)+,$fffffa25.w		;

		move.l	save_timer,$110.w		; restore timer-d vector

		move	d0,sr				; ints back
		rts

; ======================================================

timer:		movem.l	d0-a6,-(sp)

		ifnd	NO_FPU
		fsave	-(sp)
		fmovem.l fpcr/fpsr/fpiar,-(sp)
		fmovem.x fp0-fp7,-(sp)
		endif

		jsr	_TimerDCallback

		ifnd	NO_FPU
		fmovem.x (sp)+,fp0-fp7
		fmovem.l (sp)+,fpcr/fpsr/fpiar
		frestore (sp)+
		endif

		movem.l	(sp)+,d0-a6
		bclr	#4,$fffffa11.w			; clear busybit
		rte

; ------------------------------------------------------
		section	bss
; ------------------------------------------------------

		align	2

save_mfp:	ds.b	4

save_timer:	ds.l	1
