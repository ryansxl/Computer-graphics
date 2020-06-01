//
//  main.cpp
//  pro1
//
//  Created by Shuai Ryan on 10/10/19.
//  Copyright © 2019 Shuai Ryan. All rights reserved.
//
#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;
int numpol;

struct point{
    int x;
    int y;
};

bool GreaterSort (point a,point b) { return (a.x>b.x); }
bool LessSort (point a,point b) { return (a.x<b.x); }


struct pol{
    vector <point> vel;
    int name;
    int number;
};

struct info{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

vector <pol> vec;


/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;


void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void dda(int x0, int y0, int x1, int y1);
void bresenham(int x0, int y0, int x1, int y1);
void drawpolygon1(vector<point> vel);
void readfile();
bool inrange(int a, int a0, int a1);
bool intersect(point a0, point a1, point b0, point b1, point* result);
info info2(vector<point> vel);
void raster(vector<point> vel);

vector<point> translate(vector<point> vel, int dx, int dy);
vector<point> scale(vector<point>  vel, float dx, float dy);
vector<point> rotate(vector<point> vel, float angle);
point center(vector<point> vel);
vector<point> scalecenter(vector<point> vel,float dx,float dy);
vector<point> rotatecenter(vector<point>  vel,float angle);

bool insideleft(point a0, int xmin);
bool insideright(point a0, int xmax);
bool insidetop(point a0, int ymax);
bool insidebottom(point a0, int ymin);
point intersecy(point a,point b, int y0);
point intersecx(point a,point b, int x0);
void clipping (vector<point> vel, int xmin, int xmax, int ymin, int ymax);

void savefile(vector <pol> vec, vector<point> vel0,int i);

int main(int argc, char **argv)
{
    
    //the number of pixels in the grid
    grid_width = 200;
    grid_height = 160;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;
    
    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;
    
    /*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
    glutCreateWindow("project 1");
    
    
    /*defined glut callback functions*/
    glutDisplayFunc(display); //rendering calls here
    glutReshapeFunc(reshape); //update GL on window size change
    glutMouseFunc(mouse);     //mouse button events
    glutMotionFunc(motion);   //mouse movement events
    //glutKeyboardFunc(key);    //Keyboard events
    glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
    check();
}
         //called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
    glutPostRedisplay();
}

//this is where we render the screen
void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -80, 80);
      
    vec.clear();
    readfile();
    int p,o;
    o = (unsigned)vec.size();
    cout << o <<endl ;
    

   // savefile(vec);
   // clipping(vec[0].vel, 30, 60, 20, 50);
    //raster(vec[0].vel);

    int show;
    int polygon = -1;
    char op;
    int  dx,dy;
    float scalenum1,scalenum2;
    float anglenum;
    int xmin,xmax,ymin,ymax;
    bool clippingnum =false;
    bool clippingnum2 =false;
    bool draw=false;
    vector<point> vel1;
    vel1.clear();
    cout<<"show polygons"<<endl;
    cin>>show;
    if (show !=1)
        {
        cout<<"operation with polygon"<<endl;
        cin >> polygon;
        if (polygon<0||polygon>=o)
        {cout<<"error"<<endl;
            cout<<"no this polygon"<<endl;
        }
            
        else if (0<= polygon<= o-1)
        {
            cout<<"which operation"<<endl;
            cin >> op;
            switch (op) {
                case 't':
                    {
                        cout << "enter the vector"<< endl;
                        cin >> dx;
                        cin >> dy;
                        vel1 = translate(vec[polygon].vel, dx, dy);
                        savefile(vec, vel1, polygon);
                    }
                    break;
                case 's':
                    {
                        cout << "enter the number of scale"<< endl;
                        cin >> scalenum1;
                        cin >> scalenum2;
                        vel1 = scalecenter(vec[polygon].vel, scalenum1, scalenum2);
                        savefile(vec, vel1, polygon);
                    }
                    break;
                case 'r':
                    {
                        cout << "enter the angel"<< endl;
                        cin >> anglenum;
                        vel1 = rotatecenter(vec[polygon].vel, anglenum);
                        savefile(vec, vel1, polygon);
                    }
                    break;
                case'c':
                    {
                        cout << "enter the clipping number"<< endl;
                        cin >> xmin;
                        cin >> xmax;
                        cin >> ymin;
                        cin >> ymax;
                        clipping(vec[polygon].vel, xmin, xmax, ymin, ymax);
                        clippingnum = true;
                    }
                    break;
                case'a':
                {
                    cout << "enter the clipping number"<< endl;
                    cin >> xmin;
                    cin >> xmax;
                    cin >> ymin;
                    cin >> ymax;
                    for(p=0; p<o; p++)
                     {
                          clipping(vec[p].vel, 20, 50, 20, 50);
                    }
                          clippingnum2 = true;
                }
                    break;
                case'd':
                {
                    cout << "draw all polygons"<< endl;
                    for(p=0; p<o; p++)
                     {
                          drawpolygon1(vec[p].vel);
                    }
                          draw = true;
                }
                    break;
                default:
                {
                    cout<<"no this operation"<<endl;
                }
                    break;
            }
         }
        }
  
            

     if((!clippingnum)&&(!clippingnum2)&&(!draw))
     {
       for(p=0; p<o; p++)
     {
          raster(vec[p].vel);
    }
     }
    
    else if(clippingnum)
    {
           for(p=0; p<o; p++)
         {
             if (p != polygon)
             { raster(vec[p].vel);}
        }
    }
/*
    vector<point> vel1,vel2,vel3;
    vel1.clear();
    vel2.clear();
    vel3.clear();
    vel1=translate(vec[1].vel, -50, -50);
    vel2=scalecenter(vec[2].vel, 2, 1.5);
    vel3=rotatecenter(vec[1].vel, 45);
    raster(vel1);
    raster(vel2);
    raster(vel3);

    savefile(vec,vel3,1);
*/
    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}


//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}

/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    win_width = width;
    win_height = height;
    
    //creates a rendering area across the window
    glViewport(0,0,width,height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,grid_width,0,grid_height,-10,10);
    
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width/(float)grid_width;
    
    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
    //check for opengl errors
    check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
    cout<<(unsigned)vec.size()<<endl;
    switch(ch)
    {
        default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
            break;
    }
    //redraw the scene after keyboard input
    glutPostRedisplay();
}


//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)((win_height-y)/pixel_size));
    switch(button)
    {
        case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
        case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
        default:
            printf("UNKNOWN "); //any other mouse button
            break;
    }
    if(state !=GLUT_DOWN)  //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n");  //button clicked
    
    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
    glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        printf("GLERROR: There was an error %s\n",gluErrorString(err) );
        exit(1);
    }
}


void readfile()
{
    string line;
    int namepol;
    ifstream file;
    int numpol;
    int numpoint;
    int x;
    int y;
    int i,j;
    float m,n;
    
   // file.open("/Users/shuairyan/Desktop/2/2/input.txt");
    file.open("/Users/shuairyan/Desktop/test_scene.txt");
    if (!file)
    {
        cout<<"file not exist"<<endl;
    }
    if (file.is_open())
        {
            file >> numpol;
            for(i=0; i<numpol; i++)
            {
                file >> numpoint;
                namepol = i;
                vector<point> aa;
                aa.clear();
                for(j=0; j<numpoint; j++)
                {
                    file>> m;
                    file>> n;
                    x=round(m);
                    y=round(n);
                    point bb={x,y};
                    aa.push_back(bb);
                }
                pol cc={aa,namepol,numpoint};
                vec.push_back(cc);
            }
        }
    cout<<vec.size()<<endl;
    file.close();
}

void savefile(vector <pol> vec, vector<point> vel0 , int i)
{
    ofstream file;
    file.open("/Users/shuairyan/Desktop/test_scene.txt");
    if (!file)
    {
        cout<<"file not exist"<<endl;
        return;
    }
    
    int r = (unsigned)vec.size();
    int p,q;
    file << r <<"\n";
    
    for(int s=0; s<r ; s++)
    {
        int t = (unsigned)vec[s].vel.size();
        file << t <<"\n";
        
        if(s != i)
        {
            for (int u=0; u<t; u++)
            {
                p=vec[s].vel[u].x;
                q=vec[s].vel[u].y;
                file << p <<"  "<< q <<"\n";
            }
        }
        
        else if(s == i)
        {
            for (int u=0; u<t; u++)
            {
                p=vel0[u].x;
                q=vel0[u].y;
                file << p <<"  "<< q <<"\n";
            }
        }
    }
    
}


int min(int x, int y)
{
    if (x<=y) return x;
    else return y;
}

void dda(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int step,i;
    float x = x0;
    float y = y0;
    float xincrement,yincrement;
    
    if(abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
    
    xincrement= float(dx) / float(step);
    yincrement= float(dy) / float(step);
    
    for(i=0; i<=step; i++)
    {
        draw_pix(round(x), round(y));
        x += xincrement;
        y += yincrement;
    }
}

void bresenham(int x0, int y0, int x1, int y1)
{
    int i,j;
    int x,y;
    bool m = false;
    
    if (abs( y1 - y0 )>abs( x1 - x0 ))
    {
        m= true;
        i=x0;
        x0=y0;
        y0=i;
        j=x1;
        x1=y1;
        y1=j;
    }
    
    if(x0 > x1)
    {
        i=x0;
        x0=x1;
        x1=i;
        j=y0;
        y0=y1;
        y1=j;
    }
    
    int dx= x1 - x0;
    int dy= y1 - y0;
    
    x=x0;
    y=y0;
    
    int p1 = 2*dy - dx;
    int p2 = 2*dy + dx;
    
    if (dy >= 0)
    {
        for(x;x<=x1;x++)
        {
            if(m) draw_pix(y, x);
            else draw_pix(x, y);
            if(p1<0) p1 += 2*dy;
            else
            {
                y++;
                p1 += (2*dy-2*dx);
            }
        }
    }
    else
    {
        for(x;x<=x1;x++)
        {
            if(m) draw_pix(y, x);
            else draw_pix(x, y);
            if(p2>0) p2 += 2*dy;
            else
            {
                y--;
                p2 += (2*dy+2*dx);
            }
        }
    }
}


void drawpolygon1(vector<point> vel)
{
    int i;
    int x0,x1,y0,y1;
    int m= (unsigned)vel.size();
    for (i=0;i<m-1;i++)
    {
        x0=vel[i].x;
        y0=vel[i].y;
        x1=vel[i+1].x;
        y1=vel[i+1].y;
        dda(x0, y0, x1, y1);
    }
    x0=vel[m-1].x;
    y0=vel[m-1].y;
    x1=vel[0].x;
    y1=vel[0].y;
    dda(x0, y0, x1, y1);
}

vector<point> translate(vector<point> vel, int dx, int dy)
{
    vector<point> vel2;
    int m,n;
    for(int i=0; i<vel.size(); i++)
    {
         m = vel[i].x + dx;
         n = vel[i].y + dy;
        vel2.push_back({m,n});
    }
    return vel2;
}

vector<point> scale(vector<point>  vel, float dx, float dy)
{
    vector<point> vel2;
    float x1,y1;
    int m,n;
    
    for(int i=0; i<vel.size(); i++)
    {
        x1 = float(vel[i].x) * dx;
        y1 = float(vel[i].y) * dy;
        m= round(x1);
        n= round(y1);
        vel2.push_back({m,n});
    }
    return vel2;
}

vector<point> rotate(vector<point>  vel, float angle)
{
    vector<point> vel2;
    float mm = 3.1415926/180;
    int m,n;

    for(int i=0; i<vel.size(); i++)
    {
        float newx = cos(angle * mm) * vel[i].x - sin(angle* mm) * vel[i].y;
        float newy = sin(angle * mm) * vel[i].x + cos(angle * mm) * vel[i].y;
        m= round(newx);
        n= round(newy);
        vel2.push_back({m,n});
    }
    return vel2;
}


point center(vector<point> vel)
{
    if(vel.empty()) cout<< "no center"<< endl;
    
    float centerx=0;
    float centery=0;
    int x0,y0;
    for(int i=0; i<vel.size(); i++)
    {
        centerx += float(vel[i].x);
        centery += float(vel[i].y);
    }
    centerx /= vel.size();
    centery /= vel.size();
    
    x0=int(centerx);
    y0=int(centery);
    point aa={x0,y0};
    return aa;
}

vector<point> scalecenter(vector<point> vel,float dx,float dy)
{
    vector<point> vel2,vel3,vel4;
    
    point aa = center(vel);
    vel2 = translate(vel,-aa.x,-aa.y);
    vel3 = scale(vel2,dx,dy);
    vel4 = translate(vel3,aa.x,aa.y);
    return vel4;
}

vector<point> rotatecenter(vector<point>  vel,float angle)
{
    vector<point> vel2,vel3,vel4;
    point aa = center(vel);
    
    vel2 = translate(vel,-aa.x,-aa.y);
    vel3 = rotate(vel2,angle);
    vel4 = translate(vel3,aa.x,aa.y);
    return vel4;
}


bool intersect(point a0, point a1, point b0, point b1, point* result)
{
    point d1={a0.x - a1.x, a0.y-a1.y};
    point d2={b0.x-b1.x, b0.y-b1.y};
    float mm = (d1.x*d2.y)-(d1.y*d2.x);
    
    if((mm> -0.0001) && (mm < 0.0001)) {return false;}
    
    point nn={b0.x-a0.x,b0.y-a0.y};
    float t = (nn.x*d2.y-nn.y*d2.x)/mm;
    
    result->x= round(a0.x+(t * d1.x));
    result->y= round(a0.y+(t* d1.y));
    
    return (inrange(result->x, a0.x, a1.x) && inrange(result->y, a0.y, a1.y)&& inrange(result->x, b0.x, b1.x)&& inrange(result->y, b0.y, b1.y));
}


bool inrange(int a, int a0, int a1)
{
    if ((a <= a0 || a <= a1)&&(a>=a0 || a>=a1))
        return true;
    else
        return false;
}



info info2(vector<point> vel)
{
    int x0,x1,y0,y1;
    int i,j;
    
    x0=vel[0].x;
    x1=vel[0].x;
    y0=vel[0].y;
    y1=vel[0].y;//
    
    j= (unsigned)vel.size();
    for(i=0; i<j ;i++)
    {
        if (vel[i].x <= x0) x0=vel[i].x;
        if(vel[i].x >= x1) x1=vel[i].x;
        
        if (vel[i].y <= y0) y0=vel[i].y;
        if(vel[i].y >= y1) y1=vel[i].y;
    }
    info aa={x0,x1,y0,y1};
    return aa;
}

void raster(vector<point> vel)
{
    info aa= info2(vel);
    int xmin=aa.xmin-5;
    int xmax=aa.xmax+5;
    int ymin=aa.ymin-5;
    int ymax=aa.ymax+5;

    
    for(int j=ymin; j<=ymax; j++)
    {
        if (vel.empty()) break;
        vector<point> drawpoint;
        drawpoint.clear();
        for(int k=0; k<vel.size();k++)
        {
            if ( k>0 && k<(vel.size()-2))
            {
                if( j== vel[k].y && j== vel[k+1].y)
                {
                    dda(vel[k].x, j, vel[k+1].x, j);
                    if((vel[k-1].y>j && vel[k+2].y<j)||(vel[k-1].y>j && vel[k+2].y<j))
                    {
                        drawpoint.push_back(vel[k]);
                    }
                }

                else if(j == vel[k].y)
                {
                    if((vel[k-1].y>j && vel[k+1].y>j)||(vel[k-1].y<j && vel[k+1].y<j))
                    {   drawpoint.push_back(vel[k]);
                        drawpoint.push_back(vel[k]);
                    }
                    else if((vel[k-1].y>j && vel[k+1].y<j)||(vel[k-1].y<j && vel[k+1].y>j))
                    drawpoint.push_back(vel[k]);
                }
                else
                {
                    point start={xmin,j};
                    point end={xmax,j};
                    point result;
                    if(intersect(start, end, vel[k], vel[k+1], &result))
                    {  if (!((result.x == vel[k].x  && result.y == vel[k].y) || (result.x == vel[k+1].x  && result.y == vel[k+1].y)))
                            {drawpoint.push_back(result);}
                    }
                }
            }
            else if (k==0)
            {
                if( j== vel[k].y && j== vel[k+1].y)
                {
                    dda(vel[k].x, j, vel[k+1].x, j);
                    if((vel[vel.size()-1].y>j && vel[k+2].y<j)||(vel[vel.size()-1].y>j && vel[k+2].y<j))
                    { drawpoint.push_back(vel[k]);}
                }
                else if(j == vel[k].y)
                {
                    if((vel[vel.size()-1].y>j && vel[k+1].y>j)||(vel[vel.size()-1].y<j && vel[k+1].y<j))
                    {   drawpoint.push_back(vel[k]);
                        drawpoint.push_back(vel[k]);
                    }
                    else if((vel[vel.size()-1].y>j && vel[k+1].y<j)||(vel[vel.size()-1].y<j && vel[k+1].y>j))
                    drawpoint.push_back(vel[k]);
                }
                else
                {
                    point start={xmin,j};
                    point end={xmax,j};
                    point result;
                    
                    if(intersect(start, end, vel[k], vel[k+1], &result))
                    {
                    if (!((result.x == vel[k].x  && result.y == vel[k].y) || (result.x == vel[k+1].x  && result.y == vel[k+1].y)))
                            {drawpoint.push_back(result);}
                    }
                }
            }
            else if (k==(unsigned)vel.size()-2)
            {
                if( j== vel[k].y && j== vel[k+1].y)
                {
                    dda(vel[k].x, j, vel[k+1].x, j);
                    if((vel[k-1].y>j && vel[0].y<j)||(vel[k-1].y>j && vel[0].y<j))
                    {   drawpoint.push_back(vel[k]);}
                }
                
                else if(j == vel[k].y)
                {
                    if((vel[k-1].y>j && vel[k+1].y>j)||(vel[k-1].y<j && vel[k+1].y<j))
                    {   drawpoint.push_back(vel[k]);
                        drawpoint.push_back(vel[k]);
                    }
                    else if((vel[k-1].y>j && vel[k+1].y<j)||(vel[k-1].y<j && vel[k+1].y>j))
                    drawpoint.push_back(vel[k]);
                }
                else
                {
                    point start={xmin,j};
                    point end={xmax,j};
                    point result;
                    
                    if(intersect(start, end, vel[k], vel[k+1], &result))
                        {
                              if (!((result.x == vel[k].x  && result.y == vel[k].y) || (result.x == vel[k+1].x  && result.y == vel[k+1].y)))
                                {drawpoint.push_back(result);}
                        }
                }
            }
            else if (k==(unsigned)vel.size()-1)
            {
                if( j== vel[k].y && j== vel[0].y)
                {
                    dda(vel[k].x, j, vel[0].x, j);
                    if((vel[k-1].y>j && vel[1].y<j)||(vel[k-1].y>j && vel[1].y<j))
                    {   drawpoint.push_back(vel[k]);}
                }
                
                else if(j == vel[k].y)
                {
                    if((vel[k-1].y>j && vel[0].y>j)||(vel[k-1].y<j && vel[0].y<j))
                     {   drawpoint.push_back(vel[k]);
                         drawpoint.push_back(vel[k]);
                     }
                     else if((vel[k-1].y>j && vel[0].y<j)||(vel[k-1].y<j && vel[0].y>j))
                     drawpoint.push_back(vel[k]);
                }
                else
                {
                    point start={xmin,j};
                    point end={xmax,j};
                    point result;
                    
                    if(intersect(start, end, vel[k], vel[0], &result))
                        {
                            
                        if (!((result.x == vel[k].x  && result.y == vel[k].y) || (result.x == vel[0].x  && result.y == vel[0].y)))
                            {drawpoint.push_back(result);}
                        }
                }
            }
        }
        sort(drawpoint.begin(),drawpoint.end(),LessSort);
        if(!drawpoint.empty())
            {
            for (int i =0; i < (unsigned)drawpoint.size()-1; i+=2)
                {
                dda(drawpoint[i].x, drawpoint[i].y, drawpoint[i+1].x, drawpoint[i+1].y);
                }
            }
    }
}


bool insideleft(point a0, int xmin)
{
    if (a0.x >= xmin) return true;
    else
        return false;
}

bool insideright(point a0, int xmax)
{
    if (a0.x <= xmax) return true;
    else
        return false;
}

bool insidetop(point a0, int ymax)
{
    if (a0.y <= ymax) return true;
    else
        return false;
}

bool insidebottom(point a0, int ymin)
{
    if (a0.y >= ymin) return true;
    else
        return false;
}

point intersecx(point a,point b, int x0)
{
    point result;
    int dx= b.x-a.x;
    int dy= b.y-a.y;
    float k=float(dy)/float(dx);
    float mm= float (a.y)+ k * float(x0 - a.x);
    int newy= round(mm);
    result = {x0,newy};
    return result;
}

point intersecy(point a,point b, int y0)
{
    point result;
    int dx= b.x-a.x;
    int dy= b.y-a.y;
    float k=float(dx)/float(dy);
    float mm= float (a.x)+ k * float(y0 - a.y);
    int newx= round(mm);
    result = {newx,y0};
    return result;
}



void clipping (vector<point> vel, int xmin, int xmax, int ymin, int ymax)
{
    vector<point> vel1,vel2;
    int i,j;
    
    j=(unsigned)vel.size();
    for(i=0; i<j-1; i++)
    {
        if ( (!(insideleft(vel[i], xmin))) &&(insideleft(vel[i+1], xmin)) )
        {
            point result;
            result = intersecx(vel[i], vel[i+1], xmin);
            vel1.push_back(result);
            vel1.push_back(vel[i+1]);
        }
        else if(  (insideleft(vel[i], xmin)) && (insideleft(vel[i+1], xmin)) )
            vel1.push_back(vel[i+1]);
        else if( (insideleft(vel[i], xmin)) && (!(insideleft(vel[i+1], xmin))) )
            {
                point result;
                result = intersecx(vel[i], vel[i+1], xmin);
                vel1.push_back(result);
            }
    }
    if ( (!(insideleft(vel[j-1], xmin))) &&(insideleft(vel[0], xmin)) )
    {
        point result;
        result = intersecx(vel[j-1], vel[0], xmin);
        vel1.push_back(result);
        vel1.push_back(vel[0]);
    }
    else if(  (insideleft(vel[j-1], xmin)) && (insideleft(vel[0], xmin)) )
        vel1.push_back(vel[0]);
    else if( (insideleft(vel[j-1], xmin)) && (!(insideleft(vel[0], xmin))) )
        {
            point result;
            result = intersecx(vel[j-1], vel[0], xmin);
            vel1.push_back(result);
        }
    
    
    j=(unsigned)vel1.size();
    for(i=0; i<j-1; i++)
    {
        if ( (!(insidetop(vel1[i], ymax))) &&(insidetop(vel1[i+1], ymax)) )
        {
            point result;
            result = intersecy(vel1[i], vel1[i+1], ymax);
            vel2.push_back(result);
            vel2.push_back(vel1[i+1]);
        }
        else if(  (insidetop(vel1[i], ymax)) && (insidetop(vel1[i+1], ymax)) )
            vel2.push_back(vel1[i+1]);
        else if( (insidetop(vel1[i], ymax)) && (!(insidetop(vel1[i+1], ymax))) )
            {
                point result;
                result = intersecy(vel1[i], vel1[i+1], ymax);
                vel2.push_back(result);
            }
    }
    
    if ( (!(insidetop(vel1[j-1], ymax))) &&(insidetop(vel1[0], ymax)) )
    {
        point result;
        result = intersecy(vel1[j-1], vel1[0], ymax);
        vel2.push_back(result);
        vel2.push_back(vel1[0]);
    }
    else if(  (insidetop(vel1[j-1], ymax)) && (insidetop(vel1[0], ymax)) )
        vel2.push_back(vel1[0]);
    else if( (insidetop(vel1[j-1], ymax)) && (!(insidetop(vel1[0], ymax))) )
        {
            point result;
            result = intersecy(vel1[j-1], vel1[0], ymax);
            vel2.push_back(result);
        }
    
    vector<point> vel3;

    j=(unsigned)vel2.size();
    for(i=0; i<j-1; i++)
    {
        if ( (!(insideright(vel2[i], xmax))) &&(insideright(vel2[i+1], xmax)) )
        {
            point result;
            result = intersecx(vel2[i], vel2[i+1], xmax);
            vel3.push_back(result);
            vel3.push_back(vel2[i+1]);
        }
        else if(  (insideright(vel2[i], xmax)) && (insideright(vel2[i+1], xmax)) )
            vel3.push_back(vel2[i+1]);
        else if( (insideright(vel2[i], xmax)) && (!(insideright(vel2[i+1], xmax))) )
            {
                point result;
                result = intersecx(vel2[i], vel2[i+1], xmax);
                vel3.push_back(result);
            }
    }
    if ( (!(insideright(vel2[j-1], xmax))) &&(insideright(vel2[0], xmax)) )
    {
        point result;
        result = intersecx(vel2[j-1], vel2[0], xmax);
        vel3.push_back(result);
        vel3.push_back(vel2[0]);
    }
    else if(  (insideright(vel2[j-1], xmax)) && (insideright(vel2[0], xmax)) )
        vel3.push_back(vel2[0]);
    else if( (insideright(vel2[j-1], xmax)) && (!(insideright(vel2[0], xmax))) )
        {
            point result;
            result = intersecx(vel2[j-1], vel2[0], xmax);
            vel3.push_back(result);
        }
    
    vector<point> vel4;
    
    
    j=(unsigned)vel3.size();
    for(i=0; i<j-1; i++)
    {
        if ( (!(insidebottom(vel3[i], ymin))) &&(insidebottom(vel3[i+1], ymin)) )
        {
            point result;
            result = intersecy(vel3[i], vel3[i+1], ymin);
            vel4.push_back(result);
            vel4.push_back(vel3[i+1]);
        }
        else if(  (insidebottom(vel3[i], ymin)) && (insidebottom(vel3[i+1], ymin)) )
            vel4.push_back(vel3[i+1]);
        else if( (insidebottom(vel3[i], ymin)) && (!(insidebottom(vel3[i+1], ymin))) )
            {
                point result;
                result = intersecy(vel3[i], vel3[i+1], ymin);
                vel4.push_back(result);
            }
    }
    if ( (!(insidebottom(vel3[j-1], ymin))) &&(insidebottom(vel3[0], ymin)) )
    {
        point result;
        result = intersecy(vel3[j-1], vel3[0], ymin);
        vel4.push_back(result);
        vel4.push_back(vel3[0]);
    }
    else if(  (insidebottom(vel3[j-1], ymin)) && (insidebottom(vel3[0], ymin)) )
        vel4.push_back(vel3[0]);
    else if( (insidebottom(vel3[j-1], ymin)) && (!(insidebottom(vel3[0], ymin))) )
        {
            point result;
            result = intersecy(vel3[j-1], vel3[0], ymin);
            vel4.push_back(result);
        }

    raster(vel4);
    
}




