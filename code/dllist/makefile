OBJ=main.o dllist-driver.o dllist.o
dllist:$(OBJ)
	g++ -o dllist $(OBJ)
dllist.o:dllist.h
dllist-driver.o:dllist.h
main.o:dllist.h
clean:
	rm *.o dllist
