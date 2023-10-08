/* wait-for-signal.c: Sleep until given signal, otherwise SIGUSR1.
 *
 *  Copyright © 2023 Revolution Robotics, Inc.
 *
 *  This file is part of inhibit-suspend.
 */
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INT_MAX
# define INT_MAX __INT_MAX__
#endif

#ifndef INT_MIN
# define INT_MIN (-__INT_MAX__ - 1)
#endif

typedef void (*sighandler_t)(int);

static bool valid_int (char *, int *);
static int decode_signal (char *);
static void handle_signal (int);

char *signal_names[] = {
    "EXIT",
    "SIGHUP",
    "SIGINT",
    "SIGQUIT",
    "SIGILL",
    "SIGTRAP",
    "SIGABRT",
    "SIGBUS",
    "SIGFPE",
    "SIGKILL",
    "SIGUSR1",
    "SIGSEGV",
    "SIGUSR2",
    "SIGPIPE",
    "SIGALRM",
    "SIGTERM",
    "SIGSTKFLT",
    "SIGCHLD",
    "SIGCONT",
    "SIGSTOP",
    "SIGTSTP",
    "SIGTTIN",
    "SIGTTOU",
    "SIGURG",
    "SIGXCPU",
    "SIGXFSZ",
    "SIGVTALRM",
    "SIGPROF",
    "SIGWINCH",
    "SIGIO",
    "SIGPWR",
    "SIGSYS",
    NULL,
    NULL,
    "SIGRTMIN",
    "SIGRTMIN+1",
    "SIGRTMIN+2",
    "SIGRTMIN+3",
    "SIGRTMIN+4",
    "SIGRTMIN+5",
    "SIGRTMIN+6",
    "SIGRTMIN+7",
    "SIGRTMIN+8",
    "SIGRTMIN+9",
    "SIGRTMIN+10",
    "SIGRTMIN+11",
    "SIGRTMIN+12",
    "SIGRTMIN+13",
    "SIGRTMIN+14",
    "SIGRTMIN+15",
    "SIGRTMAX-14",
    "SIGRTMAX-13",
    "SIGRTMAX-12",
    "SIGRTMAX-11",
    "SIGRTMAX-10",
    "SIGRTMAX-9",
    "SIGRTMAX-8",
    "SIGRTMAX-7",
    "SIGRTMAX-6",
    "SIGRTMAX-5",
    "SIGRTMAX-4",
    "SIGRTMAX-3",
    "SIGRTMAX-2",
    "SIGRTMAX-1",
    "SIGRTMAX"
  };

int signal_caught = 0;

int
main (int argc, char *argv[])
{
  sigset_t mask, oldmask;
  int ign_signal[] = {
    SIGHUP,
    SIGINT,
    SIGQUIT,
    SIGUSR1,
    SIGUSR2,
    SIGPIPE,
    SIGALRM,
    SIGCHLD,
    SIGTSTP
  };
  char *pgm = strchr (argv[0], '/') ? strrchr (argv[0], '/') + 1 : argv[0];
  int ign_signals = sizeof ign_signal / sizeof ign_signal[0];
  int signo = SIGUSR1;

  if (argc && (signo = decode_signal(argv[1])) == -1)
    {
      fprintf (stderr, "Usage: %s [sigspec] \n", pgm);
      exit (1);
    }

  /* Add signo handler */
 if (signal (signo, (sighandler_t) handle_signal) == SIG_ERR)
    {
      fprintf (stderr, "%s\n", strerror (errno));
      exit (1);
    }

  /* Block signo */
  if (sigemptyset (&mask) < 0
      || sigaddset (&mask, signo) < 0
      || sigprocmask (SIG_BLOCK, &mask, &oldmask) < 0)
    {
      fprintf (stderr, "%s\n", strerror (errno));
      exit (1);
    }

  /* Ignore other signals */
  for (int i = 0; i < ign_signals; ++i)
    if (ign_signal[i] != signo)
      signal(ign_signal[i], SIG_IGN);

  /* Wait for signo */
  fprintf(stderr, "Waiting for %s...\n", signal_names[signo]);

  while (!signal_caught)
    if (sigsuspend (&oldmask) == -1 && errno != EINTR)
      {
        fprintf (stderr, "%s\n", strerror (errno));
        exit (1);
      }

  /* Restore other signals */
  for (int i = 0; i < ign_signals; ++i)
    if (signo != ign_signal[i])
      signal(ign_signal[i], SIG_DFL);

  signal_caught = 0;

  /* Unblock signo */
  if (sigprocmask (SIG_UNBLOCK, &mask, NULL) < 0)
    {
      fprintf (stderr, "%s\n", strerror (errno));
      exit (1);
    }

  exit (0);
}

/*
 * decode_signal: Derived from bash function of the same name (see:
 *     https://git.savannah.gnu.org/cgit/bash.git/tree/trap.c#n231)
 *
 * Given STRING, return the indicated integer signal number. If STRING is
 * "2", "SIGINT", or "INT", then 2 is returned. Return -1 if STRING
 * doesn't contain a valid signal descriptor.
 */
static int
decode_signal (char *string)
{
  int signo;
  char *name;
  char *short_name;

  /* Try converting string to signo using strtol. */
  if (valid_int (string, &signo))
    return (0 < signo && signo < NSIG) ? signo : -1;

#if defined (SIGRTMIN) && defined (SIGRTMAX)
  if (strncasecmp (string, "SIGRTMIN+", 9) == 0)
    {
      if (valid_int (string+9, &signo) && signo >= 0 && signo <= SIGRTMAX - SIGRTMIN)
        return (SIGRTMIN + signo);
      else
        return -1;
    }
  else if (strncasecmp (string, "RTMIN+", 6) == 0)
    {
      if (valid_int (string+6, &signo) && signo >= 0 && signo <= SIGRTMAX - SIGRTMIN)
        return (SIGRTMIN + signo);
      else
        return -1;
    }
  else if (strncasecmp (string, "SIGRTMAX-", 9) == 0)
    {
      if (valid_int (string+9, &signo) && signo >= 0 && signo <= SIGRTMAX - SIGRTMIN)
        return (SIGRTMAX -  signo);
      else
        return -1;
    }
  else if (strncasecmp (string, "RTMAX-", 6) == 0)
    {
      if (valid_int (string+6, &signo) && signo >= 0 && signo <= SIGRTMAX - SIGRTMIN)
        return (SIGRTMAX -  signo);
      else
        return -1;
    }
#endif /* SIGRTMIN && SIGRTMAX */

  for (signo = 1; signo < NSIG; ++signo)
    {
      if ((name = signal_names[signo]) == NULL)
        continue;
      else if (strcasecmp (string, name) == 0)
        return ((int) signo);

      /* A leading `SIG' may be omitted. */
      short_name = name + 3;
      if (strcasecmp (string, short_name) == 0)
        return ((int) signo);
    }

  return -1;
}

static bool
valid_int (char *s, int *ip)
{
  char *endp = NULL;
  long l = strtol(s, &endp, 10);

  /* Long and/or Integer overflow or underflow */
  if (errno == ERANGE || l < INT_MIN || INT_MAX < l)
    return false;

  /* Conversion successful */
  else if (*endp == '\0')
    {
      *ip = (int) l;
      return true;
    }

  /* Conversion incomplete */
  return false;
}

static void
handle_signal (int signo)
{
  if (0 < signo && signo < NSIG)
    signal_caught = 1;
}
