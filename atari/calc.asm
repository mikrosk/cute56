rhost		MACRO	dst
		jclr	#0,X:<<$ffe9,*
		movep	X:<<$ffeb,dst
		ENDM

whost		MACRO	src
		jclr	#1,X:<<$ffe9,*
		movep	src,X:<<$ffeb
		ENDM

		ORG	P:$0000
		jmp	>start

		ORG	P:$0040
start:		rhost	x0

		tfr	x0,a	#>1,x0
		add	x0,a

		whost	a
		jmp	<start
