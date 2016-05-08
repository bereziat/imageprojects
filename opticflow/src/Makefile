CPPFLAGS = `imlib2-config --cflags` `inrinfo --cflags` -I/opt/X11/include
LDFLAGS  = `imlib2-config --libs` `inrinfo --libs`

OPEN = xdg-open

all: img2img flo2flo flo2col

img2img: img2img.o utils.o
flo2flo: flo2flo.o utils.o
flo2col: flo2col.o utils.o colorcode.o
flo2col.o flo2flo.o utils.o: utils.h

test-img2img: img2img
	./img2img ../data/rubberwhale/frame11.png test1.png 128
	$(OPEN) test1.png

test-flo2flo: flo2flo
	./flo2flo ../data/rubberwhale/flow10.flo test2.flo

test-flo2col: flo2col
	./flo2col ../data/rubberwhale/flow10.flo test3.png
	$(OPEN) test3.png

clean:
	rm -f img2img flo2flo flo2col *.o
	rm -f *.flo *.png

