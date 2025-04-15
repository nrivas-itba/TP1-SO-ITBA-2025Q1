#ifndef ARI_H
#define ARI_H

#define ARI_VIEW_EXIT "View exited (%d)\n"
#define ARI_PLAYER_EXIT "Player %s (%u) exited (%d) with a score of %u / %u / %u\n"

#define ARI_OPTARG "w:h:d:p:v:s:t:"
#define ARI_OPTARG_MOD "w:h:v:d:p:s:t:"
#define ARI_WRONG_USAGE "Usage: %s [-w width] [-h height] [-d delay] [-s seed] [-v view] [-t timeout] -p player1 player2 ...\n"
#define ARI_NO_VIEW "Info: Delay parameter ignored since there is no view.\n"
#define ARI_NO_PLAYER "Error: At least one player must be specified using -p.\n"
#define ARI_TOO_MANY_PLAYERS "Error: At most %d players can be specified using -p.\n"
#define ARI_WRONG_BOARD_DIMENSIONS "Error: Minimal board dimensions: %dx%d. Given %dx%d\n"

#define ARI_CLEAR "clear"
#define ARI_WIDTH "width: %d\n"
#define ARI_HEIGHT "height: %d\n"
#define ARI_DELAY "delay: %u\n"
#define ARI_TIMEOUT "timeout: %u\n"
#define ARI_SEED "seed: %u\n"
#define ARI_VIEW "view: %s\n"
#define ARI_NUM_PLAYERS "num_players: %u\n"
#define ARI_PLAYER_NAME_STRING_FORMAT "  %s\n"

#define ARI_PIPE "pipe"
#define ARI_FORK "fork"
#define ARI_SETUID "setuid"
#define ARI_WAITPID "waitpid"
#define ARI_WAIT "wait"
#define ARI_SNPRINTF "%d"
#define ARI_EXECVE "execve"
#define ARI_SLEEP 2

#endif