#include <stdlib.h>

#include "multilevelQueueScheduler.h"


int min( int x, int y );

//Steps to promotiion to be used later
static const int STEPS_TO_PROMOTION = 50;
static const int FOREGROUND_QUEUE_STEPS = 5;
static const int MAX_ARRAY_SIZE_RETURN = 50;

//This runs but is showing unused in Clion? Why?
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("David Hieger YPK967\n");
    printf("N/A");
}
schedule* createSchedule( ) {
    Queue* q1 =  createQueue();
    Queue* q2 =  createQueue();
    schedule* result = calloc(1, sizeof (schedule));
    result->foreQueue = q1;
    result->backQueue = q2;
    result->totalTick = 0;
    result->completedTask =0;
    return  result;
}
bool isScheduleUnfinished( schedule *ps ) {
    return  !(isEmpty(ps->foreQueue) & isEmpty(ps->backQueue));
}
void addNewProcessToSchedule( schedule *ps, char *processName, priority p ) {
    process* target = calloc(1,sizeof (process));
    target->processData = initializeProcessData(processName);
    target->processName = processName;
    target->processPriority = p;
    target->startTickCount = ps->totalTick;
    target->spentTickCount =0;
    enqueue(p==BACKGROUND ?  ps->backQueue : ps->foreQueue,target);
}
char* runNextProcessInSchedule( schedule *ps ) {
    int numSteps;
    attemptPromote( ps );
    numSteps = (!isEmpty(ps->foreQueue)) ? FOREGROUND_QUEUE_STEPS : INT_MAX;
    char** buffer = calloc(MAX_ARRAY_SIZE_RETURN,sizeof (char*));
    for (int i = 0; i < MAX_ARRAY_SIZE_RETURN; ++i) {
        buffer[i] = calloc(1, sizeof (char) );
    }
    queueType process;
    process = (!isEmpty(ps->foreQueue)) ?  dequeue(ps->foreQueue)  :  getNext(ps->backQueue);
    loadProcessData(process->processData);
    bool done =   runProcess(process->processName, buffer , &numSteps);
    if (done){
        if (process->processPriority == BACKGROUND){
            process =   dequeue(ps->backQueue);
        }
        freeProcessData();
        free(process->processName);
        ps->completedTask++;
    }else if (process->processPriority == FOREGROUND){
        enqueue(ps->foreQueue,process);
    }
    ps->totalTick+=numSteps;
    return buffer[0];
}

void attemptPromote( schedule *ps ) {
    LLNode*  old = NULL;
    LLNode* head = ps->backQueue->qFront;
    while (head != NULL){
        if(ps->totalTick - head->qt->startTickCount >= 50){
            promoteProcess(head->qt->processName,head->qt->processData);
            enqueue(ps->foreQueue,head->qt);
            if (old == NULL){
                ps->backQueue->qFront = head->pNext;
            } else{
                old->pNext = head->pNext;
            }
        }
        head = head->pNext;
    }
    if (ps->backQueue->qFront == NULL){
        ps->backQueue->qRear = NULL;
    }
    if (ps->backQueue->qRear == NULL){
        ps->backQueue->qFront = NULL;
    }
}
void printSummary( schedule *ps ) {
    printf("The number of time steps : %ld\n", ps->totalTick);
    printf("The number of processes that completed running : %ld\n", ps->completedTask);
    if (ps->completedTask == 0){
        printf("The average time taken per process : NAN");
    }else{
        printf("The average time taken per process : %.3f\n",  (double) ps->totalTick / (double) ps->completedTask);

    }
}
void freeSchedule( schedule *ps ) {
    freeQueue(ps->backQueue);
    freeQueue(ps->foreQueue);
    free(ps);
}
