#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<GL/glut.h>
#include<conio.h>
#include<string.h>

#define MAX 10
int k=0;
typedef struct circle
{
	GLfloat x;	//x axis of center
	GLfloat y;	//y axis of center
	GLfloat r;	// radius
}circle;

circle c[MAX];	//array of circles to store the center and radius of each circle
int a[MAX];		// int array for sorting algo
int initial[MAX] = {10,4,8,6,3,7,9,1,5,2};	//int array to restore random values after sorting
GLfloat initial_x1,initial_x2;	//to set the destiniation for swapping

int global_i = 0, global_j = 0;	//i and j values for bubble-sort
int swapping = 0;	//flag to check if circle are being swapped
int sorting = 0;	//start sorting only after user input

void initialise()
{
	global_i = global_j = swapping = 0; //reset all globals
	for (int i=0;i<MAX;i++)
	{
		a[i] = initial[i];	//if a[] is sorted restore from initial[]
		c[i].r = a[i]*4.0;	//4 because to fit 10 circles in screen
		c[i].y = 300.0;		//vertical center of window
		if (i == 0)
			c[i].x = 45.0;	// first circle starts from 45 offset
		else
			c[i].x = c[i-1].x + 90.0;//(c[i-1].r+c[i].r+10.0); //distance between circles = sum of 2 max readii
		printf("%f - %f - %f\n",c[i].x,c[i].y,c[i].r); //for testing purpose don worry
	}
}

//func to display text on screen char by char
void bitmap_output(int x, int y, char *string, void *font)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

//function to integer to string
void int_str(int rad,char r[])
{
	switch(rad)
	{
		case 1 : strcpy(r, "1"); break;
		case 2 : strcpy(r, "2"); break;
		case 3 : strcpy(r, "3"); break;
		case 4 : strcpy(r, "4"); break;
		case 5 : strcpy(r, "5"); break;
		case 6 : strcpy(r, "6"); break;
		case 7 : strcpy(r, "7"); break;
		case 8 : strcpy(r, "8"); break;
		case 9 : strcpy(r, "9"); break;
		case 10 : strcpy(r, "10"); break;
	}

}

//draw circle by drawing consecutive triangle fans
void circle_draw(circle c)
{
	float i;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(c.x, c.y);	//center of circle
	for (i=0;i<360;i+=1)
		glVertex2f(c.x + sin(i) * c.r, c.y + cos(i) * c.r);
	glEnd();

	//display the value of circle below
	int x = c.x-2;
	int y = c.y-(c.r+10);
	int rad = c.r / 4;
	char r[3] = "";
	int_str(rad,r);
	glColor3f(0.0,0.0,0.0);
	bitmap_output(x, y, r, GLUT_BITMAP_TIMES_ROMAN_10);

}

// swaps circles cc1 and cc2 by changing their centers
void swap_circles(circle *cc1,circle *cc2)
{
	if (swapping == 0)	//if circles are not being swapped set destination for each circles
	{
		initial_x1 = cc1->x; //center of circle in left
		initial_x2 = cc2->x; //center of circle in right
		swapping = 1;	//means cicle are being swapped
		printf("%f - %f\n",cc1->r,cc2->r);

	}

	if (initial_x1 <= cc2->x)	//decrease the center of circle in right till its > center of left circle
		cc2->x -= 1.0;		//speed of animation

	if (initial_x2 >= cc1->x)//increase the center of circle in left till its < center of right circle
		cc1->x += 1.0;

	printf("one %f - %f\n",initial_x1,cc2->x);
	printf("two %f - %f\n",initial_x2,cc1->x);

	// if difference between destination and center < 0.3 then cicles are swapped
	if (abs(initial_x1-cc2->x) < 0.3 && abs(initial_x2-cc1->x) < 0.3)  //set this to speed of animation
	{
		swapping = 0;

		int temp = cc1->x;
		cc1->x = cc2->x;
		cc2->x = temp;

		temp = cc1->y;
		cc1->y = cc2->y;
		cc2->y = temp;

		temp = cc1->r;
		cc1->r = cc2->r;
		cc2->r = temp;
	}

}


void sort()	//bubble sort algo
{
	if (!swapping) //if not in process of swappin 2 circles only then get 2 new circles to swap
	{
		while (global_i < 10)
		{
			global_j = global_i;
			while (global_j < 9)
			{
				if (a[global_j] > a[global_j+1])
				{
					printf("%d %d\n",global_j,a[global_j]);
					int temp = a[global_j];
					a[global_j] = a[global_j+1];
					a[global_j+1] = temp;
					goto SWAP;

				}
				global_j ++;
			}
			global_i ++;
		}
	}
SWAP:
	printf("swapping --> %d - %d\n",global_j,global_j+1);
	bitmap_output(10, 375, "Swapping ->",GLUT_BITMAP_9_BY_15);

	char r[3]="";
	int_str(a[global_j],r);
	bitmap_output(150, 375, r,GLUT_BITMAP_9_BY_15);
	int_str(a[global_j+1],r);
	bitmap_output(175, 375, r,GLUT_BITMAP_9_BY_15);

	swap_circles(&c[global_j],&c[global_j+1]);
}

void display_text()
{
	bitmap_output(200, 565, "DYNAMIC SORTING ALGORITHM VISUALISER",GLUT_BITMAP_TIMES_ROMAN_24);//title larger font
	glBegin(GL_LINE_LOOP); //to underline the title
		glVertex2f(180, 560);
		glVertex2f(750, 560);
	glEnd();

	//other text small font
	bitmap_output(10, 525, "This program sorts a random set of numbers in ascending order displaying them graphically as ",GLUT_BITMAP_9_BY_15);
	bitmap_output(10, 505, "circles with varying radii.",GLUT_BITMAP_9_BY_15);


	if (sorting == 0)//if not sorting display menu
	{
		bitmap_output(10, 455, "MENU",GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 435, "Press s to SORT",GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 415, "Press r to RANDOMISE",GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 395, "Esc to QUIT",GLUT_BITMAP_9_BY_15);
	}
	else if (sorting == 1)
	{
		glColor3f(0.5,0.5,0.5);
		bitmap_output(10, 455, "Sorting in progress...",GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 435, "Please do not quit",GLUT_BITMAP_9_BY_15);
		glColor3f(0.0,0.0,0.0);
	}

}

void front()
{
	glColor3f(0.0,0.0,1.0);
	bitmap_output(390, 565, "WELCOME!",GLUT_BITMAP_TIMES_ROMAN_24);
	glBegin(GL_LINE_LOOP);
		glVertex2f(388, 560);
		glVertex2f(524, 560);
	glEnd();
	bitmap_output(440, 535, "TO",GLUT_BITMAP_TIMES_ROMAN_24);
	glBegin(GL_LINE_LOOP);
		glVertex2f(438, 531);
		glVertex2f(475, 531);
	glEnd();

	bitmap_output(200, 495, "DYNAMIC SORTING ALGORITHM VISUALISER",GLUT_BITMAP_TIMES_ROMAN_24);
	glBegin(GL_LINE_LOOP);
		glVertex2f(198, 490);
		glVertex2f(731, 490);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
	  	glVertex2f(594,120.0);glVertex2f(594,145);glVertex2f(836,145);glVertex2f(836,120.0);
    glEnd();
	glColor3f(0.0,1.0,0.0);
	bitmap_output(600, 125, "Press Enter to continue.......",GLUT_BITMAP_HELVETICA_18);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);

	if (k==0)
    front();
	else
	{

	display_text();

	glPointSize(2.0);
	for (int i=0;i<MAX;i++)
	{
		glColor3f(1.0,0.0,0.0);
		circle_draw(c[i]);
	}
	if (global_j+1 < MAX && sorting == 1) // call sort only on key press
		sort();
	else
		sorting = 0;
	}

	glFlush();
	glutSwapBuffers();

}

void keyboard (unsigned char key, int x, int y)
{
	if(key==13)
		k=1;
	if (k==1)
	{
	switch (key)
	{
		case 27 : exit (0); //27 is the ascii code for the ESC key
		case 's' : sorting = 1; break;
		case 'r' : initialise(); break;
	}
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<=h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h/ (GLfloat) w, 2.0* (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat) w/ (GLfloat) h, 2.0* (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
}

void init(void)
{
		glClearColor(1.0,1.0,1.0,0.0);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0.0,900.0,0.0,600.0);
}

 int  main(int argc, char ** argv)
{
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
		glutInitWindowPosition(50,50);
		glutInitWindowSize(900,600);
		glutCreateWindow("Dynamic Sorting Algorithm Visualiser");
		init();
		initialise();
		glutDisplayFunc(display);
		glutIdleFunc (display);
		glutKeyboardFunc (keyboard);
		glutMainLoop();
}
