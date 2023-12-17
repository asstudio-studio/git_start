#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#define  PI   3.141592653

#define Step  0.5

 /*-----Define a unit box--------*/
 /* Vertices of the box */
float  points[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} };

int    face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };

int    cube[6] = { 0, 1, 2, 3, 4, 5 };

float  colors[][3] = { {0.5, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.0, 0.5},
{0.5, 0.9, 0}, {0.0, 0.5, 0.9}, {0.5, 0, 0.9} };

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;


/*-Declare car position, orientation--*/
float  self_ang = -90.0, glob_ang = 0.0;
float  position[3] = { 8.0, 0.0, 0.0 };
float  bld_ang=10.0;    //螺旋槳角度
/*-----Define window size----*/
int width = 600, height = 600;


/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

/*-----Drawing stye
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int style = 0;
double loc_x = 0, loc_y = 4, loc_z = 0;
double face_ang = 0, limb_ang = 0, small_limb_ang = 0, joint_ang1 = 0, joint_ang2 = 0, body_ang = 0, fly_ang = 0;
int now_dire = 1;
int isKneeDown = 0, isDance = 0, isJump = 0, isFly = 0, rdx = 0, rdz = 0;
double eye_x = 45, eye_y = 70, eye_z = 55, cx = 25, cy = 0, cz = 25;
int engine = 0;

void draw_scene(void);
float norm2(float v[]);

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    /*---Create quadratic objects---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    if (disk == NULL) {
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }

    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
}
void draw_cube()
{
    int    i;

    for (i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}
void draw_rectangle(void)
{
    glBegin(GL_POLYGON);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 0);
    glEnd();
}
void draw_sphere(void)
{
    glColor3f(0.9, 0.1, 0.5);
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_LINE);
    }
    gluSphere(sphere, 1.0,   /* radius=1.5 */
        12,            /* composing of 24 slices*/
        12);           /* composing of 24 stacks */
}
void draw_case(void)
{
    glColor3f(200 / 255.0, 128 / 255.0, 64 / 255.0);
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_LINE);
    }
    gluSphere(sphere, 1.0,   /* radius=1.5 */
        24,            /* composing of 24 slices*/
        24);           /* composing of 24 stacks */
}
void draw_fly(void)
{
    glColor3f(255 / 255.0, 128 / 255.0, 64 / 255.0);
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_LINE);
    }
    gluSphere(sphere, 1,   /* radius=1.5 */
        36,            /* composing of 24 slices*/
        36);           /* composing of 24 stacks */
}
void draw_cylinder(double top, double bott, double hei)
{
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
    }
    /*--- Draw a cylinder ---*/
    gluCylinder(cylind, top, bott, /* radius of top and bottom circle */
        hei,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

}
void draw_floor()
{
    int i, j;
    //glFillModel(GL_FRONT, GL_LINE);
    for (i = 0; i < 50; i += 5)
        for (j = 0; j < 50; j += 5) {
            if ((i + j) % 10 == 0) glColor3f(0.3, 0.3, 0);
            else glColor3f(0.9, 0.9, 0.9);
            glBegin(GL_POLYGON);
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.0, j + 5);
            glVertex3f(i + 5, 0.0, j + 5);
            glVertex3f(i + 5, 0.0, j);
            glEnd();
        }
}
void draw_axes()
{

    /*----Draw a white sphere to represent the original-----*/
    glColor3f(0.9, 0.9, 0.9);

    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

    /*----Draw three axes in colors, yellow, meginta, and cyan--*/
    /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

    /* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
}

/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
  glEnd();
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (x) {
    case 4:       /* Perspective */

        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no transform is
           applied upon eye coordinate system
        */
        gluLookAt(eye[0], eye[1], eye[2],
            eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
            u[1][0], u[1][1], u[1][2]);
        break;

    case 1:       /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case 2:       /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        break;
    case 3:
        gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
double z_near = 1.5;
double z_far = 50.0;
double fovy = 90.0;
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4) {
        gluPerspective(fovy, (double)width / (double)height, z_near, z_far);
    }
    else {
        if (width > height)
            glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
                40.0 * (float)height / (float)width,
                -0.0, 100.0);
        else
            glOrtho(-40.0 * (float)width / (float)height,
                40.0 * (float)width / (float)height, -40.0, 40.0,
                -0.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}


/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
int is_draw_volume = 0;
void draw_view()
{
    int    i;

    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix();
    double asp = (double)width / (double)height;
    double h = 2 * tan(fovy / 2.0 * PI / 180.0) * z_far;
    double w = asp * h;
    double h_n= 2 * tan(fovy / 2.0 * PI / 180.0) * z_near;
    double w_n = asp * h_n;
    /*view volume*/
    if (is_draw_volume) {
        glColor3f(255 / 255.0, 128 / 255.0, 64 / 255.0);
        double xx, yy, zz;//用來存初始不經任何旋轉的座標
        double x1, y1, z1;//用來存對y軸旋轉後的座標
        double x2, y2, z2;//用來存對x軸旋轉後的座標
        double x3, y3, z3;//用來存對z軸旋轉後的座標
        //用來存底部4個頂點xyz值的容器
        double p1x, p1y, p1z;
        double p2x, p2y, p2z;
        double p3x, p3y, p3z;
        double p4x, p4y, p4z;
        //----------------------------
        glBegin(GL_LINES);
        xx = eye[0] + w / 2.0; yy = eye[1] + h / 2.0, zz = eye[2] - z_far;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p1x = x3; p1y = y3; p1z = z3;
        glVertex3f(eye[0], eye[1], eye[2]);
        glVertex3f(x3, y3, z3);
        glEnd();
        glBegin(GL_LINES);
        xx = eye[0] - w / 2.0; yy = eye[1] + h / 2.0; zz = eye[2] - z_far;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p2x = x3; p2y = y3; p2z = z3;
        glVertex3f(eye[0], eye[1], eye[2]);
        glVertex3f(x3, y3, z3);
        glEnd();
        glBegin(GL_LINES);
        xx = eye[0] + w / 2.0; yy = eye[1] - h / 2.0; zz = eye[2] - z_far;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p3x = x3; p3y = y3; p3z = z3;
        glVertex3f(eye[0], eye[1], eye[2]);
        glVertex3f(x3, y3, z3);
        glEnd();
        glBegin(GL_LINES);
        xx = eye[0] - w / 2.0; yy = eye[1] - h / 2.0; zz = eye[2] - z_far;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p4x = x3; p4y = y3; p4z = z3;
        glVertex3f(eye[0], eye[1], eye[2]);
        glVertex3f(x3, y3, z3);
        glEnd();
        /*far_square*/
        glBegin(GL_LINES);
        glVertex3f(p1x, p1y, p1z);
        glVertex3f(p2x, p2y, p2z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p2x, p2y, p2z);
        glVertex3f(p4x, p4y, p4z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p4x, p4y, p4z);
        glVertex3f(p3x, p3y, p3z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p1x, p1y, p1z);
        glVertex3f(p3x, p3y, p3z);
        glEnd();
        /*near_square*//*
        xx = eye[0] + w_n / 2.0; yy = eye[1] + h_n / 2.0, zz = eye[2] - z_near;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p1x = x3; p1y = y3; p1z = z3;
        xx = eye[0] - w_n / 2.0; yy = eye[1] + h_n / 2.0; zz = eye[2] - z_near;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p2x = x3; p2y = y3; p2z = z3;
        xx = eye[0] + w_n / 2.0; yy = eye[1] - h_n / 2.0; zz = eye[2] - z_near;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p3x = x3; p3y = y3; p3z = z3;
        xx = eye[0] - w_n / 2.0; yy = eye[1] - h_n / 2.0; zz = eye[2] - z_near;
        x1 = xx * cos(eyeAngy * PI / 180.0) + zz * sin(eyeAngy * PI / 180.0); y1 = yy; z1 = zz * cos(eyeAngy * PI / 180.0) - xx * sin(eyeAngy * PI / 180.0);//對y軸旋轉
        x2 = x1; y2 = y1 * cos(eyeAngx * PI / 180.0) - z1 * sin(eyeAngx * PI / 180.0); z2 = y1 * sin(eyeAngx * PI / 180.0) + z1 * cos(eyeAngx * PI / 180.0);//對x軸旋轉
        x3 = x2 * cos(eyeAngz * PI / 180.0) - y2 * sin(eyeAngz * PI / 180.0); y3 = x2 * sin(eyeAngz * PI / 180.0) + y2 * cos(eyeAngz * PI / 180.0); z3 = z2;//對z軸旋轉
        p4x = x3; p4y = y3; p4z = z3;
        glBegin(GL_LINES);
        glVertex3f(p1x, p1y, p1z);
        glVertex3f(p2x, p2y, p2z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p3x, p3y, p3z);
        glVertex3f(p4x, p4y, p4z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p4x, p4y, p4z);
        glVertex3f(p2x, p2y, p2z);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(p1x, p1y, p1z);
        glVertex3f(p3x, p3y, p3z);
        glEnd();*/
    }
    /**/

    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();
    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
}


/*--------------------------------------------------------
 * Display callback procedure to draw the scene
 */
void display()
{
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (style) {
    case 0:
        make_view(4);
        make_projection(4);
        glViewport(width / 2, 0, width / 2, height / 2);
        draw_scene();

        make_view(1);
        make_projection(1);
        glViewport(0, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(1);
        draw_view();

        make_view(2);
        glViewport(width / 2, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(2);
        draw_view();

        make_view(3);
        glViewport(0, 0, width / 2, height / 2);
        draw_scene();
        make_view(3);
        draw_view();
        draw_view();
        break;
    case 1:
        glViewport(0, 0, width, height);
        make_view(1);
        make_projection(1);
        draw_scene();
        break;
    case 2:
        glViewport(0, 0, width, height);
        make_view(2);
        make_projection(2);
        draw_scene();
        break;
    case 3:
        glViewport(0, 0, width, height);
        make_view(3);
        make_projection(3);
        draw_scene();
        break;
    case 4:
        glViewport(0, 0, width, height);
        make_view(4);
        make_projection(4);
        draw_scene();
        break;
    }
    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
    return;

}

/*-------------------------------------------------------
 * This procedure draw the car
 */
void draw_scene()
{
    draw_floor();
    draw_axes();
    //return;
    /*-------Draw the obstacle ----*/
    glPushMatrix();
    glTranslatef(20, 0, 20);
    glScalef(10, 35, 10);
    draw_cube();
    glPopMatrix();
    glTranslatef(0, 0, 0);

    glTranslatef(position[0], position[1]+5.5, position[2]); 
    glRotatef(face_ang, 0.0, 1.0, 0.0);

    glPushMatrix();              /* Save M1 coord. sys */
    glScalef(17.0, 10.0, 12.0);    /* Scale up the axes */
    draw_cube();
    glPopMatrix();               /* Get M1 back */

    /*-------Draw the front wheels -----*/
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();              /* Save M1 coord. sys */
    glTranslatef(3.0, -2.0, 3.0); /* Go to left wheel position */
    glutSolidTorus(1.0,  /* inner radius */
                   2.0,  /* outer radius */
                   24,   /* divided into 18 segments */
                   12);  /* 12 rings */
    glPopMatrix();

    glPushMatrix();              /* Save M1 coord. sys */
    glTranslatef(3.0, -2.0, 9.0);/* Go to right wheel position */
    glutSolidTorus(1.0,  /* inner radius */
                   2.0,  /* outer radius */
                   24,   /* divided into 18 segments */
                   12);  /* 12 rings */
    glPopMatrix();

    /*------Draw back wheels ----*/
    glColor3f(1.0, 0.4, 0.0);
    glPushMatrix();              /* Save M1 coord. sys */
    glTranslatef(14.0, -2.0, 3.0); /* Go to left wheel position */
    glutSolidTorus(1.0,  /* inner radius */
                   2.0,  /* outer radius */
                   24,   /* divided into 18 segments */
                   12);  /* 12 rings */
    glPopMatrix();

    glPushMatrix();              /* Save M1 coord. sys */
    glTranslatef(14.0, -2.0, 9.0);/* Go to right wheel position */
    glutSolidTorus(1.0,  /* inner radius */
                   2.0,  /* outer radius */
                   24,   /* divided into 18 segments */
                   12);  /* 12 rings */
    glPopMatrix();

    
    /*---- Draw forearm of the windmill ---*/
    if(cylind==NULL){               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }   
    if(sphere==NULL){
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }
    glPushMatrix();
    glTranslatef(8.5, 10.0, 6.0);

    glRotatef(-90,1.0,0.0,0.0);
    glColor3f(0.68, 0.68, 0.68);
    gluCylinder(cylind, 1.0, 1.0, 4.0, 12, 3);

    glTranslatef(0.0, 0.0, 5.0); /* goto end of forearm, M4 coord. sys. */
    glColor3f(0.2, 0.2, 0.95);
    gluSphere(sphere, 1.5,   /* radius=1.5 */
            12,            /* composing of 12 slices*/
            12);           /* composing of 12 stacks */
    /*------Draw three blades ---*/
    glColor3f(1.0, 1.0, 1.0);
    
    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    glRotatef(5, 1.0, 0.0, 0.0);
    glScalef(1.1,1.2,1.5);
    draw_blade();/* draw the first blade */
    
    glRotatef(90.0, 0.0, 0.0, 1.0);    /* rotate by 90 degree, M6 coord. sys */
    glRotatef(5, 1.0, 0.0, 0.0);
    glScalef(1.1,1.2,1.5);
    draw_blade();/* draw 2nd blade */
    
    glRotatef(90.0, 0.0, 0.0, 1.0);    /* rotate by 90 degree,M7 coord. sys */
    glRotatef(5, 1.0, 0.0, 0.0);
    glScalef(1.1,1.2,1.5);
    draw_blade();/* draw 3rd blade */

    glRotatef(90.0, 0.0, 0.0, 1.0);    /* rotate by 90 degree,M7 coord. sys */
    glRotatef(5, 1.0, 0.0, 0.0);
    glScalef(1.1,1.2,1.5);
    draw_blade();/* draw 4rd blade */
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17.0, 3.0, 3.0);
    glScalef(10,3,3);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(24.0, 3.0, 6.0);

    glColor3f(0.2, 0.2, 0.95);
    gluSphere(sphere, 0.75,   /* radius=1.5 */
            12,            /* composing of 12 slices*/
            12);           /* composing of 12 stacks */
    /*------Draw three blades ---*/
    glColor3f(0.8, 0.0, 1.0);
    
    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    glScalef(0.8,0.5,1);
    draw_blade();/* draw the first blade */
    
    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
    glScalef(0.8,0.5,1);
    draw_blade();/* draw 2nd blade */
    
    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
    glScalef(0.8,0.5,1);
    draw_blade();/* draw 3rd blade */
    glPopMatrix();

    /*---- Draw other parts. ---*/
    return;
}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (double)w / (double)h, z_near, z_far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int wave_mode = 1;
void wave(double level, double limit) {
    if (limb_ang >= limit)wave_mode = 0;
    else if (limb_ang <= limit * -1)wave_mode = 1;
    if (wave_mode == 1)limb_ang += level;
    else limb_ang -= level;
}
int little_mode = 0;
void little_wave(double level, double limit_up, double limit_down) {
    if (small_limb_ang >= limit_up)little_mode = 0;
    else if (small_limb_ang <= limit_down)little_mode = 1;
    if (little_mode == 1)small_limb_ang += level;
    else small_limb_ang -= level;
}
int body_mode = 1;
void body_wave(double level) {
    if (body_ang >= 30)body_mode = 0;
    else if (body_ang <= -30)body_mode = 1;
    if (body_mode == 1)body_ang += level;
    else body_ang -= level;
}
int fly_mode = 1;
void fly_wave(double level) {
    if (fly_ang >= 70)fly_mode = 0;
    else if (fly_ang <= 30)fly_mode = 1;
    if (fly_mode == 1)fly_ang += level;
    else fly_ang -= level;
}
int isRun = 0;
double speed = 0.5;
int notOb() {
    if (loc_x >= 19 && loc_x <= 31 && loc_z >= 19 && loc_z <= 31 && loc_y <= 40) {
        return 0;
    }
    return 1;
}
void goStraight() {
    if (face_ang >= 0 && face_ang <= 180) {
        double a = sin(face_ang * PI / 180) * speed;
        if (a < 0)a *= -1;
        if (loc_x < 49 && notOb())loc_x += a;
        if (!notOb())
            while (loc_x >= 19)loc_x -= a;
    }
    else {
        double a = sin(face_ang * PI / 180) * speed;
        if (a < 0)a *= -1;
        if (loc_x > 0 && notOb()) loc_x -= a;
        if (!notOb())
            while (loc_x <= 31)loc_x += a;
    }
    if ((face_ang >= 0 && face_ang <= 90) || (face_ang >= 270 && face_ang <= 360)) {
        double a = cos(face_ang * PI / 180) * speed;
        if (a < 0)a *= -1;
        if (loc_z < 49 && notOb())loc_z += a;
        if (!notOb())
            while (loc_z >= 19)loc_z -= a;
    }
    else {
        double a = cos(face_ang * PI / 180) * speed;
        if (a < 0)a *= -1;
        if (loc_z > 0 && notOb()) loc_z -= a;
        if (!notOb())
            while (loc_z <= 31)loc_z += a;
    }
    //printf("%f %f", loc_x, loc_y);
}
int isSpaceWalk = 0, walk = 1;;

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int ix, int iy)
{
    int    i;
    float  x[3], y[3], z[3];

    /*--- Control the car. ----*/
    if (key == 'Q' || key == 'q') exit(0);
    if (key == 'a') {// 往左走
        position[2] += Step*cos(face_ang*PI/180.0);
        position[1] -= Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'w') {// 往後走
        position[0] -= Step*cos(face_ang*PI/180.0);
        position[2] += Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'd') {// 往右走
        position[2] -= Step*cos(face_ang*PI/180.0);
        position[1] += Step*sin(face_ang*PI/180.0);
    }
    else if (key == 's') {// 往前走
        position[0] += Step*cos(face_ang*PI/180.0);
        position[2] -= Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'r') {// 順時鐘轉
        face_ang -= 3;
        if (face_ang < 0)face_ang += 360;
    }
    else if (key == 'l') {// 逆時鐘轉
        face_ang += 3;
        if (face_ang > 360)face_ang -= 360;
    }
    else if (key == '+') {// add height when fly
        position[1] += Step*cos(face_ang*PI/180.0);
        position[2] -= Step*sin(face_ang*PI/180.0);
        engine+=1;
    }
    else if (key == '-') {// subn height when fly
        if(engine == 0){
            return;
        }
        else{
            position[1] -= Step*cos(face_ang*PI/180.0);
            position[2] += Step*sin(face_ang*PI/180.0);
            engine-=1;
        }
    }
    else if (key == 'E') {// 往右前走
        face_ang -= 3;
        if (face_ang < 0)face_ang += 360;
        position[0] -= Step*cos(face_ang*PI/180.0);
        position[2] += Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'D') {// 往右後走
        face_ang += 3;
        if (face_ang > 360)face_ang -= 360;
        position[0] += Step*cos(face_ang*PI/180.0);
        position[2] -= Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'W') {// 往左前走
        face_ang += 3;
        if (face_ang > 360)face_ang -= 360;
        position[0] -= Step*cos(face_ang*PI/180.0);
        position[2] += Step*sin(face_ang*PI/180.0);
    }
    else if (key == 'S') {// 往左後走
        face_ang -= 3;
        if (face_ang < 0)face_ang += 360;
        position[0] += Step*cos(face_ang*PI/180.0);
        position[2] -= Step*sin(face_ang*PI/180.0);
    }
        /*------transform the EYE coordinate system ------*/
    else if (key == 'y') {
        eyeDy += 0.5;       /* move up */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
    }
    else if (key == 'u') {
        eyeDy += -0.5;       /* move down */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
    }
    else if (key == 'o') {
        eyeDx += -0.5;       /* move left */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
    }
    else if (key == 'p') {
        eyeDx += 0.5;        /* move right */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
    }
    else if (key == 'x') {
        eyeDz += 0.5;        /* go near */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[2][i];
    }
    else if (key == 'c') {
        eyeDz += -0.5;       /* go far */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[2][i];
    }
    else if (key == 'j') {             /* pitching */
        eyeAngx += 5.0;
        if (eyeAngx > 360.0) eyeAngx -= 360.0;
        y[0] = u[1][0] * cv - u[2][0] * sv;
        y[1] = u[1][1] * cv - u[2][1] * sv;
        y[2] = u[1][2] * cv - u[2][2] * sv;

        z[0] = u[2][0] * cv + u[1][0] * sv;
        z[1] = u[2][1] * cv + u[1][1] * sv;
        z[2] = u[2][2] * cv + u[1][2] * sv;

        for (i = 0; i < 3; i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'k') {
        eyeAngx += -5.0;
        if (eyeAngx < 0.0) eyeAngx += 360.0;
        y[0] = u[1][0] * cv + u[2][0] * sv;
        y[1] = u[1][1] * cv + u[2][1] * sv;
        y[2] = u[1][2] * cv + u[2][2] * sv;

        z[0] = u[2][0] * cv - u[1][0] * sv;
        z[1] = u[2][1] * cv - u[1][1] * sv;
        z[2] = u[2][2] * cv - u[1][2] * sv;

        for (i = 0; i < 3; i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'm') {            /* heading */
        eyeAngy += 5.0;
        if (eyeAngy > 360.0) eyeAngy -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[2][i];
            z[i] = sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'M') {
        eyeAngy += -5.0;
        if (eyeAngy < 0.0) eyeAngy += 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] + sv * u[2][i];
            z[i] = -sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'z') {            /* rolling */
        eyeAngz += 5.0;
        if (eyeAngz > 360.0) eyeAngz -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[1][i];
            y[i] = sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    else if (key == 'Z') {
        eyeAngz += -5.0;
        if (eyeAngz < 0.0) eyeAngz += 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] + sv * u[1][i];
            y[i] = -sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    else if (key == 'P') {/* view volume pyramid */
        if (!is_draw_volume)is_draw_volume = 1;
        else is_draw_volume = 0;
    }
    else if (key == '0') {
        style = 0;
    }
    else if (key == '1') {
        style = 1;
    }
    else if (key == '2') {
        style = 2;
    }
    else if (key == '3') {
        style = 3;
    }
    else if (key == '4') {
        style = 4;
    }
    else if (key == '>') {/* zoom in */
        if (fovy > 1)fovy--;
    }
    else if (key == '<') {
        if (fovy < 90)fovy++;/* zoom out */
    }
    display();
}
int jump_mode = 1;
void idle_func() {
    if (engine > 0){
        bld_ang += 5.0;
        if(bld_ang>360.0){
            bld_ang -= 360.0;
        }
    }
    display(); /* show the scene again */
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("view");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    /*  glutIdleFunc(display); */
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_quit);
    glutIdleFunc(idle_func);
    glutMainLoop();
}


/*------------------------------------------
 * Procedure to compute norm of vector v[]
 */
float norm2(float v[])
{
    double  sum;
    float   temp;

    sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    temp = (float)sqrt(sum);
    return temp;
}
