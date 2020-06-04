#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <unistd.h>
char * show (char**, int);
char * add (char**, int, char*, int);
char * del (char**, int, int);
char * search (char **, int, char*, int);
void itoa (int, char*);
void reverse (char*);
int kek (int);

void itoa(int n, char s[]) {
     int i, sign;
 
     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 int kek (int N) {
	int k = 1;
	while (N/10) { 
		N = N/10;
		k++;
	}
	return k;
}
char * show (char** text, int N) {
	char * answer;
	int cur_answer = 0;
	int i;
	answer = (char*)malloc(N*1024+1);
	for (i = 0; i < N; i++) {
		strcpy (&answer[cur_answer], text[i]);
		cur_answer = cur_answer + strlen(text[i])+1;
		answer[cur_answer-1] = '\n';
	}
	answer[cur_answer] = '\0';
	return answer;
}
char* add (char** text, int N, char * adding, int paragraph) {
	char * answer;
	int i;	
	answer = (char*)malloc(25);
	for (i = N; i >= paragraph; i--) {
		strcpy (text[i], (const char*)text[i-1]);
	}
	strcpy (text[paragraph-1], (const char*)adding);
	strcpy (answer, "Done\n");
	return answer;
}
char * search (char** text, int N, char * string, int depth) {
	char * answer;
	int i, j, length, number = 0, noch, ilength;
	int haha = depth;
	int cur_number = 1;
	int cur_answer = 0;
	bool numbers[1024];
	length = strlen (string);
	answer = (char*)malloc(N*1024);
	answer[cur_answer] = '\0';
	if (depth < strlen(string)) {
		strcpy (answer, "Incorrect Depth\n");
		return answer;
	}
	for (i = 0; i < N; i++) {
		ilength = strlen(text[i]);
		for (j = 0; j < ilength; j++){
			numbers[j] = false;
			if (strncmp(&text[i][j], string, length) == 0) {
				number++;
				numbers[j] = true;
			}
		}
		noch = 1;
		for (j = 0; j < ilength; j++){
			if (numbers[j]) {
				itoa (i+1, &answer[cur_answer]);
				cur_answer = cur_answer + kek(i+1)+1;
				answer[cur_answer-1] = ' ';
				itoa (number, &answer[cur_answer]);
				cur_answer = cur_answer + kek(number)+1;
				answer[cur_answer-1] = ' ';
				itoa (noch, &answer[cur_answer]);
				cur_answer = cur_answer + kek(number)+1;
				answer[cur_answer-1] = ' ';
				noch++;
				if ((j >= (depth-length)/2 + (depth - length)%2)&&((j + length + (depth-length)/2) <= ilength)) strncpy (&answer[cur_answer],&text[i][j-(depth-length)/2-(depth-length)%2], depth);
				if ((j < (depth-length)/2 + (depth - length)%2)&&((j + length + (depth-length)/2) <= ilength)) strncpy (&answer[cur_answer],&text[i][0],depth);
				if ((j >= (depth-length)/2 + (depth - length)%2)&&((j + length + (depth-length)/2) > ilength)) strncpy (&answer[cur_answer],&text[i][ilength-depth], depth);
				//if ((j < (depth-length)/2 + (depth - length)%2)&&((j + length + (depth-length)/2) > ilength)) {
				if (depth > ilength) {
					strncpy (&answer[cur_answer],&text[i][0], ilength);
					depth = ilength;
				}
				cur_answer = cur_answer+depth+1;	
				depth = haha;
				answer[cur_answer-1] = '\n';
			}		
		}
		number = 0;
	}
	answer[cur_answer] = '\0';
	return answer;
}



char * del (char ** text, int N, int paragraph) {
	char * answer;
	int i;
	answer = (char*)malloc(25);
	for (i = paragraph-1; i < N-1; i++) {
		strcpy (text[i], text[i+1]);
	}
	strcpy (answer, "Done\n");
	return answer;
}
int main(int argc, char *argv[])
{
    int as, ms;
    struct sockaddr_in server;
    char buf[1024]; 
	char* answer;
	int str_length;
	char c = '0';
	int i, j, Par_Num = 0, length; //number of paragraphs
	char ** text;
	int chislo;
	FILE* in;
    as = socket(AF_INET, SOCK_STREAM, 0 ); 

 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(1234); 


    bind(as, (struct sockaddr *) &server, sizeof(server));

    listen(as, 5); 
	
	in = fopen ("text.txt","r");
	while (c != EOF) {
		c = fgetc(in);
		if (c == '\n') Par_Num++;
	}
	fclose(in);
	in = fopen ("text.txt","r");
	text = (char**)malloc(Par_Num*sizeof(char*));
	if (text == NULL) {
		printf ("Mem Error\n");
		exit(1);
	}
	for (i = 0; i < Par_Num; i++) {
		text[i] = (char*)malloc(1026);
	}
	for (i = 0; i < Par_Num; i++) {
		fgets (text[i], 1024, in);
		length = strlen(text[i]);
		if (text[i][length-1] == '\n'||text[i][length-1] == 13) text[i][length-1] = '\0';
		if (text[i][length-2] == '\n'||text[i][length-2] == 13) text[i][length-2] = '\0';
	}
	fclose(in);
    while(1) {
		//ms = accept( as, 0, 0 );
        bzero( buf, sizeof(buf)); 
		gets (buf);
		/*if (readFromClient(ms, buf)<0){
			exit(1);
		}*/
		//read(ms, buf, sizeof(buf));
		//printf ("%s\n",buf);
        if (strcmp(buf, "quit") == 0 ) {
			answer = (char*)malloc(7);
			strcpy (answer, "Quit\n");
			puts (answer);
			free (answer);
			break;
		}
		if (strncmp(buf, "show", 4) == 0) answer = show(text, Par_Num);
		if (strncmp(buf, "add", 3) == 0) {
			i = 3;
			while (buf[i] < '0'&&buf[i]>'9') i++;
			chislo = atoi(&buf[i]);
			i = i + kek(chislo)+1;
			while (buf[i] == ' ') i++;
			if (chislo < 0||chislo > Par_Num+1) {
				answer = (char*)malloc(22);
				strcpy (answer, "Incorrect number\n");
			}
			else {
				text = (char**)realloc(text, (Par_Num+1)*sizeof(char*));
				text[Par_Num] = (char*)malloc(1026);
				answer = add (text, Par_Num, &buf[i], chislo);
				Par_Num++;
			}
		}
		if (strncmp(buf, "search", 6) == 0) {
			i = 6;
			while (buf[i] == ' ') i++;
			chislo = atoi(&buf[i]);
			i = i + kek(chislo) + 1;
			while (buf[i] == ' ') i++;
			answer = search (text, Par_Num, &buf[i], chislo);
		}
		if (strncmp(buf, "delete", 6) == 0) {
			i = 6;
			while (buf[i] < '0'&&buf[i]>'9') i++;
			chislo = atoi(&buf[i]);
			if (chislo < 0||chislo > Par_Num) {
				answer = (char*)malloc(22);
				strcpy (answer, "Incorrect number\n");
			}
			else {
				answer = del (text, Par_Num, chislo);
				free (text[Par_Num-1]);
				text = (char**)realloc(text, (Par_Num-1)*sizeof(char*));
				Par_Num--;
			}
		}
		if (answer == NULL) {
			answer = (char*)malloc(20);
			strcpy (answer, "Incorrect request\n");
		}
		//writeClient (ms, (const char*)answer);
		str_length = strlen(answer);
		//write(ms, answer, str_length);
		puts (answer);
		free (answer);
		//close (ms);
		answer = NULL;
    }
	// djldkjs
	in = fopen ("text.txt","w");
	for (i = 0; i < Par_Num; i++) {
		fprintf (in, "%s\n", text[i]);
		free(text[i]);
	}
	free(text);
	fclose(in);
    close(as);
    return 0;
}
