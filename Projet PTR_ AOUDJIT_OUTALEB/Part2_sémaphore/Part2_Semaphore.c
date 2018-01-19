#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...

				//gcc -Wall Part2_Semaphore.c -lpthread // compiler
				// ./a.out 1> 1_Sem.txt  // executer

sem_t * sem_sens;
sem_t * sem_sensInverse;

sem_t * semtrain1;
sem_t * semtrain2;
sem_t * semtrain3;

pthread_cond_t voie_libre = PTHREAD_COND_INITIALIZER;

char compar1[3][2];
char compar2[3][2];
char RaZ[1][2]={{'\0','\0'}};

char data1[] = {'A','B','C'};
char data_i1[] = {'C','B','A'};
char data2[] = {'A','B','D','C'};
char data_i2[] = {'C','B','A'};
char data_i3[] = {'C','E','A'};

clock_t t1d[30], t2d[30],t3d[30];
clock_t t1a[30], t2a[30],t3a[30];


pthread_mutex_t fifo  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifo1  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifo2  = PTHREAD_MUTEX_INITIALIZER;

void* sens(void *arg) {
	int id = *((int*) arg);
	int i;
	
	for (i = 0; i < 2; i++) {

		if ((id==0)){
		printf("le Train %d  veut s'engager sur la voie  %c%c\n", id+1,data1[i],data1[i+1]);
        	fflush(stdout);
		compar1[id][0]=data1[i]; //
		compar1[id][1]=data1[i+1];// enregistrer la voie dans compar1
		}

		if (id==1){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data2[i],data2[i+1]);		
        	fflush(stdout);
		compar1[id][0]=data2[i];
		compar1[id][1]=data2[i+1];// enregistrer la voie dans compar1
		}
		
		if (id==2){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);
		compar1[id][0]=data2[i];
		compar1[id][1]=data2[i+1];// enregistrer la voie dans compar1
		}
		
		
			// Train1 			
		if (id==0){
		   		// si un train est sur cette voie dans le sens opposé 
		       if ((compar1[id][0]== compar2[id+1][0])&&(compar1[id][1]== compar2[id+1][1])){

				sleep(1);			    
			sem_wait(semtrain1);	// on bloque ce train (1)		
			}
			else if ((compar1[id][0]== compar2[id+2][0])&&(compar1[id][1]== compar2[id+2][1])){
					sleep(1);	
			     sem_wait(semtrain1);// on bloque ce train (1)

			}	
					sleep(1);
			printf("le Train %d  est sur la voie  %c%c\n", id+1,data1[i],data1[i+1]);
			fflush(stdout);
		}
			// Train2 
		if (id==1){
		   		// si un train est sur cette voie dans le sens opposé 
			if ((compar1[id][0]== compar2[id-1][0])&&(compar1[id][1]== compar2[id-1][1])){

				sleep(1);			
			     sem_wait(semtrain2);// on bloque ce train (2)	
			}
			else if((compar1[id][0]== compar2[id+1][0])&&(compar1[id][1]== compar2[id+1][1])){	

				sleep(1);
				sem_wait(semtrain2);// on bloque ce train (2)	
			}
					sleep(1);
			printf("le Train %d  est sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
			fflush(stdout);
		}
			// Train3 
		if (id==2){
		   		// si un train est sur cette voie dans le sens opposé 			
			if ((compar1[id][0]== compar2[id-1][0])&&(compar1[id][1]== compar2[id-1][1])){
						sleep(1);
				sem_wait(semtrain3); // on bloque ce train (3)
			}
			 if((compar1[id][0]== compar2[id-2][0])&&(compar1[id][1]== compar2[id-2][1])){	
						sleep(1);
				sem_wait(semtrain3); // on bloque ce train (3)
			}
		printf("le Train %d  est sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);        	
		}

		sleep(rand() % 2);
		
		 
		if (id==0){
		
			printf("le Train %d  sort de la voie  %c%c\n", id+1,data1[i],data1[i+1]);
			fflush(stdout);
			sleep(1);
		   		// si un train est sur cette voie dans le sens opposé 
			 if ((compar1[id][0]== compar2[id+1][0])&&(compar1[id][1]== compar2[id+1][1])){
	
				sleep(1);				
				sem_post(semtrain2); //(après que ce train sort) on débloque le train opposé
			}
			else if ((compar1[id][0]== compar2[id+2][0])&&(compar1[id][1]== compar2[id+2][1])){	
				sem_post(semtrain3); //(après que ce train sort) on débloque le train opposé

			}	

			compar1[id][0]=RaZ[0][0];
			compar1[id][1]=RaZ[0][0];

		}

		if (id==1){
			printf("le Train %d  sort de la voie %c%c\n", id+1,data2[i],data2[i+1]);
        		fflush(stdout);
			sleep(1);
		   		// si un train est sur cette voie dans le sens opposé 
			if ((compar1[id][0]== compar2[id-1][0])&&(compar1[id][1]== compar2[id-1][1])){

				sleep(1);				
			     	sem_post(semtrain1);	//(après que ce train sort) on débloque le train opposé
			}
			else if((compar1[id][0]== compar2[id+1][0])&&(compar1[id][1]== compar2[id+1][1])){	
				sleep(1);				
			     	sem_post(semtrain3);	//(après que ce train sort) on débloque le train opposé
			}

		
			compar1[id][0]=RaZ[0][0];
			compar1[id][1]=RaZ[0][0];

		}

		if (id==2){
			printf("le Train %d  sort de la voie %c%c\n", id+1,data2[i],data2[i+1]);
			fflush(stdout);
		   		// si un train est sur cette voie dans le sens opposé 
			if ((compar1[id][0]== compar2[id-1][0])&&(compar1[id][1]== compar2[id-1][1])){
				sleep(1);				
			     	sem_post(semtrain2);	//(après que ce train sort) on débloque le train opposé
			}
			 if((compar1[id][0]== compar2[id-2][0])&&(compar1[id][1]== compar2[id-2][1])){	
				sleep(1);				
			     	sem_post(semtrain1);	//(après que ce train sort) on débloque le train opposé
			}


			compar1[id][0]=RaZ[0][0];
			compar1[id][1]=RaZ[0][0];

		}
		
		sleep(rand() % 3); 
	
	}
		
	printf("le train %d : fini un sens\n", id+1);
       	fflush(stdout);
    return NULL;
}

void* sens_inverse(void *arg) {
	int id = *((int*) arg);
	int i;
	

	for (i = 0; i < 2; i++) {
		if (id==0){
		printf("le Train %d  veut s'engager sur la voie  %c%c\n", id+1,data_i1[i],data_i1[i+1]);
        	fflush(stdout);
		compar2[id][0]=data_i1[i+1];
		compar2[id][1]=data_i1[i]; // enregistrer la voie dans compar2
		}

		if (id==1){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
        	fflush(stdout);
		compar2[id][0]=data_i2[i+1];
		compar2[id][1]=data_i2[i]; // enregistrer la voie dans compar2
		}

		if (id==2){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
        	fflush(stdout);
		compar2[id][0]=data_i3[i+1];
		compar2[id][1]=data_i3[i]; // enregistrer la voie dans compar2
		}
		
    		
			// Train1 		
		if (id==0){
			// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
			if ((compar2[id][0]== compar1[id+1][0])&&(compar2[id][1]== compar1[id+1][1])){
						sleep(1);
				sem_wait(semtrain1); // on bloque ce train1
			}
			 else if ((compar2[id][0]== compar1[id+2][0])&&(compar2[id][1]== compar1[id+2][1])){	
						sleep(1);	
				sem_wait(semtrain1); // on bloque ce train1 
			}
			printf("le Train %d  est sur la voie  %c%c\n", id+1,data_i1[i],data_i1[i+1]);
			fflush(stdout);
			compar2[id][0]=data_i1[i+1];
			compar2[id][1]=data_i1[i];

		}
			// Train2 
		if (id==1){
			// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
			if ((compar2[id][0]== compar1[id-1][0])&&(compar2[id][1]== compar1[id-1][1])){
					sleep(1);
			sem_wait(semtrain2); // on bloque ce train2 
			} 
			else if	((compar2[id][0]== compar1[id+1][0])&&(compar2[id][1]== compar1[id+1][1])){	
				sleep(1);
			sem_wait(semtrain2); // on bloque ce train 2
			}
				sleep(1);
			printf("le Train %d  est sur la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
			fflush(stdout);
			compar2[id][0]=data_i2[i+1];
			compar2[id][1]=data_i2[i];
		}

			// Train3 
		if (id==2){
			// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
			if ((compar2[id][0]== compar1[id-1][0])&&(compar2[id][1]== compar1[id-1][1])){
				sleep(1);
				sem_wait(semtrain3); // on bloque ce train3 
			}
			else if((compar2[id][0]== compar1[id-2][0])&&(compar2[id][1]== compar1[id-2][1])){
					sleep(1);	
				sem_wait(semtrain3); // on bloque ce train 3
			}
					sleep(1);
		printf("le Train %d  est sur la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
        	fflush(stdout);
			compar2[id][0]=data_i3[i+1];
			compar2[id][1]=data_i3[i];
		}

		sleep(rand() % 2); 


		if (id==0){
		
			printf("le Train %d  sort de la voie  %c%c\n", id+1,data_i1[i],data_i1[i+1]);
			fflush(stdout);
				// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
 			if ((compar2[id][0]== compar1[id+1][0])&&(compar2[id][1]== compar1[id+1][1])){
				sleep(1);
				sem_post(semtrain2); //(après que ce train sort) on débloque le train opposé
			}
			 if ((compar2[id][0]== compar1[id+2][0])&&(compar2[id][1]== compar1[id+2][1])){	
				sleep(1);
				sem_post(semtrain3); //(après que ce train sort) on débloque le train opposé
			}
			compar2[id][0]=RaZ[0][0];
			compar2[id][1]=RaZ[0][0];

		}

		if (id==1){

			printf("le Train %d  sort de la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
			fflush(stdout);

			// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
			if ((compar2[id][0]== compar1[id-1][0])&&(compar2[id][1]== compar1[id-1][1])){
				sleep(1);
				sem_post(semtrain1); //(après que ce train sort) on débloque le train opposé
			}
			 if	((compar2[id][0]== compar1[id+1][0])&&(compar2[id][1]== compar1[id+1][1])){	
				sleep(1);
				sem_post(semtrain3); //(après que ce train sort) on débloque le train opposé
			}
			compar2[id][0]=RaZ[0][0];
			compar2[id][1]=RaZ[0][0];

		}

		if (id==2){
			printf("le Train %d  sort la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
			fflush(stdout);
			
			// si un train est sur cette voie dans le sens opposé (compar1 = compar2)
			if (!(compar2[id][0]== compar1[id-1][0])&&(compar2[id][1]== compar1[id-1][1])){
				sleep(1);
				sem_post(semtrain1); //(après que ce train sort) on débloque le train opposé
			}
			 if(!(compar2[id][0]== compar1[id-2][0])&&(compar2[id][1]== compar1[id-2][1])){	
				sleep(1);
				sem_post(semtrain2); //(après que ce train sort) on débloque le train opposé
			}
			compar2[id][0]=RaZ[0][0];
			compar2[id][1]=RaZ[0][0];

		}
								
		sleep(rand() % 3); // le train fait une pause

	}
	printf(" fin de parcour le train %d : \n", id+1);
       	fflush(stdout);
    return NULL;
}

void* Train1(void *arg) {
	float t1,s1;
	int i;

	for (i=0;i<30;i++){

		t1d[i] = clock();		// temps de dépare
		
		sens(arg);
			
        	sens_inverse(arg);

		t1a[i] = clock()-t1d[i]; 	// temps de trajet =  temps de D'arrivée- temps de dépare
		s1=s1+t1a[i];			// somme des temps de trajet 
	
		fprintf(stderr,"le temps de parcours du train 1 jusqu'ici : %f\n",s1);
        	fflush(stderr);
	}
	t1=s1/30; // la moyenne de temps de trajet
	fprintf(stderr,"le temps moyen de parcours du train 1 : %f\n",t1);
	fprintf(stderr,"le temps total de parcours du train 1 : %f\n",s1);
    return NULL;
}
void* Train2(void *arg) {
	float t2,s2;	
	int i;
	
	for (i=0;i<30;i++){
		
		t2d[i] = clock();		// temps de dépare

		sens(arg);		
	
        	sens_inverse(arg);

		t2a[i] = clock()-t2d[i]; 	// temps de trajet =  temps de D'arrivée- temps de dépare
		s2=s2+t2a[i];			// somme des temps de trajet 
		
		fprintf(stderr,"le temps de parcours du train 2 jusqu'ici : %f\n",s2);
        	fflush(stderr);
	}
	t2=s2/30; // la moyenne de temps de trajet
	fprintf(stderr,"le temps moyen de parcours du train 2 : %f\n",t2);
	fprintf(stderr,"le temps total de parcours du train 2 : %f\n",s2);
	
    return NULL;
}
void* Train3(void *arg) {
	float t3,s3;
	int i;

	for (i=0;i<30;i++){
		
		t3d[i] = clock();		// temps de dépare

		sens(arg);		
	
        	sens_inverse(arg);

		t3a[i] = clock()-t3d[i];	// temps de trajet =  temps de D'arrivée- temps de dépare
		s3=s3+t3a[i];			// somme des temps de trajet 
		
		fprintf(stderr,"le temps de parcours du train 3 jusqu'ici : %f\n",s3);
        	fflush(stderr);
	}
	t3=s3/30; // la moyenne de temps de trajet
	fprintf(stderr,"le temps moyen de parcours du train 3 : %f\n",t3);
	fprintf(stderr,"le temps total de parcours du train 3 : %f\n",s3);
    return NULL;
}

int main() {
	int i,j, nb[3];
	srand(time(NULL)); // initialisation de rand
    pthread_t tid[3];
    
    semtrain1 = sem_open("semtrain1", O_CREAT, S_IRUSR | S_IWUSR, 0);
    semtrain2 = sem_open("semtrain2", O_CREAT, S_IRUSR | S_IWUSR, 0);
    semtrain3 = sem_open("semtrain3", O_CREAT, S_IRUSR | S_IWUSR, 0);


    
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 2; j++) {
		compar1[i][j]=RaZ[0][0];
		compar2[i][j]=RaZ[0][0];
	}
    }

for (i = 0; i < 1; i++) {
		nb[i] = i;
    	pthread_create(&tid[i], NULL, Train1, (void*) &nb[i]);
    }
	for (i = 0; i < 1; i++) {
		nb[i+1] = i+1;
		pthread_create(&tid[i+1], NULL, Train2, (void*) &nb[i+1]);
    }
	for (i = 0; i < 1; i++) {
		nb[i+2] = i+2;
		pthread_create(&tid[i+2], NULL, Train3, (void*) &nb[i+2]);
    }
		

    for (i = 0; i < 3; i++) {
		pthread_join(tid[i], NULL);
    }
    
    puts("les Trajets sont terminées");
    fflush(stdout);

    sem_close(semtrain1);
    sem_close(semtrain2);
    sem_close(semtrain3);

    sem_unlink("semtrain1");
    sem_unlink("semtrain2");
    sem_unlink("semtrain3");

    pthread_mutex_destroy(&fifo);
    pthread_mutex_destroy(&fifo1);
    pthread_mutex_destroy(&fifo2);
    return 0;
}
