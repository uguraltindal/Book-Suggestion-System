#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define nKitap 8
#define kKisi 3
typedef struct kullanicilar{
	int deger[10];
	float benzerlik[30];
	int mostSim[3];
	
}USER;

void coefficient(USER *users,int nUser) 
{ 
  	int i,j,k;
  	int n;
    int topA,topB,topAB; 
    int sqrTop_a, sqrTop_b; 
	float corr;
	for(i=0; i < nUser-1; i++)	// T�m kullan�c�lar i�in yap�l�yor.
	{	
		for(j=i+1; j < nUser; j++)	// j=i+1 deme sebebimiz.U1 ve U15 de�eri �oktan kar��la�t�r�lm�� olacak.
		{							// Bu sayede efektif bir �ekilde U15'in benzerlik de�erlerine bakarken kontrole U16'dan ba�lay�p son de�ere kadar gidiyoruz. 
			topA = 0, topB = 0, topAB = 0,sqrTop_a = 0,sqrTop_b = 0; 
			n=nKitap;				
			for (k = 0; k < nKitap; k++) 	// Kitap say�m�z 8. Kullan�c�lar�n kitap puanlar�na g�re benzerliklerini burada al�yoruz.
		    { 
		    	if(users[i].deger[k]!=0 || users[j].deger[k]!=0) // Son payla��lan duyuruda 2 tane pearson hesaplama mant��� vard�.Ben 2. olan� tercih ettim yani 2 kullan�c� da bir kitab� okumad�ysa o kitab� hesaba katm�yorum.
		    	{												 // Bu if sayesinde 2'sinin de okumad��� kitaplar i�in o kitab� hesaba katm�yoruz.
		        topA += users[i].deger[k]; 						 
		   
		        topB += users[j].deger[k]; 
		  
		        topAB += users[i].deger[k] * users[j].deger[k]; 	// IF'e girdiysek gerekli i�lemleri yap�yoruz.Ad�m ad�m toplama ve karekok i�lemleri dahil.
		  
		        sqrTop_a += users[i].deger[k] * users[i].deger[k]; 
		        sqrTop_b += users[j].deger[k] * users[j].deger[k];
				}
				else
				{
					n--;				// IF'e girmediyse demek ki o kitab� 2 kullan�c� da okumam�� demektir.Bu kitab� hesaba katmad���m�z i�in bu 2 kullan�c�n�n kar��la�t�rmas�nda kitap say�s� de�erini 1 eksilttik.
				}
			
		    } 
		    float corr = (float)(n * topAB - topA * topB)  / sqrt((n * sqrTop_a - topA * topA)   * (n * sqrTop_b - topB * topB)); 
		   
			users[i].benzerlik[j]=corr;		// Benzerlik de�erlerini struct yap�s�nda saklad�k.
			users[j].benzerlik[i]=corr;		// Kar��la�t�r�lan kullan�c�lar�n benzerlik de�erlerini struct yap�s�ndaki o kullan�c�lar�n benzerlik arraylerine kaybettik.
		
		}
	
	}  

} 

int input(USER *users,char kitaplar[][30])		// Kullan�c�/Puanlar tablosunun kullan�c�/kontrol eden kolayl��� i�in puanlar.txt dosyas�ndan otomatik olarak �ekimi..
{
	int i=0;
	FILE *data=fopen("puanlar.txt","r");
	if(data==NULL)
	{
		printf("\n\n\tPlease make sure that the puanlar.txt file in the rar is in the same location as the .c extension code in order not to enter the values one by one while you are trying...");
	}
	else
	{
		printf("\n\n\tFile opened successfully.");
	}
	while(!feof(data)) // Program�n sonuna kadar gider ve degerleri satir satir almis olur.
	{
		
		fscanf(data,"%d %d %d %d %d %d %d %d",&users[i].deger[0],&users[i].deger[1],&users[i].deger[2],&users[i].deger[3],&users[i].deger[4],&users[i].deger[5],&users[i].deger[6],&users[i].deger[7]);
		i++;
	}
	// Degerleri tek tek elle girip surekli denemek zor olacagi icin dosyadan cekmeyi dusundum.
	// Bu sayede 25 kullan�c� ve 8 kitap icin 200 girdi girmek yerine size atacag�m dosya �zerinden �devde istenilen ��kt�lar� siz de g�rebilirsiniz.
	return i;	// dosyanin kac satir indigine gore kullanici sayimizi elde ettik.
	
}

void prediction(USER *users,int m,char kitaplar[][30],int indx)
{
	float topAranan;
	float topAranan2;
	float pay;
	int i,j,k;
	float topSim=0.0;
	int temp=8;
	int okunmayan[8];
	int count=0;
	float puanlar[8];
	
	printf("\n\n\t(NU%d) Books that user has not read      :    ",indx);
	for(i=0;i<kKisi;i++)
	{
		topSim+=users[m].benzerlik[users[m].mostSim[i]];		// Benzerlik de�erlerinin toplam�na b�lece�imiz i�in ilk ba�ta benzerlik de�erleri toplam�n� bulduk.
	}
	for(i=0;i<nKitap;i++)
	{
		if(users[m].deger[i]==0)			// Okunmayan kitaplar hesaba kat�lmayacak
		{
			okunmayan[count]=i;				// Okunmayan kitaplar�n index degerlerini ge�ici bir diziye att�k.
			count++;
			printf("(%s)   ",kitaplar[i]);	// Aranan kullan�c�n�n hangi kitaplar� okumad���n� yazd�rd�k.		
			temp--;
		}
		else
		{
			topAranan+=users[m].deger[i];
		}
	}
	topAranan=topAranan/temp;				// Aramak istedigimiz �RN(Nu1) i�in ortalama puan de�erini elde ettik.
	int temp2=8;
	printf("\n\n\t(NU%d) Score predictions for its user       :  ",indx); 	// Kullan�c�n�n okumad��� kitaplar�n hepsi i�in puan tahminlerini yazd�rd�k.
	for(i=0;i<count;i++)
	{
		pay=0;
		for(j=0;j<kKisi;j++)
		{
			temp2=8;
			topAranan2=0;

			for(k=0;k<nKitap;k++)
			{
				if(users[users[m].mostSim[j]].deger[k]==0)
				{
					temp2--;
				}
				else
				{
					topAranan2+=users[users[m].mostSim[j]].deger[k];
				}
			}

			topAranan2=topAranan2/temp2;		// Buras� Aranan Kullan�c�n�n en benzer oldu�u kullan�c�lar�n ortalama kitap puan de�erlerinin hesaplan�p aralar�ndaki benzerlikle �arp�ld��� k�s�m.
		
			topAranan2=users[users[m].mostSim[j]].deger[okunmayan[i]]-topAranan2;
			
			pay+=topAranan2*users[m].benzerlik[users[m].mostSim[j]];	// Pay k�sm� += ile en yak�n 3 ki�i istendi�i i�in toplanarak devam ediyor.
		}
	
		float sonuc=topAranan+(pay/topSim);		// En son topAranan = rA  pay = benzerlik* (rBkitap-rBort)  topSim=En ba�ta buldu�umuz en benzer 3 de�er i�in toplamBenzerlik de�eri.
		if(sonuc<0)
		{
			sonuc=0;							// Testlerime g�re ve Kullan�c�/Kitap puan listesinde genelde en benzer kullan�c�lar�n ayn� kitaplar� okumamas�yla olu�tu�u i�in 
		}										// ��nk� 2'si de o kitab� okumad�ysa o benzerlik hesaplamas�nda hesaba kat�lm�yor.
		printf("  (%s)  Estimated Point --> (%1.2f)  ",kitaplar[okunmayan[i]],sonuc);	// Okunmayan her kitap i�in tahmini puanlar
		puanlar[okunmayan[i]]=sonuc;			// Puanlar ge�ici dizisine puanlar� atarak orada en y�ksek puan� bulup hangi kitap oldu�unu kullan�c�ya �nerece�iz.
	}
		
		float max=puanlar[0];
		int index;
		for(i=0;i<nKitap;i++)
		{
			if(max<puanlar[i])
			{
				max=puanlar[i];					// En y�ksek puana sahip kitab�n bulunmas�
				index=i;
			}
		}
		
		printf("\n\n\t Recommended book for (NU%d)   :    (%s)    Estimated Point --> (%1.2f) \n",indx,kitaplar[index],max);	// En y�ksek puana sahip kitab�n �nerilmesi ve puan�n�n yazd�r�lmaas�
		
}

void mostSims(USER *users,int nUser,int m)			// En benzer kullan�c�lar� kaydetti�imiz fonksiyon
{
	int i,j;
    float max; 
	int index;
	float temp[30];
	int count=0;

	for(i=0; i < nUser; i++)
	{
		if(i!=m)
		{
			temp[i]=users[m].benzerlik[i];		// Benzerlik de�erlerini �nce ge�ici diziye att�k kontorl i�in ve users listemizi bozmamak i�in.
			count++;
			if(count==8)
			{
				count=0;
			}
		}
		
	}
    for (j = 0; j < kKisi; j++) 				// �stenilen ki�i say�s�
	{
        max = temp[0];
        index = 0;
        for (i = 1; i < nUser; i++) 
		{
            if (max < temp[i] && i!=m) 		    // SORUNUN 2.�IKKINDA �STEND��� G�B� T�M DE�ERLER� SIRALAMADIM.
			{									// �stenilen ki�i say�s� kadar i�lem yapt�m.Diziyi s�ralam�� olsayd�m s�ralama algoritmalar�na g�re (n^2) veya (n log n) gibi de�erlerle bu i�lemi yapacakt�m.
                max = temp[i];					// Fakat burada �rne�in ki�i say�s� 3 ise O(n)*3 ile i�lemi yap�yorum ve zaman karma��kl���m bu y�zden O(n) ile max benzerlikleri elde ediyorum.
                index = i;
            }
        }
        users[m].mostSim[j] = index;
        temp[index]=-1.0;
    }
    
}
int main()
{
	int nUser;			// Kullan�c� say�s�
	char kitaplar[10][30];	// Kitaplar�n oldugu dizi
	int i,j,m;
	USER users[30];		// Kullan�c� listesi
	char secim;
	int indx=1;
	printf("\n\t Enter the books that the readers have read in order. (This section is symbolic. You can enter the values you want in the book names. Ex : Harry Potter, Attack on Titan, Lord of the Rings) \n\n");
	for(j=0;j < nKitap; j++)
	{
		printf("\n %d.Book Name : ",j+1);
		gets(kitaplar[j]);
	}
	nUser=input(users,kitaplar);
	printf("\n\n\t%d user information was extracted from the file..\n\n",nUser);
	coefficient(users,nUser);
	for(m=21;m<26;m++)
	{
		mostSims(users,nUser,m-1);
		printf("\n\n"); 
		printf("\n\t(NU%d) 3 people whose user is MOST SIMILAR  :   ",indx);
		
		for(j=0;j<kKisi;j++)
			{
				printf(" (U%d) ",users[m-1].mostSim[j]+1);
			}
		
		prediction(users,m-1,kitaplar,indx);
		indx++;
		sleep(1);
	}
	
	

	printf("\n\n\n\n\t\t\t\t Closing...\n\n");
	sleep(2);
	
	return 0;
}
