all:
	wmake -e CPPFLAGS=-DVERSION='\"$(shell git rev-parse --short HEAD)$(shell git status --porcelain | grep -q '^[ MADRCU]' && echo '*')\"'

clean:
	wmake clean
