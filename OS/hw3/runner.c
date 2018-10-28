/**
@author     Dushan Terzikj

Problem Sheet 3
Operating Systems
Prof. Juergen Schoenwaelder
*/
/*
 * p3-runner/runner-template.c --
 *
 *      Runners meeting regularly (if they manage). See Operating
 *      Systems '2018 assignment #3 for more details.
 */

//#define _REENTRANT
//#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define LATE_THRESHOLD          5
#define LONELY_THRESHOLD        10

#define GROUP_SLEEPING          0x01
#define GROUP_ASSEMBLING        0x02
#define GROUP_RUNNING           0x03
#define GROUP_FINISHING         0x04

#define RUNNER_SLEEPING         0x01
#define RUNNER_LEADING          0x02
#define RUNNER_ASSEMBLING       0x03
#define RUNNER_RUNNING          0x04
#define RUNNER_WAITING          0x05

typedef struct group {
    unsigned        state;      /* the state of the running group */
    unsigned        arriving;   /* number of runnners arriving */
    unsigned        running;    /* number of runnners running */
} group_t;

typedef struct runner {
    unsigned        id;         /* identity of the runner */
    pthread_t       tid;        /* thread identifier */
    unsigned        state;      /* state of the runner */
    unsigned        late;       /* number of runs missed (late arrival) */
    unsigned        lonely;     /* number of runs without any other runners */
    unsigned        runs;       /* number of runs completed */
    group_t        *group;      /* the group this runner belongs to */
} runner_t;

typedef unsigned int ui;

static const char *progname = "runner";
static pthread_mutex_t group_lock;
static pthread_cond_t running_cond_var;

static void* runners_life(void *data)
{
    runner_t *runner = (runner_t *) data;
    group_t *group = runner->group;

    assert(runner && runner->group);

    while (runner->late < LATE_THRESHOLD
           && runner->lonely < LONELY_THRESHOLD) {

        runner->state = RUNNER_SLEEPING;
        (void) fprintf(stderr, "r%d sleeping\n", runner->id);
        /* not very random but good enough here */
        usleep((__useconds_t) (172000 + random() % 10000));

        /* add additional logic to model the runners here */
        pthread_mutex_lock(&group_lock);

        /* If a runner arrives late */
        if(group->state == GROUP_FINISHING || group->state == GROUP_RUNNING){
            pthread_mutex_unlock(&group_lock);
            runner->late++;
            (void) printf("r%d arrived late\n", runner->id);
            continue;
        }

        /* A new group starts assembling */
        (void) printf("r%d assembling for running\n", runner->id);
        if(group->arriving == 0){
            /* Set the group leader */
            runner->state = RUNNER_LEADING;
            group->running++;
            group->state = GROUP_ASSEMBLING;
            (void) printf("r%d is the leader of the group\n", runner->id);
        } else {
            group->running++;
            runner->state = RUNNER_ASSEMBLING;
        }
        pthread_mutex_unlock(&group_lock);

        if(runner->state == RUNNER_ASSEMBLING){
            pthread_mutex_lock(&group_lock);
            while(group->state != GROUP_RUNNING){
                pthread_cond_wait(&running_cond_var, &group_lock);
            }
            pthread_mutex_unlock(&group_lock);
        } else {

            /* the session leader is expected to wait for some time for
               additional runners to arrive and join the session */
            usleep((__useconds_t) (3600 + random() % 100));
            pthread_mutex_unlock(&group_lock); /* group starts running */
            if(group->running == 1) { /* If only one person is running */
                runner->lonely++;
                (void) printf("r%d is alone\n", runner->id);
            }
            group->state = GROUP_RUNNING;
            (void) printf("GROUP STARTS RUNNING!\n");
            pthread_mutex_unlock(&group_lock);
            pthread_cond_broadcast(&running_cond_var);
        }
        runner->state = RUNNER_RUNNING;
        (void) printf("r%d starts running\n", runner->id);

        pthread_mutex_lock(&group_lock);
        /* Group starts arriving (finishing the run) */
        if(group->running > 1){
            group->running--;
            group->arriving++;
            while(group->state != GROUP_FINISHING){
                pthread_cond_wait(&running_cond_var, &group_lock);
            }
            pthread_mutex_unlock(&group_lock);
        } else {
            group->running--;
            group->arriving++;
            group->state = GROUP_FINISHING;
            pthread_mutex_unlock(&group_lock);
            pthread_cond_broadcast(&running_cond_var);
        }

        pthread_mutex_lock(&group_lock);
        group->arriving--;
        if(group->arriving == 0){ /* Group has finished. Done. */
            group->state = GROUP_SLEEPING;
        }
        pthread_mutex_unlock(&group_lock);
        runner->state = RUNNER_WAITING;

        runner->runs++;
        (void) printf("r%d finishes running\n", runner->id);
    }


    return NULL;
}

int main(int argc, char *argv[])
{
    int c, n = 1;
    runner_t *runner = NULL;
    int rc = EXIT_SUCCESS;

    group_t group = {
            .state      = GROUP_SLEEPING,
            .arriving   = 0,
            .running    = 0,
    };

    while ((c = getopt(argc, argv, "n:h")) >= 0) {
        switch (c) {
            case 'n':
                if ((n = atoi(optarg)) <= 0) {
                    (void) fprintf(stderr, "number of runners must be > 0\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                (void) printf("Usage: %s [-n runners] [-h]\n", progname);
                exit(EXIT_SUCCESS);
            default:
                (void) printf("Usage %s [-n runners] [-h]\n", progname);
                exit(EXIT_SUCCESS);
        }
    }

    runner = calloc((size_t) n, sizeof(runner_t));
    if (! runner) {
        (void) fprintf(stderr, "%s: calloc() failed\n", progname);
        rc = EXIT_FAILURE;
        goto cleanup;
    }

    for (ui i = 0; i < n; i++) {
        runner[i].id = i;
        runner[i].state = RUNNER_SLEEPING;
        runner[i].late = 0;
        runner[i].runs = 0;
        runner[i].group = &group;

        rc = pthread_create(&runner[i].tid, NULL, runners_life, &runner[i]);
        if(rc){
            fprintf(stderr, "%s: %s: unable to create thread %d: %s\n", progname,
                    __func__, i, strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        (void)pthread_join(runner[i].tid, NULL);
    }

    for (int i = 0; i < n; i++) {
        (void) printf("r%d stopped after %3d run(s): "
                      "%3d run(s) missed, %3d run(s) alone\n",
                      runner[i].id, runner[i].runs,
                      runner[i].late, runner[i].lonely);
    }

    cleanup:

    if (runner) { free(runner); }

    return rc;
}