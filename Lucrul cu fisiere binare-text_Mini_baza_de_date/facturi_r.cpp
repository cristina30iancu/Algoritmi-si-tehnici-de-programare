#include <stdio.h>
#include <conio.h>
#include<string.h>
#pragma warning(disable:4996)

/*Alegeți un tip de entități pentru care să stocați informații într-un fișier binar
 organizat relativ: trebuie să stabiliți o cheie relativă (numerică, valori în secvență).
Stabiliți modelul informațional al entităților (cîmpurile care se reprezintă).*/

struct Factura {      //definirea entitatii alese
	char is;
	int cod;      //cheie relativa fisier binar 
	char nume[30];
	int an;
	float valoare;
};

int dimensiune_fisier(FILE* f, int rec_size)
{
	long crt_pos;
	int dim;
	crt_pos = ftell(f);
	fseek(f, 0, SEEK_END);
	dim = ftell(f) / rec_size;
	fseek(f, crt_pos, SEEK_SET);
	return dim;
}

/*Creați și populați un fișier binar organizat relativ cu articole care descriu entitățile
 alese. Datele de intrare trebuie preluate de la tastatură. Trebuie introduse cel puțin
20 de articole valide.*/
void Creare_populare()
{
	printf("Puteti adauga facturi noi, sau tastati 0 pentru a nu adauga mai multe.\n");
	char numef[30] = "Facturi.dat";
	FILE* f;
	Factura x;
	int i, codcheie, dim;
	fopen_s(&f, numef, "wb");
	//fopen_s(&f, numef, "rb+"); //am ales rb+ in loc de wb pentru a nu pierde datele deja introduse, se da uncomment aici si comment sus 
	int nr = 0;
	printf("\n%d.Codul facturii: ", ++nr);
	scanf_s("%d", &codcheie);
	while (!feof(stdin) && (codcheie))
	{
		dim = dimensiune_fisier(f, sizeof(Factura));
		if (codcheie >= dim)
		{
			x.is = 0;
			fseek(f, 0, SEEK_END);
			for (i = 0;i < codcheie - dim;i++)
				fwrite(&x, sizeof(Factura), 1, f);
		}
		fseek(f, codcheie * sizeof(Factura), SEEK_SET);
		fread(&x, sizeof(Factura), 1, f);
		if (x.is == 1)
			printf("\nNu putem adauga o factura noua cu codul %d deoarece este ocupat de factura %s!\n", codcheie, x.nume);
		else
		{
			x.cod = codcheie;
			printf_s("\tNume: ");
			gets_s(x.nume);
			gets_s(x.nume);
			printf_s("\tAn: ");
			scanf_s("%d", &x.an);
			printf("\tValoare (RON):");
			scanf_s("%f", &x.valoare);
			x.is = 1;
			fseek(f, codcheie * sizeof(Factura), SEEK_SET);
			fwrite(&x, sizeof(Factura), 1, f);
		}
		printf("\n%d.Codul facturii: ", ++nr);
		scanf_s("%d", &codcheie);
	}
	fclose(f);
	printf("\n\Fisierul <%s> a fost creat cu succes! Apasati o tasta pentru a continua.", numef);
	_getch();
}

/* consultare integrală a fișierului creat. */
void Afisare_tastatura()
{
	char numef[30] = "Facturi.dat";
	char numefistext[30] = "Lista_facturi.txt";
	FILE* f, *g;
	Factura x;
	int crt, i, codcheie, dim;
	fopen_s(&f, numef, "rb");
	if (!f)
		printf_s("\nNu s-a putut deschide fisierul.");
	else
	{
		fopen_s(&g, numefistext, "wt");
		fprintf(g, "  \n               LISTA FACTURI  \n");
		fprintf(g, "   ===================================================  \n");
		fprintf(g, "   ||CRT||COD||        NUME        || AN ||  VALOARE||   \n");
		fprintf(g, "   ===================================================  \n");
		printf("\nPentru a vizualiza facturile, apasati o tasta:\n");
		_getch();
		printf("   \n                 LISTA FACTURI  \n");
		printf("   ===================================================  \n");
		printf("   ||CRT||COD||        NUME        || AN ||  VALOARE|| \n");
		printf("   =================================================== \n");

		crt = 0;
		fread(&x, sizeof(Factura), 1, f);
		while (!feof(f))
		{
			if (x.is == 1)
			{
				printf("   ||%3d||%3d||%20s||%4d||  %4.3f||\n", ++crt, x.cod, x.nume, x.an, x.valoare);
				fprintf_s(g, "   ||%3d||%3d||%20s||%4d||  %4.3f||\n", crt, x.cod, x.nume, x.an, x.valoare);
			}
			fread(&x, sizeof(Factura), 1, f);
		}
		fclose(g);
		fclose(f);
		printf_s("\n\tFisierul <%s> a fost creat cu succes!", numefistext);
	}
	printf_s("\n\nApasati o tasta pentru a continua.");
	_getch();
}

/*operații de consultare si modificare pentru factura identificata unic prin valoarea cheii. */
void Gasire_Modificare_Cheie()
{
	char numef[30] = "Facturi.dat";
	FILE* f;
	fopen_s(&f, numef, "rb+");
	int codf;
	float valoare;
	printf_s("\n\n\tCodul facturii cautate: ");
	scanf("%d", &codf);
	while ((codf != 0) && (!feof(f)))
	{
		Factura x;
		int dim = dimensiune_fisier(f, sizeof(Factura));
		if (codf >= dim)
			printf_s("\nNu exista factura cu acest cod. Incercati din nou?");
		else
		{
			fseek(f, codf * sizeof(Factura), SEEK_SET);
			fread(&x, sizeof(Factura), 1, f);
			if (x.is == 0)
				printf("\nNu exista factura cu acest cod. Incercati din nou?");
			else
			{
				printf("   \n               LISTA FACTURI  \n");
				printf("   ============================================  \n");
				printf("   ||COD||        NUME        || AN ||VALOARE|| \n");
				printf("   ============================================  \n");
				printf("   ||%3d||%20s||%4d||%4.3f||\n", x.cod, x.nume, x.an, x.valoare);
				//Modificarea:
				printf_s("\nDaca doriti sa modificati valoarea facturii, tastati-o aici. Altfel, tastati ""0 "": ");
				getc(stdin);
				scanf_s("%f", &valoare);
				if (valoare != 0)
				{
					x.valoare = valoare;
					fseek(f, ftell(f) - sizeof(Factura), 0);
					fwrite(&x, sizeof(Factura), 1, f);
					fseek(f, 0, 1);
					printf("\n\tValoare schimbata cu succes!");
					_getch();
				}
				else printf_s("\nNicio schimbare efectuata.");
			}
		}
		printf_s("\n\tCodul facturii cautate: ");
		scanf_s("%d", &codf);
	}
	fclose(f);
	printf_s("\nCautare finalizata.Apasati o tasta.");
	_getch();
}

/*2. operații de consultare si modificare pentru factura identificata unic prin valoarea unui alt camp. (am ales nume) */
void Gasire_Modificare_Nume()
{
	char numef[30] = "Facturi.dat";
	FILE* f;
	Factura x;
	int vb, j;
	float valoare;
	char nume[30];
	fopen_s(&f, numef, "rb+");
	if (!f)
		printf_s("\nNu s-a putut deschide fisierul.");
	else
	{
		printf("\n\n\nIncepe cautarea dupa nume. Tastati ""stop"" daca doriti sa va opriti. ");
		getc(stdin);
		printf_s("\n\n\tNumele facturii cautate(sau stop): ");
		gets_s(nume);
		while (strcmp("stop", nume) != 0)
		{
			rewind(f);
			vb = 0;
			fread(&x, sizeof(Factura), 1, f);
			while (!feof(f))
			{
				if ((strcmp(x.nume, nume) == 0)&&vb==0) 
				{
					vb = 1; 
					printf("   \n               LISTA FACTURI  \n");
					printf("   ============================================  \n");
					printf("   ||COD||        NUME        || AN ||VALOARE|| \n");
					printf("   ============================================  \n");
					printf("   ||%3d||%20s||%4d||%4.3f||\n", x.cod, x.nume, x.an, x.valoare);
					printf_s("\nDaca doriti sa modificati valoarea facturii, tastati-o aici. Altfel, tastati ""0"": ");
					scanf_s("%f", &valoare);
					if (valoare != 0)
					{
						x.valoare = valoare;
						fseek(f, ftell(f) - sizeof(Factura), 0);
						fwrite(&x, sizeof(Factura), 1, f);
						fseek(f, 0, 1);
						printf("\n\tValoare schimbata cu succes!");
					}
					else printf_s("\nNicio schimbare efectuata.\n");
				}
				fread(&x, sizeof(Factura), 1, f);
			}
			if (!vb)   printf_s("\nNu exista factura cu acest nume!");
			getc(stdin);
			printf_s("\nDaca doriti sa cautati alta factura, tastati numele facturii (sau stop): "); 
			gets_s(nume);
		}
		fclose(f);
	}
	printf_s("\nCautare finalizata.Apasati o tasta pentru a continua.");
	_getch();
}

/*3. operații de consultare si modificare pentru un grup de facturi identificate printr-o valoare comună a unui camp(an)*/

void Gasire_Modificare_An()
{
	char numef[30] = "Facturi.dat";
	FILE* f;
	Factura x;
	int vb, st, j;
	int an;
	float valoare;
	fopen_s(&f, numef, "rb+");
	if (!f)
		printf_s("\nNu s-a putut deschide fisierul.");
	else
	{
		printf("\n\n\nIncepe cautarea dupa an. Tastati ""0"" daca doriti sa va opriti. ");
		printf_s("\n\nAnul pentru care se realizeaza cautarea(sau 0): ");
		scanf("%d", &an);
		while (an != 0)
		{
			rewind(f);
			vb = 0;
			fread(&x, sizeof(Factura), 1, f);
			printf("\n\t\tAfisarea facturilor din anul %d:", an);
			while (!feof(f))
			{
				if ((x.is == 1) && (x.an == an))
				{
					vb = 1;
					printf(" \n  ============================================  \n");
					printf("   ||%3d||%20s||%4d||%4.3f||\n", x.cod, x.nume, x.an, x.valoare);
					printf_s("\nDaca doriti sa modificati valoarea facturii, tastati-o aici. Altfel, tastati ""0"": ");
					getc(stdin);
					scanf_s("%f", &valoare);
					if (valoare != 0)
					{
						x.valoare = valoare;
						fseek(f, ftell(f) - sizeof(Factura), 0);
						fwrite(&x, sizeof(Factura), 1, f);
						fseek(f, 0, 1);
						printf("\n\tValoare schimbata cu succes!");
						_getch();
					}
					else   printf_s("\n\tNicio schimbare efectuata.");
				}
				fread(&x, sizeof(Factura), 1, f);
			}
			if (!vb)
			{
				printf_s("\nNu exista factura din acest an.");
				//printf_s("\n Incercati din nou? An cautat: ");
				//scanf_s("%d", &an);
			}

			printf_s("\n\n Doriti sa cautati alt an? An cautat (sau 0): ");
			scanf_s("%d", &an);

		}
		fclose(f);
	}
	printf_s("\nGata.Apasati o tasta pentru a termina.");
	_getch();
}
void main()
{  
	Creare_populare(); //se comenteaza acest subprogram daca nu se doreste adaugarea altor facturi sau pierderea datelor, in cazul in care fisierul binar e deschis cu wb
	Afisare_tastatura();
	Gasire_Modificare_Cheie();
    Gasire_Modificare_Nume();
	Gasire_Modificare_An();
}