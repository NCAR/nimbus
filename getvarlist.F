#include "ftape.h"

C	Currently not supported, make the user use getfirst and
C	getnext and construct their own list.  --Chris

	function getvariablelist(vl)
	character*(*)	vl

	character	vn*8

	rc = GetNumberItems(n)

	if (rc .eq. ERR) then
		getvariablelist = ERR
	else
		vn = GetFirst()

		do 37 WHILE (VN .NE. '        ')
			vl(i) = vn
			vn = GetNext()
37		continue
	endif

	end
