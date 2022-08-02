#include "header.h"
#include "BufferFunctions.h"

int sequencia;
int last_seq;
int retorna_tipo(unsigned char buffer[],int *args_ls,unsigned char dir[]){

    int i=0;
    int a=0;
    int l=0;
    unsigned char cmd[7];
    unsigned char arg1[63];
    unsigned char arg2[6];
    sscanf(buffer,"%s %s %s",cmd,arg1,arg2);
    if(!strncmp(cmd,"ls",3)){
        if(!strncmp("-a",arg1,3))
            a=1;
        if(!strncmp("-l",arg1,3))
            l=1;
        if(!strncmp("-a",arg2,3))
            a=1;
        if(!strncmp("-l",arg2,3))
            l=1;
        if(a==1 && l==0)
            *args_ls=1;
        if(a==0 && l==1)
            *args_ls=2;
        if(a==1 && l==1)
            *args_ls=3;
        return LSL;
    }
    if(!strncmp("rls",cmd,3)){
        if(!strncmp("-a",arg1,3))
            a=1;
        if(!strncmp("-l",arg1,3))
            l=1;
        if(!strncmp("-a",arg2,3))
            a=1;
        if(!strncmp("-l",arg2,3))
            l=1;
        if(a==1 && l==0)
            *args_ls=1;
        if(a==0 && l==1)
            *args_ls=2;
        if(a==1 && l==1)
            *args_ls=3;
        return LS;
    }
    if(!strncmp("cd",cmd,3)){
        strncpy(dir,arg1,63);
        return CDL;
    }
    if(!strncmp("rcd",cmd,3)){
        strncpy(dir,arg1,63);
        return CD;
    }
    if(!strncmp("mkdir",cmd,6)){
        strncpy(dir,arg1,63);
        return MKDIRL;
    }
    if(!strncmp("rmkdir",cmd,6)){
        strncpy(dir,arg1,63);
        return MKDIR;
    }
    else
        return 0;
}
//enp7s0f0

int main(int argc, char const *argv[])
{
    int batata=0;
    unsigned char *buffer = (unsigned char *) malloc(BYTES); //to receive data
    memset(buffer,0,BYTES);
    int args_ls=0;
    int buflen;
    int send_len= ConexaoRawSocket("lo");
    unsigned char input[76];
    unsigned char dir[63];
    limpa_string(input,76);
    limpa_string(dir,63);
    int tipo;
    char cwd[PATH_MAX];
    sequencia=0;
    last_seq=-1;
    if(send_len<0)
    {
        printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
        return -1;
    }
    system("clear");
    //while(1){
        limpa_string(input,76);
        limpa_string(dir,63);
        printf("Cliente:%s$ ",getcwd(cwd, sizeof(cwd)));
        fgets(input,76,stdin);
        tipo=retorna_tipo(input,&args_ls,dir);
        limpa_string(input,76);
        switch (tipo)
        {
            case CDL:
                chdir(dir);
                break;
            case LSL:
            {
                if(args_ls==0)
                    system("ls");
                if(args_ls==1)
                    system("ls -a");
                if(args_ls==2)
                    system("ls -l");
                if(args_ls==3)
                    system("ls -a -l");
                args_ls=0;
                break;
            }
            case MKDIRL:
                mkdir(dir,0700);
                break;
            case LS:
                input[0]=args_ls;
                constroi_buffer(send_len,sequencia,input,tipo);
                args_ls=0;
                break;
            case CD:
                constroi_buffer(send_len,sequencia,dir,tipo);
                limpa_string(dir,63);
                while(1)
                {buflen=recvfrom(send_len,buffer,BYTES,0,NULL,0);
                if(buflen<0){
                    printf("error in reading recvfrom function\n");
                    return -1;
                }
                DesmontaBuffer(buffer,dir,&batata,&last_seq);}
                break;
            case MKDIR:
                constroi_buffer(send_len,sequencia,dir,tipo);
                break;
            default:
                constroi_buffer(send_len,sequencia,input,tipo);
                break;
        }
        sequencia++;
        if (sequencia==8)
            sequencia=0;
    //}





  return 0;
}