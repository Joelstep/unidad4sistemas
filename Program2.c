#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

struct threadParam_t
{
    mqd_t mq;
};

void *Hablar(void *parg)
{
    struct threadParam_t *pargTmp = (struct threadParam_t *)parg;

    //mqd_t mq;

    char palabras[64];

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    pargTmp->mq = mq_open("/mq1", O_RDONLY | O_CREAT, 0644, &attr);

    while (1)
    {
        mq_receive(pargTmp->mq, palabras, 32, NULL);
        //fgets(palabras, sizeof(palabras), mq);
        printf("Imprimiendo: %s\n", palabras);
        if (strncmp(palabras, "exit", strlen("exit")) == 0)
        {
            break;
        }
        /*if(palabras, "dejar", strlen("dejar")) == 0)
        {

        }*/
    }

    //mq_close(pargTmp->mq);
    mq_unlink("/mq0");

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t threadID1;

    /*FILE *archivo = fopen("chat.txt", "+a");

    if (archivo == NULL)
    {
        perror("error");
        return (EXIT_FAILURE);
    }*/
    mqd_t mq = mq_open("/mq0", O_WRONLY);
    struct threadParam_t threadParam1 = {mq};

    char almacen[64];

    pthread_create(&threadID1, NULL, &Hablar, &threadParam1);

    while (1)
    {
        fgets(almacen, sizeof(almacen), stdin);
        //fputs(almacen, mq);

        if (almacen[strlen(almacen) - 1] == '\n')
            almacen[strlen(almacen) - 1] = 0;
        mq_send(mq, almacen, strlen(almacen) + 1, 0);

        if (strncmp(almacen, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }

    pthread_join(threadID1, NULL);

    printf("%s", almacen);

    mq_close(mq);

    exit(EXIT_SUCCESS);
}