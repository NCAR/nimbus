FUNCTION sid_bin2dec64,inp
   ;FUNCTION to convert decimal to binary.
   ;Also works for 2d arrays, individual numbers are in rows.
   ;Code copied from http://orpheus.nascom.nasa.gov/cds/#SOFTWARE
   ;Originally by Pike, modified 10-31-2000 by Aaron Bansemer for 2d image use
   ;Modified October 2001 (A.B.) for 64-bit processing for use with DMT probes.
   ;Using unsigned long64 int for 2d processing
   
   ;  initialise output and find size of input
   s = size(inp,/dim)
   if n_elements(s) eq 1 then n=1 else n=s[1]
   if s[0] gt 64 then print, 'possible overflow in bin2dec'
   out = ulon64arr(n)   

   ;  switch array around for convenience
   x = reverse(byte(inp),1)

   ;  calculate integer
   for i=0,s[0]-1 do out = out + ulong64(x(i,*))*2ULL^i

   return,out
END