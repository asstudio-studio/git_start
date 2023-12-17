#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#define  PI   3.141592653

#define Step  0.5

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
                      {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
                      {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0},
{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} };

/*----Define faces, these num. are indices of vertices---*/
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

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

/*----Define normals of faces ----*/
float  normal[][4] = { {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
              {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0} };

/*----Define material properties for cube -----*/
float  mat_ambient[] = { 0.19125, 0.0735, 0.0225, 1.0 };
float  mat_diffuse[] = { 0.7038, 0.27048, 0.0828, 1.0 };
float  mat_specular[] = { 0.256777, 0.137622, 0.086014, 1.0 };
float  mat_shininess = 0.1;
float  sph_ambient[] = { 0.24725, 0.1995, 0.0745, 1.0 };
float  sph_diffuse[] = { 0.75164, 0.60648, 0.22648, 1.0 };
float  sph_specular[] = { 0.628281, 0.555802, 0.366065, 1.0 };
float  sph_shininess = 0.4;
float  cyl_ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };
float  cyl_diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
float  cyl_specular[] = { 0.508273, 0.508273, 0.508273, 1.0 };
float  cyl_shininess = 0.4;
float  rock_ambient[] = { 0.05375, 0.05, 0.06625, 1.0 };
float  rock_diffuse[] = { 0.18275, 0.17, 0.22525, 1.0 };
float  rock_specular[] = { 0.332741, 0.328634, 0.346435, 1.0 };
float  rock_shininess = 0.3;
float  pla_ambient[] = { 0.0, 0.1, 0.06, 1.0 };
float  pla_diffuse[] = { 0.0, 0.50980392, 0.50980392, 1.0 };
float  pla_specular[] = { 0.50196078, 0.50196078, 0.50196078, 1.0 };
float  pla_shininess = 0.25;
float  flr_diffuse[] = { 0.60, 0.60, 0.60, 1.0 };
float  flr_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
float  flr_specular[] = { 0.0, 0.0, 0.0, 1.0 };

/*----Define light source properties -------*/
float  lit_position[] = { 10.0, 14.0, 0.0, 1.0 };
float  lit_direction[] = { -1.0, -1.0, 1.0, 0.0 };
float  lit_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
float  lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float  lit_cutoff = 60.0;
float  lit_exponent = 8.0;
float  lit2_position[] = { 1.5, 5, 4.0, 1.0 };
float  lit2_direction[] = { 0.0, -1.0, 1.0, 0.0 };
float  lit2_diffuse[] = { 1, 1, 1, 1.0 };
float  lit2_specular[] = { 1, 1, 1, 1.0 };
float  lit2_cutoff = 60.0;
float  lit2_exponent = 8.0;
float  lit3_position[] = { 1.5, 8, 1.5, 1.0 };
float  lit3_direction[] = { 0.0, -1.0, 0.0, 0.0 };
float  lit3_diffuse[] = { 1, 1, 1, 1.0 };
float  lit3_specular[] = { 1, 1, 1, 1.0 };
float  lit3_cutoff = 60.0;
float  lit3_exponent = 8.0;
float  lit4_position[] = { 0.0, 30, 0.0, 1.0 };
float  lit4_direction[] = { 0.0, -1.0, 0.0, 0.0 };
float  lit4_diffuse[] = { 0.8, 0.5, 0.3, 1.0 };
float  lit4_specular[] = { 0.8, 0.5, 0.3, 1.0 };
float  lit4_cutoff = 60.0;
float  lit4_exponent = 8.0;

float  lit1_position[] = { -1, 10, 50, 0.0 };
float  lit1_pos[][4] = { {-10, 10, -10, 0.0},{-10, 10, 10, 0.0},{10, 10, 10, 0.0},{10, 10, -10, 0.0} };
float  lit1_diffuse[] = { 0.95, 0.95, 0.95, 1.0 };

float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

/*----Some global variables for transformation------*/
float  lit_angle = 0.0;
float lit4_angle = 0.0;
float  cube_angle = 0.0;
float  head = 0.0, roll = 0.0, Epitch = 0.0, Wpitch = 0.0;
float  up = 0.0, right = 0.0, zoom = 0.0;

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

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);  /* Enable depth buffer for shading computing */
    glEnable(GL_NORMALIZE);   /* Enable mornalization  */

    glEnable(GL_LIGHTING);    /* Enable lighting effects */
    glEnable(GL_LIGHT0);      /* Turn on light0 */
    glEnable(GL_LIGHT1);      /* Turn on light1 */
    glEnable(GL_LIGHT2);      /* Turn on light2 */
    glEnable(GL_LIGHT3);      /* Turn on light3 */
    glEnable(GL_LIGHT4);      /* Turn on light4 */
    /*-----Define light0 ---------*/
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

    /*-----Define light2 ---------*/
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    /*-----Define light3 ---------*/
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lit3_cutoff);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lit3_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, lit3_specular);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, lit3_exponent);
    /*-----Define light4 ---------*/
    glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lit4_cutoff);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lit4_diffuse);
    glLightfv(GL_LIGHT4, GL_SPECULAR, lit4_specular);
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, lit4_exponent);

    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);


    /*-----Define some global lighting status -----*/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
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
    float  range;

    /*-----Define some material properties shared by every one ---*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
        range = 1.0;
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}
void draw_ob()
{
    int    i;
    float  range;

    /*-----Define some material properties shared by every one ---*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, pla_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pla_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pla_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, pla_shininess);

    for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
        range = 1.0;
        glNormal3fv(normal[i]);
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
    glMaterialfv(GL_FRONT, GL_AMBIENT, sph_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sph_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sph_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, sph_shininess);
    glColor3f(0.9, 0.1, 0.5);
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_LINE);
    }
    gluSphere(sphere, 1.0,   /* radius=1.5 */
        12,            /* composing of 24 slices*/
        12);           /* composing of 24 stacks */
}
void draw_rock() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, rock_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rock_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rock_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, rock_shininess);
    glColor3f(0.9, 0.1, 0.5);
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        if (sphere == NULL) {
            sphere = gluNewQuadric();
            gluQuadricDrawStyle(sphere, GLU_LINE);
        }
        gluSphere(sphere, 0.5,   /* radius=1.5 */
            6,            /* composing of 24 slices*/
            6);           /* composing of 24 stacks */
        glPopMatrix();
    }
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0, 0, i);
        if (sphere == NULL) {
            sphere = gluNewQuadric();
            gluQuadricDrawStyle(sphere, GLU_LINE);
        }
        gluSphere(sphere, 0.5,   /* radius=1.5 */
            6,            /* composing of 24 slices*/
            6);           /* composing of 24 stacks */
        glPopMatrix();
    }
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
        glTranslatef(50, 0, i);
        if (sphere == NULL) {
            sphere = gluNewQuadric();
            gluQuadricDrawStyle(sphere, GLU_LINE);
        }
        gluSphere(sphere, 0.5,   /* radius=1.5 */
            6,            /* composing of 24 slices*/
            6);           /* composing of 24 stacks */
        glPopMatrix();
    }
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
        glTranslatef(i, 0, 50);
        if (sphere == NULL) {
            sphere = gluNewQuadric();
            gluQuadricDrawStyle(sphere, GLU_LINE);
        }
        gluSphere(sphere, 0.5,   /* radius=1.5 */
            6,            /* composing of 24 slices*/
            6);           /* composing of 24 stacks */
        glPopMatrix();
    }
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
    glMaterialfv(GL_FRONT, GL_AMBIENT, cyl_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cyl_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cyl_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, cyl_shininess);
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
    glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse);  /*diffuse color */
    glMaterialfv(GL_FRONT, GL_SPECULAR, flr_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
    glNormal3f(0.0, 1.0, 0.0);
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
    double h_n = 2 * tan(fovy / 2.0 * PI / 180.0) * z_near;
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
int sun_loc = 0;
float rob_lit[] = { 1.0,1.0,1.0,0.0 };
float spot_lit[] = { 1.0,1.0,1.0,0.0 };
float pt_lit[] = { 1.0,0.0,0.0,0.0 };
float pt2_lit[] = { 0.8,0.5,0.3,0.0 };
int islight = 1;
float lit2_ang=0.0;
float up_exp = 1.1, down_exp = 0.9;
void draw_scene()
{
    glPushMatrix();
    glLoadIdentity();
    /*----position light1 (fixed) in eye coord sys ---*/
    glLightfv(GL_LIGHT1, GL_POSITION, lit1_pos[sun_loc]);  /*fixed position in eye space---*/
    /*---draw the light source ---*/
    glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glColor3f(0.80, 0.80, 0.0);
    glutSolidSphere(0.3, 12, 12);
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    /*----Define Viewing Matrix-----*/
    gluLookAt(8.0, 8.0, 20.0, 8.0, 4.0, 4.0, 0.0, 1.0, 0.0);

    /*-------Rotate light position----*/
    glPushMatrix();
    glTranslatef(8.0, 0.0, 8.0);          /* Move to [8, 0, 8] */
    glRotatef(lit_angle, 0.0, 1.0, 0.0);  /* Rotate about y-axis */
    /*-------Draw light source as a red cube----*/
    glPushMatrix();
    glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
    glColor3f(pt_lit[0], pt_lit[1], pt_lit[2]);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    if (islight)glEnable(GL_LIGHTING);      /* Turn on lighting */
    else glDisable(GL_LIGHTING);
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    glPopMatrix();
    //------------------------------------------------------
    /*-------Rotate light position----*/
    glPushMatrix();
    glTranslatef(25, 0, 25);
    glRotatef(lit2_ang, 1, 0, 0);
    //printf("%f", lit2_ang);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glTranslatef(lit4_position[0], lit4_position[1], lit4_position[2]);
    glColor3f(pt2_lit[0], pt2_lit[1], pt2_lit[2]);
    glutWireSphere(5, 8, 8);
    glPopMatrix();
    if (islight)glEnable(GL_LIGHTING);      /* Turn on lighting */
    else glDisable(GL_LIGHTING);
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT4, GL_POSITION, lit4_position);
    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lit4_direction);
    glPopMatrix();
    //----------------------------------------------------------------
    draw_floor();
    //draw_axes();
    //return;
    /*-------Draw the obstacle ----*/
    glPushMatrix();
    glTranslatef(20, 0, 20);
    glScalef(10, 35, 10);
    draw_ob();
    glPopMatrix();
    glTranslatef(0, 0, 0);
    //-------------------
    draw_rock();
    //-------------------
    /*------- Draw the robot ----------*/
    //body
    glTranslatef(loc_x, loc_y, loc_z);
    glRotatef(face_ang, 0, 1, 0);
    glRotatef(body_ang, 1, 0, 0);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glTranslatef(lit2_position[0], lit2_position[1], lit2_position[2]);
    glColor3f(rob_lit[0], rob_lit[1], rob_lit[2]);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    if (islight)glEnable(GL_LIGHTING);      /* Turn on lighting */
    else glDisable(GL_LIGHTING);
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);
    //glPopMatrix();
    //----------------------
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glTranslatef(lit3_position[0], lit3_position[1], lit3_position[2]);
    glColor3f(spot_lit[0], spot_lit[1], spot_lit[2]);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    if (islight)glEnable(GL_LIGHTING);      /* Turn on lighting */
    else glDisable(GL_LIGHTING);
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT3, GL_POSITION, lit3_position);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lit3_direction);
    //----------------------
    glPushMatrix();
    glScalef(3.0, 4.0, 3.0);
    draw_cube();
    glPopMatrix();
    //arm1
    glTranslatef(-0.5, 3.0, 1.5);
    glPushMatrix();
    if (isKneeDown || isJump)glRotatef(limb_ang * -1, 1, 0, 0);
    else glRotatef(limb_ang, 1, 0, 0);
    glPushMatrix();
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.3, 0.9, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glTranslatef(0.0, -2, 0);
    glRotatef(small_limb_ang, 1, 0, 0);
    glPushMatrix();
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.3, 0.9, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    //arm2
    glTranslatef(4, 0, 0);
    glPushMatrix();
    glRotatef(limb_ang * -1, 1, 0, 0);
    glPushMatrix();
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.3, 0.9, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glTranslatef(0.0, -2, 0);
    glRotatef(small_limb_ang, 1, 0, 0);
    glPushMatrix();
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.3, 0.9, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    //leg1
    glTranslatef(-3, -3, 0);
    glPushMatrix();
    glRotatef(limb_ang * -1, 1, 0, 0);
    glPushMatrix();
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.9, 0.3, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glTranslatef(0.0, -2, 0);
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(small_limb_ang * -1, 1, 0, 0);
    glColor3f(0.9, 0.3, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glPopMatrix();

    //leg2
    glTranslatef(2, 0, 0);
    glPushMatrix();
    //if (!isDance) {
    if (isKneeDown || isJump)glRotatef(limb_ang * -1, 1, 0, 0);
    //else if(isDance)glRotatef(limb_ang * -1, 1, 0, 0);
    else glRotatef(limb_ang, 1, 0, 0);
    //}
    glPushMatrix();
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glColor3f(0.9, 0.3, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glTranslatef(0.0, -2, 0);
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    draw_sphere();
    glPopMatrix();
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(small_limb_ang * -1, 1, 0, 0);
    glColor3f(0.9, 0.3, 0.9);
    draw_cylinder(0.5, 0.5, 2);
    glPopMatrix();
    glPopMatrix();
    //head
    glTranslatef(-1.5, 4, 0);
    glPushMatrix();
    glRotatef(body_ang, 0, 1, 0);
    draw_cube();
    glPopMatrix();

    if (isFly) {
        glPushMatrix();
        glTranslatef(1, -2, -1.5);
        glPushMatrix();
        glRotatef(fly_ang * -1, 0, 1, 0);
        glTranslatef(0, 0, -2);
        glPushMatrix();
        glScalef(0.2, 5, 1.5);
        draw_fly();
        glPopMatrix();
        glPopMatrix();
        glTranslatef(-1, 0, 0);
        //draw_sphere();
        glPushMatrix();
        glRotatef(fly_ang, 0, 1, 0);
        glTranslatef(0, 0, -2);
        glPushMatrix();
        glScalef(0.2, 5, 1.5);
        draw_fly();
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
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
int isLight0 = 1, isLight1 = 1, isLight2 = 1, isLight3 = 1, isSun = 0;
void my_quit(unsigned char key, int ix, int iy)
{
    int    i;
    float  x[3], y[3], z[3];

    /*--- Control the car. ----*/
    if (key == 'Q' || key == 'q') exit(0);
    if (key == 'a') {// 往左走
        if (isKneeDown || isDance)return;
        double tmp_ang = face_ang;
        face_ang += 270;
        if (face_ang > 360)face_ang -= 360;
        goStraight();
        face_ang = tmp_ang;
        if (isRun)wave(15, 90);
        else wave(5, 45);
        if (isRun) little_wave(5, 0, -90);
        else little_wave(5, 0, -45);
    }
    else if (key == 'w') {// 往後走
        if (isKneeDown || isDance)return;
        if (face_ang >= 0 && face_ang <= 180) {
            double a = sin(face_ang * PI / 180) * speed;
            if (a < 0)a *= -1;
            if (loc_x > 0 && notOb()) loc_x -= a;
            if (!notOb())
                while (loc_x <= 31)loc_x += a;
        }
        else {
            double a = sin(face_ang * PI / 180) * speed;
            if (a < 0)a *= -1;
            if (loc_x < 49 && notOb()) loc_x += a;
            if (!notOb())
                while (loc_x >= 19)loc_x -= a;
        }
        if ((face_ang >= 0 && face_ang <= 90) || (face_ang >= 270 && face_ang <= 360)) {
            double a = cos(face_ang * PI / 180) * speed;
            if (a < 0)a *= -1;
            if (loc_z > 0 && notOb()) loc_z -= a;
            if (!notOb())
                while (loc_z <= 31)loc_z += a;
        }
        else {
            double a = cos(face_ang * PI / 180) * speed;
            //printf("%f\n", a);
            if (a < 0)a *= -1;
            if (loc_z < 49 && notOb()) loc_z += a;
            if (!notOb())
                while (loc_z >= 19)loc_z -= a;
        }
        //printf("\n%f %f\n", loc_x, loc_z);
        if (isRun)wave(15, 90);
        else wave(5, 45);
        if (isRun) little_wave(5, 0, -90);
        else little_wave(5, 0, -45);
    }
    else if (key == 'd') {// 往右走
        if (isKneeDown || isDance)return;
        double tmp_ang = face_ang;
        face_ang += 90;
        if (face_ang > 360)face_ang -= 360;
        goStraight();
        face_ang = tmp_ang;
        if (isRun)wave(15, 90);
        else wave(5, 45);
        if (isRun) little_wave(5, 0, -90);
        else little_wave(5, 0, -45);
    }
    else if (key == 's') {// 往前走
        if (isKneeDown || isDance)return;
        goStraight();
        if (isRun)wave(15, 90);
        else wave(5, 45);
        if (isRun) little_wave(5, 0, -90);
        else little_wave(5, 0, -45);
    }
    else if (key == 'r') {// 順時鐘轉
        face_ang -= 3;
        if (face_ang < 0)face_ang += 360;
    }
    else if (key == 'l') {// 逆時鐘轉
        face_ang += 3;
        if (face_ang > 360)face_ang -= 360;
    }
    else if (key == 'n') {// 蹲下
        if (!isKneeDown) {
            loc_y -= 2;
            limb_ang = 90;
            small_limb_ang = -90;
            isKneeDown = 1;
        }
        else {
            loc_y += 2;
            limb_ang = 0;
            small_limb_ang = 0;
            isKneeDown = 0;
        }
    }
    else if (key == 'D') {// dance
        isDance = 1;
        //dance();
        //isDance = 0;
    }
    else if (key == 'f') {// fly
        if (isKneeDown || isJump)return;
        if (!isFly) {
            isFly = 1;
            loc_y = 10;
            body_ang = 30;
        }
        else {
            isFly = 0;
            loc_y = 4;
            body_ang = 0;
        }
    }
    else if (key == '+') {// add height when fly
        if (isFly) {
            if (loc_y < 49)loc_y++;
        }
    }
    else if (key == '-') {// subn height when fly
        if (isFly) {
            if (loc_y > 11 && notOb())loc_y--;
        }
    }
    else if (key == 'W') {// space walk
        if (isKneeDown || isDance)return;
        isSpaceWalk = 1;
    }
    else if (key == 'R') {// run
        isRun = 1;
    }
    else if (key == 'g') {// from walk to run
        if (walk) {
            walk = 0;
            speed = 3;
        }
        else {
            walk = 1;
            speed = 0.5;
        }
    }
    else if (key == 'S') {// stop from run & dance
        isRun = 0;
        isDance = 0;
        isSpaceWalk = 0;
        body_ang = 0;
        limb_ang = 0;
        small_limb_ang = 0;
    }
    else if (key == 32) {
        if (isKneeDown)return;
        loc_y -= 2;
        limb_ang = 90;
        small_limb_ang = -90;
        isKneeDown = 1;
        display();
        isKneeDown = 0;
        Sleep(300);
        loc_y = 2;
        limb_ang = 90;
        small_limb_ang = -90;
        display();
        loc_y = 4;
        limb_ang = 0;
        small_limb_ang = 0;
        isJump = 1;
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
    else if (key == 'b') {
        lit_angle += 5.0;
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
    else if (key == 'A') {
        if (isLight0) {
            isLight0 = 0;
            pt_lit[0] = 0;
            pt_lit[1] = 0;
            pt_lit[2] = 0;
            glDisable(GL_LIGHT0);
        }
        else {
            pt_lit[0] = lit_diffuse[0];
            pt_lit[1] = lit_diffuse[1];
            pt_lit[2] = lit_diffuse[2];
            isLight0 = 1;
            glEnable(GL_LIGHT0);
        }
    }
    else if (key == 'B') {
        if (isLight1) {
            isLight1 = 0;
            glDisable(GL_LIGHT1);
        }
        else {
            isLight1 = 1;
            glEnable(GL_LIGHT1);
        }
    }
    else if (key == 'X') {
        if (isLight2) {
            isLight2 = 0;
            rob_lit[0] = 0;
            rob_lit[1] = 0;
            rob_lit[2] = 0;
            glDisable(GL_LIGHT2);
        }
        else {
            rob_lit[0] = lit2_diffuse[0];
            rob_lit[1] = lit2_diffuse[1];
            rob_lit[2] = lit2_diffuse[2];
            isLight2 = 1;
            glEnable(GL_LIGHT2);
        }
    }
    else if (key == '=') {
    if (isLight3) {
        isLight3 = 0;
        spot_lit[0] = 0;
        spot_lit[1] = 0;
        spot_lit[2] = 0;
        glDisable(GL_LIGHT3);
    }
    else {
        spot_lit[0] = lit3_diffuse[0];
        spot_lit[1] = lit3_diffuse[1];
        spot_lit[2] = lit3_diffuse[2];
        isLight3 = 1;
        glEnable(GL_LIGHT3);
    }
    }
    else if (key == 'E') {
        if (lit2_cutoff < 90)lit2_cutoff++;
        printf("%f", lit2_cutoff);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    }
    else if (key == 'F') {
        if (lit2_cutoff > 15)lit2_cutoff--;
        printf("%f", lit2_cutoff);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    }
    else if (key == 'H') {
        if (lit_cutoff < 90)lit_cutoff++;
        printf("%f", lit_cutoff);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
    }
    else if (key == 'I') {
        if (lit_cutoff > 15)lit_cutoff--;
        printf("%f", lit_cutoff);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
    }
    else if (key == 'C') {
        glShadeModel(GL_SMOOTH);
    }
    else if (key == 'G') {
        glShadeModel(GL_FLAT);
    }
    //float  lit1_diffuse[] = { 0.95, 0.95, 0.95, 1.0 };
    else if (key == 'N') {// R
        lit1_diffuse[0] = 0.95;
        lit1_diffuse[1] = 0.01;
        lit1_diffuse[2] = 0.01;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    else if (key == 'O') {// G
        lit1_diffuse[0] = 0.01;
        lit1_diffuse[1] = 0.95;
        lit1_diffuse[2] = 0.01;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    else if (key == 'T') {// B
        lit1_diffuse[0] = 0.01;
        lit1_diffuse[1] = 0.01;
        lit1_diffuse[2] = 0.95;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    else if (key == 'U') {// W
        lit1_diffuse[0] = 0.95;
        lit1_diffuse[1] = 0.95;
        lit1_diffuse[2] = 0.95;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    else if (key == 'Y') {
        sun_loc = (sun_loc + 1) % 4;
        printf("%d", sun_loc);
        //glLightfv(GL_LIGHT1, GL_POSITION, lit1_pos[sun_loc]);
    }
    else if (key == 'K') {//rob_r
        lit2_diffuse[0] = 0.95;
        lit2_diffuse[1] = 0.05;
        lit2_diffuse[2] = 0.05;
        lit2_specular[0] = 0.95;
        lit2_specular[1] = 0.05;
        lit2_specular[2] = 0.05;
        rob_lit[0] = 0.95;
        rob_lit[1] = 0.05;
        rob_lit[2] = 0.05;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    }
    else if (key == 'L') {//rob_g
        lit2_diffuse[0] = 0.05;
        lit2_diffuse[1] = 0.95;
        lit2_diffuse[2] = 0.05;
        lit2_specular[0] = 0.05;
        lit2_specular[1] = 0.95;
        lit2_specular[2] = 0.05;
        rob_lit[0] = 0.05;
        rob_lit[1] = 0.95;
        rob_lit[2] = 0.05;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    }
    else if (key == 'V') {//rob_b
        lit2_diffuse[0] = 0.05;
        lit2_diffuse[1] = 0.05;
        lit2_diffuse[2] = 0.95;
        lit2_specular[0] = 0.05;
        lit2_specular[1] = 0.05;
        lit2_specular[2] = 0.95;
        rob_lit[0] = 0.05;
        rob_lit[1] = 0.05;
        rob_lit[2] = 0.95;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    }
    else if (key == 'J') {//rob_w
        lit2_diffuse[0] = 0.95;
        lit2_diffuse[1] = 0.95;
        lit2_diffuse[2] = 0.95;
        lit2_specular[0] = 0.95;
        lit2_specular[1] = 0.95;
        lit2_specular[2] = 0.95;
        rob_lit[0] = 0.95;
        rob_lit[1] = 0.95;
        rob_lit[2] = 0.95;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    }
    else if (key == 't') {//pt_r
        lit_diffuse[0] = 0.95;
        lit_diffuse[1] = 0.05;
        lit_diffuse[2] = 0.05;
        lit_specular[0] = 0.95;
        lit_specular[1] = 0.05;
        lit_specular[2] = 0.05;
        pt_lit[0] = 0.95;
        pt_lit[1] = 0.05;
        pt_lit[2] = 0.05;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    }
    else if (key == 'e') {//pt_g
        lit_diffuse[0] = 0.05;
        lit_diffuse[1] = 0.95;
        lit_diffuse[2] = 0.05;
        lit_specular[0] = 0.05;
        lit_specular[1] = 0.95;
        lit_specular[2] = 0.05;
        pt_lit[0] = 0.05;
        pt_lit[1] = 0.95;
        pt_lit[2] = 0.05;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    }
    else if (key == 'h') {//pt_b
        lit_diffuse[0] = 0.05;
        lit_diffuse[1] = 0.05;
        lit_diffuse[2] = 0.95;
        lit_specular[0] = 0.05;
        lit_specular[1] = 0.05;
        lit_specular[2] = 0.95;
        pt_lit[0] = 0.05;
        pt_lit[1] = 0.05;
        pt_lit[2] = 0.95;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    }
    else if (key == 'i') {//pt_w
        lit_diffuse[0] = 0.95;
        lit_diffuse[1] = 0.95;
        lit_diffuse[2] = 0.95;
        lit_specular[0] = 0.95;
        lit_specular[1] = 0.95;
        lit_specular[2] = 0.95;
        pt_lit[0] = 0.95;
        pt_lit[1] = 0.95;
        pt_lit[2] = 0.95;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    }
    else if (key == '5') {
        if (islight)islight = 0;
        else islight = 1;
    }
    else if (key == '6') {
        if (lit1_diffuse[0] * up_exp <= 1 && lit1_diffuse[1] * up_exp <= 1 && lit1_diffuse[2] * up_exp <= 1) {
            lit1_diffuse[0] *= up_exp;
            lit1_diffuse[1] *= up_exp;
            lit1_diffuse[2] *= up_exp;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
        }
    }
    else if (key == '7') {
        if (lit1_diffuse[0] * down_exp >= 0 && lit1_diffuse[1] * down_exp >= 0 && lit1_diffuse[2] * down_exp >= 0) {
            lit1_diffuse[0] *= down_exp;
            lit1_diffuse[1] *= down_exp;
            lit1_diffuse[2] *= down_exp;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
        }
    }
    else if (key == '8') {
        if (lit2_diffuse[0] * up_exp <= 1 && lit2_diffuse[1] * up_exp <= 1 && lit2_diffuse[2] * up_exp <= 1
            && lit2_specular[0] * up_exp <= 1 && lit2_specular[1] * up_exp <= 1 && lit2_specular[2] * up_exp <= 1) {
            lit2_diffuse[0] *= up_exp;
            lit2_diffuse[1] *= up_exp;
            lit2_diffuse[2] *= up_exp;
            lit2_specular[0] *= up_exp;
            lit2_specular[1] *= up_exp;
            lit2_specular[2] *= up_exp;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
        }
    }
    else if (key == '9') {
        if (lit2_diffuse[0] * down_exp >= 0 && lit2_diffuse[1] * down_exp >= 0 && lit2_diffuse[2] * down_exp >= 0
            && lit2_specular[0] * down_exp >= 0 && lit2_specular[1] * down_exp >= 0 && lit2_specular[2] * down_exp >= 0) {
            lit2_diffuse[0] *= down_exp;
            lit2_diffuse[1] *= down_exp;
            lit2_diffuse[2] *= down_exp;
            lit2_specular[0] *= down_exp;
            lit2_specular[1] *= down_exp;
            lit2_specular[2] *= down_exp;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
        }
    }
    else if (key == '(') {
        if (lit_diffuse[0] * up_exp <= 1 && lit_diffuse[1] * up_exp <= 1 && lit_diffuse[2] * up_exp <= 1
            && lit_specular[0] * up_exp <= 1 && lit_specular[1] * up_exp <= 1 && lit_specular[2] * up_exp <= 1) {
            lit_diffuse[0] *= up_exp;
            lit_diffuse[1] *= up_exp;
            lit_diffuse[2] *= up_exp;
            lit_specular[0] *= up_exp;
            lit_specular[1] *= up_exp;
            lit_specular[2] *= up_exp;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
        }
    }
    else if (key == ')') {
        if (lit_diffuse[0] * down_exp >= 0 && lit_diffuse[1] * down_exp >= 0 && lit_diffuse[2] * down_exp >= 0
            && lit_specular[0] * down_exp >= 0 && lit_specular[1] * down_exp >= 0 && lit_specular[2] * down_exp >= 0) {
            lit_diffuse[0] *= down_exp;
            lit_diffuse[1] *= down_exp;
            lit_diffuse[2] *= down_exp;
            lit_specular[0] *= down_exp;
            lit_specular[1] *= down_exp;
            lit_specular[2] *= down_exp;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
        }
    }
    else if (key == '.') {
        
        if (isSun) {
            isSun= 0;
            pt2_lit[0] = 0;
            pt2_lit[1] = 0;
            pt2_lit[2] = 0;
            glDisable(GL_LIGHT4);
        }
        else {
            pt2_lit[0] = lit4_diffuse[0];
            pt2_lit[1] = lit4_diffuse[1];
            pt2_lit[2] = lit4_diffuse[2];
            isSun = 1;
            glEnable(GL_LIGHT4);
        }
    }
    display();
}
int jump_mode = 1;
float col1=0.0, col2=0.0, col3 = 0.0;
void chanhe_spot_color() {
    if (col1 < 1)col1 += 0.001;
    else col1 = 0;
    if (col2 < 1)col2 += 0.005;
    else col2 = 0;
    if (col3 < 1)col3 += 0.007;
    else col3 = 0;
    lit3_diffuse[0] = col1;
    lit3_diffuse[1] = col2;
    lit3_diffuse[2] = col3;
    lit3_specular[0] = col1;
    lit3_specular[1] = col2;
    lit3_specular[2] = col3;
    spot_lit[0] = col1;
    spot_lit[1] = col2;
    spot_lit[2] = col3;
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lit3_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, lit3_specular);
}
void idle_func() {
    if (isLight3) {
        if (col1 < 1)col1 += 0.0001;
        else col1 = 0;
        if (col2 < 1)col2 += 0.0005;
        else col2 = 0;
        if (col3 < 1)col3 += 0.0007;
        else col3 = 0;
        lit3_diffuse[0] = col1;
        lit3_diffuse[1] = col2;
        lit3_diffuse[2] = col3;
        lit3_specular[0] = col1;
        lit3_specular[1] = col2;
        lit3_specular[2] = col3;
        spot_lit[0] = col1;
        spot_lit[1] = col2;
        spot_lit[2] = col3;
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lit3_diffuse);
        glLightfv(GL_LIGHT3, GL_SPECULAR, lit3_specular);
        display();
    }
    if (isSun) {
        lit2_ang += 0.05;
        display();
    }
    if (isRun) {
        goStraight();
        wave(1, 45);
        little_wave(1, 0, -90);
        display();
    }
    if (isDance) {
        //if (limb_ang > 360)limb_ang -= 360;
        //limb_ang += 1;
        wave(0.2, 90);
        little_wave(0.2, 0, -45);
        body_wave(0.2);
        display();
    }
    if (isSpaceWalk) {
        wave(0.3, 45);
        little_wave(0.3, 0, -45);
        display();
    }
    if (isJump) {
        wave(0.5, 45);
        little_wave(0.5, 0, -90);
        if (jump_mode)loc_y += 0.2;
        else loc_y -= 0.2;
        if (loc_y >= 30)jump_mode = 0;
        else if (loc_y <= 4)jump_mode = 1;
        display();
        if (loc_y <= 4) {
            isJump = 0;
            loc_y -= 2;
            limb_ang = 90;
            small_limb_ang = -90;
            isKneeDown = 1;
            display();
            Sleep(300);
            isKneeDown = 0;
            loc_y += 2;
            limb_ang = 0;
            small_limb_ang = 0;
            display();
        }
    }
    if (isFly) {
        fly_wave(0.3);
        display();
        //printf("%f", fly_ang);
    }
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