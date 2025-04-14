# TP1-SO-ITBA-2025Q1

Grupo 10 <!-- Que se quiere sacar 10 -->

```md
▄▖▌        ▄▖▌          
▌ ▛▌▛▌▛▛▌▛▌▌ ▛▌▀▌▛▛▌▛▌▛▘
▙▖▌▌▙▌▌▌▌▙▌▙▖▌▌█▌▌▌▌▙▌▄▌
         ▌          ▌   
```

> [!WARNING]
>
> ## Requirements
>
> `Docker`, `agodio/itba-so-multi-platform:3.0` and a `PVS-Studio` license

> [!IMPORTANT]
>
> ## Build
>
> Run the `run.sh` file to enter the docker container
>
> ```sh
> ./run.sh
>
> # Or enter manually
> docker run --rm -v ${PWD}:/root --security-opt seccomp:unconfined -it agodio/itba-so-multi-platform:3.0
> ```
>
> Then, enter `root` and execute `make`
>
> ```sh
> cd ~
> make all
> ```
>
> Lastly, execute the `ChompChamps` binary
>
> ```sh
> make run p=<PLAYERS> w=<WIDTH> h=<HEIGHT> d=<DELAY> t=<TIMEOUT>
> 
> # Or enter manually
> ./bin/master.o -p ./bin/player10.o -v ./bin/view.o
> ```
>
> If you want to use the original binary provided, it is located in `bin/originales/`. Optionally, you can use the parameter `m=original` in the `make run`

> [!NOTE]
>
> ## Parameters
>
> - `[-w width]`: Board width. **Default and min**: `10`
> - `[-h height]`: Board height. **Default and min**: `10`
> - `[-d delay]`: Print wait time in `ms`. **Default**: `10`
> - `[-t timeout]`: Movement request timeout in `s`. **Default**: `200`
> - `[-s seed]`: Seed utilized for generating the board. **Default**: `time(NULL)`
>   - **In `make_run`**: `time(NULL)`, unmodifiable
> - `[-v view]`: View binary route. **Default**: No view
>   - **In `make_run`**: `bin/view.o`, unmodifiable
> - `-p player1 player2`
>   - **In `make_run`**: `bin/player10.o`, unmodifiable, you can only decide the amount of players

> [!NOTE]
>
> ## PVS-Studio analyzer & Valgrind
>
> Register your PVS-Studio credentials:
>
> ```sh
> pvs-studio-analyzer credentials "<Your User>" "XXXX-XXXX-XXXX-XXXX"
>
> make pvs # Will generate the PVS-Studio files
> ```
>
> If you want to use the Valgrind tool, you can use `make run_valgrind` with the same parameters as `make_run`. Optionally, you can run it manually:
>
> ```sh
> valgrind ./bin/master.o -p ./bin/player10.o -v ./bin/view.o
> ```
