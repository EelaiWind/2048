TARGET = Conscious
GAME = Gameboard
EVALUE = Evalue

$(TARGET).exe : $(TARGET).o $(GAME).c $(EVALUE).o
	gcc  $(TARGET).o $(GAME).o $(EVALUE).o -o $(TARGET).exe
