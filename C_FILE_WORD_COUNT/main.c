#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

int* count_words(const char *filename, int *err_code, int N, ...){
    if(filename==NULL || N<1){
        if(err_code!=NULL) *err_code=1;
        return NULL;
    }
    FILE* f = fopen(filename, "r");
    if(f==NULL){
        if(err_code!=NULL) *err_code=2;
        return NULL;
    }
    int* word_count = calloc(N, sizeof(int));
    if(word_count==NULL){
        if(err_code!=NULL) *err_code=3;
        fclose(f);
        return NULL;
    }
    va_list args;
    va_start(args, N);
    //long position;
    int sign;
    char* hldr;
    int count=0;
    for(int i=0; i<N; i++) {
        hldr = va_arg(args, char*);

        while (1){
            for(int y=0; y< (int)strlen(hldr); y++){
                sign= fgetc(f);
                if(feof(f)) break;
                if(sign!=*(hldr+y)){
                    while (sign!=' ' && sign!='\n' && sign!='"' && sign!='-' && sign!='~' && sign!='\''){
                        sign = fgetc(f);
                        if(feof(f)) break;
                    }
                    y=-1;
                }
                if(y==(int)strlen(hldr)-1){
                    sign = fgetc(f);
                    if(sign==' ' || sign=='\'' || sign=='\n' || sign=='.'   || sign==',' || sign=='-' || sign==':')
                    count++;
                }
            }
            if(feof(f)) break;
        }

        fseek(f, 0, SEEK_SET);
        *(word_count+i)=count;
        count=0;
    }
    va_end(args);
    fclose(f);
    if(err_code!=NULL) *err_code=0;
    return word_count;
}

int main(int argc, char** argv) {
    if(argc<3){
        printf("Not enough arguments");
        return 9;
    }

    int err;
    int* tab;
    for(int i=2; i<argc; i++){
        tab = count_words(*(argv+1), &err, 1, *(argv+i));
        if(err==2){
            printf("Couldn't open file");
            return 4;
        }
        if(err==3){
            printf("Failed to allocate memory");
            return 8;
        }
        printf("%d\n", *tab);
        free(tab);
    }

    return 0;
}
