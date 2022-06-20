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
	for(i=0; i < nUser-1; i++)	// Tüm kullanýcýlar için yapýlýyor.
	{	
		for(j=i+1; j < nUser; j++)	// j=i+1 deme sebebimiz.U1 ve U15 deðeri çoktan karþýlaþtýrýlmýþ olacak.
		{							// Bu sayede efektif bir þekilde U15'in benzerlik deðerlerine bakarken kontrole U16'dan baþlayýp son deðere kadar gidiyoruz. 
			topA = 0, topB = 0, topAB = 0,sqrTop_a = 0,sqrTop_b = 0; 
			n=nKitap;				
			for (k = 0; k < nKitap; k++) 	// Kitap sayýmýz 8. Kullanýcýlarýn kitap puanlarýna göre benzerliklerini burada alýyoruz.
		    { 
		    	if(users[i].deger[k]!=0 || users[j].deger[k]!=0) // Son paylaþýlan duyuruda 2 tane pearson hesaplama mantýðý vardý.Ben 2. olaný tercih ettim yani 2 kullanýcý da bir kitabý okumadýysa o kitabý hesaba katmýyorum.
		    	{												 // Bu if sayesinde 2'sinin de okumadýðý kitaplar için o kitabý hesaba katmýyoruz.
		        topA += users[i].deger[k]; 						 
		   
		        topB += users[j].deger[k]; 
		  
		        topAB += users[i].deger[k] * users[j].deger[k]; 	// IF'e girdiysek gerekli iþlemleri yapýyoruz.Adým adým toplama ve karekok iþlemleri dahil.
		  
		        sqrTop_a += users[i].deger[k] * users[i].deger[k]; 
		        sqrTop_b += users[j].deger[k] * users[j].deger[k];
				}
				else
				{
					n--;				// IF'e girmediyse demek ki o kitabý 2 kullanýcý da okumamýþ demektir.Bu kitabý hesaba katmadýðýmýz için bu 2 kullanýcýnýn karþýlaþtýrmasýnda kitap sayýsý deðerini 1 eksilttik.
				}
			
		    } 
		    float corr = (float)(n * topAB - topA * topB)  / sqrt((n * sqrTop_a - topA * topA)   * (n * sqrTop_b - topB * topB)); 
		   
			users[i].benzerlik[j]=corr;		// Benzerlik deðerlerini struct yapýsýnda sakladýk.
			users[j].benzerlik[i]=corr;		// Karþýlaþtýrýlan kullanýcýlarýn benzerlik deðerlerini struct yapýsýndaki o kullanýcýlarýn benzerlik arraylerine kaybettik.
		
		}
	
	}  

} 

int input(USER *users,char kitaplar[][30])		// Kullanýcý/Puanlar tablosunun kullanýcý/kontrol eden kolaylýðý için puanlar.txt dosyasýndan otomatik olarak çekimi..
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
	while(!feof(data)) // Programýn sonuna kadar gider ve degerleri satir satir almis olur.
	{
		
		fscanf(data,"%d %d %d %d %d %d %d %d",&users[i].deger[0],&users[i].deger[1],&users[i].deger[2],&users[i].deger[3],&users[i].deger[4],&users[i].deger[5],&users[i].deger[6],&users[i].deger[7]);
		i++;
	}
	// Degerleri tek tek elle girip surekli denemek zor olacagi icin dosyadan cekmeyi dusundum.
	// Bu sayede 25 kullanýcý ve 8 kitap icin 200 girdi girmek yerine size atacagým dosya üzerinden ödevde istenilen çýktýlarý siz de görebilirsiniz.
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
		topSim+=users[m].benzerlik[users[m].mostSim[i]];		// Benzerlik deðerlerinin toplamýna böleceðimiz için ilk baþta benzerlik deðerleri toplamýný bulduk.
	}
	for(i=0;i<nKitap;i++)
	{
		if(users[m].deger[i]==0)			// Okunmayan kitaplar hesaba katýlmayacak
		{
			okunmayan[count]=i;				// Okunmayan kitaplarýn index degerlerini geçici bir diziye attýk.
			count++;
			printf("(%s)   ",kitaplar[i]);	// Aranan kullanýcýnýn hangi kitaplarý okumadýðýný yazdýrdýk.		
			temp--;
		}
		else
		{
			topAranan+=users[m].deger[i];
		}
	}
	topAranan=topAranan/temp;				// Aramak istedigimiz ÖRN(Nu1) için ortalama puan deðerini elde ettik.
	int temp2=8;
	printf("\n\n\t(NU%d) Score predictions for its user       :  ",indx); 	// Kullanýcýnýn okumadýðý kitaplarýn hepsi için puan tahminlerini yazdýrdýk.
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

			topAranan2=topAranan2/temp2;		// Burasý Aranan Kullanýcýnýn en benzer olduðu kullanýcýlarýn ortalama kitap puan deðerlerinin hesaplanýp aralarýndaki benzerlikle çarpýldýðý kýsým.
		
			topAranan2=users[users[m].mostSim[j]].deger[okunmayan[i]]-topAranan2;
			
			pay+=topAranan2*users[m].benzerlik[users[m].mostSim[j]];	// Pay kýsmý += ile en yakýn 3 kiþi istendiði için toplanarak devam ediyor.
		}
	
		float sonuc=topAranan+(pay/topSim);		// En son topAranan = rA  pay = benzerlik* (rBkitap-rBort)  topSim=En baþta bulduðumuz en benzer 3 deðer için toplamBenzerlik deðeri.
		if(sonuc<0)
		{
			sonuc=0;							// Testlerime göre ve Kullanýcý/Kitap puan listesinde genelde en benzer kullanýcýlarýn ayný kitaplarý okumamasýyla oluþtuðu için 
		}										// Çünkü 2'si de o kitabý okumadýysa o benzerlik hesaplamasýnda hesaba katýlmýyor.
		printf("  (%s)  Estimated Point --> (%1.2f)  ",kitaplar[okunmayan[i]],sonuc);	// Okunmayan her kitap için tahmini puanlar
		puanlar[okunmayan[i]]=sonuc;			// Puanlar geçici dizisine puanlarý atarak orada en yüksek puaný bulup hangi kitap olduðunu kullanýcýya önereceðiz.
	}
		
		float max=puanlar[0];
		int index;
		for(i=0;i<nKitap;i++)
		{
			if(max<puanlar[i])
			{
				max=puanlar[i];					// En yüksek puana sahip kitabýn bulunmasý
				index=i;
			}
		}
		
		printf("\n\n\t Recommended book for (NU%d)   :    (%s)    Estimated Point --> (%1.2f) \n",indx,kitaplar[index],max);	// En yüksek puana sahip kitabýn önerilmesi ve puanýnýn yazdýrýlmaasý
		
}

void mostSims(USER *users,int nUser,int m)			// En benzer kullanýcýlarý kaydettiðimiz fonksiyon
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
			temp[i]=users[m].benzerlik[i];		// Benzerlik deðerlerini önce geçici diziye attýk kontorl için ve users listemizi bozmamak için.
			count++;
			if(count==8)
			{
				count=0;
			}
		}
		
	}
    for (j = 0; j < kKisi; j++) 				// Ýstenilen kiþi sayýsý
	{
        max = temp[0];
        index = 0;
        for (i = 1; i < nUser; i++) 
		{
            if (max < temp[i] && i!=m) 		    // SORUNUN 2.ÞIKKINDA ÝSTENDÝÐÝ GÝBÝ TÜM DEÐERLERÝ SIRALAMADIM.
			{									// Ýstenilen kiþi sayýsý kadar iþlem yaptým.Diziyi sýralamýþ olsaydým sýralama algoritmalarýna göre (n^2) veya (n log n) gibi deðerlerle bu iþlemi yapacaktým.
                max = temp[i];					// Fakat burada örneðin kiþi sayýsý 3 ise O(n)*3 ile iþlemi yapýyorum ve zaman karmaþýklýðým bu yüzden O(n) ile max benzerlikleri elde ediyorum.
                index = i;
            }
        }
        users[m].mostSim[j] = index;
        temp[index]=-1.0;
    }
    
}
int main()
{
	int nUser;			// Kullanýcý sayýsý
	char kitaplar[10][30];	// Kitaplarýn oldugu dizi
	int i,j,m;
	USER users[30];		// Kullanýcý listesi
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
