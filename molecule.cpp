
/****************************************************************************
**
** Copyright (C) 2011 Christian B. Huebschle & George M. Sheldrick
** All rights reserved.
** Contact: chuebsch@moliso.de
**
** This file is part of the ShelXle
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file COPYING included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
****************************************************************************/
#include <molecule.h> 
#include <QtOpenGL>
#include <QGLWidget>
#if defined (Q_WS_MAC)
#include <glu.h>
#else
#include <GL/glu.h>
#endif
const double g2r=180.0/M_PI;
#include "itsme.h"

Molecule::Molecule(){
proba=50;
qPeakRad=0.05;
growQPeak=false;
tubes=0;
qboMin=0.97;
qboMax=1.67;
HAWink=135;
HAMax=2.5;
dratom=0;
LOD=4;
highlightEquivalents = true;
mQolor=QColor(128,76,76);
monoQrom=false;
nopm1=false;
bondColorStyle=0;
bondStrength=0.05;
//nonPositiveDefinite=0;
Kovalenz_Radien[0  ]=55  ;Kovalenz_Radien[1  ]=1   ;Kovalenz_Radien[2  ]=123 ;Kovalenz_Radien[3  ]=90  ;
Kovalenz_Radien[4  ]=80  ;Kovalenz_Radien[5  ]=77  ;Kovalenz_Radien[6  ]=74  ;Kovalenz_Radien[7  ]=71  ;
Kovalenz_Radien[8  ]=72  ;Kovalenz_Radien[9  ]=1   ;Kovalenz_Radien[10 ]=154 ;Kovalenz_Radien[11 ]=149 ;
Kovalenz_Radien[12 ]=118 ;Kovalenz_Radien[13 ]=111 ;Kovalenz_Radien[14 ]=106 ;Kovalenz_Radien[15 ]=102 ;
Kovalenz_Radien[16 ]=99  ;Kovalenz_Radien[17 ]=1   ;Kovalenz_Radien[18 ]=203 ;Kovalenz_Radien[19 ]=174 ;
Kovalenz_Radien[20 ]=144 ;Kovalenz_Radien[21 ]=132 ;Kovalenz_Radien[22 ]=122 ;Kovalenz_Radien[23 ]=118 ;
Kovalenz_Radien[24 ]=117 ;Kovalenz_Radien[25 ]=117 ;Kovalenz_Radien[26 ]=116 ;Kovalenz_Radien[27 ]=124 ;
Kovalenz_Radien[28 ]=117 ;Kovalenz_Radien[29 ]=125 ;Kovalenz_Radien[30 ]=126 ;Kovalenz_Radien[31 ]=122 ;
Kovalenz_Radien[32 ]=120 ;Kovalenz_Radien[33 ]=116 ;Kovalenz_Radien[34 ]=114 ;Kovalenz_Radien[35 ]=1   ;
Kovalenz_Radien[36 ]=218 ;Kovalenz_Radien[37 ]=191 ;Kovalenz_Radien[38 ]=162 ;Kovalenz_Radien[39 ]=145 ;
Kovalenz_Radien[40 ]=134 ;Kovalenz_Radien[41 ]=130 ;Kovalenz_Radien[42 ]=127 ;Kovalenz_Radien[43 ]=125 ;
Kovalenz_Radien[44 ]=125 ;Kovalenz_Radien[45 ]=128 ;Kovalenz_Radien[46 ]=134 ;Kovalenz_Radien[47 ]=148 ;
Kovalenz_Radien[48 ]=144 ;Kovalenz_Radien[49 ]=141 ;Kovalenz_Radien[50 ]=140 ;Kovalenz_Radien[51 ]=136 ;
Kovalenz_Radien[52 ]=133 ;Kovalenz_Radien[53 ]=1   ;Kovalenz_Radien[54 ]=235 ;Kovalenz_Radien[55 ]=198 ;
Kovalenz_Radien[56 ]=169 ;Kovalenz_Radien[57 ]=165 ;Kovalenz_Radien[58 ]=165 ;Kovalenz_Radien[59 ]=164 ;
Kovalenz_Radien[60 ]=164 ;Kovalenz_Radien[61 ]=162 ;Kovalenz_Radien[62 ]=185 ;Kovalenz_Radien[63 ]=161 ;
Kovalenz_Radien[64 ]=159 ;Kovalenz_Radien[65 ]=159 ;Kovalenz_Radien[66 ]=157 ;Kovalenz_Radien[67 ]=157 ;
Kovalenz_Radien[68 ]=156 ;Kovalenz_Radien[69 ]=170 ;Kovalenz_Radien[70 ]=156 ;Kovalenz_Radien[71 ]=144 ;
Kovalenz_Radien[72 ]=134 ;Kovalenz_Radien[73 ]=130 ;Kovalenz_Radien[74 ]=128 ;Kovalenz_Radien[75 ]=126 ;
Kovalenz_Radien[76 ]=127 ;Kovalenz_Radien[77 ]=130 ;Kovalenz_Radien[78 ]=134 ;Kovalenz_Radien[79 ]=149 ;
Kovalenz_Radien[80 ]=148 ;Kovalenz_Radien[81 ]=147 ;Kovalenz_Radien[82 ]=146 ;Kovalenz_Radien[83 ]=146 ;
Kovalenz_Radien[84 ]=145 ;Kovalenz_Radien[85 ]=1   ;Kovalenz_Radien[86 ]=0   ;Kovalenz_Radien[87 ]=1   ;
Kovalenz_Radien[88 ]=188 ;Kovalenz_Radien[89 ]=165 ;Kovalenz_Radien[90 ]=161 ;Kovalenz_Radien[91 ]=142 ;
Kovalenz_Radien[92 ]=130 ;Kovalenz_Radien[93 ]=151 ;Kovalenz_Radien[94 ]=182 ;
//printf("Kovalenz_Radien H hat :%d \n",Kovalenz_Radien[0]);
  /*  //EN{
    ElNeg[0]=220; ElNeg[1]=550; ElNeg[2]= 97; ElNeg[3]=147;
    ElNeg[4]=201; ElNeg[5]=250; ElNeg[6]=307; ElNeg[7]=350;
    ElNeg[8]=410; ElNeg[9]=480; ElNeg[10]=101;ElNeg[11]=123;
    ElNeg[12]=147;ElNeg[13]=174;ElNeg[14]=206;ElNeg[15]=244;
    ElNeg[16]=283;ElNeg[17]=320;ElNeg[18]= 91;ElNeg[19]=104;
    ElNeg[20]=120;ElNeg[21]=132;ElNeg[22]=145;ElNeg[23]=156;
    ElNeg[24]=160;ElNeg[25]=164;ElNeg[26]=170;ElNeg[27]=175;
    ElNeg[28]=175;ElNeg[29]=166;ElNeg[30]=182;ElNeg[31]=202;
    ElNeg[32]=220;ElNeg[33]=248;ElNeg[34]=274;ElNeg[35]=290;
    ElNeg[36]= 89;ElNeg[37]= 99;ElNeg[38]=111;ElNeg[39]=122;
    ElNeg[40]=123;ElNeg[41]=130;ElNeg[42]=136;ElNeg[43]=142;
    ElNeg[44]=145;ElNeg[45]=130;ElNeg[46]=142;ElNeg[47]=146;
    ElNeg[48]=149;ElNeg[49]=172;ElNeg[50]=182;ElNeg[51]=201;
    ElNeg[52]=221;ElNeg[53]=240;ElNeg[54]= 86;ElNeg[55]= 97;
    ElNeg[56]=108;ElNeg[57]=108;ElNeg[58]=107;ElNeg[59]=107;
    ElNeg[60]=107;ElNeg[61]=107;ElNeg[62]=110;ElNeg[63]=111;
    ElNeg[64]=110;ElNeg[65]=110;ElNeg[66]=110;ElNeg[67]=111;
    ElNeg[68]=111;ElNeg[69]=106;ElNeg[70]=114;ElNeg[71]=123;
    ElNeg[72]=133;ElNeg[73]=140;ElNeg[74]=146;ElNeg[75]=152;
    ElNeg[76]=155;ElNeg[77]=142;ElNeg[78]=142;ElNeg[79]=144;
    ElNeg[80]=144;ElNeg[81]=155;ElNeg[82]=167;
    */
    for (int i=0; i<109; i++){
      if (i<95) arad[i]=(i)?Kovalenz_Radien[i]/(250.0):Kovalenz_Radien[i]/(500.0);
      else arad[i]=0.5;
      AtomStyle[i]=ATOM_STYLE_WALLS|ATOM_STYLE_RINGS|ATOM_STYLE_SPHERE;

      switch (i){
      case 0://H
        AtomColor[i]=QColor(Qt::white).darker(20);
        break;
      case 2://Li
        AtomColor[i]=QColor(Qt::darkRed);
	AtomStyle[i]|=ATOM_STYLE_METAL;
        break;
      case 3://Be
        AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
        break;
      case 4://B
        AtomColor[i]=QColor(Qt::darkBlue);
        break;
      case 5://C
        AtomColor[i]=QColor("#242424");
	AtomStyle[i]|=ATOM_STYLE_WHITERING;
        break;
      case 6://N
        AtomColor[i]=QColor(Qt::blue);
	AtomStyle[i]|=ATOM_STYLE_WHITERING;
        break;
      case 7://O
        AtomColor[i]=QColor(Qt::red);
	AtomStyle[i]|=ATOM_STYLE_WHITERING;
        break;
      case 8://F
        AtomColor[i]=QColor("chartreuse");
        break;
      case 9://ne
	AtomColor[i]=QColor(192,192,192,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 10://na
	AtomColor[i]=QColor(239,154,251,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 11://mg
	AtomColor[i]=QColor(51,153,255,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 12://al
		AtomColor[i]=QColor(102,179,255,128);
		break;
      case 13://si
		AtomColor[i]=QColor(128,128,128,128);
		break;
      case 14://p
        AtomColor[i]=QColor(183,41,244,128);
        break;
      case 15://s
        AtomColor[i]=QColor(190,190,0,128);
        break;
      case 16://cl
	AtomColor[i]=QColor( 65,111, 65,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;

      case 18://k
	AtomColor[i]=QColor(192,46,180,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 19://ca
	AtomColor[i]=QColor(170,170,128,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 20:
	AtomColor[i]=QColor(236,136,171,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 21:
	AtomColor[i]=QColor(242,59,3,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 22:
	AtomColor[i]=QColor(100,128,0,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 23:
	AtomColor[i]=QColor(255,255,0,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 24:
	AtomColor[i]=QColor(204,153,153,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 25:
	AtomColor[i]=QColor(0,162,0,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 26:
	AtomColor[i]=QColor(109,0,109,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 27:
	AtomColor[i]=QColor(170,82,64,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 28://Cu
        AtomColor[i]=QColor("salmon");
	AtomStyle[i]|=ATOM_STYLE_METAL;

        break;
      case 29:
	AtomColor[i]=QColor(63,123,111,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 30:
	AtomColor[i]=QColor(78,254,208,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 31:
	AtomColor[i]=QColor(136,136,136,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 32:
		AtomColor[i]=QColor(191,218,255,128);
		break;
      case 33:
		AtomColor[i]=QColor(90,23,23,128);
		break;
      case 34:
		AtomColor[i]=QColor(222,188,133,128);
		break;
      case 35:
		AtomColor[i]=QColor(240,120,0,128);
		break;
      case 36:	
	AtomColor[i]=QColor(50,15,0,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 37:
	AtomColor[i]=QColor(192,192,192,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 38:
	AtomColor[i]=QColor(204,113,32,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 39:
	AtomColor[i]=QColor(236,97,59,128);
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 40:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 41:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 42:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 43:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 44:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 45:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 46:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 47:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 48:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 49:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 54:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 55:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 56:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 57:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 58:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 59:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 61:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 62:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 63:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 67:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 68:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 69:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 70:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 71:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 72:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 73:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 74:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 75:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 76:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 77:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 79:
	AtomStyle[i]|=ATOM_STYLE_METAL;
	AtomColor[i]=QColor("silver");
	break;
      case 80:
	AtomStyle[i]|=ATOM_STYLE_METAL;
	AtomColor[i]=QColor("silver");
	break;
      case 81:
	AtomStyle[i]|=ATOM_STYLE_METAL;
	AtomColor[i]=QColor("silver");
	break;
      case 82:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 83: 
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 86:
	AtomStyle[i]|=ATOM_STYLE_METAL;
	AtomColor[i]=QColor("silver");
	break;
      case 87:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 88:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 89:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 91:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 92:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 93:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;
      case 97:
	AtomColor[i]=QColor("silver");
	AtomStyle[i]|=ATOM_STYLE_METAL;
	break;

      case 78://Au
        AtomColor[i]=QColor("goldenrod");
	AtomStyle[i]|=ATOM_STYLE_METAL;

        break;
      default:
        AtomColor[i]=QColor("silver");
        break;
      }
      AtomColor[i].setAlpha(127);
    }
    AtomStyle[0]=ATOM_STYLE_SPHERE|ATOM_STYLE_NOLABEL|ATOM_STYLE_NOADP;
}

void Molecule::sphere(int adp){
  if (adp) glEnable(GL_BLEND);
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricNormals(q, GL_SMOOTH);
  if (dratom){ 
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    if (dratom==3)glColor3d(0.4,0.8,1.0);
    if (dratom<3) glColor3d(1.0,1.0,0.0);
    gluQuadricDrawStyle(q,GLU_LINE);
    if (dratom==3)gluSphere(q,0.33,7,7);
    else if (dratom==5)gluSphere(q,1.00,7,7);
    else gluSphere(q,0.3,5,5);
    glEnable(GL_DEPTH_TEST);
  }
  else{ 
    gluQuadricDrawStyle(q,GLU_FILL);
    gluSphere(q,0.96,4*LOD,9*LOD);
    glDisable(GL_BLEND);
  }
  gluDeleteQuadric(q);
}

void Molecule::ellipse(int style){
  glDisable(GL_BLEND);
  if (style&ATOM_STYLE_WALLS){
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  if (style&ATOM_STYLE_PLAID) glBindTexture(GL_TEXTURE_2D, adpwall_plaid);
  else glBindTexture(GL_TEXTURE_2D, adpwall);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glNormal3f(-1.0,0,0);

  glTexCoord2d(0,0);
  glVertex3f(0.0,-1.0,-1.0);
  glTexCoord2d(0,1);
  glVertex3f(0.0,-1.0, 1.0);
  glTexCoord2d(1,1);
  glVertex3f(0.0, 1.0, 1.0);
  glTexCoord2d(1,0);
  glVertex3f(0.0, 1.0,-1.0);

  glNormal3f(0.0,-1,0);
  glTexCoord2d(0,0);
  glVertex3f(-1.0,0.0,-1.0);
  glTexCoord2d(0,1);
  glVertex3f(-1.0,0.0, 1.0);
  glTexCoord2d(1,1);
  glVertex3f( 1.0,0.0, 1.0);
  glTexCoord2d(1,0);
  glVertex3f( 1.0,0.0,-1.0);

  glNormal3f(0.0,0,-1);
  glTexCoord2d(0,0);
  glVertex3f(-1.0,-1.0,0.0);
  glTexCoord2d(0,1);
  glVertex3f(-1.0, 1.0,0.0);
  glTexCoord2d(1,1);
  glVertex3f( 1.0, 1.0,0.0);
  glTexCoord2d(1,0);
  glVertex3f( 1.0,-1.0,0.0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  }
//  glEnable(GL_CULL_FACE);
  if ((style&ATOM_STYLE_SOLID)&& ( style&ATOM_STYLE_SPHERE)){
    GLUquadricObj *q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);
    //gluQuadricTexture(q,GL_TRUE);
    gluSphere(q,0.96,4*LOD,9*LOD);
    gluDeleteQuadric(q);
   }
  if (style&ATOM_STYLE_RINGS){
    glPushMatrix();
    if (style&ATOM_STYLE_WHITERING) glColor3f(1,1,1);
    else glColor3f(0,0,0);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    GLUquadricObj *q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);   // ein Zylinder
    glPushMatrix();
    gluCylinder(q, 1.0, 1.0, 0.15 , 5*LOD, 1);
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(q, 1.0, 1.0, 0.15 , 5*LOD, 1);
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0,0.0,1.0,0.0);
    gluCylinder(q, 1.0, 1.0, 0.15 , 5*LOD, 1);
    glPopMatrix();

    gluDeleteQuadric(q);
    glPopMatrix();
  glEnable(GL_LIGHTING);
  }
}

void Molecule::dCube(GLfloat rad){
  glDisable(GL_BLEND);
  glBegin(GL_QUADS);
  glNormal3f(0,0,1);
  glVertex3f(-rad,-rad, rad);
  glVertex3f( rad,-rad, rad);
  glVertex3f( rad, rad, rad);
  glVertex3f(-rad, rad, rad);
  glNormal3f(0,0,-1);
  glVertex3f(-rad,-rad, -rad);
  glVertex3f(-rad, rad, -rad);
  glVertex3f( rad, rad, -rad);
  glVertex3f( rad,-rad, -rad);
  glNormal3f(0,1,0);
  glVertex3f(-rad, rad,-rad);
  glVertex3f(-rad, rad, rad);
  glVertex3f( rad, rad, rad);
  glVertex3f( rad, rad,-rad);
  glNormal3f(0,-1,0);
  glVertex3f(-rad, -rad,-rad);
  glVertex3f( rad, -rad,-rad);
  glVertex3f( rad, -rad, rad);
  glVertex3f(-rad, -rad, rad);
  glNormal3f(1,0,0);
  glVertex3f(rad, -rad, -rad);
  glVertex3f(rad, rad, -rad);
  glVertex3f(rad, rad, rad);
  glVertex3f(rad, -rad, rad);
  glNormal3f(-1,0,0);
  glVertex3f(-rad, -rad, -rad);
  glVertex3f(-rad, -rad, rad);
  glVertex3f(-rad, rad, rad);
  glVertex3f(-rad, rad, -rad);
  glEnd();
}

void Molecule::ikosa(double R){
    /*
    (0, Â±1, Â±f)
    (Â±1, Â±f, 0)
    (Â±f, 0, Â±1)
    */

  glDisable(GL_BLEND);
double P =R*(1 / 2.0 *(1+sqrt(5.0)));
double xx=3*sqrt(1+P*P);
double l=1/xx, p=P/xx;
glBegin(GL_TRIANGLES);

glNormal3f(-p,0,2*p+l);//123
glVertex3f(0,-R,P); //1
glVertex3f(0, R,P); //2
glVertex3f(-P,0,R);//3

glNormal3f(p,0,l+2*p);//124
glVertex3f( P, 0,R);//4
glVertex3f( 0, R,P);//2
glVertex3f( 0,-R,P);//1

glNormal3f(-l-2*p,-p,0);//356
glVertex3f(-R,-P, 0);//5
glVertex3f(-P, 0, R);//3
glVertex3f(-P, 0,-R);//6

glNormal3f(-p-l,-l-p,l+p);//135
glVertex3f(-P, 0,R);//3
glVertex3f(-R,-P,0);//5
glVertex3f( 0,-R,P); //1

glNormal3f(l+p,-l-p,l+p);//148
glVertex3f(0,-R,P);//1
glVertex3f(R,-P,0);//8
glVertex3f(P, 0,R);//4

glNormal3f(0,-l-2*p,p);//158
glVertex3f( 0,-R,P); //1
glVertex3f(-R,-P,0);//5
glVertex3f( R,-P,0);//8

glNormal3f(l+2*p,-p,0);//478
glVertex3f(R,-P, 0);//8
glVertex3f(P, 0,-R);//7
glVertex3f(P, 0, R);//4

glNormal3f(2*p+l,p,0);//479
glVertex3f(P,0,R);//4
glVertex3f(P,0,-R);//7
glVertex3f(R,P,0);//9

glNormal3f(l+p,l+p,l+p);//249
glVertex3f(0,R,P);//2
glVertex3f(P,0,R);//4
glVertex3f(R,P,0);//9

glNormal3f(-l-p ,-p-l,-l-p);//56 11
glVertex3f(-R,-P, 0);//5
glVertex3f(-P, 0,-R);//6
glVertex3f( 0,-R,-P);//11

glNormal3f(0,-2*p-l,-p);//58 11
glVertex3f( R,-P, 0);//8
glVertex3f(-R,-P, 0);//5
glVertex3f( 0,-R,-P);//11

glNormal3f(p+l,-p-l,-l-p);//78 11
glVertex3f(P, 0,-R);//7
glVertex3f(R,-P, 0);//8
glVertex3f(0,-R,-P);//11

glNormal3f(0,2*p+l,p);//29 12
glVertex3f( R, P,0);//9
glVertex3f(-R, P,0);//12
glVertex3f( 0, R,P);//2

glNormal3f(-p-l,l+p,l+p);//23 12
glVertex3f(-P,0,R);//3
glVertex3f( 0,R,P);//2
glVertex3f(-R,P,0);//12

glNormal3f(-2*p-l,p,0);//36 12
glVertex3f(-P,0,-R);//6
glVertex3f(-P,0, R);//3
glVertex3f(-R,P, 0);//12

glNormal3f(p,0,-2*p-l);//710 11
glVertex3f(0, R,-P);//10
glVertex3f(P, 0,-R);//7
glVertex3f(0,-R,-P);//11

glNormal3f(l+p,l+p,-p-l);//7910
glVertex3f(0,R,-P);//10
glVertex3f(R,P, 0);//9
glVertex3f(P,0,-R);//7

glNormal3f(-l-p,l+p,-l-p);//610 12
glVertex3f( 0,R,-P);//10
glVertex3f(-P,0,-R);//6
glVertex3f(-R,P, 0);//12

glNormal3f(0,l+2*p,-p);//910 12
glVertex3f( 0,R,-P);//10
glVertex3f(-R,P, 0);//12
glVertex3f( R,P, 0);//9

glNormal3f(-p, 0,-2*p-l);//610 11
glVertex3f( 0, R,-P);//10
glVertex3f( 0,-R,-P);//11
glVertex3f(-P, 0,-R);//6
glEnd();
}

double Molecule::dieder(V3 a,V3 b, V3 c){

  static double erg;
  double A[3],B[3],sig;
  sig=a.x*b.y*c.z - a.z*b.y*c.x +
    a.z*b.x*c.y - a.x*b.z*c.y +
    a.y*b.z*c.x - a.y*b.x*c.z;

  A[0]= a.y*b.z - a.z*b.y;
  A[1]=-a.x*b.z + a.z*b.x;
  A[2]= a.x*b.y - a.y*b.x;

  B[0]=-b.y*c.z + b.z*c.y;
  B[1]= b.x*c.z - b.z*c.x;
  B[2]=-b.x*c.y + b.y*c.x;
  //  printf("A%f B%f\n",sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]),sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]));
  erg=(A[0]*B[0]+A[1]*B[1]+A[2]*B[2])/(sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2])*sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]));
  erg=acos(erg)/M_PI*180.0f;
 return (sig>0)?(erg):(-erg);
}

double Molecule::winkel(V3 a,V3 b){

  static double erg;
  if ((Norm(a)<0.1)||(Norm(b)<0.1)) return 0;
  erg= a*b /(sqrt(Norm(a))*sqrt(Norm(b)));
//  erg=(a.x*b.x+a.y*b.y+a.z*b.z)/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)*sqrt(b.x*b.x+b.y*b.y+b.z*b.z));
  erg=acos(erg)/M_PI*180.0;
 return(erg);
}

V3 Molecule::kreuzX(double x1,double y1,double z1,double x2,double y2,double z2) {

  static V3 erg;
  erg.x=-y1*z2+z1*y2;
  erg.y=-z1*x2+x1*z2;
  erg.z=-x1*y2+y1*x2;
 return(erg);
}


bool Molecule::decodeSymmCard(QString symmCard){ 
 /*! decodes a symmetry card like 'SYMM -X, 1/2+Y, -Z' and feeds cell.symmops and cell.trans lists.
  * @param symmCard like 'SYMM -X, 1/2+Y, -Z'.
  * \returns true on sucess.
  * */ 
  QString sc=symmCard.toUpper().remove("SYMM").trimmed();
  sc=sc.remove("'");
  sc=sc.remove(" ");
  QStringList axe=sc.split(",");
  QStringList bruch;
  if (axe.size()!=3) return false;
  double sx[3],sy[3],sz[3],t[3];
  for (int i=0; i<3; i++){
    sx[i]=0;sy[i]=0;sz[i]=0;t[i]=0;
    if (axe.at(i).contains("-X")) {sx[i]=-1.0;axe[i].remove("-X");}
    else if (axe.at(i).contains("X")) {sx[i]=1.0;axe[i].remove("X");}
    if (axe.at(i).contains("-Y")) {sy[i]=-1.0;axe[i].remove("-Y");}
    else if (axe.at(i).contains("Y")) {sy[i]=1.0;axe[i].remove("Y");}
    if (axe.at(i).contains("-Z")) {sz[i]=-1.0;axe[i].remove("-Z");}
    else if (axe.at(i).contains("Z")) {sz[i]=1.0;axe[i].remove("Z");}
    if (axe.at(i).endsWith("+")) axe[i].remove("+");
    if (axe.at(i).contains("/")) {
      bruch=axe.at(i).split("/");
      if (bruch.size()==2) t[i]=bruch.at(0).toDouble() / bruch.at(1).toDouble();
    }
    else if (!axe.at(i).isEmpty()) t[i]=axe.at(i).toDouble();
  }
  Matrix sm = Matrix(sx[0],sy[0],sz[0],	  sx[1],sy[1],sz[1],  sx[2],sy[2],sz[2]);

  cell.symmops.append(sm);
  cell.trans.append(V3(t[0],t[1],t[2]));
  return true;
}

bool Molecule::decodeSymmCardEQIV(QString symmCard){
  /*! decodes a symmetry card like 'SYMM -X, 1/2+Y, -Z' and feeds Molecule.symmopsEQIV, Molecule.transEQIV and Molecule.labelEQIV.
   * \returns true on success.
   * @param symmCard like 'SYMM -X, 1/2+Y, -Z'.
   */
  QString sc=symmCard.toUpper().remove("EQIV").trimmed();
  labelEQIV.append(sc.section(' ',0,0));
  sc.remove(labelEQIV.last());
  sc=sc.remove("'");
  sc=sc.remove(" ");
  QStringList axe=sc.split(",");
  QStringList bruch;
  if (axe.size()!=3) return false;
  double sx[3],sy[3],sz[3],t[3];
  for (int i=0; i<3; i++){
    sx[i]=0;sy[i]=0;sz[i]=0;t[i]=0;
    if (axe.at(i).contains("-X")) {sx[i]=-1.0;axe[i].remove("-X");}
    else if (axe.at(i).contains("X")) {sx[i]=1.0;axe[i].remove("X");}
    if (axe.at(i).contains("-Y")) {sy[i]=-1.0;axe[i].remove("-Y");}
    else if (axe.at(i).contains("Y")) {sy[i]=1.0;axe[i].remove("Y");}
    if (axe.at(i).contains("-Z")) {sz[i]=-1.0;axe[i].remove("-Z");}
    else if (axe.at(i).contains("Z")) {sz[i]=1.0;axe[i].remove("Z");}
    if (axe.at(i).endsWith("+")) axe[i].remove("+");
    if (axe.at(i).contains("/")) {
      bruch=axe.at(i).split("/");
      if (bruch.size()==2) t[i]=bruch.at(0).toDouble() / bruch.at(1).toDouble();
    }
    else if (!axe.at(i).isEmpty()) t[i]=axe.at(i).toDouble();
  }
  Matrix sm = Matrix(sx[0],sy[0],sz[0],	  sx[1],sy[1],sz[1],  sx[2],sy[2],sz[2]);
  symmopsEQIV.append(sm);
  transEQIV.append(V3(t[0],t[1],t[2]));
//  qDebug()<<sx[0]<<sy[0]<<sz[0]<<   sx[1]<<sy[1]<<sz[1]<<  sx[2]<<sy[2]<<sz[2]<<t[0]<<t[1]<<t[2];
  return true;
}

QString Molecule::symmCard2Code(QString symmCard){
  /*! Creates a internal symmetry code from a human readable symmetry card.
   * @param symmCard human readable symmetry card like "-X, 1/2+Y, -Z"
   * \returns internal symmetry code like 'n_555:1'
   */
    QString sc=symmCard.toUpper().trimmed();
    sc=sc.remove("'");
    sc=sc.remove(" ");
    QStringList axe=sc.split(",");
    QStringList bruch;
    if (axe.size()!=3) return "";
    double sx[3],sy[3],sz[3],t[3];
    for (int i=0; i<3; i++){
      sx[i]=0;sy[i]=0;sz[i]=0;t[i]=0;
      if (axe.at(i).contains("-X")) {sx[i]=-1.0;axe[i].remove("-X");}
      else if (axe.at(i).contains("X")) {sx[i]=1.0;axe[i].remove("X");}
      if (axe.at(i).contains("-Y")) {sy[i]=-1.0;axe[i].remove("-Y");}
      else if (axe.at(i).contains("Y")) {sy[i]=1.0;axe[i].remove("Y");}
      if (axe.at(i).contains("-Z")) {sz[i]=-1.0;axe[i].remove("-Z");}
      else if (axe.at(i).contains("Z")) {sz[i]=1.0;axe[i].remove("Z");}
      if (axe.at(i).endsWith("+")) axe[i].remove("+");
      if (axe.at(i).contains("/")) {
        bruch=axe.at(i).split("/");
        if (bruch.size()==2) t[i]=bruch.at(0).toDouble() / bruch.at(1).toDouble();
      }
      else if (!axe.at(i).isEmpty()) t[i]=axe.at(i).toDouble();
    }
    Matrix sm = Matrix(sx[0],sy[0],sz[0],	  sx[1],sy[1],sz[1],  sx[2],sy[2],sz[2]);
    if (!cell.symmops.contains(sm)) return "";
    V3 r;
    r.x=fmod(t[0]+10,1.0);
    r.y=fmod(t[1]+10,1.0);
    r.z=fmod(t[2]+10,1.0);
    for (int i=0; i<cell.symmops.size();i++){
        if ((cell.symmops.at(i)==sm)&&(cell.trans.at(i)==r)){
            QString ss=QString("%1_%2%3%4:1,")
                    .arg(i+1)
                    .arg((int)(t[0]-r.x) +5)
                    .arg((int)(t[1]-r.y) +5)
                    .arg((int)(t[2]-r.z) +5);
            return ss;
        }
    }
    return "";
}

double Molecule::shortestDistance(QString sc){
  /*! computes the shortest distance for the given internal symmetry code.
   */
    double erg=100000;
    V3 frac;
    int s,h,k,l,symmgroup;
    sscanf(sc.toLatin1(),"%d_%1d%1d%1d:%d",&s,&h,&k,&l,&symmgroup);
        //printf("BS:!%s! %d h%d k%d l%d sg%d\n",brauchSymm.at(j).toStdString().c_str(),s,h,k,l,symmgroup);
    h-=5;
    k-=5;
    l-=5;
    s--;
    for (int i=0;i<asymm.size();i++){
        if ((asymm[i].molindex==symmgroup)&&(asymm[i].an>-1)){
            frac=cell.symmops.at(s)*asymm[i].frac+cell.trans.at(s)+V3(h,k,l);
            for (int j=0;j<asymm.size();j++){
                if (asymm.at(j).an<0)continue;
                erg=qMin(erg,fl(frac.x-asymm.at(j).frac.x,frac.y-asymm.at(j).frac.y,frac.z-asymm.at(j).frac.z));
            }
        }
    }
    return erg;
}



void Molecule::loadSettings(){
  /*! Loads molecular parameters stored in shelxle.ini 
   */
  QSettings einstellung( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME );
  //einstellung.sync();
  einstellung.beginGroup("PSE");
  pseSize=einstellung.value("FontSize",10).toInt();
  einstellung.endGroup();
  einstellung.beginGroup("Atoms");
  int acsize;
  acsize = einstellung.beginReadArray("CovaleceRadii");
 // printf("H hat %d actsize %d\n", Kovalenz_Radien[0], acsize);
  for (int i = 0; i < acsize; ++i) {
    einstellung.setArrayIndex(i);
    Kovalenz_Radien[i] = einstellung.value("Radius").toInt();
//    printf ("%d = %d\n",i,Kovalenz_Radien[i]);
  }
  einstellung.endArray();
  acsize = einstellung.beginReadArray("BallRadii");
  for (int i = 0; i < acsize; ++i) {
    einstellung.setArrayIndex(i);
    arad[i]=einstellung.value("Radius").toDouble();
  }
  einstellung.endArray();
  acsize = einstellung.beginReadArray("AtomColors");
 // printf("H hat %d Col actsize %d\n", Kovalenz_Radien[0], acsize);
  for (int i = 0; i < acsize; ++i) {
    einstellung.setArrayIndex(i);
    AtomColor[i] = einstellung.value("Color").value<QColor>();
    AtomColor[i].setAlphaF(0.5);
  }
  einstellung.endArray();
  acsize = einstellung.beginReadArray("AtomStyles");

  for (int i = 0; i < acsize; ++i) {
    einstellung.setArrayIndex(i);
    AtomStyle[i] = einstellung.value("Style").toInt();
  }
  einstellung.endArray();
  einstellung.endGroup();
  einstellung.beginGroup("Bonds");
  bondColor=einstellung.value("BondColor",Qt::darkGray).value<QColor>();
  bondColorStyle=einstellung.value("BondStyle",0).toInt();
  bondStrength=einstellung.value("BondStrength",0.05).toDouble();
  HAMax=einstellung.value("HBondMaxDist",2.5).toDouble();
  HAWink=einstellung.value("HBondMaxAng",135.0).toDouble();

  enviBondColor=einstellung.value("enviBondColor",Qt::darkGreen).value<QColor>();
  enviHBColor=einstellung.value("enviHBColor",Qt::darkYellow).value<QColor>();
  enviDefaultColor=einstellung.value("enviDefaultColor",Qt::black).value<QColor>();
  einstellung.endGroup();
  einstellung.beginGroup("QPeaks");
  qPeakRad = einstellung.value("QPeaksRadius",0.05).toDouble();
  einstellung.endGroup();
  einstellung.beginGroup("Window");
  LOD=einstellung.value("LevelOfDetail",4).toInt();
  einstellung.endGroup();
}
void Molecule::setHBondMaxDist(double d){if ((d>0.0)&&(d<4.0)) HAMax=d;}
void Molecule::setHBondMaxAngl(double w){if ((w>0.0)&&(w<180.0)) HAWink=w;}

double Molecule::hbdist(){return HAMax;}
double Molecule::hbangl(){return HAWink;}
#include <stdio.h>
QString Molecule::symmcode2human(int s){
  /*! @param s the Nth symmetry operation of the space group.
   *  \returns human readable string. 
   */
    QString erg;
    Matrix m;
    V3 t;
    int h=0,k=0,l=0;
    m=cell.symmops.at(s);
    QString symstrX,symstrY,symstrZ;
    if (m.m11==1) symstrX.append("+x");
    if (m.m11==-1) symstrX.append("-x");
    if (m.m21==1) symstrX.append("+y");
    if (m.m21==-1) symstrX.append("-y");
    if (m.m31==1) symstrX.append("+z");
    if (m.m31==-1) symstrX.append("-z");

    if (m.m12==1) symstrY.append("+x");
    if (m.m12==-1) symstrY.append("-x");
    if (m.m22==1) symstrY.append("+y");
    if (m.m22==-1) symstrY.append("-y");
    if (m.m32==1) symstrY.append("+z");
    if (m.m32==-1) symstrY.append("-z");

    if (m.m13==1) symstrZ.append("+x");
    if (m.m13==-1) symstrZ.append("-x");
    if (m.m23==1) symstrZ.append("+y");
    if (m.m23==-1) symstrZ.append("-y");
    if (m.m33==1) symstrZ.append("+z");
    if (m.m33==-1) symstrZ.append("-z");
    t=cell.trans.at(s);
    V3 zaehler,nenner;
    double egal;
    for (int g=1;g<7; g++){
      nenner.x=(t.x<0)?-g:g;;
      zaehler.x=(int)round((t.x+h)*g);
      if (fabs(modf(t.x*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.y=(t.y<0)?-g:g;
      zaehler.y=(int)round((t.y+k)*g);
      if (fabs(modf(t.y*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.z=(t.z<0)?-g:g;
      zaehler.z=(int)round((t.z+l)*g);
      if (fabs(modf(t.z*g,&egal))<0.05) break;
    }
    erg=(QString(" %1/%2%3, %4/%5%6, %7/%8%9")
                    .arg(zaehler.x)
                    .arg(nenner.x)
                    .arg(symstrX)
                    .arg(zaehler.y)
                    .arg(nenner.y)
                    .arg(symstrY)
                    .arg(zaehler.z)
                    .arg(nenner.z)
                    .arg(symstrZ));
  erg.remove(QRegExp("0/\\d"));
  erg.replace("1/1","1");
  erg.replace("2/1","2");
  erg.replace("3/1","3");
  erg.replace("4/1","4");
  erg.replace("5/1","5");
  erg.replace("6/1","6");
  return erg;
}

QString Molecule::symmcode2human(QString brauchSymm){
  /*! @param brauchSymm internal symmetry code n_555
   *  \returns human readable string 
   */
    QString erg;
    Matrix m;
    V3 t;
    int h,k,l,s;
    sscanf(brauchSymm.toLatin1(),"%d_%1d%1d%1d",&s,&h,&k,&l);
    h-=5;
    k-=5;
    l-=5;
    s--;
    m=cell.symmops.at(s);
    QString symstrX,symstrY,symstrZ;
    if (m.m11==1) symstrX.append("+x");
    if (m.m11==-1) symstrX.append("-x");
    if (m.m21==1) symstrX.append("+y");
    if (m.m21==-1) symstrX.append("-y");
    if (m.m31==1) symstrX.append("+z");
    if (m.m31==-1) symstrX.append("-z");

    if (m.m12==1) symstrY.append("+x");
    if (m.m12==-1) symstrY.append("-x");
    if (m.m22==1) symstrY.append("+y");
    if (m.m22==-1) symstrY.append("-y");
    if (m.m32==1) symstrY.append("+z");
    if (m.m32==-1) symstrY.append("-z");

    if (m.m13==1) symstrZ.append("+x");
    if (m.m13==-1) symstrZ.append("-x");
    if (m.m23==1) symstrZ.append("+y");
    if (m.m23==-1) symstrZ.append("-y");
    if (m.m33==1) symstrZ.append("+z");
    if (m.m33==-1) symstrZ.append("-z");
    t=cell.trans.at(s);
    V3 zaehler,nenner;
    double egal;
    for (int g=1;g<7; g++){
      nenner.x=(t.x<0)?-g:g;;
      zaehler.x=(int)round((t.x+h)*g);
      if (fabs(modf(t.x*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.y=(t.y<0)?-g:g;
      zaehler.y=(int)round((t.y+k)*g);
      if (fabs(modf(t.y*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.z=(t.z<0)?-g:g;
      zaehler.z=(int)round((t.z+l)*g);
      if (fabs(modf(t.z*g,&egal))<0.05) break;
    }
    erg=(QString("%1/%2%3, %4/%5%6, %7/%8%9")
                    .arg(zaehler.x)
                    .arg(nenner.x)
                    .arg(symstrX)
                    .arg(zaehler.y)
                    .arg(nenner.y)
                    .arg(symstrY)
                    .arg(zaehler.z)
                    .arg(nenner.z)
                    .arg(symstrZ));


  erg.remove(QRegExp("0/\\d"));
  erg.replace("1/1","1");
  erg.replace("2/1","2");
  erg.replace("3/1","3");
  erg.replace("4/1","4");
  erg.replace("5/1","5");
  erg.replace("6/1","6");
  return erg;
}

QString Molecule::symmcode2human(QString brauchSymm, int j){
  /*! @param brauchSymm internal symmetry code n_555
   *  @param j symmetry number to prepend the human readable string returned.
   *  \returns human readable string 
   */
    QString erg;
    Matrix m;
    V3 t;
    int h,k,l,s;
    sscanf(brauchSymm.toLatin1(),"%d_%1d%1d%1d",&s,&h,&k,&l);
    h-=5;
    k-=5;
    l-=5;
    s--;
    m=cell.symmops.at(s);
    QString symstrX,symstrY,symstrZ;
    if (m.m11==1) symstrX.append("+x");
    if (m.m11==-1) symstrX.append("-x");
    if (m.m21==1) symstrX.append("+y");
    if (m.m21==-1) symstrX.append("-y");
    if (m.m31==1) symstrX.append("+z");
    if (m.m31==-1) symstrX.append("-z");

    if (m.m12==1) symstrY.append("+x");
    if (m.m12==-1) symstrY.append("-x");
    if (m.m22==1) symstrY.append("+y");
    if (m.m22==-1) symstrY.append("-y");
    if (m.m32==1) symstrY.append("+z");
    if (m.m32==-1) symstrY.append("-z");

    if (m.m13==1) symstrZ.append("+x");
    if (m.m13==-1) symstrZ.append("-x");
    if (m.m23==1) symstrZ.append("+y");
    if (m.m23==-1) symstrZ.append("-y");
    if (m.m33==1) symstrZ.append("+z");
    if (m.m33==-1) symstrZ.append("-z");
    t=cell.trans.at(s);
    V3 zaehler,nenner;
    double egal;
    for (int g=1;g<7; g++){
      nenner.x=(t.x<0)?-g:g;;
      zaehler.x=(int)round(((t.x+h)*g));//+0.0001
      if (fabs(modf(t.x*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.y=(t.y<0)?-g:g;
      zaehler.y=(int)round(((t.y+k)*g));//+0.001
      if (fabs(modf(t.y*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.z=(t.z<0)?-g:g;
      zaehler.z=(int)round(((t.z+l)*g));//+0.001
      if (fabs(modf(t.z*g,&egal))<0.05) break;
    }

    erg=(QString("&raquo;%1 :<b> %2/%3%4, %5/%6%7, %8/%9%10</b><br>")
                    .arg(j)
                    .arg(zaehler.x)
                    .arg(nenner.x)
                    .arg(symstrX)
                    .arg(zaehler.y)
                    .arg(nenner.y)
                    .arg(symstrY)
                    .arg(zaehler.z)
                    .arg(nenner.z)
                    .arg(symstrZ));

  erg.remove(QRegExp("0/\\d"));
  erg.replace("1/1","1");
  erg.replace("2/1","2");
  erg.replace("3/1","3");
  erg.replace("4/1","4");
  erg.replace("5/1","5");
  erg.replace("6/1","6");
  return erg;
}

QString Molecule::symmcode2human(QStringList brauchSymm){
  /*! @param brauchSymm list of internal symmetry codes 
   *  \returns human readable string sepearated by <br>
   */
  QString erg;
  Matrix m;
  V3 t;
  int h,k,l,s;
  for (int j=0; j<brauchSymm.size();j++){
    sscanf(brauchSymm[j].toLatin1(),"%d_%1d%1d%1d",&s,&h,&k,&l);
    h-=5;
    k-=5;
    l-=5;
    s--;
    m=cell.symmops.at(s);
    QString symstrX,symstrY,symstrZ;
    if (m.m11==1) symstrX.append("+x");
    if (m.m11==-1) symstrX.append("-x");
    if (m.m21==1) symstrX.append("+y");
    if (m.m21==-1) symstrX.append("-y");
    if (m.m31==1) symstrX.append("+z");
    if (m.m31==-1) symstrX.append("-z");

    if (m.m12==1) symstrY.append("+x");
    if (m.m12==-1) symstrY.append("-x");
    if (m.m22==1) symstrY.append("+y");
    if (m.m22==-1) symstrY.append("-y");
    if (m.m32==1) symstrY.append("+z");
    if (m.m32==-1) symstrY.append("-z");

    if (m.m13==1) symstrZ.append("+x");
    if (m.m13==-1) symstrZ.append("-x");
    if (m.m23==1) symstrZ.append("+y");
    if (m.m23==-1) symstrZ.append("-y");
    if (m.m33==1) symstrZ.append("+z");
    if (m.m33==-1) symstrZ.append("-z");
    t=cell.trans.at(s);
    V3 zaehler,nenner;
    double egal;
    for (int g=1;g<7; g++){
      nenner.x=(t.x<0)?-g:g;;
      zaehler.x=(int)round((t.x+h)*g);
      if (fabs(modf(t.x*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.y=(t.y<0)?-g:g;
      zaehler.y=(int)round((t.y+k)*g);
      if (fabs(modf(t.y*g,&egal))<0.05) break;
    }
    for (int g=1;g<7; g++){
      nenner.z=(t.z<0)?-g:g;
      zaehler.z=(int)round((t.z+l)*g);
      if (fabs(modf(t.z*g,&egal))<0.05) break;
    }
    erg.append(QString("&raquo;%1 :<b> %2/%3%4, %5/%6%7, %8/%9%10</b><br>")
                    .arg(j+1)
		    .arg(zaehler.x)
		    .arg(nenner.x)
		    .arg(symstrX)
		    .arg(zaehler.y)
		    .arg(nenner.y)
		    .arg(symstrY)
		    .arg(zaehler.z)
		    .arg(nenner.z)
		    .arg(symstrZ));

  }
  erg.remove(QRegExp("0/\\d"));
  erg.replace("1/1","1");
  erg.replace("2/1","2");
  erg.replace("3/1","3");
  erg.replace("4/1","4");
  erg.replace("5/1","5");
  erg.replace("6/1","6");
  return erg;
}

void Molecule::frac2kart(V3 x, V3 &y){
  /*!
   * converts a fractional coordinate into a cartesian using cell information.
   * @param[in] x fractional coordinate.
   * @param[out] y cartesian coordinate.
   */
  Matrix u;
  u.m11 = cell.a;
  u.m21 = 0.0;
  u.m31 = 0.0;
  u.m12 = cell.b * cell.cosga;
  u.m22 = cell.b * cell.singa;
  u.m32 = 0.0;
  u.m13 = cell.c * cell.cosbe;
  u.m23 = cell.tau;
  u.m33 = cell.c * cell.phi / cell.singa;

  y.x = x.x * u.m11 + x.y * u.m12 + x.z * u.m13;
  y.y = x.x * u.m21 + x.y * u.m22 + x.z * u.m23;
  y.z = x.x * u.m31 + x.y * u.m32 + x.z * u.m33;

}

void Molecule::kart2frac(V3 x, V3 &y){
  /*!
   * converts a cartesian coordinate into a fractional using cell information.
   * @param[in] x cartesian coordinate.
   * @param[out] y fractional coordinate.
   */
  Matrix u;
  y.x =0.0 ;
  y.y =0.0 ;
  y.z =0.0;
  u.m11 = 1.0/cell.a;
  u.m21 = 0.0;
  u.m31 = 0.0;
  u.m12 = -1.0/(cell.a * cell.tanga);
  u.m22 = 1.0/(cell.b * cell.singa);
  u.m32 = 0.0;
  u.m13 = (cell.cosal*cell.cosga-cell.cosbe)/(cell.a*cell.phi*cell.singa);
  u.m23 = (cell.cosbe*cell.cosga-cell.cosal)/(cell.b*cell.phi*cell.singa);
  u.m33 = cell.singa/(cell.c*cell.phi);
  // Wird jetzt hier genauso wie in int Tab B S.345 gerechnet (M^-1^).
  y.x = x.x * u.m11 + x.y * u.m12 + x.z * u.m13;
  y.y = x.x * u.m21 + x.y * u.m22 + x.z * u.m23;
  y.z = x.x * u.m31 + x.y * u.m32 + x.z * u.m33;
}

QString Molecule::pse(int oz){

  static QStringList PSE;

  PSE<<"H"<<"He"<<"Li"<<"Be"<<"B"<<"C"<<"N"<<"O"<<"F"<<"Ne"<<"Na"<<"Mg"<<"Al"<<"Si"<<"P"<<"S"<<"Cl"<<"Ar"<<
	  "K"<<"Ca"<<"Sc"<<"Ti"<<"V"<<"Cr"<<"Mn"<<"Fe"<<"Co"<<"Ni"<<"Cu"<<"Zn"<<"Ga"<<"Ge"<<"As"<<"Se"<<"Br"<<"Kr"<<
          "Rb"<<"Sr"<<"Y"<<"Zr"<<"Nb"<<"Mo"<<"Tc"<<"Ru"<<"Rh"<<"Pd"<<"Ag"<<"Cd"<<"In"<<"Sn"<<"Sb"<<"Te"<<"I"<<"Xe"<<
	  "Cs"<<"Ba"<< "La"<<"Ce"<<"Pr"<<"Nd"<<"Pm"<<"Sm"<<"Eu"<<"Gd"<<"Tb"<<"Dy"<<"Ho"<<"Er"<<"Tm"<<"Yb"<<"Lu"<<
	  "Hf"<<"Ta"<<"W"<<"Re"<<"Os"<<"Ir"<<"Pt"<<"Au"<<"Hg"<<"Tl"<<"Pb"<<"Bi"<<"Po"<<"At"<<"Rn"<<"Fr"<<"Ra"<<
	  "Ac"<<"Th"<<"Pa"<<"U"<<"Np"<<"Pu"<<"Am"<<"Cm"<<"Bk"<<"Cf"<<"Es"<<"Fm"<<"Md"<<"No"<<"Lr"<<"Ku"<<"Ha"<<"Rf"<<"Ns"<<"Hs"<<"Mt";
  if ((oz>-1)&&(oz<PSE.size())) return PSE.at(oz);
  return "";

}

int Molecule::getOZ(QString S1){
  QString s=S1.section(QRegExp("[^A-Za-z]"),0,0);
  s=s.toUpper();
  static QStringList PSE;
  if (s=="CNT") return -66;
  if (s=="D") return 0;
  PSE<<"H"<<"HE"<<"LI"<<"BE"<<"B"<<"C"<<"N"<<"O"<<"F"<<"NE"<<"NA"<<"MG"<<"AL"<<"SI"<<"P"<<"S"<<"CL"<<"AR"<<
	  "K"<<"CA"<<"SC"<<"TI"<<"V"<<"CR"<<"MN"<<"FE"<<"CO"<<"NI"<<"CU"<<"ZN"<<"GA"<<"GE"<<"AS"<<"SE"<<"BR"<<"KR"<<
          "RB"<<"SR"<<"Y"<<"ZR"<<"NB"<<"MO"<<"TC"<<"RU"<<"RH"<<"PD"<<"AG"<<"CD"<<"IN"<<"SN"<<"SB"<<"TE"<<"I"<<"XE"<<
	  "CS"<<"BA"<< "LA"<<"CE"<<"PR"<<"ND"<<"PM"<<"SM"<<"EU"<<"GD"<<"TB"<<"DY"<<"HO"<<"ER"<<"TM"<<"YB"<<"LU"<<
	  "HF"<<"TA"<<"W"<<"RE"<<"OS"<<"IR"<<"PT"<<"AU"<<"HG"<<"TL"<<"PB"<<"BI"<<"PO"<<"AT"<<"RN"<<"FR"<<"RA"<<
	  "AC"<<"TH"<<"PA"<<"U"<<"NP"<<"PU"<<"AM"<<"CM"<<"BK"<<"CF"<<"ES"<<"FM"<<"MD"<<"NO"<<"LR"<<"KU"<<"HA"<<"RF"<<"NS"<<"HS"<<"MT";

  return PSE.indexOf(s);
}

void Molecule::uniqueInCell(){
  /*! Moves centers of gravity of each fragment into the unit cell box and close to the origin.
   * */
  QList<int> flags;
  int toflipp=0;
  for (int i=0; i<asymm.size(); i++) if (asymm.at(i).an>-1){
      flags.append(-1);
      toflipp++;
  }else flags.append(0);
  flags[0]=1;
  double dk;
  V3 prime,dp,D,floorD;
//  V3 D,floorD;
  QList<V3> minMol;
  for (int i=0; i<maxmol; i++) minMol.append(V3(0,0,0));
  SdmItem sdmItem;
  sdmItem.a1=0;
  sdmItem.a2=1;
  sdmItem.sn=0;
  sdmItem.covalent=true;
  QList<SdmItem> starter;
  QList<int> mii;
  for (int j=0; j<maxmol; j++){
    int mi=0;
    for (int i=0; i<asymm.size(); i++){
      if ((asymm.at(i).molindex-1)!=j) continue;
      minMol[j]+=asymm.at(i).frac;
      mi++;
    }
    mii.append(mi);
    minMol[j]*=1.0/mi;
  }
  for (int j=0; j<maxmol; j++){
    if (mii.at(j)==0){
      sdmItem.sn=0;
      sdmItem.floorD=V3(floor(minMol.at(j).x),floor(minMol.at(j).y),floor(minMol.at(j).z));
      sdmItem.a2=sdmItem.a1=j;
      starter.append(sdmItem);
    }else{
    double min=99999.0;
      for (int n=0;n<cell.symmops.size();  n++){
	prime=cell.symmops.at(n) * minMol.at(j) + cell.trans.at(n);
        D=prime + V3(0.05,0.05,0.05) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
        dp=D - floorD - V3(0.5,0.5,0.5);
	dk=fl(dp.x,dp.y,dp.z);
	if (dk<min){
	  sdmItem.d=dk;
	  min=dk;
	  sdmItem.floorD=floorD;
	  sdmItem.a1=j;
	  sdmItem.a2=j;
	  sdmItem.sn=n;
	}
      }
    starter.append(sdmItem);
  }
  }
  for (int i=0; i<asymm.size(); i++){
    if (asymm.at(i).molindex<1) continue;
    int n=  starter.at(asymm.at(i).molindex-1).sn;
    V3 f=starter.at(asymm.at(i).molindex-1).floorD;
    asymm[i].frac= cell.symmops.at(n) * asymm.at(i).frac + cell.trans.at(n) - f;
    frac2kart(asymm[i].frac,asymm[i].pos);
  
  }
  packer(sdmcompleter());
  showatoms.clear();
  for (int i=0; i<asymm.size();i++){
    showatoms.append(asymm[i]);
    showatoms[i].molindex=asymm[i].molindex;
  }
  showbonds.clear();
  showbonds=connecting(showatoms);
}

void Molecule::enviSDM(double range){
  /*! Calculates the shortes distance matrix for the ENVIronment functionality in the given range.
   * @param range maximal distances around each atom to generate a matrix entry. 
   */

  // George Sheldrick Seminar ideas
  double dk,dddd;
  V3 prime,dp,D,floorD;
  envi_sdm.clear();
  SdmItem sdmItem;
  sdmItem.a1=0;
  sdmItem.a2=1;
  sdmItem.sn=0;
  //sdmItem.p1=sdmItem.p2=V3(0,0,0);
  for (int i=0; i<asymm.size(); i++){ 
    for (int j=0; j<asymm.size(); j++ ){
    //  bool hma=false;
      for (int n=0;n<cell.symmops.size();  n++){
	prime=cell.symmops.at(n) * asymm.at(i).frac + cell.trans.at(n);
	D=prime - asymm.at(j).frac+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	for (int h=-1; h<2; h++){
	  for (int k=-1; k<2; k++){
	    for (int l=-1; l<2; l++){
	      V3 fD=floorD+V3(h,k,l);  
	      dp=D - fD - V3(0.5,0.5,0.5);
	      dk=fl(dp.x,dp.y,dp.z);
	      if ((dk>0.01)&&((range)>=dk)){
		sdmItem.d=dk;
		sdmItem.floorD=fD;
		sdmItem.a1=i;
		sdmItem.a2=j;
		sdmItem.sn=n;
		if ((asymm[i].an>-1)&&(asymm[j].an>-1)&&
				((asymm[i].part*asymm[j].part==0)||
				 (asymm[i].part==asymm[j].part)))
		  dddd=(Kovalenz_Radien[asymm[i].an]+ Kovalenz_Radien[asymm[j].an])*0.012;
		else dddd=0;
		if (sdmItem.d<dddd){
		  sdmItem.covalent=true;
		}else sdmItem.covalent=false;
		envi_sdm.append(sdmItem);
	      }
	    }
	  }
	}
      }
    }
  }
  qSort(envi_sdm.begin(),envi_sdm.end());
  //histogram
  /*
  double hmi,hma;
  hmi=envi_sdm.first().d;
  hma=envi_sdm.last().d;
  double hstep=(hma-hmi)/20.0;
  printf("%g %g %g %d\n",hma,hmi,hstep,envi_sdm.size());
  if (hstep<0.01) return;
  int hbox[20]={
      0,0,0,0,0,
      0,0,0,0,0,
      0,0,0,0,0,
      0,0,0,0,0};
  for (int i=0; i<envi_sdm.size();i++){
  int hind=(int) (envi_sdm.at(i).d/hstep);
  hbox[hind]++;
  }
  for (int i=0;i<20;i++){
    printf("%g-%g %d  (%g)\n",hmi+hstep*i,hmi+i*hstep+hstep,hbox[i],hma);
  }
// */
}


QStringList Molecule::sdmcompleter(){
  /*! Calculates the shortest distance matrix. 
   * Counts the number of fragmets in the asymmetric unit. 
   * Finds possible hydrogen bond contacts.
   * \returns internal symmtry code list for grown structures.
   */
  // George Sheldrick Seminar ideas
//  printf("sdm1 %d\n",__LINE__);
//  QTime zeit; zeit.start();
  double dk,dddd;
  V3 prime,dp,D,floorD;
  contact.clear();
  sdm.clear();
  SdmItem sdmItem;
  sdmItem.a1=0;
  sdmItem.a2=0;
  sdmItem.sn=0;
  SdmItem sdmItem2;
  sdmItem2.a1=0;
  sdmItem2.a2=0;
  sdmItem2.sn=0;

  knoepfe.clear();
  Knopf kn;
  kn.neighbors.clear();
  for (int i=0; i<asymm.size(); i++){
    kn.neighbors.clear();
    for (int j=0; j<asymm.size(); j++ ){
      double min=1000000;
      bool hma=false;
      for (int n=0;n<cell.symmops.size();  n++){
      //for (int n=0;n<1;  n++){
	prime=cell.symmops.at(n) * asymm.at(i).frac + cell.trans.at(n);
	D=prime - asymm.at(j).frac+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
        dk=fl(dp.x,dp.y,dp.z);
        if (n) dk+=0.0001;
        if ((dk>0.01)&&((min+0.00)>=dk)){
	  min=fmin(dk,min);
	  sdmItem.d=min;
	  sdmItem.floorD=floorD;
	  sdmItem.a1=i;
	  sdmItem.a2=j;
	  sdmItem.sn=n;
	  hma=true;
	}
      }
      if ((asymm[sdmItem.a1].an>-1)&&(asymm[sdmItem.a2].an>-1)&&
((asymm[sdmItem.a1].part*asymm[sdmItem.a2].part==0)||
(asymm[sdmItem.a1].part==asymm[sdmItem.a2].part)))
		dddd=(Kovalenz_Radien[asymm[sdmItem.a1].an]+ Kovalenz_Radien[asymm[sdmItem.a2].an])*0.012;
      else dddd=0;
      if (sdmItem.d<dddd){
          if (hma) kn.neighbors.append(j);
	sdmItem.covalent=true;
      }else sdmItem.covalent=false;
      if (hma) sdm.append(sdmItem);
    }
    knoepfe.append(kn);
  }
//  printf("sdm2 %d\n",zeit.elapsed()); zeit.restart();
  for (int i=0; i<asymm.size(); i++){ //wasserstoffbrueckensuche
    for (int j=0; j<asymm.size(); j++ ){
      for (int nn=0;nn<cell.symmops.size();  nn++){
          prime=cell.symmops.at(nn) * asymm.at(i).frac + cell.trans.at(nn);
          D=prime - asymm.at(j).frac+ V3(0.5,0.5,0.5) ;
          floorD=V3(floor(D.x),floor(D.y),floor(D.z));
          dp=D - floorD - V3(0.5,0.5,0.5);
          dk=fl(dp.x,dp.y,dp.z);
          if((1)&&(abs(asymm[i].an-7)<2)&&(abs(asymm[j].an-7)<2)&&(fabs(dk-2.725)<0.275)){//N O F mit abstanden zwischen 2.45 bis 3 A koennten H=Bruecken sein
              sdmItem2.d=dk;
              sdmItem2.floorD=floorD;
              sdmItem2.a1=i;
              sdmItem2.a2=j;
              sdmItem2.sn=nn;
              sdmItem2.covalent=true;
              double ang1=361,ang2=361,an;
              for (int oo=0; oo<knoepfe.at(i).neighbors.size(); oo++){
                  an=wl(asymm.at(j).frac,
                            cell.symmops.at(nn) * asymm.at(i).frac + cell.trans.at(nn)-floorD,
                            cell.symmops.at(nn) * asymm.at(knoepfe.at(i).neighbors.at(oo)).frac + cell.trans.at(nn)-floorD);
            ang1=(an<ang1)?an:ang1;
          }
          for (int oo=0; oo<knoepfe.at(j).neighbors.size(); oo++){
            an=wl(
                            cell.symmops.at(nn) * asymm.at(i).frac  + cell.trans.at(nn)-floorD,
                            asymm.at(j).frac,
                            asymm.at(knoepfe.at(j).neighbors.at(oo)).frac);
            ang2=(an<ang2)?an:ang2;
          }
          if ((qMax(ang1,ang2)!=361)&&((qMin(ang1,ang2)<90)||(qMax(ang1,ang2)>180))) sdmItem2.covalent=false;
          else {
            contact.append(sdmItem2);
          }
        }
      }
    }
  }
 // printf("sdm3 %d\n",zeit.elapsed());//\n"); zeit.restart();
  QString bs;
  QStringList brauchSymm;
  int someleft=0,nextmol=0;
    maxmol=1;
    for (int i=0; i<asymm.size();i++) asymm[i].molindex=-1;
    asymm[0].molindex=1;//starter;
    do {
      nextmol=0;
      do {
	someleft=0;
	for  (int i=0; i<sdm.size();i++){
	  if ((sdm.at(i).covalent)&&(asymm[sdm.at(i).a1].molindex*asymm[sdm.at(i).a2].molindex<0)) {
	    asymm[sdm.at(i).a1].molindex=maxmol;
            asymm[sdm.at(i).a2].molindex=maxmol;
	    someleft++;
	  }
	}
      }while (someleft);
      for ( int i=0; i<asymm.size();i++) {
	if ((asymm[i].an>-1)&&(asymm[i].molindex<0)) {nextmol=i;break;}
      }
      if (nextmol) {
        asymm[nextmol].molindex=(++maxmol);
       }
    }while (nextmol);
    Fragments=QString("<b>The asymmetric unit contains %1 fragments.</b><br>").arg(maxmol);
  for (int k =0; k<sdm.size();k++){
    if (sdm.at(k).covalent){
      if (asymm[sdm.at(k).a1].molindex<1) continue;
      if (asymm[sdm.at(k).a1].molindex>6) continue;
      for (int n=0;n<cell.symmops.size();  n++){
	if (((asymm[sdm.at(k).a1].part!=0)&&(asymm[sdm.at(k).a2].part!=0)&&(asymm[sdm.at(k).a1].part!=asymm[sdm.at(k).a2].part)))continue;
	if ((asymm[sdm.at(k).a1].an==asymm[sdm.at(k).a2].an)&&(asymm[sdm.at(k).a1].an==0)) continue;
	prime=cell.symmops.at(n) * asymm[sdm.at(k).a1].frac + cell.trans.at(n);
	D=prime - asymm[sdm.at(k).a2].frac+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
	if ((n==0)&&(V3(0,0,0)==floorD)) continue;
        dk=fl(dp.x,dp.y,dp.z);
	dddd=(sdm.at(k).d+0.2);
	if ( (dk>0.001)&&(dddd>=dk)) {
          bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm[sdm.at(k).a1].molindex);
	  if  (!brauchSymm.contains(bs)) {
	    brauchSymm.append(bs);
	  }
	}
      }
    }
  }
//  printf("sdm4 %d\n",zeit.elapsed()); zeit.restart();
  qSort(sdm.begin(),sdm.end());
//  printf("sdm5 %d\n",zeit.elapsed());//\n"); zeit.restart();
  enviSDM(2.5);
//  printf("sdm5 %d\n",zeit.elapsed());//\n"); zeit.restart();

  QList<int> flags;
    for (int i=0; i<asymm.size(); i++) flags.append((asymm.at(i).an<0)?-1:1);
  for (int i=0; i<envi_sdm.size(); i++)
    if ((flags.at(envi_sdm.at(i).a1)*flags.at(envi_sdm.at(i).a2))==-1) {
      //if ((qbeforehkl)||((flags.at(sdm.at(i).a1)==-1)&&((asymm.at(sdm.at(i).a2).an>-1)))){
     //   if (((asymm[envi_sdm.at(i).a1].Label=="Q11")||(asymm[envi_sdm.at(i).a2].Label=="Q11"))&&(envi_sdm.at(i).d<2.6))
     //       qDebug()<<asymm[envi_sdm.at(i).a1].Label<<asymm[envi_sdm.at(i).a2].Label<<envi_sdm.at(i).d<<flags[envi_sdm.at(i).a1]<<flags[envi_sdm.at(i).a2];
	if (asymm[envi_sdm.at(i).a1].an>-1) continue;
    if (asymm[envi_sdm.at(i).a1].an==-66) continue;
	if ((envi_sdm.at(i).sn==0)&&(envi_sdm.at(i).floorD==V3(0,0,0))) {flags[envi_sdm.at(i).a1]=1;continue;}
        if (envi_sdm.at(i).d>2.4) continue;
        asymm[envi_sdm.at(i).a1].frac = cell.symmops.at(envi_sdm.at(i).sn) * asymm[envi_sdm.at(i).a1].frac + cell.trans.at(envi_sdm.at(i).sn) - envi_sdm.at(i).floorD;
	flags[envi_sdm.at(i).a1]=1;
        frac2kart(asymm[envi_sdm.at(i).a1].frac,asymm[envi_sdm.at(i).a1].pos);
      //}
    }
//  printf("sdm6 %d\n",zeit.elapsed()); zeit.restart();
  return brauchSymm;
}

/*
  qDebug()<<neededSymm.size()<<ss;
// */

Connection Molecule::connecting(const CEnvironment &atom,bool eac){
  /*! Creates bonds and lbonds depending on covalent radii.
   * If edit atom style flag (eac) is not set FREE and BIND instructions are considered.
   * Feeds also Molecule.knoepfe list.
   * \returns the Connection bond list.
   * */
  Connection bonds;
  bonds.clear();
  lbonds.clear();
  MyBond bond;
  double soll=0;
  for (int i=0; i<atom.size(); i++){
    for (int j=0; j<=i; j++ ){
      double d = sqrt(Distance(atom.at(i).pos,atom.at(j).pos));
      if ((atom.at(i).part<0)&&(atom.at(j).part!=0)&&(atom.at(i).symmGroup!=atom.at(j).symmGroup)) continue;
      if ((atom.at(j).part<0)&&(atom.at(i).part!=0)&&(atom.at(i).symmGroup!=atom.at(j).symmGroup)) continue;
      if ((atom.at(i).an==0)&&(atom.at(j).an==0)) continue;
      if ((atom.at(i).an>-1)&&(atom.at(j).an>-1)&&
          ((!atom.at(i).part)||(!atom.at(j).part)||(atom.at(i).part==atom.at(j).part)))
        soll=(Kovalenz_Radien[atom.at(i).an]+ Kovalenz_Radien[atom.at(j).an])*0.012;
      else
        soll=-1;
      if ((d>0.1)&&(d<soll)) {
        bond.ato1=&atom[i];
        bond.ato2=&atom[j];
        bond.a1=i;
        bond.a2=j;
        bond.length=d;
        bonds.append(bond);
      }
      else if ((d<qboMax)&&(d>qboMin)){
      if ((atom.at(i).an==0)||(atom.at(j).an==0)) continue;
        if  ((!atom.at(i).part)||(!atom.at(j).part)||(atom.at(i).part==atom.at(j).part)){ 
        bond.ato1=&atom[i];
        bond.ato2=&atom[j];
        bond.a1=i;
        bond.a2=j;
        bond.length=d;
        lbonds.append(bond);
	}
      }
    }
  }
  if (!eac) for (int i=0; i<freeatoms.size(); i++){
    MyAtom a1, a2;
    bool bo=false;
    if (freeatoms.at(i).Lab1.contains('_')) a1.resiNr=freeatoms.at(i).Lab1.section('_',1,1).toInt(&bo);
    if (!bo) a1.resiNr=-1;
    if (freeatoms.at(i).Lab2.contains('_'))a2.resiNr=freeatoms.at(i).Lab2.section('_',1,1).toInt(&bo);
    if (!bo)a2.resiNr=-1;
    a1.Label = freeatoms.at(i).Lab1.section('_',0,0);
    a2.Label = freeatoms.at(i).Lab2.section('_',0,0);
    //printf("#%d %d\n",a2.resiNr,a1.resiNr);

    for (int j=0;j<bonds.size();j++){
      if (((a1.resiNr>-1)&& (bonds.at(j).ato1->resiNr!=a1.resiNr))|| 
       ((a2.resiNr>-1)&& (bonds.at(j).ato2->resiNr!=a2.resiNr))) continue;
      if (((a1.resiNr>-1)&& (bonds.at(j).ato2->resiNr!=a1.resiNr))|| 
       ((a2.resiNr>-1)&& (bonds.at(j).ato1->resiNr!=a2.resiNr))) continue;
      if (((bonds.at(j).ato1->Label.section('_',0,0).toUpper()== a1.Label)&&
                              (bonds.at(j).ato2->Label.section('_',0,0).toUpper()== a2.Label))||
                      ((bonds.at(j).ato2->Label.section('_',0,0).toUpper()== a1.Label)&&
                       (bonds.at(j).ato1->Label.section('_',0,0).toUpper()== a2.Label))){
	bonds.removeAt(j);
        j=qMax(j-1,0);
      }
    }
  }
  if (!eac) for (int i=0; i<bindatoms.size(); i++){
    MyAtom a1, a2;
    bool bo=false;
    int sy1=-1,sy2=-1;
    if (bindatoms.at(i).Lab1.contains("_$"))sy1=bindatoms.at(i).Lab1.section("_$",1,1).toInt(&bo);
    if (bindatoms.at(i).Lab2.contains("_$"))sy2=bindatoms.at(i).Lab2.section("_$",1,1).toInt(&bo);
    bo=false;
    if (bindatoms.at(i).Lab1.contains('_'))a1.resiNr=bindatoms.at(i).Lab1.section('_',1,1).toInt(&bo);
    if (!bo) a1.resiNr=-1;
    if (bindatoms.at(i).Lab2.contains('_'))a2.resiNr=bindatoms.at(i).Lab2.section('_',1,1).toInt(&bo);
    if (!bo)a2.resiNr=-1;
    a1.Label = bindatoms.at(i).Lab1.section('_',0,0);
    a2.Label = bindatoms.at(i).Lab2.section('_',0,0);
    //printf("#%d %d\n",a2.resiNr,a1.resiNr);
    double mindd=50.0;
   // if (qMax(sy1,sy2)>-1) 
    {
      for (int iii=0; iii<atom.size(); iii++){
	for (int jjj=0; jjj<atom.size(); jjj++){
//	  qDebug()<<eqivMap.value(QString("$%1").arg(sy2));
	  if ((a1.resiNr>-1)&& (atom.at(iii).resiNr!=a1.resiNr)) continue;
	  if ((a2.resiNr>-1)&& (atom.at(jjj).resiNr!=a2.resiNr)) continue;
	  if ((sy1>-1)&&(atom.at(iii).symmGroup==0)) continue;
	  if ((sy2>-1)&&(atom.at(jjj).symmGroup==0)) continue;
	  if ((sy1>-1)&&(eqivMap.value(QString("$%1").arg(sy1))!=atom.at(iii).symmGroup)) continue;
	  if ((sy2>-1)&&(eqivMap.value(QString("$%1").arg(sy2))!=atom.at(jjj).symmGroup)) continue;
	  if ((atom.at(iii).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper() == a1.Label.toUpper())&&
			  (atom.at(jjj).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper() == a2.Label.toUpper())){
	    bond.ato1=&atom[iii];
	    bond.ato2=&atom[jjj];
	    bond.a1=iii;
	    bond.a2=jjj;
	    bond.length=sqrt(Distance(atom[iii].pos,atom[jjj].pos));
	    if ((mindd+0.01)>=bond.length) bonds.append(bond);
	    mindd=qMin(bond.length, mindd);

	  }
	  }
	}
      }
/*    else 
    for (int j=0; j<sdm.size(); j++){
      if ((a1.resiNr>-1)&& (asymm[sdm.at(j).a1].resiNr!=a1.resiNr)) continue;
      if ((a2.resiNr>-1)&& (asymm[sdm.at(j).a2].resiNr!=a2.resiNr)) continue;
      if ((sy1>-1)&&(atom[sdm.at(j).a1].symmGroup==0)) continue;
      if ((sy2>-1)&&(atom[sdm.at(j).a2].symmGroup==0)) continue;
      if ((sy1>-1)&&(eqivMap.value(QString("$%1").arg(sy1))!=atom[sdm.at(j).a1].symmGroup)) continue;
      if ((sy2>-1)&&(eqivMap.value(QString("$%1").arg(sy2))!=atom[sdm.at(j).a2].symmGroup)) continue;
      if ((asymm[sdm.at(j).a1].Label.section('_',0,0).toUpper() == a1.Label.toUpper())&&
                      (asymm[sdm.at(j).a2].Label.section('_',0,0).toUpper() == a2.Label.toUpper())){
	bond.ato1=&atom[sdm.at(j).a1];
	bond.ato2=&atom[sdm.at(j).a2];
	bond.a1=sdm.at(j).a1;
        bond.a2=sdm.at(j).a2;
	bond.length=sqrt(Distance(atom[sdm.at(j).a1].pos,atom[sdm.at(j).a2].pos));
        //bond.length=sdm.at(j).d;
	//if (sdm.at(j).sn) continue;
        //if (sdm.at(j).floorD==V3(0,0,0)){
        if (mindd>bond.length) bonds.append(bond);
        mindd=qMin(bond.length, mindd);
        //}
      }
    }// */
  for (int j=0; j < bonds.size(); j++){
    if (((bonds.at(j).ato1->Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper()==a1.Label.toUpper())&&
     (bonds.at(j).ato2->Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper()==a2.Label.toUpper()))||
    ((bonds.at(j).ato1->Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper()==a2.Label.toUpper())&&
     (bonds.at(j).ato2->Label.section('_',0,0).section(QString::fromUtf8("»"),0,0).toUpper()==a1.Label.toUpper()))) 
      if (bonds.at(j).length>(mindd+0.1)) {
	bonds.removeAt(j);
	if (j) j--;
      }
  }
  }
  //-->Verknopfung   !!!
  knoepfe.clear();
  Knopf kn;
  kn.neighbors.clear();
  for (int i=0; i<asymm.size(); i++){
    kn.neighbors.clear();
    for (int j=0; j<bonds.size(); j++){
      if (bonds.at(j).a1==i){
      kn.neighbors.append(bonds.at(j).a2);
      }
      if (bonds.at(j).a2==i){
      kn.neighbors.append(bonds.at(j).a1);
      }
    }
    knoepfe.append(kn);  
  }

  
  return bonds;
}

double Molecule::fl(double x,double y,double z){
  double a,b,c;
  a = (cell.ga==90.0)?0.0:2.0*x*y*cell.a*cell.b*cell.cosga;
  b = (cell.be==90.0)?0.0:2.0*x*z*cell.a*cell.c*cell.cosbe;
  c = (cell.al==90.0)?0.0:2.0*y*z*cell.b*cell.c*cell.cosal;
  double erg=sqrt(x*x*cell.a*cell.a+
		  y*y*cell.b*cell.b+
		  z*z*cell.c*cell.c+
		  a+b+c);
  return erg;
}

double Molecule::wl(V3 p2, V3 p1, V3 p3){
  V3 f1=p2-p1;//12
  V3 f2=p3-p1;//13
  V3 f3=p3-p2;//23
  double a=0,b=0,c=0;
  a =  (cell.ga==90.0)?0.0:2.0*f1.x*f1.y*cell.a*cell.b*cell.cosga;
  a += (cell.be==90.0)?0.0:2.0*f1.x*f1.z*cell.a*cell.c*cell.cosbe;
  a += (cell.al==90.0)?0.0:2.0*f1.y*f1.z*cell.b*cell.c*cell.cosal;

  b =  (cell.ga==90.0)?0.0:2.0*f2.x*f2.y*cell.a*cell.b*cell.cosga;
  b += (cell.be==90.0)?0.0:2.0*f2.x*f2.z*cell.a*cell.c*cell.cosbe;
  b += (cell.al==90.0)?0.0:2.0*f2.y*f2.z*cell.b*cell.c*cell.cosal;

  c = ( cell.ga==90.0)?0.0:2.0*f3.x*f3.y*cell.a*cell.b*cell.cosga;
  c += (cell.be==90.0)?0.0:2.0*f3.x*f3.z*cell.a*cell.c*cell.cosbe;
  c += (cell.al==90.0)?0.0:2.0*f3.y*f3.z*cell.b*cell.c*cell.cosal;
  double d12,d13,d23;
  d12 = f1.x*f1.x*cell.a*cell.a+
	f1.y*f1.y*cell.b*cell.b+
        f1.z*f1.z*cell.c*cell.c+a;	
  d13 = f2.x*f2.x*cell.a*cell.a+
	f2.y*f2.y*cell.b*cell.b+
        f2.z*f2.z*cell.c*cell.c+b;	
  d23 = f3.x*f3.x*cell.a*cell.a+
	f3.y*f3.y*cell.b*cell.b+
        f3.z*f3.z*cell.c*cell.c+c;
  double erg=acos((d12+d13-d23)/(2.0*sqrt(d12)*sqrt(d13)))*g2r;
  return erg;
}

void Molecule::cellSetup(){
  cell.cosal=cos(cell.al/g2r);
  cell.cosbe=cos(cell.be/g2r);
  cell.cosga=cos(cell.ga/g2r);
  cell.singa=sin(cell.ga/g2r);
  cell.tanga=tan(cell.ga/g2r);
  cell.phi=  sqrt(1-(cell.cosal*cell.cosal)-
		  (cell.cosbe*cell.cosbe)-(cell.cosga*cell.cosga)
                  +2*cell.cosal*cell.cosbe*cell.cosga);
  cell.tau=cell.c *((cell.cosal- cell.cosbe* cell.cosga)/ cell.singa);
  cell.V = cell.a*cell.b*cell.c*cell.phi;
  cell.as=cell.c*cell.b*sin(cell.al/g2r)/cell.V;
  cell.bs=cell.c*cell.a*sin(cell.be/g2r)/cell.V;
  cell.cs=cell.a*cell.b*cell.singa/cell.V;
}

void Molecule::applyLatticeCentro(int gitter){
  /*! Adds centrings and inversion symmetry to the symmops and trans list. 
   * @param gitter the parameter of the shelx LATT instruction. ==> see Shelxl manual.
   */
  int z=cell.symmops.size();
  Matrix inv(-1.0,0.0,0.0, 0.0,-1.0,0.0, 0.0,0.0,-1.0);  
  cell.centeric=false;
  if (gitter>0){ 
    for (int i=0; i<z;i++){
      Matrix m=cell.symmops.at(i)*inv;
      cell.symmops.append(m);
      cell.trans.append(cell.trans.at(i));
    }
  cell.centeric=true;
  }
  gitter=(gitter>0)?gitter:-gitter;
  z=cell.symmops.size();
  cell.centered=false;
  switch (gitter){
	  case 5 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(0.0, 0.5, 0.5);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;
		  }
		  break;
	  case 6 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(0.5, 0.0, 0.5);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;
		  }
		  break;
	  case 7 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(0.5, 0.5, 0.0);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;
		  }
		  break;
	  case 4 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(0.0, 0.5, 0.5);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    tt = cell.trans.at(i)+V3(0.5, 0.0, 0.5);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    tt = cell.trans.at(i)+V3(0.5, 0.5, 0.0);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;
		  }
		  break;
	  case 2 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(0.5, 0.5, 0.5);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;
		  }
		  break;
	  case 3 :
		  for (int i=0; i<z;i++){
		    V3 tt = cell.trans.at(i)+V3(2.0/3.0, 1.0/3.0, 1.0/3.0);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    tt = cell.trans.at(i)+V3(1.0/3.0, 2.0/3.0, 2.0/3.0);
		    tt.x=(tt.x>1)?tt.x-1:tt.x;
		    tt.y=(tt.y>1)?tt.y-1:tt.y;
		    tt.z=(tt.z>1)?tt.z-1:tt.z;
		    cell.symmops.append(cell.symmops.at(i));
		    cell.trans.append(tt);
		    cell.centered=true;

		  }
		  break;
	  case 0 :break;  
  }

}

double Molecule::dimension(){
  /*! calculates the maximal interatomic distance (Ang) of showatoms environment if this is 0 then 10.0 is returned instead.
   */
  double max=0,gg=0;
  for (int i=0;i<showatoms.size();i++)
    for (int j=i+1;j<showatoms.size();j++){
      if ((showatoms[j].an>=0)&&(showatoms[j].an>=0)) 
	gg=sqrt( Distance(showatoms[i].pos,showatoms[j].pos));
      max=(max<gg)?gg:max;
    }
    if (max==0) return 10.0;
  return max;
}


#define Ato4d(arr)       arr[0], arr[1], arr[2], arr[3]
#include <QGLWidget>
void Molecule::atoms(CEnvironment atom,int proba){
  /*! Draws atoms as spheres or as ellipses cubes or icosahedrons.
   * @params atoms list of atoms to be drawn .
   * @params proba ellipsoid probability (currently 10 30 50 70 90 %).
   *
   * Molecule.dratom controls the wireframe style of the resulting atom .
   *
   * Molecule.tubes atom balls is drawn in bondstrength.
   *
   * Molecule.adp atom is drawn as a ellipsoid. 
   *
   * Molecule.monoQrom Q-Peaks are drawn in Color Molecule.mQolor .
   * */
int myAdp=adp;
int myStyle=7;
int pminus=0;
int wire=0;
GLfloat white[4]={0.2f, 0.2f, 0.3f, 0.0f};
GLfloat black[4]={0.0f, 0.0f, 0.0f, 1.0f};
for  (int i=0; i<atom.size();i++){
if ((!dratom)&&(atom.at(i).hidden)) continue;
if ((nopm1)&&(atom.at(i).symmGroup)&&(atom.at(i).part<0))continue;
if ((dratom==2)&&(atom.at(i).part==0))continue;
if ((dratom==2)&&(atom.at(i).symmGroup)&&(atom.at(i).part<0))continue;
if ((dratom==2)&&(atom.at(i).hidden))continue;
if ((dratom==5)&&(atom.at(i).hidden))continue;
int  nonPositiveDefinite=0;
    glPushMatrix();
    nonPositiveDefinite=0;
    glTranslated(atom.at(i).pos.x,atom.at(i).pos.y,atom.at(i).pos.z);
    double rad=(atom.at(i).an==-2)?0.5:qPeakRad;
    bool parthigh=false;
    if (dratom==5){
      wire=1;
      dratom=0;
    }
    if (dratom==2) {
      dratom=0;
      parthigh=true;
    }
    if (!dratom){
 //     printf("ph %d alpha %g tubes %d,myAdp %d %d\n",parthigh,AtomColor[atom.at(i).an].alphaF(),tubes,myAdp,((!myAdp)||((myStyle&ATOM_STYLE_SPHERE)&&(!(myStyle&ATOM_STYLE_SOLID)))));
     if (atom.at(i).an>-1) {
       myStyle=AtomStyle[atom.at(i).an];
       if (atom.at(i).Label.contains("(noADP)")) myStyle|=ATOM_STYLE_NOADP;
       glColor4d(AtomColor[atom.at(i).an].redF(),
		       AtomColor[atom.at(i).an].greenF(),
		       AtomColor[atom.at(i).an].blueF(),
		       AtomColor[atom.at(i).an].alphaF());
       rad=arad[atom.at(i).an];
       if (parthigh) {
	 QColor pcolor;
	 switch (atom.at(i).part){
		 case 1: pcolor=QColor("lightskyblue");break;
		 case 2: pcolor=QColor("darkgoldenrod");break;
		 case 3: pcolor=Qt::green;break;
		 case 4: pcolor=Qt::yellow;break;
		 case 5: pcolor=QColor("orange");break;
		 case 6: pcolor=Qt::black;break;
		 case 7: pcolor=QColor("lightsalmon");break;
		 case 8: pcolor=QColor("lightseagreen");break;
		 case -1: pcolor=QColor("aquamarine");break;
		 case -2: pcolor=QColor("olive");break;
		 case -3: pcolor=QColor("bisque");break;
                 case 666: pcolor=QColor("fuchsia");break;
		 default: if (atom.at(i).part>0)
			    pcolor=QColor::colorNames().at(qMin(atom.at(i).part,QColor::colorNames().size()-1));
			  else pcolor=Qt::magenta;
	 }
         if (atom.at(i).part==666) pcolor.setAlpha(200);
         else pcolor.setAlpha(140);
	 glColor4d(pcolor.redF(),
			 pcolor.greenF(),
			 pcolor.blueF(),
			 pcolor.alphaF());
	 
       }

                  if ((highlightEquivalents)&&(!parthigh)&&(atom.at(i).symmGroup)) {
                      glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,white );
                    glColor4d(qMin(1.0,(AtomColor[atom.at(i).an].redF()+0.1)   * 1.2),
                                qMin(1.0,(AtomColor[atom.at(i).an].greenF()+0.1) * 1.2),
                                qMin(1.0,(AtomColor[atom.at(i).an].blueF()+0.1)  * 1.2),
                                    1.0);
                  }
                  else glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,black );
                  if ((atom.at(i).symmGroup)&&(atom.at(i).part<0)){
                    glColor4d(AtomColor[atom.at(i).an].redF(),
                                  AtomColor[atom.at(i).an].greenF(),
				  1.0,
				  0.3);
		    pminus=1;
                    myStyle|=ATOM_STYLE_NOADP;
		  }
                myAdp=(myStyle&ATOM_STYLE_NOADP)?0:adp;
    }
     if (wire) {myStyle|=ATOM_STYLE_SPHERE;if (myStyle&ATOM_STYLE_SOLID)myStyle-=ATOM_STYLE_SOLID; }
     if (tubes){
       rad=bondStrength;
       if (atom.at(i).part==666) rad*=4.5;
       myAdp=0;
       nonPositiveDefinite=0;
     }
    if ((atom.at(i).an<0)||(atom.at(i).Label.startsWith('Q',Qt::CaseInsensitive))){
      glColor4d(1.0 ,0.7, 0.8 ,0.4);
      if (atom.at(i).an==-66) {
	rad*=2;
	glColor4d(0.6,0.3,0.1,0.3);
      }
      if((atom.at(i).an==-1)&&(!monoQrom)) Farbverlauf(atom.at(i).peakHeight, pmin, pmax);
      if (monoQrom) glColor3d(mQolor.redF(),mQolor.greenF(),mQolor.blueF());
//      if (!wire)
	ikosa(rad);

    }
    else {
      if (myAdp){
	V3 ev=V3(0,0,0);
	double *arr=jacobi(atom.at(i).uc,ev);
	if ((ev.x>0)&&(ev.y>0)&&(ev.z>0)){
          nonPositiveDefinite=0;
	glRotated(Ato4d(arr));
	double psc=(parthigh)?1.32:1.0;
        if (atom.at(i).part==666) psc*=1.2;
	switch (proba ) {
		case 10 :{ glScaled(0.76*sqrt(ev.x)*psc,0.76*sqrt(ev.y)*psc,0.76*sqrt(ev.z)*psc);break;}   //Hauptachsen der Eliipsoide 10% Wahrscheinlichkeit
		case 30 :{ glScaled(1.19*sqrt(ev.x)*psc,1.19*sqrt(ev.y)*psc,1.19*sqrt(ev.z)*psc);break;}   //Hauptachsen der Eliipsoide 30% Wahrscheinlichkeit
		case 50 :{ glScaled(1.54*sqrt(ev.x)*psc,1.54*sqrt(ev.y)*psc,1.54*sqrt(ev.z)*psc);break;}   //Hauptachsen der Eliipsoide 50% Wahrscheinlichkeit
		case 70 :{ glScaled(1.91*sqrt(ev.x)*psc,1.91*sqrt(ev.y)*psc,1.91*sqrt(ev.z)*psc);break;}   //Hauptachsen der Eliipsoide 70% Wahrscheinlichkeit
		case 90 :{ glScaled(2.50*sqrt(ev.x)*psc,2.50*sqrt(ev.y)*psc,2.50*sqrt(ev.z)*psc);break;}   //Hauptachsen der Eliipsoide 90% Wahrscheinlichkeit
		default: ;
        }
      }else {nonPositiveDefinite=1;}
      }
      
       if (parthigh) {
	 rad*=1.32;
         if (atom.at(i).part==666) rad*=1.2;
         glEnable(GL_BLEND);
         myStyle&=(ATOM_STYLE_SPHERE|ATOM_STYLE_NOADP);
	 myStyle|=ATOM_STYLE_SPHERE;
       }

      /*
#define ATOM_STYLE_WALLS 1
#define ATOM_STYLE_RINGS 2
#define ATOM_STYLE_SPHERE 4
#define ATOM_STYLE_SOLID 8
#define ATOM_STYLE_WHITERING 16
#define ATOM_STYLE_NOLABEL 32
*/
      if (atom.at(i).an>-1) {
        if (myAdp&&intern) {
          if (!nonPositiveDefinite) ellipse(myStyle);
          else dCube(rad);
          }
        else if ((!myAdp)||((myStyle&ATOM_STYLE_SPHERE)&&(!(myStyle&ATOM_STYLE_SOLID)))){
          if (!myAdp) glScaled(rad,rad,rad);
          if (!nonPositiveDefinite) {
	    if (wire)dratom =5;
	    sphere(myAdp|pminus);
	  }
          else dCube(rad);
        }
      }
      else
        ikosa(rad);
    } 
    } 
    else{
        atom[i].hidden=0;
      sphere(0);
    }
//    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,black );     
    glPopMatrix();
    if (parthigh) dratom=2;
  }
}


void Molecule::Farbverlauf (GLfloat wrt,GLfloat min,GLfloat max){
    if (min+0.001>=max)max+=0.002;
  GLclampd ff[4];
  int lauf=0;
  const GLclampd farbe[6][4]={{1.0,0.0,0.0,1.0},
                        {1.0,1.0,0.0,1.0},
                        {0.0,1.0,0.0,1.0},
                        {0.0,1.0,1.0,1.0},
                        {0.0,0.0,1.0,1.0},
                        {1.0,0.0,1.0,1.0}};
  GLfloat nwrt=(wrt-min)/(max-min);
  nwrt=(nwrt>=1.0)?0.99999:nwrt;
  nwrt=(nwrt<=0.0)?0.00001:nwrt;
  lauf=(int (nwrt/0.2));
  nwrt-=(0.2*lauf);
  nwrt/=(0.2);

  ff[0]=(1.0-nwrt)*farbe[lauf][0]+farbe[lauf+1][0]*nwrt;
  ff[1]=(1.0-nwrt)*farbe[lauf][1]+farbe[lauf+1][1]*nwrt;
  ff[2]=(1.0-nwrt)*farbe[lauf][2]+farbe[lauf+1][2]*nwrt;
  ff[3]=1.0;

  glColor4dv(ff);
 return;
}

void Molecule::bonds(Connection bond){ 
 /*! @params bond list of bonds to be drawn
  * draws bonds as cylinders with radius Molecule.bondStrength .
  * Bonds start and stop at the atom surface.
  */ 
  GLfloat white[4]={0.2, 0.2, 0.3, 0.0};
  GLfloat black[4]={0.0, 0.0, 0.0, 1.0};
  glDisable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, hbtex);
  //printf("Doing Bonds adp=%d proba=%d tubes%d\n",adp,proba,tubes);
  V3 vec,bondir;
  double kurz=0,ara1,ara2;
  double wink;
  bool parthigh=false;
  double psc=1.0;
  if (dratom==2) {
    dratom=0;
    parthigh=true;
    psc=1.32;
    glEnable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  for (int i=0; i<bond.size();i++){
      if (bond.at(i).ato1->hidden) continue;
      if (bond.at(i).ato2->hidden) continue;
      if ((parthigh)&&((bond.at(i).ato1->part==0)||(bond.at(i).ato2->part==0))) continue;
      if ((parthigh)&&(((bond.at(i).ato1->part<0)&&(bond.at(i).ato1->symmGroup))||((bond.at(i).ato2->part<0)&&(bond.at(i).ato2->symmGroup)))) continue;
      if ((nopm1)&&(bond.at(i).ato1->symmGroup)&&(bond.at(i).ato1->part<0))continue;
      if ((nopm1)&&(bond.at(i).ato2->symmGroup)&&(bond.at(i).ato2->part<0))continue;
      if ((ATOM_STYLE_METAL&AtomStyle[bond.at(i).ato1->an])!=(ATOM_STYLE_METAL&AtomStyle[bond.at(i).ato2->an])){
	glEnable(GL_TEXTURE_2D);
      } else glDisable(GL_TEXTURE_2D);
    glColor4d(0.5,0.3,0.3,1.0);
    if (adp) {
      V3 hin=Normalize(bond.at(i).ato2->pos-bond.at(i).ato1->pos);
      double r=1;
      switch (proba){
	      case 10:r=0.76;break;
	      case 30:r=1.19;break;
	      case 50:r=1.54;break;
	      case 70:r=1.91;break;
	      case 90:r=2.50;break;
      }      
      ara1=sqrt((hin*bond.at(i).ato1->uc)*hin);
      ara1*=r;
      ara2=sqrt((hin*bond.at(i).ato2->uc)*hin);
      ara2*=r;
//      printf("%s-%s %f %f %d %f\n",bond.at(i).ato1->Label.toStdString().c_str(),bond.at(i).ato2->Label.toStdString().c_str(),ara1, ara2, proba,r);
    }
    else{
      ara1=arad[bond.at(i).ato1->an];
      ara2=arad[bond.at(i).ato2->an];
    }
    if (((bond.at(i).ato1->symmGroup)&&(bond.at(i).ato1->part<0))||
		    (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato1->an])) ara1=arad[bond.at(i).ato1->an];
    if (((bond.at(i).ato2->symmGroup)&&(bond.at(i).ato2->part<0))||
		    (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato2->an])) ara2=arad[bond.at(i).ato2->an];
    if (tubes) ara1=ara2=bondStrength;
    if (bond.at(i).ato2->an>-1) {
        glColor4d(AtomColor[bond.at(i).ato2->an].redF(),
                        AtomColor[bond.at(i).ato2->an].greenF(),
                        AtomColor[bond.at(i).ato2->an].blueF(),
                        AtomColor[bond.at(i).ato2->an].alphaF());
      
    }

    ara1=qMax(ara1,bondStrength);
    ara2=qMax(ara2,bondStrength);

    if ((highlightEquivalents)&&(bond.at(i).ato2->symmGroup|bond.at(i).ato1->symmGroup))
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,white );
    else glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,black );
    if ((bond.at(i).ato2->symmGroup|bond.at(i).ato1->symmGroup)&&((bond.at(i).ato2->part<0)||(bond.at(i).ato1->part<0))){
      //		    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,blue );
      glColor4d(AtomColor[bond.at(i).ato2->an].redF(),
                AtomColor[bond.at(i).ato2->an].greenF(),
		1.0,
		0.3);
      glEnable(GL_BLEND);
    }
    vec.x=(bond.at(i).ato2->pos.y-bond.at(i).ato1->pos.y);
    vec.y=(bond.at(i).ato1->pos.x-bond.at(i).ato2->pos.x);
    vec.z=0;
    if (Norm(vec)) vec=Normalize(vec);
    else vec=V3(0,1,0);
    bondir =V3(0,0,0);
    if (bondStrength<ara2)
      bondir= Normalize(bond.at(i).ato1->pos-bond.at(i).ato2->pos)*(0.9*sqrt((ara2*ara2)-bondStrength*bondStrength));
    kurz=sqrt(Norm(bondir));//  */
    //printf("%s--%s kurz %f len %f ara2 %f\n",bond.at(i).ato1->Label.toStdString().c_str(),bond.at(i).ato2->Label.toStdString().c_str(),kurz,bond.at(i).length,ara2);
    glPushMatrix();
    //    glTranslated (mitsav.x,mitsav.y,mitsav.z);
    glTranslated (
    bond.at(i).ato2->pos.x+bondir.x,
    bond.at(i).ato2->pos.y+bondir.y,
    bond.at(i).ato2->pos.z+bondir.z);//Anfangspunkt
    wink=acos(((bond.at(i).ato1->pos.z-bond.at(i).ato2->pos.z)/
			    (sqrt((bond.at(i).ato1->pos.x-bond.at(i).ato2->pos.x)*(bond.at(i).ato1->pos.x-bond.at(i).ato2->pos.x)+
				  (bond.at(i).ato1->pos.y-bond.at(i).ato2->pos.y)*(bond.at(i).ato1->pos.y-bond.at(i).ato2->pos.y)+
				  (bond.at(i).ato1->pos.z-bond.at(i).ato2->pos.z)*(bond.at(i).ato1->pos.z-bond.at(i).ato2->pos.z)))))/M_PI*180.0;

    glRotated(wink,vec.x,vec.y,vec.z); // drehen
    GLUquadricObj *q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);   // ein Zylinder
    gluQuadricTexture(q,GL_TRUE);
       if (parthigh) {
	 glEnable(GL_BLEND);
	 QColor pcolor;
	 switch (bond.at(i).ato1->part){
		 case 1: pcolor=QColor("lightskyblue");break;
		 case 2: pcolor=QColor("darkgoldenrod");break;
		 case 3: pcolor=Qt::green;break;
		 case 4: pcolor=Qt::yellow;break;
		 case 5: pcolor=QColor("orange");break;
		 case 6: pcolor=Qt::black;break;
		 case 7: pcolor=QColor("lightsalmon");break;
		 case 8: pcolor=QColor("lightseagreen");break;
		 case -1: pcolor=QColor("aquamarine");break;
		 case -2: pcolor=QColor("olive");break;
		 case -3: pcolor=QColor("bisque");break;
                 case 666: pcolor=QColor("fuchsia");break;
		 default: if (bond.at(i).ato1->part>0)
			    pcolor=QColor::colorNames().at(qMin(bond.at(i).ato1->part,QColor::colorNames().size()-1));
			  else pcolor=Qt::magenta;
	 }
	 pcolor.setAlpha(140);
	 glColor4d(pcolor.redF(),
			 pcolor.greenF(),
			 pcolor.blueF(),
			 pcolor.alphaF());
	 
       }
    //if (trans) {glEnable(GL_BLEND);glEnable(GL_CULL_FACE);}
    gluCylinder(q, bondStrength*psc, bondStrength*psc, (float)(bond.at(i).length /2.0f -kurz), 5*LOD, 1);
    //if (trans) {glDisable(GL_BLEND);glDisable(GL_CULL_FACE);}
    glPopMatrix();

    if (bond.at(i).ato1->an>-1){ 
        glColor4d(AtomColor[bond.at(i).ato1->an].redF(),
                        AtomColor[bond.at(i).ato1->an].greenF(),
                        AtomColor[bond.at(i).ato1->an].blueF(),
                        AtomColor[bond.at(i).ato1->an].alphaF());
      
    }
    if ((bond.at(i).ato2->symmGroup|bond.at(i).ato1->symmGroup)&&((bond.at(i).ato2->part<0)||(bond.at(i).ato1->part<0))){
      //		    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,blue );
      glColor4d(AtomColor[bond.at(i).ato1->an].redF(),
                AtomColor[bond.at(i).ato1->an].greenF(),
		1.0,
		0.3);
      glEnable(GL_BLEND);
    }

    vec.x=(bond.at(i).ato1->pos.y-bond.at(i).ato2->pos.y);
    vec.y=(bond.at(i).ato2->pos.x-bond.at(i).ato1->pos.x);
    vec.z=0;
    if (Norm(vec)) vec=Normalize(vec);
    else vec=V3(0,1,0);

    bondir =V3(0,0,0);
    if (bondStrength<ara1)
      bondir= Normalize(bond.at(i).ato2->pos-bond.at(i).ato1->pos)*(0.9*sqrt((ara1*ara1)-bondStrength*bondStrength));
    kurz=sqrt(Norm(bondir));// */
    glPushMatrix();
    //    glTranslated (mitsav.x,mitsav.y,mitsav.z);
    glTranslated (
		    bond.at(i).ato1->pos.x+bondir.x,
		    bond.at(i).ato1->pos.y+bondir.y,
		    bond.at(i).ato1->pos.z+bondir.z);//Anfangspunkt
    wink=acos(((bond.at(i).ato2->pos.z-bond.at(i).ato1->pos.z)/
			    (sqrt((bond.at(i).ato2->pos.x-bond.at(i).ato1->pos.x)*(bond.at(i).ato2->pos.x-bond.at(i).ato1->pos.x)+
				  (bond.at(i).ato2->pos.y-bond.at(i).ato1->pos.y)*(bond.at(i).ato2->pos.y-bond.at(i).ato1->pos.y)+
				  (bond.at(i).ato2->pos.z-bond.at(i).ato1->pos.z)*(bond.at(i).ato2->pos.z-bond.at(i).ato1->pos.z)))))/M_PI*180.0;
    glRotated(wink,vec.x,vec.y,vec.z); // drehen
    q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);   // ein Zylinder
    gluQuadricTexture(q,GL_TRUE);
    //if (trans) {glEnable(GL_BLEND);glEnable(GL_CULL_FACE);}
       if (parthigh) {
	 glEnable(GL_BLEND);
	 QColor pcolor;
	 switch (bond.at(i).ato2->part){
		 case 1: pcolor=QColor("lightskyblue");break;
		 case 2: pcolor=QColor("darkgoldenrod");break;
		 case 3: pcolor=Qt::green;break;
		 case 4: pcolor=Qt::yellow;break;
		 case 5: pcolor=QColor("orange");break;
		 case 6: pcolor=Qt::black;break;
		 case 7: pcolor=QColor("lightsalmon");break;
		 case 8: pcolor=QColor("lightseagreen");break;
		 case -1: pcolor=QColor("aquamarine");break;
		 case -2: pcolor=QColor("olive");break;
		 case -3: pcolor=QColor("bisque");break;
                 case 666: pcolor=QColor("fuchsia");break;
		 default: if (bond.at(i).ato2->part>0)
			    pcolor=QColor::colorNames().at(qMin(bond.at(i).ato2->part,QColor::colorNames().size()-1));
			  else pcolor=Qt::magenta;
	 }
	 pcolor.setAlpha(140);
	 glColor4d(pcolor.redF(),
			 pcolor.greenF(),
			 pcolor.blueF(),
			 pcolor.alphaF());
	 
       }
    gluCylinder(q, bondStrength*psc, bondStrength*psc, (float)bond.at(i).length/2.0f-kurz, 5*LOD, 1);

    gluDeleteQuadric(q);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
  }

}

QString Molecule::h_bonds(Connection bond,CEnvironment atoms) {
  /*! calculates Hydrogen bonds and draws them as stippled cylinders 
   * \returns a html table of the found H-bonds
   */

  glDisable(GL_CULL_FACE);
  if (HAMax<0.01)return "";
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, hbtex);
  glEnable(GL_TEXTURE_2D);
  QString hbonds;
  double gg=0,kk=0,ll=0;
  double kurz=0,ara1,ara2;
  float wink,hb_wink;
  V3 vec,bondir;
  const MyAtom *Hyd,*Acc,*Don;//Wasserstoff Akzeptor und Donor
  hbonds.append("<h2>Hydrogen Bonds</h2><table border=1><tr><th>Donor---Hydrogen...Acceptor</th><th>Don--Hyd [&Aring;]</th><th> Hyd--Acc [&Aring;]</th><th>Don--Acc [&Aring;]</th><th>D--H-----A </th></tr>");
  for (int i=0;i<bond.size();i++) {
      if (((bond.at(i).ato1->an == 0)&&
           (bond.at(i).ato2->an >  5)&&
           (bond.at(i).ato2->an <  9))||
          ((bond.at(i).ato2->an == 0)&&
           (bond.at(i).ato1->an >  5)&&
           (bond.at(i).ato1->an <  9))) {
        Hyd=(bond.at(i).ato1->an==0)?bond.at(i).ato1:bond.at(i).ato2;
        Don=(bond.at(i).ato1->an==0)?bond.at(i).ato2:bond.at(i).ato1;
        for (int j=0; j<atoms.size();j++){
            if ((atoms.at(j).an<6)||(atoms.at(j).an>8)) continue;
	    if ((Don->part!=0)&&(atoms.at(j).part!=Don->part)) continue;
	    if ((Hyd->part!=0)&&(atoms.at(j).part!=Hyd->part)) continue;

            //printf("%d %d %d\n",bond.at(i).ato1->an,bond.at(i).ato2->an,atoms.at(j).an);
	    Acc=&atoms.at(j);
	    if (Acc==Don) continue;
	    gg=sqrt( Distance(Hyd->pos,atoms.at(j).pos));


	    if (gg<=HAMax){
	      hb_wink=winkel(Acc->pos-Hyd->pos, Don->pos-Hyd->pos);
	      /* printf("%s %s %s %f %f\n",bond.at(i).ato1->Label.toStdString().c_str(),
		 bond.at(i).ato2->Label.toStdString().c_str(),
		 atoms.at(j).Label.toStdString().c_str(),gg,hb_wink);*/
	      if ((gg>=1.3)&&(fabs(hb_wink)>HAWink)) {//wenn AbstandRichtig
		ll=sqrt(Distance(Acc->pos,Don->pos));
		kk=sqrt(Distance(Don->pos,Hyd->pos));
		hbonds.append(QString("<tr><td>%1---%2...%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7&deg;</td></tr>")
				.arg(Don->Label)
				.arg(Hyd->Label)
				.arg(Acc->Label)
				.arg(kk,6,'f',2)
				.arg(gg,6,'f',2)
				.arg(ll,6,'f',3)
				.arg(hb_wink,5,'f',1));
		glPushMatrix();
		ara1=arad[0];
		ara2=arad[Acc->an];
		if (adp) {
		  V3 hin=Normalize(bond.at(i).ato2->pos-bond.at(i).ato1->pos);
		  double r=1;
		  switch (proba){
			  case 10:r=0.76;break;
			  case 30:r=1.19;break;
			  case 50:r=1.54;break;
			  case 70:r=1.91;break;
			  case 90:r=2.50;break;
		  }      
		  ara1=sqrt((hin*bond.at(i).ato1->uc)*hin);
		  ara1*=r;
		  ara2=sqrt((hin*bond.at(i).ato2->uc)*hin);
		  ara2*=r;
		  //      printf("%s-%s %f %f %d %f\n",bond.at(i).ato1->Label.toStdString().c_str(),bond.at(i).ato2->Label.toStdString().c_str(),ara1, ara2, proba,r);
		}
		if (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato1->an])ara1=arad[0];
		if (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato2->an])ara2=arad[Acc->an];
		if (tubes) ara1=ara2=bondStrength;
		bondir =V3(0,0,0);
		if (bondStrength<ara2)
		  bondir= Normalize(Hyd->pos-Acc->pos)*(1.2*sqrt((ara2*ara2)-bondStrength*bondStrength));
		if (bondStrength<ara1)
		  kurz=(1.2*sqrt((ara1*ara1)-bondStrength*bondStrength)+1.2*sqrt(ara2*ara2)-bondStrength*bondStrength);//  */
		else kurz=0;
		glTranslated(Acc->pos.x+bondir.x,Acc->pos.y+bondir.y,Acc->pos.z+bondir.z);//z
		vec=kreuzX(Hyd->pos.x-Acc->pos.x,
				Hyd->pos.y-Acc->pos.y,
				Hyd->pos.z-Acc->pos.z,
				0.0f,0.0f,1.0f);
		wink=acos(((Hyd->pos.z-Acc->pos.z)/
					(sqrt((Acc->pos.x-Hyd->pos.x)*(Acc->pos.x-Hyd->pos.x)+
					      (Acc->pos.y-Hyd->pos.y)*(Acc->pos.y-Hyd->pos.y)+
					      (Acc->pos.z-Hyd->pos.z)*(Acc->pos.z-Hyd->pos.z)))))/M_PI*180.0;
		//printf("%f %f %f %f\n",wink, vec.x, vec.y, vec.z);
		glRotatef(wink, vec.x, vec.y, vec.z); // drehen
		GLUquadricObj *q = gluNewQuadric();
		gluQuadricNormals(q, GL_SMOOTH);   // ein Zylinder
		//if (!adp)
		if (gg>2.0) glColor4f(1,0.6,0,1);	else glColor4f(1,1,0,1);
		gluQuadricTexture(q,GL_TRUE);
		gluCylinder(q,bondStrength*0.85 , bondStrength*0.85, (float)gg-kurz, 5*LOD, 1);

        gluDeleteQuadric(q);
        glPopMatrix();
	      }
	    }
      }
    }
  }
//glDisable( GL_BLEND);
  glDisable(GL_ALPHA_TEST);

  glDisable(GL_TEXTURE_2D);
  glEnable(   GL_CULL_FACE);
  if (hbonds.contains("deg;")) hbonds.append("</table><br>");
  else hbonds.clear();
  return hbonds;
}

void Molecule::h_bonds2(Connection bond,CEnvironment atoms) {
 /*! calculates Hydrogen bonds and draws them as stippled cylinders 
  */
  glDisable(GL_CULL_FACE);
  if (HAMax<0.01)return ;
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, hbtex);
  glEnable(GL_TEXTURE_2D);
  double gg=0;//,kk=0,ll=0;
  double kurz=0,ara1,ara2;
  float wink,hb_wink;
  V3 vec,bondir;
  const MyAtom *Hyd,*Acc,*Don;//Wasserstoff Akzeptor und Donor
  for (int i=0;i<bond.size();i++) {
      if (((bond.at(i).ato1->an == 0)&&
           (bond.at(i).ato2->an >  5)&&
           (bond.at(i).ato2->an <  9))||
          ((bond.at(i).ato2->an == 0)&&
           (bond.at(i).ato1->an >  5)&&
           (bond.at(i).ato1->an <  9))) {
        Hyd=(bond.at(i).ato1->an==0)?bond.at(i).ato1:bond.at(i).ato2;
        Don=(bond.at(i).ato1->an==0)?bond.at(i).ato2:bond.at(i).ato1;
        for (int j=0; j<atoms.size();j++){
            if ((atoms.at(j).an<6)||(atoms.at(j).an>8)) continue;
	    if ((Don->part!=0)&&(atoms.at(j).part!=Don->part)) continue;
	    if ((Hyd->part!=0)&&(atoms.at(j).part!=Hyd->part)) continue;

            //printf("%d %d %d\n",bond.at(i).ato1->an,bond.at(i).ato2->an,atoms.at(j).an);
        Acc=&atoms.at(j);
        if (Acc==Don) continue;
        gg=sqrt( Distance(Hyd->pos,atoms.at(j).pos));


        if (gg<=HAMax){
          hb_wink=winkel(Acc->pos-Hyd->pos, Don->pos-Hyd->pos);
         /* printf("%s %s %s %f %f\n",bond.at(i).ato1->Label.toStdString().c_str(),
                 bond.at(i).ato2->Label.toStdString().c_str(),
                 atoms.at(j).Label.toStdString().c_str(),gg,hb_wink);*/
          if ((gg>=1.3)&&(fabs(hb_wink)>HAWink)) {//wenn AbstandRichtig
            //ll=sqrt(Distance(Acc->pos,Don->pos));
            //kk=sqrt(Distance(Don->pos,Hyd->pos));
            glPushMatrix();
	    ara1=arad[0];
	    ara2=arad[Acc->an];
	    if (adp) {
	      V3 hin=Normalize(bond.at(i).ato2->pos-bond.at(i).ato1->pos);
	      double r=1;
	      switch (proba){
		      case 10:r=0.76;break;
		      case 30:r=1.19;break;
		      case 50:r=1.54;break;
		      case 70:r=1.91;break;
		      case 90:r=2.50;break;
	      }      
	      ara1=sqrt((hin*bond.at(i).ato1->uc)*hin);
	      ara1*=r;
	      ara2=sqrt((hin*bond.at(i).ato2->uc)*hin);
	      ara2*=r;
	      //      printf("%s-%s %f %f %d %f\n",bond.at(i).ato1->Label.toStdString().c_str(),bond.at(i).ato2->Label.toStdString().c_str(),ara1, ara2, proba,r);
	    }
	    if (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato1->an])ara1=arad[0];
	    if (ATOM_STYLE_NOADP&AtomStyle[bond.at(i).ato2->an])ara2=arad[Acc->an];
	    if (tubes) ara1=ara2=bondStrength;
	    bondir =V3(0,0,0);
	    if (bondStrength<ara2)
	      bondir= Normalize(Hyd->pos-Acc->pos)*(1.2*sqrt((ara2*ara2)-bondStrength*bondStrength));
	    if (bondStrength<ara1)
	    kurz=(1.2*sqrt((ara1*ara1)-bondStrength*bondStrength)+1.2*sqrt(ara2*ara2)-bondStrength*bondStrength);//  */
	    else kurz=0;
            glTranslated(Acc->pos.x+bondir.x,Acc->pos.y+bondir.y,Acc->pos.z+bondir.z);//z
            vec=kreuzX(Hyd->pos.x-Acc->pos.x,
                       Hyd->pos.y-Acc->pos.y,
                       Hyd->pos.z-Acc->pos.z,
                       0.0f,0.0f,1.0f);
            wink=acos(((Hyd->pos.z-Acc->pos.z)/
                                    (sqrt((Acc->pos.x-Hyd->pos.x)*(Acc->pos.x-Hyd->pos.x)+
                                          (Acc->pos.y-Hyd->pos.y)*(Acc->pos.y-Hyd->pos.y)+
                                          (Acc->pos.z-Hyd->pos.z)*(Acc->pos.z-Hyd->pos.z)))))/M_PI*180.0;
            //printf("%f %f %f %f\n",wink, vec.x, vec.y, vec.z);
            glRotatef(wink, vec.x, vec.y, vec.z); // drehen
            GLUquadricObj *q = gluNewQuadric();
            gluQuadricNormals(q, GL_SMOOTH);   // ein Zylinder
            //if (!adp)
            if (gg>2.0) glColor4f(1,0.6,0,1);	else glColor4f(1,1,0,1);
            gluQuadricTexture(q,GL_TRUE);
            gluCylinder(q,bondStrength*0.85 , bondStrength*0.85, (float)gg-kurz, 5*LOD, 1);

            gluDeleteQuadric(q);
            glPopMatrix();
          }
        }
      }
    }
  }
//glDisable( GL_BLEND);
  glDisable(GL_ALPHA_TEST);

  glDisable(GL_TEXTURE_2D);
  glEnable(   GL_CULL_FACE);
}
void Molecule::hirsh(Connection bond){
  return;
  V3 hin;
//  double zab,zba;
  for (int i=0; i<bond.size();i++){
    hin=Normalize(bond.at(i).ato2->pos-bond.at(i).ato1->pos);
//    zab=(hin*bond.at(i).ato1->uc)*hin; 
//    zba=(hin*bond.at(i).ato2->uc)*hin; 
/*    printf("%-9s = %-9s zab %g zba %g  %g\n",
		    bond.at(i).ato1->Label.toStdString().c_str(),
		    bond.at(i).ato2->Label.toStdString().c_str(),
		    zab,zba,fabs(zab-zba)*1000);*/
  }
}

void Molecule::lbond(){
  /*! draws line bonds from lbonds list. 
   */

    glPushMatrix();
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(0.5,0.3,0.3);
    for (int i=0; i<lbonds.size();i++)
    if (lbonds.at(i).ato1->hidden+lbonds.at(i).ato2->hidden==0){
        glVertex3d(lbonds.at(i).ato1->pos.x,lbonds.at(i).ato1->pos.y,lbonds.at(i).ato1->pos.z);
        glVertex3d(lbonds.at(i).ato2->pos.x,lbonds.at(i).ato2->pos.y,lbonds.at(i).ato2->pos.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);    
    glDisable(GL_BLEND);
    glPopMatrix();

}

void Molecule::dbond(Connection bond){
   /*! draws line bonds from bond list .
    */

    glPushMatrix();
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glLineWidth(3);
    glColor3f(0.0,0.0,0.0);
    for (int i=0; i<bond.size();i++)
    if (bond.at(i).ato1->hidden+bond.at(i).ato2->hidden==0){
        glVertex3d(bond.at(i).ato1->pos.x,bond.at(i).ato1->pos.y,bond.at(i).ato1->pos.z);
        glVertex3d(bond.at(i).ato2->pos.x,bond.at(i).ato2->pos.y,bond.at(i).ato2->pos.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);    
    glDisable(GL_BLEND);
    glPopMatrix();

}
void Molecule::unitCell(){
  /*!draws a unit cell box
   *
   * */
  V3 uz0f,uz1f,uz2f,uz3f,uz4f,uz5f,uz6f,uz7f;
  V3 uz0k,uz1k,uz2k,uz3k,uz4k,uz5k,uz6k,uz7k;
  uz0f.x=0.0;  uz0f.y=0.0;  uz0f.z=0.0;
  uz1f.x=1.0;  uz1f.y=0.0;  uz1f.z=0.0;
  uz2f.x=0.0;  uz2f.y=1.0;  uz2f.z=0.0;
  uz3f.x=0.0;  uz3f.y=0.0;  uz3f.z=1.0;
  uz4f.x=1.0;  uz4f.y=1.0;  uz4f.z=0.0;
  uz5f.x=1.0;  uz5f.y=0.0;  uz5f.z=1.0;
  uz6f.x=0.0;  uz6f.y=1.0;  uz6f.z=1.0;
  uz7f.x=1.0;  uz7f.y=1.0;  uz7f.z=1.0;
  frac2kart(uz0f,uz0k);
  frac2kart(uz1f,uz1k);
  frac2kart(uz2f,uz2k);
  frac2kart(uz3f,uz3k);
  frac2kart(uz4f,uz4k);
  frac2kart(uz5f,uz5k);
  frac2kart(uz6f,uz6k);
  frac2kart(uz7f,uz7k);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLineWidth(2);
      glPushMatrix();
      glBegin(GL_LINES);
      glColor4f(1.0f,0.0f,0.0f,1.0);
      glVertex3d(uz0k.x,uz0k.y,uz0k.z);
      glVertex3d(uz1k.x,uz1k.y,uz1k.z);

      glColor4f(0.0f,1.0f,0.0f,1.0);
      glVertex3d(uz0k.x,uz0k.y,uz0k.z);
      glVertex3d(uz2k.x,uz2k.y,uz2k.z);

      glColor4f(0.0f,0.0f,1.0f,1.0);
      glVertex3d(uz0k.x,uz0k.y,uz0k.z);
      glVertex3d(uz3k.x,uz3k.y,uz3k.z);

      glColor4f(0.6f,0.6f,0.8f,1.0);
      glVertex3d(uz1k.x,uz1k.y,uz1k.z);
      glVertex3d(uz4k.x,uz4k.y,uz4k.z);

      glVertex3d(uz1k.x,uz1k.y,uz1k.z);
      glVertex3d(uz5k.x,uz5k.y,uz5k.z);

      glVertex3d(uz2k.x,uz2k.y,uz2k.z);
      glVertex3d(uz4k.x,uz4k.y,uz4k.z);

      glVertex3d(uz2k.x,uz2k.y,uz2k.z);
      glVertex3d(uz6k.x,uz6k.y,uz6k.z);

      glVertex3d(uz3k.x,uz3k.y,uz3k.z);
      glVertex3d(uz5k.x,uz5k.y,uz5k.z);

      glVertex3d(uz3k.x,uz3k.y,uz3k.z);
      glVertex3d(uz6k.x,uz6k.y,uz6k.z);

      glVertex3d(uz7k.x,uz7k.y,uz7k.z);
      glVertex3d(uz5k.x,uz5k.y,uz5k.z);

      glVertex3d(uz7k.x,uz7k.y,uz7k.z);
      glVertex3d(uz4k.x,uz4k.y,uz4k.z);

      glVertex3d(uz7k.x,uz7k.y,uz7k.z);
      glVertex3d(uz6k.x,uz6k.y,uz6k.z);

      glEnd();
      glPopMatrix();
  glLineWidth(1);
  glEnable(GL_LIGHTING);  
  glDisable(GL_BLEND);
}

void Molecule::Uf2Uo(const Matrix x, Matrix & y) {
  /*! Turns fractional Uij's into cartesinan based Uij's.
   * @param[in] x fractional Uij matrix.
   * @param[out] y cartesinan Uij matrix.
   */
  Matrix o,a,w;   /*Cholesky decomposition of the real space Metric tensor
		    Wird fr die Umrechnung von fraktionellen in kartesischen Korrdinaten bentigt.*/
  a.m11 =cell.as;
  a.m12 = 0;
  a.m13 = 0;
  a.m21 = 0;
  a.m22 = cell.bs;
  a.m23 = 0;
  a.m31 = 0;
  a.m32 = 0;
  a.m33 = cell.cs;
  w=(a*x)*a;
  o.m11 =cell.a;
  o.m12 = 0.0;
  o.m13 = 0.0;
  o.m21 = cell.b*cell.cosga;
  o.m22 = cell.b*cell.singa;
  o.m23 = 0.0;
  o.m31 = cell.c* cell.cosbe;
  o.m32 = cell.tau;
  o.m33 = cell.c* cell.phi / cell.singa ;
//  Matrix to=;
  y=(o*w)*transponse(o);
/*  printf("%f %f %f %f %f %f %f %f %f\n%f %f %f %f %f %f %f %f %f\n%f %f %f %f %f %f %f %f %f\n%f %f %f %f %f %f %f %f %f\n",
		  o.m11,o.m12,o.m13,o.m21,o.m22,o.m23,o.m31,o.m32,o.m33,
		  to.m11,to.m21,to.m31,to.m12,to.m22,to.m32,to.m13,to.m23,to.m33,  
		  x.m11,x.m12,x.m13,x.m21,x.m22,x.m23,x.m31,x.m32,x.m33,
		  w.m11,w.m12,w.m13,w.m21,w.m22,w.m23,w.m31,w.m32,w.m33
		  );*/
//  printf("%f %f %f %f %f %f\n",y.m11*10000,y.m22*10000,y.m33*10000,y.m12*10000,y.m13*10000,y.m23*10000);//		  */
}

void Molecule::Usym (Matrix x,Matrix sym, Matrix & y){
  /*! Applies the symmetry matrix sym to Uij's 
   * @param[in] x Uij matrix.
   * @param[in] sym symmtry matrix.
   * @param[out] y resulting Uij matrix. 
   */
  //y=(transponse(sym)*x)*sym;
  y=(sym*x)*transponse(sym);
}
//void Molecule::

#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau);
//NumericalRecepies....
double * Molecule::jacobi(const Matrix &uij, V3 &ev) { 

  int j,iq,ip,i,n=3,nrot; 
  double tresh=0,theta,tau,t,sm,s,h,g,c; 
  double a[3][3],b[3],z[3],v[3][3],d[3];
  a[0][0]=uij.m11;
  a[0][1]=uij.m12;
  a[0][2]=uij.m13;
  a[1][0]=uij.m21;
  a[1][1]=uij.m22;
  a[1][2]=uij.m23;
  a[2][0]=uij.m31;
  a[2][1]=uij.m32;
  a[2][2]=uij.m33;
  static double erg[4]={0.0,1.0,0.0,0.0};
  for (ip=1;ip<=n;ip++) {   
    for (iq=1;iq<=n;iq++) v[ip-1][iq-1]=0.0; 
    v[ip-1][ip-1]=1.0; 
  } 
  for (ip=1;ip<=n;ip++) {  
    b[ip-1]=d[ip-1]=a[ip-1][ip-1]; 
    z[ip-1]=0.0; 
  } 
  nrot=0; 
  for (i=1;i<=150;i++) {
    sm=0.0; 
    for (ip=1;ip<=n-1;ip++) { 
      for (iq=ip+1;iq<=n;iq++) 
	sm += fabs(a[ip-1][iq-1]);       
    } 

    //printf("sm =%20.19f\n",sm);

    if (float(sm) < tresh) { 
      if ((v[0][0]+v[1][1]+v[2][2])!=3.0) {
      erg[0]=acos((v[0][0]+v[1][1]+v[2][2]-1.0)/2.0);
      erg[1]=(v[2][1]-v[1][2])/(2.0*sin(erg[0]));
      erg[2]=(v[0][2]-v[2][0])/(2.0*sin(erg[0]));
      erg[3]=(v[1][0]-v[0][1])/(2.0*sin(erg[0]));
      erg[0]*=180.0/M_PI;}
      else {erg[0]=0.0;erg[1]=1.0;erg[2]=0.0;erg[3]=0.0; }
      //printf("%d??ERG:%f %f %f %f\n",i,Ato4d(erg));
      /*
      printf("=%d======================================\n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f\n========================================\n",i,
		      d[0],d[1],d[2],v[0][0],v[0][1],v[0][2]
		  ,v[1][0],v[1][1],v[1][2]
		  ,v[2][0],v[2][1],v[2][2]
		  );*/
     ev=V3(d[0],d[1],d[2]);
     return (double*) erg;
    }
    if (i < 4) tresh=0.00001;  
    else tresh=0.0001;
    for (ip=1;ip<=n-1;ip++) { 
      for (iq=ip+1;iq<=n;iq++) { 
	//printf("\np:%i q:%i i:%i nrot:%i\n",ip,iq,i,nrot);
	g=100.0*fabs(a[ip-1][iq-1]);  
	if ((i > 4) && ((fabs(d[ip-1])+g) == fabs(d[ip-1])) && ((fabs(d[iq-1])+g) == fabs(d[iq-1]))) {a[ip-1][iq-1]=0.0;}
	else if (fabs(a[ip-1][iq-1]) >= tresh) { 
	  h=d[iq-1]-d[ip-1]; 
	  if ((fabs(h)+g) == fabs(h)) {t=(a[ip-1][iq-1])/h; } 
	  else { theta=0.5*h/(a[ip-1][iq-1]);  
	  t=1.0/(fabs(theta)+sqrt(1.0+theta*theta)); 
	  if (theta < 0.0) {t = -1.0*t;}
	  } 
	  c=1.0/sqrt(1+t*t); 
	  s=t*c; 
	  tau=s/(1.0+c); 
	  h=t*a[ip-1][iq-1];
	  z[ip-1] -= h;
	  z[iq-1] += h;
	  d[ip-1] -= h;
	  d[iq-1] += h;
	  a[ip-1][iq-1]=0.0;
	  for (j=1;j<=ip-1;j++) { 
           ROTATE(a,j-1,ip-1,j-1,iq-1)
	      //printf("%i %i %i %i",j,ip,j,iq);
	      } 
	  for (j=ip+1;j<=iq-1;j++) { 
           ROTATE(a,ip-1,j-1,j-1,iq-1)
	      //printf("%i %i %i %i ",ip,j,j,iq);
	      } 
	  for (j=iq+1;j<=n;j++) {  
           ROTATE(a,ip-1,j-1,iq-1,j-1)
	      //printf("%i %i %i %i",ip,j,iq,j);
	      } 
	  for (j=1;j<=n;j++) { 
           ROTATE(v,j-1,ip-1,j-1,iq-1)
	      } 
	  ++(nrot);	  
	  //    printf("U|\n%f %f %f  \n%f %f %f\n%f %f %f\nV|\n%f %f %f  \n%f %f %f\n%f %f %f\n\n",a[0][0],a[1][0],a[2][0],a[0][1],a[1][1],a[2][1],a[0][2],a[1][2],a[2][2],v[0][0],v[1][0],v[2][0],v[0][1],v[1][1],v[2][1],v[0][2],v[1][2],v[2][2]);
	} //else ;//printf("nix:%f p%i q%i",fabs(a[ip-1][iq-1]),ip,iq);
      } 
    } 
    for (ip=1;ip<=n;ip++) { 
      b[ip-1] += z[ip-1];
      d[ip-1] =b[ip-1];
      z[ip-1] =0.0;
    } 
  } 
  erg[0]=acos((v[0][0]+v[1][1]+v[2][2]-1.0)/2.0);
  if (erg[0]==0) {
    erg[1]=1.0;
    erg[2]=0.0;
    erg[3]=0.0;
  }else{
  erg[1]=(v[2][1]-v[1][2])/(2.0*sin(erg[0]));
  erg[2]=(v[0][2]-v[2][0])/(2.0*sin(erg[0]));
  erg[3]=(v[1][0]-v[0][1])/(2.0*sin(erg[0]));
  erg[0]*=180.0/M_PI;
  }
  /*printf("=%d=======================================\n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f \n%8.5f %8.5f %8.5f\n========================================\n",i,
		  d[0],d[1],d[2],v[0][0],v[0][1],v[0][2]
		  ,v[1][0],v[1][1],v[1][2]
		  ,v[2][0],v[2][1],v[2][2]

		  );*/
  ev=V3(d[0],d[1],d[2]);
 return (double*)erg;
}

void Molecule::fuse(){
  /*! reduces the showatoms list to the asymmetric unit.
   * */
    HumanSymmetry=QString("Used Symmetry:<br>%1").arg(symmcode2human(QStringList()));
    usedSymmetry.clear();
    showatoms.clear();
    for (int i=0; i<asymm.size();i++){
        showatoms.append(asymm[i]);
        showatoms[i].molindex=asymm[i].molindex;
    }
    showbonds.clear();
    showbonds=connecting(showatoms);
}

void Molecule::grow(){
  /*! completes covalent bound molecule fragments. 
   *
   * */
  showatoms.clear();
  for (int i=0; i<asymm.size();i++){
    showatoms.append(asymm[i]);
    showatoms[i].molindex=asymm[i].molindex;
  }
  showbonds.clear();
  QStringList brauchSymm;
  QString bs;
  brauchSymm.clear();
  V3 prime,dp,D,floorD;
  double dk,dddd;
  for (int k =0; k<sdm.size();k++){
    if ((sdm.at(k).covalent)||(growQPeak)){
      if (!(growQPeak)&&(asymm[sdm.at(k).a1].molindex<1)) continue;
      for (int n=0;n<cell.symmops.size();  n++){
	if (((asymm[sdm.at(k).a1].part!=0)&&(asymm[sdm.at(k).a2].part!=0)&&(asymm[sdm.at(k).a1].part!=asymm[sdm.at(k).a2].part)))continue;
	if ((asymm[sdm.at(k).a1].an==asymm[sdm.at(k).a2].an)&&(asymm[sdm.at(k).a1].an==0)) continue;
	prime=cell.symmops.at(n) * asymm[sdm.at(k).a1].frac + cell.trans.at(n);
	D=prime - asymm[sdm.at(k).a2].frac+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
	if ((n==0)&&(V3(0,0,0)==floorD)) continue;
	dk=fl(dp.x,dp.y,dp.z);
	//printf ("%f n%d\n",dk,n);
	dddd=(sdm.at(k).covalent)?(sdm.at(k).d+0.2):0;
	if ((asymm[sdm.at(k).a1].an<0)&&(asymm[sdm.at(k).a2].an<0)) dddd=1.8;//||
	if ( (dk>0.001)&&(dddd>=dk)) {
	  bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm[sdm.at(k).a1].molindex);
	  //	     printf("%s %s %g %g\n",asymm[sdm.at(k).a1].Label.toStdString().c_str(),asymm[sdm.at(k).a2].Label.toStdString().c_str(),sdm.at(k).d,dddd);
	  //             printf("(grow)%s\n",bs.toStdString().c_str());
	  if  (!brauchSymm.contains(bs)) {
	    brauchSymm.append(bs);
	  }
	}
      }
    }
  }

  for (int i=0; i<bindatoms.size(); i++){
    MyAtom a1, a2;
    bool bo=false;
    int sy1=-1,sy2=-1;
    if (bindatoms.at(i).Lab1.contains("_$"))sy1=bindatoms.at(i).Lab1.section("_$",1,1).toInt(&bo);
    if (bindatoms.at(i).Lab2.contains("_$"))sy2=bindatoms.at(i).Lab2.section("_$",1,1).toInt(&bo);
    bo=false;
    if (bindatoms.at(i).Lab1.contains('_'))a1.resiNr=bindatoms.at(i).Lab1.section('_',1,1).toInt(&bo);
    if (!bo) a1.resiNr=-1;
    if (bindatoms.at(i).Lab2.contains('_'))a2.resiNr=bindatoms.at(i).Lab2.section('_',1,1).toInt(&bo);
    if (!bo)a2.resiNr=-1;
    a1.Label = bindatoms.at(i).Lab1.section('_',0,0);
    a2.Label = bindatoms.at(i).Lab2.section('_',0,0);
    int frag1=0,frag2=0;
    for (int ii=0; ii<asymm.size();ii++){
      if ((a1.resiNr>-1)&& (asymm[ii].resiNr!=a1.resiNr)) continue;
      if ((a2.resiNr>-1)&& (asymm[ii].resiNr!=a2.resiNr)) continue;
      if (asymm[ii].Label.section('_',0,0).toUpper() == a1.Label.toUpper()){
	frag1=asymm[ii].molindex;
      }
      if (asymm[ii].Label.section('_',0,0).toUpper() == a2.Label.toUpper()){
	frag2=asymm[ii].molindex;
      }
    }
    QString ss;
    QList<Matrix> smm;
    QList<V3> tmm;
    int syy1=-1;
    if (sy1>-1) syy1=labelEQIV.indexOf(QString("$%1").arg(sy1));
    if (syy1>-1) {
      smm.append(symmopsEQIV.at(syy1));
      tmm.append(transEQIV.at(syy1));
    }
    int syy2=-1;
    if (sy2>-1) syy2=labelEQIV.indexOf(QString("$%1").arg(sy2));
    if (syy2>-1) {
      smm.append(symmopsEQIV.at(syy2));
      tmm.append(transEQIV.at(syy2));
    }
    for (int ui=0; ui<smm.size(); ui++)
      if (cell.symmops.contains(smm.at(ui))) {
	V3 r;
	r.x=fmod(tmm.at(ui).x+10,1.0);
	r.y=fmod(tmm.at(ui).y+10,1.0);
	r.z=fmod(tmm.at(ui).z+10,1.0);

//	qDebug()<<"da"<<r.x<<r.y<<r.z<<maxmol<<frag1<<frag2<<cell.symmops.indexOf(smm.at(ui));
	for (int is=0; is<cell.symmops.size();is++){
	  if ((cell.symmops.at(is)==smm.at(ui))&&(cell.trans.at(is)==r))
	    for (int yi=1; yi<=5; yi++){
	      ss=QString("%1_%2%3%4:%5,")
		      .arg(is+1)
		      .arg((int)(tmm.at(ui).x-r.x) +5)
		      .arg((int)(tmm.at(ui).y-r.y) +5)
		      .arg((int)(tmm.at(ui).z-r.z) +5)
		      .arg(((sy1>-1)&&(!ui))?frag1:frag2);
	      if  (!brauchSymm.contains(ss)) {
		brauchSymm.append(ss);
//		printf("### %d  = %d\n",((sy1>-1)&&(!ui))?sy1:sy2,brauchSymm.size());
		eqivMap[labelEQIV.at(((sy1>-1)&&(!ui))?sy1-1:sy2-1)]=brauchSymm.size();
//		qDebug()<<ss;
	      }
	    }
	}
      }
  }

  /*  for (int i=0; i<sdm.size();i++){
      if ((sdm.at(i).sn==0)&&(V3(0,0,0)==sdm.at(i).floorD)) continue;
      double dddd=((Kovalenz_Radien[asymm[sdm.at(i).a1].an]+ Kovalenz_Radien[asymm[sdm.at(i).a2].an])*0.01+0.5);
      if (dddd>sdm.at(i).d){
      bs=QString("%1_%2%3%4:%5,").arg(sdm.at(i).sn+1).arg(5-(int)sdm.at(i).floorD.x).arg(5-(int)sdm.at(i).floorD.y).arg(5-(int)sdm.at(i).floorD.z).arg(asymm[sdm.at(i).a1].molindex);
      / *        printf("%s %f %s %s\n",bs.toStdString().c_str(),sdm.at(i).d,
      asymm[sdm.at(i).a1].Label.toStdString().c_str(),
      asymm[sdm.at(i).a2].Label.toStdString().c_str());
   * /
   if  (!brauchSymm.contains(bs)) {
   brauchSymm.append(bs);
   }
   }
   } */
  //packer(brauchSymm);
  usedSymmetry+=brauchSymm;    
  complete();
  showbonds.clear();
  showbonds=connecting(showatoms);
}

void Molecule::fillCell(){
  /*! Fills the unit cell with molecules.
   *
   * */
    showatoms.clear();
    for (int i=0; i<asymm.size();i++){
        showatoms.append(asymm[i]);
        showatoms[i].molindex=asymm[i].molindex;
    }
    showbonds.clear();
    QStringList brauchSymm;
    QString bs;
    brauchSymm.clear();
    V3 prime,dp,D,floorD;
    //double dk,dddd;
     for (int k =0; k<sdm.size();k++){
        if (sdm.at(k).covalent){
          if (asymm[sdm.at(k).a1].molindex<1) continue;
	  for (int n=0;n<cell.symmops.size();  n++){
	    //if (((asymm[sdm.at(k).a1].part!=0)&&(asymm[sdm.at(k).a2].part!=0)&&(asymm[sdm.at(k).a1].part!=asymm[sdm.at(k).a2].part)))continue;
	    //if ((asymm[sdm.at(k).a1].an==asymm[sdm.at(k).a2].an)&&(asymm[sdm.at(k).a1].an==0)) continue;
	    D=prime=cell.symmops.at(n) * asymm[sdm.at(k).a1].frac + cell.trans.at(n);
	    //D=prime - asymm[sdm.at(k).a2].frac+ V3(0.5,0.5,0.5) ;
	    floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	    //dp=D - floorD - V3(0.5,0.5,0.5);
	    if ((n==0)&&(V3(0,0,0)==floorD)) continue;
	    //dk=fl(dp.x,dp.y,dp.z);
	    //printf ("%f n%d\n",dk,n);
	    //dddd=(sdm.at(k).d+0.2);
	    //if ( (dk>0.001)&&(dddd>=dk)) {
	    bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm[sdm.at(k).a1].molindex);
	    //	     printf("%s %s %g %g\n",asymm[sdm.at(k).a1].Label.toStdString().c_str(),asymm[sdm.at(k).a2].Label.toStdString().c_str(),sdm.at(k).d,dddd);
	    //             printf("(grow)%s\n",bs.toStdString().c_str());
	    if  (!brauchSymm.contains(bs)) {
	      brauchSymm.append(bs);
	    }
	    //}
         }
       }
     }
  /*  for (int i=0; i<sdm.size();i++){
        if ((sdm.at(i).sn==0)&&(V3(0,0,0)==sdm.at(i).floorD)) continue;
     double dddd=((Kovalenz_Radien[asymm[sdm.at(i).a1].an]+ Kovalenz_Radien[asymm[sdm.at(i).a2].an])*0.01+0.5);
     if (dddd>sdm.at(i).d){
        bs=QString("%1_%2%3%4:%5,").arg(sdm.at(i).sn+1).arg(5-(int)sdm.at(i).floorD.x).arg(5-(int)sdm.at(i).floorD.y).arg(5-(int)sdm.at(i).floorD.z).arg(asymm[sdm.at(i).a1].molindex);
/ *        printf("%s %f %s %s\n",bs.toStdString().c_str(),sdm.at(i).d,
               asymm[sdm.at(i).a1].Label.toStdString().c_str(),
               asymm[sdm.at(i).a2].Label.toStdString().c_str());
               * /
        if  (!brauchSymm.contains(bs)) {
          brauchSymm.append(bs);
         }
    }
    } */
    packer(brauchSymm);
    showbonds.clear();
    showbonds=connecting(showatoms);
}

void Molecule::expandAll(){
  /*! like Molecule.expandAt but this searches vor neighboring molecules arround every atom of the asymmetric unit and adds them to the showatoms list.
   */
  //if (index>=showatoms.size()) return;
//  V3 expander= showatoms.at(index).frac;
  V3 prime,floorD,D;
  double SUCHRAD=3.2;
  for (int i=0; i<asymm.size();i++){//Rapunzel lass dein H herunter
    if (asymm.at(i).an==0) {
      SUCHRAD-=0.7;
      break;
    }
  }
//  printf(" Rapunzels H ist %f lang\n",SUCHRAD);
  QStringList brauchSymm;
  QString bs;
  brauchSymm.clear();
//  brauchSymm+=(usedSymmetry);
  double dk;
  for (int j=0; j<asymm.size();j++){
    if (asymm.at(j).an<0) continue;
    for (int n=0; n<cell.symmops.size();n++){
      for (int i=0; i<asymm.size();i++){
	if (asymm.at(i).an<0) continue;
	prime=cell.symmops.at(n) * asymm.at(i).frac + cell.trans.at(n);
	D=prime - asymm.at(j).frac + V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	if ((n==0)&&(floorD==V3(0,0,0))) {continue;}
	D=D - floorD - V3(0.5,0.5,0.5);
	dk=fl(D.x,D.y,D.z);
	if (dk<SUCHRAD){
	  bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm.at(i).molindex);
	  if  ((!brauchSymm.contains(bs))&&(asymm.at(i).molindex>0)) {
	    brauchSymm.append(bs);
	  }
	}
      }
    }
  }
  showatoms.clear();
  for (int i=0; i<asymm.size();i++){
    showatoms.append(asymm[i]);
    showatoms[i].molindex=asymm[i].molindex;
  }
  packer(brauchSymm);
  showbonds.clear();
  showbonds=connecting(showatoms);
}


void Molecule::complete(){
  /*! If the current visible atoms are covalently bound to  symmetry equivalents the latter are added to the showatoms list.
   * */
  //if (index>=showatoms.size()) return;
//  V3 expander= showatoms.at(index).frac;
  V3 prime,floorD,D;
  QStringList brauchSymm;
  QString bs;
//  brauchSymm.clear();
  brauchSymm+=(usedSymmetry);
  double dk;
  int s,h,k,l,symmgroup;
  V3 pos0;
  for (int sy=0; sy<usedSymmetry.size(); sy++){ 
    sscanf(usedSymmetry.at(sy).toLatin1(),"%d_%1d%1d%1d:%d",&s,&h,&k,&l,&symmgroup);
    h-=5;
    k-=5;
    l-=5;
    s--;
    for (int j=0; j<asymm.size();j++){
      if (asymm.at(j).an<0) continue;
      for (int n=0; n<cell.symmops.size();n++){
	for (int i=0; i<asymm.size();i++){
	  if (asymm.at(i).an<1) continue;
	  if (asymm[i].molindex!=symmgroup) continue;
	  pos0=cell.symmops.at(s)*asymm.at(i).frac+cell.trans.at(s)+V3(h,k,l);
	  prime=cell.symmops.at(n) * asymm.at(j).frac + cell.trans.at(n);
	  D=prime - pos0 + V3(0.5,0.5,0.5) ;
	  floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	  if ((n==0)&&(floorD==V3(0,0,0))) {continue;}
	  D=D - floorD - V3(0.5,0.5,0.5);
	  dk=fl(D.x,D.y,D.z);
	  double dddd=qMin(1.7,(Kovalenz_Radien[asymm.at(i).an]+ Kovalenz_Radien[asymm.at(j).an])*0.012);
	  if (dk<dddd){
	    bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm.at(i).molindex);
	    if  ((!brauchSymm.contains(bs))&&(asymm.at(i).molindex>0)) {
	      brauchSymm.append(bs);
	    }
	  }
	}
      }
    }
  }
  showatoms.clear();
  for (int i=0; i<asymm.size();i++){
    showatoms.append(asymm[i]);
    showatoms[i].molindex=asymm[i].molindex;
  }
  packer(brauchSymm);
  showbonds.clear();
  showbonds=connecting(showatoms);
}

void Molecule::expandAt(int index){
  /*! searches SUCHRAD=3.2 A around the given atom for symmetry equivalent molecules and adds them to the showatoms list.
   *  if Hydrogens are inside the structure the SUCHRAD is reduced about 0.7 A.
   *  @param index index of the specified atom from the current showatoms list.
   */
  if (index>=showatoms.size()) return;
  V3 expander= showatoms.at(index).frac;
  V3 prime,floorD,D;
  double SUCHRAD=3.2;
  for (int i=0; i<asymm.size();i++){//Rapunzel lass dein H herunter
    if (asymm.at(i).an==0) {
      SUCHRAD-=0.7;
      break;
    }
  }
  //printf(" Rapunzels H ist %f lang\n",SUCHRAD);
  QStringList brauchSymm;
  QString bs;
  brauchSymm.clear();
  brauchSymm+=(usedSymmetry);
  double dk;
  for (int n=0; n<cell.symmops.size();n++){
    for (int i=0; i<asymm.size();i++){
      prime=cell.symmops.at(n) * asymm.at(i).frac + cell.trans.at(n);
      D=prime - expander + V3(0.5,0.5,0.5) ;
      floorD=V3(floor(D.x),floor(D.y),floor(D.z));
      if ((n==0)&&(floorD==V3(0,0,0))) {continue;}
      D=D - floorD - V3(0.5,0.5,0.5);
      dk=fl(D.x,D.y,D.z);
      if (dk<SUCHRAD){
	bs=QString("%1_%2%3%4:%5,").arg(n+1).arg(5-(int)floorD.x).arg(5-(int)floorD.y).arg(5-(int)floorD.z).arg(asymm.at(i).molindex);
        if  ((!brauchSymm.contains(bs))&&(asymm.at(i).molindex>0)) {
	  brauchSymm.append(bs);
	}
      }
    }
  }
  showatoms.clear();
  for (int i=0; i<asymm.size();i++){
    showatoms.append(asymm[i]);
    showatoms[i].molindex=asymm[i].molindex;
  }
  packer(brauchSymm);
  showbonds.clear();
  showbonds=connecting(showatoms);
}

void Molecule::packer(QStringList brauchSymm){
  /*! Packs symmetry equivalent atoms according to the given list of internal symmetry codes and adds them to the showatoms list
   * HumanSymmetry is feeeded with a human readalble list of used symmetry.
   * @param brauchSymm list of internal symmetry codes.
   */
  usedSymmetry.clear();
  usedSymmetry+=(brauchSymm);
  MyAtom  newAtom;
  newAtom.hidden=0;
  int s,h,k,l,gibscho=0,symmgroup;
  //      balken->setMinimum(0);
  //      balken->setMaximum(brauchSymm.size());
  //      balken->show();
  HumanSymmetry=QString("Used Symmetry:<br>%1").arg(symmcode2human(brauchSymm));
  QString pre,suff;
  for (int j=0;j<brauchSymm.size();j++){
      //	balken->setValue(j);

      sscanf(brauchSymm.at(j).toLatin1(),"%d_%1d%1d%1d:%d",&s,&h,&k,&l,&symmgroup);
      //printf("BS:!%s! %d h%d k%d l%d sg%d\n",brauchSymm.at(j).toStdString().c_str(),s,h,k,l,symmgroup);
      h-=5;
      k-=5;
      l-=5;
      s--;
      for (int i=0;i<asymm.size();i++){
          // if (asymm[i].molindex) printf ("doch %d %d %s\n",asymm[i].an,asymm[i].molindex,asymm.at(i).Label.toStdString().c_str());
          if ((asymm[i].molindex==symmgroup)&&(asymm[i].an>-1)){
              newAtom.frac=cell.symmops.at(s)*asymm[i].frac+cell.trans.at(s)+V3(h,k,l);
              newAtom.part=asymm[i].part;
              frac2kart(newAtom.frac,newAtom.pos);
              newAtom.Label=QString("%1%2%3")
                            .arg(asymm.at(i).Label)
                            .arg(QString::fromUtf8("»"))
                            .arg(j+1);// */
              newAtom.an=asymm[i].an;
              newAtom.symmGroup=j+1;
              newAtom.sof_org=asymm[i].sof_org;
              newAtom.molindex=asymm[i].molindex;
              newAtom.ResiClass=asymm[i].ResiClass;
              newAtom.resiNr=asymm[i].resiNr;
              if ((asymm[i].uc.m12==0.0 )&&(asymm[i].uc.m23==0.0)&&(asymm[i].uc.m13==0.0)){
                  newAtom.uc.m11=newAtom.uc.m22=newAtom.uc.m33=asymm[i].uf.m11;
                  newAtom.uc.m12=newAtom.uc.m13=newAtom.uc.m23=newAtom.uc.m21=newAtom.uc.m31=newAtom.uc.m32=0.0;
		  newAtom.uf=asymm[i].uf;
              }
              else {
                  Usym(asymm[i].uf,cell.symmops[s],newAtom.uf);
                  Uf2Uo(newAtom.uf,newAtom.uc);
              }
              gibscho=0;
	      if (newAtom.part>=0){
                  for(int gbt=0;gbt<showatoms.size();gbt++){
                      if (showatoms.at(gbt).an<0) continue;
                      if (showatoms.at(gbt).part!=newAtom.part)continue;
                      if (fl(newAtom.frac.x-showatoms[gbt].frac.x,
                             newAtom.frac.y-showatoms[gbt].frac.y,
                             newAtom.frac.z-showatoms[gbt].frac.z)<0.2) gibscho=1;
                  }
              }
              if (!gibscho) {
                showatoms.append(newAtom);
              }
          }// /*
          else if ((growQPeak)&&(asymm[i].an==-1)&&(asymm[i].molindex==symmgroup)){
            newAtom.frac=cell.symmops.at(s)*asymm[i].frac+cell.trans.at(s)+V3(h,k,l);
            newAtom.part=asymm[i].part;
            frac2kart(newAtom.frac,newAtom.pos);
            newAtom.Label=asymm.at(i).Label;
            newAtom.an=asymm[i].an;
            //newAtom.symmGroup=j+1;
	    newAtom.sof_org=asymm[i].sof_org;
	    newAtom.sof=asymm[i].sof;
            newAtom.molindex=asymm[i].molindex;
            newAtom.ResiClass=asymm[i].ResiClass;
            newAtom.resiNr=asymm[i].resiNr;
	    newAtom.peakHeight=asymm[i].peakHeight;
	    newAtom.orginalLine= asymm[i].orginalLine;
	    gibscho=0;
	    if (newAtom.part>=0){
	      for(int gbt=0;gbt<showatoms.size();gbt++){
                if (fl(newAtom.frac.x-showatoms[gbt].frac.x,
                       newAtom.frac.y-showatoms[gbt].frac.y,
                       newAtom.frac.z-showatoms[gbt].frac.z)<0.1) gibscho=1;
              }
	    }
            if (!gibscho) {
                showatoms.append(newAtom);
            }
        }// */
      }
  }
      //statusBar()->showMessage(tr("Neighbor search is finished"));


}
