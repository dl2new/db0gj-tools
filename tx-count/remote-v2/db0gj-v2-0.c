/* DB0GJ Aktivitaetszaehler */
/* Neue Version direkt am Relais */
/* Compilieren: gcc -Wall -o db0gj-vx db0gj-vx.c -l bcm2835 */
/* V2.0 23.12.2017 DL2NEW

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <bcm2835.h>

/* Pin11=Gpio17  Pin22=Gpio25 */
#define TAST RPI_GPIO_P1_11
#define LED RPI_GPIO_P1_22

#define CNT "/home/pi/db0gj/db0gj.cnt"
#define LOG "/home/pi/db0gj/db0gj-log.txt"
#define CROSS "/home/pi/db0gj/db0gj-cross.html"
#define INDEX "/home/pi/db0gj/index.html"

#define DEBUG

int main(int argc, char **argv)
{
	uint8_t val, blink;
	uint16_t cnt, z1, z2, z3, total_qso, max_qso;
	char timebuf[30];
	char timebuf2[30];
	char* timetmp;
	char* p;
	FILE *fp1;
	FILE *fp2;
	time_t now, starttime;
	int zeichen, crosscnt;
	
	/* Init GPIO */
	if (!bcm2835_init()) return 1;
	bcm2835_gpio_fsel(TAST, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(LED, BCM2835_GPIO_FSEL_OUTP);
	
	/* Scanner initialisieren */
	if(argc == 2)
	{
		if(!strcmp(argv[1], "-i"))
		{
			fp1 = fopen (CNT, "w");
			if(fp1 == NULL)
			{
				printf("Datei CNT konnte nicht geoeffnet werden.\n");
				return(-1);
			}
			else
			{
				now = time(0);
			
				fprintf(fp1, "%ld\n%hu\n%hu\n%hu\n%hu\n%hu\n", now,0,0,0,0,0);
				fclose(fp1);
				#ifdef DEBUG
				printf("Init CNT:\n%ld\n%hu\n%hu\n%hu\n%hu\n%hu\n", now,0,0,0,0,0);
				#endif
			}
		
			fp1 = fopen (LOG, "w");
			if(fp1 == NULL)
			{
				printf("Datei LOG konnte nicht geoeffnet werden.\n");
				return(-1);
			}
			else
			{
				now = time(0);
			
				/* Start: \n am Ende der Zeit entfernen */
				timetmp = ctime(&now);
				strcpy(timebuf, timetmp);
				p = strchr(timebuf, '\n');
				if(p)
				{
					*p = '\0';
				}
				/* End: \n am Ende der Zeit entfernen */
			
				fprintf(fp1, "%s Neustart\n", timebuf);
				fclose(fp1);
				#ifdef DEBUG
				printf("Init LOG: %s Neustart\n", timebuf);
				#endif
			}
		}
	}
	
	/* Zaehler aus Datei einlesen */
	fp1 = fopen (CNT, "r");
	if(fp1 == NULL)
	{
		printf("Datei konnte nicht geoeffnet werden.\n");
	}
	else
	{
		fscanf(fp1, "%ld\n%hu\n%hu\n%hu\n%hu\n%hu\n", &starttime, &z1, &z2, &z3, &total_qso, &max_qso);
		fclose(fp1);
		#ifdef DEBUG
		printf("Eingelesene Werte: %ld %hu %hu %hu %hu %hu\n", starttime, z1, z2, z3, total_qso, max_qso);
		printf("Starttime %s\n", ctime(&starttime));
		#endif
	}
	
	/* Loggen: Scanner gestartet */
	fp1 = fopen (LOG, "a");
	if(fp1 == NULL)
	{
		printf("Datei LOG konnte nicht geoeffnet werden.\n");
		return(-1);
	}
	else
	{
		now = time(0);
		
		/* Start: \n am Ende der Zeit entfernen */
		timetmp = ctime(&now);
		strcpy(timebuf, timetmp);
		p = strchr(timebuf, '\n');
		if(p)
		{
			*p = '\0';
		}
		/* End: \n am Ende der Zeit entfernen */
		
		fprintf(fp1, "%s DB0GJ-Scanner gestartet\n", timebuf);
		fclose(fp1);
		#ifdef DEBUG
		printf("%s DB0GJ-Scanner gestartet\n", timebuf);
		#endif
	}
	
	val = 0;
	blink = 0;
	
	/* Start Dauerschleife */
	while(1)
	{
		if(blink == 0)
		{
			bcm2835_gpio_write(LED, 0);
			blink = 1;
		}
		else
		{
			bcm2835_gpio_write(LED, 1);
			blink = 0;
		}
		
		/* Relais offen? */
		val = bcm2835_gpio_lev(TAST);
		if(val == 1)
		{
			cnt = 0;
			/* Schleife läuft, solange Relais offen ist */
			while(bcm2835_gpio_lev(TAST))
			{
				#ifdef DEBUG
				printf("Empfang \n");
				#endif
				bcm2835_gpio_write(LED, 1);
				bcm2835_delay(250);
				bcm2835_gpio_write(LED, 0);
				bcm2835_delay(250);
				bcm2835_gpio_write(LED, 1);
				bcm2835_delay(250);
				bcm2835_gpio_write(LED, 0);
				bcm2835_delay(250);
				cnt += 1;
			}
			/* Relais ist abgefallen */
			#ifdef DEBUG
			printf("Sekunden: %d\n", cnt);
			#endif
			bcm2835_gpio_write(LED, 0);
			if(cnt < 5)
			{
				/* nur Stoerung */
				z1 += 1;
			}
			else
			{
				if(cnt < 25)
				{
					/* Oeffnung ohne QSO */
					z2 += 1;
					
					/* Loggen: Oeffnung ohne QSO */
					fp1 = fopen (LOG, "a");
					if(fp1 == NULL)
					{
						printf("Datei LOG konnte nicht geoeffnet werden.\n");
						return(-1);
					}
					else
					{
						now = time(0);
		
						/* Start: \n am Ende der Zeit entfernen */
						timetmp = ctime(&now);
						strcpy(timebuf, timetmp);
						p = strchr(timebuf, '\n');
						if(p)
						{
							*p = '\0';
						}
						/* End: \n am Ende der Zeit entfernen */
		
						fprintf(fp1, "%s Relais geoeffnet ohne QSO: %hu %hu %hu %hu %hu \n", timebuf, z1,z2,z3,total_qso,max_qso);
						fclose(fp1);
						#ifdef DEBUG
						printf("%s Relais geoeffnet ohne QSO: %hu %hu %hu %hu %hu \n", timebuf, z1,z2,z3,total_qso,max_qso);
						#endif
					}
					
				}
				else
				{
					/* Oeffnung mit QSO */
					z3 += 1;
					total_qso = total_qso + cnt;
					if(max_qso < cnt) max_qso = cnt;
					
					/* Loggen: Oeffnung mit QSO */
					fp1 = fopen (LOG, "a");
					if(fp1 == NULL)
					{
						printf("Datei LOG konnte nicht geoeffnet werden.\n");
						return(-1);
					}
					else
					{
						now = time(0);
		
						/* Start: \n am Ende der Zeit entfernen */
						timetmp = ctime(&now);
						strcpy(timebuf, timetmp);
						p = strchr(timebuf, '\n');
						if(p)
						{
							*p = '\0';
						}
						/* End: \n am Ende der Zeit entfernen */
		
						fprintf(fp1, "%s Relais geoeffnet mit QSO: %hu %hu %hu %hu %hu \n", timebuf, z1,z2,z3,total_qso,max_qso);
						fclose(fp1);
						#ifdef DEBUG
						printf("%s Relais geoeffnet mit QSO: %hu %hu %hu %hu %hu \n", timebuf, z1,z2,z3,total_qso,max_qso);
						#endif
					}
					
				}
			}
			
			/* Counter schreiben */
			fp1 = fopen (CNT, "w");
			if(fp1 == NULL)
			{
				printf("Datei CNT konnte nicht geoeffnet werden.\n");
				return(-1);
			}
			else
			{
				fprintf(fp1, "%ld\n%hu\n%hu\n%hu\n%hu\n%hu\n", starttime,z1,z2,z3,total_qso,max_qso);
				fclose(fp1);
				#ifdef DEBUG
				printf("%ld %hu %hu %hu %hu %hu\n", starttime,z1,z2,z3,total_qso,max_qso);
				#endif
			}
			
			/* Aktuelle html-Datei erzeugen */
			#ifdef DEBUG
			printf("Schreibe index.html\n");
			#endif
			fp1 = fopen(CROSS, "r");
			fp2 = fopen(INDEX, "w");
			if(fp1 == NULL || fp2 == NULL)
			{
				printf("Fehler bei fopen CROSS-INDEX");
				exit(0);
			}
			
			crosscnt = 0;
			
			/* Start: \n am Ende der Zeit entfernen */
			timetmp = ctime(&starttime);
			strcpy(timebuf2, timetmp);
			p = strchr(timebuf2, '\n');
			if(p)
			{
				*p = '\0';
			}
			/* End: \n am Ende der Zeit entfernen */

			while((zeichen=getc(fp1)) != EOF)
			{
				if(zeichen=='+')
				{
					crosscnt += 1;
					
					if(crosscnt==1) fprintf(fp2, "%hu", z2);
					if(crosscnt==2) fprintf(fp2, "%hu", z3);
					if(crosscnt==3) fprintf(fp2, "%hu", total_qso);
					if(crosscnt==4) fprintf(fp2, "%hu", max_qso);
					if(crosscnt==5) fprintf(fp2, "%s", timebuf2);
					if(crosscnt==6) fprintf(fp2, "%s", timebuf);
				}
				else
					putc(zeichen, fp2);
			}
			
			fclose(fp1);
			fclose(fp2);
			/* END Aktuelle html-Datei erzeugen */

		}
		else
		{
			#ifdef DEBUG
			printf("Warten auf Signal \n");
			#endif
			bcm2835_delay(1000);
		}
	}
	return 0;
}
