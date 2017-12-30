/* DB0GJ Aktivitaetszaehler */
/* Neue Version direkt am Relais */
/* Signalerfassung, Log in Datei schreiben, Zaehler in Datei sichern */
/* Programmstart mit -i: Zaehler werden auf Null gesetzt */
/* Compilieren: gcc -Wall -o db0gj-vx db0gj-vx.c -l bcm2835 */
/* V2.0 27.12.2017 DL2NEW */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <bcm2835.h>

/* Fotodiode: Pin11=Gpio17 und Pin1=3,3V - Spitze an +, Platte an Pin11 */
/* LED:       Pin22=Gpio25 und Pin6=GND  - langes Bein an Pin22, kurzes an GND, Vorwiderstand 330Ohm an + */
#define TAST RPI_GPIO_P1_11
#define LED RPI_GPIO_P1_22

#define CNT "/home/pi/db0gj/db0gj.cnt"
#define LOG "/home/pi/db0gj/db0gj-log.txt"

#define DEBUG

int main(int argc, char **argv)
{
	/* val = Rueckgabewert GPIO, blink = LED blinken */ 
	unsigned short val, blink;

	/* cnt = Sekunden Relais offen bei einem Durchgang */
	/* z1 = Anzahl Stoerungen */
	/* z2 = Anzahl Oeffnungen ohne QSO */
	/* z3 = Anzahl Oeffnungen mit QSO */
	/* max_qso = laengstes Einzel-QSO in Sekunden */
	unsigned int cnt, z1, z2, z3, max_qso;

	/* total_qso = Gesamtsekunden Relais offen */
	unsigned long total_qso;

	char timebuf[30];
	char* timetmp;
	char* p;
	FILE *fp1;
	time_t now, starttime;
	
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
			
				fprintf(fp1, "%ld\n%u\n%u\n%u\n%u\n%u\n", now,0,0,0,0,0);
				fclose(fp1);
				#ifdef DEBUG
				printf("Init CNT:\n%ld\n%u\n%u\n%u\n%u\n%u\n", now,0,0,0,0,0);
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
		fscanf(fp1, "%ld\n%u\n%u\n%u\n%lu\n%u\n", &starttime, &z1, &z2, &z3, &total_qso, &max_qso);
		fclose(fp1);
		#ifdef DEBUG
		printf("Eingelesene Werte: %ld %u %u %u %lu %u\n", starttime, z1, z2, z3, total_qso, max_qso);
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
		
						fprintf(fp1, "%s Relais geoeffnet ohne QSO: %u %u %u %lu %u \n", timebuf, z1,z2,z3,total_qso,max_qso);
						fclose(fp1);
						#ifdef DEBUG
						printf("%s Relais geoeffnet ohne QSO: %u %u %u %lu %u \n", timebuf, z1,z2,z3,total_qso,max_qso);
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
		
						fprintf(fp1, "%s Relais geoeffnet mit QSO: %u %u %u %lu %u \n", timebuf, z1,z2,z3,total_qso,max_qso);
						fclose(fp1);
						#ifdef DEBUG
						printf("%s Relais geoeffnet mit QSO: %u %u %u %lu %u \n", timebuf, z1,z2,z3,total_qso,max_qso);
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
				fprintf(fp1, "%ld\n%u\n%u\n%u\n%lu\n%u\n", starttime,z1,z2,z3,total_qso,max_qso);
				fclose(fp1);
				#ifdef DEBUG
				printf("%ld %u %u %u %lu %u\n", starttime,z1,z2,z3,total_qso,max_qso);
				#endif
			}
			
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
