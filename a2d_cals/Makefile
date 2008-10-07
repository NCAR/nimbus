#
#    Makefile for a tool that converts lines taken from a dump from the
#    PSQL calibration database:
#
# psql calibrations
#
# calibrations=# \o /tmp/a2d-cals.dat
# calibrations=# select var_name, cal_date, cal from calibrations ;
# calibrations=# \q
#
# grep 1600_CH3_.T a2d-cals.dat
#  A2D1600_CH3_2T        | 2008-07-30 21:38:59 | {-0.194403,1.06952}
#  A2D1600_CH3_1T        | 2008-07-30 22:11:29 | {-0.388886,1.06898}
#
#    into xml lines like these:
#
# grep 1600_CH3_.T a2d-cals.dat | psql2xml
# <!-- A2D1600_CH3_2T        | 2008-07-30 21:38:59 | {-0.194403,1.06952} -->
#                     <parameter name="gain"     value="2"       type="float"/> <parameter name="bipolar"      value="true"       type="bool"/>
#                     <parameter name="corSlope" value="1.06952" type="float"/> <parameter name="corIntercept" value="-0.194403"  type="float"/>
# <!-- A2D1600_CH3_1T        | 2008-07-30 22:11:29 | {-0.388886,1.06898} -->
#                     <parameter name="gain"     value="1"       type="float"/> <parameter name="bipolar"      value="true"       type="bool"/>
#                    <parameter name="corSlope" value="1.06898" type="float"/> <parameter name="corIntercept" value="-0.388886"  type="float"/>
#
CC=		g++
BINDIR=		${JLOCAL}/bin

SRCS=	psql2xml.cc

PROG=	psql2xml

${PROG}: ${SRCS}
	${CC} -o ${PROG} ${SRCS}

install: ${PROG}
	test -d ${BINDIR} || mkdir ${BINDIR}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${PROG}
