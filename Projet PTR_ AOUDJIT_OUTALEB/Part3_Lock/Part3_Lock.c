#include <stdio.h>   // printf
#include <pthread.h> // pthread
#include <unistd.h>  // sleep
#include <stdlib.h>  // srand
#include <time.h>    // time
					//gcc -Wall Part3_Lock.c -lpthread //compiler
					// ./a.out 1> 1_lock.txt		   // executer

char data1[] = {'A','B','C'};
char data_i1[] = {'C','B','A'};
char data2[] = {'A','B','D','C'};
char data_i2[] = {'C','B','A'};
char data_i3[] = {'C','E','A'};

clock_t t1d[30], t2d[30],t3d[30];
clock_t t1a[30], t2a[30],t3a[30];

pthread_rwlock_t lock;
pthread_cond_t cond;

void* sens(void *arg) {
	int id = *((int*) arg);
	int i;
	
	for (i = 0; i < 2; i++) {

		if ((id==0)){
		printf("le Train %d  veut s'engager sur la voie  %c%c\n", id+1,data1[i],data1[i+1]);
        	fflush(stdout);
		}

		if (id==1){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data2[i],data2[i+1]);		
        	fflush(stdout);
		}
		
		if (id==2){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);
		}
		pthread_rwlock_rdlock(&lock); // le train de sens aquière le verrou 
		
		
		if (id==0){
		
		printf("le Train %d  est sur la voie  %c%c\n", id+1,data1[i],data1[i+1]);
        	fflush(stdout);
		
		}
		if (id==1){

				
		printf("le Train %d  est sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);
        	
		}
		if (id==2){		
		printf("le Train %d  est sur la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);        	
		}

		sleep(rand() % 2);

		

		if (id==0){
		printf("le Train %d  sort de la voie  %c%c\n", id+1,data1[i],data1[i+1]);
        	fflush(stdout);
		}

		if (id==1){
				
		printf("le Train %d  sort de la voie %c%c\n", id+1,data2[i],data2[i+1]);
        	fflush(stdout);
		}

		if (id==2){
					
		printf("le Train %d  sort de la voie %c%c\n", id+1,data2[i],data2[i+1]);

        	fflush(stdout);
		}

		
		pthread_rwlock_unlock(&lock); // le trains sens libère le verrou 					
		sleep(rand() % 3); // le train fait une pause
	
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
		}

		if (id==1){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
        	fflush(stdout);
		}

		if (id==2){
		printf("le Train %d  veut s'engager sur la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
        	fflush(stdout);
		}
		
		pthread_rwlock_wrlock(&lock); // le train de sensInverse aquière le verrou

		if (id==0){
		printf("le Train %d  est sur la voie  %c%c\n", id+1,data_i1[i],data_i1[i+1]);
        	fflush(stdout);
		}

		if (id==1){
		printf("le Train %d  est sur la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
        	fflush(stdout);
		}

		if (id==2){
		printf("le Train %d  est sur la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
        	fflush(stdout);
		}

		sleep(rand() % 2); 


		if (id==0){
		printf("le Train %d  sort de la voie  %c%c\n", id+1,data_i1[i],data_i1[i+1]);
        	fflush(stdout);
		}

		if (id==1){
		printf("le Train %d  sort de la voie %c%c\n", id+1,data_i2[i],data_i2[i+1]);
        	fflush(stdout);
		}

		if (id==2){
		printf("le Train %d  sort la voie %c%c\n", id+1,data_i3[i],data_i3[i+1]);
        	fflush(stdout);
		}

		 pthread_rwlock_unlock(&lock);	// le train de sensInverse aquière le verrou					
		sleep(rand() % 3); // le train fait une pause

	}
	printf(" fin de parcour le train %d : \n", id+1);
       	fflush(stdout);
    return NULL;
}

void* Train1(void *arg) {
	
	int i;
	float t1,s1;
	for (i=0;i<30;i++){
		t1d[i] = clock(); // temps de dépare
		sens(arg);
		
	
        	sens_inverse(arg);
		t1a[i] = clock()-t1d[i]; // temps de trajet =  temps de D'arrivée- temps de dépare
		s1=s1+t1a[i]; // somme des temps de trajet 
	fprintf(stderr,"le temps de parcours du train 1 jusqu'ici : %f\n",s1);
        	fflush(stderr);
	}
	
	t1=s1/30; // la moyenne de temps de trajet
	fprintf(stderr,"le temps moyen de parcours du train 1 : %f\n",t1);
        	fflush(stderr);
	fprintf(stderr,"le temps totaux de parcours du train 1: %f\n",s1);
        	fflush(stderr);
    return NULL;
}
void* Train2(void *arg) {
	
	float t2,s2;
                 	
	int i;
	for (i=0;i<30;i++){
		t2d[i] = clock();		// temps de dépare
		sens(arg);			

	
        	sens_inverse(arg);
		t2a[i] = clock()-t2d[i];	// temps de trajet =  temps de D'arrivée- temps de dépare
	   	 s2=s2+t2a[i];			// somme des temps de trajet 
	fprintf(stderr,"le temps de parcours du train 2 jusqu'ici : %f\n",s2);
        	fflush(stderr);
	}
	
	t2=s2/30; 				// la moyenne de temps de trajet 
	fprintf(stderr,"le temps moyen de parcours du train 2 : %f\n",t2);
	fprintf(stderr,"le temps total des parcours du train 2 :%f\n",s2);

	
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
	}
	
	t3=s3/30;				// la moyenne de temps de trajet
	fprintf(stderr,"le temps moyen de parcours du train 3 : %f\n",t3);
	fprintf(stderr,"le temps total de parcours du train 3 : %f\n",s3);

    return NULL;
}


int main() {
	int i, nb[3];
	srand(time(NULL)); // initialisation de rand
    pthread_t tid[3];

    pthread_rwlock_init(&lock, NULL);
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

   	pthread_rwlock_destroy(&lock);
   
    pthread_cond_destroy(&cond);

    return 0;
}
