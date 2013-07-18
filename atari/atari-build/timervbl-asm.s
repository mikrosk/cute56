		xdef	_asm_timervbl_init
		xdef	_asm_timervbl_deinit
		xref	_TimerVblCallback

; ------------------------------------------------------
		section	text
; ------------------------------------------------------

_asm_timervbl_init:
		move	sr,d0				; save sr
		ori	#$0700,sr			; ints off

		move.l	$70.w,save_vbl			; save vbl vector
		move.l	#vbl,$70.w			; own vbl

		move	d0,sr				; ints back
		rts

_asm_timervbl_deinit:
		move	sr,d0				; save sr
		ori	#$0700,sr			; ints off

		move.l	save_vbl,$70.w			; restore vbl vector

		move	d0,sr				; ints back
		rts

; ======================================================

vbl:		movem.l	d0-a6,-(sp)

		ifnd	NO_FPU
		fsave	-(sp)
		fmovem.l fpcr/fpsr/fpiar,-(sp)
		fmovem.x fp0-fp7,-(sp)
		endif

		jsr	_TimerVblCallback

		ifnd	NO_FPU
		fmovem.x (sp)+,fp0-fp7
		fmovem.l (sp)+,fpcr/fpsr/fpiar
		frestore (sp)+
		endif

		movem.l	(sp)+,d0-a6
		rte

; ------------------------------------------------------
		section	bss
; ------------------------------------------------------

		align	2

save_vbl:	ds.l	1
