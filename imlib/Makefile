
imlib-exemple: imlib-exemple.c
	gcc $^ -o $@ `imlib2-config --cflags --libs` -I/opt/X11/include
test:
	./imlib-exemple olena.jpg sortie.jpg 150
	if [ `uname` = Linux ]; then xdg-open sortie.jpg; else open sortie.jpg; fi

clean:
	rm imlib-exemple

.PHONY: test clean
