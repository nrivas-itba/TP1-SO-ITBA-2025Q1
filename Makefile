include Makefile.inc

.PHONY: clean

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
	@if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo Running with custom master...; \
		make all && sleep 1 && $(MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O); \
	else \
		echo Running with original master...; \
		make all && sleep 1 && $(_MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O); \
	fi

run_mult:
	@if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo Running with custom master...; \
		make all && sleep 1 && $(MASTER_O) -d 100 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	else \
		echo Running with original master...; \
		make all && sleep 1 && $(_MASTER_O) -d 100 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	fi
