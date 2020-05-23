#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> //per tolower

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>

void * thread1_function(void * arg) {
    char * text = (char *) arg;
    int length  = strlen(text);
    int * counter = malloc(sizeof(int));
    *counter = 0;
    for(int i = 0; i < length; i++){
        if(text[i] == 'a' || text[i] == 'e' || text[i] == 'i' || text[i] == 'o' || text[i] == 'u')
            (*counter)++;
    }

    return *counter;
}

void * thread2_function(void * arg) {
    char * text = (char *) arg;
    int length  = strlen(text);
    int * counter = malloc(sizeof(int));
    *counter = 0;
    for(int i = 0; i < length; i++){
        if(text[i] != 'a' && text[i] != 'e' && text[i] != 'i' && text[i] != 'o' && text[i] != 'u' && text[i] != '\n' && text[i] != ' ')
            (*counter)++;
    }

    return *counter;
}

void * thread3_function(void * arg) {
    char * text = (char *) arg;
    int length  = strlen(text);
    int * counter = malloc(sizeof(int));
    *counter = 0;
    for(int i = 0; i < length; i++){
        if(text[i] == '\n' || text[i] == ' ')
            (*counter)++;
    }

    return *counter;
}

int main(int argc, char * argv[]) {
    int res;
    pthread_t t1,t2,t3;
    void * vocali;
    void * consonanti;
    void * others;

    char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";
    int length = strlen(content);

    //conversione a minuscolo per la stringa content
    char * lowered = malloc(length+1);
    if(lowered == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<length; i++) {
        lowered[i] = tolower(content[i]);
    }
    lowered[length]='\0';

    //creazione threads
    res = pthread_create(&t1, NULL, thread1_function, lowered);
    if(res != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&t2, NULL, thread2_function, lowered);
    if(res != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&t3, NULL, thread3_function, lowered);
    if(res != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    //attendo valori threads
    res = pthread_join(t1, &vocali);
    if(res != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(t2, &consonanti);
    if(res != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(t3, &others);
    if(res != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    printf("\nNumero di vocali: %d\nNumero di consonanti: %d\nNumero di spazi o a capo: %d\n", (int) vocali, (int) consonanti, (int) others);
    printf("\ncheck: %d  sum: %d", length, (int)vocali+(int)consonanti+(int)others);
    return 0;
}

