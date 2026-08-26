#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

#define NUM_PRUEBAS 10

typedef void (*func) (int N,std::vector<std::vector<double>>& vec);

enum Type {
	COL,
	FIL,
	LOCAL_TEMP_A,
	LOCAL_TEMP_B
};

std::string CastType(Type tipe) {
	switch (tipe) {
		case COL:
			return "Columnas";
		case FIL:
			return "Filas";
		case LOCAL_TEMP_A:
			return "Localidad Temporal A";
		case LOCAL_TEMP_B:
			return "Localidad Temporal B";
		default:
			return "Unknown";
	}
}

void FillMatrix(int N,std::vector<std::vector<double>>& vec){
	vec.resize(N);
	for (int i = 0;i < N;i++) {
		vec[i].resize(N);
		for (int j = 0;j < N;j++) {
			vec[i][j] = 1.0;
		}
	}
}

void IterRows(int N,std::vector<std::vector<double>>& vec) {
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < N;j++) {
			vec[i][j] *= 2.0;
		}
	}	
}

void IterCols(int N,std::vector<std::vector<double>>& vec) {
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			vec[i][j] *= 2.0;
		}
	}
}

void LocTmpA(int N, std::vector<std::vector<double>>& vec) {
	for (int i = 0;i < N;i++) {
		vec[i][0] += 1;
		vec[i][0] += 1;
		vec[i][0] += 1;
	}
}

void LocTmpB(int N, std::vector<std::vector<double>>& vec) {
	for (int k = 0;k < 3;k++) {
		for (int i = 0;i < N;i++) {
			vec[i][0] += 1;
		}
	}
}

void SetUp(int N,std::vector<std::vector<double>>& vec,Type tipe,func funcion) {
	FillMatrix(N, vec);

	double media = 0.0;
	std::string type = CastType(tipe);
	std::cout << std::string(40, '=') << std::endl;
	std::cout << " Comenzando ejecucion de " << type << " | N = " << N << std::endl;
	for (int i = 0;i < NUM_PRUEBAS;i++) {
		auto inicio = std::chrono::high_resolution_clock::now();
		funcion(N,vec);
		auto fin = std::chrono::high_resolution_clock::now();
		double tiempo = (std::chrono::duration<double,std::milli>(fin - inicio)).count();
		media += tiempo;

		std::cout << "Tiempo a " << type << " -> " << tiempo << "ms" << std::endl;
	}
	std::cout << "Media de tiempo "<< type << " -> " << media / NUM_PRUEBAS << "ms" << std::endl;
	std::cout << std::string(40, '=') << std::endl << std::endl;

}



int main(){
	std::vector<std::vector<double>> vec;

	// Prueba Parte B
	FillMatrix(20000, vec);

	// Prueba Parte C
	SetUp(20000, vec, FIL, &IterRows);

	// Prueba Parte D
	SetUp(20000, vec, COL, &IterCols);

	// Pruebas para filas
	SetUp(100,vec, FIL, &IterRows);
	SetUp(250, vec, FIL, &IterRows);
	SetUp(500, vec, FIL, &IterRows);
	SetUp(1000, vec, FIL, &IterRows);
	SetUp(2000, vec, FIL, &IterRows);
	SetUp(4000, vec, FIL, &IterRows);


	// Pruebas para columnas
	SetUp(100, vec, COL, &IterCols);
	SetUp(250, vec, COL, &IterCols);
	SetUp(500, vec, COL, &IterCols);
	SetUp(1000, vec, COL, &IterCols);
	SetUp(2000, vec, COL, &IterCols);
	SetUp(4000, vec, COL, &IterCols);


	//Patrones temporales local A
	SetUp(20000, vec, LOCAL_TEMP_A, &LocTmpA);

	//Patrones temporales local B
	SetUp(20000, vec, LOCAL_TEMP_B, &LocTmpB);
}

