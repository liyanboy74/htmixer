/**
 * @file HTMixer.c
 * @author Liyanboy74 (https://github.com/liyanboy74/htmixer)
 * @brief  Static HTML Page Generator
 * @version v0.7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define SIZE_OF_BUFFER          0x1fffff
#define SIZE_OF_NAME            128
#define MAX_VAR_NUM             65535
#define MAX_INPUT_FILE          10

#define MAX_VAR_INTERDEPENDENT  5
#define MAX_NESTED_LOOP         3

#define VERSION_INFO "HTMixer V0.7"

const char *HELP_INFO = "\
---------------------------- HTMixer Help ----------------------------------\r\n\
HTMixer is a Static HTML page generator. This tool can combine different pages and add content to them. \
Content can be specific to one or more templates. \
This tool is written in C language and can be used for any file, but customized for Github Pages.\r\n\
More info at https://github.com/liyanboy74/htmixer .\r\n\r\n\
COMMANDS:                                                                   \r\n\
[FILE]          Output          Output file name                            \r\n\
                                Ex: HTMixer ./blog/index.html               \r\n\
                                                                            \r\n\
-v [FILE]       Var             Name of file contains variable              \r\n\
                                Ex: HTMixer -v home-text.txt blog-text.txt  \r\n\
                                                                            \r\n\
-d [FILE]       Doc             Name of file contains html,style or body    \r\n\
                                Ex: HTMixer -d home.html shop.html          \r\n\
                                                                            \r\n\
--version       Version         Print version info                          \r\n\
-l[0-3]         Set log level   Print more info while generating            \r\n\
                                Ex: HTMixer -l1                             \r\n\
                                Default set to 1                            \r\n\
                                                                            \r\n\
-?,-h,--help    Help            Print this list                             \r\n\
---------------------------------- END -------------------------------------";

typedef struct{
    char* buff;
    size_t len;
}my_buff_s;

typedef struct
{
    size_t size;
    char * loc;
    char name[SIZE_OF_NAME];
}my_var;

my_var var_list[MAX_VAR_NUM];
unsigned int var_c=0;
int print_info_level=1;

my_buff_s* my_buff_init()
{
    my_buff_s* buff;
    buff=(my_buff_s*)malloc(sizeof(my_buff_s));
    buff->len=0;
    buff->buff=(char*)malloc(SIZE_OF_BUFFER);
    return buff;
}

void my_buff_deinit(my_buff_s* buff)
{
    free(buff->buff);
    free(buff);
}

void print_var_list()
{
    int i,j;
    for(i=0;i<var_c;i++)
    {
        printf("%d: %s[%ld] -> ",i,var_list[i].name,var_list[i].size);
        for(j=0;j<var_list[i].size;j++)
        {
            putchar(var_list[i].loc[j]);
        }
        printf("\r\n");
    }
}

int search_var_list(char* Name)
{
    int i,j;
    for(i=0;i<var_c;i++)
    {
        if(strcmp(Name,var_list[i].name)==0)return i;
    }
    return -1;
}

size_t get_me_out(char*buff)
{
    size_t index=0;
    int d=1;
    while(d!=0)
    {
        if(buff[index]=='{'&&buff[index+1]=='{')
        {
            d++;
            index++;
        }
        else if(buff[index]=='}'&&buff[index+1]=='}')
        {
            d--;
            index++;
        }
        index++;
    }
    return index-2;
}

void catch_var_list(char * fileName)
{
    FILE *fp;
    size_t s,i,k;

    fp=fopen(fileName,"r");
    if(fp!=NULL)
    {
        my_buff_s* lbuff=my_buff_init();

        s=fread(lbuff->buff,1,SIZE_OF_BUFFER,fp);
        fclose(fp);

        for(i=0;i<s;i++)
        {
            if(lbuff->buff[i]=='{')
            {
                if(lbuff->buff[i+1]=='{')
                {
                    if(lbuff->buff[i+2]!='\r'&&lbuff->buff[i+2]!='\n'&&lbuff->buff[i+2]!=' '&&lbuff->buff[i+2]!='}')
                    {
                        i+=2;
                        for(k=0;lbuff->buff[i]!='\n'&&lbuff->buff[i]!='\r'&&lbuff->buff[i]!='\t'&&lbuff->buff[i]!=' '&&lbuff->buff[i]!='}';k++,i++)
                        {
                            var_list[var_c].name[k]=lbuff->buff[i];
                        }
                        var_list[var_c].name[k]='\0';

                        if(lbuff->buff[i]=='}')continue; // Empty Var
                        if(lbuff->buff[i]=='\r'||lbuff->buff[i]=='\n'||lbuff->buff[i]==' ')i++; //Jump

                        var_list[var_c].size=get_me_out(&lbuff->buff[i]);
                        var_list[var_c].loc=malloc(var_list[var_c].size);
                        memcpy(var_list[var_c].loc,&lbuff->buff[i],var_list[var_c].size);
                        var_c++;
                    }
                }
            }
        }
        my_buff_deinit(lbuff);
    }
    else if(print_info_level>0)printf("Can't open %s\r\n",fileName);
}

my_buff_s* replace_loop_counter_val(my_buff_s* buff)
{
    FILE *fp,*fpt;
    size_t s,i,k=0,j=0;

    char *p;
    my_buff_s* lbuff=my_buff_init();

    char var_name[SIZE_OF_NAME];
    int ss=-1;

    s=buff->len;

    while(p=strstr(buff->buff+k,"{{FOR({{"),p!=NULL)
    {
        p+=8;//6+2
        j=(p-2)-(buff->buff+k);
        memcpy(lbuff->buff+lbuff->len,buff->buff+k,j);
        lbuff->len+=j;
        k+=j;

        if(*p!='\r'&&*p!='\n'&&*p!='}'&&*p!=' ')
        {
            for(i=0;*p!='}';i++,p++)var_name[i]=*p;
            var_name[i]='\0';
            k+=(i+4);
            ss=search_var_list(var_name);
            if(ss!=-1)
            {
                memcpy(lbuff->buff+lbuff->len,var_list[ss].loc,var_list[ss].size);
                lbuff->len+=var_list[ss].size;
            }
        }
    }

    memcpy(lbuff->buff+lbuff->len,buff->buff+k,s-k);
    lbuff->len+=(s-k);

    lbuff->buff[lbuff->len]='\0';
    my_buff_deinit(buff);
    return lbuff;
}

my_buff_s* replace_var_list(my_buff_s* buff)
{
    size_t s,i,k=0,j=0;

    char *p;
    my_buff_s* lbuff=my_buff_init();
    char var_name[SIZE_OF_NAME];
    int ss=-1;

    s=buff->len;

    while(p=strstr(buff->buff+k,"{{"),p!=NULL)
    {
        j=p-(buff->buff+k);
        memcpy(lbuff->buff+lbuff->len,buff->buff+k,j);
        lbuff->len+=j;
        k+=j;

        p+=2;

        if(*p!='\r'&&*p!='\n'&&*p!='}'&&*p!=' ')
        {
            for(i=0;*p!='}';i++,p++)var_name[i]=*p;
            var_name[i]='\0';
            k+=(i+4);

            ss=search_var_list(var_name);
            if(ss!=-1)
            {
                memcpy(lbuff->buff+lbuff->len,var_list[ss].loc,var_list[ss].size);
                lbuff->len+=var_list[ss].size;
            }
        }
        else
        {
            memcpy(lbuff->buff+lbuff->len,p-2,2);
            lbuff->len+=2;
            k+=2;
        }
    }

    memcpy(lbuff->buff+lbuff->len,buff->buff+k,s-k);
    lbuff->len+=(s-k);

    lbuff->buff[lbuff->len]='\0';
    my_buff_deinit(buff);
    return lbuff;
}

my_buff_s* remove_space(my_buff_s* buff)
{
    size_t s,k=0,j=0;

    char *p;
    my_buff_s* lbuff=my_buff_init();
    char var_name[SIZE_OF_NAME];
    int ss=-1;

    s=buff->len;

    while(p=strstr(buff->buff+k,"{{"),p!=NULL)
    {
        j=p-(buff->buff+k);
        memcpy(lbuff->buff+lbuff->len,buff->buff+k,j);
        lbuff->len+=j;
        k+=j;

        if(p[2]==' ')k+=3;
        else k+=2;

        memcpy(lbuff->buff+lbuff->len,p,2);
        lbuff->len+=2;
    }

    memcpy(lbuff->buff+lbuff->len,buff->buff+k,s-k);
    lbuff->len+=(s-k);

    lbuff->buff[lbuff->len]='\0';
    my_buff_deinit(buff);
    return lbuff;
}

my_buff_s* cheack_loops(my_buff_s* buff)
{
    size_t h,l,i,s,t,k=0,j=0;

    char *p;
    my_buff_s* lbuff=my_buff_init();
    char var_name[SIZE_OF_NAME];
    int ss=-1;

    char CBuff[SIZE_OF_NAME];

    int loop_s=0,loop_e=0,loop_c=0,loop_p=0;
    char *loop_start_loc,*loop_end_loc;

    s=buff->len;

    while(p=strstr(buff->buff+k,"{{FOR("),p!=NULL)
    {
        j=p-(buff->buff+k);
        memcpy(lbuff->buff+lbuff->len,buff->buff+k,j);
        lbuff->len+=j;
        k+=j;

        for(j=0;*(p+j)!=')';j++);j++;
        k+=j;
        l=get_me_out(p+j);

        loop_start_loc=(char*)(p+j);
        loop_end_loc=(char*)(loop_start_loc+l);

        loop_p=0;
        sscanf(p+2,"FOR(%d,%d,%d)",&loop_s,&loop_e,&loop_p);

        if(print_info_level>2)printf("FOR Detected! - [%d] - [%d,%d,%d]\r\n",l,loop_s,loop_e,loop_p);

        for(loop_c=loop_s;loop_c!=loop_e;)
        {
            for(h=0;h<l;)
            {
                // Don't add loop counter at end of var name
                if(*(loop_start_loc+h)=='}'&&*(loop_start_loc+h+1)=='}'&&\
                *(loop_start_loc+h-1)!='}'&&*(loop_start_loc+h-1)!=' '&&*(loop_start_loc+h-1)!='\r'&&\
                *(loop_start_loc+h-1)!='\n'&&*(loop_start_loc+h-1)!='\t'&&*(loop_start_loc+h-1)!='>'&&\
                *(loop_start_loc+h-1)!='#')
                {
                    sprintf(CBuff,"-%d}}",loop_c);
                    t=strlen(CBuff);
                    memcpy(lbuff->buff+lbuff->len,CBuff,t);
                    lbuff->len+=t;
                    h+=2;
                }
                else
                {
                    memcpy(lbuff->buff+lbuff->len,loop_start_loc+h,1);
                    lbuff->len+=1;
                    h++;
                }
            }
            if(loop_p==0)loop_c++;
            else loop_c+=loop_p;
        }
        k+=(l+2);//Skip "}}"
    }

    memcpy(lbuff->buff+lbuff->len,buff->buff+k,s-k);
    lbuff->len+=(s-k);

    lbuff->buff[lbuff->len]='\0';
    my_buff_deinit(buff);
    return lbuff;
}

void clear_var_list()
{
    int i;
    for(i=0;i<var_c;i++)
    {
        free(var_list[i].loc);
    }
}

void cat_and_catch_files(my_buff_s *buff,char * fileName)
{
    FILE *fp;
    size_t s;

    if(fp=fopen(fileName,"r"),fp!=NULL)
    {
        while(s=fread((char*)(buff->buff+buff->len),1,SIZE_OF_BUFFER,fp),s>0)
        {
            buff->len+=s;
        }
        fclose(fp);
    }
}

int cont_slash(char *dir)
{
    int i,ret;
    for(i=0,ret=0;i<strlen(dir);i++)
    {
        if(dir[i]=='/' || dir[i]=='\\')ret++;
    }
    return ret;
}

int check_dir(char * path)
{
    int i,s,j,t;
    char dir[SIZE_OF_NAME];
    char temp;

    // File exist
    if(access(path,F_OK)==0) return 0;

    strcpy(dir,path);
    j=cont_slash(path);

    // File in folder
    if(j>1)
    {
        for(i=0,s=0;i<j;s++)
        {
            if(dir[s]=='/' || dir[s]=='\\')
            {

                i++;
                temp=dir[s];
                dir[s]='\0';
                mkdir(dir);
                dir[s]=temp;
            }
        }
        dir[s-1]='\0';
        if(print_info_level>2) printf("Make dir [%s]\r\n",dir);
    }
    return 1;
}

int check_files_details(char *genFileName,char *var,char var_c,char *doc,char doc_c)
{
    int i;
    struct stat filestat;
    time_t gTime=0,fTime=0,TTime=0;

    if(genFileName[0]=='\0')
    {
        if(print_info_level>0)printf("Error : Output file name not found!\r\n");
        return 1;
    }

    for(i=0;i<var_c;i++)
    {
        if(access(var+(i*SIZE_OF_NAME),F_OK)==0)
        {
            stat(var+(i*SIZE_OF_NAME),&filestat);
            TTime=filestat.st_mtime;
            if(TTime>fTime)fTime=TTime;
        }
        else
        {
            if(print_info_level>0)printf("Cant access to file [%s]\r\n",var+(i*SIZE_OF_NAME));
            return 3;
        }
    }

    for(i=0;i<doc_c;i++)
    {
        if(access(doc+(i*SIZE_OF_NAME),F_OK)==0)
        {
            stat(doc+(i*SIZE_OF_NAME),&filestat);
            TTime=filestat.st_mtime;
            if(TTime>fTime)fTime=TTime;
        }
        else
        {
            if(print_info_level>0)printf("Cant access to file [%s]\r\n",doc+(i*SIZE_OF_NAME));
            return 3;
        }
    }

    if(check_dir(genFileName)) return 0;

    stat(genFileName,&filestat);
    gTime=filestat.st_mtime;

    if(gTime>fTime)
    {
        if(print_info_level>0)printf("File [%s] Already up to date!\r\n",genFileName);
        return 2;
    }

    return 0;
}

int main(int argc,char* argv[])
{
    FILE *fp;
    int j,sel=0;

    char var_c=0;
    char doc_c=0;

    char var[MAX_INPUT_FILE][SIZE_OF_NAME];
    char doc[MAX_INPUT_FILE][SIZE_OF_NAME];

    char genFileName[SIZE_OF_NAME]="\0";

    my_buff_s* buff=my_buff_init();

    if(print_info_level>2)printf("Buff init OK - [%d]\r\n",buff->len);

    for(j=1;argc>j;j++)
    {
        if(strcmp(argv[j],"-v")==0)
        {
            sel=1;
        }
        else if(strcmp(argv[j],"-d")==0)
        {
            sel=2;
        }
        else if(strncmp(argv[j],"-l",2)==0)
        {
            print_info_level=argv[j][2]-'0';
            if(print_info_level>2)printf("Print info level set to %d \r\n",print_info_level);
        }
        else if(strcmp(argv[j],"--version")==0)
        {
            printf("%s\r\n",VERSION_INFO);
            return 0;
        }
        else if(strcmp(argv[j],"-?")==0 || strcmp(argv[j],"-h")==0||strcmp(argv[j],"--help")==0)
        {
            puts(HELP_INFO);
            return 0;
        }
        else
        {
            if(sel==0)
            {
                strcpy(genFileName,argv[j]);
            }
            else if(sel==1)
            {
                strcpy(var[var_c],argv[j]);
                var_c++;
            }
            else if(sel==2)
            {
                strcpy(doc[doc_c],argv[j]);
                doc_c++;
            }
        }
    }

    if(!check_files_details(genFileName,(char*)var,var_c,(char*)doc,doc_c))
    {
        for(j=0;j<var_c;j++)
        {
            catch_var_list(var[j]);
        }

        if(print_info_level>1) print_var_list();

        for(j=0;j<doc_c;j++)
        {
            cat_and_catch_files(buff,doc[j]);
        }

        if(print_info_level>2) printf("Cat Buff len is %d Byte.\r\n",buff->len);

        buff=replace_loop_counter_val(buff);

        for(j=0;j<MAX_NESTED_LOOP;j++)
        {
            buff=cheack_loops(buff);
        }

        for(j=0;j<MAX_VAR_INTERDEPENDENT;j++)
        {
            buff=replace_var_list(buff);
        }

        buff=remove_space(buff);

        fp=fopen(genFileName,"w");
        if(fp!=NULL)
        {
            fwrite(buff->buff,1,buff->len,fp);
            fclose(fp);
        }
        else if(print_info_level>0) printf("Can't open file [%s]",genFileName);

        clear_var_list();
    }

    my_buff_deinit(buff);
    return 0;
}
