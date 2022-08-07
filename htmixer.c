/*
 HTMixer
 By Liyanboy74
 https://github.com/liyanboy74/htmixer
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_BUFFER 0x1fffff
#define SIZE_OF_NAME 128
#define MAX_VAR_NUM  65535
#define MAX_INPUT_FILE 10

typedef struct
{
    size_t size;
    char * loc;
    char name[SIZE_OF_NAME];
}my_var;

my_var var_list[MAX_VAR_NUM];
unsigned int var_c=0;

char *buff;

void print_var_list()
{
    int i,j;
    for(i=0;i<var_c;i++)
    {
        printf("%d: %s[%ld]\r\n",i,var_list[i].name,var_list[i].size);
        for(j=0;j<var_list[i].size;j++)
        {
            putchar(var_list[i].loc[j]);
        }
        puts("\r\n");
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
    char *buff;
    size_t s,i,k;

    buff=(char*)malloc(SIZE_OF_BUFFER);
    if(buff!=NULL)
    {
        fp=fopen(fileName,"r");
        s=fread(buff,1,SIZE_OF_BUFFER,fp);
        for(i=0;i<s;i++)
        {
            if(buff[i]=='{'&& buff[i+1]=='{')
            {
                if(buff[i+2]!='\r'&&buff[i+2]!='\n'&&buff[i+2]!=' '&&buff[i+2]!='}')
                {
                    i+=2;
                    for(k=0;buff[i]!='\n'&&buff[i]!='\r'&&buff[i]!='\t'&&buff[i]!=' '&&buff[i]!='}';k++,i++)
                    {
                        var_list[var_c].name[k]=buff[i];
                    }
                    if(buff[i]!='}')i++;
                    if(buff[i]=='\r'||buff[i]=='\n')i++;
                    var_list[var_c].name[k]='\0';

                    var_list[var_c].size=get_me_out(&buff[i]);
                    var_list[var_c].loc=malloc(var_list[var_c].size);
                    memcpy(var_list[var_c].loc,&buff[i],var_list[var_c].size);
                    var_c++;
                }
            }
        }
        fclose(fp);
        free(buff);
    }
}

void replace_var_list(char * FileName)
{
    FILE *fp,*fpt;
    size_t s,i,k;

    char var_name[SIZE_OF_NAME];
    int ss=-1;

    char Tempfile[]="rvl.tmp";

    fp=fopen(FileName,"r");
    fpt=fopen(Tempfile,"w");

    s=fread(buff,1,SIZE_OF_BUFFER,fp);
    for(i=0;i<s;i++)
    {
        if(buff[i]=='{'&& buff[i+1]=='{')
        {
            if(buff[i+2]!='\r'&&buff[i+2]!='\n'&&buff[i+2]!='}'&&buff[i+2]!=' ')
            {
                i+=2;
                for(k=0;buff[i]!='}';k++,i++)
                {
                    var_name[k]=buff[i];
                }
                var_name[k]='\0';
                i++;

                ss=search_var_list(var_name);
                if(ss!=-1)
                {
                    fwrite(var_list[ss].loc,var_list[ss].size,1,fpt);
                }
            }
            else
            {
                fwrite(&buff[i],1,1,fpt);
            }
        }
        else
        {
            fwrite(&buff[i],1,1,fpt);
        }
    }
    fclose(fp);
    fclose(fpt);
    remove(FileName);
    rename(Tempfile,FileName);
}

void replace_loop_counter_val(char * FileName)
{
    FILE *fp,*fpt;
    size_t s,i=0,k=0,j=0;

    char *p;

    char var_name[SIZE_OF_NAME];
    int ss=-1;

    char Tempfile[]="rlc.tmp";

    fp=fopen(FileName,"r");
    fpt=fopen(Tempfile,"w");

    s=fread(buff,1,SIZE_OF_BUFFER,fp);

    while(p=strstr(buff+k,"{{FOR({{"),p!=NULL)
    {
        p+=8;//6+2
        j=(p-2)-(buff+k);
        fwrite(buff+k,1,j,fpt);
        k+=j;

        for(i=0;*p!='}';i++,p++)var_name[i]=*p;
        var_name[i]='\0';
        k+=(i+4);

        ss=search_var_list(var_name);
        if(ss!=-1)
        {
            fwrite(var_list[ss].loc,var_list[ss].size,1,fpt);
        }
    }

    fwrite(buff+k,1,s-k,fpt);

    fclose(fp);
    fclose(fpt);
    remove(FileName);
    rename(Tempfile,FileName);
}

void latest_replace_var_list(char * FileName)
{
    FILE *fp,*fpt;
    size_t s,i,k;

    char var_name[SIZE_OF_NAME];
    int ss=-1;

    char TempFile[]="lrvl.tmp";

    fp=fopen(FileName,"r");
    fpt=fopen(TempFile,"w");

    s=fread(buff,1,SIZE_OF_BUFFER,fp);
    for(i=0;i<s;i++)
    {
        if(buff[i]=='{'&& buff[i+1]=='{')
        {
            if(buff[i+2]!='\r'&&buff[i+2]!='\n'&&buff[i+2]!='}')
            {
                if(buff[i+2]!=' ')
                {
                    i+=2;
                    for(k=0;buff[i]!='}';k++,i++)
                    {
                        var_name[k]=buff[i];
                    }
                    var_name[k]='\0';
                    i++;

                    ss=search_var_list(var_name);
                    if(ss!=-1)
                    {
                        fwrite(var_list[ss].loc,var_list[ss].size,1,fpt);
                    }
                }
                else//jump from space
                {
                    fwrite(&buff[i],1,2,fpt);
                    i+=2;
                }
            }
            else
            {
                fwrite(&buff[i],1,1,fpt);
            }
        }
        else
        {
            fwrite(&buff[i],1,1,fpt);
        }
    }
    fclose(fp);
    fclose(fpt);
    remove(FileName);
    rename(TempFile,FileName);
}

void cheack_loops(char * FileName)
{
    FILE *fp,*fpt;
    size_t s,i,k,l;

    int loop_s=0,loop_e=0,loop_c=0,loop_p=0;
    size_t loop_start_loc,loop_end_loc;
    size_t len=0;

    char CBuff[SIZE_OF_NAME];
    int for_f=0,for_c=0;

    char TempFile[]="cl.tmp";

    fp=fopen(FileName,"r");
    fpt=fopen(TempFile,"w");

    s=fread(buff,1,SIZE_OF_BUFFER,fp);

    for(i=0;i<s;i++)
    {
        if(buff[i]=='{'&& buff[i+1]=='{')
        {
            i+=2;
            //FOR Detector
            if(strncmp(&buff[i],"FOR(",4)==0)
            {
                //Optional 'loop_p' , Default ++
                sscanf(&buff[i],"FOR(%d,%d,%d)",&loop_s,&loop_e,&loop_p);
                while(buff[i++]!=')');
                len=get_me_out(&buff[i]);
                loop_start_loc=i;
                loop_end_loc=i+len;
                //FOR
                for(loop_c=loop_s;loop_c!=loop_e;for_f=0,for_c=0)
                {
                    for(k=0;k<len;k++)
                    {
                        if(buff[i+k]=='{'&&buff[i+k+1]=='{')
                        {
                            if(strncmp(&buff[i+k+2],"FOR(",4)==0)
                            {
                                //Internal FOR Detect
                                for_f=1;
                            }
                            else
                            {
                                //Internal Var of FOR
                                if(for_f)for_c++;
                            }
                            fwrite(&buff[i+k],1,2,fpt);
                            k+=1;
                        }
                        else if(buff[i+k]=='}'&&buff[i+k+1]=='}')
                        {
                            //Add Counter for Internal Var
                            if(for_c)
                            {
                                for_c--;
                                sprintf(CBuff,"-%d}}",loop_c);
                            }
                            //Close Internal FOR
                            else if(for_f)
                            {
                                for_f--;
                                strcpy(CBuff,"}}");
                            }
                            //Add Counter for Var
                            else
                            {
                                sprintf(CBuff,"-%d}}",loop_c);
                            }
                            l=strlen(CBuff);
                            fwrite(CBuff,l,1,fpt);
                            k+=1;
                        }
                        else fwrite(&buff[i+k],1,1,fpt);
                    }

                    if(loop_p==0)loop_c++;
                    else loop_c+=loop_p;
                }
                i=loop_end_loc+2;
                loop_p=0;
            }
            else
            {
                fwrite(&buff[(i-2)],1,(size_t)3,fpt);
            }
        }
        else
        {
            fwrite(&buff[i],1,1,fpt);
        }
    }
    fclose(fp);
    fclose(fpt);
    remove(FileName);
    rename(TempFile,FileName);
}

void clear_var_list()
{
    int i;
    for(i=0;i<var_c;i++)
    {
        free(var_list[i].loc);
    }
}

void cat_files(FILE *fpt,char * fileName)
{
    FILE *fp;
    size_t s;

    if(fp=fopen(fileName,"r"),fp!=NULL)
    {
        while (s=fread(buff,1,SIZE_OF_BUFFER,fp),s>0)
        {
            fwrite(buff,1,s,fpt);
        }
        fclose(fp);
    }
}

int main(int argc,char* argv[])
{
    FILE *fp,*fpt;
    int j,sel=0;

    char var_c=0;
    char doc_c=0;

    char var[MAX_INPUT_FILE][SIZE_OF_NAME];
    char doc[MAX_INPUT_FILE][SIZE_OF_NAME];

    char genFileName[SIZE_OF_NAME]="\0";

    buff=(char*)malloc(SIZE_OF_BUFFER);
    if (buff==NULL)return 1;

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

    if(genFileName[0]!='\0')
    {
        for(j=0;j<var_c;j++)
        {
            catch_var_list(var[j]);
        }
        //print_var_list();

        if(fpt=fopen(genFileName,"w"),fpt!=NULL)
        {
            for(j=0;j<doc_c;j++)
            {
                cat_files(fpt,doc[j]);
            }
            fclose(fpt);
        }

        replace_loop_counter_val(genFileName);

        for(j=0;j<2;j++)
        {
            cheack_loops(genFileName);
        }

        for(j=0;j<5;j++)
        {
            replace_var_list(genFileName);
        }

        latest_replace_var_list(genFileName);

        clear_var_list();
    }

    free(buff);
    return 0;
}
