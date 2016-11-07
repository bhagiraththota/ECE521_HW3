CC = gcc
CFLAGS=	-g
CSRC=	main.c utils.c res.c isrc.c vsrc.c vcvs.c vccs.c ccvs.c cccs.c tf.c gyro.c opamp.c  dio.c  mosfet.c fetlim.c limvds.c pnjlim.c bjt.c
COBJ=	main.o utils.o res.o isrc.o vsrc.o vcvs.o vccs.o ccvs.o cccs.o tf.o gyro.o opamp.o  dio.o  mosfet.o fetlim.o limvds.o pnjlim.o bjt.o

.c.o:	${CSRC}
	${CC} ${CFLAGS} -c $*.c
myspice: ${COBJ} 
	${CC} ${CFLAGS} -o myspice ${COBJ} sparse/sparse.a -lm
clean:
	rm myspice ${COBJ} result*
