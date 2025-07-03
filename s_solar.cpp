/*

  Programa realizado para simular el sistema solar

*/
#include <iostream>
#include <fstream> //Biblioteca para leer los datos del fichero
#include <math.h> //Funciones matematicas
#include <string> //Para trabajar con strings
#include <iomanip> //Para modificar la entrada y salida estandar
#include <stdlib.h> 


//Caracteristicas del programa
#define N 10//Definimos el numero de cuerpos que tendra nuestro sistema
#define h 10e-3 //Espaciando que usaremos en el algoritmo
#define t_max 3000 //Tiempo de duracion de la simulacion
#define pi acos(-1.0)

//Definimos los factores de reescala de las variables y constantes
#define M (long double)1.98847e6 //Masa del Sol
#define c (long double)1.49597e11 //Distancia Tierra-Sol
#define G (long double)6.67384e-11 //Cte de gravitacion universal
#define r_t (long double)1.991095380983256e-07 //Reescala del tiempo ha medir como unidad equivalente a 58.1 dias que es el tiempo que tardan los efectos del movimiento en ser notables
#define conv_v (long double)3.357260577489655e-02
#define conv_dias (long double)58.129179468833314 //Para pasar el tiempo reescalado a dias



using namespace std;

//Definicion de funciones

//Funciones para trabajar con archivos
bool leer(long double[],long double[],long double[],long double[],long double[]);
bool iniciar(ofstream[]);
void cerrar(ofstream[]);
void periodo();

//Funciones de propiedades fisicas del sistema
inline long double aceleracionx(long double,long double,long double,long double,long double);
inline long double aceleraciony(long double,long double,long double,long double,long double);
inline long double momento(long double[],long double[],long double[],long double[],long double[]);
inline long double energia(long double[],long double[],long double[],long double[],long double[]);


int main(){

	//Creamos un array para almacenar los datos de cada cuerpo
	long double m[N],x[N],y[N],vx[N],vy[N],ax[N],ay[N],t=(long double)0.0,wx[N],wy[N],l=(long double)0.0;	
	ofstream fichero[N+1];//Para leer y guardar los datos de los cuerpos y la energia y momento angular
	

	//Inicializamos los vectores velocidad y el array de la suma de momentos angulares
	for(unsigned int i=0;i<N;i++){
		ax[i]=(long double)0.0;
		ay[i]=(long double)0.0;
	}

  
	if(!leer(m,x,y,vx,vy)){

		if(!iniciar(fichero)){

		//Calculamos las trayectorias de los cuerpos por el algoritmo de Verlet

		//Empezamos calculando las aceleraciones iniciales
		for(unsigned int i=0;i<N;i++){
			for(unsigned int j=0;j<N;j++){
				if(i!=j){
					ax[i]+=aceleracionx(m[j],x[i],y[i],x[j],y[j]);
					ay[i]+=aceleraciony(m[j],x[i],y[i],x[j],y[j]);
				}
			}
// 			cout<<i<<endl<<ax[i]<<endl<<ay[i]<<endl<<endl<<endl;
		}

		while(t<=t_max){

			//Guardamos de la energia en los archivos
			fichero[N]<<setprecision(20)<<t<<setw(5)<<""<<energia(m,x,y,vx,vy)<<setw(5)<<""<<momento(m,x,y,vx,vy)<<endl<<endl<<endl;
			
			for(unsigned int i=0;i<N;i++){
				fichero[i]<<setprecision(17)<<t<<setw(5)<<""<<x[i]<<setw(5)<<""<<y[i]<<endl<<endl<<endl;
				
			}

			//Sacamos las nuevas posiciones de cada cuerpo
			for(unsigned int i=0;i<N;i++){
				x[i]=x[i]+h*vx[i]+h*h*ax[i]/(long double)2.0;
				y[i]=y[i]+h*vy[i]+h*h*ay[i]/(long double)2.0;
			
				//Calculamos el vector auxiliar de la velocidad
				wx[i]=vx[i]+h*ax[i]/(long double)2.0;
				wy[i]=vy[i]+h*ay[i]/(long double)2.0;
			}

			//Borramos las componenetes de la aceleracion para reescribirlas
			for(unsigned int i=0;i<N;i++){
				ax[i]=(long double)0.0;
				ay[i]=(long double)0.0;
			}

			//Calculamos las nuevas aceleraciones asiciadas a cada cuerpo
			for(unsigned int i=0;i<N;i++){
				for(unsigned int j=0;j<N;j++){
					if(i!=j){
						ax[i]+=aceleracionx(m[j],x[i],y[i],x[j],y[j]);
						ay[i]+=aceleraciony(m[j],x[i],y[i],x[j],y[j]);
					}
				}
// 				cout<<i<<endl<<ax[i]<<endl<<ay[i]<<endl<<endl<<endl;
			}

			//Calculamos las nuevas velocidades de cada cuerpo
			for(unsigned int i=0;i<N;i++){
				vx[i]=wx[i]+h*ax[i]/(long double)2.0;
				vy[i]=wy[i]+h*ay[i]/(long double)2.0;

			}

			//Incrementamos el contador
			t+=h;
		}
		
			//Cerramos los ficheros con los datos
			cerrar(fichero);
		}else cout<<"No se pudo crear los ficheros con los resultados"<<endl;

	}else cout<<"No se pudo leer los archivos con los datos"<<endl;
	
  	//periodo();

  return 0;
}

/*
  Funcion para incializar los vectores con los datos de los ficheros

  Entrada:
   -masa
   -posicion en coordenadas cartesianas
   -velocidad en cartesianas

  Salida:
   .Si se ha logrado la operacion con exito o no

*/
bool leer(long double m[],long double x[],long double y[],long double v_x[],long double v_y[]){

	//Variable para saber si ha habido algun fallo con algun archivo
	bool flag=false;
	ifstream fichero;

	//Leemos los datos de los arhivos
	for(unsigned int i=0;i<N;i++){

		fichero.open("cuerpos_perihelio/"+to_string(i)+".dat");
		if(fichero.is_open()){

		//Guardmos los datos en los vectores
		fichero>>m[i];
		fichero>>x[i];
		fichero>>y[i];
		fichero>>v_x[i];
		fichero>>v_y[i];
		
		//Aplicamos los factores de reescala
		m[i]=m[i]/M;
		x[i]=x[i]/c;
		y[i]=y[i]/c;
		v_x[i]=v_x[i]*conv_v;
		v_y[i]=v_y[i]*conv_v;
		
//   		cout<<i<<endl<<endl<<m[i]<<endl<<x[i]<<endl<<y[i]<<endl<<v_x[i]<<endl<<v_y[i]<<endl<<endl<<endl<<endl;
// 		cout<<1/(r_t*c)<<endl<<endl;
		fichero.close();

		}else flag=true;//Contador de archivos que no se han abierto

	}


	return flag;
}

/*
	
	Funcion para asignar a cada elemento del array en que archivo escribir la trayectoria del planeta
	
	Entrada:
	-Vector de archivos

	Salida:
	-Variable boobleana que nos indica si se ha completado o no la operacion

*/

bool iniciar(ofstream F[]){

	//Variable para saber si se han abierto o no los archivos
	bool flag=false;

	for(unsigned int i=0;i<N;i++){
		F[i].open("movimiento_cuerpos/"+to_string(i)+".dat");

		//Comprobamos is se ha abierto o no el archivo
		if(!F[i].is_open()) flag=true;
		
	}

	//Creamos el fichero donde guardaremos los datos de la energia y el momento angular del sistema
	F[N].open("caracteristicas/datos.dat");
	if(!F[N].is_open()) flag=true;

	return flag;
}

/*

  Fucion para cerrar todos los ficheros con los que estamos trabajando

  Entrada:
   -Vector de ficheros

  Salida:
  -Nada
  
*/
void cerrar(ofstream F[]){
	for(unsigned int i=0;i<N+1;i++)
		F[i].close();
}

/*

  Funcion para devolver la aceleracion de dos cuerpos

  Entrada:
   -masa
   -vector posicon en cartesianas del cuerpo 1
   -vector '''''''''''''''''''''''''''''''''' 2

  Salida:
   -Aceleracion en el eje x
   

*/

inline long double aceleracionx(long double m,long double x1,long double y1,long double x2,long double y2){

	//Devolvemos la aceleracion en el eje x
	return m*(x2-x1)/(long double)pow(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),(long double)3.0);
  
}


/*

	Funcion para devolver la aceleracion de dos cuerpos

	Entrada:
	-masa
	-vector posicon en cartesianas del cuerpo 1
	-vector '''''''''''''''''''''''''''''''''' 2

	Salida:
	-Aceleracion en el eje y
   

*/

inline long double aceleraciony(long double m,long double x1,long double y1,long double x2,long double y2){

	//Devolvemos la aceleracion en el eje x
	return m*(y2-y1)/(long double)pow(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),(long double)3.0);
  
}


/*
  Funcion para obtener el modulo y la direccion del momento angular del sistema

  Entrada:
   -vector masa
   -vector posicion
   -vector velocidad

  Salida:
   -Momento angular total del sistema

*/

inline long double momento(long double m[],long double x[],long double y[],long double vx[],long double vy[]){
	
	long double l=(long double)0.0;
	
	for(unsigned int i=0;i<N;i++){
		l+=m[i]*(x[i]*vy[i]-vx[i]*y[i]);
	}
	return l;
}

/*
  Funcion para determinar la energia total del sistema

  Entrada:
   -Vector de las masas
   -Vector de las velocidades
   -Vector de las posiciones

  Salida:
   -Energia del sistema

*/
inline long double energia(long double m[],long double x[],long double y[],long double vx[],long double vy[]){

	long double T=(long double)0.0,V=(long double)0.0;

	//Calculamos primero la suma de energias cineticas del sistema
	for(unsigned int i=0;i<N;i++)
		T+=m[i]*(vx[i]*vx[i]+vy[i]*vy[i])/(long double)2.0;

	//Calculamos la suma de energias potenciales
	for(unsigned int i=0;i<N;i++){
		for(unsigned int j=0;j<N;j++){
			if(i!=j){
				V-=m[i]*m[j]/(long double)sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			}
		}
	}

	//Como cada energia potencial la hemos sumado dos veces, entonces dividimos entre 2
	return T+V/(2.0);
}


/*
  Funcion para determinar mostrar los periodos de las orbitas de cada planeta

  Entrada:

  Salida:

*/
void periodo(){
	
	long double t1,t2,r1,r_0,r2=(long double)0.0,y,T[N],t;
	ifstream F; //Para leer los datos
	ofstream Fich;//Guardamos los datos en un fichero
	
	//Definimos un array con los periodos correctos de cada orbita y asi poder calculaer los errores realtivos
	T[1]=(long double)88.0;
	T[2]=(long double)224.7;
	T[3]=(long double)365.2;
	T[4]=(long double)687.0;
	T[5]=(long double)4331.0;
	T[6]=(long double)10747.0;
	T[7]=(long double)30589.0;
	T[8]=(long double)59800.0;
	T[9]=(long double)90588.0;
	
	/*
		Para calcular el periodo de la orbita de cada planeta, vamos a buscar el afelio y a partir de eso emplearemos la 3 ley de Kepler
	*/
	Fich.open("caracteristicas/periodo.dat");
	
	if(Fich.is_open()){
		for(unsigned int i=1;i<N;i++){
			F.open("movimiento_cuerpos/"+to_string(i)+".dat");
			
			//Comprobamos que se haya abierto
			if(F.is_open()){
				
				
				//Iniciamos el contador cundo estemos en la parte positiva del eje x
				
				if(i!=9){
					do{
						F>>t1;
						F>>r1;
						F>>y;
					}while(r1<(long double)0.0);
					
					//Buscamos el maximo
					do{
						F>>t2;
						F>>r2;
						F>>y;
						
						//Comprobamos si el valor encontrado es un maximo
						if(r2>r1){
							r1=r2;
							t1=t2;
						}
					}while(r2>(long double)0.0);
					
				//Caso particular para pluton
				}else{
					do{
						F>>t1;
						F>>r1;
						F>>y;
					}while(r1>(long double)0.0);
					
					
					//Buscamos el maximo
					do{
						F>>t2;
						F>>r2;
						F>>y;
						
						//Comprobamos si el valor encontrado es un maximo
						if(r2<r1){
							r1=r2;
							t1=t2;
						}
					}while(r2<(long double)0.0);
				}
				
				F.close();
				
				t=(long double)2.0*t1*(conv_dias);
				
				//Mostramos el periodo
 				Fich<<i<<setw(5)<<""<<t<<setw(5)<<""<<(long double)abs(t-T[i])*100.00/T[i]<<endl<<endl<<endl;
				
			}else cout<<"No se pudo leer los datos del periodo"<<endl;
			
		}
		
		Fich.close();
	}else cout<<"No se pudo guardar los periodos"<<endl;
	
}
