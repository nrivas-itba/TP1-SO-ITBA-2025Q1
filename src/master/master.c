// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <libgen.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include "../utils/gameLogic.h"
#include "../utils/utils.h"
#include "../utils/ipc.h"
#include "../utils/ari.h"

#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10
#define DEFAULT_DELAY 200
#define DEFAULT_TIMEOUT 10
#define DEFAULT_SEED time(NULL)

void validateArgs(gameConfig_t* gameConfig, int playerCount, int width, int height) {
  if (playerCount < MIN_PLAYERS) {
    fprintf(stderr, ARI_NO_PLAYER);
    exit(EXIT_FAILURE);
  }

  if (width < DEFAULT_WIDTH || height < DEFAULT_HEIGHT) {
    fprintf(stderr, ARI_WRONG_BOARD_DIMENSIONS, DEFAULT_WIDTH, DEFAULT_HEIGHT, width, height);
    exit(EXIT_FAILURE);
  }

  return;
}

void createPlayer(char* name, player_t* player) {
  //memset(dest,0,sizeof(dest[0])*MAX_PLAYERS); The chomp champs original implementation uses memset to initialize players to 0. But it is not neccesary since strncpy guarantees to write 'count' characters, filling with 0 if source is shorter than len.
  strncpy(player->name, basename(name), MAX_PLAYER_NAME_LEN);
  player->name[MAX_PLAYER_NAME_LEN - 1] = 0; //strncpy does not guarantee a null terminated string
  player->score = 0;
  player->invalidMovementRequestsCount = 0;
  player->validMovementRequestsCount = 0;
  player->isBlocked = 0;
}

void initializeGameSync(gameSync_t* gameSync) {
  semInit(&(gameSync->printNeeded), 0);
  semInit(&(gameSync->printDone), 0);
  semInit(&(gameSync->masterWantsToReadMutex), 1);
  semInit(&(gameSync->readGameStateMutex), 1);
  semInit(&(gameSync->readersCountMutex), 1);
  gameSync->readersCount = 0;
}


void initializeRandomBoard(gameState_t* gameState, unsigned int seed) {
  srand(seed);
  for (unsigned int i = 0; i < gameState->height * gameState->width; i++) {
    gameState->board[i] = (rand() % MAX_BOARD_VALUE) + MIN_BOARD_VALUE;
  }
}

void copyPlayers(player_t dest[MAX_PLAYERS], player_t org[MAX_PLAYERS]) {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    dest[i] = org[i]; //OBS: inefficient, it would be ideal to directly write the players in the shared memory.
  }
}

void configureGame(int argc, char* argv[], gameConfig_t* gameConfig) {
  int opt;
  int playerCount = 0;
  int width = DEFAULT_WIDTH;
  int height = DEFAULT_HEIGHT;
  char isDelayDeclared = 0;
  player_t playerList[MAX_PLAYERS] = { 0 };

  while ((opt = getopt(argc, argv, ARI_OPTARG_MOD)) != -1) {
    switch (opt) {
    case 'w':
      width = atoi(optarg);
      break;
    case 'h':
      height = atoi(optarg);
      break;
    case 'd':
      gameConfig->delay = atoi(optarg);
      if (gameConfig->delay < 0) {
        gameConfig->delay = 0;
      }
      isDelayDeclared = 1;
      break;
    case 't':
      gameConfig->timeout = atoi(optarg);
      if (gameConfig->timeout < 0) {
        gameConfig->timeout = 0;
      }
      break;
    case 's':
      gameConfig->seed = atoi(optarg);
      break;
    case 'v':
      gameConfig->view = optarg;
      break;
    case 'p':
      while (optind - 1 < argc && argv[optind - 1][0] != '-') {
        if (playerCount < MAX_PLAYERS) {
          createPlayer(argv[optind - 1], &playerList[playerCount]);

          gameConfig->playerPaths[playerCount] = argv[optind - 1];
          playerCount++;
        }
        else {
          fprintf(stderr, ARI_TOO_MANY_PLAYERS, MAX_PLAYERS);
          exit(EXIT_FAILURE);
        }
        optind++;
      }
      optind--; // Fix index for getopt
      break;
    default:
      fprintf(stderr, ARI_WRONG_USAGE, argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  validateArgs(gameConfig, playerCount, width, height);

  if (isDelayDeclared && gameConfig->view == NULL) {
    fprintf(stderr, ARI_NO_VIEW);
  }

  gameConfig->sync = createShm(GAME_SYNC,
    sizeof(*(gameConfig->sync)),
    1,
    &gameConfig->syncFd
  );

  gameConfig->state = createShm(GAME_STATE,
    sizeof(*(gameConfig->state)) + sizeof(gameConfig->state->board[0]) * width * height,
    0,
    &gameConfig->stateFd
  );

  gameConfig->state->width = width;
  gameConfig->state->height = height;
  gameConfig->state->playerCount = playerCount;
  gameConfig->state->isOver = 0;

  copyPlayers(gameConfig->state->playerList, playerList);

  initializeRandomBoard(gameConfig->state, gameConfig->seed);
  initializeGameSync(gameConfig->sync);
}

void printArgs(gameConfig_t* gameConfig) {
  printf(ARI_WIDTH, gameConfig->state->width);
  printf(ARI_HEIGHT, gameConfig->state->height);
  printf(ARI_DELAY, (unsigned int)gameConfig->delay);
  printf(ARI_TIMEOUT, (unsigned int)gameConfig->timeout);
  printf(ARI_SEED, gameConfig->seed);
  printf(ARI_VIEW, gameConfig->view ? gameConfig->view : "-");
  printf(ARI_NUM_PLAYERS, gameConfig->state->playerCount);
  for (int i = 0; i < gameConfig->state->playerCount; i++) {
    printf(ARI_PLAYER_NAME_STRING_FORMAT, gameConfig->playerPaths[i]);
  }
  sleep(ARI_SLEEP);
}

static inline double calculatePosition(double size, char isX, unsigned int i, unsigned int playerCount) {
  if (playerCount == 1) {
    return size / 2;
  }
  double angle = (i * M_PI * 2) / playerCount;
  return (isX ? cos(angle) : sin(angle)) * (size / 3) + size / 2 + 0.5;
}

void spawnPlayers(gameState_t* gameState) {
  for (unsigned int i = 0; i < gameState->playerCount; i++) {
    gameState->playerList[i].x = calculatePosition(gameState->width, 1, i, gameState->playerCount);
    gameState->playerList[i].y = calculatePosition(gameState->height, 0, i, gameState->playerCount);
    gameState->board[gameState->width * gameState->playerList[i].y + gameState->playerList[i].x] = -i;
  }
}

pid_t forkToView(char* view, unsigned int width, unsigned int height) {
  pid_t pid = fork();
  if (pid == -1) {
    errExit(ARI_FORK);
  }
  if (pid == 0) {
    if (setuid(1000) == -1) {
      errExit(ARI_SETUID);
    }
    execveWithArgs(view, width, decimalLen(width), height, decimalLen(height));
  }
  return pid;
}

void spawnPlayerProcesses(gameState_t* gameState, char** playerPaths, pipefd_t* pipefd) {
  for (unsigned int i = 0; i < gameState->playerCount; i++) {
    pid_t pid = fork();
    if (pid == -1) {
      errExit(ARI_FORK);
    }
    if (pid == 0) {
      if (setuid(1000) == -1) {
        errExit(ARI_SETUID);
      }
      closeForeignPipes(i, gameState->playerCount, pipefd);
      closeFd(pipefd[i].read);
      dup2Fd(pipefd[i].write, 1);
      closeFd(pipefd[i].write); //We validate this, even though ChompChamps (6b398ab0f1be541975002579f26f509f) doesn't
      execveWithArgs(playerPaths[i], gameState->width, decimalLen(gameState->width), gameState->height, decimalLen(gameState->height));
    }
    else {
      gameState->playerList[i].pid = pid;
    }
  }
}

void waitForView(pid_t view) {
  int statLoc;
  if (waitpid(view, &statLoc, 0) == -1) {
    errExit(ARI_WAITPID);
  }
  printf(ARI_VIEW_EXIT, statLoc); //OBS Original chomp champs does not use WEXITSTATUS -- Asked by email and was mentioned it wasn't necessary
  return;
}

static inline unsigned int findPlayer(player_t* playerList, unsigned int playerCount, pid_t pid) {
  unsigned int i;
  for (i = 0; i < playerCount; i++) {
    if (playerList[i].pid == pid) {
      break;
    }
  }
  return i;
}

void waitAllPlayers(player_t* playerList, unsigned int playerCount) {
  for (unsigned int i = 0; i < playerCount; i++) {
    int statLoc;
    pid_t pid = wait(&statLoc);
    if (pid == -1) {
      errExit(ARI_WAIT);
    }
    unsigned int playerNumber = findPlayer(playerList, playerCount, pid);
    printf(ARI_PLAYER_EXIT,
      playerList[playerNumber].name,
      playerNumber,
      statLoc,
      playerList[playerNumber].score,
      playerList[playerNumber].validMovementRequestsCount,
      playerList[playerNumber].invalidMovementRequestsCount
    );
  }
}

void unInitializeGameSync(gameSync_t* gameSync) {
  semDestroy(&(gameSync->printNeeded));
  semDestroy(&(gameSync->printDone));
  semDestroy(&(gameSync->masterWantsToReadMutex));
  semDestroy(&(gameSync->readGameStateMutex));
  semDestroy(&(gameSync->readersCountMutex));
}

void unconfigureGame(gameConfig_t* gameConfig) {
  unInitializeGameSync(gameConfig->sync);
  closeMem(GAME_STATE,
    sizeof(*(gameConfig->state)) + sizeof(gameConfig->state->board[0]) * gameConfig->state->width * gameConfig->state->height,
    gameConfig->state,
    &gameConfig->stateFd
  );
  closeMem(GAME_SYNC,
    sizeof(*(gameConfig->sync)),
    gameConfig->sync,
    &gameConfig->syncFd
  );
}

int main(int argc, char* argv[]) {
  gameConfig_t gameConfig = {
      .delay = DEFAULT_DELAY,
      .seed = DEFAULT_SEED,
      .timeout = DEFAULT_TIMEOUT,
      .view = NULL,
      .playerPaths = {0}
  };

  system(ARI_CLEAR); //I dont like this, I would use CSI 2 J ("\033[2J"), But the profesor's bynary uses system()
  configureGame(argc, argv, &gameConfig);
  printArgs(&gameConfig);

  spawnPlayers(gameConfig.state);

  pid_t viewPid = 0;
  if (gameConfig.view) {
    viewPid = forkToView(gameConfig.view, gameConfig.state->width, gameConfig.state->height);
  }

  pipefd_t pipefd[MAX_PLAYERS];
  createPipes(gameConfig.state->playerCount, pipefd);
  spawnPlayerProcesses(gameConfig.state, gameConfig.playerPaths, pipefd);
  closeWritePipes(gameConfig.state->playerCount, pipefd); //The original chomp champs separates this outside of player spawn

  game(&gameConfig, pipefd);

  if (viewPid > 0) {
    waitForView(viewPid);
  }

  waitAllPlayers(gameConfig.state->playerList, gameConfig.state->playerCount);

  unconfigureGame(&gameConfig);
  return 0;
}