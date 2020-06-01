//
//  main.cpp
//  project3
//
//  Created by Shuai Ryan on 11/10/19.
//  Copyright © 2019 Shuai Ryan. All rights reserved.
//

#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
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

struct vec{
    float x;
    float y;
    float z;
};

struct color{
    float r;
    float g;
    float b;
};

struct position{
    float x;
    float y;
    float z;
};

struct point3d{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float dr;
    float dg;
    float db;
    vec nvec;
    vector<vec> allnvec;
};

struct triangle{
    int num1;
    int num2;
    int num3;
    point3d point1;
    point3d point2;
    point3d point3;
    int spec;
    vec nvec;
};

struct point3d1{
    int x;
    int y;
    int z;
    float r;
    float g;
    float b;
};

struct triangle1{
    int num1;
    int num2;
    int num3;
    point3d1 point1;
    point3d1 point2;
    point3d1 point3;
    int minx;
    int miny;
    int minz;
};

struct poly3d{
    int name;
    vector<point3d> vecpoint3d;
    vector<triangle> vectriangle;
};

vector<poly3d> vecpoly3d;

struct point2d{
    float mm1;
    float mm2;
    float r;
    float g;
    float b;
    float dr;
    float dg;
    float db;
};

struct info0{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

vec chacheng(vec m1, vec m2);
float diancheng(vec m1,vec m2);
vec normalvec(float x0,float y0,float z0,float x1, float y1, float z1);
float distance0(float x0,float y0,float z0,float x1, float y1, float z1);
vec reflect(vec m1, vec m2);
float power(float mm, int nn);
int min(int a, int b, int c);
int max0(int a, int b, int c);

vector<triangle1> sortx(vector<triangle1> tri0 );
vector<triangle1> sorty(vector<triangle1> tri0 );
vector<triangle1> sortz(vector<triangle1> tri0 );
void drawpix0(int m, int n,float r,float g,float b);
void drawpix1(int x, int y,float r0,float g0, float b0);

void init();
void idle();
void check();
void display();

void drawline3(float m0, float m1 , float n0, float n1,float r,float g,float b);
void drawline2(float m0, float m1 , float n0, float n1);
void drawpix(float m, float n,float r,float g,float b);
void drawlinexy(point3d1 point1 , point3d1 point2);
void drawtrianglexy(triangle1 triangle0);
void projectxy(vector<triangle1> tri0);
point3d1 interxy(int mm, point3d1 point1, point3d1 point2);
vector<point3d1> intersectxy(int mm, triangle1 triangle0);
void rasterxy(triangle1 triangle0);

info0 getinfoxy(triangle1 triangle0);

vector<triangle1> transfer( vector<poly3d> vecpoly3d0);
vector<triangle1> transxz(vector<triangle1> aaa);
vector<triangle1> transyz(vector<triangle1> aaa);

float max1(float a, float b, float c);
void drawpix2(int x0, int y0,float r0,float g0, float b0);
bool check(int m,vector<int> nn);
void randraw(int num,int color,int x,int y);


void readfile(bool phongon,int choice);
void randraw1(int num,int x,int y);
void display();
void drawpix3(int x0, int y0,float r0,float g0, float b0);


bool phongon = false;
bool half = false;
int whichcolor = 0;
int random0=0;
color kaIA={0.3,0.3,0.3};
color IL={0.75,0.75,0.75};
position from={0.5,0.5,1.1};
position light={0.75,0.75,0.75};
float K=0.5;
color ks={0.7,0.7,0.7};

int grid_width=400;
int grid_height=400;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size=1;

/*Window information*/
int win_height=800;
int win_width=800;

int main(int argc, char **argv)
{
    

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("project3");
    
    glutDisplayFunc(display); //rendering calls here

    
    glutIdleFunc(idle);       //Function called while program is
    
    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}


void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluOrtho2D(0, 1.0, 0, 1.0);
     gluOrtho2D(0, 400, 0, 400);
    check();
}

void idle()
{
    glutPostRedisplay();
}

void check()
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        printf("GLERROR: There was an error %s\n",gluErrorString(err) );
        exit(1);
    }
}

void drawpix(float m, float n,float r,float g,float b)
{
    glPointSize(0.8);
    glBegin(GL_POINTS);
    glColor3f(r,g,b);
    glVertex3f(m,n,0);
    glEnd();
}

void display()
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    vecpoly3d.clear();
    
    int whichfile;
    cout << "Which file??? 0:bunny 1:cube" << endl;
    cin >> whichfile;
    int pho;
    cout << "Turn on the Phong Model??? 0:close 1:open" <<endl;
    cin >> pho;
    if(pho==1)  phongon= true;
    readfile(phongon,whichfile);
    if(phongon)
    {
        cout <<"now the location of light is :0.75  0.75  0.75"<<endl;
        cout <<"Do you want to move the light?? 0:not move  1:move"<<endl;
        int move;
        cin >> move;
        if(move==1)
        {
            float x00,y00,z00;
            cout<<"Enter the new location"<<endl;
            cin>>x00;
            cin>>y00;
            cin>>z00;
            light={x00,y00,z00};
        }
        float m00,m11,m22;
        cout <<"Enter kaIA"<<endl;
        cin >>m00;
        cin >>m11;
        cin >>m22;
        kaIA={m00,m11,m22};
        float n00;
        cout <<"Enter IL(intensity of light)"<<endl;
        cin >>n00;
        IL={n00,n00,n00};
        cout<<"Enter K(distance)"<<endl;
        cin >> K;
        float s00,s11,s22;
        cout <<"Enter the location of eye"<<endl;
        cin>>s00;
        cin>>s11;
        cin>>s22;
        from={s00,s11,s22};
        float u00,u11,u22;
        cout<<"Enter ks(color of light)"<<endl;
        cin >>u00;
        cin>>u11;
        cin >>u22;
        ks={u00,u11,u22};
    }
    cout<<"Turn on the Halftone Model??? 0:close 1:open"<<endl;
    int hal;
    cin >> hal;
    if(hal == 1) half = true;
    if(half)
    {
        cout<<"which kind of half 0: (black and white) 1:( r,g,b)"<<endl;
        cin>>whichcolor;
        if(whichcolor==0)
        {
            cout<<"1:random 0:not random"<<endl;
            cin>>random0;
        }
        
    }
    
    
    glViewport(0, 0, 800, 800);
    drawline2(0, 200, 400, 200);
    drawline2(200, 0, 200, 400);

    glViewport(0, 0, 400, 400);
    vector<triangle1> aaa;
    aaa.clear();
    vector<triangle1> bbb;
    bbb.clear();
    aaa=transfer(vecpoly3d);
    bbb=sortz(aaa);
    projectxy(bbb);

    glViewport(0, 400, 400, 400);
    vector<triangle1> sss;
    sss.clear();
    vector<triangle1> xxx;
    xxx.clear();
    sss=transxz(aaa);
    xxx=sortz(sss);
    projectxy(sss);
    
    
    glViewport(400, 400, 400, 400);
     vector<triangle1> ppp;
     ppp.clear();
     vector<triangle1> qqq;
     qqq.clear();
     ppp=transyz(aaa);
     qqq=sortz(ppp);
     projectxy(qqq);
    
    
    glViewport(400, 0, 400, 400);
 

    vector<triangle1>().swap(aaa);
    vector<triangle1>().swap(bbb);
    vector<triangle1>().swap(sss);
    vector<triangle1>().swap(xxx);
    vector<triangle1>().swap(ppp);
    vector<triangle1>().swap(qqq);
    
     glFlush();
     
     //blits the current opengl framebuffer on the screen
     glutSwapBuffers();
     //checks for opengl errors
     check();
}



vec chacheng(vec m1, vec m2)
{
    float x0,y0,z0;
    float mm;
    x0=m1.y*m2.z-m1.z*m2.y;
    y0=m1.z*m2.x-m1.x*m2.z;
    z0=m1.x*m2.y-m1.y*m2.x;
    mm = sqrt(x0*x0+y0*y0+z0*z0);
    x0 /= mm;
    y0 /= mm;
    z0 /= mm;
    vec m0={x0,y0,z0};
    return m0;
}

vec normalvec(float x0,float y0,float z0,float x1, float y1, float z1)
{
    float x2,y2,z2;
    x2=x1-x0;
    y2=y1-y0;
    z2=z1-z0;
    float mm;
    mm=sqrt(x2*x2+y2*y2+z2*z2);
    x2 /=mm;
    y2 /=mm;
    z2 /=mm;
    vec aa={x2,y2,z2};
    return aa;
}

vec reflect(vec m1, vec m2)
{
    float cos= diancheng(m1, m2);
    vec m3 = {cos*m2.x,cos*m2.y,cos*m2.z};
    vec m4 = {m3.x-m1.x,m3.y-m1.y,m3.z-m1.z};
    vec m5 = {2*m4.x,2*m4.y,2*m4.z};
    vec m6 = {m1.x+m5.x,m1.y+m5.y,m1.z+m5.z};
    return m6;
}

float diancheng(vec m1,vec m2)
{
    float aaa;
    aaa=(m1.x*m2.x+m1.y*m2.y+m1.z*m2.z);
    return aaa;
}

float power(float mm, int nn)
{
    float mm0 = 1.0;
    for(int i=0;i<nn;i++)
    {
        mm0 = mm0 * mm;
    }
    return mm0;
}

float distance0(float x0,float y0,float z0,float x1, float y1, float z1)
{
    float x2,y2,z2;
    x2=x1-x0;
    y2=y1-y0;
    z2=z1-z0;
    float mm;
    mm=sqrt(x2*x2+y2*y2+z2*z2);
    return mm;
}

int min0(int a, int b, int c)
{
    int mm;
    if(a>=b)
    {
        mm=b;
        if(b>=c) mm=c;
    }
    else
    {
        mm=a;
        if(a>=c) mm=c;
    }
    return mm;
}

int max0(int a, int b, int c)
{
    int mm;
    if(a>=b)
    {
        mm=a;
        if(c>=a) mm=c;
    }
    else
    {
        mm=b;
        if(c>=b) mm=c;
    }
    return mm;
}

float max1(float a, float b, float c)
{
    float mm;
    if(a>=b)
    {
        mm=a;
        if(c>=a) mm=c;
    }
    else
    {
        mm=b;
        if(c>=b) mm=c;
    }
    return mm;
};

void readfile(bool phongon,int choice)
{
    int namepol;
    ifstream file;
    int numpol;
    int numpoint;
    int numtriangle;
    
    int i,j,k;
    float x,y,z;
    int mm1,mm2,mm3;
    float dr,dg,db;
    float maxdcolor=0;
    
    if(choice==0)
    {file.open("/Users/shuairyan/Desktop/bunny.txt");}
    else
    {file.open("/Users/shuairyan/Desktop/cube_and_icosahedron.txt");}
    if(!file)
    {
        cout<<"Error: File not exist!!!"<<endl;
    }
    
    if(file.is_open())
        {
            file >> numpol;
            for(i=0;i<numpol;i++)
            {
                file >> namepol;
                file >> numpoint;
                vector<point3d> aa;
                aa.clear();
                vector<triangle> dd;
                dd.clear();
        
                for (j=0; j<numpoint; j++)
                {
                    file>>x;
                    file>>y;
                    file>>z;
                    vec nn={0,0,0};
                    vector<vec> allnn;
                    allnn.clear();
                    point3d bb={x,y,z,0,0,0,0,0,0,nn,allnn};
                    aa.push_back(bb);
                    vector<vec>().swap(allnn);
                }
                
                for (j=0; j<numpoint; j++)
                {
                    file>>dr;
                    file>>dg;
                    file>>db;
                    if(dr>=maxdcolor) maxdcolor=dr;
                    if(dg>=maxdcolor) maxdcolor=dg;
                    if(db>=maxdcolor) maxdcolor=db;
                    aa[j].dr=dr;
                    aa[j].dg=dg;
                    aa[j].db=db;
                    aa[j].r=dr;
                    aa[j].g=dg;
                    aa[j].b=db;
                }
                
                if(maxdcolor>0)
                {
                    for (j=0; j<numpoint; j++)
                    {
                        aa[j].dr /= maxdcolor;
                        aa[j].dg /= maxdcolor;
                        aa[j].db /= maxdcolor;
                        aa[j].r  /= maxdcolor;
                        aa[j].g  /= maxdcolor;
                        aa[j].b  /= maxdcolor;
                    }
                }
               
                file >> numtriangle;
                for(k=0;k<numtriangle;k++)
                {
                    file>>mm1;
                    file>>mm2;
                    file>>mm3;
                    point3d point1=aa[mm1-1];
                    point3d point2=aa[mm2-1];
                    point3d point3=aa[mm3-1];
                    
                    vec nn1={point2.x-point1.x,point2.y-point1.y,point2.z-point1.z};
                    vec nn2={point3.x-point1.x,point3.y-point1.y,point3.z-point1.z};
                    vec nn0=chacheng(nn1, nn2);
                    
                    aa[mm1-1].allnvec.push_back(nn0);
                    aa[mm2-1].allnvec.push_back(nn0);
                    aa[mm3-1].allnvec.push_back(nn0);
                    
                    point1=aa[mm1-1];
                    point2=aa[mm2-1];
                    point3=aa[mm3-1];
                    
                    triangle ss={mm1,mm2,mm3,point1,point2,point3,0,nn0};
                    
                    dd.push_back(ss);
                }
                
                for(k=0;k<numtriangle;k++)
                {
                    int spec0;
                    file >> spec0;
                    dd[k].spec=spec0;
                }
                
                for (j=0; j<numpoint; j++)
                {
                    int numnvec = (unsigned) aa[j].allnvec.size();
                    float x0=0,y0=0,z0=0;
                    if (numnvec !=0)
                    {
                        for(int q=0;q<numnvec;q++)
                        {
                            x0 += aa[j].allnvec[q].x;
                            y0 += aa[j].allnvec[q].y;
                            z0 += aa[j].allnvec[q].z;
                        }
                        float cc=sqrt(x0*x0+y0*y0+z0*z0);
                        x0 /=cc;
                        y0 /=cc;
                        z0 /=cc;
                        aa[j].nvec.x=x0;
                        aa[j].nvec.y=y0;
                        aa[j].nvec.z=z0;
                    }
                }
                
                for(k=0;k<numtriangle;k++)
                {

                    int nn0=dd[k].num1;
                    int nn1=dd[k].num2;
                    int nn2=dd[k].num3;
                    dd[k].point1=aa[nn0-1];
                    dd[k].point2=aa[nn1-1];
                    dd[k].point3=aa[nn2-1];
                }
                poly3d pp={namepol,aa,dd};
                vecpoly3d.push_back(pp);
                vector<point3d>().swap(aa);
                vector<triangle>().swap(dd);
            }
        }
    file.close();
    
    if(phongon)
    {
        numpol=(unsigned)vecpoly3d.size();
        float maxcolor=0;
        for(i=0;i<numpol;i++)
        {
            numpoint=(unsigned)vecpoly3d[i].vecpoint3d.size();
            for(j=0;j<numpoint;j++)
            {
                float Ipr,Ipg,Ipb;
                int spec = 1;
                float dis1;
                float xp=vecpoly3d[i].vecpoint3d[j].x;
                float yp=vecpoly3d[i].vecpoint3d[j].y;
                float zp=vecpoly3d[i].vecpoint3d[j].z;
                color kd={vecpoly3d[i].vecpoint3d[j].dr,vecpoly3d[i].vecpoint3d[j].dg,vecpoly3d[i].vecpoint3d[j].db};
                vec lvec = normalvec(xp, yp, zp, light.x, light.y, light.z);
                vec nvec = vecpoly3d[i].vecpoint3d[j].nvec;
                vec vvec = normalvec(xp, yp, zp, from.x, from.y, from.z);
                dis1 = distance0(xp, yp, zp, from.x, from.y, from.z);
                vec rvec = reflect(lvec, nvec);
                
                float cos1 = diancheng(lvec, nvec);
                float cos2 = diancheng(rvec, vvec);
                float ncos2 = power(cos2, spec);
                float cos3 = diancheng(nvec,vvec);
                
                if((cos1 >0 && cos3 <0)||(cos1 <0 && cos3 >0))
                {
                    Ipr=abs(kaIA.r);
                    Ipg=abs(kaIA.g);
                    Ipb=abs(kaIA.b);
                }
                else if (cos2<0)
                {
                    Ipr=abs(kaIA.r+IL.r/(dis1+K)*kd.r*cos1);
                    Ipg=abs(kaIA.g+IL.g/(dis1+K)*kd.g*cos1);
                    Ipb=abs(kaIA.b+IL.b/(dis1+K)*kd.b*cos1);
                }
                else
                {
                    Ipr=abs(kaIA.r+IL.r/(dis1+K)*kd.r*cos1+IL.r/(dis1+K)*ks.r*ncos2);
                    Ipg=abs(kaIA.g+IL.g/(dis1+K)*kd.g*cos1+IL.g/(dis1+K)*ks.g*ncos2);
                    Ipb=abs(kaIA.b+IL.b/(dis1+K)*kd.b*cos1+IL.b/(dis1+K)*ks.b*ncos2);
                }
                
                if(Ipr>=maxcolor) maxcolor=Ipr;
                if(Ipg>=maxcolor) maxcolor=Ipg;
                if(Ipb>=maxcolor) maxcolor=Ipb;
                vecpoly3d[i].vecpoint3d[j].r=Ipr;
                vecpoly3d[i].vecpoint3d[j].g=Ipg;
                vecpoly3d[i].vecpoint3d[j].b=Ipb;
            }// for j
        }//for i
        
        for(i=0;i<numpol;i++)
        {
            numpoint=(unsigned)vecpoly3d[i].vecpoint3d.size();
            if(maxcolor>1)
            {
                for(j=0;j<numpoint;j++)
                {
                    vecpoly3d[i].vecpoint3d[j].r /= maxcolor;
                    vecpoly3d[i].vecpoint3d[j].g /= maxcolor;
                    vecpoly3d[i].vecpoint3d[j].b /= maxcolor;
                }
            }
            numtriangle=(unsigned)vecpoly3d[i].vectriangle.size();
            for(k=0;k<numtriangle;k++)
            {
                mm1=vecpoly3d[i].vectriangle[k].num1;
                mm2=vecpoly3d[i].vectriangle[k].num2;
                mm3=vecpoly3d[i].vectriangle[k].num3;
                vecpoly3d[i].vectriangle[k].point1=vecpoly3d[i].vecpoint3d[mm1-1];
                vecpoly3d[i].vectriangle[k].point2=vecpoly3d[i].vecpoint3d[mm2-1];
                vecpoly3d[i].vectriangle[k].point3=vecpoly3d[i].vecpoint3d[mm3-1];
            }
        }
    }
}


void drawline2(float m0, float m1 , float n0, float n1)
{
    glBegin(GL_LINES);
    glColor3f(0,0,0);
    glVertex2f(m0, m1);
    glVertex2f(n0, n1);
    glEnd();
}

void drawline3(float m0, float m1 , float n0, float n1,float r,float g,float b)
{
    glBegin(GL_LINES);
    glColor3f(r,g,b);
    glVertex2f(m0, m1);
    glVertex2f(n0, n1);
    glEnd();
}

vector<triangle1> transfer( vector<poly3d> vecpoly3d0)
{
    int numpoly,numtriangle;
    int pix;
    if(half==true) pix=400/3;
    else pix=400;
    
    int i,j;
    
    vector<triangle1> aaa;
    aaa.clear();
    numpoly=(unsigned)vecpoly3d0.size();
    for(i=0;i<numpoly;i++)
    {
        numtriangle=(unsigned)vecpoly3d0[i].vectriangle.size();
        for(j=0;j<numtriangle;j++)
        {
            int num1,num2,num3;
            int minx0,miny0,minz0;
            num1=vecpoly3d0[i].vectriangle[j].num1;
            num2=vecpoly3d0[i].vectriangle[j].num2;
            num3=vecpoly3d0[i].vectriangle[j].num3;
            float x1,y1,z1,x2,y2,z2,x3,y3,z3;
            int xx1,yy1,zz1,xx2,yy2,zz2,xx3,yy3,zz3;
            float r1,g1,b1,r2,g2,b2,r3,g3,b3;
            point3d1 point11,point22,point33;
            triangle1 triangle1;
            
            r1=vecpoly3d0[i].vectriangle[j].point1.r;
            g1=vecpoly3d0[i].vectriangle[j].point1.g;
            b1=vecpoly3d0[i].vectriangle[j].point1.b;
            x1=vecpoly3d0[i].vectriangle[j].point1.x;
            y1=vecpoly3d0[i].vectriangle[j].point1.y;
            z1=vecpoly3d0[i].vectriangle[j].point1.z;
            xx1= round(x1*pix);
            yy1= round(y1*pix);
            zz1= round(z1*pix);
            point11={xx1,yy1,zz1,r1,g1,b1};
            
            r2=vecpoly3d0[i].vectriangle[j].point2.r;
            g2=vecpoly3d0[i].vectriangle[j].point2.g;
            b2=vecpoly3d0[i].vectriangle[j].point2.b;
            x2=vecpoly3d0[i].vectriangle[j].point2.x;
            y2=vecpoly3d0[i].vectriangle[j].point2.y;
            z2=vecpoly3d0[i].vectriangle[j].point2.z;
            
            xx2= round(x2*pix);
            yy2= round(y2*pix);
            zz2= round(z2*pix);
            point22={xx2,yy2,zz2,r2,g2,b2};
            
            r3=vecpoly3d0[i].vectriangle[j].point3.r;
            g3=vecpoly3d0[i].vectriangle[j].point3.g;
            b3=vecpoly3d0[i].vectriangle[j].point3.b;
            x3=vecpoly3d0[i].vectriangle[j].point3.x;
            y3=vecpoly3d0[i].vectriangle[j].point3.y;
            z3=vecpoly3d0[i].vectriangle[j].point3.z;
            
            xx3= round(x3*pix);
            yy3= round(y3*pix);
            zz3= round(z3*pix);
            point33={xx3,yy3,zz3,r3,g3,b3};
            
            minx0=min0(xx1, xx2, xx3);
            miny0=min0(yy1, yy2, yy3);
            minz0=min0(zz1, zz2, zz3);
            
            triangle1={num1,num2,num3,point11,point22,point33,minx0,miny0,minz0};
            aaa.push_back(triangle1);
        }
    }
    return aaa;
}


vector<triangle1> sortz(vector<triangle1> tri0 )
{
    int numtriangle = (unsigned)tri0.size();
    int i,j;
    triangle1 tem;
    
    for(i=0;i<numtriangle-1;i++)
    {
        for(j=0;j<numtriangle-1-i;j++)
        {
            if(tri0[j].minz< tri0[j+1].minz)
            {
                tem= tri0[j];
                tri0[j]=tri0[j+1];
                tri0[j+1]=tem;
            }
        }
    }
    
    return tri0;
}

vector<triangle1> sortx(vector<triangle1> tri0 )
{
    int numtriangle = (unsigned)tri0.size();
    int i,j;
    triangle1 tem;
    
    for(i=0;i<numtriangle-1;i++)
    {
        for(j=0;j<numtriangle-1-i;j++)
        {
            if(tri0[j].minx< tri0[j+1].minx)
            {
                tem= tri0[j];
                tri0[j]=tri0[j+1];
                tri0[j+1]=tem;
            }
        }
    }
    
    return tri0;
}

vector<triangle1> sorty(vector<triangle1> tri0 )
{
    int numtriangle = (unsigned)tri0.size();
    int i,j;
    triangle1 tem;
    
    for(i=0;i<numtriangle-1;i++)
    {
        for(j=0;j<numtriangle-1-i;j++)
        {
            if(tri0[j].miny< tri0[j+1].miny)
            {
                tem= tri0[j];
                tri0[j]=tri0[j+1];
                tri0[j+1]=tem;
            }
        }
    }
    return tri0;
}

void drawpix0(int m, int n,float r,float g,float b)
{
    glPointSize(1);
    glBegin(GL_POINTS);
    glColor3f(r,g,b);
    glVertex3f(m,n,0);
    glEnd();
}


void randraw1(int num,int x,int y)
{
    glPointSize(1);
    glBegin(GL_POINTS);
    glColor3f(0,0,0);
    
    
    if(num == 0)
    {
        glVertex3f(x,y,0);
    }
    else if (num == 1)
    {
        glVertex3f(x+1,y,0);
    }
    else if (num == 2)
    {
        glVertex3f(x+2,y,0);
    }
    else if (num == 3)
    {
        glVertex3f(x,y+1,0);
    }
    else if (num == 4)
    {
        glVertex3f(x+1,y+1,0);
    }
    else if (num == 5)
    {
        glVertex3f(x+2,y+1,0);
    }
    else if (num == 6)
    {
        glVertex3f(x,y+2,0);
    }
    else if (num == 7)
    {
        glVertex3f(x+1,y+2,0);
    }
    else
    {
        glVertex3f(x+2,y+2,0);
    }
    glEnd();
}


void drawpix1(int x0, int y0,float r0,float g0, float b0)
{
    glPointSize(1);
    int x,y;
    x=3*x0-1;
    y=3*y0-1;
    int on;
    int nor,nog,nob;
    on=round(9*max1(r0,g0,b0));
    nor=round(r0/(r0+g0+b0)*float(on));
    nog=round(g0/(r0+g0+b0)*float(on));
    nob=round(b0/(r0+g0+b0)*float(on));
    
    float r[10]={1,1,1,1,1,1,1,1,1,1};
    float g[10]={1,1,1,1,1,1,1,1,1,1};
    float b[10]={1,1,1,1,1,1,1,1,1,1};
    if(on>0)
    {
        for (int i =1;i<=on;i++)
        {
            r[i]=0;
            g[i]=0;
            b[i]=0;
        }
    }
    
        glBegin(GL_POINTS);
        glColor3f(r[4],g[4],b[4]);
        glVertex3f(x+1,y+1,0);
    glColor3f(r[9],g[9],b[9]);
        glVertex3f(x+2,y+1,0);
    glColor3f(r[6],g[6],b[6]);
        glVertex3f(x+3,y+1,0);
    glColor3f(r[5],g[5],b[5]);
        glVertex3f(x+1,y+2,0);
    glColor3f(r[1],g[1],b[1]);
        glVertex3f(x+2,y+2,0);
    glColor3f(r[2],g[2],b[2]);
        glVertex3f(x+3,y+2,0);
    glColor3f(r[8],g[8],b[8]);
        glVertex3f(x+1,y+3,0);
    glColor3f(r[3],g[3],b[3]);
        glVertex3f(x+2,y+3,0);
    glColor3f(r[7],g[7],b[7]);
        glVertex3f(x+3,y+3,0);
        glEnd();
    
}


bool check(int m,vector<int> nn)
{
    int num=(unsigned)nn.size();
    if(num==0) return false;
    else if(num>0)
    {
        int i;
        for(i=0;i<num;i++)
        {
            if(m == nn[i] ) return true;
                }
        return false;
    }
    else
    {return false;}
}

void randraw(int num,int color,int x,int y)
{
    glPointSize(1);
    glBegin(GL_POINTS);
    if(color==1)
    {  glColor3f(1,0,0);}
    else if(color==2)
    {  glColor3f(0,1,0);}
    else
    {  glColor3f(0,0,1);}
    
    
    if(num == 0)
    {
        glVertex3f(x,y,0);
    }
    else if (num == 1)
    {
        glVertex3f(x+1,y,0);
    }
    else if (num == 2)
    {
        glVertex3f(x+2,y,0);
    }
    else if (num == 3)
    {
        glVertex3f(x,y+1,0);
    }
    else if (num == 4)
    {
        glVertex3f(x+1,y+1,0);
    }
    else if (num == 5)
    {
        glVertex3f(x+2,y+1,0);
    }
    else if (num == 6)
    {
        glVertex3f(x,y+2,0);
    }
    else if (num == 7)
    {
        glVertex3f(x+1,y+2,0);
    }
    else
    {
        glVertex3f(x+2,y+2,0);
    }
    glEnd();
}

void drawpix2(int x0, int y0,float r0,float g0, float b0)
{
    int x,y;
    x=3*x0;
    y=3*y0;
    int on;
    int nor,nog,nob;
    on=round(9*max1(r0,g0,b0));
    nor=round(r0/(r0+g0+b0)*float(on));
    nog=round(g0/(r0+g0+b0)*float(on));
    nob=round(b0/(r0+g0+b0)*float(on));
    
    vector<int> aaaa;
    aaaa.clear();
    
    for(int i=1;i<=nor;i++)
    {
        int num0 = rand()%9;
        bool in = true;
        while(in)
        {
            in = check(num0, aaaa);
            if(in)
            {num0 = rand()%9;}
        }
        aaaa.push_back(num0);
        randraw(num0, 1, x, y);
    }
    
    for(int i=1;i<=nog;i++)
    {
        int num0 = rand()%9;
        bool in = true;
        while(in)
        {
            in = check(num0, aaaa);
            if(in)
            {num0 = rand()%9;}
        }
        aaaa.push_back(num0);
        randraw(num0, 2, x, y);
    }
    
    for(int i=1;i<=nob;i++)
    {
        int num0 = rand()%9;
        bool in = true;
        while(in)
        {
            in = check(num0, aaaa);
            if(in)
            {num0 = rand()%9;}
        }
        aaaa.push_back(num0);
        randraw(num0, 3, x, y);
    }
}

void drawpix3(int x0, int y0,float r0,float g0, float b0)
{
    int x,y;
    x=3*x0;
    y=3*y0;
    int on;
    on=round(9*max1(r0,g0,b0));
    
    vector<int> aaaa;
    aaaa.clear();
    
    for(int i=1;i<on;i++)
    {
        int num0 = rand()%9;
        bool in = true;
        while(in)
        {
            in = check(num0, aaaa);
            if(in)
            {num0 = rand()%9;}
        }
        aaaa.push_back(num0);
        randraw1(num0, x, y);
    }
    
}

void drawlinexy(point3d1 point1 , point3d1 point2)
{
    
    int x0 = point1.x;
    int y0 = point1.y;
    int x1 = point2.x;
    int y1 = point2.y;
    int z0 = point1.z;
    int z1 = point2.z;
    float r0 = point1.r;
    float g0 = point1.g;
    float b0 = point1.b;
    float r1 = point2.r;
    float g1 = point2.g;
    float b1 = point2.b;
    float r3,g3,b3;
    bool mmm = false;
    
    int dx = x1 - x0;
    int dy = y1 - y0;
    int step,i;
    float x = x0;
    float y = y0;
    float xincrement,yincrement;
    
    if(abs(dx) > abs(dy))
    {
        step = abs(dx);
        mmm = true;
    }
    else
        step = abs(dy);
    
    if(step!=0)
    {
        xincrement= float(dx) / float(step);
        yincrement= float(dy) / float(step);
        
        for(i=0; i<=step; i++)
        {
            int x3,y3;
            x3=round(x);
            y3=round(y);
            if (mmm)
            {
                r3=float(abs(x3-x1))/step*r0+float(abs(x3-x0))/step*r1;
                g3=float(abs(x3-x1))/step*g0+float(abs(x3-x0))/step*g1;
                b3=float(abs(x3-x1))/step*b0+float(abs(x3-x0))/step*b1;
            }
            else
            {
                r3=float(abs(y3-y1))/step*r0+float(abs(y3-y0))/step*r1;
                g3=float(abs(y3-y1))/step*g0+float(abs(y3-y0))/step*g1;
                b3=float(abs(y3-y1))/step*b0+float(abs(y3-y0))/step*b1;
            }
            
            if(half == true)
            {
                if(whichcolor==0)
                {
                    if(random0==0)
                    drawpix1(x3,y3,r3,g3,b3);
                    else
                    drawpix3(x3,y3,r3,g3,b3);
                }
                else
                drawpix2(x3,y3,r3,g3,b3);
            }
            else
            {drawpix0(x3,y3,r3,g3,b3);}
            
            x += xincrement;
            y += yincrement;
        }
    }
    
    else if(step == 0)
    {
        if(z0<=z1)
        {
            if(half == true)
            {
                if(whichcolor==0)
                {
                    if(random0==0)
                    drawpix1(x0,y0,r0,g0,b0);
                    else
                    drawpix3(x0,y0,r0,g0,b0);
                }
                else
                drawpix2(x0,y0,r0,g0,b0);
            }
            else
            {drawpix0(x0,y0,r0,g0,b0);}
        }
        
        else
        {
            if(half == true)
            {
                if(whichcolor==0)
                {
                    if(random0==0)
                    drawpix1(x1,y1,r1,g1,b1);
                    else
                    drawpix3(x1,y1,r1,g1,b1);
                }
                else
                drawpix2(x1,y1,r1,g1,b1);
            }
            else
            {drawpix0(x1,y1,r1,g1,b1);}
        }
        
    }
}

void drawtrianglexy(triangle1 triangle0)
{
    drawlinexy(triangle0.point1, triangle0.point2);
    drawlinexy(triangle0.point1, triangle0.point3);
    drawlinexy(triangle0.point2, triangle0.point3);
}

void projectxy(vector<triangle1> tri0)
{
    int numtriangle = (unsigned)tri0.size();
    int i;
    
    for(i=0;i<numtriangle;i++)
    {
       // drawtrianglexy(tri0[i]);
        rasterxy(tri0[i]);
    }
}

info0 getinfoxy(triangle1 triangle0)
{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3;
    int xmin,xmax,ymin,ymax;
    
    x1 = triangle0.point1.x;
    y1 = triangle0.point1.y;
    z1 = triangle0.point1.z;
    
    x2 = triangle0.point2.x;
    y2 = triangle0.point2.y;
    z2 = triangle0.point2.z;
    
    x3 = triangle0.point3.x;
    y3 = triangle0.point3.y;
    z3 = triangle0.point3.z;
    
    xmin=min0(x1, x2, x3);
    xmax=max0(x1, x2, x3);
    ymin=min0(y1, y2, y3);
    ymax=max0(y1, y2, y3);
    
    info0 aa={xmin,xmax,ymin,ymax};
    return aa;
}

point3d1 interxy(int mm, point3d1 point1, point3d1 point2)
{
    int x1,y1,z1,x2,y2,z2;
    float r1,g1,b1,r2,g2,b2,r3,g3,b3;
    int dx,dy;
    int x3;
    int y3=mm;
    float x;
    
    x1 = point1.x;
    y1 = point1.y;
    z1 = point1.z;
    r1 = point1.r;
    g1 = point1.g;
    b1 = point1.b;

    x2 = point2.x;
    y2 = point2.y;
    z2 = point2.z;
    r2 = point2.r;
    g2 = point2.g;
    b2 = point2.b;
    
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    
    x=float(x2-x1)*float(mm-y1)/float(y2-y1)+x1;
    x3=round(x);
    
    if(dx>=dy)
    {
        r3=float(abs(x3-x1))/dx*r2+float(abs(x3-x2))/dx*r1;
        g3=float(abs(x3-x1))/dx*g2+float(abs(x3-x2))/dx*g1;
        b3=float(abs(x3-x1))/dx*b2+float(abs(x3-x2))/dx*b1;
    }
    else
    {
        r3=float(abs(y3-y1))/dy*r2+float(abs(y3-y2))/dy*r1;
        g3=float(abs(y3-y1))/dy*g2+float(abs(y3-y2))/dy*g1;
        b3=float(abs(y3-y1))/dy*b2+float(abs(y3-y2))/dy*b1;
    }
    
    point3d1 aaa={x3,y3,0,r3,g3,b3};
    return aaa;
}


vector<point3d1> intersectxy(int mm, triangle1 triangle0)
{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3;
    vector<point3d1> bbb;
    bbb.clear();
     
    x1 = triangle0.point1.x;
    y1 = triangle0.point1.y;
    z1 = triangle0.point1.z;
     
    x2 = triangle0.point2.x;
    y2 = triangle0.point2.y;
    z2 = triangle0.point2.z;
     
    x3 = triangle0.point3.x;
    y3 = triangle0.point3.y;
    z3 = triangle0.point3.z;
    
    if((y1!=y2) && (y2!=y3)&&(y1!=y3))
    {
        if(mm == y1)
        {
            bbb.push_back(triangle0.point1);
            if(((mm>y2)&&(mm<y3))||((mm>y3)&&(mm<y2)))
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point2 , triangle0.point3);
                bbb.push_back(ccc0);
            }
        }
        else if (mm == y2)
        {
            bbb.push_back(triangle0.point2);
            if(((mm>y1)&&(mm<y3))||((mm>y3)&&(mm<y1)))
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point3);
                bbb.push_back(ccc0);
            }
        }
        else if (mm == y3)
        {
            bbb.push_back(triangle0.point3);
            if(((mm>y1)&&(mm<y2))||((mm>y2)&&(mm<y1)))
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point2);
                bbb.push_back(ccc0);
            }
        }
        else
        {
            if(  ( (mm>y1) && (mm<y2) ) || ( (mm<y1) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point2);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y1) && (mm<y3) ) || ( (mm<y1) && (mm>y3) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point3);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y3) && (mm<y2) ) || ( (mm<y3) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point3 , triangle0.point2);
                bbb.push_back(ccc0);
            }
        }
    }
    
    else if (y1==y2)
    {
        if(mm == y1)
        {
            bbb.push_back(triangle0.point1);
            bbb.push_back(triangle0.point2);
        }
        if(mm == y3)
        {bbb.push_back(triangle0.point3);}
        else
        {
            if(  ( (mm>y1) && (mm<y2) ) || ( (mm<y1) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point2);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y1) && (mm<y3) ) || ( (mm<y1) && (mm>y3) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point3);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y3) && (mm<y2) ) || ( (mm<y3) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point3 , triangle0.point2);
                bbb.push_back(ccc0);
            }
        }

    }
    else if (y1==y3)
    {
        if(mm == y1)
        {
            bbb.push_back(triangle0.point1);
            bbb.push_back(triangle0.point3);
        }
        if(mm == y2)
        {bbb.push_back(triangle0.point2);}
        else
        {
            if(  ( (mm>y1) && (mm<y2) ) || ( (mm<y1) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point2);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y1) && (mm<y3) ) || ( (mm<y1) && (mm>y3) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point3);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y3) && (mm<y2) ) || ( (mm<y3) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point3 , triangle0.point2);
                bbb.push_back(ccc0);
            }
        }
    }
    
    else if (y2==y3)
    {
        if(mm == y2)
        {
            bbb.push_back(triangle0.point3);
            bbb.push_back(triangle0.point2);
        }
        if(mm == y1)
        {bbb.push_back(triangle0.point1);}
        else
        {
            if(  ( (mm>y1) && (mm<y2) ) || ( (mm<y1) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point2);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y1) && (mm<y3) ) || ( (mm<y1) && (mm>y3) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point1 , triangle0.point3);
                bbb.push_back(ccc0);
            }
            if(  ( (mm>y3) && (mm<y2) ) || ( (mm<y3) && (mm>y2) )  )
            {
                point3d1 ccc0;
                ccc0 = interxy(mm,triangle0.point3 , triangle0.point2);
                bbb.push_back(ccc0);
            }
        }
    }
    
    return bbb;
    
}


void rasterxy(triangle1 triangle0)
{
    info0 aaa = getinfoxy(triangle0);
    int x1,y1,z1,x2,y2,z2,x3,y3,z3;
    x1 = triangle0.point1.x;
    y1 = triangle0.point1.y;
    z1 = triangle0.point1.z;
     
    x2 = triangle0.point2.x;
    y2 = triangle0.point2.y;
    z2 = triangle0.point2.z;
     
    x3 = triangle0.point3.x;
    y3 = triangle0.point3.y;
    z3 = triangle0.point3.z;
   // int xmin=aaa.xmin;
   // int xmax=aaa.xmax;
    int ymin=aaa.ymin;
    int ymax=aaa.ymax;
    int i;
    
    if (  !((y1==y2)&&(y1==y3) ) && !( (x1==x2)&&(x1==x3) ) )
    {
        for(i=ymax;i>ymin;i--)
        {
            vector<point3d1> bbb;
            bbb.clear();
            
            bbb=intersectxy(i, triangle0);
            int num;
            num = (unsigned)bbb.size();
            if(num == 1)
            {
                if(half == true)
                {
                    if(whichcolor==0)
                    {
                        if (random0==0)
                        drawpix1(bbb[0].x, bbb[0].y, bbb[0].r, bbb[0].g, bbb[0].g);
                        else
                        drawpix3(bbb[0].x, bbb[0].y, bbb[0].r, bbb[0].g, bbb[0].g);
                    }
                    else
                        drawpix2(bbb[0].x, bbb[0].y, bbb[0].r, bbb[0].g, bbb[0].g);
                }
                else
                {
                    drawpix0(bbb[0].x, bbb[0].y, bbb[0].r, bbb[0].g, bbb[0].g);
                }
            }
            
            else if(num == 2)
            {drawlinexy(bbb[0], bbb[1]);}
        }
    }
    
    else if( ((y1==y2)&&(y1==y3) ) && ( (x1==x2)&&(x1==x3) ) )
    {
        if( (z1 >= z2)&& (z1>= z3))
        {
            if(half == true)
            {
                if (whichcolor==0)
                {
                    if (random0==0)
                    drawpix1(triangle0.point1.x, triangle0.point1.y, triangle0.point1.r, triangle0.point1.g, triangle0.point1.b);
                    else
                    drawpix3(triangle0.point1.x, triangle0.point1.y, triangle0.point1.r, triangle0.point1.g, triangle0.point1.b);
                }
                else
                    drawpix2(triangle0.point1.x, triangle0.point1.y, triangle0.point1.r, triangle0.point1.g, triangle0.point1.b);
            }
            else
            {
                drawpix0(triangle0.point1.x, triangle0.point1.y, triangle0.point1.r, triangle0.point1.g, triangle0.point1.b);
            }
        }
        
        
        if( (z2 >= z1)&& (z2>= z3))
            {
                if(half == true)
                {
                    if(whichcolor==0)
                    {
                        if (random0==0)
                            drawpix1(triangle0.point2.x, triangle0.point2.y, triangle0.point2.r, triangle0.point2.g, triangle0.point2.b);
                        else
                            drawpix3(triangle0.point2.x, triangle0.point2.y, triangle0.point2.r, triangle0.point2.g, triangle0.point2.b);
                    }
                    else
                        drawpix2(triangle0.point2.x, triangle0.point2.y, triangle0.point2.r, triangle0.point2.g, triangle0.point2.b);
                }
                else
                {
                    drawpix0(triangle0.point2.x, triangle0.point2.y, triangle0.point2.r, triangle0.point2.g, triangle0.point2.b);
                }
            }
            
        if( (z3 >= z2)&& (z3>= z1))
            
            {
                if(half == true)
                {
                    if (whichcolor==0)
                    {
                        if(random0==0)
                            drawpix1(triangle0.point3.x, triangle0.point3.y, triangle0.point3.r, triangle0.point3.g, triangle0.point3.b);
                        else
                            drawpix3(triangle0.point3.x, triangle0.point3.y, triangle0.point3.r, triangle0.point3.g, triangle0.point3.b);
                    }
                    else
                        drawpix2(triangle0.point3.x, triangle0.point3.y, triangle0.point3.r, triangle0.point3.g, triangle0.point3.b);
                }
                else
                {
                    drawpix0(triangle0.point3.x, triangle0.point3.y, triangle0.point3.r, triangle0.point3.g, triangle0.point3.b);
                }
            }
    }
      
}

vector<triangle1> transxz(vector<triangle1> aaa)
{

    int numtri = (unsigned)aaa.size();
    int i;
    int tem1,tem2,tem3;
    int tem11;
    
    for (i=0;i<numtri;i++)
    {
        tem1=aaa[i].point1.z;
        aaa[i].point1.z=aaa[i].point1.y;
        aaa[i].point1.y=tem1;
        
        tem2=aaa[i].point2.z;
        aaa[i].point2.z=aaa[i].point2.y;
        aaa[i].point2.y=tem2;
        
        tem3=aaa[i].point3.z;
        aaa[i].point3.z=aaa[i].point3.y;
        aaa[i].point3.y=tem3;
        
        tem11=aaa[i].minz;
        aaa[i].minz=aaa[i].miny;
        aaa[i].miny=tem11;
    }
    
    return aaa;
}

vector<triangle1> transyz(vector<triangle1> aaa)
{

    int numtri = (unsigned)aaa.size();
    int i;
    int tem1,tem2,tem3;

    
    for (i=0;i<numtri;i++)
    {
        tem1=aaa[i].point1.z;
        tem2=aaa[i].point1.x;
        tem3=aaa[i].point1.y;
        aaa[i].point1.x=tem3;
        aaa[i].point1.y=tem1;
        aaa[i].point1.z=tem2;
        
        tem1=aaa[i].point2.z;
        tem2=aaa[i].point2.x;
        tem3=aaa[i].point2.y;
        aaa[i].point2.x=tem3;
        aaa[i].point2.y=tem1;
        aaa[i].point2.z=tem2;
        
        tem1=aaa[i].point3.z;
        tem2=aaa[i].point3.x;
        tem3=aaa[i].point3.y;
        aaa[i].point3.x=tem3;
        aaa[i].point3.y=tem1;
        aaa[i].point3.z=tem2;
        
        tem1=aaa[i].minz;
        tem2=aaa[i].minx;
        tem3=aaa[i].miny;
        aaa[i].minz=tem2;
        aaa[i].miny=tem1;
        aaa[i].minx=tem3;
    }
    return aaa;
}
