//
//  main.cpp
//  project2
//
//  Created by Shuai Ryan on 10/25/19.
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

int grid_width=80;
int grid_height=80;
float pixel_size=10;
int win_height=800;
int win_width=800;

struct point3d{
    float x;
    float y;
    float z;
};

struct line3d{
    point3d point1;
    point3d point2;
    int num1;
    int num2;
};

struct poly3d{
    int name;
    vector<point3d> vec3d1;
    vector<line3d>  vec3d2;
};

vector<poly3d> vec3d3;

struct point2d{
    float mm1;
    float mm2;
};

struct line2d{
    point2d point1;
    point2d point2;
    int num1;
    int num2;
};

struct poly2d{
    int name;
    vector<point2d> vec2d1;
    vector<line2d> vec2d2;
};

struct info{
    float mm1min;
    float mm1max;
    float mm2min;
    float mm2max;
};


void init();
void idle();
void display();

void drawpix(float m, float n);
void drawline(float m0, float m1 , float n0, float n1);
void drawline2(float m0, float m1 , float n0, float n1);
void drawline3(float m0, float m1 , float n0, float n1);

void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();

void readfile();
void savefile(vector<poly3d> vec1, poly3d polyh1, int index);

info readinfo(poly2d polyg);

poly2d projectionxz(poly3d polyh);
poly2d projectionxy(poly3d polyh);
poly2d projectionyz(poly3d polyh);
void normalization(poly2d polyg, float mm1min,float mm1max, float mm2min, float mm2max,bool rotation,float x00, float y00, float x11, float y11);
void drawxy(bool rotation,float x00, float y00, float x11, float y11);
void drawxz(bool rotation,float x00, float y00, float x11, float y11);
void drawyz(bool rotation,float x00, float y00, float x11, float y11);
poly3d translate(poly3d polyh1, float x0, float y0,float z0);
poly3d scale(poly3d polyh1, float x0, float y0,float z0);
point3d center(poly3d polyh1);
poly3d scalecenter(poly3d polyh1, float x0, float y0,float z0);
poly3d rotatez(poly3d polyh1,float angle);
poly3d rotatetoz(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1);
poly3d rotatetoorignal(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1);
poly3d rotate111(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1,float angle);

int main(int argc, char **argv)
{
    

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("project2");
    
    glutDisplayFunc(display); //rendering calls here

    
    glutIdleFunc(idle);       //Function called while program is
    
    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}

/*initialize gl stufff*/
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1.0, 0, 1.0);
    check();
}


void idle()
{
    glutPostRedisplay();
}


void display()
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    vec3d3.clear();
    readfile();
    int numpolyh = (unsigned) vec3d3.size();
    cout<<"There are    "<<numpolyh<<"  polyhedras"<<endl;
    
    int operation;
    int index;
    char op;
    float dx,dy,dz;
    float scale1,scale2,scale3;
    float x0=0,y0=0,z0=0;
    float x1=0,y1=0,z1=0;
    float anglenum;
    bool rotation = false;
    
    
    cout<<"Any operation with polyhedras ?"<<endl;
    cin >> operation;
    poly3d polyh1;
    
    if (operation == 1)
    {
        cout<<"which polyhedras?"<<endl;
        cin >> index ;
        if (index >= numpolyh || index < 0)
        {
            cout<<"Error:NO this polyhedra!!"<<endl;
        }
        
        else
        {
            cout<<"Which operation???(t:translation   s: scale    r:rotate)"<<endl;
            cin >> op;
            switch(op)
            {
                case 't':
                {
                    cout << "Enter the translation vector."<<endl;
                    cin >> dx;
                    cin >> dy;
                    cin >> dz;
                    polyh1 = translate(vec3d3[index], dx, dy, dz);
                    savefile(vec3d3, polyh1, index);
                }
                    break;
                    
                case 's':
                {
                    cout << "Enter the scaling factor"<<endl;
                    cin >> scale1;
                    cin >> scale2;
                    cin >> scale3;
                    polyh1 = scale(vec3d3[index], scale1, scale2, scale3);
                    savefile(vec3d3, polyh1, index);
                }
                    break;
                
                case 'r':
                {
                    cout << "Enter two points on the rotation axis"<<endl;
                    cin >> x0;
                    cin >> y0;
                    cin >> z0;
                    cin >> x1;
                    cin >> y1;
                    cin >> z1;
                    cout <<"Enter rotation angle"<<endl;
                    cin >> anglenum;
                    polyh1 = rotate111(vec3d3[index], x0, y0, z0, x1, y1, z1, anglenum);
                    savefile(vec3d3, polyh1, index);
                    rotation = true;
                }
                    break;
                    
                default:
                {
                    cout<<"Error: No this operation!!!"<<endl;
                }
                    break;
            }
            vec3d3.clear();
            readfile();
        }
    }

    glViewport(0, 0, 800, 800);
      drawline2(0, 0.5, 1, 0.5);
      drawline2(0.5, 0, 0.5, 1);
      drawpix(0, 0);
      drawpix(1, 1);
      drawpix(0.5, 0.5);
      drawpix(0, 0.5);
      drawpix(0,1);
      drawpix(0.5,0);
      drawpix(0.5,1);
      drawpix(1, 0);
      drawpix(1, 0.5);

    
      glViewport(0, 400, 400, 400);
      drawxz(rotation,x0,z0,x1,z1);


      glViewport(400, 400, 400, 400);
      drawxy(rotation,x0,y0,x1,y1);
      
      glViewport(0, 0, 400, 400);
      drawyz(rotation,y0,z0,y1,z1);
    
      glViewport(400, 0, 400, 400);

    
      glFlush();
    
    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    check();
}

void drawpix(float m, float n)
{
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(m , n ,0);
    glEnd();
}

void drawline(float m0, float m1 , float n0, float n1)
{
    glBegin(GL_LINES);
    glColor3f(0.5,0.5,1.0);
    glVertex2f(m0, m1);
    glVertex2f(n0, n1);
    glEnd();
}

void drawline2(float m0, float m1 , float n0, float n1)
{
    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    glVertex2f(m0, m1);
    glVertex2f(n0, n1);
    glEnd();
}

void drawline3(float m0, float m1 , float n0, float n1)
{
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(m0, m1);
    glVertex2f(n0, n1);
    glEnd();
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

void readfile()
{
    int namepol;
    ifstream file;
    int numpol;
    int numpoint;
    int numline;
  
    int i,j,k;
    float x,y,z;
    int m,n;
    
 //   file.open("/Users/shuairyan/Desktop/bunny_scene.txt");
    file.open("/Users/shuairyan/Desktop/test_scene\ .txt");
    if (!file)
    {
        cout<<"file not exist"<<endl;
    }
    if (file.is_open())
        {
            file >> numpol;
            for(i=0; i<numpol; i++)
            {
                file >> namepol;
                file >> numpoint;
                vector<point3d> aa;
                aa.clear();
                vector<line3d>  dd;
                dd.clear();
                
                for(j=0; j<numpoint; j++)
                {
                    file>> x;
                    file>> y;
                    file>> z;
                    point3d bb={x,y,z};
                    aa.push_back(bb);
                }
                file >> numline;
                
                for(k=0; k<numline; k++)
                {
                    file>> m;
                    file>> n;
                    point3d point1 = aa[m-1];
                    point3d point2 = aa[n-1];
                    line3d  cc={ point1 , point2, m , n };
                    dd.push_back(cc);
                }
                poly3d mm={namepol,aa,dd};
                vec3d3.push_back(mm);
                vector<point3d>().swap(aa);
                vector<line3d>().swap(dd);
            }
        }

    file.close();
}

void savefile(vector<poly3d> vec1, poly3d polyh1, int index)
{
    ofstream file;
    //    file.open("/Users/shuairyan/Desktop/bunny_scene.txt");
    file.open("/Users/shuairyan/Desktop/test_scene\ .txt");
    if (!file)
    {
        cout<<"file not exist"<<endl;
        return;
    }

    int i,j,k;
    float mm1,mm2,mm3;
    int num1,num2;
    
    int numpoly = (unsigned)vec1.size();
    file << numpoly << "\n";
    
    for (i=0; i<numpoly; i++)
    {
        int numpoint = (unsigned) vec1[i].vec3d1.size();
        int numline  = (unsigned) vec1[i].vec3d2.size();
        
        file<< i <<"\n";
        file<<numpoint<<"\n";
        
        if(i != index)
        {
            for(j=0; j<numpoint;j++)
            {
                mm1=vec1[i].vec3d1[j].x;
                mm2=vec1[i].vec3d1[j].y;
                mm3=vec1[i].vec3d1[j].z;
                file << mm1<<"  "<<mm2<<"   "<<mm3<<"\n";
            }
        }
        
        else if (i == index)
        {
            for(j=0;j<numpoint;j++)
            {
                mm1=polyh1.vec3d1[j].x;
                mm2=polyh1.vec3d1[j].y;
                mm3=polyh1.vec3d1[j].z;
                file << mm1<<"  "<<mm2<<"   "<<mm3<<"\n";
            }
        }
        
        
        file<< numline <<"\n";
        
        for(k=0;k<numline;k++)
        {
            num1=vec1[i].vec3d2[k].num1;
            num2=vec1[i].vec3d2[k].num2;
            file << num1<<" "<<num2<<"\n";
        }
    }
        file.close();
}


poly2d projectionxy(poly3d polyh)
{
    int namepoly = polyh.name;
    int numpoint = (unsigned)polyh.vec3d1.size();
    int numline = (unsigned)polyh.vec3d2.size();
    int i,j;
    float mm1,mm2;
    int no1,no2;
    
    vector<point2d> point;
    point.clear();
    vector<line2d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyh.vec3d1[i].x;
        mm2 = polyh.vec3d1[i].y;
        point2d aa={mm1,mm2};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh.vec3d2[j].num1;
        no2 = polyh.vec3d2[j].num2;
        point2d point11 = point[no1 -1];
        point2d point22 = point[no2 -1];
        line2d bb ={point11,point22,no1,no2};
        line.push_back(bb);
    }
    
    poly2d cc= {namepoly,point,line};
    vector<point2d>().swap(point);
    vector<line2d>().swap(line);
    
    return cc;
}

poly2d projectionxz(poly3d polyh)
{
    int namepoly = polyh.name;
    int numpoint = (unsigned)polyh.vec3d1.size();
    int numline = (unsigned)polyh.vec3d2.size();
    int i,j;
    float mm1,mm2;
    int no1,no2;
    
    vector<point2d> point;
    point.clear();
    vector<line2d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyh.vec3d1[i].x;
        mm2 = polyh.vec3d1[i].z;
        point2d aa={mm1,mm2};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh.vec3d2[j].num1;
        no2 = polyh.vec3d2[j].num2;
        point2d point11 = point[no1 -1];
        point2d point22 = point[no2 -1];
        line2d bb ={point11,point22,no1,no2};
        line.push_back(bb);
    }
    
    poly2d cc= {namepoly,point,line};
    vector<point2d>().swap(point);
    vector<line2d>().swap(line);
    
    return cc;
}


poly2d projectionyz(poly3d polyh)
{
    int namepoly = polyh.name;
    int numpoint = (unsigned)polyh.vec3d1.size();
    int numline = (unsigned)polyh.vec3d2.size();
    int i,j;
    float mm1,mm2;
    int no1,no2;
    
    vector<point2d> point;
    point.clear();
    vector<line2d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyh.vec3d1[i].y;
        mm2 = polyh.vec3d1[i].z;
        point2d aa={mm1,mm2};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh.vec3d2[j].num1;
        no2 = polyh.vec3d2[j].num2;
        point2d point11 = point[no1 -1];
        point2d point22 = point[no2 -1];
        line2d bb ={point11,point22,no1,no2};
        line.push_back(bb);
    }
    
    poly2d cc= {namepoly,point,line};
    vector<point2d>().swap(point);
    vector<line2d>().swap(line);
    
    return cc;
}

void drawxy(bool rotation,float x00, float y00, float x11, float y11)
{
    int numpoly = (unsigned)vec3d3.size();
    int i;
    info info1,info2;
    float mm11,mm12,mm21,mm22;
    
    poly2d nn2;
    nn2 = projectionxy(vec3d3[0]);
    info1=readinfo(nn2);
    mm11=info1.mm1min;
    mm12=info1.mm1max;
    mm21=info1.mm2min;
    mm22=info1.mm2max;

    for (i=1; i<numpoly; i++)
    {
        nn2 = projectionxy(vec3d3[i]);
        info2 = readinfo(nn2);
        if (info2.mm1min <= mm11) mm11 = info2.mm1min;
        if (info2.mm1max >= mm12) mm12 = info2.mm1max;
        if (info2.mm2min <= mm21) mm21 = info2.mm2min;
        if (info2.mm2max >= mm22) mm22 = info2.mm2max;
    }
    
    for (i=0; i<numpoly; i++)
    {
        poly2d nn3;
        nn3 = projectionxy(vec3d3[i]);
        normalization(nn3,mm11,mm12,mm21,mm22,rotation,x00,y00,x11,y11);
    }
    
}

void drawxz(bool rotation,float x00, float y00, float x11, float y11)
{
    int numpoly = (unsigned)vec3d3.size();
    int i;
    info info1,info2;
    float mm11,mm12,mm21,mm22;
    
    poly2d nn2;
    nn2 = projectionxz(vec3d3[0]);
    info1=readinfo(nn2);
    mm11=info1.mm1min;
    mm12=info1.mm1max;
    mm21=info1.mm2min;
    mm22=info1.mm2max;

    for (i=1; i<numpoly; i++)
    {
        nn2 = projectionxz(vec3d3[i]);
        info2 = readinfo(nn2);
        if (info2.mm1min <= mm11) mm11 = info2.mm1min;
        if (info2.mm1max >= mm12) mm12 = info2.mm1max;
        if (info2.mm2min <= mm21) mm21 = info2.mm2min;
        if (info2.mm2max >= mm22) mm22 = info2.mm2max;
    }
    
    for (i=0; i<numpoly; i++)
    {
        poly2d nn3;
        nn3 = projectionxz(vec3d3[i]);
        normalization(nn3,mm11,mm12,mm21,mm22,rotation,x00,y00,x11,y11);
    }
    
}

void drawyz(bool rotation,float x00, float y00, float x11, float y11)
{
    int numpoly = (unsigned)vec3d3.size();
    int i;
    info info1,info2;
    float mm11,mm12,mm21,mm22;
    
    poly2d nn2;
    nn2 = projectionyz(vec3d3[0]);
    info1=readinfo(nn2);
    mm11=info1.mm1min;
    mm12=info1.mm1max;
    mm21=info1.mm2min;
    mm22=info1.mm2max;

    for (i=1; i<numpoly; i++)
    {
        nn2 = projectionyz(vec3d3[i]);
        info2 = readinfo(nn2);
        if (info2.mm1min <= mm11) mm11 = info2.mm1min;
        if (info2.mm1max >= mm12) mm12 = info2.mm1max;
        if (info2.mm2min <= mm21) mm21 = info2.mm2min;
        if (info2.mm2max >= mm22) mm22 = info2.mm2max;
    }
    
    for (i=0; i<numpoly; i++)
    {
        poly2d nn3;
        nn3 = projectionyz(vec3d3[i]);
        normalization(nn3,mm11,mm12,mm21,mm22,rotation,x00,y00,x11,y11);
    }
    
}

poly3d translate(poly3d polyh1, float x0, float y0,float z0)
{
    int namepoly = polyh1.name;
    int numpoint = (unsigned)polyh1.vec3d1.size();
    int numline = (unsigned)polyh1.vec3d2.size();
    int i,j;
    float mm1,mm2,mm3;
    int no1,no2;
    
    vector<point3d> point;
    point.clear();
    vector<line3d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyh1.vec3d1[i].x;
        mm2 = polyh1.vec3d1[i].y;
        mm3 = polyh1.vec3d1[i].z;
        mm1 += x0;
        mm2 += y0;
        mm3 += z0;
        point3d aa={mm1,mm2,mm3};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh1.vec3d2[j].num1;
        no2 = polyh1.vec3d2[j].num2;
        point3d point11 = point[no1 -1];
        point3d point22 = point[no2 -1];
        line3d bb ={point11,point22,no1,no2};
        line.push_back(bb);
    }
    
    poly3d cc= {namepoly,point,line};
    vector<point3d>().swap(point);
    vector<line3d>().swap(line);
    return cc;
}

poly3d scale(poly3d polyh1, float x0, float y0,float z0)
{
    int namepoly = polyh1.name;
    int numpoint = (unsigned)polyh1.vec3d1.size();
    int numline = (unsigned)polyh1.vec3d2.size();
    int i,j;
    float mm1,mm2,mm3;
    int no1,no2;
    
    vector<point3d> point;
    point.clear();
    vector<line3d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyh1.vec3d1[i].x;
        mm2 = polyh1.vec3d1[i].y;
        mm3 = polyh1.vec3d1[i].z;
        mm1 *= x0;
        mm2 *= y0;
        mm3 *= z0;
        point3d aa={mm1,mm2,mm3};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh1.vec3d2[j].num1;
        no2 = polyh1.vec3d2[j].num2;
        point3d point11 = point[no1 -1];
        point3d point22 = point[no2 -1];
        line3d bb ={point11,point22,no1,no2};
        line.push_back(bb);
    }
    
    poly3d cc= {namepoly,point,line};
    vector<point3d>().swap(point);
    vector<line3d>().swap(line);
    return cc;
}

point3d center(poly3d polyh1)
{
    float centerx = 0.0,centery =0.0,centerz =0.0;
    
    int mm = (unsigned)polyh1.vec3d1.size();
    int i;
    for (i=0;i<mm;i++)
    {
        centerx += polyh1.vec3d1[i].x;
        centery += polyh1.vec3d1[i].y;
        centerz += polyh1.vec3d1[i].z;
    }
    centerx /= mm;
    centery /= mm;
    centerz /= mm;
    
    point3d nn={centerx,centery,centerz};
    return nn;
}

poly3d scalecenter(poly3d polyh1, float x0, float y0,float z0)
{
    point3d center1= center(polyh1);
    poly3d polyh2= translate(polyh1, - center1.x, - center1.y, - center1.z);
    poly3d polyh3= scale(polyh2, x0, y0, z0);
    poly3d polyh4= translate(polyh3, center1.x, center1.y, center1.z);
    return polyh4;
}


poly3d rotatez(poly3d polyh1,float angle)
{
        float pai = 3.1415926/180;

        int namepoly = polyh1.name;
        int numpoint = (unsigned)polyh1.vec3d1.size();
        int numline = (unsigned)polyh1.vec3d2.size();
        int i,j;
        float nn1,nn2,nn3;
        float mm1,mm2,mm3;
        int no1,no2;
        
        vector<point3d> point;
        point.clear();
        vector<line3d> line;
        line.clear();
        

        for(i=0; i<numpoint; i++)
        {
            nn1 = polyh1.vec3d1[i].x;
            nn2 = polyh1.vec3d1[i].y;
            nn3 = polyh1.vec3d1[i].z;
            mm1 = nn1*cos(angle * pai)-nn2*sin(angle * pai);
            mm2 = nn1*sin(angle * pai)+nn2*cos(angle * pai) ;
            mm3 = nn3;
            point3d aa={mm1,mm2,mm3};
            point.push_back(aa);
        }
        
        for(j=0; j<numline ; j++)
        {
            no1 = polyh1.vec3d2[j].num1;
            no2 = polyh1.vec3d2[j].num2;
            point3d point11 = point[no1 -1];
            point3d point22 = point[no2 -1];
            line3d bb ={point11,point22,no1,no2};
            line.push_back(bb);
        }
        
        poly3d cc= {namepoly,point,line};
        vector<point3d>().swap(point);
        vector<line3d>().swap(line);
        return cc;
    }

poly3d rotatetoz(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1)
{
    float a = x1-x0;
    float b = y1-y0;
    float c = z1-z0;
    float d = sqrt(a*a+b*b+c*c);
    float dx = a/d;
    float dy = b/d;
    float dz = c/d;
    float l = sqrt(dy*dy+dz*dz);
    
    int namepoly = polyh1.name;
    int numpoint = (unsigned)polyh1.vec3d1.size();
    int numline = (unsigned)polyh1.vec3d2.size();
    int i,j;
    float nn1,nn2,nn3;
    float mm1,mm2,mm3;
    int no1,no2;
    
    vector<point3d> point11;
    point11.clear();
    vector<point3d> point22;
    point22.clear();
    vector<line3d> line;
    line.clear();
    

    for(i=0; i<numpoint; i++)
    {
        nn1 = polyh1.vec3d1[i].x;
        nn2 = polyh1.vec3d1[i].y;
        nn3 = polyh1.vec3d1[i].z;
        mm1 = nn1;
        mm2 = nn2*dz/l-nn3*dy/l;
        mm3 = nn2*dy/l+nn3*dz/l;
        point3d aa={mm1,mm2,mm3};
        point11.push_back(aa);
    }
    
    for(i=0; i<numpoint; i++)
    {
        nn1 = point11[i].x;
        nn2 = point11[i].y;
        nn3 = point11[i].z;
        mm1 = nn1*l-nn3*dx;
        mm2 = nn2;
        mm3 = nn1*dx+nn3*l;
        point3d aa={mm1,mm2,mm3};
        point22.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh1.vec3d2[j].num1;
        no2 = polyh1.vec3d2[j].num2;
        point3d point111 = point22[no1 -1];
        point3d point222 = point22[no2 -1];
        line3d bb ={point111,point222,no1,no2};
        line.push_back(bb);
    }
    
    poly3d cc= {namepoly,point22,line};
    vector<point3d>().swap(point11);
    vector<point3d>().swap(point22);
    vector<line3d>().swap(line);
    return cc;
    
}


poly3d rotatetoorignal(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1)
{

    float a = x1-x0;
    float b = y1-y0;
    float c = z1-z0;
    float d = sqrt(a*a+b*b+c*c);
    float dx = a/d;
    float dy = b/d;
    float dz = c/d;
    float l = sqrt(dy*dy+dz*dz);
    
    int namepoly = polyh1.name;
    int numpoint = (unsigned)polyh1.vec3d1.size();
    int numline = (unsigned)polyh1.vec3d2.size();
    int i,j;
    float nn1,nn2,nn3;
    float mm1,mm2,mm3;
    int no1,no2;
    
    vector<point3d> point11;
    point11.clear();
    vector<point3d> point22;
    point22.clear();
    vector<line3d> line;
    line.clear();
    
    for(i=0; i<numpoint; i++)
    {
        nn1 = polyh1.vec3d1[i].x;
        nn2 = polyh1.vec3d1[i].y;
        nn3 = polyh1.vec3d1[i].z;
        mm1 = nn1*l+nn3*dx;
        mm2 = nn2;
        mm3 = -nn1*dx+nn3*l;
        point3d aa={mm1,mm2,mm3};
        point11.push_back(aa);
    }

    for(i=0; i<numpoint; i++)
    {
        nn1 = point11[i].x;
        nn2 = point11[i].y;
        nn3 = point11[i].z;
        mm1 = nn1;
        mm2 = nn2*dz/l+nn3*dy/l;
        mm3 = -nn2*dy/l+nn3*dz/l;
        point3d aa={mm1,mm2,mm3};
        point22.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyh1.vec3d2[j].num1;
        no2 = polyh1.vec3d2[j].num2;
        point3d point111 = point22[no1 -1];
        point3d point222 = point22[no2 -1];
        line3d bb ={point111,point222,no1,no2};
        line.push_back(bb);
    }
    
    poly3d cc= {namepoly,point22,line};
    vector<point3d>().swap(point11);
    vector<point3d>().swap(point22);
    vector<line3d>().swap(line);
    return cc;
    
}


poly3d rotate111(poly3d polyh1,float x0,float y0,float z0,float x1, float y1,float z1,float angle)
{
    
    poly3d polyh2= translate(polyh1, -x0, -y0, -z0);
    poly3d polyh3= rotatetoz(polyh2, x0, y0, z0, x1, y1, z1);
    poly3d polyh4= rotatez(polyh3, angle);
    poly3d polyh5= rotatetoorignal(polyh4, x0, y0, z0, x1, y1, z1);
    poly3d polyh6= translate(polyh5, x0, y0, z0);

    return polyh6;
}


info readinfo(poly2d polyg)
{
    float mm11,mm12;
    float mm21,mm22;
    
    int numpoint = (unsigned)polyg.vec2d1.size();
    int i;
    
    mm11=polyg.vec2d1[0].mm1;
    mm12=polyg.vec2d1[0].mm1;
    mm21=polyg.vec2d1[0].mm2;
    mm22=polyg.vec2d1[0].mm2;
    for(i=0;i<numpoint;i++)
    {
        if(polyg.vec2d1[i].mm1 <= mm11) mm11=polyg.vec2d1[i].mm1 ;
        if(polyg.vec2d1[i].mm1 >= mm12) mm12=polyg.vec2d1[i].mm1 ;
        if(polyg.vec2d1[i].mm2 <= mm21) mm21=polyg.vec2d1[i].mm2;
        if(polyg.vec2d1[i].mm2 >= mm22)
            mm22=polyg.vec2d1[i].mm2;
    }
    
    info aa={mm11,mm12,mm21,mm22};
    return aa;
}

void normalization(poly2d polyg, float mm1min,float mm1max, float mm2min, float mm2max,bool rotation,float x00, float y00, float x11, float y11)
{
    
    float dmm1,dmm2;
    dmm1=(mm1max-mm1min);
    dmm2=(mm2max-mm2min);
    
    float d;
    if(dmm1>=dmm2) d=dmm1;
    else  d=dmm2;
    
    int numpoint = (unsigned)polyg.vec2d1.size();
    int numline = (unsigned)polyg.vec2d2.size();
    
    int i,j;
    float mm1,mm2;
    int no1,no2;
    
    vector<point2d> point;
    point.clear();
    
    for(i=0; i<numpoint; i++)
    {
        mm1 = polyg.vec2d1[i].mm1;
        mm2 = polyg.vec2d1[i].mm2;
        mm1 -= mm1min;
        mm1 /= d;
        mm2 -= mm2min;
        mm2 /= d;
        
        point2d aa={mm1,mm2};
        point.push_back(aa);
    }
    
    for(j=0; j<numline ; j++)
    {
        no1 = polyg.vec2d2[j].num1;
        no2 = polyg.vec2d2[j].num2;
        drawline(point[no1 -1].mm1, point[no1 -1].mm2, point[no2 -1].mm1, point[no2 -1].mm2);
    }
    
    if (rotation)
    {
        x00 -= mm1min;
        x00 /= d;
        y00 -= mm2min;
        y00/= d;
        x11 -= mm1min;
        x11 /= d;
        y11 -= mm2min;
        y11 /= d;
        drawline3(x00, y00, x11, y11);
    }
    
    vector<point2d>().swap(point);
    
}


