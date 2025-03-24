include Makefile.inc

all: vista player10 master

vista:
	cd $(VISTA_DIR) && make all

player10:
	cd $(PLAYER10_DIR) && make all

master:
	cd $(MASTER_DIR) && make all

clean:
	cd $(BIN_DIR) && rm *.o

run:
	make all && sleep 1 && $(_MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O)

run_mult:
	make all && sleep 1 && $(_MASTER_O) -d 100 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O)

run_custom:
	make all && sleep 1 && $(MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O)

run_mult_custom:
	make all && sleep 1 && $(MASTER_O) -d 100 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O)
