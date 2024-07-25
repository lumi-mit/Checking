#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include <time.h>
#include <stdbool.h>

void clean_buffer()
{
	char tampon;
	while((tampon=getchar())!='\n');
}

void pause()
{
	clean_buffer();
	getchar();
}

int get_number(FILE *file)
{
	int number=0;
	char letter;
	rewind(file);
	while((letter=fgetc(file))!=EOF)
	{
		if(letter=='\n')
		{
			number++;
		}
	}
	return (number);
}

FILE *open_file(char *filename,char *mode)
{
	FILE *fic;
	fic=fopen(filename,mode);;
	if(fic==NULL)
	{
		printf("Failure opening %s \n",filename);
	}
	return (fic);
}

int name_count(FILE *file)
{
	int number=0;
	char letter;
	while((letter=fgetc(file))!=';')
	{
		number++;
	}
	fseek(file,-number,SEEK_CUR);
	return (number+1);
}

int firstname_count(FILE *file)
{

	int number=0;
	char letter;
	while((letter=fgetc(file))!='\n')
	{
		number++;
	}
	fseek(file,-number,SEEK_CUR);
	return (number+1);
}

Etudiant *get_student(FILE *file,FILE *fic,int student_number)
{
	Etudiant *student;
	int i,nbr_name;
	student=(Etudiant*)malloc(student_number*sizeof(Etudiant));
	rewind(file);
	for(i=0;i<student_number;i++)
	{
		nbr_name=name_count(file);
		student[i].nom=(char*)malloc	(nbr_name*sizeof(char));
		fscanf(file,"%[^;]",student[i].nom);
		nbr_name=firstname_count(file);
		student[i].prenom=(char*)malloc(nbr_name*sizeof(char));
		fscanf(file,"%[^\n]",student[i].prenom);
		student[i].check=3;
    }
	return (student);
}

int already_check(FILE *fic,int number,char *Date_d)
{
	int line_date;
	char texte[80];
	if(!date_repeat(fic,&line_date,Date_d))
	{
		move_cur(fic,number+line_date);
		fgets(texte,80,fic);	
		if(strstr(texte,"OUI")||strstr(texte,"NON")||strstr(texte,"PERSO")||strstr(texte,"ABS"))
		{
			printf("Student already check \nPress [Enter]: \n");
			pause();
			return (1);
		}
	}
	return (0);
}

int machine(Etudiant *l1,FILE *fic,int *start,int student_number,char *Date_d)
{ 
	int number=0,choix,i,j=0;
	char letter;
	char choice[50];
	system("clear");
	printf(	"+===========================================================================================+");
		printf(	"\n|[%s]                            STUDENT CHECKING                             |\n"
			"+===========================================================================================+",Date_d);
	printf("\n--->Enter student's number or name [exit <0>]: ");
	label:
	scanf(" %s",choice);
	if(atoi(choice)==0&&strlen(choice)<3&&choice[0]<65)
	{
		*start=0;
	}
	else if(strlen(choice)<3&&choice[0]<65)
	{
		number=atoi(choice);
		if(number > student_number)
		{
			printf("Student number max is : %d \nTry again: ",student_number);
			goto label;
		}
		else
		{
			twist:
			if(number==0)
			{
				printf("Student not found\nRetry:");
				goto label;
			}
		    else if(!already_check(fic,number-1,Date_d)&&number!=0)
			{
				number--;
				printf(	"\n+===========================================================================================+\n"
						"|   Choose  with border [number] to check:                                                  |\n"
						"|                 ~> NON                   [0]                                              |\n"	
						"|                 ~> OUI                   [1]                                              |\n"
						"|                 ~> PERSONNEL             [2]                                              |\n"
						"|                                                                                           |\n"
						"+===========================================================================================+\n");
				printf("===>|%s %s     | >[number]: ",l1[number].nom,l1[number].prenom);
				t:
				scanf(" %d",&choix);
				if(choix<0||choix>2)
				{
					printf("\tInvalid Choice,Retry: ");
					goto t;
				}
				switch(choix)
				{
					case NON:
						l1[number].check=NON;
						break;
					case OUI:
						l1[number].check=OUI;
						break;
					case PERSONNEL:
						l1[number].check=PERSONNEL;
						break;
					default:
						break;
				}
				printf("\tCheck successful .....\n\tPress [Enter]:");
				pause();
				return (number);
			}
		}
	}
	else
	{
		for(i=0;i<student_number;i++)
		{
			while(j<student_number)
			{
				lower(l1[j].nom);
				j++;
			}
			if(strstr(lower(l1[i].nom),lower(choice))||strstr(lower(l1[i].prenom),lower(choice)))
			{
				clean_buffer();
				printf(	"+===========================================================================================+\n");
				printf("=>| %s %s |",l1[i].nom,l1[i].prenom);
				printf("\tIt's you [y/n] ? >>");
				again:
				scanf("%c",&letter);
				if(letter=='n'||letter=='N')
				{
					continue;
				}
				else if(letter=='y'||letter=='Y'||letter=='o'||letter=='O')
				{
					number=i+1;
					goto twist;
					break;
				}
				else
				{
					printf("Invalid choice,Retry [y/n]:");
					goto again;
				}
				printf(	"\n+===========================================================================================+\n");
			}
		}
		printf(	" ==>Student not in database: %s  "
				"\n+===========================================================================================+\n"
				"\nPress any button: ",choice);
		pause();
	}
	return (-1);
}

int date_repeat(FILE *fic,int *line_date,char *Date_d)
{
	char test[DIM];
	rewind(fic);
	int line=0;
	while(!feof(fic))
	{
		*line_date=line;
		fgets(test,DIM,fic);
		if(strstr(test,Date_d))
		{
				return (0);
		}
		line++;
	}
	return (1);
}

void move_cur(FILE *fic,int pos)
{
	int i=-3;
	char line[DIM];
	rewind(fic);
	while(i!=pos)
	{
		fgets(line,DIM,fic);
		i++;
	}
}

void print_row(FILE* fic,Etudiant *l1,int i,char *Date_h)
{
	switch(l1[i].check)
	{
		case NON:
			fprintf(fic,"%02d |%-30s |%-30s\t|  NON  | %s  |\n",i+1,l1[i].nom,l1[i].prenom,Date_h);
			break;
		case OUI:
			fprintf(fic,"%02d |%-30s |%-30s\t|  OUI  | %s  |\n",i+1,l1[i].nom,l1[i].prenom,Date_h);
			break;
		case PERSONNEL:
			fprintf(fic,"%02d |%-30s |%-30s\t| PERSO | %s  |\n",i+1,l1[i].nom,l1[i].prenom,Date_h);
			break;
		default:
			fprintf(fic,"%02d |%-30s |%-30s\t|       |           |\n",i+1,l1[i].nom,l1[i].prenom);
			break;
	}
}

void print_file(FILE *fic,Etudiant *l1,int student_number,int numero,char *Date_h,char *Date_d)
{
	int i,line_date=0;
	if(date_repeat(fic,&line_date,Date_d))
	{
		fseek(fic,0,SEEK_END);
		fprintf(fic,"\n\n\t\t\t\t====> %s <====\n",Date_d);
		fprintf(fic,"N° |              NOM              |               PRENOM               |MACHINE|   HEURE   |\n");
		fprintf(fic,"---|-------------------------------|------------------------------------|-------|-----------|\n");
		for(i=0;i<student_number;i++)
		{
			print_row(fic,l1,i,Date_h);
		}	
	}
	else
	{
		if(numero>-1)
		{
			move_cur(fic,numero+line_date);
			print_row(fic,l1,numero,Date_h);
		}
	}
}

void search_student(FILE *fic,int *start,Etudiant *l1,int student_number,char *Date_d)  
{
	char name[50],test[DIM],iname[150];
	char letter;
	int i=0,j=0,k;
	int oui=0,non=0,perso=0,abs=0;
	rewind(fic);
	system("clear");
	printf(	"+===========================================================================================+");
	printf(	"\n|[%s]                            SEARCH CHECKING                              |\n"
		"+===========================================================================================+\n",Date_d);
	printf("-->Enter name or number  [exit<0>]: ");
	scanf(" %[^\n]",name);
	printf(	"\n+===========================================================================================+\n");
	if(strcmp(name,"0")==0)
	{
		*start=0;
	}
	else
	{
		while(j<student_number)
		{
			lower(l1[j].nom);
			j++;
		}
		j=0;
		while(j<student_number)
		{
			if( j+1==atoi(name) || strstr(lower(l1[j].nom),lower(name)) ||  strstr(lower(l1[j].prenom),lower(name)))
			{	
				i++;
				k=j;	
				printf(" %02d | %-30s |%-30s \n",j+1,l1[j].nom,l1[j].prenom);
			}
			j++;
		}
		printf(	"+===========================================================================================+\n");
		if(i>1)
		{
			printf("Choose the number : ");
			clean_buffer();
			scanf("%d",&k);
			k--;	
			i=1;
		}
		if(i==1)
		{
			printf("\n\tName         : %s\n\tFirstname    : %s \n\tN°           : %d \nSTAT:\n"
					"\t\t\t+--------------------------------------+\n"
					"\t\t\t|       DATE       |MACHINE|    HOUR   |\n"
					"\t\t\t|------------------|-------|-----------|\n",l1[k].nom,l1[k].prenom,k+1);	
			rewind(fic);
			while(!feof(fic))
			{ 
				fgets(test,DIM,fic);
				if(strstr(test,"====>"))
				{
					sscanf(test,"				====> %[^<]",iname);
					printf("\t\t\t| %s",iname);
				}
				if(strstr(test,l1[k].prenom))
				{
					if(strstr(test,"|  OUI  |"))
					{
						oui++;
					}
					else if(strstr(test,"|  NON  |"))
					{
						non++;
					}
					else if(strstr(test,"| PERSO |"))
					{
						perso++;
					}
					else if(strstr(test,"|       |"))
					{
						abs++;
					}
					fseek(fic,-22,SEEK_CUR);
					while((letter=fgetc(fic))!='\n')
					{
						printf("%c",letter);		
					}
					printf("\n");
				}
			}
			printf("\t\t\t+--------------------------------------+\n");
			printf(	"\n\t\t\t+-----------------------------+\n"
					"\t\t\t|    CHECK-PC   |    Number   |\n"
					"\t\t\t+-----------------------------+\n");
			printf(	"\t\t\t|      OUI      |     %02d      |\n",oui);
			printf(	"\t\t\t|      NON      |     %02d      |\n",non);
			printf(	"\t\t\t|      PERSO    |     %02d      |\n",perso);
			printf(	"\t\t\t|      ABSENT   |     %02d      |\n",abs);
			printf(	"\t\t\t+-----------------------------+\n");
			printf(	"+===========================================================================================+");
		}
		if(i==0)
		{
			printf("Failed to search student  : %s \n",name);
		}
	}
	if(strcmp(name,"0")!=0)
	{
		printf("\nPress [Enter] to continue: ");
		pause();
	}	
}

void search_date(FILE *fic,int *start,Etudiant *l1,char *Date_d)
{
	char test[DIM],letter;
	int i=0,j,a=0,date,voir;
	system("clear");
	rewind(fic);
	printf(	"+===========================================================================================+");
	printf(	"\n|[%s]                            SEARCH DATE                                  |\n"
		"+===========================================================================================+\n",Date_d);
	printf(	"|                                                                                           |\n");
	while(!feof(fic))
	{
		fgets(test,DIM,fic);
		if(strstr(test,"====>"))
		{
			i++;
			j=0;
			a=0;
			printf("|                    ~>[");
			while(test[j]!='<')
			{
				j++;
				if(a==1&&test[j]!='<')
				{
					printf("%c",test[j]);
				}
				if(test[j]=='>')
				{
					a=1;	
				}
			}
			printf("]-----------------[%02d]                            |\n",i);
		}
	}
	printf(	"|                                                                                           |"
			"\n+===========================================================================================+\n");
	printf("-->Choose a date [x] or [exit<0>]: ");
	make:
	scanf("%d",&date);
	if(date<0||date >i)
	{
		printf("Invalid choice,Retry: ");
		goto make;
	}
	rewind(fic);
	i=0;
	if(date==0)
	{
		*start=0;
	}
	else
	{
		voir=1;
		while(voir)
		{
			display_voir(fic,date,&voir,l1);
		}
	}
	if(date!=0)
	{
		printf("Press [Enter] :");
		pause();		
	}
}

void update_HTML(FILE *fic)
{
	FILE *file;
	char letter;
	int i=0;
	file=open_file("file.html","w");
	rewind(fic);
	fprintf(file,	"<HTML>\n"
    				"\t<HEAD>\n"
        			"\t\t<TITLE>\n"
         			"\t\t\tCHECK MACHINE\n"
        			"\t\t</TITLE>\n"
   					"\t</HEAD>\n"
					"\t<BODY BACKGROUND=\"ressource/background.JPG\">\n"
					"\t\t<FONT SIZE=\"6\"  COLOR=\"red\"><CENTER> ETUDIANT MIT LICENCE 1 </CENTER></FONT> <BR>");
	fprintf(file,"<CENTER><TABLE  BGCOLOR=\"#E0FFFF\" BORDER=\"1\">");
	while((letter=fgetc(fic))!=EOF)
	{	
		if(letter=='\n')
		{
			fprintf(file,"\t\t\t\t</TD>\n\t\t\t</TR>\n\t\t\t<TR>\n\t\t\t\t<TD>");
		}
		if(letter=='|'||letter=='<'||letter=='>')
		{
			fprintf(file,"\t\t\t\t</TD>\n\t\t\t\t<TD>");
		}
		if(letter=='<')
		{
			fprintf(fic,"<BR>");
		}
		if(letter!='='&&letter!='<'&&letter!='>'&&letter!='|')
		{
			fprintf(file,"%c",letter);
		}
	}
	fprintf(file,"</TABLE></CENTER>");
	fprintf(file,	"\t</BODY>\n"
					"</HTML>");
	printf("Update success....\nPress [Enter] to continue: \n");
	pause();
	close_file(file);
}

void menu(FILE *file,FILE *fic,Etudiant *l1,int student_number)
{
	int choix,open=OUI,numero,start;
	char Date_h[30],Date_d[30];
	time_t rawtime;
	time(&rawtime);
	tm *local=localtime(&rawtime);
	sprintf(Date_h,"%02d:%02d:%02d",local->tm_hour,local->tm_min,local->tm_sec);
	sprintf(Date_d,"%02d // %02d // %2d",local->tm_mday,local->tm_mon+1,local->tm_year+1900);
	rewind(fic);
	while(open)
	{
		start=1;
		system("clear");
		printf(	"+===========================================================================================+");
		printf(	"\n|[%s]                            MAIN MENU                                    |\n",Date_d);
		printf(	"+===========================================================================================+\n"
				"|                                                                                           |\n"
				"|             ~>Checking a student-----------------------------------------[1]              |\n"
				"|             ~>Search for student data------------------------------------[2]              |\n"
				"|             ~>Search Date------------------------------------------------[3]              |\n"
				"|             ~>Update HTML file-------------------------------------------[4]              |\n"          
				"|             ~>Quit-------------------------------------------------------[5]              |\n"
				"|                                                                                           |\n"
				"+===========================================================================================+");
		printf("\n       --> Make  a  choice [x]: ");
		label1:
		scanf("%d",&choix);
		if(choix > 5||choix <0)
		{
			printf("Invalid Choice\nTry again: ");
			goto label1;
		}
		switch(choix)
		{
			case 1:
				while(start)
				{	
					numero=machine(l1,fic,&start,student_number,Date_d);
					print_file(fic,l1,student_number,numero,Date_h,Date_d);
				}
				break;
			case 2:
				while(start)
				{
					search_student(fic,&start,l1,student_number,Date_d);
				}
				break;
			case 3:
				while(start)
				{
					search_date(fic,&start,l1,Date_d);
				}
				break;
			case 4:
				update_HTML(fic);
				break;
			case 5:
				open=NON;
				break;
			default: 
				break;
		}
	}
}

void free_student(Etudiant *l1,int student_number)
{
	int i;
	for(i=0;i<student_number;i++)
	{
		free(l1[i].nom);
		free (l1[i].prenom);
	}
}

void close_file(FILE *file)
{
	if(file!=NULL)
	{
		fclose(file);	
	}
}

char *lower(char *sentence)
{
	char *phrase;
	int i;
	phrase=(char*)malloc(strlen(sentence)*sizeof(char));
	for(i=0;i<strlen(sentence);i++)
	{	if(sentence[i]<97)
		{
			phrase[i]=sentence[i]+32;
		}
		else{
			phrase[i]=sentence[i];
		}
	}
	return (phrase);
}

void display_voir(FILE* fic,int date,int *voir,Etudiant *l1)
{
	int search_datee ;
	printf(	"+===========================================================================================+\n");
	printf(	"|                                                                                           |\n"
			"|           ~>Display check \"OUI\"     :-----------------------[1]                           |\n"
			"|           ~>Display check \"NON\"     :-----------------------[2]                           |\n"
			"|           ~>Display check \"ABSENT\"  :-----------------------[3]                           |\n"
			"|           ~>Display check \"PERSO\"   :-----------------------[4]                           |\n"
			"|           ~>Display ALL List        :-----------------------[5]                           |\n"
			"|                                                                                           |\n");
	printf(	"+===========================================================================================+\n");
	printf("==> Make a choice [x]  or [exit<0>]: ");
	scanf("%d",&search_datee);
	printf(	"+===========================================================================================+\n");
	if(search_datee==0)
	{
		*voir=0;
	}
	else
	{
		switch(search_datee)
		{
			case VOUI:
				check_value(l1,fic,"|  OUI  |",date);
				break;

			case VNON:
				check_value(l1,fic,"|  NON  |",date);
				break;

			case VABSENT:
				check_value(l1,fic,"|       |",date);
				break;
			case VPERSONNEL:
				check_value(l1,fic,"| PERSO |",date);
				break;
			case LIST:
				chose_date(fic,date);
				break;
			default:
				break;
		}
		if(date!=0)
		{
			printf("Press [Enter] to continue: ");
			pause();
		}
	}
}

void check_value(Etudiant *l1,FILE *fic,char *value,int date)
{
	int i=0,j=0,number=0;
	char test[100];
	char letter;
	rewind(fic);
	printf("     +---------------------------------------------------------------------------------+\n"
		"     | N°|                NAME            |            FIRSTNAME           |  HOUR     |\n"
			"     +---------------------------------------------------------------------------------+\n");
	while(!feof(fic))
	{
		fgets(test,DIM,fic);
		if(strstr(test,"====>"))
		{
			i++;
			if(i==date)
			{
				while(j<80)
				{
					j++;
					fgets(test,DIM,fic);
					if(strstr(test,value))
					{
						printf("     |%02d | %-30s | %-30s",j-2,l1[j-3].nom,l1[j-3].prenom);
						fseek(fic,-15,SEEK_CUR);
						while((letter=fgetc(fic))!='\n')
						{
							printf("%c",letter);
						}
						printf("\n");
						number++;		
					}
				}
			}
		}
	}
	printf("     +---------------------------------------------------------------------------------+\n");
	printf("\tStudent Number : %d / 77 \n\tPercentage     : %g%% \n",number,(float)number*(float)100/(float)77);
	printf(	"+===========================================================================================+\n");
}

void chose_date(FILE *fic,int date)
{
	char test[100];
	rewind(fic);
	int i=0,j;
	int oui=0,non=0,perso=0,abs=0;
	while(!feof(fic))
	{
		fgets(test,DIM,fic);
		if(strstr(test,"====>"))
		{
			i++;
			if(i==date)
			{
				j=0;
				printf("%s",test);
				printf("--------------------------------------------------------------------------------------------|\n");
				while(j<79)
				{
					j++;
					fgets(test,DIM,fic);
					printf("%s",test);
					if(strstr(test,"|  OUI  |"))
					{
						oui++;
					}
					else if(strstr(test,"|  NON  |"))
					{
						non++;
					}
					else if(strstr(test,"| PERSO |"))
					{
						perso++;
					}
					else if(strstr(test,"|       |"))
					{
						abs++;
					}
				}
				printf("--------------------------------------------------------------------------------------------|\n");
			}
		}
	}
	printf("STAT :\n");
	printf(	"\n\t\t\t+------------------------------------------------+\n"
			"\t\t\t|    CHECK-PC   |   Student   |    Percentage    |\n"
			"\t\t\t+------------------------------------------------+\n");
	printf(	"\t\t\t|      OUI      |     %02d      |       %02d%%        |\n",oui,oui*100/77);
	printf(	"\t\t\t|      NON      |     %02d      |       %02d%%        |\n",non,non*100/77);
	printf(	"\t\t\t|      PERSO    |     %02d      |       %02d%%        |\n",perso,perso*100/77);
	printf(	"\t\t\t|      ABSENT   |     %02d      |       %02d%%        |\n",abs,abs*100/77);
	printf(	"\t\t\t+------------------------------------------------+\n");
	printf(	"+===========================================================================================+\n");
}
