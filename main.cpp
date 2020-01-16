#include <stdlib.h>    
#include <GL/glut.h>
#include<stdio.h>
#include<string.h>
#include <math.h>
#include <iostream>
#include <vector>


/*
Oyunun amacý bir top vasýtasý ile üstte duran bloklarý patlatmak.
Oyunda hareket ettirilen bir bar var bu sayede topun düþmesini engelliyoruz.
Barý A ve D tuþlarý ayný zamanda sað ve sol yön tuþlarý ile hareket ettirebiliriz.
Space ve yukarý yön tuþu ile oyunu baþlatabiliriz.
Esc ve alt yön tuþu ile oyundan çýkabiliriz.
Toplamda 50 adet kutu olup hepsini patlattýðýmýzda oyunu kazanýyoruz.
Bir adet score tabelamýz var bunun sayesinde skoru görebiliyoruz skorumuz 50 olduðunda ekranda kazandýðýmýz yazýyor ve oyuna tekrardan baþlýyoruz.
*/

	int score = 0; // Bu deðiþken sayesinde skoru tutup oyunun bitiþini kontrol ediyoruz
	int brick_color = 1,text_color = 5,size = 1;
	float rate = 7;
	int rows = 5; // satýr adedi
	int columns = 10; //sutün adedi
    int width = 500, height = 500;
		
    GLfloat Ball_specular[] = { 0.8, 0.7, 0.2, 0.5 };
    GLfloat Ball_diffuse[] = { 0.8, 0.7, 0.2, 0.5};
    GLfloat Ball_shininess[] = { 15.0 };   // Topun Iþýklandýrmasý için yazýldý
    GLfloat Bar_specular[] = {10.0, 10.0, 10.0, 10.0 };
    GLfloat Bar_diffuse[] = { 10.0, 10.0, 10.0, 10.0 };
    GLfloat Bar_shininess[] = { 50.0 };  // Barýn ýþýklandýrmasý 
    GLfloat Lives_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

 
	GLfloat paddle_color_array[][3] = {{1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1}}; // Bar renk dizisi gerek duyu
	GLfloat text_color_array[][4] = {{1,0,0,1},{0,1,0,1},{0,0,0,1},{1,0,1,0},{1,1,0,1},{1,1,1,1}}; // Yazý renk dizisi gerek duyu
	GLfloat paddle_size[] = {2,4,6}; // Barýn çizimi 
	GLfloat ambient1[] = {1,1,1};
	GLfloat px,start = 0;  // Bar hareketi için
	GLfloat bx = 0,by = -12.8 ;  // topun hareketi için gerekli 
	GLfloat dirx=0,diry=0;   // Topun Hýzý

// Kordinatlar için yapý
struct brick_coords{
	
	GLfloat x;
	GLfloat y;
};

//Kutularýn konduðu dizi
brick_coords brick_array[50][50];




void BarCiz() // Barýn çizildiði fonksiyon
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


void brick(GLfloat x,GLfloat y, GLfloat z) // Tekli kutunun çizildiði yer
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


void draw_bricks() //Çizilen kutunun çoðaltýlýp yerleþtirildiði fonksiyon
{
	
	int i,j;
	if(start == 0)
	{
		for(i = 1;i<=rows;i++)
		{
			for(j = 1;j<=columns;j++) // Aralarýnda ki uzaklýðýn verildiði yer
			{   
				brick_array[i][j].x = (GLfloat)(j*3.5*0.9);
				brick_array[i][j].y = (GLfloat)(i*2*0.8) ;
			}
		}
	}
	
	//Yerleþtirildiði yer
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

void TopCiz() // Topun çizildiði fonsiyon
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
void init (){ //Iþýklandýrmanýn ayarlandýðý fonksiyon. Iþýðýn rengi pozisyonu burda ayarlandý.
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
// mouse ile barýn hareketi saðlanýyor
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





// Ekranda skorun tutulmasýna yarayan fonksiyon. Ayný zamanda oyunun bittiðini anladýðýmýz yer.
void text( int score)
{
	glDisable(GL_LIGHTING);
	char text[40];
	if(score <50)
	sprintf(text,"Skor: %d   ",sc);
	else
	{
	  sprintf(text,"Kazandýnýz Tebrikler !!");
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

void display (void) { // Yukarýda yazdýðýmýz fonksiyonlarý çaðýrýp çalýþtýrðýmýz kýsým
	
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
void hit()  // Kutularý vurduðumuz ve onlarý vurulduktan sonra yok ettiðimiz fonksiyon
{
	//vurma ve kutularý yok etme fonksiyonu
	int i,j;
	for(i = 1;i<=rows;i++)
		for(j=1;j<=columns;j++)
		{
			// Alttan veya üstten vurmasý durumunda kutunun yok olmasý
			if((bx>=brick_array[i][j].x-19.5-0.1 )&&( bx<=brick_array[i][j].x + 3-19.5+ 0.1))  
			{
				if(by >=brick_array[i][j].y+5-0.1 && by <=brick_array[i][j].y+5 +1.2 +0.1)
				{
					//kutularý yok etmek için ekrandan kutuyu çýkarýyoruz
					brick_array[i][j].x = 0; 
					brick_array[i][j].y = 0;
					diry= diry*-1;
				// Her bir kutu yok olduktan sonra skor artýyor
					score++;
				}
			
			}
			else if(by >=brick_array[i][j].y+5 -0.1 && by <=brick_array[i][j].y+5 +1.2+0.1)  // Yanlardan vurmasý durumunda kutunun yok olma durumu
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

// sürekli çalýþacak idle fonksiyonu. Bu fonksiyonla birlikte ekranda vurulan karelerin çizilmemesi
// ve topun hareketli görünmesi saðlanýyor.
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
	// topun hýzý
	rate-=0.0001;
	
	float x = paddle_size[size];
	
	//Bu kýsýmda topun yönü ve hýzý düzenleniyor
	// alttaki bloða çarpýp çarpmadýðýna baðlý olarak.
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
void ozel_klavye(int key, int x, int y){ // Yön tuþlarý ile hareket saðlanýyor
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


void keyboard (unsigned char key, int x, int y) // Klavyeden basýlan tuþlar ile hareket saðlanýyor
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




void timer(int value){ // Topun oynanan süreç içerisinde hýzlanmasý saðlanýyor
	dirx *= 1.01; // Top her saniye %10 hýzlanýr
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
