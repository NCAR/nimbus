      PROGRAM setDMTchannels
c	This program sets the size thresholds for any particle probes that use the DMT
c	SPP electronics (SPP-100, SPP-200 or SPP-300), and also does the thresholds for
c	the Forward and Back scatter of the CAPS probe, all for any index of refraction 
c	that is selected. The only required information is the desired refractive index 
c	and the size in the maximum
c	gain stage channel. The gain ratio is assumed between gain stages. 
c	in which the calibration particle was sized. In addition, one must select how many
c	channels (10,20,30 or 40), and for multiple gain stage SPPs (and MASP) the number 
c	of channels in each gain stage are also requested.
	real psize(2000),scatter(2,2000)
	real szmin(10,5),szmax(10,5),nchmx(3),chnlsiz(40)
	real gainR(2,5),maxsiz(3),mxsizes(3,5),fspmx(4),prbref(5)
	real refreal(2)
	integer ngain(5),nchnl(3),chnlcnt(40),counts,ioff(3)
	character*1 ans
	character*50 filout,chnlfil
	common szmn(10),szmx(10),c0(10),c1(10),ncoeff,adcCounts
	common /view/chnlsiz,chnlcnt,ngain,nchnl,ioff
c	Average Beam diameter for FSSP's
	data bdiam/0.18/
c	Response Time
	data taur/5e-7/
	data mxsizes/
     $0.14,0.3,3,
     $0.7,20,20,
     $47.,47.,47.,
     $0.7,20.,50.,
     $0.7,20.,50./
	data fspmx/47.,32.,15.,7.5/
	data prbref/1.58,1.58,1.33,1.58,1.58/
	data szmin/
     $0.1,.3,.5,.8,1.0,1.5,2.0,2.5,3.0,5.0,
     $0.1,0.7,1.0,3.0,6.0,9.0,12.0,16.,20.0,30.,
     $0.1,0.7,1.0,3.0,6.0,9.0,12.0,16.,20.0,30.,
     $0.1,0.7,1.0,3.0,6.0,9.0,12.0,16.,20.0,30.,
     $0.1,0.7,1.0,3.0,6.0,9.0,12.0,16.,20.0,30./
	data szmax/
     $.3,.5,.8,1.0,1.5,2.0,2.5,3.0,5.0,20.0,
     $0.7,1.0,3.0,6.0,9.0,12.0,16.0,20.0,30.0,100.0,
     $0.7,1.0,3.0,6.0,9.0,12.0,16.0,20.0,30.0,100.0,
     $0.7,1.0,3.0,6.0,9.0,12.0,16.0,20.0,30.0,100.0,
     $0.7,1.0,3.0,6.0,9.0,12.0,16.0,20.0,30.0,100.0/
	data ncoeff/10/,ngain/3,2,1,3,3/
	data gainR/57.8,88.5,233.,1.0,1.0,1.0,22.,23.,22.,23./
	nprb=0
	do while (nprb.lt.1 .or. nprb.gt. 5)
		write(6,7000)
7000		format('Enter probe type'/'1=pcasp (spp-200)'/
     $	'2=fssp-300 (spp-300)'/'3=fssp-100 (spp-100)'/'4=CAPS Forward'/
     $	'5=CAPS Backward')
		read(5,*)nprb
	end do
	refreal(1)=prbref(nprb)
	if (nprb.eq.3)then
		write(6,6999)
6999		format('Range (1,2,3,4) ',$)
		read(5,*)irng
		mxsizes(1,3)=fspmx(irng)
	end if
	ntot=0
	do while (ntot.ne.10 .and. ntot.ne.20 .and. ntot.ne.30 .and. 
     $	ntot.ne.40)
		write(6,7001)
7001		format('Enter total number of channels (10,20,30, or 40) ',$)
		read(5,*)ntot
	end do
	if (nprb.lt.4)then
		adcCounts = 4096
	else
		adcCounts = 1024
	end if
	nsum=ntot
	if (ngain(nprb).gt.1)then
		do 20 i=1,ngain(nprb)
			if (i .lt. ngain(nprb))then
				write(6,7006)i,nsum
7006				format('Enter the number of channels in gain stage ',i1,
     $			'( <= ',i2,')')
				read(5,*)nchnl(i)
				nsum = nsum - nchnl(i)
			else
				nchnl(i)=nsum
			end if
			maxsiz(i) = mxsizes(i,nprb)
20		continue
	else
		nchnl(1)=ntot
		maxsiz(1) = mxsizes(1,nprb)
	end if
	ioff(1)=0
	nchmx(1)=nchnl(1)
	if (ngain(nprb).gt.1)then
		do i=2,ngain(nprb)
			ioff(i) = ioff(i-1)+nchnl(i-1)
			nchmx(i)=nchmx(i-1)+nchnl(i)
		enddo
	end if
	write(6,7003)
7003	format('Enter the refractive index of particles ',$)
	read(5,*)refreal(2)
	refimg=0.0
c	Compute the scattering efficiencies
	print *,'Calculating scattering cross sections - Please wait'
	if (nprb .eq. 1)then
		call pspsct(psize,scatter,refreal,refimg,nmie,ngain(nprb))
	else
		call fspsct(psize,scatter,refreal,refimg,nmie,nprb,ngain(nprb))
	end if
c	Search through the scattering arrays to find the equivalent maximum sizes
	if (refreal(2).ne.refreal(1))then
		do i=1,ngain(nprb)
			sz=maxsiz(i)
			j=1
			do while (psize(j).lt. sz)
				sct=scatter(1,j)
				j=j+1
			end do
			j=1
			sizemn=0.0
			sizemx=0.0
			do while (j.lt. nmie)
				if (scatter(2,j) .gt. sct*.95 .and. 
     $				scatter(2,j) .lt. sct*1.05)then
					if (sizemn.eq.0)then
						sizemn=psize(j)
					else
						sizemx=psize(j)
					end if
				end if
				j=j+1
			end do
			if (sizemx.eq.0)sizemx=sizemn
			maxsiz(i)=sqrt(sizemn*sizemx)
		enddo
	else
		do i=1,nmie
			scatter(2,i)=scatter(1,i)
		enddo
	end if
	do 10 i=1,10
		szmn(i) = szmin(i,nprb)
		szmx(i) = szmax(i,nprb)
10	continue
c	Now calculate the best fit coefficients for the desired refractive index
	do 50 ic = 1,ncoeff
		if (ic .eq. 1)then
			xf = psize(1)
			yf = scatter(2,1)
		else
			xf = szmn(ic)
			yf = c0(ic-1)*xf**c1(ic-1)
		end if
		npts = 0
		sx= 0.0
		sy = 0.0
		sqx = 0.0
		sqy = 0.0
		sxy = 0.0
		do 25 i=1,nmie
			if (psize(i) .lt. (szmn(ic)*.9))goto 25
			if (psize(i) .gt. (szmx(ic)*1.1))goto 30
			npts = npts + 1
			x=alog(psize(i)/xf)
			sx = sx + x
			sqx = sqx + x*x
			y=alog(scatter(2,i)/yf)
			sy = sy + y
			sqy = sqy + y*y
			sxy = sxy + x*y
25		continue
30		xmn = sx/npts
		ymn = sy/npts
		c1(ic) = sxy/sqx
		c0(ic) = 
     $	exp(alog(yf) - c1(ic)*alog(xf))
50	continue
c	This section is in just to do a check on the integrity of the spline fit
c	write(55,9500)
9500	format('size calmie calest obsmie obsest')
c	do 130 i=1,nmie
c		j=1 
c		do while ((psize(i).lt.szmn(j).or.psize(i).gt.szmx(j)).and.
c    $				j.lt.10)
c			j=j+1
c		end do
c		compute the estimated intensities
c		scat = c0(j)*psize(i)**c1(j)
c		write(55,9501)psize(i),scatter(i),scat
9501		format(f7.3,2e12.4)
130	continue
c	Now that we have this information, we have enough to compute the size thresholds
c	Use the maximum size to get the scattering intensity calibration
	call setsizes(maxsiz,nprb)

c	Now display these settings to see if changes are desired
	call display(nprb)
c	Go into edit mode here.
	ichnl=0
	do while (ichnl.ge.0)
		write(6,9010)
9010		format('Enter channel # and new size (nn,size-0,0=end) ',$)
		ichnl=0
		do while (ichnl.lt.1 .or. ichnl.gt. ntot)
			read(5,*)ichnl,siznew
			if (ichnl.lt.1)goto 66
		end do
c	Check if the maximum channel size has been changed in a stage.
		do i=1,ngain(nprb)
			if (ichnl .eq. nchmx(i))then
c	If it was, make sure this is what is wanted
				write(6,8300)i
8300				format('This will redefine all sizes in gain stage', i2,
     $			' Is this what you want? (y,n)',$)
				read(5,8301)ans
8301				format(a1)
				if (ans .eq. 'y')then
					maxsiz(i)=siznew
					call setsizes(maxsiz,nprb)
				end if
			end if
		enddo
		call gain(ichnl,igain,nprb)
		chnlsiz(ichnl) = siznew
		chnlcnt(ichnl) = size2Counts(siznew,maxsiz(igain))+
     $	(igain-1)*adcCounts
		call display(nprb)
	end do
c	Now write these thresholds out to a file that can be read by the same program.
66	write(6,9050)
9050	format('Enter a name for the new table file ',$)
	read(5,7110)chnlfil
7110	format(a50)
	open(50,file=chnlfil)
	write(50,9051)
9051	format("Channel   Size  Counts")
	do 600 i=1,ntot
		write(50,9060)i,chnlsiz(i),chnlcnt(i)
9060		format(i7,f7.3,i8)
600	continue
	end
	subroutine setsizes(maxsiz,nprb)
	common /view/chnlsiz,chnlcnt,ngain,nchnl,ioff
	common szmn(10),szmx(10),c0(10),c1(10),ncoeff,adcCounts
	real chnlsiz(40),maxsiz(3)
	integer ngain(5),nchnl(3),chnlcnt(40),ioff(3)
	do 200 igain = ngain(nprb),1,-1
		cnts = adcCounts
		diamax = maxsiz(igain)
		cnts = 5
		diamin = counts2Size(cnts,maxsiz(igain))
		if (diamin .lt. szmn(1))diamin=szmn(1)
		deldia = (diamax-diamin)/nchnl(igain)
		do 175 i=1,nchnl(igain)
			chnlsiz(i+ioff(igain)) = deldia*i + diamin
175		continue
200	continue
c	We are now ready to go through all the channels and find the counts
	do 400 igain = 1,ngain(nprb)
		cntoff = (igain-1)*adcCounts
		do 300 i=1,nchnl(igain)
			sz = chnlsiz(i+ioff(igain))
			chnlcnt(i+ioff(igain)) = 
     $			size2Counts(sz,maxsiz(igain))+cntoff
300		continue
400	continue
	return
	end
	subroutine gain(ichnl,igain,nprb)
	common /view/chnlsiz,chnlcnt,ngain,nchnl,ioff
	real chnlsiz(40)
	integer ngain(5),nchnl(3),chnlcnt(40),ioff(3)
	nlow = 0
	do 50 i=1,ngain(nprb)
		if (ichnl.ge.nlow .and. ichnl.le.(nchnl(i)+nlow))igain=i
		nlow = nlow+nchnl(i)
50	continue
	return
	end
	subroutine display(nprb)
	common /view/chnlsiz,chnlcnt,ngain,nchnl,ioff
	real chnlsiz(40)
	integer ngain(5),nchnl(3),chnlcnt(40),ioff(3)
	write(6,9000)
9000	format('CH   size  cnts CH   size  cnts CH   size  cnts',
     $' CH   size  cnts CH   size  cnts')
	jchnl=0
	ifrst=0
	do 500 igain = 1,ngain(nprb)
		do 450 i=1,nchnl(igain)
			jcol = jcol+1
			jchnl=jchnl+1
			k = i+ioff(igain)
			if (ifrst.eq.0)then
				ifrst=1
				if (nprb.eq.1)then
					write(6,9001)jchnl,chnlsiz(k),int(chnlcnt(k))
9001					format(i2,f7.3,i6,$)
				else
					write(6,9004)jchnl,chnlsiz(k),int(chnlcnt(k))
9004					format(i2,f7.2,i6,$)
				end if
			else
				if (nprb.eq.1)then
					write(6,9003)jchnl,chnlsiz(k),int(chnlcnt(k))
9003					format(i3,f7.3,i6,$)
				else
					write(6,9005)jchnl,chnlsiz(k),int(chnlcnt(k))
9005					format(i3,f7.2,i6,$)
				end if
			end if
			if (mod(jchnl,1).eq.0)then
				write(6,9002)
				ifrst=0
			end if
9002			format()
450		continue
500	continue
	return
	end

c____________________________________________________________________________
c <<<<<<<<<<<<<<<<<<<<<<<<<<<< counts2Size >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
c counts2Size    -	This function returns the particle size corresponding
c						to "counts" and the maximum size in this gain stage

	function counts2Size(counts,szmax)
	double precision	scaleFactor, intensity, xsctMin,xsctMax;
	common szmn(10),szmx(10),c0(10),c1(10),ncoeff,adcCounts
c	Compute the scale factor. This requires finding the scatter intensity
c	associated with the maximum size sent to this function
	i = 0
	j = 1
	do while (i .lt. ncoeff)
		i=i+1
		if (szmax.ge.szmn(i).and.szmax.le.szmx(i))then
		   j = i
		end if
	end do
	if (j.eq.0)j=ncoeff
	scaleFactor = adcCounts/
     $				(c0(j) * szmax**c1(j))

c	Convert requested counts to scattering intensity							
	intensity = counts/scaleFactor

c	Find the coefficient range for the requested size
	i = 0
	j = 0
	do while (i .lt. ncoeff)
		i=i+1
c	Convert the size limits to intensities
		xsctMin = 
     $		c0(i) * szmn(i)** c1(i)
		xsctMax = 
     $		c0(i) * szmx(i)** c1(i)

		if (intensity .gt. xsctMin .and. intensity .le. xsctMax)then
		   j = i
		end if
	end do
	if (j.eq.0)j=ncoeff

c	 Now that we have the size range, invert the power equation to get size		*/
	counts2Size =
     $		(intensity/c0(j))**(1.0/c1(j))
	return
	end
c<<<<<<<<<<<<<<<<<<<<<<<<<<<< End of counts2Size >>>>>>>>>>>>>>>>>>>>>>>>>*/
c____________________________________________________________________________
c <<<<<<<<<<<<<<<<<<<<<<<<<<<< size2Counts >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
c size2Counts    - 	This function returns the number of A/D counts that
c    					corresponds to a specific size			
c	Return Value:	The number of counts (0-adcCounts) corresponds to given size
c						= -1 if an error finding the value. This can occur if 
c						maximum size is > 50, or < .3. Will also happen if index	
c						is <1.31 or > 1.58												
c____________________________________________________________________________
	function size2Counts(size,szmax)
	common szmn(10),szmx(10),c0(10),c1(10),ncoeff,adcCounts
	double precision scaleFactor, intensity
c	 Compute the scale factor. This requires finding the scatter intensity
c	associated with the maximum size sent to this function
	i = 0
	j = 0
	do while (i .lt. ncoeff )
		i=i+1
		if (szmax.ge.szmn(i).and. szmax.le.szmx(i))then
		   j = i
		end if
	end do
	if (j.eq.0)j=ncoeff

	scaleFactor = adcCounts/
     $				(c0(j) * szmax**c1(j))
c	Find the coefficient range for the requested size
	i = 0
	j = 0
	do while (i .lt. ncoeff )
	i=i+1
		if (size .ge. szmn(i) .and. size .le. szmx(i))then
		   j = i
		end if
	end do
	if (j.eq.0)j=ncoeff
c	Convert the size to an intensity
	intensity = c0(j) *size**c1(j)

c	Return this value converted to counts
	size2Counts = intensity * scaleFactor
	return
	end
c<<<<<<<<<<<<<<<<<<<<<<<<<<<< End of size2Counts >>>>>>>>>>>>>>>>>>>>>>>>>*/

c	This subroutine calculates the scattering efficiency for FSSPs with forward
c	scattering angle of 4-12 degrees, or CAPS with forward or backward scattering
c	collection angles of 4.5-13.2
	subroutine fspsct(psize,scatter,refreal,refimg,npnts,nprb,ngain)
      complex cm,ci,cim,f(600),g(600),t1,t2
      common /cfcom/ f,g,cnrm
      dimension pnmllg(600),cnrm(600)
	real psize(2000),scatter(2,2000),refreal(2)
      pi = 3.14159265358979
	do irf=1,2
	if (irf.eq.1 .or.(irf.eq.2 .and. refreal(2).ne.refreal(1)))then
      dtr = pi/180.0
      ci = (0.0,1.0)
      ip = 1      
      dlt = 0.25
	wvlngth = 0.6328
      nang = int(8.0/dlt)+1
      thsrt = 4.0*dtr
      dlt = 8.0*dtr/real(nang-1)
	if (nprb .eq. 4)then
		wvlngth = 0.685
         dlt = 0.25
		nang = int(8.7/dlt)+1
		thsrt = 4.5*dtr
		dlt = 8.7*dtr/real(nang-1)
	end if
	if (nprb .eq. 5)then
		wvlngth = 0.685
         dlt = 0.25
		nang = int(8.7/dlt)+1
		thsrt = 166.8*dtr
		dlt = 8.7*dtr/real(nang-1)
	end if
      cmr = refreal(irf)
	cmi = refimg
      cm = cmplx(cmr,cmi)
      x = 0.09
      do 80 ix = 1,2000
		if (x.ge. 1.0)x = x + .05
		if (x.lt. 1.0)x = x + .002
		if (x.gt.100)goto 90
		psize(ix) = x
		xc = pi*x/0.6328
		call sphere(xc,cm,ip,nc)
		nci = nc+1
		sum = 0.0
		do 70 i = 1,nang
			theta = thsrt+real(i-1)*dlt
			sinth = sin(theta)
			costh = cos(theta)
			call genlgp(theta,pnmllg,nci)
			t1 = 0.0
			t2 = 0.0
			do 60 n = 1,nc
      			n1 = n+1
				cim = ci**(-n1)
				rn = real(n)
				p1 = rn*costh*pnmllg(n1)-(rn+1.0)*pnmllg(n)
				p2 = pnmllg(n1)
				t1 = t1+cim*(p2*f(n)+ci*p1*g(n))*cnrm(n)
				t2 = t2+cim*(p1*f(n)+ci*p2*g(n))*cnrm(n)
60			continue
			tp = sinth*(abs(t1)**2+abs(t2)**2)/2.0
			if(i.eq.1.or.i.eq.nang) tp = tp/2.0
			sum = sum+tp
70		continue
		sum = dlt*sum*10000.*((0.6328e-06)**2)/(2.0*pi)
         scatter(irf,ix) = sum
		if (mod(ix,100).eq. 0)print *,irf,psize(ix),sum
80    continue 
90    npnts = ix-1
	end if
	end do
	return
      end
      subroutine sphere(x,cm,ip,nc)
c     ..............................................................
c     .  calculate the scattered field f(n) and g(n) coefficients  .
c     .    the f(n) and g(n) for theta incident polarization are,  .
c     .    within an n-dependent factor, the same as the b(n) and  .
c     .    a(n) coefficients, respectively, defined in C.F.        .
c     .    Bohren and D.R. Huffman, Absorption and Scattering of   .
c     .    Light by Small Particles (Wiley- Interscience,New       .
c     .    York,1983), p.100                                       .
c     ..............................................................
      complex b,z,cm,ci,hkl(600),an,amat(600),f(600),g(600)
      common /cfcom/ f,g,cnrm
      dimension cnrm(600)
      ci = (0.0,1.0)
c     ......................................................
c     .  set the number of terms required for convergence  .
c     ......................................................
      xc = x+4.05*x**.3333+2.0                                          eq 4.16
      nc = int(xc)
      nci = nc+1
      z = cm*x
c     ..................................................
c     .  logarithmic derivative calculation - set the  .
c     .    starting order for downward recursion       .
c     ..................................................
      nmx = int(max(xc,abs(z)))+15                                      eq 4.20
      an = 0.0
      do 10 n = nmx,nc+1,-1
        rn = real(n)
        an = rn/z-1.0/(an+rn/z)
10    continue
      amat(nc) = an
      do 20 n = nc,2,-1
        rn = real(n)
        amat(n-1) = rn/z-1.0/(amat(n)+rn/z)                             eq 4.19
20    continue
c     ...................................................
c     .  calculate the Bessel functions - the order is  .
c     .    incremented by one in the hkl(*) array       .
c     ...................................................
      call besh(x,hkl,nci)
      bj = real(hkl(1))
c     ................................
c     .  calculate the coefficients  .
c     ................................
      do 30 n = 1,nc
        rn = real(n)
        rf = 2.0*rn*(rn+1.0)
        bjm = bj
        bj = real(hkl(n+1))
c     .......................................................
c     .  scattering coefficients for theta                  .
c     .    (parallel) incident polarization                 .
c     .    f(n) = -ci**n*rf*(Bohren and Huffman's b(n))     .
c     .    g(n) = ci**(n+1)*rf*(Bohren and Huffman's a(n))  .
c     .......................................................
        b = cm*amat(n)+rn/x
        f(n) = -ci**n*rf*(b*bj-bjm)/(b*hkl(n+1)-hkl(n))                 eq 4.18a
        b = amat(n)/cm+rn/x
        g(n) = ci**(n+1)*rf*(b*bj-bjm)/(b*hkl(n+1)-hkl(n))              eq 4.18b
        if(ip.eq.2) then
c     ...............................................
c     .  scattering coefficients for phi            .
c     .    (perpendicular) incident polarization    .
c     ...............................................
          f(n) = -f(n)                                                  eq 4.8a
          g(n) = g(n)                                                   eq 4.8b
        end if
c     ........................................
c     .  calculate the normalization factor  .
c     .    (used in main program)            .
c     ........................................
        cnrm(n) = (2.0*rn+1.0)/(rf*rn*(rn+1.0))
30    continue
      return
      end
      subroutine besh(x,hankel,nc)
c     ...................................................
c     .  calculate Hankel functions                     .
c     .  bj = Bessel function of the first kind         .
c     .  by = Bessel function of the second kind        .
c     .  x = real argument                              . 
c     .  nc = number of orders (0 to nc-1)              .
c     .  the order of the functions is incremented by   .
c     .    one in the bj(*),by(*) and hankel(*) arrays  .
c     .                                                 .
c     .  arrays are set for nc = 226 maximum            .
c     ...................................................
      complex hankel(nc)
      dimension bj(600),by(600),t(3)
c     ................................................
c     .  by(*) calculation - obtain the zeroeth and  .
c     .                      first order functions   .
c     ................................................
      a = sin(x)/x                                                      eq 4.68
      by(1) = -cos(x)/x                                                 eq 4.69a
      by(2) = by(1)/x-a                                                 eq 4.69b
c     ...........................................................
c     .  obtain the higher order functions by upward recursion  .
c     ...........................................................
        do 10 n = 3,nc
        rn = real(n-2)
        by(n) = (2.0*rn+1.0)*by(n-1)/x-by(n-2)
10      continue
c     ................................................
c     .  bj(*) calculation - set the starting order  .
c     .                      for downward recursion  .
c     ................................................
      nst = nc+int((101.0+x)**.5)                                       eq 4.21
c     ....................................................
c     .  the t(*) array is used to recur down to the     .
c     .    two highest order functions that are needed   .
c     .  set starting values for the two highest orders  .
c     .    nst and nst-1                                 .
c     ....................................................
      t(3) = 0.0
      t(2) = 1.0e-35
c     ...................................................
c     .  recur downward to obtain orders nc-1 and nc-2  .
c     ...................................................
        do 20 i = nst-1,nc-1,-1
        ri = real(i)
        t(1) = (2.0*ri+1.0)*t(2)/x-t(3)
        t(3) = t(2)
        t(2) = t(1)
20      continue
c     ...............................................
c     .  continue downward recursion to order zero  .
c     ...............................................
      bj(nc) = t(3)
      bj(nc-1) = t(2)
        do 30 i = nc-2,1,-1
        ri = real(i)
        bj(i) = (2.0*ri+1.0)*bj(i+1)/x-bj(i+2)
30      continue                
c     ..................................................
c     .  calculate the scale factor and the functions  .
c     ..................................................
      alpha = a/bj(1)
        do 40 k = 1,nc
        hankel(k) = cmplx(bj(k)*alpha,by(k))
40      continue
      return
      end
      subroutine genlgp(theta,pnmllg,nc)
c     ........................................................
c     .  calculate associated Legendre functions (argument   .
c     .    cos(theta)) divided by sin(theta) for m = 1       .
c     .  generate first two orders by formula and remaining  .
c     .    orders by recursion                               .
c     .                                                      .
c     .  pnmllg = associated Legendre function/sin(theta)    .
c     .  nc = number of orders (0 to nc-1)                   .
c     .  the order of the associated Legendre functions is   .
c     .    incremented by one in the pnmllg(*) array         .
c     ........................................................
      dimension pnmllg(nc)
      costh = cos(theta)
c     ..............................
c     .  calculate orders 0 and 1  .
c     ..............................
      pnmllg(1) = 0.0                                                   eq 4.70a
      pnmllg(2) = 1.0                                                   eq 4.70b
c     .................................................
c     .  recur upward to obtain all remaining orders  .
c     .................................................
      do 10 n = 3,nc 
      rn = real(n-1)
      pnmllg(n) = ((2.0*rn-1.0)*costh*pnmllg(n-1)
     1             -rn*pnmllg(n-2))/(rn-1.0)                            eq 4.71
10    continue 
      return 
      end 
c	This subroutine calculates the scattering efficiency for PCASPs with
c	scattering angle of 35 - 135 degrees
	subroutine pspsct(psize,scatter,refreal,refimg,npnts,ngain)
      integer N
      real rad,R,elem1(0:180),elem2(0:180),elem3(0:180),
     1	    tot,pii,lambda 
	real scatter(2,1000),psize(1000),refreal(2)
      complex refrel,s1(2000),s2(2000),S1t(0:180),S2t(0:180)
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
	do irf=1,2
	if (irf.eq.1 .or.(irf.eq.2 .and. refreal(2).ne.refreal(1)))then
	refim = refimg
      refre = refreal(irf)
	diam = 0.09
	Do 1200 k=1,1000
	  if (diam .ge. 1)diam = diam + .02
		if (diam .lt. 1)diam = diam + .002
		rad = diam/2.0
	   refmed = 1.0
c  ***************************************************************
c    refractive index of sphere = refre + i * refim
c  ***************************************************************
	   refrel = cmplx( refre, refim ) / refmed
c  ***************************************************************
c    radius (rad) and wavelength (wavelength (wavel) same units.
c  ***************************************************************
	   wavel = .6328
	   x = 2. * 3.14159265 * rad * refmed / wavel
c  ***************************************************************
c    nang = number of angles between 0 and 90 degrees.
c    Matrix elements calculated at 2 * nang - 1 angles
c    including 0, 90, 180 degrees.
c  ***************************************************************
	   nang = 91
	   dang = 1.570796327 / float(nang - 1)
         call bhmie( x, refrel, nang, s1, s2, qext, qsca, qback )
c  ***************************************************************
c    s33 and s34 matrix elements normalized by s11.
c    s11 is normalized to 1.0 in the forward direction.
c    pol = degree of polarization (incident unpolarized light).
c  ***************************************************************
         s11nor = 0.5 * ( cabs( s2(1) ) ** 2 + cabs( s1(1) ) ** 2 )
         nan = 2 * nang - 1
         do 355 j = 1, nan
           aj = j
           s11 = 0.5 * cabs( s2(j) ) * cabs( s2(j) )
           s11 = s11 + 0.5 * cabs( s1(j) ) * cabs( s1(j) )
           s12 = 0.5 * cabs( s2(j) ) * cabs( s2(j) )
           s12 = s12 - 0.5 * cabs( s1(j) ) * cabs( s1(j) )
           pol = -s12 / s11
           s33 = real( s2(j) * conjg( s1(j) ) )
           s33 = s33 / s11
           s34 = aimag( s2(j) * conjg( s1(j) ) )
           s34 = s34 / s11
           s11 = s11 / s11nor
           ang = dang * (aj - 1) * 57.2958
 355     continue
cccccccccccccc PROGRAM TRAPccccccccccccccccccccccc
c test program to calc R
cccccccccccccccccccccccccccccccccccccccccccccccccc
         lambda=wavel*1.0e-5
         pii=3.14159
cccccccccccc  Section to rename the S values corresponding
c             to their theta value
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
	   do 20 i=0,180
            S1t(i)=s1(i+1)
            S2t(i)=s2(i+1)
20	   continue
cccccccccccccccccccccccccccccccccccccccccccccc
c      CALCULATE THE ELEMENTS IN THE INTEGRAL (from Garvey and Pinnick,
c	1983)
ccccccccccccccccccccccccccccccccccccccccccccc
      do 40 i=0,180
       elem1(i)=(cabs(S1t(i)+S1t(180-i)))**2.
       elem2(i)=(cabs(S2t(i)+S2t(180-i)))**2.
       elem3(i)=elem1(i)+elem2(i)
 40   continue
cccccccccccccccccccccccc
c  CALL SUBROUTINE TRAPR
cccccccccccccccccccccccc
      call trapr(elem3,tot,N,pii)
       R=((lambda**2.)/(4.*pii))*tot*(pii/180)
	psize(k) = diam
	scatter(irf,k)=R
	if (mod(k,100).eq. 0)print *,nrfrc,psize(k),R
	if (psize(k) .ge. 20.0)goto 1205
1200	continue
1205	npnts = k-1
	end if
	enddo
      return
      end
C
c  ***************************************************************
c    Subroutine BHMIE calculates amplitude scattering matrix
c    elements and efficiencies for extiction, total scattering
c    and backscattering for a given size parameter and
c    relative refractive index.
c  ***************************************************************
        subroutine bhmie ( x, refrel, nang, s1, s2, qext, qsca, qback )
        dimension amu(2000), theta(2000), pi(2000), tau(2000),
     *            pi0(2000), pi1(2000)
        complex d(3000), y, refrel, xi, xi0, xi1, an, bn, s1(2000),
     * s2(2000)
        double precision psi0, psi1, psi, dn, dx
        dx = x
        y = x * refrel
c  ***************************************************************
c    Series terminated after nstop terms.
c  ***************************************************************
        xstop = x + 4. * x ** .3333 + 2.0
        nstop = xstop
        ymod = cabs(y)
        nmx = amax1( xstop, ymod ) + 15
        dang = 1.57096327 / float( nang - 1 )
        do 555 j = 1, nang
          theta(j) = ( float(j) - 1. ) * dang
555     amu(j) = cos( theta(j) )
c  ***************************************************************
c    Logarithmic derivative d(j) calculated by downward
c    recurrence beginning with initial value 0.0 + i * 0.0
c    at j = nmx.
c  ***************************************************************
        d(nmx) = cmplx( 0.0, 0.0 )
        nn = nmx - 1
        do 120 n = 1, nn
          rn = nmx - n + 1
120     d(nmx - n) = (rn / y) - ( 1. / ( d(nmx - n + 1) + rn / y ) )
        do 666 j = 1, nang
          pi0(j) = 0.0
666     pi1(j) = 1.0
        nn = 2 * nang - 1
        do 777 j = 1, nn
          s1(j) = cmplx(0.0,0.0)
777     s2(j) = cmplx(0.0,0.0)
c  ***************************************************************
c    Ricatti - Bessel functions with real argument x
c    calculated by upward recurrence.
c  ***************************************************************
        psi0 = dcos(dx)
        psi1 = dsin(dx)
        chi0 = -sin(x)
        chi1 = cos(x)
        apsi0 = psi0
        apsi1 = psi1
        xi0 = cmplx(apsi0, -chi0)
        xi1 = cmplx(apsi1, -chi1)
        qsca = 0.0
        n = 1
200     dn = n
        rn = n
        fn = ( 2. * rn + 1. ) / ( rn * (rn + 1.) )
        psi = ( 2. * dn - 1. ) * psi1 / dx - psi0
        apsi = psi
        chi = ( 2. * rn - 1. ) * chi1 / x - chi0
        xi = cmplx(apsi, -chi)
        an = ( d(n) / refrel + rn / x ) * apsi - apsi1
        an = an / ( ( d(n) / refrel + rn / x ) * xi - xi1 )
        bn =  ( refrel * d(n) + rn / x ) * apsi - apsi1
        bn = bn / ( ( refrel * d(n) + rn / x ) * xi - xi1 )
       qsca = qsca + ( 2. * rn + 1. ) * ( cabs(an) * cabs(an) + cabs(bn)
     + * cabs(bn) )
        do 789 j = 1, nang
          jj = 2 * nang - j
          pi(j) = pi1(j)
          tau(j) = rn * amu(j) * pi(j) - ( rn + 1. ) * pi0(j)
          p = (-1.) ** ( n - 1 )
          s1(j) = s1(j) + fn * ( an * pi(j) + bn * tau(j) )
          t = (-1.) ** n
          s2(j) = s2(j) + fn * ( an * tau(j) + bn * pi(j) )
          if ( j.eq.jj ) go to 789
          s1(jj) = s1(jj) + fn * ( an * pi(j) * p + bn * tau(j) * t )
          s2(jj) = s2(jj) + fn * ( an * tau(j) * t + bn * pi(j) * p )
789     continue
        psi0 = psi1
        psi1 = psi
        apsi1 = psi1
        chi0 = chi1
        chi1 = chi
        xi1 = cmplx(apsi1, -chi1)
        n = n + 1
        rn = n
        do 999 j = 1, nang
        pi1(j) = ( ( 2. * rn - 1. ) / ( rn - 1. ) * amu(j) * pi(j) )
        pi1(j) = pi1(j) - rn * pi0(j) / ( rn - 1. )
999     pi0(j) = pi(j)
        if ( n - 1 - nstop ) 200, 300, 300
300     qsca = ( 2. / ( x * x ) ) * qsca
        qext = ( 4. / ( x * x ) ) * real( s1(1) )
        qback = ( 4. / ( x * x ) ) * cabs( s1(2 * nang - 1) )
     +* cabs( s1( 2 * nang - 1 ) )
        return
        end
cccccccccccccccccccccccccccccccccccccccccccccc
c SUBROUTINE TRAPR integrates under the angles
c using a trapezoidal rule method
cccccccccccccccccccccccccccccccccccccccccccccc
      subroutine trapr(elem3,tot,N,pii)
      integer ang1,ang2,end,N,base
      real area,tot,elem3(0:180),right,left,rad1,rad2
       ang1=35
       ang2=120
      end=ang2-1
      N=0
      base=1
      tot=0.
      do 100 i = ang1,end,1
         rad1=i*pii/180.
         rad2=(i+1)*pii/180.
         left=elem3(i)*sin(rad1)
         right=elem3(i+1)*sin(rad2)
         area= 0.5*base*(left+right)
         tot=tot+area
         N=N+1
c         write(13,*)left,right,area,tot,N
 100  continue
      return
      end
c	This subroutine calculates the scattering, extinction, and backscatter coeffcients
c	For a particular size
	subroutine calcOpticData(psize,scat,extinc,bscat,refreal,refimg)
	double precision Qext,Qscat,Qback
	double precision Qepcas,Qspcas,Qbpcas,gpcas
	double precision x,pi,radpca(500)
	double precision binwid,delbin
	double precision upper,lower
	integer nsangl
c	These are statements needed by Toon and Ackerman Code
        REAL*8 RUM,RCM,RFR,RFI,WVNO,QXT,QSCT,QRPD,QLBS
	INTEGER*4 IERR
cc Constants for extinction calculations
	rfr=refreal
	rfi=refimg
	nsangl=3
	pi=4.0d0*datan(1.0d0)
	WVL = .550
C       COMPUTE WAVE NUMBER IN UNITS OF 1/CM:
      WVNO=2.D0*PI/WVL*1.0D4
cc ** Extinction and backscatter calculations performed here **
c
c ** Q values calculated here **
	sz = psize
	if (sz .le. .99)del = .01
	if ((sz .gt. 1.0).and.(sz .lt. 10))del = .1
	if ((sz .gt. 10.))del = 1
 	lower = sz
 	upper = sz + del
	binwid=(upper-lower)/2.0d0
	delbin=binwid/100.0d0
	Qepcas=0.0d0
	Qspcas=0.0d0
	Qbpcas=0.0d0
	gpcas=0.0d0
	do 499 k=1,100
		rum=lower/2.0d0+k*delbin
		x=2.0d0*pi*rum/wvl
c	Compute now using T&A code
C       CONVERT RADIUS TO CM:
        	RCM=RUM*1.0D-4
        	CALL DMIESS(RCM,rfr,rfi,0.D0,0.D0,0.D0,
     $		WVNO,QXT,QSCT,QRPD,QLBS,1,IERR)
C       SINGLE SCATTER ALBEDO:
        	gpcas=gpcas+QRPD
		Qbpcas=Qbpcas+Qlbs
		Qepcas=Qepcas+Qxt
		Qspcas=Qspcas+Qsct
499	continue	
	Qepcas=Qepcas/100.0d0
	Qspcas=Qspcas/100.0d0
	Qbpcas=Qbpcas/100.0d0
	gpcas=gpcas/100.0d0
C       gpcas IS THE ASYMMETRY FACTOR WEIGHTED BY THE SCATTERING
C       EFFICIENCY:
      gpcas=gpcas/qspcas
	scat=Qspcas
	extinc=Qepcas
	bscat = Qbpcas
	return
	end

      SUBROUTINE DMIESS(RO,RFR,RFI,R,RE2,TMAG2,WVNO,QEXT,QSCAT,CTBRQS,
     1 QBS,IT,IERR)
C
C     DOUBLE PRECISION STATEMENT REQUIRED ON PRIME OR PC:
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C
C
      COMPLEX*16    FNAP,      FNBP,      ACAP,           W,
     2              FNA,       FNB,       RF,           RRF,
     3              RRFX,      WM1,       FN1,          FN2,
     4              TC1,       TC2,       WFN(2),       Z(4),
     5              K1,        K2,        K3,
     6              RC,        U(8),         DH1,
     7              DH2,       DH4,       P24H24,       P24H21,
     8              PSTORE,    HSTORE,    DUMMY,        DUMSQ
C
      INTEGER*4 SIZE
      PARAMETER (SIZE=8000)
      DIMENSION W(3,SIZE),ACAP(SIZE)
      DIMENSION     T(5),      TA(4),     TB(2),        TC(2),
     2   TD(2),TE(2),PI(3,1 ),TAU(3,1 ),CSTHT(1 ),THETD(1 ),SI2THT(1 ),
     3 ELTRMX(4,1 ,2)
C
C     DUMMY COMMON BLOCKS REQUIRED TO STORE BIG ARRAYS ON PRIME OR PC:
      COMMON/DUMMY1/W
      COMMON/DUMMY2/ACAP
C
C  THE PARAMETERS PI, TAU, CSTHT, THETD, SI2THT, AND ELTRMX SHOULD HAVE
C  DIMENSION IT (INSTEAD OF 1 )  AND BE DUMMY ARGUMENTS IN THE SUBROUTINE
C  CALL FOR MULTIPLE ANGLE CALCULATIONS.
C
      EQUIVALENCE   (  FNA,TB(1) ),   ( FNB,TC(1) ),
     2              ( FNAP,TD(1) ),   (FNBP,TE(1) )
C
      JX=1
      THETD(1)=0.D0
      LL=SIZE
C
      IERR=0
      IFLAG = 1
      IF ( R/RO .LT. 1.0D-06 )   IFLAG = 2
C
      IF ( JX .LE. IT )   GO TO 20
         WRITE( 6,7 )
         STOP 30
   20 RF =  DCMPLX( RFR,  -RFI )
      RC =  DCMPLX( RE2,-TMAG2 )
      X  =  RO * WVNO
      K1 =  RC * WVNO
      K2 =  RF * WVNO
      K3 =  DCMPLX( WVNO, 0.0D0 )
      Z(1) =  K2 * RO
      Z(2) =  K3 * RO
      Z(3) =  K1 * R
      Z(4) =  K2 * R
C
      X1   =  DREAL( Z(1) )
      Y1   = DIMAG( Z(1) )
      X4   =  DREAL( Z(4) )
      Y4   = DIMAG( Z(4) )
      RRF  =  1.0D0 / RF
      RX   =  1.0D0 / X
      RRFX =  RRF * RX
      T(1) =  ( X**2 ) * ( RFR**2 + RFI**2 )
      T(1) =  SQRT( T(1) )
      NMX1 =  1.1D0 * T(1)
C
      IF ( NMX1 .LE. LL-1 )   GO TO 21
      IERR=1
      RETURN
   21 NMX2 = T(1)
      IF ( NMX1 .GT.  150 )   GO TO 22
         NMX1 = 150
         NMX2 = 135
C
   22 ACAP( NMX1+1 )  =  ( 0.0D0,0.0D0 )
      IF ( IFLAG .EQ. 2 )   GO TO 26
         DO 29   N = 1,3
   29    W( N,NMX1+1 )  =  ( 0.0D0,0.0D0 )
   26 CONTINUE
      DO 23   N = 1,NMX1
         NN = NMX1 - N + 1
         ACAP(NN)=(NN+1) * RRFX - 1.0D0 / ( (NN+1) * RRFX + ACAP(NN+1) )
C
         IF ( IFLAG .EQ. 2 )   GO TO 23
            DO 31   M = 1,3
   31       W( M,NN ) = (NN+1) / Z(M+1)  -
     1                    1.0D0 / (  (NN+1) / Z(M+1)  +  W( M,NN+1 )  )
   23 CONTINUE
C
      DO 30   J = 1,JX
      IF ( THETD(J) .LT. 0.0D0 )  THETD(J) =  ABS( THETD(J) )
      IF ( THETD(J) .GT. 0.0D0 )  GO TO 24
      CSTHT(J)  = 1.0D0
      SI2THT(J) = 0.0D0
      GO TO 30
   24 IF ( THETD(J) .GE. 90.0D0 )  GO TO 25
      T(1)      =  ( 3.14159265359D0 * THETD(J) ) / 180.0D0
      CSTHT(J)  =  COS( T(1) )
      SI2THT(J) =  1.0D0 - CSTHT(J)**2
      GO TO 30
   25 IF ( THETD(J) .GT. 90.0D0 )  GO TO 28
      CSTHT(J)  =  0.0D0
      SI2THT(J) =  1.0D0
      GO TO 30
   28 WRITE( 6,5 )  THETD(J)
      STOP 34
   30 CONTINUE
C
      DO 35  J = 1,JX
      PI(1,J)  =  0.0D0
      PI(2,J)  =  1.0D0
      TAU(1,J) =  0.0D0
      TAU(2,J) =  CSTHT(J)
   35 CONTINUE
C
C   INITIALIZATION OF HOMOGENEOUS SPHERE
      T(1)   =  COS(X)
      T(2)   =  SIN(X)
      WM1    =  DCMPLX( T(1),-T(2) )
      WFN(1) =  DCMPLX( T(2), T(1) )
      TA(1)  =  T(2)
      TA(2)  =  T(1)
      WFN(2) =  RX * WFN(1) - WM1
      TA(3)  =   DREAL(WFN(2))
      TA(4)  =  DIMAG(WFN(2))
C
      IF ( IFLAG .EQ. 2 )   GO TO 560
C
      N = 1
C
C INITIALIZATION PROCEDURE FOR STRATIFIED SPHERE BEGINS HERE
C
      SINX1   =  SIN( X1 )
      SINX4   =  SIN( X4 )
      COSX1   =  COS( X1 )
      COSX4   =  COS( X4 )
      EY1     =  EXP( Y1 )
      E2Y1    =  EY1 * EY1
      EY4     =  EXP( Y4 )
      EY1MY4  =  EXP( Y1 - Y4 )
      EY1PY4  =  EY1 * EY4
      EY1MY4  =  EXP( Y1 - Y4 )
      AA  =  SINX4 * ( EY1PY4 + EY1MY4 )
      BB  =  COSX4 * ( EY1PY4 - EY1MY4 )
      CC  =  SINX1 * ( E2Y1 + 1.0D0 )
      DD  =  COSX1 * ( E2Y1 - 1.0D0 )
      DENOM  = 1.0D0 + E2Y1 * ( 4.0D0 * SINX1 * SINX1 - 2.0D0 + E2Y1 )
      REALP   =  ( AA * CC  +  BB * DD ) / DENOM
      AMAGP   =  ( BB * CC  -  AA * DD ) / DENOM
      DUMMY   =  DCMPLX( REALP, AMAGP )
      AA  =  SINX4 * SINX4 - 0.5D0
      BB  =  COSX4 * SINX4
      P24H24  =  0.5D0 + DCMPLX( AA,BB ) * EY4 * EY4
      AA  =  SINX1 * SINX4  -  COSX1 * COSX4
      BB  =  SINX1 * COSX4  +  COSX1 * SINX4
      CC  =  SINX1 * SINX4  +  COSX1 * COSX4
      DD  = -SINX1 * COSX4  +  COSX1 * SINX4
      P24H21  =  0.5D0 * DCMPLX( AA,BB ) * EY1 * EY4  +
     2           0.5D0 * DCMPLX( CC,DD ) * EY1MY4
      DH4  =  Z(4) / ( 1.0D0 + ( 0.0D0,1.0D0 ) * Z(4) )  -  1.0D0 / Z(4)
      DH1  =  Z(1) / ( 1.0D0 + ( 0.0D0,1.0D0 ) * Z(1) )  -  1.0D0 / Z(1)
      DH2  =  Z(2) / ( 1.0D0 + ( 0.0D0,1.0D0 ) * Z(2) )  -  1.0D0 / Z(2)
      PSTORE  =  ( DH4 + N / Z(4) )  *  ( W(3,N) + N / Z(4) )
      P24H24  =  P24H24 / PSTORE
      HSTORE  =  ( DH1 + N / Z(1) )  *  ( W(3,N) + N / Z(4) )
      P24H21  =  P24H21 / HSTORE
      PSTORE  =  ( ACAP(N) + N / Z(1) )  /  ( W(3,N) + N / Z(4) )
      DUMMY   =  DUMMY * PSTORE
      DUMSQ   =  DUMMY * DUMMY
C
      U(1) =  K3 * ACAP(N)  -  K2 * W(1,N)
      U(2) =  K3 * ACAP(N)  -  K2 * DH2
      U(3) =  K2 * ACAP(N)  -  K3 * W(1,N)
      U(4) =  K2 * ACAP(N)  -  K3 * DH2
      U(5) =  K1 *  W(3,N)  -  K2 * W(2,N)
      U(6) =  K2 *  W(3,N)  -  K1 * W(2,N)
      U(7) =  ( 0.0D0,-1.0D0 )  *  ( DUMMY * P24H21 - P24H24 )
      U(8) =  TA(3) / WFN(2)
C
      FNA  =  U(8) * ( U(1)*U(5)*U(7)  +  K1*U(1)  -  DUMSQ*K3*U(5) ) /
     2               ( U(2)*U(5)*U(7)  +  K1*U(2)  -  DUMSQ*K3*U(5) )
      FNB  =  U(8) * ( U(3)*U(6)*U(7)  +  K2*U(3)  -  DUMSQ*K2*U(6) ) /
     2               ( U(4)*U(6)*U(7)  +  K2*U(4)  -  DUMSQ*K2*U(6) )
      GO TO 561
  560 TC1  =  ACAP(1) * RRF  +  RX
      TC2  =  ACAP(1) * RF   +  RX
      FNA  =  ( TC1 * TA(3)  -  TA(1) ) / ( TC1 * WFN(2)  -  WFN(1) )
      FNB  =  ( TC2 * TA(3)  -  TA(1) ) / ( TC2 * WFN(2)  -  WFN(1) )
C
  561 CONTINUE
      FNAP = FNA
      FNBP = FNB
      T(1) = 1.50D0
C
      TB(1) = T(1) * TB(1)
      TB(2) = T(1) * TB(2)
      TC(1) = T(1) * TC(1)
      TC(2) = T(1) * TC(2)
      DO 60 J = 1,JX
          ELTRMX(1,J,1) = TB(1) * PI(2,J) + TC(1) * TAU(2,J)
          ELTRMX(2,J,1) = TB(2) * PI(2,J) + TC(2) * TAU(2,J)
          ELTRMX(3,J,1) = TC(1) * PI(2,J) + TB(1) * TAU(2,J)
          ELTRMX(4,J,1) = TC(2) * PI(2,J) + TB(2) * TAU(2,J)
          ELTRMX(1,J,2) = TB(1) * PI(2,J) - TC(1) * TAU(2,J)
          ELTRMX(2,J,2) = TB(2) * PI(2,J) - TC(2) * TAU(2,J)
          ELTRMX(3,J,2) = TC(1) * PI(2,J) - TB(1) * TAU(2,J)
          ELTRMX(4,J,2) = TC(2) * PI(2,J) - TB(2) * TAU(2,J)
   60 CONTINUE
C
      QEXT   = 2.0D0 * ( TB(1) + TC(1))
      QSCAT  = ( TB(1)**2 + TB(2)**2 + TC(1)**2 + TC(2)**2 ) / 0.75D0
C
      QBSR   = -2.0D0*(TC(1) - TB(1))
      QBSI   = -2.0D0*(TC(2) - TB(2))
      RMM    = -1.0D0
C
      CTBRQS = 0.0D0
      N = 2
   65 T(1) = 2*N - 1
      T(2) =   N - 1
      T(3) = 2*N + 1
      DO 70  J = 1,JX
          PI(3,J)  = ( T(1) * PI(2,J) * CSTHT(J) - N * PI(1,J) ) / T(2)
          TAU(3,J) = CSTHT(J) * ( PI(3,J) - PI(1,J) )  -
     1                          T(1) * SI2THT(J) * PI(2,J)  +  TAU(1,J)
   70 CONTINUE
C
C   HERE SET UP HOMOGENEOUS SPHERE
      WM1    =  WFN(1)
      WFN(1) =  WFN(2)
      TA(1)  =  DREAL(WFN(1))
      TA(2)  = DIMAG(WFN(1))
      WFN(2) =  T(1) * RX * WFN(1)  -  WM1
      TA(3)  =  DREAL(WFN(2))
      TA(4)  = DIMAG(WFN(2))
C
      IF ( IFLAG .EQ. 2 )   GO TO 1000
C
C   HERE SET UP STRATIFIED SPHERE
C
      DH2  =  - N / Z(2)  +  1.0D0 / ( N / Z(2) - DH2 )
      DH4  =  - N / Z(4)  +  1.0D0 / ( N / Z(4) - DH4 )
      DH1  =  - N / Z(1)  +  1.0D0 / ( N / Z(1) - DH1 )
      PSTORE  =  ( DH4 + N / Z(4) )  *  ( W(3,N) + N / Z(4) )
      P24H24  =  P24H24 / PSTORE
      HSTORE  =  ( DH1 + N / Z(1) )  *  ( W(3,N) + N / Z(4) )
      P24H21  =  P24H21 / HSTORE
      PSTORE  =  ( ACAP(N) + N / Z(1) )  /  ( W(3,N) + N / Z(4) )
      DUMMY   =  DUMMY * PSTORE
      DUMSQ   =  DUMMY * DUMMY
C
      U(1) =  K3 * ACAP(N)  -  K2 * W(1,N)
      U(2) =  K3 * ACAP(N)  -  K2 * DH2
      U(3) =  K2 * ACAP(N)  -  K3 * W(1,N)
      U(4) =  K2 * ACAP(N)  -  K3 * DH2
      U(5) =  K1 *  W(3,N)  -  K2 * W(2,N)
      U(6) =  K2 *  W(3,N)  -  K1 * W(2,N)
      U(7) =  ( 0.0D0,-1.0D0 )  *  ( DUMMY * P24H21 - P24H24 )
      U(8) =  TA(3) / WFN(2)
C
      FNA  =  U(8) * ( U(1)*U(5)*U(7)  +  K1*U(1)  -  DUMSQ*K3*U(5) ) /
     2               ( U(2)*U(5)*U(7)  +  K1*U(2)  -  DUMSQ*K3*U(5) )
      FNB  =  U(8) * ( U(3)*U(6)*U(7)  +  K2*U(3)  -  DUMSQ*K2*U(6) ) /
     2               ( U(4)*U(6)*U(7)  +  K2*U(4)  -  DUMSQ*K2*U(6) )
C
 1000 CONTINUE
      TC1  =  ACAP(N) * RRF  +  N * RX
      TC2  =  ACAP(N) * RF   +  N * RX
      FN1  =  ( TC1 * TA(3)  -  TA(1) ) /  ( TC1 * WFN(2) - WFN(1) )
      FN2  =  ( TC2 * TA(3)  -  TA(1) ) /  ( TC2 * WFN(2) - WFN(1) )
      M    =  WVNO * R
      IF ( N .LT. M )   GO TO 1002
      IF ( IFLAG .EQ. 2 )   GO TO 1001
      IF (  ABS(  ( FN1-FNA ) / FN1  )  .LT.  1.0D-09   .AND.
     1      ABS(  ( FN2-FNB ) / FN2  )  .LT . 1.0D-09  )       IFLAG = 2
      IF ( IFLAG .EQ. 1 )   GO TO 1002
 1001 FNA  =  FN1
      FNB  =  FN2
C
 1002 CONTINUE
      T(5)  =  N
      T(4)  =  T(1) / ( T(5) * T(2) )
      T(2)  =  (  T(2) * ( T(5) + 1.0D0 )  ) / T(5)
C
      CTBRQS  =  CTBRQS  +  T(2) * ( TD(1) * TB(1)  +  TD(2) * TB(2)  +
     1                               TE(1) * TC(1)  +  TE(2) * TC(2) )
     2                   +  T(4) * ( TD(1) * TE(1)  +  TD(2) * TE(2) )
      QEXT    =   QEXT  +  T(3) * ( TB(1) + TC(1) )
      T(4)    =  TB(1)**2 + TB(2)**2 + TC(1)**2 + TC(2)**2
      QSCAT   =  QSCAT  +  T(3) * T(4)
C
      RMM     =  -RMM
      QBSR    =  QBSR + T(3)*RMM*(TC(1) - TB(1))
      QBSI    =  QBSI + T(3)*RMM*(TC(2) - TB(2))
C
      T(2)    =  N * (N+1)
      T(1)    =  T(3) / T(2)
      K = (N/2)*2
      DO 80 J = 1,JX
       ELTRMX(1,J,1) = ELTRMX(1,J,1)+T(1)*(TB(1)*PI(3,J)+TC(1)*TAU(3,J))
       ELTRMX(2,J,1) = ELTRMX(2,J,1)+T(1)*(TB(2)*PI(3,J)+TC(2)*TAU(3,J))
       ELTRMX(3,J,1) = ELTRMX(3,J,1)+T(1)*(TC(1)*PI(3,J)+TB(1)*TAU(3,J))
       ELTRMX(4,J,1) = ELTRMX(4,J,1)+T(1)*(TC(2)*PI(3,J)+TB(2)*TAU(3,J))
      IF ( K .EQ. N )   GO TO 75
       ELTRMX(1,J,2) = ELTRMX(1,J,2)+T(1)*(TB(1)*PI(3,J)-TC(1)*TAU(3,J))
       ELTRMX(2,J,2) = ELTRMX(2,J,2)+T(1)*(TB(2)*PI(3,J)-TC(2)*TAU(3,J))
       ELTRMX(3,J,2) = ELTRMX(3,J,2)+T(1)*(TC(1)*PI(3,J)-TB(1)*TAU(3,J))
       ELTRMX(4,J,2) = ELTRMX(4,J,2)+T(1)*(TC(2)*PI(3,J)-TB(2)*TAU(3,J))
      GO TO 80
   75  ELTRMX(1,J,2) =ELTRMX(1,J,2)+T(1)*(-TB(1)*PI(3,J)+TC(1)*TAU(3,J))
       ELTRMX(2,J,2) =ELTRMX(2,J,2)+T(1)*(-TB(2)*PI(3,J)+TC(2)*TAU(3,J))
       ELTRMX(3,J,2) =ELTRMX(3,J,2)+T(1)*(-TC(1)*PI(3,J)+TB(1)*TAU(3,J))
       ELTRMX(4,J,2) =ELTRMX(4,J,2)+T(1)*(-TC(2)*PI(3,J)+TB(2)*TAU(3,J))
   80 CONTINUE
C
      IF ( T(4) .LT. 1.0D-14 )   GO TO 100
      N = N + 1
      DO 90 J = 1,JX
         PI(1,J)   =   PI(2,J)
         PI(2,J)   =   PI(3,J)
         TAU(1,J)  =  TAU(2,J)
         TAU(2,J)  =  TAU(3,J)
   90 CONTINUE
      FNAP  =  FNA
      FNBP  =  FNB
      IF ( N .LE. NMX2 )   GO TO 65
      IERR=2
      RETURN
  100 DO 120 J = 1,JX
      DO 120 K = 1,2
         DO  115  I= 1,4
         T(I)  =  ELTRMX(I,J,K)
  115    CONTINUE
         ELTRMX(2,J,K)  =      T(1)**2  +  T(2)**2
         ELTRMX(1,J,K)  =      T(3)**2  +  T(4)**2
         ELTRMX(3,J,K)  =  T(1) * T(3)  +  T(2) * T(4)
         ELTRMX(4,J,K)  =  T(2) * T(3)  -  T(4) * T(1)
  120 CONTINUE
      T(1)    =    2.0D0 * RX**2
C
C QBS IS THE BACK SCATTER CROSS SECTION
C
      PIG   = ACOS(-1.0D0)
      RXP4  = RX*RX/(4.0D0*PIG)
      QBS   = RXP4*(QBSR**2 + QBSI**2)
C
      QEXT    =   QEXT * T(1)
      QSCAT   =  QSCAT * T(1)
      CTBRQS  =  2.0D0 * CTBRQS * T(1)
C
      RETURN
C
    5 FORMAT( 10X,' THE VALUE OF THE SCATTERING ANGLE IS GREATER THAN
     1 90.0D0 DEGREES. IT IS ', D15.4 )
    7 FORMAT( // 10X, 'THE VALUE OF THE ARGUMENT JX IS GREATER THAN IT')
C
      END






