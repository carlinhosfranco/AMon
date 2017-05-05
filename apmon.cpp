/*
Descrição: Programa para monitorar o uso de recursos do sistema e de processos especificos. 
As métricas sao coletadas utilizando a biblioteca UPL.
Autor: Carlos Alberto Franco Maron e-mail: francocaam@gmail.com
Data: 14/04/2017 - Ultima atualizaçao: 20/04/2017
*/
#include <iostream>
#include <upl.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <new>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>

using namespace std;

char *hashtag = "##################################";

struct metrics{

char	*sys_mem;
ssize_t	memory_per_pid;
long long cache_miss;
char	*ioMetrics;
double *core_load;

}apps_metrics;

FILE *fMemPPid;
FILE *fCoreLoad;
FILE *fIO;
FILE *fGeneralMetrics;


int app_monitor(int number_of_threads, double sleep_time, char *script_file){


	time_t now = time(0);
	
	char line_status[255];
	int ran = 2;

	apps_metrics.sys_mem = UPL_getSysMemInfo();
	auto t_start = std::chrono::high_resolution_clock::now();

	char buff[255];
    	sprintf(buff,"bash ./%s %d",script_file, number_of_threads);

    	char *saida = UPL_getCommandResult(buff);

	sprintf(line_status,"ps aux | grep -c %s",saida);

	char *status = UPL_getCommandResult(line_status);

	apps_metrics.memory_per_pid = UPL_getProcMemUsage_pid(atoi(saida));

	ssize_t max_cores = UPL_getNumOfCores();
	
	unsigned long long int *total_ticks =  new unsigned long long int [max_cores];
	unsigned long long int *idle = new unsigned long long int [max_cores];

	int fd_cache;
	if(UPL_init_cache_miss_monitoring(&fd_cache) == 0){
		std::cout << "Error when UPL_init_cache_miss_monitoring(...)" << std::endl;
	}
		
	//fCoreLoad = fopen("core_load.txt","w");
	fCoreLoad = fopen("core_load.txt","a+");
	if (fCoreLoad == NULL){
		cout << "Erro ao abrir arquivo core_load.txt" << endl;
		return 1;
	}
	//fMemPPid = fopen("MemPPid.txt","w");
	fMemPPid = fopen("MemPPid.txt","a+");
	if (fCoreLoad == NULL){
		cout << "Erro ao abrir arquivo MemPPid.txt" << endl;
		return 1;
	}

	//fIO = fopen("IO.txt","w");
	fIO = fopen("IO.txt","a+");
	if (fIO == NULL){
		cout << "Erro ao abrir arquivo IO.txt" << endl;
		return 1;
	}
	
	time_t t;
	struct tm * timeinfo;
	
	while(atoi(status) != ran || status != NULL){

		if (UPL_init_cores_load_monitoring(idle, total_ticks) == NULL){
			std::cout << "Error when UPL_init_cores_load_monitoring(...)" << std::endl;
		}
		
		time (&t);
		timeinfo = localtime(&t);
		
		usleep(sleep_time);
		
		fprintf(fCoreLoad, " %s \n", asctime(timeinfo));
		apps_metrics.core_load = UPL_get_cores_load_average(idle, total_ticks);
		for (int i=0; i<UPL_getNumOfCores(); i++)
		fprintf(fCoreLoad, "CPU%d:\t%3.2lf%%\n", i, apps_metrics.core_load[i]);

		
		time (&t);
		timeinfo = localtime(&t);
		fprintf(fMemPPid, " %s \n", asctime(timeinfo));
		apps_metrics.memory_per_pid = UPL_getProcMemUsage_pid(atoi(saida));
		if (apps_metrics.memory_per_pid == -1){
			break;
		}
		fprintf(fMemPPid, "Memory:\t%ld \n", apps_metrics.memory_per_pid);

		status = UPL_getCommandResult(line_status);
		if (status == NULL){
			break;
		}

		//Não poderia ser colocado em uma Thread???
		
		apps_metrics.ioMetrics = UPL_getCommandResult("iostat -d -m -x -y");
		fprintf(fIO,"%s\n",apps_metrics.ioMetrics);

	}
	auto t_end = std::chrono::high_resolution_clock::now();
	fclose(fMemPPid);
	fclose(fCoreLoad);
	fclose(fIO);
	delete idle;
	delete total_ticks;

	apps_metrics.cache_miss = UPL_get_cache_miss(fd_cache);

	//fGeneralMetrics = fopen("GeneralMetrics.txt","w");
	fGeneralMetrics = fopen("GeneralMetrics.txt","a+");
	if (fGeneralMetrics == NULL){
		cout << "Erro ao abrir arquivo GeneralMetrics.txt" << endl;
		return 1;
	}

	auto t_total = chrono::duration<double>(t_end-t_start).count();
	
	fprintf(fGeneralMetrics,"Executation Time: %f\n\n", t_total);	
	fprintf(fGeneralMetrics,"Threads: %d\n\n", number_of_threads);
	
	fprintf(fGeneralMetrics,"%s\n\n", hashtag);

	fprintf(fGeneralMetrics,"Cache Miss: %lld\n\n", apps_metrics.cache_miss);	
	if(apps_metrics.cache_miss < 0){
		char *ERRO = "Error when UPL_get_cache_miss(...)";
		fprintf(fGeneralMetrics," %s\n\n", ERRO);
	}
	
	fprintf(fGeneralMetrics,"%s\n\n", hashtag);
	
	fprintf(fGeneralMetrics,"System Memory \n %s\n\n\n", apps_metrics.sys_mem);
	
	fprintf(fGeneralMetrics,"%s\n\n", hashtag);	

	fclose(fGeneralMetrics);

}

int main(int argc, char *argv[])
{
	if (argc <= 2){
		cout << "ERRO: ./app_monitor [threads] [intervalo_de_coleta(segundos)] [nome_script.sh]" << endl;
		return 1;
	}
	int threads = atoi(argv[1]);

	//unsigned int get_time = atoi(argv[2]);
	double get_time = atof(argv[2]);

	char *script = argv[3];
	
	cout << "Tempo: " << get_time << endl;
	app_monitor(threads,get_time,script);

	return 0;
}
