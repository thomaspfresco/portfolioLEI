/* ===================================================================================
    Thomas Fresco | 2019219057
======================================================================================= */

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "RgbImage.h"
#include "arrays.h"
#include <iostream>

#define RED         1.0, 0.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define BLUE     0.0, 0.0, 1.0, 1.0

#define FUNDO     0.1,0.5,1,1.0

#define PI         3.14159

void timer(int);

//piano
static GLuint     baixo[] = {3, 2, 1, 0};
static GLuint     borda_esquerda[] = {4, 5, 6, 7};
static GLuint     borda_direita[] = {8, 9, 10 ,11};
static GLuint     topo[] = { 12, 13, 14, 15};
static GLuint     diagonal[] = {16, 17, 18, 19};
static GLuint     tras[] = {20, 21, 22, 23};
static GLuint     esquerda1[] = {24, 25, 26, 27};
static GLuint     esquerda2[] = {28, 29, 30, 31};
static GLuint     direita1[] = {32, 33, 34, 35};
static GLuint     direita2[] = {36, 37, 38, 39};
static GLuint     frente1[] = {40, 41, 42, 43};
static GLuint     frente2[] = {44, 45, 46, 47};
static GLuint     frente3[] = {48, 49, 50, 51};
static GLuint     dentro1[] = {52, 53, 54, 55};
static GLuint     dentro2[] = {56, 57, 58, 59};
static GLuint     dentro3[] = {60, 61, 62, 63};
static GLuint     dentro_fundo[] = {64, 65, 66, 67};

//cubo
static GLuint     cimaCubo[] = {0, 1, 2, 3};
static GLuint     baixoCubo[] = {4, 5, 6, 7};
static GLuint     esquerdaCubo[] = {8, 9, 10, 11};
static GLuint     direitaCubo[] = {12, 13, 14, 15};
static GLuint     frenteCubo[] = {16, 17, 18, 19};
static GLuint     trasCubo[] = {20, 21, 22, 23};

// Objectos (sistema coordenadas)
GLint wScreen = 800, hScreen = 600; // janela (pixeis)
//GLint wScreen = 1920, hScreen = 1080;
GLfloat xC = 100.0, yC = 100.0, zC = 100.0; // Mundo (unidades mundo)

//auxiliares movimento teclas do piano
GLfloat ang1=0,ang2=0,ang3=0,ang4=0,ang5=0,ang6=0,ang7=0,ang8=0,ang9=0,ang10=0,ang11=0,ang12=0,ang13=0,movFader=5;
GLboolean t1=false,t2=false,t3=false,t4=false,t5=false,t6=false,t7=false,t8=false,t9=false,t10=false,t11=false,t12=false,t13=false,fader=false, map=false;

// Visualizacao/Observador
GLfloat  rVisao = 15, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { 5+rVisao * cos(aVisao), 3.0, 3+rVisao * sin(aVisao) };
GLfloat  angZoom = 45;
GLfloat  incZoom = 3;

//=================================================================== TEXTURAS
RgbImage imag;
//------------------------------------------------------------ Skybox
GLUquadricObj* esfera = gluNewQuadric();
//------------------------------------------------------------ Texturas (5 texturas)
GLuint   texture[13];

GLfloat dia=0;
GLint auxCorPontual = 0;
GLint auxCorFoco = 0;

GLfloat posPontual[4] = { 0.0, 5.0, 0.0, 1.0 };
GLfloat corAmbPontual[4] = { 0.5, 0.5, 0.5, 0 };
GLfloat corDifPontual[4] = { 0.5, 0.5, 0.5, 1 };
GLfloat corEspPontual[4] = { 0.5, 0.5, 0.5, 1 };

GLboolean moveFoco=false,movePontual=false;
GLfloat inclinacaoFoco=0;

GLfloat posFoco[] = {5 + 15 * cos(inclinacaoFoco), 15.0,3 + 15 * sin(inclinacaoFoco), 1.0};
GLfloat corDifFoco[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat corEspFoco[] =  {0.0,0.0,0.0,1};
GLfloat direcaoFoco[] = {5-posFoco[0],-posFoco[1],3-posFoco[2], 0.0};

GLfloat luzGlobalCorAmb[4] = { dia, dia, dia, 1.0 };

GLboolean pontual=true,foco=false,malha1=false,malha2=true,auxMaterial=false;

GLint dim = 64;
GLint aberturaFoco = 16;
GLfloat intensidadeFoco = 1.0, intensidadePontual = 0.5;

GLint luzRfoco = 1, luzGfoco = 1, luzBfoco = 1;
GLint luzRpontual = 1, luzGpontual = 1, luzBpontual = 1;

char texto[50];

void desenhaTexto(char* string, GLfloat x, GLfloat y, GLfloat z) {
    glRasterPos3f(x, y, z);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

void initMaterials(int material) {
    switch (material) {
        
        case 0: //plastico vermelho
            glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticAmb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticSpec);
            glMaterialf(GL_FRONT, GL_SHININESS, redPlasticCoef);
            break;
        
        case 1: //plastico branco
            glMaterialfv(GL_FRONT, GL_AMBIENT, whitePlasticAmb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, whitePlasticDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, whitePlasticSpec);
            glMaterialf(GL_FRONT, GL_SHININESS, whitePlasticCoef);
            break;
        
        case 2: //plastico preto
            glMaterialfv(GL_FRONT, GL_AMBIENT, blackPlasticAmb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, blackPlasticDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, blackPlasticSpec);
            glMaterialf(GL_FRONT, GL_SHININESS, blackPlasticCoef);
            break;
        case 3:
            glMaterialfv(GL_FRONT, GL_AMBIENT, goldAmb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, goldDif);
            glMaterialfv(GL_FRONT, GL_SPECULAR, goldSpec);
            glMaterialf(GL_FRONT, GL_SHININESS, goldCoef);
            break;
    }
}

void initTexturas() {
    
    //-----------------------------------------  fader
    glGenTextures(1, &texture[12]);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    imag.LoadBmpFile("fader.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano escuro
    glGenTextures(1, &texture[11]);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    imag.LoadBmpFile("pianoEscuro.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano preto
    glGenTextures(1, &texture[10]);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    imag.LoadBmpFile("pianoPreto.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano lados
    glGenTextures(1, &texture[9]);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    imag.LoadBmpFile("ladoDireito.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano lados
    glGenTextures(1, &texture[8]);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    imag.LoadBmpFile("ladoEsquerdo.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano tras
    glGenTextures(1, &texture[7]);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    imag.LoadBmpFile("trasPiano.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  piano topo
    glGenTextures(1, &texture[6]);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    imag.LoadBmpFile("topoPiano.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    
    //-----------------------------------------  piano generico
    glGenTextures(1, &texture[5]);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    imag.LoadBmpFile("piano.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  tecla brancas
    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    imag.LoadBmpFile("brancas.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  tecla preta
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    imag.LoadBmpFile("pretas.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  coluna
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    imag.LoadBmpFile("parede.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    //-----------------------------------------  tampo
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    imag.LoadBmpFile("marble.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
    
    
    //----------------------------------------- Esfera - skybox envolvente
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    imag.LoadBmpFile("office.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,imag.GetNumCols(),
    imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
    imag.ImageData());
}

void initLights() {
    
    //Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

    //pontual
    glLightfv(GL_LIGHT0, GL_POSITION, posPontual);
    glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbPontual);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifPontual);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corEspPontual);
    
    //foco
    glLightfv(GL_LIGHT1, GL_POSITION, posFoco);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corDifFoco);
    glLightfv(GL_LIGHT1, GL_SPECULAR,corEspFoco);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoFoco);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
}

void inicializa(void)
{
    initTexturas();
    glClearColor(FUNDO);        //Apagar
    glEnable(GL_DEPTH_TEST);    //Profundidade
    glShadeModel(GL_SMOOTH);    //Interpolacao de cores
    
    glEnable(GL_NORMALIZE);
    
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    
    initLights();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    initMaterials(1);
    
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

void drawEixos()
{
    // Eixo X
    glColor4f(RED);
    glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(30, 0, 0);
    glEnd();
    // Eixo Y
    glColor4f(GREEN);
    glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 30, 0);
    glEnd();
    // Eixo Z
    glColor4f(BLUE);
    glBegin(GL_LINES);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 30);
    glEnd();

}

void drawPiano() {
    initMaterials(1);
    
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glColorPointer(3, GL_FLOAT, 0, cores);
    glEnableClientState(GL_COLOR_ARRAY);
    
    //textura simples
    
    glTexCoordPointer(2, GL_FLOAT, 0, texturasPiano);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, borda_esquerda);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, borda_direita);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda1);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita1);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente1);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente2);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente3);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    //texturas personalizadas ---------------------------------------
    
    //textura topo
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, topo);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //textura tras
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //textura lado esquerdo

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //textura lado direito

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //textura escura
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, diagonal);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //textura preta
    initMaterials(2);
    glPushMatrix();
    glTranslatef(0, 0.2,0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, dentro1);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, dentro2);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, dentro3);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, dentro_fundo);
    glPopMatrix();
}

void drawCubo() {
    float med_dim = (float)dim / 2;
    
    glVertexPointer(3, GL_FLOAT, 0, verticesCubo);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, normaisCubo);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    if (malha1==false || malha2==false) {
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cimaCubo);
    }

    else if (malha2==true){
        glPushMatrix();
        glTranslatef(0.5, 0, 0.5);
        glScalef(-1,1, 1);
        glRotatef(270, 0, 1, 0);
        glTranslatef(-0.5, 0, -0.5);
        glScalef(0.5,1, 0.5);
        glNormal3f(0, 1, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, 1, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, 1, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, 1, (float)(i + 1) / med_dim);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, 1, (float)(i + 1) / med_dim);
            }
        glEnd();
        glPopMatrix();
        
    }
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixoCubo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerdaCubo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direitaCubo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frenteCubo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, trasCubo);
}

void drawTecla1() {
    
    initMaterials(1);
    
    glPushMatrix();
    glTranslatef(0.65,0.1+0.2,2.6);
    glRotatef(ang1, 1, 0, 0);
    glScaled(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
}

void drawTecla2() {
    if (auxMaterial) initMaterials(3);
    else initMaterials(2);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[3]);
    
    glPushMatrix();
    glTranslatef(1.45, 0.3+0.2, 2.6);
    glRotatef(ang2, 1, 0, 0);
    glScalef(0.5,0.7,1.8);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla3() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(1.75, 0.1+0.2, 2.6);
    glRotatef(ang3, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla4() {
    
    if (auxMaterial) initMaterials(3);
    else initMaterials(2);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[3]);
    
    glPushMatrix();
    glTranslatef(2.55, 0.3+0.2, 2.6);
    glRotatef(ang4, 1, 0, 0);
    glScalef(0.5,0.7,1.8);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla5() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(2.85, 0.1+0.2, 2.6);
    glRotatef(ang5, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla6() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(3.95, 0.1+0.2, 2.6);
    glRotatef(ang6, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla7() {
    
    if (auxMaterial) initMaterials(3);
    else initMaterials(2);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[3]);
    
    glPushMatrix();
    glTranslatef(4.75, 0.3+0.2, 2.6);
    glRotatef(ang7, 1, 0, 0);
    glScalef(0.5,0.7,1.8);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla8() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(5.05, 0.1+0.2, 2.6);
    glRotatef(ang8, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla9() {
    
    if (auxMaterial) initMaterials(3);
    else initMaterials(2);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[3]);
    
    glPushMatrix();
    glTranslatef(5.85, 0.3+0.2, 2.6);
    glRotatef(ang9, 1, 0, 0);
    glScalef(0.5,0.7,1.8);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla10() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(6.15, 0.1+0.2, 2.6);
    glRotatef(ang10, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla11() {
    
    if (auxMaterial) initMaterials(3);
    else initMaterials(2);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[3]);
    
    glPushMatrix();
    glTranslatef(6.95, 0.3+0.2, 2.6);
    glRotatef(ang11, 1, 0, 0);
    glScalef(0.5,0.7,1.8);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla12() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(7.25, 0.1+0.2, 2.6);
    glRotatef(ang12, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawTecla13() {
    
    initMaterials(1);
    
    //glTexCoordPointer(2, GL_FLOAT, 0, texturasCubo);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glPushMatrix();
    glTranslatef(8.35, 0.1+0.2, 2.6);
    glRotatef(ang13, 1, 0, 0);
    glScalef(1,0.7,3.3);
    drawCubo();
    glPopMatrix();
    
    //glDisable(GL_TEXTURE_2D);
}

void drawBaseFader() {
    
    initMaterials(2);
    
    //glColorPointer(3, GL_FLOAT, 0, preto);
    //glEnableClientState(GL_COLOR_ARRAY);
    
    glPushMatrix();
    glTranslatef(5, 1+0.2, 0.9);
    glScalef(4,0.1,0.3);
    drawCubo();
    glPopMatrix();
}

void drawFader() {
    
    glPushMatrix();
    initMaterials(3);
    glEnable(GL_BLEND);
    glColor4f(1, 1, 1, 0.1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTranslatef(movFader, 1+0.2+0.1, 0.6);
    glScalef(0.2,0.3,0.9);
    drawCubo();
    
    glDisable(GL_BLEND);
    glPopMatrix();
    

}

void drawSuporte() {
    //tampo
    malha1=true;
    glColorPointer(3, GL_FLOAT, 0, branco);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, texturasTampo);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    
    glPushMatrix();
    glTranslatef(5, 0, 3);
    glScalef(14,1,14);
    glTranslatef(-0.5, -1, -0.5);

    drawCubo();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    malha1=false;
    //coluna
    
    glColorPointer(3, GL_FLOAT, 0, branco);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, texturasColuna);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    
    glPushMatrix();
    glTranslatef(5, -0.5, 3);
    glScalef(13,50,13);
    glTranslatef(-0.5, -1, -0.5);
    drawCubo();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void drawCenario() {
    initMaterials(1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
        glTranslatef(5, 0, 3);
        glRotatef(-90, 1,0,0);
        gluQuadricDrawStyle(esfera, GLU_FILL);
        gluQuadricNormals(esfera, GLU_SMOOTH);
        gluQuadricTexture(esfera, GL_TRUE);
        gluSphere(esfera, 100.0, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawScene() {
    drawPiano();

    drawTecla1();
    drawTecla2();
    drawTecla3();
    drawTecla4();
    drawTecla5();
    drawTecla6();
    drawTecla7();
    drawTecla8();
    drawTecla9();
    drawTecla10();
    drawTecla11();
    drawTecla12();
    drawTecla13();
    
    drawSuporte();
    
    drawCenario();
    
    drawBaseFader();
    drawFader();
    
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_LIGHTING);
    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xC, xC, -xC, xC, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);
    //texto
    glColor3f(1, 1, 1);
    
    if (foco) snprintf(texto, 50, "Luz Foco: ON [%d%s] | Ang: %d | R:%d G:%d B:%d",(int)(intensidadeFoco*100),"%",aberturaFoco,luzRfoco,luzGfoco,luzBfoco);
    else snprintf(texto, 50, "Luz Foco: OFF | Ang: %d | R:%d G:%d B:%d",aberturaFoco,luzRfoco,luzGfoco,luzBfoco);
    desenhaTexto(texto, -95, 1, -90);
    
    if (pontual) snprintf(texto, 50, "Luz Pontual: ON [%d%s] | R:%d G:%d B:%d",(int)(intensidadePontual*100),"%",luzRpontual,luzGpontual,luzBpontual);
    else snprintf(texto, 50, "Luz Pontual: OFF | R:%d G:%d B:%d",luzRpontual,luzGpontual,luzBpontual);
    desenhaTexto(texto, -95, 1, -85);
    
    snprintf(texto, 50, "Luz Ambiente: %d%s", (int)(dia*100),"%");
    desenhaTexto(texto, -95, 1, -80);
    
    if (auxMaterial) snprintf(texto, 50, "Teclas Pretas: Ouro | Coef: %.1u",goldCoef);
    else snprintf(texto, 50, "Teclas Pretas: Plastico | Coef: %.1u",goldCoef);
    desenhaTexto(texto, -95, 1, -75);
    
    if (malha2) snprintf(texto, 50, "Malha: ON | Res: %d",dim);
    else snprintf(texto, 50, "Malha: OFF | Res: %d",dim);
    desenhaTexto(texto, -95, 1, -70);
    
    glEnable(GL_LIGHTING);
    
    //projeção secundária (mapa, ortogonal)
    if (map) {
    luzGlobalCorAmb[0] = 1;
    luzGlobalCorAmb[1] = 1;
    luzGlobalCorAmb[2] = 1;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
    //glutPostRedisplay();
        
    glViewport(0, 0, 150, 150);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5-2, 5+2, -3-4, 3+4, -5.0, 5.0);
    //gluOrtho2D(-5-2, 5+2, -3-4, 3+4);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5,1,3,5, 0, 3, 0, 0, -1);
    drawScene();
    }
    
    luzGlobalCorAmb[0] = dia;
    luzGlobalCorAmb[1] = dia;
    luzGlobalCorAmb[2] = dia;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
    //glutPostRedisplay();
    
    if (pontual) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
    
    if (foco) glEnable(GL_LIGHT1);
    else glDisable(GL_LIGHT1);
    
    //projeção principal
    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsP[0],obsP[1],obsP[2], 5, 0.6, 3, 0, 1, 0);
    
    glLightfv(GL_LIGHT1, GL_POSITION, posFoco);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corDifFoco);
    glLightfv(GL_LIGHT1, GL_SPECULAR,corEspFoco);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.01);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoFoco);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    //drawEixos();
    drawScene();
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    
    switch (key) {
        case 'a':
            t1 = true;
            break;
            
        case 'w':
            t2 = true;
            break;
            
        case 's':
            t3 = true;
            break;
            
        case 'e':
            t4 = true;
            break;
            
        case 'd':
            t5 = true;
            break;
            
        case 'f':
            t6 = true;
            break;
            
        case 't':
            t7 = true;
            break;
            
        case 'g':
            t8 = true;
            break;
            
        case 'y':
            t9 = true;
            break;
            
        case 'h':
            t10 = true;
            break;
            
        case 'u':
            t11 = true;
            break;
            
        case 'j':
            t12 = true;
            break;
            
        case 'k':
            t13 = true;
            break;
            
        case 'b': //mover fader
            if (fader) fader = false;
            else fader = true;
            break;
            
        case 'm': //mapa on-off
            if (map) map = false;
            else map = true;
            break;
            
        case '+':
            if (angZoom >= 10*2) angZoom-=10; // zoom in
            break;
        
        case '-':
            if (angZoom <= 90) angZoom+=10; // zoom out
            break;
            
        case '0':
            angZoom = 45; // zoom reset
            break;
            
        case '1':
            if (dia == 1) dia = 0;
            else dia+=0.25;
            luzGlobalCorAmb[0] = dia;
            luzGlobalCorAmb[1] = dia;
            luzGlobalCorAmb[2] = dia;

            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
            glutPostRedisplay();
            break;
            
        case '2':
            if ((int)intensidadePontual == 1) intensidadePontual = 0.1;
            else intensidadePontual += 0.1;
            
            corAmbPontual[0]=luzRpontual * intensidadePontual;
            corAmbPontual[1]=luzGpontual * intensidadePontual;
            corAmbPontual[2]=luzBpontual * intensidadePontual;
            corDifPontual[0]=luzRpontual * intensidadePontual;
            corDifPontual[1]=luzGpontual * intensidadePontual;
            corDifPontual[2]=luzBpontual * intensidadePontual;
            corEspPontual[0]=luzRpontual * intensidadePontual;
            corEspPontual[1]=luzGpontual * intensidadePontual;
            corEspPontual[2]=luzBpontual * intensidadePontual;
            glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbPontual);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifPontual);
            glLightfv(GL_LIGHT0, GL_SPECULAR, corEspPontual);
            //glutPostRedisplay();
            break;
            
        case 'z':
            pontual = !pontual;
            break;
        
        case '3':
            if (auxCorPontual == 3) auxCorPontual = 0;
            else auxCorPontual+=1;
            
            if (auxCorPontual == 0) {
                luzRpontual = 1;
                luzGpontual = 1;
                luzBpontual = 1;
            } else if (auxCorPontual == 1) {
                luzRpontual = 1;
                luzGpontual = 0;
                luzBpontual = 0;
            } else if (auxCorPontual == 2) {
                luzRpontual = 0;
                luzGpontual = 1;
                luzBpontual = 0;
            } else if (auxCorPontual == 3) {
                luzRpontual = 0;
                luzGpontual = 0;
                luzBpontual = 1;
            }
            
            corAmbPontual[0]=luzRpontual * intensidadePontual;
            corAmbPontual[1]=luzGpontual * intensidadePontual;
            corAmbPontual[2]=luzBpontual * intensidadePontual;
            corDifPontual[0]=luzRpontual * intensidadePontual;
            corDifPontual[1]=luzGpontual * intensidadePontual;
            corDifPontual[2]=luzBpontual * intensidadePontual;
            corEspPontual[0]=luzRpontual * intensidadePontual;
            corEspPontual[1]=luzGpontual * intensidadePontual;
            corEspPontual[2]=luzBpontual * intensidadePontual;
            
            glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbPontual);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifPontual);
            glLightfv(GL_LIGHT0, GL_SPECULAR, corEspPontual);
            //glutPostRedisplay();
            break;
        
        case 'x':
            foco = !foco;
            break;
            
        case '4':
            if ((int)intensidadeFoco == 1) intensidadeFoco = 0.1;
            else intensidadeFoco += 0.1;
            
            corDifFoco[0]=luzRfoco * intensidadeFoco;
            corDifFoco[1]=luzGfoco * intensidadeFoco;
            corDifFoco[2]=luzBfoco * intensidadeFoco;
            
            glLightfv(GL_LIGHT1, GL_DIFFUSE, corDifFoco);
            break;
        
        case '6':
            if (auxCorFoco == 3) auxCorFoco = 0;
            else auxCorFoco+=1;
            
            if (auxCorFoco == 0) {
                luzRfoco = 1;
                luzGfoco = 1;
                luzBfoco = 1;
            } else if (auxCorFoco == 1) {
                luzRfoco = 1;
                luzGfoco = 0;
                luzBfoco = 0;
            } else if (auxCorFoco == 2) {
                luzRfoco = 0;
                luzGfoco = 1;
                luzBfoco = 0;
            } else if (auxCorFoco == 3) {
                luzRfoco = 0;
                luzGfoco = 0;
                luzBfoco = 1;
            }
            
            corDifFoco[0]=luzRfoco * intensidadeFoco;
            corDifFoco[1]=luzGfoco * intensidadeFoco;
            corDifFoco[2]=luzBfoco * intensidadeFoco;
            
            glLightfv(GL_LIGHT1, GL_DIFFUSE, corDifFoco);
            //glutPostRedisplay();
            break;
            
        case '7':
            if (goldCoef < 52.0) goldCoef += 1;
            else goldCoef = 1;
            //initMaterials(3);
            break;
            
        case '8':
            if (dim == 256) dim = 1;
            else dim = 2 * dim;
            //glutPostRedisplay();
            break;

        case '5':
            if (aberturaFoco == 20) aberturaFoco=4;
            else aberturaFoco+=2;
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
            //glutPostRedisplay();
            break;
            
        case 'c':
            auxMaterial = !auxMaterial;
            //glutPostRedisplay();
            break;
            
        case 'v':
            malha2 = !malha2;
            //glutPostRedisplay();
            break;
            
        case 32:
            if (moveFoco == false) moveFoco = true;
            if (movePontual == false) {
                posPontual[0] = -30;
                movePontual = true;
            }
            break;
            
        // sair (esc)
        case 27:
            exit(0);
            break;
        }
}

void teclasNotAscii(int key, int x, int y) {
    
    if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.2);
    if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.2);
    if (key == GLUT_KEY_LEFT)  aVisao = (aVisao + 0.1);
    if (key == GLUT_KEY_RIGHT) aVisao = (aVisao - 0.1);

    obsP[0] = 5 + rVisao * cos(aVisao);      // actualiza posicao (X)
    obsP[2] = 3 + rVisao * sin(aVisao);      // actualiza posicao (Z)
        
    glutPostRedisplay();
}


//======================================================= MAIN
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Fader(b) | Mapa(m) | Zoom(+,-,0) | Amb(1) | Pont(z,2,3) | Foco(x,4,5,6) | Material(c,7) | Malha(v,8) | (space)");
    
    inicializa();

    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,timer,0);
    glutMainLoop();

    return 0;
}

void timer(int) {
    
    if (t1) if (ang1<5)ang1+=0.4; else t1 = false;
    if (t1 == false) if (ang1>0.4) ang1-=0.4;
    
    if (t2) if (ang2<5)ang2+=0.4; else t2 = false;
    if (t2 == false) if (ang2>0.4) ang2-=0.4;
    
    if (t3) if (ang3<5)ang3+=0.4; else t3 = false;
    if (t3 == false) if (ang3>0.4) ang3-=0.4;
    
    if (t4) if (ang4<5)ang4+=0.4; else t4 = false;
    if (t4 == false) if (ang4>0.4) ang4-=0.4;
    
    if (t5) if (ang5<5)ang5+=0.4; else t5 = false;
    if (t5 == false) if (ang5>0.4) ang5-=0.4;
    
    if (t6) if (ang6<5)ang6+=0.4; else t6 = false;
    if (t6 == false) if (ang6>0.4) ang6-=0.4;
    
    if (t7) if (ang7<5)ang7+=0.4; else t7 = false;
    if (t7 == false) if (ang7>0.4) ang7-=0.4;
    
    if (t8) if (ang8<5)ang8+=0.4; else t8 = false;
    if (t8 == false) if (ang8>0.4) ang8-=0.4;
    
    if (t9) if (ang9<5)ang9+=0.4; else t9 = false;
    if (t9 == false) if (ang9>0.4) ang9-=0.4;
    
    if (t10) if (ang10<5)ang10+=0.4; else t10 = false;
    if (t10 == false) if (ang10>0.4) ang10-=0.4;
    
    if (t11) if (ang11<5)ang11+=0.4; else t11 = false;
    if (t11 == false) if (ang11>0.4) ang11-=0.4;
    
    if (t12) if (ang12<5)ang12+=0.4; else t12 = false;
    if (t12 == false) if (ang12>0.4) ang12-=0.4;
    
    if (t13) if (ang13<5)ang13+=0.4; else t13 = false;
    if (t13 == false) if (ang13>0.4) ang13-=0.4;
    
    if (fader == true) {
        if(movFader<(9-0.3)) movFader+=0.1;
    } else if(movFader>(5+0.1)) movFader-=0.1;
    
    if (moveFoco == true) {
        //posFoco[] = {20.0, 15.0, 18.0, 1.0};
        if (inclinacaoFoco < 6 + 0.2) {
            inclinacaoFoco +=0.1;
            posFoco[0] = 5 + 15 * cos(inclinacaoFoco);
            posFoco[2] = 3 + 15 * sin(inclinacaoFoco);
            
            direcaoFoco[0] = 5 -posFoco[0];
            direcaoFoco[1] = 0 - posFoco[1];
            direcaoFoco[2] = 3 - posFoco[2];
        
        } else {
            moveFoco = false;
            inclinacaoFoco = 0;
            }
    }
    
    if (movePontual == true) {
        
        corAmbPontual[0]=0;
        corAmbPontual[1]=0;
        corAmbPontual[2]=0;
        corDifPontual[0]=1;
        corDifPontual[1]=1;
        corDifPontual[2]=1;
        corEspPontual[0]=1;
        corEspPontual[1]=1;
        corEspPontual[2]=1;
        if (posPontual[0]<30) posPontual[0]+=1;
        else movePontual = false;
        glLightfv(GL_LIGHT0, GL_POSITION, posPontual);
        glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbPontual);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifPontual);
        glLightfv(GL_LIGHT0, GL_SPECULAR, corEspPontual);
        
    } else {
        posPontual[0]=0;
        corAmbPontual[0]=luzRpontual * intensidadePontual;
        corAmbPontual[1]=luzGpontual * intensidadePontual;
        corAmbPontual[2]=luzBpontual * intensidadePontual;
        corDifPontual[0]=luzRpontual * intensidadePontual;
        corDifPontual[1]=luzGpontual * intensidadePontual;
        corDifPontual[2]=luzBpontual * intensidadePontual;
        corEspPontual[0]=luzRpontual * intensidadePontual;
        corEspPontual[1]=luzGpontual * intensidadePontual;
        corEspPontual[2]=luzBpontual * intensidadePontual;
        glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbPontual);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifPontual);
        glLightfv(GL_LIGHT0, GL_SPECULAR, corEspPontual);
        glLightfv(GL_LIGHT0, GL_POSITION, posPontual);
    }
    glutPostRedisplay();
    glutTimerFunc(30,timer,0);
}
