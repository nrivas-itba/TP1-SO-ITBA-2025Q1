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
> ./bin/master.o 
> ```

> [!NOTE]
>
> ## PVS - Studio
>
> Register your PVS-Studio credentials:
>
> ```sh
> pvs-studio-analyzer credentials "<Your User>" "XXXX-XXXX-XXXX-XXXX"
> make pvs # Will generate the PVS-Studio files
> ```
