#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mysql.h>


#define BUF_SIZE 255

const char *host = (char *)"127.0.0.1";
const char *user = (char *)"root";
const char *pw = (char *)"training";
const char *db = (char *)"test";

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	int i;
	char inputid[BUF_SIZE]="Input your ID.\n";
	char M_ID[BUF_SIZE]="";
	char inputstnum[BUF_SIZE]="Input your student No..\n";
	char STUNUM[BUF_SIZE]="";
	int option;
	FILE *fp;
	char s1[BUF_SIZE/2];
	char s2[BUF_SIZE/2];
	char s3[BUF_SIZE/2];
	char *ptr;
	MYSQL *connection;
	MYSQL_RES *sql_result;	
	MYSQL_ROW *sql_row;
	char query[BUF_SIZE];
	
	char query1 = "insert into goodbam set sound = ";
	char query2 = ", motion = ";
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	/*
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	option = 1;   
	if(serv_sock==-1)
		error_handling("socket() error");
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(9998);

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz=sizeof(clnt_adr);


	//mysql connection
	connection = mysql_init(NULL);
	if(mysql_real_connect(connection, host, user, pw, db, 3306, NULL, 0) == NULL)
	{
		printf("error : %d, %s\n", mysql_errno(connection), mysql_error(connection));
		close(serv_sock);
		return 1;	
	}


	for(i=0; i<1; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);
		memset(&query, 0, BUF_SIZE);		
		while(1)
		{
			write(clnt_sock, inputstnum, strlen(inputstnum)); //4. write
			memset(STUNUM, 0, sizeof(STUNUM));
			read(clnt_sock, STUNUM, BUF_SIZE); //5. read
			printf("%s\n", STUNUM);
			//string tokenizer
			ptr=strtok(STUNUM," ");
			strcpy(s1,ptr);
			ptr=strtok(NULL, " ");
			strcpy(s2, ptr);
			ptr=strtok(NULL," ");
			strcpy(s3,ptr);	
		//sql insert
			
			//query excution
			strcat(query, query1);
			strcat(query, s1);
			strcat(query, query2);
			strcat(query, s2);
			state = mysql_query(connection, query);
			if(!strcmp(s3, "end"))
				{break;}
		//last option -> switch to endl
		}
		fclose(fp);

		close(clnt_sock); //6. 
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
