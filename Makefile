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

run: # No deberia haber codigo repetido despues de cada if, se puede poner un flag MASTER, que tome el valor de _MASTER_O o de MASTER_O dependiendo del argumento
	 # La variable _MASTER_O no es significativa. Ambas deberian especificar en su nombre si son "ORIGINAL" o "CUSTOM/PERSONALIZED/SELF_DEVELOP/ETC"
	@if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo Running with custom master...; \
		make all && sleep 1 && $(MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O); \ #
	else \
		echo Running with original master...; \
		make all && sleep 1 && $(_MASTER_O) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O); \
	fi

runARM:
	make all && sleep 1 && $(_MASTER_O_ARM) -d 0 -t 1 -v $(VISTA_O) -p $(PLAYER10_O)

run_mult:
	@if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo Running with custom master...; \
		make all && sleep 1 && $(MASTER_O) -d 1 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	else \
		echo Running with original master...; \
		make all && sleep 1 && $(_MASTER_O) -d 1 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	fi

run_multARM: #TODO la arquitectura se deberia determinar automaticamente. O a lo sumo añadir el flag opcionalmente, sin repetir codigo
	@if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo Running with custom master...; \
		make all && sleep 1 && $(MASTER_O) -d 1 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	else \
		echo Running with original master...; \
		make all && sleep 1 && $(_MASTER_O_ARM) -d 1 -t 10 -v $(VISTA_O) -p $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O) $(PLAYER10_O); \
	fi
# Make testee todo con valgrin y pvs studio