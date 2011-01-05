/*
 * Control De Movimiento.
 *
 * Usamos tres controladores Fuzzy para este control, uno para cada dimension 
 * (x,y,angulo).
 *
 * 
 * Fusificacion de e y de:
 * 
 *  NB           NM        NS          ZE          PS        PM           PB
 * |\            /\        /\          /\          /\        /\            /| 20
 * |  \         /  \      /  \        /  \        /  \      /  \         /  |
 * |    \      /    \    /    \      /    \      /    \    /    \      /    |
 * |      \   /      \  /      \    /      \    /      \  /      \   /      |
 * |        \/        \/        \  /        \  /        \/        \/        |
 * |        / \       /\         \/          \/         /\       / \        |
 * |_______/____\____/__\________/\__________/\________/__\____/____\_______| 0
 *-100      -75      -50      -25      0       25       50       75       100
 *
 * Por ahora haremos la misma fuzzyficacion a las dos entradas.
 * 
 * Usaremos parabolas en cambio de los triangulos.
 * 
 * x1, x2 son raices (y=0)
 * y=ax^2+bx+c
 * a=1/(x1*x2)
 * b=-(1/x1+1/x2)
 * c=100
 *
 * Reglas de control:
 *
 * NB=0 NM=1 NS=2 ZE=3 PS=4 PM=5 PB=6
 *
 * du| NB NM NS ZE PS PM PB   du| 0 1 2 3 4 5 6
 * ------------------------   -----------------
 * NB| PB PB PB PB PM PS ZE   0 | 6 6 6 6 5 4 3
 * NM| PB PB PM PM PS ZE NS   1 | 6 6 5 5 4 3 2
 * NS| PB PM PS PS ZE NS NM   2 | 6 5 4 4 3 2 1
 * ZE| PM PM PS ZE NS NM NM   3 | 5 5 4 3 2 1 1
 * PS| PM PS ZE NS NS NM NB   4 | 5 4 3 2 2 1 0
 * PM| PS ZE NS NM NM NB NB   5 | 4 3 2 1 1 0 0
 * PB| ZE NS NM NB NB NB MB   6 | 3 2 1 0 0 0 0
 *
 * du de| NB NM NS ZE PS PM PB   du de| 0 1 2 3 4 5 6
 *  e                             e
 * ---------------------------   --------------------
 * NB   | PB PB PB PB PM PS ZE   0    | 6 6 6 6 5 4 3
 * NM   | PB PB PM PM PS ZE NS   1    | 6 6 5 5 4 3 2
 * NS   | PB PM PS PS ZE NS NM   2    | 6 5 4 4 3 2 1
 * ZE   | PM PM PS ZE NS NM NM   3    | 5 5 4 3 2 1 1
 * PS   | PM PS ZE NS NS NM NB   4    | 5 4 3 2 2 1 0
 * PM   | PS ZE NS NM NM NB NB   5    | 4 3 2 1 1 0 0
 * PB   | ZE NS NM NB NB NB MB   6    | 3 2 1 0 0 0 0
 * 
 * Defusificacion:
 *
 *  NB           NM        NS          ZE          PS        PM           PB
 * |\            /\        /\          /\          /\        /\            /| 20
 * |  \         /  \      /  \        /  \        /  \      /  \         /  |
 * |    \      /    \    /    \      /    \      /    \    /    \      /    |
 * |      \   /      \  /      \    /      \    /      \  /      \   /      |
 * |        \/        \/        \  /        \  /        \/        \/        |
 * |        / \       /\         \/          \/         /\       / \        |
 * |_______/____\____/__\________/\__________/\________/__\____/____\_______| 0
 *-100      -75      -50      -25      0       25       50       75       100 
 *
 * Por ahora el grafico es el mismo.
 *
 */

//#define DEBUG 1
#include "Fuzzy.h"

/*
struct FUZZY {
  int16_t lastPosition;
  int16_t e;
  int16_t de;
  uint8_t parabolas[7][2]; 
  uint8_t reglas_control[7][7];
  int8_t fuzzy_e[7], fuzzy_de[7]; // fuzzyficacion
  int8_t du[4][2]; // reglas que gatillan
  int8_t area[200]; // area resultante
  int16_t r;
};
*/
struct FUZZY fuzzy[2] = {

    0,0,0,
    {
    -100, -55,			// NB
     -75, -30,			// NM
     -50,   0,			// NS
     -20,  20,			// ZE
       0,  50,			// PS
      30,  75,			// PM
      55,  100,			// PB
    },
    {	// [0 a 6]

      PB, PB, PB, PB, PM, PS, ZE,	//j
      PB, PB, PM, PM, PS, ZE, NS,
      PB, PM, PS, PS, ZE, NS, NM,
      PM, PM, PS, ZE, NS, NM, NM,
      PM, PS, ZE, NS, NS, NM, NB,
      PS, ZE, NS, NM, NM, NB, NB,
      ZE, NS, NM, NB, NB, NB, NB,
    //i
    },
    {0,0,0,0,0,0,0,},{0,0,0,0,0,0,0,},
    {{0,0,},{0,0,},{0,0,},{0,0,},},
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
    0,

    0,0,0,
    {
    -100, -55,			// NB
     -75, -30,			// NM
     -50,   0,			// NS
     -20,  20,			// ZE
       0,  50,			// PS
      30,  75,			// PM
      55,  100,			// PB
    },
    {	// [0 a 6]

      PB, PB, PB, PB, PM, PS, ZE,	//j
      PB, PB, PM, PM, PS, ZE, NS,
      PB, PM, PS, PS, ZE, NS, NM,
      PM, PM, PS, ZE, NS, NM, NM,
      PM, PS, ZE, NS, NS, NM, NB,
      PS, ZE, NS, NM, NM, NB, NB,
      ZE, NS, NM, NB, NB, NB, NB,
    //i
    },
    {0,0,0,0,0,0,0,},{0,0,0,0,0,0,0,},
    {{0,0,},{0,0,},{0,0,},{0,0,},},
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
    0,
};

int Fuzzy (struct FUZZY *f, int16_t referencia, int16_t posicion_actual) {
  uint8_t i=0, j=0, k=0;
  uint8_t k_tmp=0;
  int8_t altura=0;		// [0 a 100]
  int8_t resultado=0;		// [-100 a 100]
  float integral_x=0;		// [0 a 500000]
  uint16_t integral=0;		// [0 a 10000]

  int16_t e = referencia - posicion_actual;
  int16_t de = e - f->lastE;
  f->lastE = e;
/****************/
/* Fusificacion */
/****************/

#ifdef DEBUG
  printf ("e: %d de:%d\n", e, de);
  printf ("Fusificacion\n\n");
#endif


/* Fusificacion del error */
  for (i = 0; i < 7; i++)
    if ((f->parabolas[i][0] < e) && (e < f->parabolas[i][1]))
      {
        f->fuzzy_e[i] =
          (int) 100 *( (float) (e - f->parabolas[i][0]) * (e - f->parabolas[i][1]) ) /
          ( ((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) - f->parabolas[i][0]) *
           ((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) - f->parabolas[i][1]) );

#ifdef DEBUG
//printf("pillado en %d con el valor: %d\n",i,fuzzy_e[i]);
#endif
      }
    else
      f->fuzzy_e[i] = 0;

/* Fusificacion de la derivada del error */

  for (i = 0; i < 7; i++)
    if ((f->parabolas[i][0] < de) && (de < f->parabolas[i][1]))
      {
        f->fuzzy_de[i] =
          (int) 100 *((float) (de - f->parabolas[i][0]) *
                      (de -
                       f->parabolas[i][1])) /
          (((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) -
            f->parabolas[i][0]) *
           ((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) -
            f->parabolas[i][1]));
      }
    else
      f->fuzzy_de[i] = 0;


#ifdef DEBUG
  for (i = 0; i < 7; i++)
    printf ("fuzzy e[%d]: %d -- fuzzy de[%d]: %d\n", i, (int) f->fuzzy_e[i],
            i, (int) f->fuzzy_de[i]);
#endif


/*********************/
/* Reglas de control */
/*********************/

#ifdef DEBUG
  printf ("Reglas de Control\n\n");
#endif

/* que no debe ser otro valor que 0, 1, 2, 3 (solo 4 reglas deben disparar por los graficos de fusificacion)*/

  k = 0;
  for (i = 0; i < 7; i++)
    for (j = 0; j < 7; j++)
      {

        if (f->fuzzy_e[i] && f->fuzzy_de[j])
          {
            f->du[k][0] = f->reglas_control[i][j];
            if (f->fuzzy_e[i] < f->fuzzy_de[j])
              f->du[k][1] = f->fuzzy_e[i];
            else
              f->du[k][1] = f->fuzzy_de[j];
            k++;
          }
      }

#ifdef DEBUG
  for (i = 0; i < k; i++)
      printf ("du[%d][0]: %d (gatilló esta regla) -- du[%d][1]: %d (con tal umbral)\n", i, f->du[i][0], i, f->du[i][1]);
#endif


/******************/
/* Defusificacion */
/******************/

#ifdef DEBUG
  printf ("Defusificacion\n\n");
#endif

/* OR a las areas para usar el metodo de mamdani, se podria usar el metodo de sugeno para alivianar el proceso */

  for (i = 0; i < 200; i++)	// limpiando el arreglo
    f->area[i] = 0;

  k_tmp = k;
  for (k = 0; k < k_tmp; k++)
    {
#ifdef DEBUG
//	  printf ("cambio de k:%d\n", k);
#endif
      for (i = 0; i < 7; i++)
        {
#ifdef DEBUG
//	      printf ("cambio de i:%d\n", i);
#endif
          if (f->du[k][0] == i)
            {
              for (j = f->parabolas[i][0]; j <= f->parabolas[i][1]; j++)
                {
                  altura = (int) 100 *((float) (j - f->parabolas[i][0]) * (j - f->parabolas[i][1])) / (((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) - f->parabolas[i][0]) * ((float) ((f->parabolas[i][0] + f->parabolas[i][1]) / 2) - f->parabolas[i][1]));	//calculando la altura de la parabola
#ifdef DEBUG

//                printf ("altura[i:%d, j:%d]: %d\n", i, j, altura);
#endif
                  if (altura < f->du[k][1])	// cortando la parte de arriba del area
                    {	// altura dentro del rango
                      if (f->area[j + 100] < altura)	// haciendo OR a las areas
                        {
                          f->area[j + 100] = altura;
#ifdef DEBUG

//                        printf ("almacenando area[%d]: %d\n", j, area[j+100]);
#endif
                        }
                    }
                  else
                    {	// cortando el area a la altura de du[k][1]
                      if (f->area[j + 100] < f->du[k][1])
                        {	// haciendo OR a las areas
                          f->area[j + 100] = f->du[k][1];
#ifdef DEBUG
//                        printf ("almacenando corte area[%d]: %d\n", j, area[j+100]);
#endif
                        }
                    }
                }
            }
        }
    }


/* resultado =  sumatoria[f (x) * x] / sumatoria[f (x)] */

  for (j = 0; j < 200; j++)
    integral_x += (float) f->area[j] * (j - 100);


  for (j = 0; j < 200; j++)
    integral += f->area[j];

  resultado = (int) ((float) integral_x / integral);



#ifdef DEBUG
  for (i = 0; i < 200; i++)
    {
      printf ("area %d: %d \n", i-100, f->area[i]);
    }
  printf ("integral * x: %f\n", integral_x);
  printf ("integral: %d\n", integral);
  printf ("resultado: %d\n", resultado);
#endif


  return resultado;



}
