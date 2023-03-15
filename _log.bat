if (%1)==(OUT) goto out
:in
	settcc
	goto end
:out
	settcc -r
:end
