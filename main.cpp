#include <stdlib.h>    
#include <GL/glut.h>
#include<stdio.h>
#include<string.h>
#include <math.h>
#include <iostream>
#include <vector>


/*
Oyunun amac� bir top vas�tas� ile �stte duran bloklar� patlatmak.
Oyunda hareket ettirilen bir bar var bu sayede topun d��mesini engelliyoruz.
Bar� A ve D tu�lar� ayn� zamanda sa� ve sol y�n tu�lar� ile hareket ettirebiliriz.
Space ve yukar� y�n tu�u ile oyunu ba�latabiliriz.
Esc ve alt y�n tu�u ile oyundan ��kabiliriz.
Toplamda 50 adet kutu olup hepsini patlatt���m�zda oyunu kazan�yoruz.
Bir adet score tabelam�z var bunun sayesinde skoru g�rebiliyoruz skorumuz 50 oldu�unda ekranda kazand���m�z yaz�yor ve oyuna tekrardan ba�l�yoruz.
*/

	int score = 0; // Bu de�i�ken sayesinde skoru tutup oyunun biti�ini kontrol ediyoruz
	int brick_color = 1,text_color = 5,size = 1;
	float rate = 7;
	int rows = 5; // sat�r adedi
	int columns = 10; //sut�n adedi
    int width = 500, height = 500;
		
    GLfloat Ball_specular[] = { 0.8, 0.7, 0.2, 0.5 };
    GLfloat Ball_diffuse[] = { 0.8, 0.7, 0.2, 0.5};
    GLfloat Ball_shininess[] = { 15.0 };   // Topun I��kland�rmas� i�in yaz�ld�
    GLfloat Bar_specular[] = {10.0, 10.0, 10.0, 10.0 };
    GLfloat Bar_diffuse[] = { 10.0, 10.0, 10.0, 10.0 };
    GLfloat Bar_shininess[] = { 50.0 };  // Bar�n ���kland�rmas� 
    GLfloat Lives_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

 
	GLfloat paddle_color_array[][3] = {{1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1}}; // Bar renk dizisi gerek duyu
	GLfloat text_color_array[][4] = {{1,0,0,1},{0,1,0,1},{0,0,0,1},{1,0,1,0},{1,1,0,1},{1,1,1,1}}; // Yaz� renk dizisi gerek duyu
	GLfloat paddle_size[] = {2,4,6}; // Bar�n �izimi 
	GLfloat ambient1[] = {1,1,1};
	GLfloat px,start = 0;  // Bar hareketi i�in
	GLfloat bx = 0,by = -12.8 ;  // topun hareketi i�in gerekli 
	GLfloat dirx=0,diry=0;   // Topun H�z�

// Kordinatlar i�in yap�
struct brick_coords{
	
	GLfloat x;
	GLfloat y;
};

//Kutular�n kondu�u dizi
brick_coords brick_array[50][50];




void BarCiz() // Bar�n �izildi�i fonksiyon
{
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Bar_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Bar_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, Bar_shininess);
	glBegin(GL_POLYGON);
	glVertex3f(-paddle_size[size]+px,0-15,0);
	glVertex3f(paddle_size[size]+px,0-15,0);
	glVertex3f(paddle_size[size]+px,1-15,0);
	glVertex3f(-paddle_size[size]+px,1-15,0);
	glEnd();

}


void brick(GLfloat x,GLfloat y, GLfloat z) // Tekli kutunun �izildi�i yer
{	
	glDisable(GL_LIGHTING);
	glColor3ub(255,0,0);
	glBegin(GL_QUADS);
	glVertex3f(x,y,z);
	glVertex3f(x+3,y,z);
	glVertex3f(x+3,y+1,z);
	glVertex3f(x,y+1,z);       
	glEnd();
	glEnable(GL_LIGHTING);
}


void draw_bricks() //�izilen kutunun �o�alt�l�p yerle�tirildi�i fonksiyon
{
	
	int i,j;
	if(start == 0)
	{
		for(i = 1;i<=rows;i++)
		{
			for(j = 1;j<=columns;j++) // Aralar�nda ki uzakl���n verildi�i yer
			{   
				brick_array[i][j].x = (GLfloat)(j*3.5*0.9);
				brick_array[i][j].y = (GLfloat)(i*2*0.8) ;
			}
		}
	}
	
	//Yerle�tirildi�i yer
	glPushMatrix();
		glTranslatef(-19.5,5,0);
	
		for(i = 1;i<=rows;i+=1)
		{
			for(j = 1;j<=columns;j+=1)
			{
				if(!(brick_array[i][j].x==0 || brick_array[i][j].y ==0))
				{
					brick(brick_array[i][j].x,brick_array[i][j].y,0);
				}
			}
		}
	glPopMatrix();
	
}

void TopCiz() // Topun �izildi�i fonsiyon
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Ball_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ball_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, Ball_shininess);	
	glEnable(GL_LIGHTING);
	glTranslatef(bx,by,0);
	glScalef(1.0, 1.0, 0.5);
	glutSolidSphere(1.0, 52, 52);	   
	glPopMatrix();
	
}
void init (){ //I��kland�rman�n ayarland��� fonksiyon. I����n rengi pozisyonu burda ayarland�.
	GLfloat light0_position[] = { -25, 55, 0 };
	GLfloat light1_position[] = { 25, -55, 0 };
	GLfloat light0_color[] = { 1.0, 1.0, 1.0, 50.0 };
	GLfloat light1_color[] = { 1.0, 1.0, 1.0, 0.0 };


	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	
}
// mouse ile bar�n hareketi sa�lan�yor
void mousemotion(int x,int y)
{
  
  if(start == 1)
  {
    px=(x-glutGet(GLUT_WINDOW_WIDTH)/2)/20;
    if(px>15)
	  {
		  px=15;
	  }
	  if(px<-15)
	  {
		  px=-15;
	  }
    }
    
    else glutSetCursor(GLUT_CURSOR_INHERIT);
}





// Ekranda skorun tutulmas�na yarayan fonksiyon. Ayn� zamanda oyunun bitti�ini anlad���m�z yer.
void text( int score)
{
	glDisable(GL_LIGHTING);
	char text[40];
	if(score <50)
	sprintf(text,"Skor: %d   ",sc);
	else
	{
	  sprintf(text,"Kazand�n�z Tebrikler !!");
	  start = 0;
	  by = -12.8;
	  bx = 0;
	  dirx = 0;
	  diry = 0;
	  px = 0;	

	}
	// The color
	glColor4fv(text_color_array[text_color]);
	// Position of the text to be printer
	glPushMatrix();
	glTranslatef(-1,0,0);
	glRasterPos3f(0, 0, 20);
	for(int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void display (void) { // Yukar�da yazd���m�z fonksiyonlar� �a��r�p �al��t�r��m�z k�s�m
	
	glClearColor (0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,0,0,0,-25,0,1,0);
	glTranslatef(0,0,-25);
	BarCiz(); 
	draw_bricks();
	TopCiz();
	text(score);
	glutSwapBuffers();
}
void reshape (int w, int h) {
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode (GL_MODELVIEW);
}
void hit()  // Kutular� vurdu�umuz ve onlar� vurulduktan sonra yok etti�imiz fonksiyon
{
	//vurma ve kutular� yok etme fonksiyonu
	int i,j;
	for(i = 1;i<=rows;i++)
		for(j=1;j<=columns;j++)
		{
			// Alttan veya �stten vurmas� durumunda kutunun yok olmas�
			if((bx>=brick_array[i][j].x-19.5-0.1 )&&( bx<=brick_array[i][j].x + 3-19.5+ 0.1))  
			{
				if(by >=brick_array[i][j].y+5-0.1 && by <=brick_array[i][j].y+5 +1.2 +0.1)
				{
					//kutular� yok etmek i�in ekrandan kutuyu ��kar�yoruz
					brick_array[i][j].x = 0; 
					brick_array[i][j].y = 0;
					diry= diry*-1;
				// Her bir kutu yok olduktan sonra skor art�yor
					score++;
				}
			
			}
			else if(by >=brick_array[i][j].y+5 -0.1 && by <=brick_array[i][j].y+5 +1.2+0.1)  // Yanlardan vurmas� durumunda kutunun yok olma durumu
			{
				if((bx>=brick_array[i][j].x-19.5 -0.1)&&( bx<=brick_array[i][j].x + 3-19.5 + 0.1 ))
				{
					brick_array[i][j].x = 0;
					brick_array[i][j].y = 0;
					dirx= dirx*-1;
					score++;				
				}
				
			}
		}
}

// s�rekli �al��acak idle fonksiyonu. Bu fonksiyonla birlikte ekranda vurulan karelerin �izilmemesi
// ve topun hareketli g�r�nmesi sa�lan�yor.
void idle()
{
	hit();
	if(bx<-16 || bx>16 && start == 1)
	{
		dirx = dirx*-1;
	}
	if(by<-15 || by>14 && start == 1)
	{
		diry = diry*-1;
	}
	bx+=dirx/(rate);
	by+=diry/(rate);
	// topun h�z�
	rate-=0.0001;
	
	float x = paddle_size[size];
	
	//Bu k�s�mda topun y�n� ve h�z� d�zenleniyor
	// alttaki blo�a �arp�p �arpmad���na ba�l� olarak.
		if( by<=-12.8 && bx<(px+x*2/3) && bx>(px+x/3)&& start == 1 )
	{
		dirx = 0.1;
		diry = 0.1;
	}
	else if(by <=-12.8 && bx<(px-x/3) && bx>(px-x*2/3) && start == 1 )
	{
	dirx = 0.1;
		diry = 0.1;
	}
	else if( by<=-12.8 && bx<(px+x/3) &&bx>(px-x/3) && start == 1)
	{
		dirx = dirx;
		diry = 0.1;
	}
	else if(by <=-12.8 && bx<(px-(x*2/3)) && bx>(px-(x+0.3)) && start == 1 )
	{
		dirx = 0.1;
		diry = 0.1;
	}
	else if(by<=-12.8 && bx<(px+(x+0.3)) && bx>(px+x/3)&& start == 1 )
	{
		dirx = 0.1;
		diry = 0.1;
	}
	else if(by<-13)
	{
		start = 0;
		by = -12.8;
		bx = 0;
		dirx = 0;
		diry = 0;
		px = 0;	
	}

	glutPostRedisplay();
}
void ozel_klavye(int key, int x, int y){ // Y�n tu�lar� ile hareket sa�lan�yor
		int specialKey = glutGetModifiers();
		switch (key) {
			
			case GLUT_KEY_LEFT :px-=3; break;
			case GLUT_KEY_RIGHT: px+=3; break;
			case GLUT_KEY_DOWN:  exit(0); break;
			case GLUT_KEY_UP : if(!start)
		{
			dirx = diry= 0.3;
			start = 1;
			score = 0;
			glutSetCursor(GLUT_CURSOR_NONE);
			
		}
		break;
	}
	if(px>15)
	{
		px=15;
	}
	if(px<-15)
	{
		px=-15;
	}
	if(start== 0)
	{
		px=0;
	}
	glutPostRedisplay();
		
	
}


void keyboard (unsigned char key, int x, int y) // Klavyeden bas�lan tu�lar ile hareket sa�lan�yor
{
	switch(key)
	{
		case 'd': px+=3; break;
		case 'a': px-=3; break;
		case 25: exit(0); break;
		case 32:
		if(!start)
		{
			dirx = diry= 0.3;
			start = 1;
			score = 0;
			glutSetCursor(GLUT_CURSOR_NONE);
			
		}
		break;
	}
	if(px>15)
	{
		px=15;
	}
	if(px<-15)
	{
		px=-15;
	}
	if(start== 0)
	{
		px=0;
	}
	glutPostRedisplay();
}




void timer(int value){ // Topun oynanan s�re� i�erisinde h�zlanmas� sa�lan�yor
	dirx *= 1.01; // Top her saniye %10 h�zlan�r
	diry *= 1.01;
	glutTimerFunc(1000, timer, 0);
}



int main (int argc,char **argv) {
	glEnable(GL_DEPTH);
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Brick Breaker");
	init();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc (idle);
	glutPassiveMotionFunc(mousemotion);
	glutKeyboardFunc(keyboard);
		glutSpecialFunc(ozel_klavye);
	glutMainLoop ();
	return 0;
}
