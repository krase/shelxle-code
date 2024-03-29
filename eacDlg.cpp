
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
#include "eacDlg.h"
#if defined (Q_WS_MAC)
#include <glu.h>
#else
#include <GL/glu.h>
#endif
#include "itsme.h"
/*
   extern void __RotateCS( double c, double s, double& X, double& Y );
   extern void glTranslateL( const double dx, const double dy, const double dz );
   extern void glRotateL( const double dang, const double x, const double y, const double z );
   */

EacGLW::EacGLW(Molecule *externMole, QWidget * parent){
  this->setParent(parent);
  mol=externMole;
  exmin=mol->pmin;
  exmax=mol->pmax;
  mol->pmax=0.99;
  mol->pmin=0.39;
  //mol->copyAcol(mol->AtomColor,mol->arad,mol->AtomStyle);
  //mol->loadSettings();
  proba=50;
  MyAtom xda;
  xda.symmGroup=0;
  xda.hidden=0;
  xda.part=0;
  //xda.ownStyle=false;
  xda.Label="H";
  xda.pos.x=0;
  xda.pos.y=0;
  xda.pos.z=0;
  xda.uc.m11=0.1;
  xda.uc.m22=0.07;
  xda.uc.m33=0.025;
  xda.uc.m13=xda.uc.m31=0.01;
  xda.uc.m12=xda.uc.m21=-0.02;
  xda.uc.m23=xda.uc.m32=0.01;
  xda.an=6;
  xd.append(xda);

  xda.Label="H1(noADP)";
  xda.pos.x=0.00001;
  xda.pos.y=0.00001;
  xda.pos.z=1.0;
  // xda.ownStyle=true;
  //xda.style=ATOM_STYLE_NOADP;
  //xda.rad=mol->arad[0];
  //xda.color=mol->AtomColor[0];
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=0;
  xd.append(xda);

  xda.Label="H2";
  //xda.ownStyle=false;
  xda.pos.x=0.81649658102978810529098386315336;
  xda.pos.y=-0.4714045208499572479996295714055;
  xda.pos.z=-0.333333;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=0;
  xd.append(xda);

  xda.Label="H3";
  xda.pos.x=0;
  xda.pos.y=0.94280904169991449599925914281199;
  xda.pos.z=-0.333333;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=0;
  xd.append(xda);

  xda.Label="H4";
  xda.pos.x=-0.81649658102978810529098386315336;
  xda.pos.y=-0.4714045208499572479996295714055;
  xda.pos.z=-0.333333;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=0;
  xd.append(xda);
  xda.Label="Q1";
  xda.pos.x=-0.81649658102978810529098386315336;
  xda.pos.y=-0.4714045208499572479996295714055;
  xda.pos.z=0.333333;
  xda.peakHeight=0.91;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=-1;
  xd.append(xda);
  xda.Label="Q2";
  xda.pos.x=0.81649658102978810529098386315336;
  xda.pos.y=0.4714045208499572479996295714055;
  xda.pos.z=0.333333;
  xda.peakHeight=0.41;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=-1;
  xd.append(xda);
  xda.Label="Q3";
  xda.pos.x=0;
  xda.pos.y=-0.94280904169991449599925914281199;
  xda.pos.z=0.333333;
  xda.peakHeight=0.71;
  xda.uc.m11=0.05;
  xda.uc.m22=0.05;
  xda.uc.m33=0.05;
  xda.uc.m13=0.0;
  xda.uc.m12=0.0;
  xda.uc.m23=0.0;
  xda.an=-1;
  xd.append(xda);
  bonds=mol->connecting(xd,true);
  xd[0].an=0;//das ist ein ... trick um bindungen vom zentralatom zu den liganden zu erzeugen.
  /*

     MyBond b;
     b.ato1=&xd[0];
     b.ato2=&xd[1];
     b.a1=0;
     b.a2=1;
     b.length=1;
     bonds.append(b);
     b.ato1=&xd[0];
     b.ato2=&xd[2];
     b.a1=0;
     b.a2=2;
     b.length=1;
     bonds.append(b);
     b.ato1=&xd[0];
     b.ato2=&xd[3];
     b.a1=0;
     b.a2=3;
     b.length=1;
     bonds.append(b);
     b.ato1=&xd[0];
     b.ato2=&xd[4];
     b.a1=0;
     b.a2=4;
     b.length=1;
     bonds.append(b);
     */
}

void EacGLW::draw(){
  if (mol->LOD>3) {
    const GLfloat  OBJ_SPE[]   = { 0.8, 0.8, 0.8, 1.0 };  
    const GLfloat  OBJ_SHIN    = 127.0;               
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,             OBJ_SPE  );
    glEnable     ( GL_COLOR_MATERIAL ) ;
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;    
    glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS,           OBJ_SHIN );
  }else{
    const GLfloat  OBJ_SPE[]   = { 0.0, 0.0, 0.0, 1.0 };  
    const GLfloat  OBJ_SHIN    = 1.0;               
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,             OBJ_SPE  );
    glEnable     ( GL_COLOR_MATERIAL ) ;
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;    
    glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS,           OBJ_SHIN );
  }
  static double rtx=2.0;  
  glRotated(rtx,1.0,0.0,0.0);
  glRotated(rtx,0.0,1.0,0.0);
  rtx+=0.5;
  mol->dratom=0;
  //mol->nonPositiveDefinite=0;
//  mol->parthigh=false;
  glPushMatrix();
  glScaled( 50, 50, 50 );
  mol->adp=1;
  mol->intern=1;
  mol->tubes=0;
  mol->atoms(xd,proba);
  if (mol->bondColorStyle){
    qglColor(mol->bondColor);
    glDisable(GL_COLOR_MATERIAL);
  }
  mol->bonds(bonds);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);
  mol->intern=0;
  mol->dratom=0;
  mol->atoms(xd,proba);
  mol->lbond();
  glDisable(GL_BLEND);
  glColor3f(0,0,0);
  glClear( GL_DEPTH_BUFFER_BIT);
  for (int i=0; i<xd.size();i++){
    if ((i==1)||(!(mol->AtomStyle[xd[i].an]&ATOM_STYLE_NOLABEL)))
      renderText( xd.at(i).pos.x,xd.at(i).pos.y,xd.at(i).pos.z, xd.at(i).Label,myFont);
  }
  glPopMatrix();
}
void EacGLW::initializeGL(){
  glEnable(GL_LINE_SMOOTH);  
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

  timer = new QTimer(this);
  connect( timer, SIGNAL(timeout()),
		  this, SLOT(updateGL()) );
  // Jede Sekunde updateTime() aufrufen
  timer->start(20);

  //glEnable(GL_POLYGON_SMOOTH);   
  myFont= QFont("Arial", 18, -1, false);
  const GLfloat  position[] = {100.0f, 100.0f,100.0f,0.0f};
  const GLfloat  diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
  const GLfloat  specular[] = { 1.0, 0.9, 0.9, 1.0 };
  const GLfloat  ambient[]  = { 0.4, 0.4, 0.4, 1.0 };

  glLightModeli(  GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );

  glLightfv( GL_LIGHT0, GL_POSITION, position );
  glLightfv( GL_LIGHT0, GL_AMBIENT,  ambient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );

  glLightfv( GL_LIGHT1, GL_POSITION, position );
  glLightfv( GL_LIGHT1, GL_DIFFUSE,  diffuse );  
  glLightfv( GL_LIGHT1, GL_AMBIENT,  ambient );

  glLightfv( GL_LIGHT2, GL_DIFFUSE,  diffuse );  
  mol->adpwall_plaid=bindTexture(QImage(QString(":/icons/adpwall.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  mol->adpwall=bindTexture(QImage(QString(":/icons/adpwall3.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  mol->hbtex=bindTexture(QImage(QString(":/icons/hbb.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  //  glEnable( GL_BLEND);
  glDisable(GL_BLEND);
  glAlphaFunc ( GL_GREATER, 0.01f ) ;
  //glEnable(GL_ALPHA_TEST);
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
  const GLfloat  OBJ_SPE[]   = { 1.0, 1.0, 1.0, 1.0 };  
  const GLfloat  OBJ_SHIN    = 127.0;                   
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,             OBJ_SPE  );
  glEnable     ( GL_COLOR_MATERIAL ) ;
  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;    
  glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS,           OBJ_SHIN );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_NORMALIZE);
  glClearColor(0.4,0.4,0.4,1.0);
  glEnable(GL_DEPTH_TEST );
  glDepthFunc(GL_LEQUAL);
  gluLookAt(0.0, 200, 50 ,   0.0, 0.0, 0.0,   0.0, -100, 400 );
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void EacGLW::resizeGL(int width, int height){
  glViewport(0, 0, width, height);        
  glGetIntegerv(GL_VIEWPORT, vp);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 29.0, (double)width/height, 5.0, 8000.0 );
}

void EacGLW::paintGL(){
  glClearColor(0.8,0.8,0.8,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, QGLWidget::width(), QGLWidget::height());        
  glGetIntegerv(GL_VIEWPORT, vp);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 29.0, (double)QGLWidget::width()/QGLWidget::height(), 5.0, 8000.0 );
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  draw();
  glPopMatrix();
}
void EacGLW::changeAtom(int OZ){
  xd[0].Label=mol->pse(OZ);
  xd[0].an=OZ;
  xd[1].Label=QString("%1(noADP)").arg(mol->pse(OZ));
  xd[1].an=OZ;
  //xd[1].ownStyle=true;
  //xd[1].style=ATOM_STYLE_NOADP;
  //xd[1].rad=mol->arad[OZ];
  //xd[1].color=mol->AtomColor[OZ];
  updateGL();
}
void EacGLW::colorDLG(){
  QColor bgcolor;    
  bgcolor=QColorDialog::getColor(mol->AtomColor[xd.at(0).an], this);
  if (bgcolor.isValid()) {
    mol->AtomColor[xd.at(0).an]=bgcolor;
    mol->AtomColor[xd.at(0).an].setAlpha(127);    
    xd[1].an=xd.at(0).an;
    mol->einstellung->beginGroup("Atoms");
    mol->einstellung->beginWriteArray("AtomColors",107);
    for (int i = 0; i < 107; ++i) {
      mol->einstellung->setArrayIndex(i);
      mol->einstellung->setValue("Color", mol->AtomColor[i]);
    }
    mol->einstellung->endArray();
    mol->einstellung->endGroup();
    mol->einstellung->sync();
    emit colorchanged(bgcolor);

  }
}

void EacDlg::colorchange(QColor bgcolor){
  colorbut->setStyleSheet(QString(
			  "QPushButton {"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg(bgcolor.name())
		  .arg(bgcolor.darker(200).name())
		  .arg(bgcolor.lighter(200).name())
		  .arg((bgcolor.value()<90)?"#ffffff":"#000000"));

}

void EacDlg::colorDLG2(){
  QColor bgcolor;
  bgcolor=QColorDialog::getColor(mol->bondColor, this);
  if (bgcolor.isValid()) {
    mol->bondColor=eacGLW->mol->bondColor=bgcolor;

    colorbut2->setStyleSheet(QString(
			    "QPushButton {"
			    "border: 1px solid #000000;"
			    "border-radius: 9px;"
			    "color: %4;"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			    "}"
			    "QPushButton:hover {"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			    "}"
			    "QPushButton:flat {"
			    "    border: none; /* no border for a flat push button */"
			    "}"
			    )
		    .arg(bgcolor.name())
		    .arg(bgcolor.darker(200).name())
		    .arg(bgcolor.lighter(200).name())
		    .arg((bgcolor.value()<90)?"#ffffff":"#000000"));
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);

  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  }
  update();
}

void EacDlg::colorDLGenvidef(){
  QColor bgcolor;
  bgcolor=QColorDialog::getColor(mol->enviDefaultColor, this);
  if (bgcolor.isValid()) {
    mol->enviDefaultColor=eacGLW->mol->enviDefaultColor=bgcolor;

    colorbutEdf->setStyleSheet(QString(
                "QPushButton {"
                "border: 1px solid #000000;"
                "border-radius: 9px;"
                "color: %4;"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
                "}"
                "QPushButton:hover {"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
                "}"
                "QPushButton:flat {"
                "    border: none; /* no border for a flat push button */"
                "}"
                )
            .arg(bgcolor.name())
            .arg(bgcolor.darker(200).name())
            .arg(bgcolor.lighter(200).name())
            .arg((bgcolor.value()<90)?"#ffffff":"#000000"));
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);

  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  }
  update();
}
void EacDlg::colorDLGenvibon(){
  QColor bgcolor;
  bgcolor=QColorDialog::getColor(mol->enviBondColor, this);
  if (bgcolor.isValid()) {
    mol->enviBondColor=eacGLW->mol->enviBondColor=bgcolor;

    colorbutEbo->setStyleSheet(QString(
                "QPushButton {"
                "border: 1px solid #000000;"
                "border-radius: 9px;"
                "color: %4;"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
                "}"
                "QPushButton:hover {"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
                "}"
                "QPushButton:flat {"
                "    border: none; /* no border for a flat push button */"
                "}"
                )
            .arg(bgcolor.name())
            .arg(bgcolor.darker(200).name())
            .arg(bgcolor.lighter(200).name())
            .arg((bgcolor.value()<90)?"#ffffff":"#000000"));
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);

  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  }
  update();
}

void EacDlg::colorDLGenvihbo(){
  QColor bgcolor;
  bgcolor=QColorDialog::getColor(mol->enviHBColor, this);
  if (bgcolor.isValid()) {
    mol->enviHBColor=eacGLW->mol->enviHBColor=bgcolor;

    colorbutEhb->setStyleSheet(QString(
                "QPushButton {"
                "border: 1px solid #000000;"
                "border-radius: 9px;"
                "color: %4;"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
                "}"
                "QPushButton:hover {"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
                "}"
                "QPushButton:flat {"
                "    border: none; /* no border for a flat push button */"
                "}"
                )
            .arg(bgcolor.name())
            .arg(bgcolor.darker(200).name())
            .arg(bgcolor.lighter(200).name())
            .arg((bgcolor.value()<90)?"#ffffff":"#000000"));
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);

  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  }
  update();
}
void EacGLW::wallA(int st){
  if (st) mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_WALLS;
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_WALLS) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_WALLS;
  if (st==Qt::PartiallyChecked) mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_PLAID;
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_PLAID) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_PLAID;

}
void EacGLW::ringA(int st){
  if (st==Qt::PartiallyChecked) mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_WHITERING;
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_WHITERING) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_WHITERING;
  if (st) mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_RINGS;
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_RINGS) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_RINGS;

}
void EacGLW::spheA(int st){
  if (st) mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_SPHERE;
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_SPHERE) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_SPHERE;

}
void EacGLW::soldA(int st){
  if (st) {
    mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_SOLID;
  }
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_SOLID) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_SOLID;

}
void EacDlg::woADP(bool b){
  walbx->setDisabled(b);
  rinbx->setDisabled(b);
  sphbx->setDisabled(b);
  sldbx->setDisabled(b);
  if (b) {
    eacGLW->mol->AtomStyle[eacGLW->xd.at(0).an]|=ATOM_STYLE_NOADP;
    sldbx->setChecked(b);
  }
  else
    if (eacGLW->mol->AtomStyle[eacGLW->xd.at(0).an]&ATOM_STYLE_NOADP)
      eacGLW->mol->AtomStyle[eacGLW->xd.at(0).an]-=ATOM_STYLE_NOADP;
}

void EacGLW::whitA(int st){
  if (st) {
    mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_WHITERING;
  }
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_WHITERING) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_WHITERING;
}

void EacGLW::nolaA(int st){
  if (st) {
    mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_NOLABEL;
  }
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_NOLABEL) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_NOLABEL;
}

void EacGLW::metalA(int st){
  if (st) {
    mol->AtomStyle[xd.at(0).an]|=ATOM_STYLE_METAL;
  }
  else if (mol->AtomStyle[xd.at(0).an]&ATOM_STYLE_METAL) mol->AtomStyle[xd.at(0).an]-=ATOM_STYLE_METAL;
}
void EacDlg::checkWall(){
  if (sldbx->checkState()&&(walbx->checkState()))walbx->setCheckState(Qt::Unchecked);
}
void EacDlg::checkSolid(){
  if (sldbx->checkState()&&(walbx->checkState()))sldbx->setCheckState(Qt::Unchecked);
}
void EacDlg::checkBall(){
  if (sphbx->checkState())sldbx->setEnabled(true);
  else sldbx->setEnabled(false);
}
void EacDlg::updateLEs(int OZ){
  eacKRAD->setValue(eacGLW->mol->Kovalenz_Radien[OZ]/100.0);
  eacRAD->setValue(eacGLW->mol->arad[OZ]);
  rinbx->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_RINGS)?((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_WHITERING)?Qt::PartiallyChecked :Qt::Checked):Qt::Unchecked);

  sphbx->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_SPHERE)?Qt::Checked:Qt::Unchecked);
  sldbx->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_SOLID)?Qt::Checked:Qt::Unchecked);
  walbx->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_WALLS)?((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_PLAID)?Qt::PartiallyChecked: Qt::Checked):Qt::Unchecked);
  nlabx->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_NOLABEL)?Qt::Checked:Qt::Unchecked);
  metalic->setCheckState((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_METAL)?Qt::Checked:Qt::Unchecked);
  noadp->setChecked((eacGLW->mol->AtomStyle[OZ]&ATOM_STYLE_NOADP)?true:false);
  woADP(noadp->isChecked());
  colorbut->setStyleSheet(QString(
			  "QPushButton {"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg(mol->AtomColor[OZ].name())
		  .arg(mol->AtomColor[OZ].darker(200).name())
		  .arg(mol->AtomColor[OZ].lighter(200).name())
		  .arg((mol->AtomColor[OZ].value()<90)?"#ffffff":"#000000"));

  update();
}

void EacDlg::probaChanged(int ix){
  eacGLW->proba=mol->proba=eacProba->itemData(ix).toInt() ;
  update();
}

void EacDlg::unicolor(bool b){
  eacGLW->mol->bondColor = mol->bondColor;
  eacGLW->mol->bondColorStyle=mol->bondColorStyle=(b)?1:0;
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);
  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);

  mol->einstellung->endGroup();
  update();
}

void EacDlg::strengthChanged(double d){
  eacGLW->mol->bondStrength=mol->bondStrength=d;
  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);
  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  update();
}

void EacDlg::QPeakRadChanged(double d){
eacGLW->mol->qPeakRad=mol->qPeakRad=d;
  mol->einstellung->beginGroup("QPeaks");
  mol->einstellung->setValue("QPeaksRadius",mol->qPeakRad);
  mol->einstellung->endGroup();
//settings...
update();	
}

void EacDlg::lodChanged(int d){
  eacGLW->mol->LOD=d;
  mol->einstellung->beginGroup("Window");
  mol->einstellung->setValue("LevelOfDetail",mol->LOD);
  mol->einstellung->endGroup();

  update();
}

void EacDlg::importSettings(){
  mol->einstellung->beginGroup("Files");
  QString dn=mol->einstellung->value("lastFile").toString();
  mol->einstellung->endGroup();
  if (!dn.isEmpty()) {
    dn.replace(QRegExp(".ins$"),".ini");
    dn.replace(QRegExp(".res$"),".ini");
  }
  QString fn = QFileDialog::getOpenFileName (this,"Exporting atom style settings...",dn,"Settings (*.ini)");
  if (fn.isNull())return;
  QSettings *eaSettings = new QSettings(fn,QSettings::IniFormat);

  //-------------------------
  //
  eaSettings->beginGroup("Atoms");
  int ri=eaSettings->beginReadArray("CovaleceRadii");
  for (int i = 0; i < ri; ++i) {
    eaSettings->setArrayIndex(i);
    mol->Kovalenz_Radien[i]=eaSettings->value("Radius").toInt();
  }
  eaSettings->endArray();

  ri = eaSettings->beginReadArray("BallRadii");
  for (int i = 0; i < ri; ++i) {
    eaSettings->setArrayIndex(i);
    mol->arad[i]=eaSettings->value("Radius").toDouble();
  }
  eaSettings->endArray();
  ri = eaSettings->beginReadArray("AtomStyles");
  for (int i = 0; i < ri; ++i) {
    eaSettings->setArrayIndex(i);
   eacGLW->mol->AtomStyle[i]=eaSettings->value("Style" ).toInt();
  }
  eaSettings->endArray();
  ri = eaSettings->beginReadArray("AtomColors");
  for (int i = 0; i < ri; ++i) {
    eaSettings->setArrayIndex(i);
    mol->AtomColor[i]=eaSettings->value("Color").value<QColor>();
    mol->AtomColor[i].setAlphaF(0.5);;
  }
  eaSettings->endArray();
  eaSettings->endGroup();

  eaSettings->beginGroup("Bonds");
  mol->bondColor = eaSettings->value("BondColor").value<QColor>();
  mol->bondColorStyle = eaSettings->value("BondStyle").toInt();;
  mol->bondStrength = eaSettings->value("BondStrength").toDouble();

  mol->enviBondColor=eaSettings->value("enviBondColor",Qt::darkGreen).value<QColor>();
  mol->enviHBColor=eaSettings->value("enviHBColor",Qt::darkYellow).value<QColor>();
  mol->enviDefaultColor=eaSettings->value("enviDefaultColor",Qt::black).value<QColor>();
  
  eaSettings->endGroup();
  delete eaSettings;

  //
  mol->einstellung->beginGroup("Atoms");
  mol->einstellung->beginWriteArray("CovaleceRadii",95);
  for (int i = 0; i < 95; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Radius", mol->Kovalenz_Radien[i]);
  }
  mol->einstellung->endArray();

  mol->einstellung->beginWriteArray("BallRadii");
  for (int i = 0; i < 109; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Radius",mol->arad[i]);
  }
  mol->einstellung->endArray();
  mol->einstellung->beginWriteArray("AtomStyles",107);
  for (int i = 0; i < 107; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Style", eacGLW->mol->AtomStyle[i]);
  }
  mol->einstellung->endArray();
  int asel=psew->selection; 
  mol->einstellung->beginWriteArray("AtomColors",107);
  for (int i = 0; i < 107; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Color", mol->AtomColor[i]);
    psew->selection=i;
    psew->colorchange(mol->AtomColor[i]);
  }
  psew->selection=asel;
  mol->einstellung->endArray();
  mol->einstellung->endGroup();

  mol->einstellung->beginGroup("Bonds");
  mol->einstellung->setValue("BondColor",mol->bondColor);
  mol->einstellung->setValue("BondStyle",mol->bondColorStyle);
  mol->einstellung->setValue("BondStrength",mol->bondStrength);
  mol->einstellung->setValue("enviBondColor",mol->enviBondColor);
  mol->einstellung->setValue("enviHBColor",mol->enviHBColor);
  mol->einstellung->setValue("enviDefaultColor",mol->enviDefaultColor);
  mol->einstellung->endGroup();
  //--------------------------------------
}

void EacDlg::exportSettings(){
  mol->einstellung->beginGroup("Files");
  QString dn=mol->einstellung->value("lastFile").toString();
  mol->einstellung->endGroup();
  if (!dn.isEmpty()) {
    dn.replace(QRegExp(".ins$"),".ini");
    dn.replace(QRegExp(".res$"),".ini");
  }
  QString fn = QFileDialog::getSaveFileName (this,"Exporting atom style settings...",dn,"Settings (*.ini)");
  QSettings *eaSettings = new QSettings(fn,QSettings::IniFormat);
  //-------------------------
  //
  eaSettings->beginGroup("Atoms");
  eaSettings->beginWriteArray("CovaleceRadii",95);
  for (int i = 0; i < 95; ++i) {
    eaSettings->setArrayIndex(i);
    eaSettings->setValue("Radius", mol->Kovalenz_Radien[i]);
  }
  eaSettings->endArray();

  eaSettings->beginWriteArray("BallRadii");
  for (int i = 0; i < 109; ++i) {
    eaSettings->setArrayIndex(i);
    eaSettings->setValue("Radius",mol->arad[i]);
  }
  eaSettings->endArray();
  eaSettings->beginWriteArray("AtomStyles",107);
  for (int i = 0; i < 107; ++i) {
    eaSettings->setArrayIndex(i);
    eaSettings->setValue("Style", eacGLW->mol->AtomStyle[i]);
  }
  eaSettings->endArray();
  eaSettings->beginWriteArray("AtomColors",107);
  for (int i = 0; i < 107; ++i) {
    eaSettings->setArrayIndex(i);
    eaSettings->setValue("Color", mol->AtomColor[i]);
  }
  eaSettings->endArray();
  eaSettings->endGroup();

  eaSettings->beginGroup("Bonds");
  eaSettings->setValue("BondColor",mol->bondColor);
  eaSettings->setValue("BondStyle",mol->bondColorStyle);
  eaSettings->setValue("BondStrength",mol->bondStrength);
  eaSettings->setValue("enviBondColor",mol->enviBondColor);
  eaSettings->setValue("enviHBColor",mol->enviHBColor);
  eaSettings->setValue("enviDefaultColor",mol->enviDefaultColor);

  eaSettings->setValue("HBondMaxDist",mol->hbdist());
  eaSettings->value("HBondMaxAng",mol->hbangl());
  eaSettings->endGroup();
  delete eaSettings;
  //--------------------------------------
  //
}

void EacDlg::kradChanged(double d){
  eacGLW->mol->Kovalenz_Radien[psew->selection]=mol->Kovalenz_Radien[psew->selection]=(int)(d*100);
  mol->einstellung->beginGroup("Atoms");
  mol->einstellung->beginWriteArray("CovaleceRadii",95);
  for (int i = 0; i < 95; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Radius", mol->Kovalenz_Radien[i]);
  }
  mol->einstellung->endArray();
  mol->einstellung->endGroup();
  mol->einstellung->sync();
  update();
}


void EacDlg::hbdistChanged(double){
    mol->einstellung->beginGroup("Bonds");
    mol->setHBondMaxDist(eacHBDist->value());
    mol->einstellung->setValue("HBondMaxDist",eacHBDist->value());
    mol->setHBondMaxAngl(eacHBAngl->value());
    mol->einstellung->setValue("HBondMaxAng",eacHBAngl->value());
    mol->einstellung->endGroup();
    mol->einstellung->sync();
    update();
}

void EacDlg::hbanglChanged(double){
    mol->einstellung->beginGroup("Bonds");
    mol->setHBondMaxDist(eacHBDist->value());
    mol->einstellung->setValue("HBondMaxDist",eacHBDist->value());
    mol->setHBondMaxAngl(eacHBAngl->value());
    mol->einstellung->setValue("HBondMaxAng",eacHBAngl->value());
    mol->einstellung->endGroup();
    mol->einstellung->sync();
    update();
}
void EacDlg::radChanged(double d){

  eacGLW->mol->arad[psew->selection] = mol->arad[psew->selection] = d;
  //eacGLW->xd[1].rad=d;
  mol->einstellung->beginGroup("Atoms");

  mol->einstellung->beginWriteArray("BallRadii");
  for (int i = 0; i < 109; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Radius",mol->arad[i]);
  }
  mol->einstellung->endArray();
  mol->einstellung->endGroup();
  mol->einstellung->sync();

  update();
}

EacDlg::EacDlg(Molecule *externMole){
  //otm=NULL;
  mol=externMole;
  mol->loadSettings();
  setWindowTitle("Edit Atom Parameters");  
  //mol->loadSettings();
  eacGLW = new EacGLW(mol,this);
  connect(eacGLW,SIGNAL(colorchanged(QColor)),this,SLOT(colorchange(QColor)));

  eacGLW->proba=mol->proba;
  //eacGLW->m=em;
  eacGLO = new QGridLayout; 
  //eacCB = new QComboBox;
  eacRAD = new QDoubleSpinBox();
  eacKRAD = new QDoubleSpinBox();
  eacHBDist = new QDoubleSpinBox();
  eacHBAngl = new QDoubleSpinBox();
  //QLabel *smbl =new QLabel("Element symbol");
  QLabel *rad =new QLabel("Sphere radius");
  QLabel *krad =new QLabel("Covalent radius");
  QGroupBox *globalStuff = new QGroupBox("global style");
  QVBoxLayout *glt= new QVBoxLayout();
  eacProba = new QComboBox(this);
  eacProba->addItem("90 % Probability",90);
  eacProba->addItem("70 % Probability",70);
  eacProba->addItem("50 % Probability",50);
  eacProba->addItem("30 % Probability",30);
  eacProba->addItem("10 % Probability",10);
  eacProba->setCurrentIndex(2);
  switch (mol->proba){
	  case 10: eacProba->setCurrentIndex(4);break; 
	  case 30: eacProba->setCurrentIndex(3);break; 
	  case 50: eacProba->setCurrentIndex(2);break; 
	  case 70: eacProba->setCurrentIndex(1);break; 
	  case 90: eacProba->setCurrentIndex(0);break; 
  }
 // printf("zwei %d \n",mol->proba);
  connect(eacProba,SIGNAL(currentIndexChanged(int)),this,SLOT(probaChanged(int)));
  QCheckBox *singleC =new QCheckBox("uni colored bonds");
  singleC->setChecked(mol->bondColorStyle);
  unicolor(mol->bondColorStyle);
  connect(singleC,SIGNAL(toggled(bool)),this,SLOT(unicolor(bool)));
  impbut = new QPushButton("Import atom styles");
  expbut = new QPushButton("Export atom styles");
  connect(impbut,SIGNAL(pressed()),this,SLOT(importSettings()));
  connect(expbut,SIGNAL(pressed()),this,SLOT(exportSettings()));
  colorbut2 =new QPushButton("Bond Color");
  colorbut2->setStyleSheet(QString(
			  "QPushButton {"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg(mol->bondColor.name())
		  .arg(mol->bondColor.darker(200).name())
		  .arg(mol->bondColor.lighter(200).name())
		  .arg((mol->bondColor.value()<90)?"#ffffff":"#000000"));
  connect(colorbut2,SIGNAL(pressed()),this,SLOT(colorDLG2()));


  colorbutEdf =new QPushButton("ENVI default color");
  colorbutEdf->setStyleSheet(QString(
              "QPushButton {"
              "border: 1px solid #000000;"
              "border-radius: 9px;"
              "color: %4;"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
              "}"
              "QPushButton:hover {"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
              "border: 0px"
              "}"
              "QPushButton:flat {"
              "    border: none; /* no border for a flat push button */"
              "}"
              )
          .arg(mol->enviDefaultColor.name())
          .arg(mol->enviDefaultColor.darker(200).name())
          .arg(mol->enviDefaultColor.lighter(200).name())
          .arg((mol->enviDefaultColor.value()<90)?"#ffffff":"#000000"));
  connect(colorbutEdf,SIGNAL(pressed()),this,SLOT(colorDLGenvidef()));

  colorbutEbo =new QPushButton("ENVI covalent color");
  colorbutEbo->setStyleSheet(QString(
              "QPushButton {"
              "border: 1px solid #000000;"
              "border-radius: 9px;"
              "color: %4;"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
              "}"
              "QPushButton:hover {"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
              "border: 0px"
              "}"
              "QPushButton:flat {"
              "    border: none; /* no border for a flat push button */"
              "}"
              )
          .arg(mol->enviBondColor.name())
          .arg(mol->enviBondColor.darker(200).name())
          .arg(mol->enviBondColor.lighter(200).name())
          .arg((mol->enviBondColor.value()<90)?"#ffffff":"#000000"));
  connect(colorbutEbo,SIGNAL(pressed()),this,SLOT(colorDLGenvibon()));

  colorbutEhb =new QPushButton("ENVI H-Bond color");
  colorbutEhb->setStyleSheet(QString(
              "QPushButton {"
              "border: 1px solid #000000;"
              "border-radius: 9px;"
              "color: %4;"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
              "}"
              "QPushButton:hover {"
              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
              "border: 0px"
              "}"
              "QPushButton:flat {"
              "    border: none; /* no border for a flat push button */"
              "}"
              )
          .arg(mol->enviHBColor.name())
          .arg(mol->enviHBColor.darker(200).name())
          .arg(mol->enviHBColor.lighter(200).name())
          .arg((mol->enviHBColor.value()<90)?"#ffffff":"#000000"));
  connect(colorbutEhb,SIGNAL(pressed()),this,SLOT(colorDLGenvihbo()));
  bondStrength = new QDoubleSpinBox();
  bondStrength->setValue(mol->bondStrength);
  bondStrength->setDecimals(2);
  bondStrength->setSingleStep(0.01);
  bondStrength->setToolTip("Bond Strength");
  connect(bondStrength,SIGNAL(valueChanged(double)),this,SLOT(strengthChanged(double)) );
  glt->addWidget(eacProba);
  glt->addWidget(singleC);
  glt->addWidget(colorbut2);
  glt->addWidget(colorbutEdf);
  glt->addWidget(colorbutEbo);
  glt->addWidget(colorbutEhb);
  QHBoxLayout *glt2= new QHBoxLayout();
  QHBoxLayout *glt5= new QHBoxLayout();
  QHBoxLayout *glt6= new QHBoxLayout();
  glt2->addWidget(bondStrength);
  QLabel *bsl = new QLabel("Bond strength");
  QLabel *lodl = new QLabel("Level of detail");
  QLabel *qradl = new QLabel("QPeak-size");
  glt2->addWidget(bsl);
  lodSpinBox = new QSpinBox();
  lodSpinBox->setValue(mol->LOD);
  lodSpinBox->setMinimum(2);
  lodSpinBox->setMaximum(15);
  connect(lodSpinBox,SIGNAL(valueChanged(int)),this,SLOT(lodChanged(int)));
  QHBoxLayout *glt3= new QHBoxLayout();
  glt3->addWidget(lodSpinBox);
  glt3->addWidget(lodl);
  glt6->addWidget(eacHBDist);
  QLabel *hbdl = new QLabel("Max H-Acceptor Distance");
  glt6->addWidget(hbdl);
  QLabel *hbal = new QLabel("Min H-Bond Angle");
  glt5->addWidget(eacHBAngl);
  glt5->addWidget(hbal);
  QDoubleSpinBox* qPeakSizeSBx = new QDoubleSpinBox();
  qPeakSizeSBx->setValue(mol->qPeakRad);
  qPeakSizeSBx->setDecimals(3);
  qPeakSizeSBx->setMinimum(0.005);
  qPeakSizeSBx->setMaximum(0.5);
  qPeakSizeSBx->setSingleStep(0.003);
  qPeakSizeSBx->setToolTip("QPeak-size");
  connect(qPeakSizeSBx,SIGNAL(valueChanged(double)),this,SLOT(QPeakRadChanged(double)) );
  QHBoxLayout *glt4= new QHBoxLayout();
  glt4->addWidget(qPeakSizeSBx);
  glt4->addWidget(qradl);

  glt->addLayout(glt2);
  glt->addLayout(glt3);
  glt->addLayout(glt4);
  glt->addLayout(glt6);
  glt->addLayout(glt5);
  globalStuff->setLayout(glt);
  walbx = new QCheckBox("Draw intersecting planes");
  walbx->setTristate();
  rinbx = new QCheckBox("Draw principal ellipses");
  rinbx->setTristate();
  sphbx = new QCheckBox("Draw ellipsoid surface");
  sldbx = new QCheckBox("Solid ellipsoid");
  noadp = new QCheckBox("No ADP");
  metalic = new QCheckBox("Dashed Bonds to others");
  nlabx = new QCheckBox("No label!");
  metalic->setCheckState ( Qt::Unchecked);
  walbx->setCheckState ( Qt::Checked);
  rinbx->setCheckState ( Qt::Checked);
  rinbx->setWhatsThis("If this check box is half selected <font color=gray><b>white</b></font> <b>outer lines</b> will be drawn around the ellipsoid which looks nice when the atom has a dark color.<br> If this is fully selected <b>black outer lines</b> will be drawn around the ellipsoid.");
  sphbx->setCheckState ( Qt::Checked);
  sldbx->setCheckState ( Qt::Unchecked);
  nlabx->setCheckState ( Qt::Unchecked);  
  noadp->setCheckState ( Qt::Unchecked);


  psew = new PSEWidget();
  connect(eacGLW,SIGNAL(colorchanged(QColor)),psew,SLOT(colorchange(QColor)));

  /*  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
      connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
      connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));*/
  connect(noadp,SIGNAL(toggled(bool)),this,SLOT(woADP(bool)));
  QPushButton *multi = new QPushButton("Style multiple Elements");
  colorbut =new QPushButton("Atom Color");
  colorbut->setStyleSheet(QString(
			  "QPushButton {"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg(mol->AtomColor[0].name())
		  .arg(mol->AtomColor[0].darker(200).name())
		  .arg(mol->AtomColor[0].lighter(200).name())
		  .arg((mol->AtomColor[0].value()<90)?"#ffffff":"#000000"));
  eacKRAD->setValue(eacGLW->mol->Kovalenz_Radien[0]/100.0);
  eacKRAD->setDecimals(2);
  eacKRAD->setSingleStep(0.01);
  eacRAD->setValue(eacGLW->mol->arad[0]);
  eacRAD->setDecimals(2);
  eacRAD->setSingleStep(0.01);
  connect(eacKRAD,SIGNAL(valueChanged(double)),this,SLOT(kradChanged(double)) );
  connect(eacRAD,SIGNAL(valueChanged(double)),this,SLOT(radChanged(double)) );
  eacHBDist->setValue(eacGLW->mol->hbdist());
  eacHBDist->setDecimals(2);
  eacHBDist->setSingleStep(0.01);

  eacHBAngl->setMaximum(180.0);
  eacHBAngl->setMinimum(0.0);
  eacHBAngl->setValue(eacGLW->mol->hbangl());
  eacHBAngl->setDecimals(1);
  eacHBAngl->setSingleStep(0.1);

  connect(eacHBDist,SIGNAL(valueChanged(double)),this,SLOT(hbdistChanged(double)) );
  connect(eacHBAngl,SIGNAL(valueChanged(double)),this,SLOT(hbanglChanged(double)) );
  eacGLO->addWidget(eacGLW,0,0,6,6);
  eacGLO->addWidget(globalStuff,0,6,1,3);
  //eacGLO->addWidget(smbl,1,7);
  eacGLO->addWidget(rad,3,7);
  eacGLO->addWidget(krad,4,7);
  //eacGLO->addWidget(eacCB,1,6);
  eacGLO->addWidget(eacRAD,3,6);
  eacGLO->addWidget(eacKRAD,4,6);
  eacGLO->addWidget(colorbut,2,6,1,3);
  eacGLO->addWidget(nlabx,5,6);
  eacGLO->addWidget(multi,4,8);
  eacGLO->addWidget(expbut,7,8);
  eacGLO->addWidget(impbut,7,7);
  eacGLO->addWidget(walbx,6,0);
  eacGLO->addWidget(rinbx,6,2);
  eacGLO->addWidget(sphbx,6,4);
  eacGLO->addWidget(sldbx,6,6);
  eacGLO->addWidget(noadp,6,7);
  eacGLO->addWidget(metalic,6,8);
  //eacGLO->addWidget(buttonBox,7,0,1,8);
  eacGLO->addWidget(psew,1,6,1,3);
  setLayout(eacGLO);
  connect(multi,SIGNAL(pressed()),this , SLOT(doMulti()));
  connect(colorbut,SIGNAL(pressed()),eacGLW,SLOT(colorDLG()));
  connect(psew,SIGNAL(pseSelected(int)), eacGLW, SLOT(changeAtom(int)));
  connect(psew,SIGNAL(pseSelected(int)), this, SLOT(updateLEs(int)));
  updateLEs(0);
  connect(walbx,SIGNAL(stateChanged(int)),eacGLW,SLOT(wallA(int)));

  connect(nlabx,SIGNAL(stateChanged(int)),eacGLW,SLOT(nolaA(int)));
  connect(walbx,SIGNAL(stateChanged(int)),this,SLOT(checkSolid()));
  connect(rinbx,SIGNAL(stateChanged(int)),eacGLW,SLOT(ringA(int)));
  connect(sphbx,SIGNAL(stateChanged(int)),eacGLW,SLOT(spheA(int)));
  connect(sphbx,SIGNAL(stateChanged(int)),this,SLOT(checkBall()));
  connect(sldbx,SIGNAL(stateChanged(int)),eacGLW,SLOT(soldA(int)));
  connect(sldbx,SIGNAL(stateChanged(int)),this,SLOT(checkWall()));
  connect(metalic,SIGNAL(stateChanged(int)),eacGLW,SLOT(metalA(int)));
}

void EacDlg::doMulti(){
  MultiStyle *m = new MultiStyle(mol);
  if (m->exec()==QDialog::Accepted){
    int stt=0;
    if (m->sphbx->checkState())stt|=ATOM_STYLE_SPHERE;
    if (m->rinbx->checkState())stt|=ATOM_STYLE_RINGS;
    if (m->rinbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_WHITERING;
    if (m->walbx->checkState())stt|=ATOM_STYLE_WALLS;
    if (m->walbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_PLAID;
    if (m->sldbx->checkState())stt|=ATOM_STYLE_SOLID;
    if (m->metalic->checkState())stt|=ATOM_STYLE_METAL;
    int mi=qMin(m->start->currentIndex(),m->end->currentIndex()),
	ma=qMax(m->start->currentIndex(),m->end->currentIndex());
    for (int i=mi;i<=ma;i++){
      mol->AtomStyle[i]=stt;    
    }

  }
}

MultiStyle::MultiStyle(Molecule *externMole){
  mol=externMole;
  setWindowTitle("Multiple Elements Styler");  
  QStringList psel;
  psel << "H" << "He" << "Li" << "Be" << "B" << "C" << "N" << "O" << "F" << "Ne" << "Na" << "Mg" << "Al" << "Si" << "P" << "S" << "Cl" << "Ar" <<
	  "K" << "Ca" << "Sc" << "Ti" << "V" << "Cr" << "Mn" << "Fe" << "Co" << "Ni" << "Cu" << "Zn" << "Ga" << "Ge" << "As" << "Se" << "Br" << "Kr" <<
	  "Rb" << "Sr" << "Y" << "Zr" << "Nb" << "Mo" << "Tc" << "Ru" << "Rh" << "Pd" << "Ag" << "Cd" << "In" << "Sn" << "Sb" << "Te" << "I" << "Xe" <<
	  "Cs" << "Ba" <<  "La" << "Ce" << "Pr" << "Nd" << "Pm" << "Sm" << "Eu" << "Gd" << "Tb" << "Dy" << "Ho" << "Er" << "Tm" << "Yb" << "Lu" <<
	  "Hf" << "Ta" << "W" << "Re" << "Os" << "Ir" << "Pt" << "Au" << "Hg" << "Tl" << "Pb" << "Bi" << "Po" << "At" << "Rn" << "Fr" << "Ra" <<
	  "Ac" << "Th" << "Pa" << "U" << "Np" << "Pu" << "Am" << "Cm" << "Bk" << "Cf" << "Es" << "Fm" << "Md" << "No" << "Lr" << "Ku" << "Ha" << "Rf" << "Ns" << "Hs" << "Mt";
  start = new QComboBox();
  end = new QComboBox();
  start->addItems(psel);
  start->setCurrentIndex(2);
  end->addItems(psel);
  end->setCurrentIndex(93);
  QLabel *staL =new QLabel("From: "); 
  QLabel *endL =new QLabel("To: "); 
  walbx = new QCheckBox("Draw intersecting planes");
  walbx->setTristate();
  rinbx = new QCheckBox("Draw principal ellipses");
  rinbx->setTristate();
  sphbx = new QCheckBox("Draw ellipsoid surface");
  sldbx = new QCheckBox("Solid ellipsoid");
  metalic = new QCheckBox("Dashed Bonds to others");
  metalic->setCheckState ( Qt::Unchecked);
  walbx->setCheckState ( Qt::Checked);
  rinbx->setCheckState ( Qt::Checked);
  sphbx->setCheckState ( Qt::Checked);
  sldbx->setCheckState ( Qt::Unchecked);
  QGridLayout *eacGLO = new QGridLayout; 
  eacGLO->addWidget(staL,0,0,1,1,Qt::AlignRight);
  eacGLO->addWidget(endL,0,2,1,1,Qt::AlignRight);
  eacGLO->addWidget(start,0,1);
  eacGLO->addWidget(end,0,3);
  eacGLO->addWidget(walbx,1,0);
  eacGLO->addWidget(rinbx,1,1);
  eacGLO->addWidget(sphbx,1,2);
  eacGLO->addWidget(sldbx,1,3);
  eacGLO->addWidget(metalic,1,4);
  connect(walbx,SIGNAL(stateChanged(int)),this,SLOT(checkSolid()));
  connect(sphbx,SIGNAL(stateChanged(int)),this,SLOT(checkBall()));
  connect(sldbx,SIGNAL(stateChanged(int)),this,SLOT(checkWall()));
  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  eacGLO->addWidget(buttonBox,2,0,1,5);
  //  eacGLO->addWidget(,1,);
  setLayout(eacGLO);
}

void MultiStyle::checkWall(){
  if (sldbx->checkState()&&(walbx->checkState()))walbx->setCheckState(Qt::Unchecked);
}
void MultiStyle::checkSolid(){
  if (sldbx->checkState()&&(walbx->checkState()))sldbx->setCheckState(Qt::Unchecked);
}
void MultiStyle::checkBall(){
  if (sphbx->checkState())sldbx->setEnabled(true);
  else sldbx->setEnabled(false);
}

void EacDlg::reject(){
  eacGLW->timer->stop();
  /*
     int i=eacGLW->xd.at(0).an;
  //  extern molekul mol; 
  mol->arad[i]=eacGLW->mol->arad[i]=eacRAD->text().toDouble();
  int stt=0;
  if (sphbx->checkState())stt|=ATOM_STYLE_SPHERE;
  if (rinbx->checkState())stt|=ATOM_STYLE_RINGS;
  if (rinbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_WHITERING;
  if (walbx->checkState())stt|=ATOM_STYLE_WALLS;
  if (walbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_PLAID;
  if (sldbx->checkState())stt|=ATOM_STYLE_SOLID;
  if (nlabx->checkState())stt|=ATOM_STYLE_NOLABEL;
  if (noadp->checkState())stt|=ATOM_STYLE_NOADP;
  if (metalic->checkState())stt|=ATOM_STYLE_METAL;
  mol->AtomStyle[i]=eacGLW->mol->AtomStyle[i]=stt;
  mol->Kovalenz_Radien[i]=eacGLW->mol->Kovalenz_Radien[i]=(int)(eacKRAD->text().toDouble()*100);*/
  mol->einstellung->beginGroup("Atoms");
  mol->einstellung->beginWriteArray("AtomStyles",107);
  for (int i = 0; i < 107; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Style", eacGLW->mol->AtomStyle[i]);
  }
  mol->einstellung->endArray();
  mol->einstellung->endGroup();
  mol->pmin=eacGLW->exmin;
  mol->pmax=eacGLW->exmax;

  psew->esm->disconnect();
  delete psew->esm;
  psew->disconnect();
  eacGLW->disconnect();
  this->disconnect();
  delete eacGLW;
  done(QDialog::Rejected);
}

void EacDlg::accept(){
  eacGLW->timer->stop();
  int i=eacGLW->xd.at(0).an;
  //  extern molekul mol;
  mol->arad[i]=eacGLW->mol->arad[i]=eacRAD->text().toDouble();
  int stt=0;
  if (sphbx->checkState())stt|=ATOM_STYLE_SPHERE;
  if (rinbx->checkState())stt|=ATOM_STYLE_RINGS;
  if (rinbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_WHITERING;
  if (walbx->checkState())stt|=ATOM_STYLE_WALLS;
  if (walbx->checkState()==Qt::PartiallyChecked)stt|=ATOM_STYLE_PLAID;
  if (sldbx->checkState())stt|=ATOM_STYLE_SOLID;
  if (nlabx->checkState())stt|=ATOM_STYLE_NOLABEL;
  if (noadp->checkState())stt|=ATOM_STYLE_NOADP;
  mol->AtomStyle[i]=eacGLW->mol->AtomStyle[i]=stt;
  mol->Kovalenz_Radien[i]=eacGLW->mol->Kovalenz_Radien[i]=(int)(eacKRAD->text().toDouble()*100);
  mol->einstellung->beginGroup("Atoms");
  mol->einstellung->beginWriteArray("AtomStyles",107);
  for (int i = 0; i < 107; ++i) {
    mol->einstellung->setArrayIndex(i);
    mol->einstellung->setValue("Style", eacGLW->mol->AtomStyle[i]);
  }
  mol->einstellung->endArray();
  mol->einstellung->endGroup();

  psew->esm->disconnect();
  delete psew->esm;
  psew->disconnect();
  eacGLW->disconnect();
  this->disconnect();
  done(QDialog::Accepted);
}
