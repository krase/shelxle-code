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
#include "chgl.h"
#if defined (Q_WS_MAC)
#include <glu.h>
#else
#include <GL/glu.h>
#endif
#include <QtGui>
#include <QtOpenGL>
#include <QDialog>

//
//#define HIDE_REASON_SELECT          1
//#define HIDE_REASON_THIS_FRAGMENT   2
//#define HIDE_REASON_OTHER_FRAGMENT  4
//#define HIDE_REASON_HYDROGEN        8
//#define HIDE_REASON_QPEAK          16 
//


inline void __RotateCS( double c, double s, double& X, double& Y ) {
  double T = X;
  X = c*X - s*Y;
  Y = s*T + c*Y;
}

void glTranslateL( const double dx, const double dy, const double dz ) {
  double mat[4][4];

  glGetDoublev( GL_MODELVIEW_MATRIX, (double*)mat );
  mat[3][0] += dx;  mat[3][1] += dy;  mat[3][2] += dz;
  glLoadMatrixd((double*)mat);
}

void glRotateL( const double dang, const double x, const double y, const double z ) {
  double mat[4][4];
#ifndef M_PI
#define	M_PI		3.14159265358979323846	/* pi */
#endif

  double s = z;
  s = sin(dang*M_PI/180);
  const double c = cos(dang*M_PI/180);
  glGetDoublev( GL_MODELVIEW_MATRIX, (double*)mat );
  //  glGetDoublev( GL_PROJECTION_MATRIX, (double*)mat );


  if( x!=0.0 ){
    __RotateCS( c, s, mat[0][1], mat[0][2] );
    __RotateCS( c, s, mat[1][1], mat[1][2] );
    __RotateCS( c, s, mat[2][1], mat[2][2] );
  }else if( y!=0.0 ){
    __RotateCS( c, s, mat[0][2], mat[0][0] );
    __RotateCS( c, s, mat[1][2], mat[1][0] );
    __RotateCS( c, s, mat[2][2], mat[2][0] );
  }else{
    __RotateCS( c, s, mat[0][0], mat[0][1] );
    __RotateCS( c, s, mat[1][0], mat[1][1] );
    __RotateCS( c, s, mat[2][0], mat[2][1] );
  }


  glLoadMatrixd((double*)mat); 

}

void ChGL::saveOrientation(){
  QString fn=QFileDialog::getSaveFileName(this, tr("Save Orientation into file "), "orientation.ini",
		                         "orientation.ini (*.ini);;");

   QFile mconf(fn);
   mconf.open(QIODevice::WriteOnly|QIODevice::Text);
   GLdouble mm[16];
   glGetDoublev(GL_MODELVIEW_MATRIX,mm);
   mconf.write(QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10/%11/%12/%13/%14/%15/%16\n")
		   .arg(mm[0]).arg(mm[1]).arg(mm[2]).arg(mm[3]).arg( mm[4]).arg(mm[5])
		   .arg(mm[6]).arg(mm[7]).arg( mm[8]).arg(mm[9]).arg(mm[10]).arg(mm[11])
		   .arg( mm[12]).arg(mm[13]).arg(mm[14]).arg(mm[15]).toLatin1());
   mconf.close();
}

void ChGL::loadOrientation(){
  QString fn=QFileDialog::getOpenFileName(this, tr("Open MoleCoolQt MolIso Settings file "), "MoleCoolQt.moliso.ini",
		                         "MoleCoolQt.moliso.ini (*.ini);;" );

  if (fn.isEmpty()) return;
  QFile miconf(fn);
  miconf.open(QIODevice::ReadOnly|QIODevice::Text);
  QString all=miconf.readAll();
  miconf.close();

  MM[0] = all.section('/',0,0).toDouble();
  MM[1] = all.section('/',1,1).toDouble(); 
  MM[2] = all.section('/',2,2).toDouble();
  MM[3] = all.section('/',3,3).toDouble();
  MM[4] = all.section('/',4,4).toDouble();
  MM[5] = all.section('/',5,5).toDouble();
  MM[6] = all.section('/',6,6).toDouble();
  MM[7] = all.section('/',7,7).toDouble();
  MM[8] = all.section('/',8,8).toDouble();
  MM[9] = all.section('/',9,9).toDouble();
  MM[10]= all.section('/',10,10).toDouble();
  MM[11]= all.section('/',11,11).toDouble();
  MM[12]= all.section('/',12,12).toDouble();
  MM[13]= all.section('/',13,13).toDouble();
  MM[14]= all.section('/',14,14).toDouble();
  MM[15]= all.section('/',15,15).toDouble();
  printf("%12g %12g %12g %12g\n%12g %12g %12g %12g\n%12g %12g %12g %12g\n%12g %12g %12g %12g\n",
MM[0], 
MM[1] ,
MM[2] ,
MM[3] ,
MM[4] ,
MM[5] ,
MM[6] ,
MM[7] ,
MM[8] ,
MM[9] ,
MM[10],
MM[11],
MM[12],
MM[13],
MM[14],
MM[15]);
  setMatrix();
//  emit mconf();
  updateGL();
}

void ChGL::setMatrix(){
  glMatrixMode(GL_MODELVIEW);
    GLdouble det=
	  MM[0]*MM[5]*MM[10] - MM[8]*MM[5]*MM[2]+
	  MM[1]*MM[6]*MM[8]  - MM[9]*MM[6]*MM[0]+
	  MM[2]*MM[4]*MM[9]  - MM[10]*MM[4]*MM[1];
    printf("Determinant is %g\n",det);
  //if ((det>0.1)&&(det<9.0)) 
  glLoadMatrixd(MM);
  /*else {
    MM[0]=1;
    MM[1]=0;
    MM[2]=0;
    MM[3]=0;
    MM[4]=0;
    MM[5]=1;
    MM[6]=0;
    MM[7]=0;
    MM[8]=0;
    MM[9]=0;
    MM[10]=1;
    MM[11]=0;
    MM[12]=0;
    MM[13]=0;
    MM[14]=-200;
    MM[15]=1;
    glLoadIdentity();
    glLoadMatrixd(MM);
  }*/
    updateGL();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (Q_WS_MAC)
ChGL::ChGL(QWidget *parent) : QGLWidget(QGLFormat(QGL::StencilBuffer | QGL::AlphaChannel|QGL::DoubleBuffer|QGL::DepthBuffer|QGL::DirectRendering),parent) {                                                        ///////////////
#else
ChGL::ChGL(QWidget *parent) : QGLWidget(QGLFormat(QGL::StencilBuffer | QGL::AlphaChannel|QGL::DoubleBuffer|QGL::DepthBuffer|QGL::DirectRendering|QGL::StereoBuffers),parent) {                                                        ///////////////
#endif
  //printf("ChGL::ChGL\n");
  inRenameMode=false;
  pause=true;
  exporting=false;
  dratpause=80;
  murx=0;
  chparent=parent;
  xrdist=-206.2;
  envirange=3.2;
  enviNoQ= new QAction("Exclude Q-peaks from environment listing", this);
  enviCova = new QAction("List only covalent bonds.",this);
  enviButt= new QToolButton(this);
  enviButt->setText("clear ENVI");
  connect(enviButt,SIGNAL(clicked()),this,SLOT(clearEnvi()));
  wireButt= new QAction("Wiremodel while rotate",this);
  wireButt->setCheckable(true);
  wireButt->setChecked(true);
  enviButt->setVisible(false);
  enviNoQ->setCheckable(true);
  enviNoQ->setChecked(false);
  enviCova->setCheckable(true);
  enviCova->setChecked(false);
  mol=new Molecule();
  warLabel=noWaitLabel=false;
  viewAngle=29.0;
  imFokus=afok=-1;
  pickradius=200.0;
  //printf("__ChGL::ChGL\n");
  xray_eye=0;
  moving = new QTimer(this);
  moving->setSingleShot(true);
  connect(moving,SIGNAL(timeout()),this,SLOT(updateGL()));
  p=pp=ppp=0;
  fogrange=60;
  xray_vision=false;
  bggradient=true;
  wirbas=bas=0;
  depthcueing=false;
  altemitte=V3(0,0,0);
  hiddenThings=false;
  hideReason=0;
  qcutoff=-9;
  setMinimumSize(200,200);
  myFont=QFont("Arial",16);
  centerSelection = new QAction(this);//for a dirty fix
  rotze=-1;                                                                                              ///////////////
  stereo_mode=0;                                                                                         ///////////////a
  atomsClickable=true;                                                                                   ///////////////
  mouseOverInteraction=true;                                                                             ///////////////
//  warfaul=
  drawUc=false;

  // 0  1  2  3
  // 4  5  6  7
  // 8  9 10 11
  //12 13 14 15
  for (int i=0; i<36; i++)foubas[i]=0;
  MM[0]=-1;
  MM[1]=MM[2]=MM[3]=MM[4]=MM[7]=MM[8]=MM[11]=MM[12]=MM[13]=0;
  MM[5]=-0.242536;
  MM[6]=0.970142;
  MM[9]=0.970142;
  MM[10]=0.242536;
  MM[15]=1.0;
  //MM[1]=MM[2]=MM[3]=MM[4]=MM[6]=MM[7]=MM[8]=MM[9]=MM[11]=MM[12]=MM[13]=0.0;
  MM[14]=-200.0;
  drawDF=drawFO= drawAx=drawAt=drawADP=drawBo=drawLa=drawHb=true;                                                ///////////////
#if (QT_VERSION >= 0x040600) && defined (Q_WS_MAC)
  grabGesture(Qt::PinchGesture);
  grabGesture(Qt::SwipeGesture);
#endif
  /*
  QGLFormat fmt=QGLFormat::defaultFormat() ;
  //fmt.setOption(QGL::StereoBuffers);
  fmt.setOption(QGL::DirectRendering|QGL::StereoBuffers);
  //fmt.setOption(QGL::);
  setFormat(fmt);
  */
  //                                                                                                     ///////////////
}                                                                                                        ///////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if (QT_VERSION >= 0x040600) && defined (Q_WS_MAC)
	bool ChGL::event(QEvent *event){
	  if (event->type() == QEvent::Gesture)
	    return gestureEvent(static_cast<QGestureEvent*>(event));
	  return QWidget::event(event);
	}

	bool ChGL::gestureEvent(QGestureEvent *event){
	  if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
	    swipeTriggered(static_cast<QSwipeGesture *>(swipe));
	  else
	    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
	      pinchTriggered(static_cast<QPinchGesture *>(pinch));
	  return true;
	}

void ChGL::pinchTriggered(QPinchGesture *gesture){
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  if (changeFlags & QPinchGesture::RotationAngleChanged) {
    qreal value = gesture->property("rotationAngle").toReal();
    rotZ(value/-36.0);
  }
  if (changeFlags & QPinchGesture::ScaleFactorChanged) {
    qreal value = gesture->property("scaleFactor").toReal();
    gZoom(value-1.0);
  }
  updateGL();
}

void ChGL::swipeTriggered(QSwipeGesture *gesture) {
  if (gesture->horizontalDirection() == QSwipeGesture::Left) glRotateL(-5.0,0.0f,1.0f,0.0f);
  if (gesture->horizontalDirection() == QSwipeGesture::Right) glRotateL(5.0,0.0f,1.0f,0.0f);
  if (gesture->verticalDirection() == QSwipeGesture::Up) glRotateL(-5.0,1.0f,0.0f,0.0f);
  if (gesture->verticalDirection() == QSwipeGesture::Down) glRotateL(5.0,1.0f,0.0f,0.0f);
  updateGL();
}
#endif



static inline void transform_point(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]) {
#define M(row,col)  m[col*4+row]
  out[0] =
	  M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
  out[1] =
	  M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
  out[2] =
	  M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
  out[3] =
	  M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}
int scrx=0,scry=0;
int scrx0=0,scry0=0;
bool rectangle=false;
static inline bool  posTo2D(V3 obj,
		const GLdouble model[16], const GLdouble proj[16],
		const GLint viewport[4],
		GLdouble * winx, GLdouble * winy) {
  GLdouble in[4], out[4];

  in[0] = obj.x;
  in[1] = obj.y;
  in[2] = obj.z;
  in[3] = 1.0;
  transform_point(out, model, in);
  transform_point(in, proj, out);

  if (in[3] == 0.0) return false;

  in[0] /= in[3];
  in[1] /= in[3];
  in[2] /= in[3];

  *winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
  *winy = viewport[1] + (1 - in[1]) * viewport[3] / 2;
  return true;

}

void ChGL::changeEnviRange(){//! a dialog for a new ChGL.envirange.
  bool ok;
  double r=QInputDialog::getDouble ( this,"Envi-Range", "change environment range", envirange,0.7,8.0,2,&ok);
  if (ok) envirange=r;
}

void ChGL::mousePressEvent(QMouseEvent *event) {
  if (rectangle){
    rectangle=false;
    updateGL();
  }
/*  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_CLIP_PLANE1);
  glDisable(GL_CLIP_PLANE2);
  xray_eye=0;*/
  lastPos = event->pos();
  p=qMin((int)p,mol->showatoms.size()-1);
  pp=qMin((int)pp,mol->showatoms.size()-1);
  ppp=qMin((int)ppp,mol->showatoms.size()-1);

  p=qMax((int)p,0);
  pp=qMax((int)pp,0);
  ppp=qMax((int)ppp,0);
  double nahda=pickradius,da=0;
  int nahdai=-1;
  for (int j=0; j<mol->showatoms.size();j++){
    if (mol->showatoms.at(j).hidden) continue;
    da=(((mol->showatoms.at(j).screenX-event->x())*( mol->showatoms.at(j).screenX-event->x()))+
		    ((mol->showatoms.at(j).screenY-event->y())*( mol->showatoms.at(j).screenY-event->y())));
    nahdai=(da<nahda)?j:nahdai;
    nahda=qMin(nahda,da);
  }
//  printf("press =>%d RNM%d %d\n",nahdai,inRenameMode,mol->selectedatoms.size());

  //  fprintf(stderr,"pressed %d %d %d  %d\n",p,pp,ppp,mol->showatoms.size());
  if (event->buttons() & Qt::MidButton){
    /*    GLint hits;
	  glSelectBuffer(MAXSELECT, selectBuf);
	  (void)glRenderMode(GL_SELECT);
	  glInitNames();
	  glPushName(~0);
	  glPushMatrix();
	  if ((stereo_mode<2)||(stereo_mode>3))   glViewport(0, 0, ww, wh);
	  else glViewport(0, 0, ww/2, wh);
	  glGetIntegerv(GL_VIEWPORT, vp);
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  if ((stereo_mode<2)||(stereo_mode>3))  {
	  gluPickMatrix(event->x(), wh-event->y(), 8, 8, vp);
	  gluPerspective( viewAngle, (double)ww/wh, 50.0, 8000.0 );
	  }
	  else {
	  gluPickMatrix(event->x() % (ww/2), wh-event->y(), 8, 8, vp);
	  gluPerspective( viewAngle, (double)(ww / 2.0)/wh, 50.0, 8000.0 );
	  }
	  glMatrixMode(GL_MODELVIEW);
    //glGetDoublev(GL_MODELVIEW_MATRIX,MM);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    glShadeModel(GL_SMOOTH);

    draw();
    glPopAttrib();
    glPopMatrix();
    /hits = glRenderMode(GL_RENDER); 
    updateGL();*/
    if (nahdai < 0) {
    }else 
      if (nahdai<mol->showatoms.size()) {
	GLuint index=nahdai;
	if (index==((GLuint)-1))return;
	rotze=((int)index<mol->showatoms.size())?index:-1;
	if (rotze>-1){ rCenter->show();

	  glGetDoublev(GL_MODELVIEW_MATRIX,MM);
	  MM[12]=MM[13]=0;
	  glLoadMatrixd(MM);
	}
	updateGL();

      }
  }
  //*/
  //  if((reSe | moveLab| invEditAble | atomsClickable| xdSetupMode) && 
  if (event->buttons() & Qt::LeftButton){
 scrx0=lastPos.x();
 scry0=lastPos.y();
    if (nahdai < 0) {
    }else 
      if (nahdai<mol->showatoms.size()) {
	GLuint index=nahdai;
	if (index==((GLuint)-1))return;
	double w=0,dw=0;
	if ((pp!=p)&&(pp!=index)&&(p!=index)) {
	  w=mol->winkel(mol->showatoms.at(p).pos-mol->showatoms.at(pp).pos,
			  mol->showatoms.at(p).pos-mol->showatoms.at(index).pos);
	  if ((ppp!=p)&&(ppp!=pp)&&(ppp!=index))
	    dw=mol->dieder(mol->showatoms.at(pp).pos-mol->showatoms.at(ppp).pos,
			    mol->showatoms.at(pp).pos-mol->showatoms.at(p).pos,
			    mol->showatoms.at(p).pos-mol->showatoms.at(index).pos);
	}
	if (atomsClickable){
	  emit jumpit(index);
	  if (!inRenameMode){
	    if (event->modifiers()==Qt::NoModifier) {
	      int isschon=-1;
	      for (int i=0; i< mol->selectedatoms.size();i++){
		isschon=((GLuint)mol->selectedatoms.at(i).style==index)?index:isschon;
	      }
	      mol->selectedatoms.clear();
	      if (isschon==-1){
		mol->selectedatoms.append(mol->showatoms[index]);
		mol->selectedatoms.last().style=index;
	      }
	    }
	    if (event->modifiers()==Qt::ShiftModifier) {
	      int min=mol->showatoms.size(), max=-1;
	      for (int i=0; i< mol->selectedatoms.size();i++){
		min=qMin(mol->selectedatoms.at(i).style,min);
		max=qMax(mol->selectedatoms.at(i).style,max);
	      }
	      min=qMin((int)index,min);
	      max=qMax((int)index,max);
	      mol->selectedatoms.clear();
	      for (int i=min; i<=max; i++){
		mol->selectedatoms.append(mol->showatoms[i]);
		mol->selectedatoms.last().style=i;
	      }
	    }
	    if (event->modifiers()==Qt::ControlModifier) {
	      if (mol->selectedatoms.contains(mol->showatoms[index])){
		mol->selectedatoms.removeOne(mol->showatoms[index]);
	      } else {
		mol->selectedatoms.append(mol->showatoms[index]);
		mol->selectedatoms.last().style=index;
	      }
	    }
	    //	  std::cout<<mol->selectedatoms.size()<<mol->selectedatoms.last().Label.toStdString()<<std::endl;
	    updateBondActions();
	    updateGL();
	  }
	  //
	  //
	  V3 hin;
	  double dmsda=0;
	  hin = Normalize(mol->showatoms.at(index).pos-mol->showatoms.at(p).pos);
	  if ((mol->showatoms.at(index).an>-1) && (mol->showatoms.at(p).an)) 
	    dmsda = fabs(((hin*mol->showatoms.at(index).uc)*hin)- ((hin*mol->showatoms.at(p).uc)*hin)); 
	  //
	  bool syok;
	  QString symml;
	  int syid=mol->showatoms.at(index).Label.section(QString::fromUtf8("»"),1).toInt(&syok);
	  if (mol->usedSymmetry.size()&&syok) symml=mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid);
	  syid=mol->showatoms.at(p).Label.section(QString::fromUtf8("»"),1).toInt(&syok);
	  if ((mol->usedSymmetry.size())&&(syok)&&(!symml.contains(mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid)))) 
	    symml+=mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid);

	  syid=mol->showatoms.at(pp).Label.section(QString::fromUtf8("»"),1).toInt(&syok);
	  if ((mol->usedSymmetry.size())&&(syok)&&(!symml.contains(mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid)))) 
	    symml+=mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid);

	  syid=mol->showatoms.at(ppp).Label.section(QString::fromUtf8("»"),1).toInt(&syok);
	  if ((mol->usedSymmetry.size())&&(syok)&&(!symml.contains(mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid)))) 
	    symml+=mol->symmcode2human(mol->usedSymmetry.at(syid-1),syid);

	  if ((index!=p)&&(index!=pp)&&(mol->showatoms.at(index).Label[0]=='Q')){
	    emit bigmessage(QString("<hr><table><tr><td><b>%1</b></td><td align=\"right\"></td></tr><tr><td><b>%1--%2</b>"
				    "</td><td align=\"right\">%5 &Aring;</td></tr><tr><td><b>%1--%2--%3</b></td><td "
				    "align=\"right\">%6&deg;</td></tr><tr><td><b>%1--%2--%3--%4 </b></td><td "
				    "align=\"right\">%7&deg;</td></tr><tr><td><b>%1</b> Peak Height: </td><td "
				    "align=\"right\">%8 e&Aring;<sup>-3</sup></td><td>[%9&hellip;%10]"
				    "</td></tr></table>%11")
			    .arg((mol->showatoms.at(index).Label))                     //1
			    .arg((mol->showatoms.at(p).Label))				//3
			    .arg((mol->showatoms.at(pp).Label))			                //5			                                                //8
			    .arg((mol->showatoms.at(ppp).Label))			                        //9
			    .arg(sqrt(Distance(mol->showatoms.at(index).pos,mol->showatoms.at(p).pos)),8,'f',3) //4
			    .arg(w,8,'f',2)
			    .arg(dw,8,'f',2)
			    .arg(mol->showatoms.at(index).peakHeight,8,'f',2)
			    .arg(mol->pmin,3,'f',2)
			    .arg(mol->pmax,3,'f',2)
			    .arg(symml)

			   );
	  }
	  else if ((index!=p)&&(index!=pp)){
	    emit bigmessage(QString("<br><hr><table><tr><td><b>%1</b> part: %8 residue: %9 %10 (fragment: %12)</td><td align=\"right\">"
				    "</td></tr><tr><td><b>%1--%2</b></td><td align=\"right\">%5 &Aring;</td><td>&nbsp;&nbsp;&nbsp;DMSDA: %13 &Aring;<sup>2</sup></td></tr>"
				    "<tr><td><b>%1--%2--%3</b></td><td align=\"right\">%6&deg;</td><td>&nbsp;&nbsp;&nbsp;1,3 Distance( DANG) %14 &Aring;</td></tr><tr><td>"
				    "<b>%1--%2--%3--%4 </b></td><td align=\"right\">%7&deg;</td></tr></table>%11")
			    .arg((mol->showatoms.at(index).Label))			       //1
			    .arg((mol->showatoms.at(p).Label))			               //2
			    .arg((mol->showatoms.at(pp).Label))			       //3
			    .arg((mol->showatoms.at(ppp).Label))			       //4
			    .arg(sqrt(Distance(mol->showatoms.at(index).pos,mol->showatoms.at(p).pos)),8,'f',3) //5
			    .arg(w,8,'f',2)			                            //6
			    .arg(dw,8,'f',2)                                               //7
			    .arg(mol->showatoms.at(index).part)
			    .arg(mol->showatoms.at(index).resiNr)
			    .arg(mol->showatoms.at(index).ResiClass)
			    .arg(symml)
			    .arg(mol->showatoms.at(index).molindex)
			    .arg(dmsda,5,'e',2)
                            .arg(sqrt(Distance(mol->showatoms.at(index).pos,mol->showatoms.at(pp).pos)),8,'f',3) 
			   );
	  }
	  //				   ,QMessageBox::Ok);
	  ppp=pp;
	  pp=p;
	  p=index;
	}

	//888
      }
  }
}

void ChGL::mouseMoveEvent(QMouseEvent *event) {
  double nahda=pickradius,da=0;
  rectangle=false;
  int nahdai=-1;
  bool changed=false;
  scrx=event->x();
  scry=event->y();
  for (int j=0; j<mol->showatoms.size();j++){
    if (mol->showatoms.at(j).hidden) continue;
    da=(((mol->showatoms.at(j).screenX-event->x())*( mol->showatoms.at(j).screenX-event->x()))+ 
		    ((mol->showatoms.at(j).screenY-event->y())*( mol->showatoms.at(j).screenY-event->y())));
    nahdai=(da<nahda)?j:nahdai;
    nahda=qMin(nahda,da);
  }
 // printf("move =>%d\n",nahdai);
  if ((mouseOverInteraction)&&(!event->buttons())) {
    if (imFokus!=nahdai) changed=true;
    imFokus=nahdai;
    if (imFokus==-1) {
      emit qpfoci(-1.0);
      emit message("");
    }
    else {
      emit message(mol->showatoms.at(imFokus).Label);
      if ((mol->showatoms.at(imFokus).an<0)) emit qpfoci(mol->showatoms.at(imFokus).peakHeight);
      else qpfoci(-1.0);
    }

    if (changed) updateGL();
    return;
  }
  GLfloat dx = GLfloat( event->x() - lastPos.x()) / ww;
  GLfloat dy = GLfloat( event->y() - lastPos.y()) / wh;
  /*
     if ((mouseOverInteraction)&&(!event->buttons())) {
     GLint hits;   glSelectBuffer(MAXSELECT, selectBuf);
     (void)glRenderMode(GL_SELECT);
     glInitNames();
     glPushName(~0);
     glPushMatrix();
     if ((stereo_mode<2)||(stereo_mode>3))  glViewport(0, 0, ww, wh);
     else glViewport(0, 0, ww/2, wh);
     glGetIntegerv(GL_VIEWPORT, vp);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     if ((stereo_mode<2)||(stereo_mode>3))  {
     gluPickMatrix(event->x(), wh-event->y(), 8, 8, vp);
     gluPerspective( viewAngle, (double)ww/wh, 50.0, 8000.0 );
     }
     else {
     gluPickMatrix(event->x() % (ww/2), wh-event->y(), 8, 8, vp);
     gluPerspective( viewAngle, (double)(ww / 2.0)/wh, 50.0, 8000.0 );
     }
     glMatrixMode(GL_MODELVIEW);
     glPushAttrib(GL_ENABLE_BIT);
     glDisable(GL_STENCIL_TEST);
     glDisable(GL_ALPHA_TEST);
     glDisable(GL_LIGHTING);
     glDisable(GL_FOG);
     glDisable(GL_NORMALIZE);
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_COLOR_MATERIAL);
     glDisable(GL_LINE_SMOOTH);
     glDisable(GL_DITHER);
     glDisable(GL_BLEND);
     glShadeModel(GL_SMOOTH);
  //     glLoadMatrixd(MM);
  bool ala=drawLa;
  drawLa=false;
  draw();
  drawLa=ala;
  glPopAttrib();
  glPopMatrix();
  hits = glRenderMode(GL_RENDER);
  //hits = glRenderMode(GL_SELECT);
  if ((hits <= 0)||(mol->showatoms.size()<(int)selectBuf[(hits-1)*4+3])) {imFokus=-1; emit qpfoci(-1.0); emit message("");}
  else {
  imFokus=selectBuf[(hits-1)*4+3];
  //  printf("Test,%d %d\n",imFokus,hits);
  if (imFokus>=0) emit message(mol->showatoms.at(selectBuf[(hits-1)*4+3]).Label);
  if ((imFokus>=0)&&(mol->showatoms.at(selectBuf[(hits-1)*4+3]).an<0)) emit qpfoci(mol->showatoms.at(selectBuf[(hits-1)*4+3]).peakHeight);
  else emit qpfoci(-1.0);
  }
  updateGL();
  //    highFocus();
  }
  // */
  if (event->buttons() & Qt::MidButton){

    moving->start(dratpause);
    glTranslateL(dx*viewAngle*3,-dy*viewAngle*3,0);
    updateGL();
  }
  
  if ((event->buttons() & Qt::LeftButton && (event->modifiers()==(Qt::ShiftModifier|Qt::ControlModifier)))) {
    rectangle=true;
    int minxp=ww,minyp=wh,maxxp=0,maxyp=0;
    minxp=qMin(scrx0,scrx);
    minyp=qMin(scry0,scry);
    maxxp=qMax(scrx0,scrx);
    maxyp=qMax(scry0,scry);
    mol->selectedatoms.clear();
    for (int j=0; j<mol->showatoms.size();j++){
      if (mol->showatoms.at(j).hidden) continue;
      if ((mol->showatoms.at(j).screenX>minxp)&&
          (mol->showatoms.at(j).screenX<maxxp)&&
          (mol->showatoms.at(j).screenY>minyp)&&
          (mol->showatoms.at(j).screenY<maxyp)){

        mol->selectedatoms.append(mol->showatoms[j]);
        mol->selectedatoms.last().style=j;
      }
    }
    updateBondActions();
    updateGL();
  }else
  if ((event->buttons() & Qt::LeftButton)) {
    if (event->modifiers()==Qt::ShiftModifier) {
    moving->start(dratpause);
    glTranslateL(dx*viewAngle*3,-dy*viewAngle*3,0);
    updateGL();}
    else
    {
      moving->start(dratpause);
      double slow=1.0;
      if (event->modifiers()==Qt::ShiftModifier) slow=0.25;
      if (event->modifiers()==Qt::ControlModifier) slow=0.06;
      glRotateL(dy*360.0*slow,1.0f,0.0f,0.0f);
      glRotateL(dx*360.0*slow,0.0f,1.0f,0.0f);
    }
    updateGL();
  }
  else if((event->buttons() & Qt::RightButton)){
    if (!invertMouseZoom->isChecked()){

      moving->start(dratpause);
      glScaled(1.0-dy,1.0-dy,1.0-dy);
      //     mlsc/=1.0-dy;
    }else {

      moving->start(dratpause);
      glScaled(1.0+dy,1.0+dy,1.0+dy);
      //     mlsc/=1.0+dy;
    }
    updateGL();
  }
  lastPos = event->pos();
}

void ChGL::wheelEvent(QWheelEvent *event){
  /*
modifiers:
Qt::NoModifier	        0x00000000	No modifier key is pressed.
Qt::ShiftModifier	    0x02000000	A Shift key on the keyboard is pressed.
Qt::ControlModifier	    0x04000000	A Ctrl key on the keyboard is pressed.
Qt::AltModifier	        0x08000000	An Alt key on the keyboard is pressed.
Qt::MetaModifier	    0x10000000	A Meta key on the keyboard is pressed.
Qt::KeypadModifier	    0x20000000	A keypad button is pressed.
Qt::GroupSwitchModifier	0x40000000	X11 only. A Mode_switch key on the keyboard is pressed.
*/ 
  int numDegrees = event->delta() / 8;
  int numSteps = numDegrees / 15;
  if (event->modifiers()==Qt::NoModifier){
    int d = myFont.pointSize();
    d=(d+numSteps>4)?d+numSteps:d;
    d=qMax(d,7);
    myFont.setPointSize(d);
    updateGL();
  }
  if (event->modifiers()==Qt::ControlModifier){
    emit diffscroll(numSteps,1); 
  } 
  if (event->modifiers()==Qt::ShiftModifier){
    emit diffscroll(numSteps,0); 
  }
  if (event->modifiers()==(Qt::AltModifier|Qt::ShiftModifier)){
    double va=viewAngle+0.1*numSteps;
    setViewAngle(va);
  } 
  if (event->modifiers()==Qt::AltModifier){
    int afr=fogrange;
    fogrange+=numSteps*5;
    fogrange=qMax(fogrange,2);
    fogrange=qMin(fogrange,199);
    //    printf("fogrange = %d\n", fogrange);
    if (afr!=fogrange){
      glFogi(GL_FOG_MODE,GL_LINEAR);
      glFogi(GL_FOG_START,205-fogrange);
      glFogi(GL_FOG_END,205+fogrange);
      GLfloat fgc[4]={(GLfloat)backGroundColor.redF(),(GLfloat)backGroundColor.greenF(),(GLfloat)backGroundColor.blueF(),1.0};
      glFogfv(GL_FOG_COLOR,fgc);
      updateGL();}
  }
  event->accept();
}

void ChGL::contextMenuEvent(QContextMenuEvent *event){
  double nahda=pickradius,da=0;
  int nahdai=-1;
  for (int j=0; j<mol->showatoms.size();j++){
    if (mol->showatoms.at(j).hidden) continue;
    da=(((mol->showatoms.at(j).screenX-event->x())*( mol->showatoms.at(j).screenX-event->x()))+
		    ((mol->showatoms.at(j).screenY-event->y())*( mol->showatoms.at(j).screenY-event->y())));
    nahdai=(da<nahda)?j:nahdai;
    nahda=qMin(nahda,da);
  }
  if ((nahdai < 0)||(mol->showatoms.size()<nahdai)) {
    return;
  }
  else {
    GLuint idx=nahdai;
    ImeanThisAtom=idx;
    if (idx>((GLuint) mol->showatoms.size())) return;
    QVariantList row;
    row.clear();
    row.append((int) p);
    row.append((int) idx);
    QVariant qvl(row);
    QList<QAction *> sfas = sfacMenu->actions();
    for (int k=0;k<sfas.size();k++){
        sfas[k]->setEnabled(sfas[k]->text()!=mol->pse(mol->showatoms.at(idx).an));
    }
    QMenu *menu = new QMenu(mol->showatoms.at(idx).Label);
    menu->addMenu(sfacMenu);
    QAction *a;//=menu->addAction(QString("Edit Style of %1").arg(mol->showatoms.at(idx).Label),parent(),SLOT(styleDialog2()));
    //a->setData(idx);
    if ((mol->selectedatoms.isEmpty())&&(mol->showatoms.at(idx).symmGroup==0)) {
      a=menu->addAction(QIcon(":icons/killselected.png"),QString("Delete %1 ").arg(mol->showatoms.at(idx).Label),
		      chparent,SLOT(deleteSelectedAtoms()));
      a->setData(idx);
    }
    a=menu->addAction(QString("Set rotation center to %1").arg(mol->showatoms.at(idx).Label),this,SLOT(setRotationCenter()));
    a->setData(idx);
    a=menu->addAction(QString("Expand contacts around %1").arg(mol->showatoms.at(idx).Label),this,SLOT(expand()));
    a->setData(idx);
    a=menu->addAction(QString("Hide this fragment"),this,SLOT(hideThisFragment()));
    a->setData(idx);
    a=menu->addAction(QString("Hide other fragments"),this,SLOT(hideOtherFragments()));
    a->setData(idx);

    if(!mol->selectedatoms.isEmpty()){
        QMenu *RestrMenu = new QMenu("Add restraints");
        QMenu *ConstrMenu = new QMenu("Add constraints");
        RestrMenu->addAction("ISOR", this, SLOT(addISOR()));

        if(mol->selectedatoms.size() == 2){
            RestrMenu->addAction("DFIX", this, SLOT(addDFIX()));
            RestrMenu->addAction("DANG", this, SLOT(addDANG()));
        }
        if(mol->selectedatoms.size() >= 4){
            RestrMenu->addAction("FLAT", this, SLOT(addFLAT()));
        }
        if(mol->selectedatoms.size() > 1){
            ConstrMenu->addAction("EXYZ", this, SLOT(addEXYZ()));
            ConstrMenu->addAction("EADP", this, SLOT(addEADP()));
            RestrMenu->addAction("RIGU", this, SLOT(addRIGU()));
            RestrMenu->addAction("DELU", this, SLOT(addDELU()));
            RestrMenu->addAction("SIMU", this, SLOT(addSIMU()));
            RestrMenu->addAction("CHIV", this, SLOT(addCHIV()));
        }
        menu->addMenu(RestrMenu);
        menu->addMenu(ConstrMenu);
    }
    if (!mol->selectedatoms.isEmpty()) a=menu->addAction("Hide selected Atoms",this,SLOT(hideSelected()));
    a=menu->addAction(QString("Select this fragment"),this,SLOT(selectThisFragment()));
    a->setData(idx);
    QMenu *enviMenu = new QMenu("ENVI-Settings");
    enviMenu->addAction("Change envi range", this, SLOT(changeEnviRange()));
    enviMenu->addAction(enviNoQ);
    enviMenu->addAction(enviCova);
    menu->addMenu(enviMenu);
    if ((int) idx < mol->asymm.size()){
      a=menu->addAction(QString("List ENVIronment of %1").arg(mol->showatoms.at(idx).Label),this,SLOT(envi()));
      a->setData(idx);
    }
    int ssgr=0;
    int adpat=0;
    for (int seli=0; seli<mol->selectedatoms.size(); seli++){
      if ((mol->showatoms.at(seli).symmGroup==0)&&(mol->selectedatoms.at(seli).uf.m22!=666.0))adpat++;
    }
    for (int seli=0; seli<mol->selectedatoms.size(); seli++){
      if (mol->selectedatoms.at(seli).symmGroup>0) ssgr+=mol->selectedatoms.at(seli).symmGroup;
      else {
	ssgr=0;
	break;
      }
    }
    if (ssgr) adpat=0;
    if (ssgr){
      a=menu->addAction(QString("Move selected atoms to selected sites"),
		    chparent,SLOT(moveSymmMateSel()));
      //////// 28.03.12
      if (mol->showatoms.at(idx).part<0){
	a=menu->addAction(QString("Copy %1 here").arg(mol->showatoms.at(idx).Label.section("_",0,0)),
			chparent,SLOT(copySymmMate()));
	a->setData(idx);
      }
      //////// -
    }else{
      if (!mol->selectedatoms.isEmpty()){
	a=menu->addAction(QIcon(":icons/killselected.png"),QString("Delete selected atoms"),
			chparent,SLOT(deleteSelectedAtoms()));
	a->setData(-1);
      }
      if (mol->showatoms.at(idx).symmGroup>0){
	a=menu->addAction(QString("Move %1 here").arg(mol->showatoms.at(idx).Label.section("_",0,0)),
		chparent,SLOT(moveSymmMate()));
	a->setData(idx);
	if (mol->showatoms.at(idx).part<0){
	  a=menu->addAction(QString("Copy %1 here").arg(mol->showatoms.at(idx).Label.section("_",0,0)),
			 chparent,SLOT(copySymmMate()));
	  a->setData(idx);
	}

      }
    }
    if ((nahdai<mol->asymm.size())&&(mol->knoepfe.at(idx).neighbors.size()==1)&&((mol->showatoms.at(idx).an==6)||(mol->showatoms.at(idx).an==5))) {

      //	printf("%s hat %d Nachbarn\n",mol->showatoms.at(idx).Label.toStdString().c_str(),mol->knoepfe.at(idx).neighbors.size());
      a=menu->addAction(QString("Add dissordered %1 Hydrogen atoms").arg(((mol->showatoms.at(idx).an==5))?"methyl":"amino"),chparent,SLOT(addDissorderedMethyl()));
      a->setData(idx);
      a=menu->addAction(QString("Add ordered %1 Hydrogen atoms").arg(((mol->showatoms.at(idx).an==5))?"methyl":"amino"),chparent,SLOT(addMethyl()));
      a->setData(idx);
    }
    if ((idx < ((GLuint) mol->asymm.size())) && (mol->asymm.at(idx).an>-1)) {
      a=menu->addAction(QString("Use %1 as new label for rename mode").arg(mol->showatoms.at(idx).Label.section("_",0,0)),chparent,SLOT(renameThisAtom()));
      a->setData(idx);
    }
    if (mol->showatoms.at(p).molindex!=mol->showatoms.at(idx).molindex){
      a=menu->addAction(QString("Inherit Labels from molecule around %1 to atoms around %2")
		      .arg(mol->showatoms.at(p).Label)
		      .arg(mol->showatoms.at(idx).Label)
		      ,chparent,SLOT(inheritLabels()));
      a->setData(qvl);

    }

    if (adpat) {
      a=menu->addAction(QIcon(":/icons/sina.png"),QString("make selected atoms isotropic (ISOT)"),chparent,SLOT(sina()));
      a->setData(-1);
    }
    else if ((mol->selectedatoms.isEmpty())&&(mol->showatoms.at(idx).symmGroup==0)&&(mol->showatoms.at(idx).uf.m22!=-666)){
      a=menu->addAction(QIcon(":/icons/sina.png"),QString("make %1 isotropic (ISOT)")
		      .arg(mol->showatoms.at(idx).Label),chparent,SLOT(sina()));
      a->setData(idx);
    }
    if(!mol->selectedatoms.isEmpty()){
        menu->addAction(QString("Make selected atoms ANIS"),this,SLOT(addANIS()));
    }
    menu->exec(event->globalPos());
    delete enviMenu;
    delete menu;
  }
}

void ChGL::Listen(){//! forces the lists to be rerendered.
  murx=1;
}

void ChGL::disSelection(){//! Un select everything.
  //rotCenter();
  QTime rzeit;
  rzeit.start();
  mol->selectedatoms.clear();
  updateBondActions();
  updateGL();
}

void ChGL::selectPair(const QString &s){
  //! Selelects two atoms from the spaces separated string s. @param space separated pair of atom labels. 
  mol->selectedatoms.clear();
  if (s.isEmpty()) {
    updateGL();
    return;
  }
  QStringList index=s.split(' ',QString::SkipEmptyParts);
  if (index.size()<2) {
    updateGL();
    return;
  }
  for (int i=0; i<index.size();i++){
    mol->selectedatoms.append(mol->showatoms[index.at(i).toInt()]);
    mol->selectedatoms.last().style=index.at(i).toInt();
  }
  //updateBondActions();
  updateGL();
}

void ChGL::selectResiByNr(int nr){//! Selects atoms in residues with the same residue number. @param nr residue number.
  mol->selectedatoms.clear();
  for (int i=0; i<mol->showatoms.size();i++){
    if (mol->showatoms.at(i).resiNr==nr) {
      mol->selectedatoms.append(mol->showatoms[i]);
      mol->selectedatoms.last().style=i;
    }
  }
  draw();
  if (hiddenThings) hideNonSelected();
  updateBondActions();
  updateGL();
}

void ChGL::invertSelection(){//!Inverts stelection, means selected atoms get disselected and vice versa.
  CEnvironment tempa=mol->selectedatoms;
//  printf("invert %d %d\n",tempa.size(),mol->selectedatoms.size());
  mol->selectedatoms.clear();

  for (int i=0; i<mol->showatoms.size();i++){
    if (!tempa.contains(mol->showatoms.at(i))) {
      mol->selectedatoms.append(mol->showatoms[i]);
      mol->selectedatoms.last().style=i;
    }
  }
  draw();
  rehide();
  updateBondActions();
  updateGL();
}

void ChGL::updateBondActions(){//!<changes the visibility state of QActions in the 'Selection Toolbar'.
  clearSelection->setVisible(!mol->selectedatoms.isEmpty());
  invSelection->setVisible(!mol->selectedatoms.isEmpty());
  centroid->setVisible(mol->selectedatoms.size()>1);
  centerSelection->setVisible((!mol->selectedatoms.isEmpty())||(centerSelection->isChecked()));
  delSelAt->setVisible(!mol->selectedatoms.isEmpty());
  delSelAt->setEnabled(!mol->selectedatoms.isEmpty());
  hideNotSelection->setVisible(!mol->selectedatoms.isEmpty());
  unhide->setVisible(hiddenThings);
  if (mol->selectedatoms.size()!=2){
    addBond->setVisible(false);
    killBond->setVisible(false);
  }
  else{
    int da=0;
    for (int i=0; i<mol->showbonds.size();i++){
      if (((mol->selectedatoms.at(0).style==mol->showbonds.at(i).a1)||
			      (mol->selectedatoms.at(0).style==mol->showbonds.at(i).a2))&&
		      ((mol->selectedatoms.at(1).style==mol->showbonds.at(i).a1)||
		       (mol->selectedatoms.at(1).style==mol->showbonds.at(i).a2))) da=i;
    }
    if (da){
      addBond->setVisible(false);
      killBond->setVisible(true);
      killBond->setData(da);
    }
    else{
      addBond->setVisible(true);
      killBond->setVisible(false);
    }
  }
}

void ChGL::connectSelection(){//!creates a bond betwen two selected atoms.
  if (mol->selectedatoms.size()!=2) return;
  MyBond b;
  b.a1=mol->selectedatoms.at(0).style;
  b.a2=mol->selectedatoms.at(1).style;
  b.ato1=&mol->showatoms[mol->selectedatoms.at(0).style];
  b.ato2=&mol->showatoms[mol->selectedatoms.at(1).style];
  b.length=sqrt(Distance(b.ato1->pos,b.ato2->pos));
  mol->showbonds.append(b);
  glDeleteLists(bas+1,1);
  mol->tubes=0;
  mol->adp=1;
  glNewList(bas+1, GL_COMPILE );{                          //BONDS
    glPushMatrix();{
      glScaled( L, L, L );
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

  glDeleteLists(bas+8,1);
  glNewList(bas+8, GL_COMPILE );{                          //BONDS //noadp
    glPushMatrix();{
      glScaled( L, L, L );
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

  glDeleteLists(bas+9,1);
  glNewList(bas+9, GL_COMPILE );{                          //BONDS tube
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=1;
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
    mol->tubes=0;
  }glEndList();

  mol->selectedatoms.clear();
  updateBondActions();
  updateGL();
}

void ChGL::disConnectSelection(int index){//! destroyes the bond between two selected atoms.
  mol->showbonds.removeAt(index);
  glDeleteLists(bas+1,1);
  mol->tubes=0;
  mol->adp=1;
  glNewList(bas+1, GL_COMPILE );{                          //BONDS
    glPushMatrix();{
      glScaled( L, L, L );
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

  glDeleteLists(bas+8,1);
  glNewList(bas+8, GL_COMPILE );{                          //BONDS //noadp
    glPushMatrix();{
      glScaled( L, L, L );
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

  glDeleteLists(bas+9,1);
  glNewList(bas+9, GL_COMPILE );{                          //BONDS tube
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=1;
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
    mol->tubes=0;
  }glEndList();

    
  mol->selectedatoms.clear();
  updateBondActions();
  updateGL();
}

void ChGL::setAtom(bool b){//!toggles atoms
  drawAt=b;
  updateGL();
}

void ChGL::setBond(bool b){//!toggles bonds
  drawBo=b;
  updateGL();
}

void ChGL::setLabel(bool b){//!toggles lables
  drawLa=b;
  updateGL();
}

void ChGL::setHBond(bool b){//!toggles H-bonds
  drawHb=b;
  updateGL();
}

void ChGL::setBGGradient(bool b){//!toggles back gound gradient
  bggradient=b;
  updateGL();
}

void ChGL::setDepthCueing(bool b){//!toggles fog
  depthcueing=b;
  updateGL();
}

void ChGL::setXRayView(bool b){//!toggles front cut planes
  xray_vision=b;
  updateGL();
}

void ChGL::setADP(bool b){//!toggles ellipsoids
  drawADP=b;
  updateGL();
}

void ChGL::setUnit(bool b){//!toggles unit cell box
  drawUc=b;
  updateGL();
}

void ChGL::changeBColor(){//! a color dialog for chosing a new back ground color
  backGroundColor= QColorDialog::getColor(backGroundColor, this);
  updateGL(); 
}

void ChGL::changeTColor(){//! a color dialog for chosing a label color
  labelColor= QColorDialog::getColor(labelColor, this);
  updateGL(); 
}

void ChGL::setMolecule(Molecule *m){//! sets the pointer to the Molecule object deletes the old one if it is exists and is differen from the new one
//  printf("setMolecule\n");
  if (mol!=m) {
    if (mol) delete mol;
    mol=m;  
  }
  double dim=mol->dimension();
  L=100.0/dim;
}

void ChGL::showMatrix(){
//  stereo_mode++;
//  stereo_mode%=4;
  updateGL();
  printf("Die MMATRIX ist:\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n",
		  MM[0],MM[1],MM[2],MM[3],
		  MM[4],MM[5],MM[6],MM[7],
		  MM[8],MM[9],MM[10],MM[11],
		  MM[12],MM[13],MM[14],MM[15]);
  GLdouble mm[16];
  glGetDoublev(GL_MODELVIEW_MATRIX,mm);
  printf("Die Mmatrix ist:\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n",
		  mm[0],mm[1],mm[2],mm[3],
		  mm[4],mm[5],mm[6],mm[7],
		  mm[8],mm[9],mm[10],mm[11],
		  mm[12],mm[13],mm[14],mm[15]);
  GLdouble det=
	  mm[0]*mm[5]*mm[10] - mm[8]*mm[5]*mm[2]+
	  mm[1]*mm[6]*mm[8]  - mm[9]*mm[6]*mm[0]+
	  mm[2]*mm[4]*mm[9]  - mm[10]*mm[4]*mm[1];
  printf("die Determinante der Drehmatrix ist: %g\n",det);
  glGetDoublev(GL_PROJECTION_MATRIX,mm);
  printf("Die Pmatrix ist:\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n",
		  mm[0],mm[1],mm[2],mm[3],
		  mm[4],mm[5],mm[6],mm[7],
		  mm[8],mm[9],mm[10],mm[11],
		  mm[12],mm[13],mm[14],mm[15]);
  det=
	  mm[0]*mm[5]*mm[10] - mm[8]*mm[5]*mm[2]+
	  mm[1]*mm[6]*mm[8]  - mm[9]*mm[6]*mm[0]+
	  mm[2]*mm[4]*mm[9]  - mm[10]*mm[4]*mm[1];
  printf("die Determinante der Drehmatrix ist: %g\n",det);
}

void ChGL::along001(){
  double va=viewAngle;
  setViewAngle(29.0);
  MM[0]=1.0;
  MM[1]=0.0;
  MM[2]=0.0;
  MM[4]=0.0;
  MM[5]=-1.0;
  MM[6]=0.0;
  MM[8]=0.0;
  MM[9]=0.0;
  MM[10]=-1.0;
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd(MM);
  setViewAngle(va);
}

void ChGL::zoomOut(){
  double va=viewAngle;
  setViewAngle(29.0);
  glMatrixMode(GL_MODELVIEW);
  glGetDoublev(GL_MODELVIEW_MATRIX,MM);
  GLdouble det=
	  MM[0]*MM[5]*MM[10] - MM[8]*MM[5]*MM[2]+
	  MM[1]*MM[6]*MM[8]  - MM[9]*MM[6]*MM[0]+
	  MM[2]*MM[4]*MM[9]  - MM[10]*MM[4]*MM[1];
  printf("Die Mmatrix ist:\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n",
		  MM[0] ,MM[1] ,MM[2] ,MM[3],
		  MM[4] ,MM[5] ,MM[6] ,MM[7],
		  MM[8] ,MM[9] ,MM[10],MM[11],
		  MM[12],MM[13],MM[14],MM[15]);
  printf("die Determinante der Matrix ist: %g\n",det);
  if (det !=0.0){
  det=pow(det,1.0/3.0);  
  MM[ 0]/=det;
  MM[ 1]/=det;
  MM[ 2]/=det;
  MM[ 4]/=det;
  MM[ 5]/=det;
  MM[ 6]/=det;
  MM[ 8]/=det;
  MM[ 9]/=det;
  MM[10]/=det;
  det=
	  MM[0]*MM[5]*MM[10] - MM[8]*MM[5]*MM[2]+
	  MM[1]*MM[6]*MM[8]  - MM[9]*MM[6]*MM[0]+
	  MM[2]*MM[4]*MM[9]  - MM[10]*MM[4]*MM[1];
  printf("##Die Mmatrix ist:\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n%9.6f %9.6f %9.6f %9.6f\n",
		  MM[0] ,MM[1] ,MM[2] ,MM[3],
		  MM[4] ,MM[5] ,MM[6] ,MM[7],
		  MM[8] ,MM[9] ,MM[10],MM[11],
		  MM[12],MM[13],MM[14],MM[15]);
  printf("##die Determinante der Matrix ist: %g\n",det);

  }
  glLoadMatrixd(MM);
  setViewAngle(va);
}

void ChGL::initializeGL(){
  glEnable(GL_LINE_SMOOTH);   
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  glEnable(GL_POLYGON_SMOOTH);  
  const GLfloat  position[] = {100.0f, 100.0f,100.0f,0.0f};
  const GLfloat  diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
  const GLfloat  specular[] = { 1.0, 1.0, 1.0, 1.0 };
  const GLfloat  ambient[]  = { 0.5, 0.5, 0.5, 1.0 };

  glLightModeli(  GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );

  glLightfv( GL_LIGHT0, GL_POSITION, position );
  glLightfv( GL_LIGHT0, GL_AMBIENT,  ambient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );

  glLightfv( GL_LIGHT1, GL_POSITION, position );
  glLightfv( GL_LIGHT1, GL_DIFFUSE,  diffuse );  
  glLightfv( GL_LIGHT1, GL_AMBIENT,  ambient );
  glLightfv( GL_LIGHT2, GL_DIFFUSE,  diffuse );  

  glDisable(GL_FOG);
  glFogi(GL_FOG_MODE,GL_LINEAR);
  glFogi(GL_FOG_START,200-fogrange);
  glFogi(GL_FOG_END,200+fogrange);
  GLfloat fgc[4]={(GLfloat)backGroundColor.redF(),(GLfloat)backGroundColor.greenF(),(GLfloat)backGroundColor.blueF(),1.0};
  glFogfv(GL_FOG_COLOR,fgc);

  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  glEnable( GL_BLEND);
  glLightModeli(  GL_LIGHT_MODEL_TWO_SIDE, 1);
  glAlphaFunc ( GL_GREATER, 0.01f ) ;
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
  const GLfloat  OBJ_SPE[]   = { 0.8, 0.8, 0.8, 1.0 };  
  const GLfloat  OBJ_SHIN    = 64.0;               
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,             OBJ_SPE  );
  glEnable     ( GL_COLOR_MATERIAL ) ;
  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;    
  glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS,           OBJ_SHIN );
  glShadeModel( GL_SMOOTH );
  //glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);
  qglClearColor(backGroundColor);

  glEnable(GL_DEPTH_TEST );
  glDepthFunc(GL_LEQUAL);
  gluLookAt(0.0, 200, 50 ,   0.0, 0.0, 0.0,   0.0, -100, 400 );
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  setMouseTracking(true);
  //Listen inititialisieren ist hier!
  initLists();
  if (foubas[0]|foubas[1]|foubas[2]){
    emit inimibas();
  }
}

void ChGL::initLists(){//!fills 10 ChGL.bas and 3 ChGL.wirbas list 
  if (mol->showatoms.isEmpty()) return;
//  QTime rzeit;
//  rzeit.start();
  setupTexture();
  if (bas) for (int i=0;i<10; i++){
    int lsts=glIsList(bas+i);
    if (lsts) {
      glDeleteLists(bas+i,1);
      //	printf("deleting List %d %d\n",bas+i,lsts);
    }
  }
  bas=glGenLists(10);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  mol->tubes=0;
  mol->dratom=0;
  glLoadMatrixd(MM);
//  printf("1g0 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas, GL_COMPILE );{                          //ATOME
    glPushMatrix();{
      glScaled( L, L, L );
      mol->intern=1;
      mol->adp=1;
      mol->atoms(mol->showatoms,mol->proba);
    }glPopMatrix();
  }glEndList();
//  printf("1g//ATOME => %d ms\n", rzeit.elapsed());  rzeit.restart();

  glNewList(bas+1, GL_COMPILE );{                          //BONDS
    glPushMatrix();{
      glScaled( L, L, L );
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

//  printf("1g3//BONDS => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+2, GL_COMPILE );{                          //ATOMS transparent ellipsoids
    glPushMatrix();{
      glScaled( L, L, L );
      mol->intern=0;
      mol->adp=1;
      mol->atoms(mol->showatoms,mol->proba);
    }glPopMatrix();
  }glEndList();

//  printf("1g3ATOMS transparent ellipsoids => %d ms\n", rzeit.elapsed());  rzeit.restart();

  glNewList(bas+3, GL_COMPILE );{                          //ATOMS balls
    glPushMatrix();{
      glScaled( L, L, L );
      mol->intern=1;
      mol->adp=0;
      mol->atoms(mol->showatoms);
    }glPopMatrix();
  }glEndList();

//  printf("1g3//ATOMS balls => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+4, GL_COMPILE );{                          //Unit Cell
    glPushMatrix();{
      glScaled( L, L, L );
      mol->unitCell();
    }glPopMatrix();
  }glEndList();


//  printf("1g3//Unit Cell => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+5, GL_COMPILE );{                          //ATOMS tubes
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=1;
      mol->intern=1;
      mol->adp=0;
      mol->atoms(mol->showatoms);
    }glPopMatrix();
  }glEndList();

//  printf("1g3//ATOMS tubes => %d ms\n", rzeit.elapsed());  rzeit.restart();
  mol->tubes=0;
  glNewList(bas+6, GL_COMPILE );{                          //QPeak-Line-BONDS
    glPushMatrix();{
      glScaled( L, L, L );
      mol->lbond();
    }glPopMatrix();
  }glEndList();

 // printf("1g3 //QPeak-Line-BONDS => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+7, GL_COMPILE );{                          //H-BONDS
    glPushMatrix();{
      glScaled( L, L, L );
      emit bigmessage(mol->h_bonds(mol->showbonds,mol->showatoms));
    }glPopMatrix();
  }glEndList();

  //printf("1g3//H-BONDS => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+8, GL_COMPILE );{                          //BONDS //noadp
    glPushMatrix();{
      glScaled( L, L, L );
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
  }glEndList();

//  printf("1g3//BONDS //noadp => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(bas+9, GL_COMPILE );{                          //BONDS tube
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=1;
      mol->adp=0;
      mol->bonds(mol->showbonds);
    }glPopMatrix();
    mol->tubes=0;
  }glEndList();

//  printf("1g3//BONDS tube => %d ms\n", rzeit.elapsed());  rzeit.restart();
  if (wirbas) for (int i=0;i<2; i++){
    int lsts=glIsList(wirbas+i);
    if (lsts) {
      glDeleteLists(wirbas+i,1);
      //	printf("deleting List %d %d\n",bas+i,lsts);
    }
  }
  wirbas=glGenLists(3);
  glNewList(wirbas,GL_COMPILE );{
    glPushMatrix();{
      glScaled( L, L, L );
      mol->dbond(mol->showbonds);
    }glPopMatrix();
  }glEndList();

//  printf("1g3dbond => %d ms\n", rzeit.elapsed());  rzeit.restart();
  glNewList(wirbas+1, GL_COMPILE );{                          //ATOME
    glPushMatrix();{
      glScaled( L, L, L );
      mol->dratom=5;
      mol->intern=0;
      mol->adp=1;
      mol->atoms(mol->showatoms,mol->proba);
    }glPopMatrix();
  }glEndList();
 // printf("1g3atoms => %d ms\n", rzeit.elapsed());  rzeit.restart();
  mol->dratom=0;
  glNewList(wirbas+2, GL_COMPILE );{                          //ATOME
    glPushMatrix();{
      glScaled( L, L, L );
      mol->dratom=5;
      mol->intern=0;
      mol->adp=0;
      mol->atoms(mol->showatoms,mol->proba);
    }glPopMatrix();
  }glEndList();
//  printf("1g3adp => %d ms\n", rzeit.elapsed());  rzeit.restart();
  mol->dratom=0;
  if (!noWaitLabel) moving->start(dratpause);
}

void ChGL::setRotationCenter(){
  //! Sets the rotation center to the spezified atom
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index==(int)((GLuint)-1))return;
  rotze=((int)index<mol->showatoms.size())?index:-1;
  if (rotze>-1){ 
    rCenter->show();
    glGetDoublev(GL_MODELVIEW_MATRIX,MM);
    MM[12]=MM[13]=0;
    glLoadMatrixd(MM);
  }
  updateGL();
}

void ChGL::setRotationCenter(int rz){
  /*! Sets the rotation center to the spezified atom
   * @param index of the spezified atom.
  */
  rotze=((int)rz<mol->showatoms.size())?rz:-1;
  if (rotze>-1){
    rCenter->show();
    glGetDoublev(GL_MODELVIEW_MATRIX,MM);
    MM[12]=MM[13]=0;
    glLoadMatrixd(MM);
  }
  updateGL();
}

void ChGL::expand(){//! Lets Molecule.expandAt(int index) serach for neighboring molecules around the specified atom. 
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index==(int)((GLuint)-1))return;
  mol->expandAt(index);
  fuse->setVisible(true);
  grow->setVisible(false);
  emit bigmessage(mol->HumanSymmetry);
  murx=1;
  updateGL();
}

void ChGL::incFontSize(){//! increases label font size
  myFont.setPointSize(myFont.pointSize()+1);
  updateGL();
}

void ChGL::decFontSize(){//! decreases label font size
  myFont.setPointSize(qMax(myFont.pointSize()-1,7));
  updateGL();

}
//#include <iostream>
void ChGL::setReNaMo(bool b){//!enter rename mode 
  inRenameMode=b;
}

void ChGL::rotCenter(){//! reset the rotation center to the center of gravity.
  rotze=-1;
  updateGL();
  rCenter->hide();
}

void ChGL::resizeGL(int width, int height) {
  if (pause) {ww=width; wh=height; return;}
  glViewport(0, 0, ww=width, wh=height);
  glGetIntegerv(GL_VIEWPORT, vp);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( viewAngle, (double)width/height, 5.0, 8000.0 );
}

void ChGL::zoom(double speed){//!scale about speed @param speed scale factor
  moving->start(dratpause);
  printf("%d\n",dratpause);
  glScaled(1.0+speed*0.1,1.0+speed*0.1,1.0+speed*0.1);  
  updateGL();  
}

void ChGL::rotY(double speed){//!rotate around Y axis @param speed in degrees
  moving->start(dratpause);
  glRotateL(-20.0*speed,0.0f,1.0f,0.0f);
  updateGL();  
}

void ChGL::gZoom(double speed){//!scale about speed @param speed scale factor 
  moving->start(dratpause);
  glScaled(1.0+speed*0.02,1.0+speed*0.02,1.0+speed*0.02);
  updateGL();
}

void ChGL::rotZ(double speed){//!rotate around Z axis @param speed in degrees  
  moving->start(dratpause);
  glRotateL(speed,0.0f,0.0f,1.0f);
  updateGL();
}

void ChGL::moveY(double speed){//!translate the molecule in Y direction
  moving->start(dratpause);
  glTranslateL(0.0,speed,0.0);
  updateGL();  
}

void ChGL::moveX(double speed){//!translate the molecule in X direction
  moving->start(dratpause);
  glTranslateL(speed,0.0,0.0);
  //  QMessageBox::information(this,"Px move","xmoved",QMessageBox::Ok);
  updateGL();  
}

void ChGL::rotX(double speed){//!rotate around X axis @param speed in degrees
  moving->start(dratpause);
  glRotateL(-20.0*speed,1.0f,0.0f,0.0f);
  updateGL();  
}


void ChGL::zalman(){//!sets the stereo mode for Zalman monitors.
  stereo_mode=1;
  minus= 1;
  updateGL();
}

QStringList ChGL::resis(){
    QStringList l;
    for (int i=0; i<mol->asymm.size(); i++){
      if (mol->asymm.at(i).resiNr>0){
        if (!l.contains(QString::number(mol->asymm.at(i).resiNr),Qt::CaseInsensitive))
            l.append(QString::number(mol->asymm.at(i).resiNr));
        if (!l.contains(mol->asymm.at(i).ResiClass))
            l.append(mol->asymm.at(i).ResiClass);
      }
    }
    if (!l.isEmpty()) l.insert(0,"*");
    return l;
}

void ChGL::addDFIX(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("DFIX: Geometrical restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *valueline = new QLineEdit(dia);
    QLineEdit *esdline = new QLineEdit(dia);
    QLabel *value = new QLabel("Value:", dia);
    QLabel *esd = new QLabel("ESD:", dia);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,2,0);
      grid->addWidget(resiCom,2,1);
    }
// Set default value for distance and esd
// Get value for DFIX. Current bond distance and default esd - 0.02
    valueline->setText(QString::number(sqrt(Distance(mol->selectedatoms.at(0).pos,mol->selectedatoms.at(1).pos)),10,3));
    esdline->setText("0.02");

    grid->addWidget(value,0,0);
    grid->addWidget(valueline,0,1);
    grid->addWidget(esd,1,0);
    grid->addWidget(esdline,1,1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,4,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertDFIX(valueline->text().toDouble(),esdline->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();

    }


//    updateBondActions();
}

void ChGL::addDANG(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("DANG: Geometrical restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *valueline = new QLineEdit(dia);
    QLineEdit *esdline = new QLineEdit(dia);
    QLabel *value = new QLabel("Value:", dia);
    QLabel *esd = new QLabel("ESD:", dia);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,2,0);
      grid->addWidget(resiCom,2,1);
    }
// Set default value for distance and esd
// Get value for DANG. Current bond distance and default esd - 0.04
    valueline->setText(QString::number(sqrt(Distance(mol->selectedatoms.at(0).pos,mol->selectedatoms.at(1).pos)),10,3));
    esdline->setText("0.04");

    grid->addWidget(value,0,0);
    grid->addWidget(valueline,0,1);
    grid->addWidget(esd,1,0);
    grid->addWidget(esdline,1,1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,4,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertDANG(valueline->text().toDouble(),esdline->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }

}

void ChGL::addFLAT(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("FLAT: Geometrical restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *esdline = new QLineEdit(dia);
    QLabel *esd = new QLabel("ESD:", dia);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,2,0);
      grid->addWidget(resiCom,2,1);
    }
// Set default value for esd
    esdline->setText("0.1");

    grid->addWidget(esd,0,0);
    grid->addWidget(esdline,0,1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,1,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertFLAT(esdline->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }

}

void ChGL::addEXYZ(){
    emit insertEXYZ(mol->selectedatoms);
    mol->selectedatoms.clear();
}

void ChGL::addEADP(){
    emit insertEADP(mol->selectedatoms);
    mol->selectedatoms.clear();
}

void ChGL::addDELU(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("DELU: ADP restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *esd1line = new QLineEdit(dia);
    QLineEdit *esd2line = new QLineEdit(dia);
    QLabel *esd1 = new QLabel("ESD1:", dia);
    QLabel *esd2 = new QLabel("ESD2:", dia);
// Set default value for distance and esd
    esd1line->setText("0.01");
    esd2line->setText("0.01");

    grid->addWidget(esd1,0,0);
    grid->addWidget(esd1line,0,1);
    grid->addWidget(esd2,1,0);
    grid->addWidget(esd2line,1,1);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,3,0);
      grid->addWidget(resiCom,3,1);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,4,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertDELU(esd1line->text().toDouble(),esd2line->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }
}

void ChGL::addSIMU(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("SIMU: ADP restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *esd1line = new QLineEdit(dia);
    QLineEdit *esd2line = new QLineEdit(dia);
    QLineEdit *dmaxline = new QLineEdit(dia);
    QLabel *esd1 = new QLabel("ESD1:", dia);
    QLabel *esd2 = new QLabel("ESD2:", dia);
    QLabel *dmax = new QLabel("dmax:", dia);
// Set default value for distance and esd
    esd1line->setText("0.04");
    esd2line->setText("0.08");
    dmaxline->setText("2.00");

    grid->addWidget(esd1,0,0);
    grid->addWidget(esd1line,0,1);
    grid->addWidget(esd2,1,0);
    grid->addWidget(esd2line,1,1);
    grid->addWidget(dmax,2,0);
    grid->addWidget(dmaxline,2,1);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,3,0);
      grid->addWidget(resiCom,3,1);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,4,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertSIMU(esd1line->text().toDouble(),esd2line->text().toDouble(),dmaxline->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }

}

void ChGL::addISOR(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("ISOR: ADP restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *esd1line = new QLineEdit(dia);
    QLineEdit *esd2line = new QLineEdit(dia);
    QLabel *esd1 = new QLabel("ESD1:", dia);
    QLabel *esd2 = new QLabel("ESD2:", dia);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,3,0);
      grid->addWidget(resiCom,3,1);
    }
// Set default values for s and st
    esd1line->setText("0.1");
    esd2line->setText("0.2");

    grid->addWidget(esd1,0,0);
    grid->addWidget(esd1line,0,1);
    grid->addWidget(esd2,1,0);
    grid->addWidget(esd2line,1,1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,4,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertISOR(esd1line->text().toDouble(),esd2line->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }
}

void ChGL::addRIGU(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("RIGU: ADP restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *esd1line = new QLineEdit(dia);
    QLineEdit *esd2line = new QLineEdit(dia);
    QLabel *esd1 = new QLabel("ESD1:", dia);
    QLabel *esd2 = new QLabel("ESD2:", dia);
// Set default value for distance and esd
    esd1line->setText("0.004");
    esd2line->setText("0.004");
    int i(0); // index for position of widgets in the container
    grid->addWidget(esd1,i,0);
    grid->addWidget(esd1line,i++,1);
    grid->addWidget(esd2,i,0);
    grid->addWidget(esd2line,i++,1);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,i,0);
      grid->addWidget(resiCom,i++,1);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,i,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertRIGU(esd1line->text().toDouble(),esd2line->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }
}

void ChGL::addCHIV(){
    QDialog *dia = new QDialog(this);
    dia->setWindowTitle(tr("CHIV: Geometrical restraint"));
    dia->setModal(false);

    QGridLayout *grid = new QGridLayout(dia);
    QLineEdit *volline = new QLineEdit(dia);
    QLineEdit *esd1line = new QLineEdit(dia);
    QLabel *vol = new QLabel("Volume:", dia);
    QLabel *esd1 = new QLabel("ESD:", dia);
// Set default value for distance and esd
    volline->setText("0.0");
    esd1line->setText("0.1");
    int i(0); // index for position of widgets in the container
    grid->addWidget(vol,i,0);
    grid->addWidget(volline,i++,1);
    grid->addWidget(esd1,i,0);
    grid->addWidget(esd1line,i++,1);
    QStringList rl = resis();
    QComboBox *resiCom = NULL;
    if (!rl.isEmpty()){
      QLabel *resi = new QLabel("Residue specific",dia);
      resiCom = new QComboBox(dia);
      resiCom->addItems(rl);
      grid->addWidget(resi,i,0);
      grid->addWidget(resiCom,i++,1);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();

    OkButton = buttonBox->addButton(QDialogButtonBox::Ok);
    CancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    buttonBox->setOrientation(Qt::Horizontal);

    grid->addWidget(buttonBox,i,0);

    dia->setLayout(grid);
    connect(OkButton, SIGNAL(clicked()), dia, SLOT(accept()));
    connect(CancelButton, SIGNAL(clicked()), dia, SLOT(reject()));

    dia->show();
    int a  = dia->exec();

    if(a == QDialog::Accepted)
    {
// read value from value line
        QString s=(rl.isEmpty())?"":resiCom->currentText();
        emit insertCHIV(volline->text().toDouble(),esd1line->text().toDouble(),mol->selectedatoms,s);
        mol->selectedatoms.clear();
    }
    else
    {
        if(a == QDialog::Rejected)
            dia->close();
    }
}

void ChGL::addANIS()
{
    emit insertANIS(mol->selectedatoms);
    mol->selectedatoms.clear();
}


void ChGL::parallel(){//!sets the stereo mode for parallel eye side by side stereo
  stereo_mode=2;
  minus=1;
  updateGL();
}

void ChGL::crosseye(){//!sets the stereo mode for crossed eye side by side stereo
  stereo_mode=3;
  minus=-1;
  updateGL();
}

void ChGL::anaglyphRedCyan(){//!sets the stereo mode for analglyph stereo Red and Cyan glasses. 
  stereo_mode=4;
  minus=1;
  updateGL();
}

void ChGL::hardwareStereo(){//!sets the stereo mode for hardware or driver supported stereo mode
    if (format().stereo()){
     stereo_mode=5;
     minus=-1;
     updateGL();
   }
  else {
    qDebug()<<"no stereo sorry!";
    emit no_hw_st();
  }
}

void ChGL::nostereo(){//!turns the stereo mode off
  stereo_mode=0;
  updateGL();
}

void ChGL::clearEnvi(){ 
  /*! Clears ChGL.enviPositions, ChGL.enviKat. and ChGL.labs */
enviPositions.clear();
labs.clear();
enviKat.clear();
enviButt->setVisible(false);
rotCenter();
}

void ChGL::envi(){
  /*! Finds neighboring atoms around a ChGL.envirange the spezified atom and passes a html table via ChGL.bigmessage
   * Feeds ChGL.enviPositions, ChGL.labs  and ChGL.enviKat. 
   * sets the rotation center to the spezified atom.
   * */
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index<0) return;
  if (index>=mol->asymm.size())return;
  mol->enviSDM(envirange);
  enviPositions.clear();
  enviKat.clear();
  labs.clear();
  QList<bool> covs;

  V3 ppc,ppf,p0;
 // int ssy=0;
  QString systr;
  QStringList bs;
  QString info;
  enviP0=mol->asymm.at(index).pos;
  info.append(QString("<hr><b>Environment of %1 </b><table border=\"0\" cellspacing=\"0\" cellpadding=\"5\">").arg(mol->asymm[index].Label));
  for (int i = 0; i < mol->envi_sdm.size(); i++){
    if ((mol->envi_sdm.at(i).a2==index)&&mol->envi_sdm.at(i).d<envirange) {
        if ((!mol->envi_sdm.at(i).covalent)&&(enviCova->isChecked())) continue;
      if ((mol->asymm[mol->envi_sdm.at(i).a1].an==-1)&&(enviNoQ->isChecked())) continue;
      ppf=mol->cell.symmops.at(mol->envi_sdm.at(i).sn) *
              mol->asymm[mol->envi_sdm.at(i).a1].frac +
              mol->cell.trans.at(mol->envi_sdm.at(i).sn) -//-
	      mol->envi_sdm.at(i).floorD;
      mol->frac2kart(ppf,ppc);
      mol->frac2kart(mol->asymm[index].frac,p0);
/*
if (fabs(mol->envi_sdm.at(i).d-sqrt(Distance(p0,ppc)))>0.5){
	ppf=mol->cell.symmops.at(mol->envi_sdm.at(i).sn) *
                mol->asymm[mol->envi_sdm.at(i).a2].frac -
                mol->cell.trans.at(mol->envi_sdm.at(i).sn) -
		mol->envi_sdm.at(i).floorD;
	mol->frac2kart(ppf,ppc);

      }// */
      enviPositions.append(ppc);
      covs.append(mol->envi_sdm.at(i).covalent);
      enviKat.append((mol->envi_sdm.at(i).covalent)?1:0);
      if((abs(mol->asymm[mol->envi_sdm.at(i).a1].an-7)<2)
              &&(abs(mol->asymm[mol->envi_sdm.at(i).a2].an-7)<2)
              &&(fabs(mol->envi_sdm.at(i).d-2.725)<0.275)){enviKat.last()=2;}

      if((mol->asymm[mol->envi_sdm.at(i).a1].an==0)&&(abs(mol->asymm[mol->envi_sdm.at(i).a2].an-7)<2)&&(fabs(mol->envi_sdm.at(i).d-1.875)<0.275)){enviKat.last()=2;}
      if((mol->asymm[mol->envi_sdm.at(i).a2].an==0)&&(abs(mol->asymm[mol->envi_sdm.at(i).a1].an-7)<2)&&(fabs(mol->envi_sdm.at(i).d-1.875)<0.275)){enviKat.last()=2;}
      bool symm=((mol->envi_sdm.at(i).sn)||(!(mol->envi_sdm.at(i).floorD==V3(0,0,0))));
      if (symm) {
	QString sss=QString("%1_%2%3%4:%5,").arg(mol->envi_sdm.at(i).sn+1).arg(5-(int)mol->envi_sdm.at(i).floorD.x).arg(5-(int)mol->envi_sdm.at(i).floorD.y).arg(5-(int)mol->envi_sdm.at(i).floorD.z).arg(mol->asymm[mol->envi_sdm.at(i).a1].molindex);
	if (!bs.contains(sss)){
	  bs.append(sss);
	}

	systr=QString("%1%2").arg(QString::fromUtf8("»")).arg(bs.indexOf(sss)+1);
        labs.append(mol->asymm[ mol->envi_sdm.at(i).a1].Label+systr);
      }
      else labs.append(mol->asymm[ mol->envi_sdm.at(i).a1].Label);
      info.append(QString("<tr><th style=\"background:%6\" >%1%4</th><td style=\"background:%5\" >%7<font color=%3> %2&nbsp;&Aring;</font></b></td>")
                      .arg(mol->asymm[ mol->envi_sdm.at(i).a1].Label)
		      .arg(mol->envi_sdm.at(i).d,8,'f',3)
		      .arg((mol->envi_sdm.at(i).covalent)?"green":"black")
		      .arg(symm?systr:"").arg((labs.size()%2)?"#eeeeee":"white")
		      .arg((labs.size()%2)?"#d4d4e4":"#efefff")
		      .arg((mol->envi_sdm.at(i).covalent)?"<b>":""));
      for (int j=0; j<enviPositions.size()-1; j++){
	double w=
                mol->winkel(p0-enviPositions.at(j),
                                p0-enviPositions.last());
//		printf("%s-%s-%s %g   %g %g %g\n",labs.at(j).toStdString().c_str(),mol->asymm[index].Label.toStdString().c_str(),labs.last().toStdString().c_str(),w,enviPositions.at(j).x ,enviPositions.at(j).y,enviPositions.at(j).z);
	info.append(QString("<td style=\"background:%3\" align=right>%4<font color=%2> %1&deg;</font></b></td>")
			.arg(w,8,'f',2)
			.arg(((mol->envi_sdm.at(i).covalent)&&(covs.at(j)))?"green":"black")
			.arg(((j+1)%2)?(labs.size()%2)?"#d4d4d4":"#e4e4e4":(labs.size()%2)?"#eeeeee":"#ffffff")
                        .arg(((mol->envi_sdm.at(i).covalent)&&(covs.at(j)))?"<b>":""));

      }
      info.append("</tr>\n");
    }
  }
  info.append("<tr><td style=\"background:#efefff\"></td><td style=\"background:#efefff\"></td>");
  for (int j=0; j<enviPositions.size()-1; j++){
    info.append(QString("<th style=\"background:%2\">%1</th>").arg(labs.at(j)).arg(((j+1)%2)?"#d4d4e4":"#efefff"));
  }
  QString symml="";
  for (int j=0; j<bs.size(); j++){
    symml+=mol->symmcode2human(bs.at(j),j+1);
  }
  info.append(QString("</tr>\n</table>%1<hr>\n").arg(symml));
  emit bigmessage(info);

  enviButt->setVisible(true);
  setRotationCenter(index);
}

void ChGL::paintGL(){
  if (pause)  return;
  warLabel = drawLa;
  GLfloat fgc[4]={(GLfloat)backGroundColor.redF(),(GLfloat)backGroundColor.greenF(),(GLfloat)backGroundColor.blueF(),1.0};
  glFogfv(GL_FOG_COLOR,fgc);
  qglClearColor(backGroundColor);
  // printf("%d %d bas %d %g \n",ww,wh,bas,viewAngle);
  glViewport(0, 0, ww, wh);
  glDisable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  printf("clear %d %d\n",ww,wh);
  glViewport(0, 0, ww, wh);
  glDisable(GL_STENCIL_TEST);
  // printf("%d ___%d %d %d\n",stereo_mode,ww,wh,mibas);
  glGetIntegerv(GL_VIEWPORT, vp);
  if (!stereo_mode){
    glDrawBuffer(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
    glMatrixMode(GL_MODELVIEW);
    if (xray_vision) {  
      glPushMatrix();
      glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
      glEnable(GL_SCISSOR_TEST);
      qglClearColor(backGroundColor);
      //glClearColor(0.0,0.0,1.0,0.7);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      xray_eye=1;
      if (!pause) draw();
      glPopMatrix();
      //      }else
      //	if (xray_eye==2)  {
      glPushMatrix();
      glScissor(vp[0],vp[1],vp[2],vp[3]);
      glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
      qglClearColor(backGroundColor);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_SCISSOR_TEST);
      xray_eye=2;
      if (!pause) draw();
      glPopMatrix();
      xray_eye=0;
  }else {
    glPushMatrix();
    glDisable(GL_SCISSOR_TEST);
    xray_eye=0;
    if (!pause) draw();
    glPopMatrix();
  }
  }else 
    if (stereo_mode==1){// stereo zalman
    glDrawBuffer(GL_BACK);
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT,viewport);

      glPushAttrib(GL_ENABLE_BIT);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(0,viewport[2],0,viewport[3],-10.0,10.0);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(0.33F,0.33F,0.0F); 
      glDisable(GL_STENCIL_TEST);
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_LIGHTING);
      glDisable(GL_FOG);
      glDisable(GL_NORMALIZE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_DITHER);
      glDisable(GL_BLEND);
      glShadeModel(GL_SMOOTH);
      glDisable(0x809D); // GL_MULTISAMPLE_ARB * / 

      glClearStencil(0);
      glColorMask(false,false,false,false);
      glDepthMask(false);
      glClear(GL_STENCIL_BUFFER_BIT);

      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 1, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

      glLineWidth(1.0);
      glBegin(GL_LINES);
      int h = viewport[3], w=viewport[2];
      int y;
      for(y=0;y<h;y+=2) {
	glVertex2i(0,y);
	glVertex2i(w,y);
      }
      glEnd();

      glColorMask(true,true,true,true);
      glDepthMask(true);

      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      //
      glPopAttrib();

      glViewport(0, 0, ww, wh);        
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, ww, wh);        
      glGetIntegerv(GL_VIEWPORT, vp);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
      glMatrixMode(GL_MODELVIEW);
      if (xray_vision){
	glPushMatrix();
	drawLa=false;
	glRotateL(minus*-1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=1;
	glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	glEnable(GL_SCISSOR_TEST);
	//      qglClearColor(backGroundColor);
	//glClearColor(0.0,0.0,1.0,0.7);
	//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!pause) draw();
	glPopMatrix();
	glPushMatrix();
	drawLa=false;
	glRotateL(minus*-1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=2;
	glScissor(vp[0],vp[1],vp[2],vp[3]);
	glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	glEnable(GL_SCISSOR_TEST);
	if (!pause) draw();
	xray_eye=0;
	glPopMatrix();
      }else{
	glPushMatrix();
	drawLa=false;
	glRotateL(minus*-1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=0;
	glDisable(GL_SCISSOR_TEST);
	if (!pause) draw();
	glPopMatrix();
      }
      drawLa= warLabel;
      if (xray_vision) {
	glPushMatrix();
	glRotateL(minus*1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=1;
	glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	glEnable(GL_SCISSOR_TEST);
	//      qglClearColor(backGroundColor);
	//glClearColor(0.0,0.0,1.0,0.7);
	//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!pause) draw();
	glPopMatrix();
	glPushMatrix();
	glRotateL(minus*1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=2;
	glScissor(vp[0],vp[1],vp[2],vp[3]);
	glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	//      qglClearColor(backGroundColor);
	//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	if (!pause) draw();
	glPopMatrix();
      }else
      {
	glPushMatrix();
	glRotateL(minus*1.5,0,1,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);// */
	xray_eye=0;
	glDisable(GL_SCISSOR_TEST);
	if (!pause) draw();
	glPopMatrix();
      }
    }else  
      if ((stereo_mode>1)&&(stereo_mode<4)) { //stereo_side by side
    glDrawBuffer(GL_BACK);
	if (xray_vision){
	  glPushMatrix();
	  glViewport(0, 0, ww/2, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=1;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	  glPushMatrix();
	  glViewport(0, 0, ww/2, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=2;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	}else{
	  glPushMatrix();
	  glViewport(0, 0, ww/2, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=0;
	  glDisable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	}


	if (xray_vision){
	  glPushMatrix();
	  glViewport( ww / 2 , 0, ww / 2,wh );        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=1;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor((ww / 2)+vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	  glPushMatrix();
	  glViewport( ww / 2 , 0,ww / 2,wh );        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=2;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor((ww / 2)+vp[2]/2,vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	}else{
	  glPushMatrix();
	  glViewport( ww / 2 , 0,ww / 2,wh );        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww/2.0)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=0;
	  glDisable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	}
      }
      else  if (stereo_mode==4){//anaglyph red cyan
	glDrawBuffer(GL_BACK);
	if (xray_vision){
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	  glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
	  glPushMatrix();
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=1;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  //bbb
	  if (!pause) draw();
	  //bbb
	  glPopMatrix();
	  glPopMatrix();
	  glPushMatrix();
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=2;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  //aaa
	  if (!pause) draw();
	  //aaa
	  glPopMatrix();
	  glPopMatrix();

	  glDisable(GL_SCISSOR_TEST);
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	  glClear(GL_DEPTH_BUFFER_BIT);
	  glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);

	  glPushMatrix();
	  glViewport( 0  , 0, ww ,wh );        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=1;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  //dddd
	  if (!pause) draw();
	  //dddd
	  glPopMatrix();
	  glPopMatrix();
	  glPushMatrix();
	  glViewport( 0 , 0,ww ,wh );        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)(ww)/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=2;
	  //	  glGetIntegerv(GL_VIEWPORT, vp);
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  //ccc
	  if (!pause) draw();
	  //ccc
	  glPopMatrix();
	  glPopMatrix();
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
	else{
	  glGetIntegerv(GL_VIEWPORT, vp);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	  glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

	  // set camera for blue eye, red will be filtered.

	  // draw scene
	  glPushMatrix();
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=0;
	  glDisable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();

	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	  glClear(GL_DEPTH_BUFFER_BIT);
	  glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);

	  // set camera for red eye, blue will be filtered.

	  // draw scene
	  glPushMatrix();
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=0;
	  glDisable(GL_SCISSOR_TEST);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}	 

      }else  if (stereo_mode==5){//hardware stereo ???
	//todo  is this working??
	//x-ray view 
	if (xray_vision){
	  glPushMatrix();//2
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();//1
	  glRotateL(minus*1.5,0,1,0);
	  xray_eye=1;
	  glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  glDrawBuffer(GL_BACK_RIGHT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  glPopMatrix();//1
	  glPushMatrix();//1
	  glRotateL(minus*1.5,0,1,0);
	  xray_eye=2;
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  glDrawBuffer(GL_BACK_RIGHT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  xray_eye=0;
	  glPopMatrix();//1
	  glPopMatrix();//2

	  glPushMatrix();//2
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);// */
	  glPushMatrix();//1
	  glRotateL(minus*-1.5,0,1,0);
	  xray_eye=1;
	  glScissor(vp[0],vp[1],vp[2]/2,vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  glDrawBuffer(GL_BACK_LEFT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  glPopMatrix();//1
	  glPushMatrix();//2
	  glRotateL(minus*-1.5,0,1,0);
	  xray_eye=2;
	  glScissor(vp[0],vp[1],vp[2],vp[3]);
	  glScissor(vp[2]/2,vp[1],vp[2],vp[3]);
	  glEnable(GL_SCISSOR_TEST);
	  glDrawBuffer(GL_BACK_LEFT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  xray_eye=0;
	  glPopMatrix();//1
	  glPopMatrix();//2
	}
	else{
	  glGetIntegerv(GL_VIEWPORT, vp);

	  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glPushMatrix();//2
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(1.5*minus,0,1,0);
	  xray_eye=0;
	  glDrawBuffer(GL_BACK_RIGHT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();

	  glPushMatrix();
	  glViewport(0, 0, ww, wh);        
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glRotateL(-1.5*minus,0,1,0);
	  xray_eye=0;
	  glDrawBuffer(GL_BACK_LEFT);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  if (!pause) draw();
	  glPopMatrix();
	  glPopMatrix();
	}
      }
}

void ChGL::setViewAngle(double ang){
  /*! sets ChGL.viewAngele for the perspective view 0 is othogonal, 29 degrees is the perpective default.
   */
  if ((ang>0.001)&&(ang<160.0)){
     // printf("View angle = %g degrees. %10.8f %f\n",ang,ang/viewAngle,viewAngle);
    glScaled(ang/viewAngle,ang/viewAngle,ang/viewAngle);
    viewAngle=ang;
    homeXY();
  }
}

void ChGL::rehide(){//! Trys to hide previous hidden atoms.
//    printf("haa %d %d %g\n",hideReason,hiddenThings,qcutoff);
  if (!hiddenThings) return;
  if ((hideReason&HIDE_REASON_SELECT)&&(mol->selectedatoms.size())) {hideNonSelected(); return;}
  else if (hideReason&HIDE_REASON_SELECT) hideReason-=HIDE_REASON_SELECT;
  int gute=frid;
  for (int i=0;i<mol->showatoms.size();i++){
    if (hideReason&HIDE_REASON_QPEAK) if (mol->showatoms[i].an<0) {if (qcutoff>0)
            mol->showatoms[i].hidden=((mol->showatoms[i].peakHeight>0)&&(mol->showatoms[i].peakHeight<qcutoff))?1:0;
        else mol->showatoms[i].hidden=((mol->showatoms[i].peakHeight<0)&&(mol->showatoms[i].peakHeight>qcutoff))?1:0;}
    if (hideReason&HIDE_REASON_HYDROGEN) mol->showatoms[i].hidden=
      (mol->showatoms.at(i).an==0)?1:mol->showatoms.at(i).hidden;
    if (hideReason&(HIDE_REASON_THIS_FRAGMENT|HIDE_REASON_OTHER_FRAGMENT)){
      if (mol->showatoms.at(i).an>=0){
	if (hideReason&HIDE_REASON_THIS_FRAGMENT) mol->showatoms[i].hidden=
	  (mol->showatoms.at(i).molindex+9999*mol->showatoms.at(i).symmGroup!=gute)?mol->showatoms.at(i).hidden:1;
	if (hideReason&HIDE_REASON_OTHER_FRAGMENT)mol->showatoms[i].hidden=
	  (mol->showatoms.at(i).molindex+9999*mol->showatoms.at(i).symmGroup!=gute)?1:mol->showatoms.at(i).hidden;
      }
    }
  }

}

void ChGL::hideNonSelected(){//! Hides all atoms that are not selected
  for (int i=0;i<mol->showatoms.size();i++) mol->showatoms[i].hidden=1;
  for(int i=0;i<mol->selectedatoms.size();i++){
    if (mol->selectedatoms.at(i).style < mol->showatoms.size())
      mol->showatoms[mol->selectedatoms.at(i).style].hidden=0;
    //   std::cout<<mol->showatoms.at(mol->selectedatoms.at(i).style).Label.toStdString()<<std::endl;
  }
  hiddenThings=true;
  hideReason|=HIDE_REASON_SELECT;
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::hideSelected(){//! Hides the selected atoms
  for(int i=0;i<mol->selectedatoms.size();i++){
    if (mol->selectedatoms.at(i).style < mol->showatoms.size())
      mol->showatoms[mol->selectedatoms.at(i).style].hidden=1;
    //   std::cout<<mol->showatoms.at(mol->selectedatoms.at(i).style).Label.toStdString()<<std::endl;
  }
  hiddenThings=true;
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::hideThisFragment(){//! Hides the specified molecular fragment.
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index>mol->showatoms.size()) return;
  int gute=mol->showatoms.at(index).molindex+9999*mol->showatoms.at(index).symmGroup;
  //int gute=mol->showatoms.at(index).molindex;
  for (int i=0;i<mol->showatoms.size();i++) 
    if (mol->showatoms.at(i).an>=0)
      mol->showatoms[i].hidden=
	      (mol->showatoms.at(i).molindex+9999*mol->showatoms.at(i).symmGroup!=gute)?mol->showatoms.at(i).hidden:1;
  //(mol->showatoms.at(i).molindex!=gute)?mol->showatoms.at(i).hidden:1;
  //
  frid=gute;
  hiddenThings=true;
  hideReason|=HIDE_REASON_THIS_FRAGMENT;
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::selectThisFragment(){//! Selects the specified molecular fragment
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index>mol->showatoms.size()) return;
  mol->selectedatoms.clear();
  int gute=mol->showatoms.at(index).molindex+9999*mol->showatoms.at(index).symmGroup;
  //int gute=mol->showatoms.at(index).molindex;
  for (int i=0;i<mol->showatoms.size();i++) 
    if (mol->showatoms.at(i).an>=0)
      if(mol->showatoms.at(i).molindex+9999*mol->showatoms.at(i).symmGroup==gute){
	mol->selectedatoms.append(mol->showatoms[i]);
	mol->selectedatoms.last().style=i;
      }
  //(mol->showatoms.at(i).molindex!=gute)?mol->showatoms.at(i).hidden:1;
  //
  updateBondActions();
  updateGL();
}

void ChGL::hideHydrogens(){//! Hides all Hydrogran atoms
  for (int i=0;i<mol->showatoms.size();i++) mol->showatoms[i].hidden=
    (mol->showatoms.at(i).an==0)?1:mol->showatoms.at(i).hidden;
  hiddenThings=true;
  hideReason|=HIDE_REASON_HYDROGEN;
  hideh->setVisible(false);
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::hideOtherFragments(){//! Hides all molecular fragments except from the specified one
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if (index==(int)((GLuint)-1))return;
  if (index>mol->showatoms.size()) return;
  int gute=mol->showatoms.at(index).molindex+9999*mol->showatoms.at(index).symmGroup;
  //int gute=mol->showatoms.at(index).molindex;
  for (int i=0;i<mol->showatoms.size();i++) 
    if (mol->showatoms.at(i).an>=0)
      mol->showatoms[i].hidden=
	      //		(mol->showatoms.at(i).molindex!=gute)?1:mol->showatoms.at(i).hidden;
	      (mol->showatoms.at(i).molindex+9999*mol->showatoms.at(i).symmGroup!=gute)?1:mol->showatoms.at(i).hidden;
  hiddenThings=true;
  frid=gute;
  hideReason|=HIDE_REASON_OTHER_FRAGMENT;
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::highliteQPeak(double co){//! highligts the first Q-Peak with a lower or equal peak height than co. @param co cutoff value.
  imFokus=-1;
  //printf("co %f\n",co)
  for (int i=0;i<mol->showatoms.size();i++){
    if (mol->showatoms[i].an==-66) continue;
    if (mol->showatoms[i].an>=0) continue;
    if (mol->showatoms[i].hidden)continue;
    if (mol->showatoms[i].peakHeight<=co) {imFokus=i; break;}
  }
  updateGL();
}

void ChGL::hideQPeaksBelow(double cutoff){//!hides all Q-Peaks below the given cutoff value. @param cutoff The cutoff value.
    //qDebug()<<cutoff;
  int vor=0,nach=0;
  for (int i=0;i<mol->showatoms.size();i++){
    vor += mol->showatoms[i].hidden;
    if ((mol->showatoms[i].an < 0)&&(cutoff>0)) mol->showatoms[i].hidden = ((mol->showatoms[i].peakHeight>=0)&&(mol->showatoms[i].peakHeight<cutoff))?1:0;
    else if ((mol->showatoms[i].an < 0)&&(cutoff<0)) mol->showatoms[i].hidden = ((mol->showatoms[i].peakHeight<=0)&&(mol->showatoms[i].peakHeight>cutoff))?1:0;
//    if (mol->showatoms[i].an < 0) printf("%d %g %d\n",mol->showatoms[i].hidden,mol->showatoms[i].peakHeight,mol->showatoms[i].peakHeight<cutoff);
    if (mol->showatoms[i].an == -66) mol->showatoms[i].hidden = 0;
    nach += mol->showatoms[i].hidden;
  }
  if (vor==nach) return;
  hiddenThings=true;
  hideReason|=HIDE_REASON_QPEAK;
  qcutoff=cutoff;
  mol->selectedatoms.clear();
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::showHidden(){//! shows all hidden objects.
  int h=0;
  for (int i=0;i<mol->showatoms.size();i++){
    mol->showatoms[i].hidden=0;
    if (mol->showatoms[i].an==0)h++;
  }
  if (h)
    hideh->setVisible(true);
  hiddenThings=false;
  hideReason=0;
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::hidePartMinusOne(bool off){//! toggles the visibility state of part -N ghost atoms @param off if true no ghosts are visible.

  mol->nopm1=off;
  murx=1;
  updateBondActions();
  updateGL();
}

void ChGL::setupTexture(){//!loads the texture images
  deleteTexture(mol->adpwall);
  deleteTexture(mol->adpwall_plaid);
  deleteTexture(mol->hbtex);

  mol->adpwall_plaid=bindTexture(QImage(QString(":/icons/adpwall.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glBindTexture(GL_TEXTURE_2D, 0);

  mol->adpwall=bindTexture(QImage(QString(":/icons/adpwall3.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glBindTexture(GL_TEXTURE_2D, 0);

  mol->hbtex=bindTexture(QImage(QString(":/icons/hbb.png")),GL_TEXTURE_2D);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glBindTexture(GL_TEXTURE_2D, 0);
}

void ChGL::homeXY(){//! Resets the molecule to rotate in the middle of the viewport.
  glGetDoublev(GL_MODELVIEW_MATRIX,MM);
  MM[12]=MM[13]=0;
  glLoadMatrixd(MM);
  updateGL();
}

void ChGL::draw(){//! draws everything some times by calling display lists.
  QFontMetrics fmtr(myFont);
  nonAtomFont = myFont;
  nonAtomFont.setPointSize(qMax(((myFont.pointSize()*2)/3),7));
  QFontMetrics fmtr2(nonAtomFont);
  if (exporting) LabelZ.clear();
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
  if (murx) {
    //    printf("www %d %d %d\n",mol->showatoms.size(),bas, mibas);
    rehide();
    initLists();
    murx=0;
  }
  V3 ori=V3(0,0,0);///rotation origin
  V3 sumse=V3(0,0,0);
  int zz=0;
  objCnt=0;
  if (mol->showatoms.size()){
    for (int i=0;i<mol->showatoms.size();i++){
      if (!mol->showatoms[i].hidden) objCnt++;
      if ((!mol->showatoms.at(i).hidden)) {//(mol->showatoms.at(i).an>-1)&&
        sumse+=mol->showatoms[i].pos;
	zz++;
      }
    }

    if (zz) sumse*=1.0/zz;
    else {
      for (int i=0;i<mol->showatoms.size();i++){
	if (!mol->showatoms.at(i).hidden) {
	  sumse+=mol->showatoms[i].pos;
	  zz++;
	}
      }
      if (zz) sumse*=1.0/zz;
    }
  }
  //  printf(" zz %d %d \n",zz,mol->showatoms.size());
  glGetDoublev( GL_MODELVIEW_MATRIX, (double*)MM );
  double gmat[16];
  glGetDoublev( GL_MODELVIEW_MATRIX, (double*)gmat );
  ori.x=gmat[0] * sumse.x + gmat[4] * sumse.y + gmat[8] * sumse.z;
  ori.y=gmat[1] * sumse.x + gmat[5] * sumse.y + gmat[9] * sumse.z;
  ori.z=gmat[2] * sumse.x + gmat[6] * sumse.y + gmat[10] * sumse.z;
  if (mol->showatoms.size()<rotze){
    rotze=-1;
    rCenter->hide();
  }
  if (rotze>-1) {
    double gmat[16];
    sumse=mol->showatoms.at(rotze).pos;
    glGetDoublev( GL_MODELVIEW_MATRIX, (double*)gmat );
    ori.x=gmat[0] * mol->showatoms.at(rotze).pos.x + gmat[4] * mol->showatoms.at(rotze).pos.y + gmat[8] *  mol->showatoms.at(rotze).pos.z;
    ori.y=gmat[1] * mol->showatoms.at(rotze).pos.x + gmat[5] * mol->showatoms.at(rotze).pos.y + gmat[9] *  mol->showatoms.at(rotze).pos.z;
    ori.z=gmat[2] * mol->showatoms.at(rotze).pos.x + gmat[6] * mol->showatoms.at(rotze).pos.y + gmat[10] * mol->showatoms.at(rotze).pos.z;
  }
  if (centerSelection->isChecked()){
    if (mol->selectedatoms.isEmpty()) {
      sumse=altemitte;//centerSelection->setChecked(false);
      glGetDoublev( GL_MODELVIEW_MATRIX, (double*)gmat );
      ori.x=gmat[0] * sumse.x + gmat[4] * sumse.y + gmat[8] * sumse.z;
      ori.y=gmat[1] * sumse.x + gmat[5] * sumse.y + gmat[9] * sumse.z;
      ori.z=gmat[2] * sumse.x + gmat[6] * sumse.y + gmat[10] * sumse.z;
    }
    else {
      sumse=V3(0,0,0);
      for (int i=0;i<mol->selectedatoms.size();i++)
	sumse+=mol->selectedatoms[i].pos;
      sumse*=1.0/mol->selectedatoms.size();
      glGetDoublev( GL_MODELVIEW_MATRIX, (double*)gmat );
      ori.x=gmat[0] * sumse.x + gmat[4] * sumse.y + gmat[8] * sumse.z;
      ori.y=gmat[1] * sumse.x + gmat[5] * sumse.y + gmat[9] * sumse.z;
      ori.z=gmat[2] * sumse.x + gmat[6] * sumse.y + gmat[10] * sumse.z;


    }
  }
  glPushMatrix();
  double mat[16];
  glEnable(GL_BLEND);
  glEnable(GL_COLOR_MATERIAL);
  glGetDoublev( GL_MODELVIEW_MATRIX, (double*)mat );
  glLoadIdentity();
  glDisable(GL_CLIP_PLANE1);
  glDisable(GL_CLIP_PLANE2);
  
  double 
	  clip_plane1[]={1.0,0.0,-1.0, xrdist},
	  clip_plane2[]={-1.0,0.0,-1.0,xrdist};
  glClipPlane(GL_CLIP_PLANE1,clip_plane1);
  glClipPlane(GL_CLIP_PLANE2,clip_plane2);
  glDisable( GL_LIGHTING ); 
  glDisable( GL_DEPTH_TEST ); 
  if ((bggradient)&&(!depthcueing)) { 
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 29, (double)ww/wh, 5.0, 8000.0 );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_QUADS);
    double xx = ((double) ww / wh) * 2.0,
	   yy = 1.77777777778;
    glColor4f(1.0,1.0,1.0,0.5);
    //glTexCoord2d(-1,-1);
    glVertex3f(-xx,-yy,-6.0);
    glColor4f(1.0,1.0,1.0,0.5);
    //glTexCoord2d(0,-1);
    glVertex3f( xx,-yy,-6.0);
    //glTexCoord2d(0,0);
    glColor4f(0.3,0.3,0.3,0.7);
    glVertex3f( xx, yy,-6.0);
    glColor4f(0.3,0.3,0.3,0.7);
    //glTexCoord2d(-1,0);
    glVertex3f(-xx, yy,-6.0);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }  
  if (rectangle){
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,ww,0.0,wh,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable(GL_LINE_STIPPLE);
    glEnable(GL_BLEND);
    glLineWidth(0.3);
    glDisable(GL_LIGHTING);
    glLineStipple(3,21845);
    glBegin(GL_LINE_STRIP);
    qglColor(labelColor);
    glVertex3f(scrx0,wh-scry0,0.0f);
    glVertex3f(scrx,wh-scry0,0.0f);
    glVertex3f(scrx,wh-scry,0.0f);
    glVertex3f(scrx0,wh-scry,0.0f);
    glVertex3f(scrx0,wh-scry0,0.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( viewAngle, (double)ww/wh, 5.0, 8000.0 );
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
  if (depthcueing) glEnable(GL_FOG);
  else glDisable(GL_FOG);
  glEnable( GL_LIGHTING ); 
  glEnable( GL_DEPTH_TEST ); 
  if (xray_eye==1)  {
    glEnable(GL_CLIP_PLANE2);
  }else 
    if (xray_eye==2)  {
      glEnable(GL_CLIP_PLANE1);
    }
  glLoadMatrixd(mat);
  glPopMatrix();
  glPushMatrix();
  if (!(altemitte==sumse)){
    //printf("neuemitte %f %f %f\n",sumse.x,sumse.y,sumse.z);
    altemitte=sumse;
    emit neuemitte(altemitte);
  }
  glTranslateL(-L*ori.x,-L*ori.y,-L*ori.z);
  if ((wireButt->isChecked())&&(moving->isActive())) {
   if (glIsList(wirbas)) glCallList(wirbas);//wirebonds
   if ((drawADP)&&(glIsList(wirbas+1))) glCallList(wirbas+1);//adps
   else if ((glIsList(wirbas+2))) glCallList(wirbas+2);//no adps
   if (glIsList(bas+6)) glCallList(bas+6);//qpeak-bonds
  }
  else {
    if (bas) {
      //     if (drawAt&&drawADP) {glEnable(GL_BLEND); glCallList(bas+2);}//transparent ellipsoids
      if (drawBo&&glIsList(bas+1)) {
	if (mol->bondColorStyle){
	  qglColor(mol->bondColor);
	  glDisable(GL_COLOR_MATERIAL);
	}
	if (drawADP) glCallList(bas+1);
	else if (tubes) glCallList(bas+9); 
	else glCallList(bas+8);//bonds
	glEnable(GL_COLOR_MATERIAL);
      }
      if (drawHb)  {
	// glCallList(bas+7);//h-bonds
	glPushMatrix();{
	  glScaled( L, L, L );
	  mol->tubes=(tubes&&!drawADP)?1:0;
	  mol->adp=(drawADP)?1:0;
	  mol->h_bonds2(mol->showbonds,mol->showatoms);
	}glPopMatrix();

      }
      if (qPeakBonds->isChecked())glCallList(bas+6);//bonds
      if (drawAt&&drawADP&&glIsList(bas)) {
	glCallList(bas);//ellipsoids
      }
      if (drawAt&&drawADP) glCallList(bas+2);//transparent ellipsoids
      if (drawAt&&!drawADP) {
	if (tubes) {glDisable(GL_BLEND);glCallList(bas+5);} //tubes
	else {glDisable(GL_BLEND);glCallList(bas+3);}//balls
      }
    }//bas
  }//wireButt

  if (foubas[0]|foubas[1]|foubas[2]) {
    double max;
    int Pers=0;
    double mm[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, (double*)mm );
    max= (fabs(mm[2])>fabs(mm[6]))?mm[2]:mm[6];
    max=(fabs(max)<fabs(mm[10]))?mm[10]:max;
    if ((max==mm[2])&&(max>0.0)) Pers=0; else
      if ((max==mm[2])&&(max<0.0)) Pers=1; else
	if ((max==mm[6])&&(max>0.0)) Pers=2; else
	  if ((max==mm[6])&&(max<0.0)) Pers=3; else
	    if ((max==mm[10])&&(max>0.0)) Pers=4; else
	      if ((max==mm[10])&&(max<0.0)) Pers=5;
    //	printf ("Pers = %d\n",Pers);
    if (!niceTrans->isChecked())Pers=0;
    glDisable(GL_CULL_FACE);
    if (fillMap->isChecked()) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (lighting->isChecked())  glEnable(GL_LIGHTING); 
    else glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    if (foact->isChecked()) {
      glCallList(foubas[2+4*Pers]);
      if (neutrons) glCallList(foubas[3+4*Pers]);
    }
    if (fofcact->isChecked()) {
      glCallList(foubas[0+4*Pers]);
      glCallList(foubas[1+4*Pers]);
    }
    glEnable(GL_LIGHTING);
    //}
//    if (fofcact->isChecked()) {
//      glCallList(foubas[0+4*Pers]);
//      glCallList(foubas[1+4*Pers]);
//    }
//    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  if (!mol->selectedatoms.isEmpty()){
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=0;
      mol->intern=1;
      mol->adp=0;
      mol->dratom=1;
      mol->atoms(mol->selectedatoms);
      qglColor(labelColor);
      mol->dratom=0;
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }glPopMatrix();
  }
  if (highlightParts->isChecked()){//PART highlighter
    int alod=mol->LOD;
    mol->LOD=3;
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=(tubes&&!drawADP)?1:0;
      mol->intern=0;
      mol->adp=(drawADP)?1:0;
      mol->dratom=2;
      mol->atoms(mol->showatoms);
      qglColor(labelColor);
    }glPopMatrix();
    glPushMatrix();{
      glScaled( L, L, L );
      mol->bonds(mol->showbonds);
    }glPopMatrix();
    mol->dratom=0;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    mol->LOD=alod;
  }
  if (mol->duplicateAtoms.size()){//Duplicate highlighter
    int alod=mol->LOD;
    mol->LOD=3;
    glPushMatrix();{
      glScaled( L, L, L );
      mol->tubes=(tubes&&!drawADP)?1:0;
      mol->intern=0;
      mol->adp=(drawADP)?1:0;
      mol->dratom=2;
      mol->atoms(mol->duplicateAtoms);

      qglColor(QColor("springgreen"));
      glDisable(GL_DEPTH_TEST);
      if (stereo_mode!=1)
	for (int ii=0; ii<mol->duplicateAtoms.size();ii++)renderText(
			mol->duplicateAtoms.at(ii).pos.x,
			mol->duplicateAtoms.at(ii).pos.y,
			mol->duplicateAtoms.at(ii).pos.z,
			"="+mol->duplicateAtoms.at(ii).Label+"=",
			myFont);

      qglColor(labelColor);
    }glPopMatrix();/*
		      glPushMatrix();{
		      glScaled( L, L, L );
		      mol->bonds(mol->showbonds);
		      }glPopMatrix();*/
    mol->dratom=0;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    mol->LOD=alod;
  }
  if ((imFokus>-1)&&(!drawLa)&&(imFokus<mol->showatoms.size()))
    if (!warLabel){
      CEnvironment fokat;
      fokat.append(mol->showatoms.at(imFokus));
      glPushMatrix();{
	glScaled( L, L, L );
	mol->tubes=0;
	mol->intern=1;
	mol->adp=0;
	mol->dratom=3;
	mol->atoms(fokat);
	qglColor(labelColor);
	mol->dratom=0;
	glDisable(GL_DEPTH_TEST);
	if (stereo_mode!=1)renderText( 
			fokat.at(0).pos.x,
			fokat.at(0).pos.y,
			fokat.at(0).pos.z, 
			fokat.at(0).Label,
			myFont);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_DEPTH_TEST);
      }glPopMatrix();

    } 
  if (enviPositions.size()){
  glPushMatrix();
  glDisable( GL_DEPTH_TEST );
  glEnable(GL_LINE_STIPPLE);
  glEnable(GL_BLEND);
  glLineWidth(2);
  glDisable(GL_LIGHTING);
  glLineStipple(3,21845);

  glScaled( L, L, L );
  glBegin(GL_LINES);
  for (int k=0;k<enviPositions.size();k++){
      if ((enviNoQ->isChecked())&&(labs.at(k).startsWith('Q',Qt::CaseInsensitive))) continue;
      switch(enviKat.at(k)){
      case 1: qglColor(mol->enviBondColor);break;
      case 2: qglColor(mol->enviHBColor);break;
      default:
         qglColor(mol->enviDefaultColor);
      }
      //if ((Distance(enviP0,enviPositions.at(k)))<envirange*envirange)
      {
  glVertex3d(enviP0.x,enviP0.y,enviP0.z);
  glVertex3d(enviPositions.at(k).x,enviPositions.at(k).y,enviPositions.at(k).z);
      }
  }
  glEnd();
  for (int k=0;k<enviPositions.size();k++){
    if ((enviNoQ->isChecked())&&(labs.at(k).startsWith('Q',Qt::CaseInsensitive))) continue;
    switch(enviKat.at(k)){
        case 1: qglColor(mol->enviBondColor);break;
        case 2: qglColor(mol->enviHBColor);break;
	    default:
           qglColor(mol->enviDefaultColor);
    }
    renderText((enviPositions.at(k).x+enviP0.x)/2.0,
		    (enviPositions.at(k).y+enviP0.y)/2.0,
		    (enviPositions.at(k).z+enviP0.z)/2.0,
		    QString::number(sqrt(Distance(enviP0,enviPositions.at(k))),'f',3)+"Ang",nonAtomFont);
    renderText(enviPositions.at(k).x,
		    enviPositions.at(k).y,
		    enviPositions.at(k).z,
		    labs.at(k),nonAtomFont);
  }
  glDisable(GL_LINE_STIPPLE);
  glEnable(GL_LIGHTING);
  glEnable( GL_DEPTH_TEST );
  glPopMatrix();
  }

  if (bas&&drawUc) glCallList(bas+4); //unit cell
  if ((drawLa)&&(!moving->isActive())&&drawUc) {
    glClear( GL_DEPTH_BUFFER_BIT);
    V3 uz0f,uz1f,uz2f,uz3f;
    V3 uz0k,uz1k,uz2k,uz3k;
    uz0f.x=0.0;  uz0f.y=0.0;  uz0f.z=0.0;
    uz1f.x=1.0;  uz1f.y=0.0;  uz1f.z=0.0;
    uz2f.x=0.0;  uz2f.y=1.0;  uz2f.z=0.0;
    uz3f.x=0.0;  uz3f.y=0.0;  uz3f.z=1.0;
    mol->frac2kart(uz0f,uz0k);
    mol->frac2kart(uz1f,uz1k);
    mol->frac2kart(uz2f,uz2k);
    mol->frac2kart(uz3f,uz3k);
    glPushMatrix();{
      glScaled(L,L,L);{
	qglColor(labelColor);

	renderText(uz0k.x,uz0k.y,uz0k.z,"0",myFont);
	glColor4f(1.0f,0.0f,0.0f,1.0);
	renderText(uz1k.x,uz1k.y,uz1k.z,"a",myFont);

	glColor4f(0.0f,1.0f,0.0f,1.0);
	renderText(uz2k.x,uz2k.y,uz2k.z,"b",myFont);

	glColor4f(0.0f,0.0f,1.0f,1.0);
	renderText(uz3k.x,uz3k.y,uz3k.z,"c",myFont);
      }glPopMatrix();
    }
  }
  if ((drawLa)&&(!moving->isActive())) {
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glClear( GL_DEPTH_BUFFER_BIT);
    glPushMatrix();{
      glScaled(L,L,L);
      GLdouble model[16];
      GLdouble proj[16];
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
//      printf("%d %d %d %d\n",viewport[0],viewport[1],viewport[2],viewport[3]);
      glGetDoublev( GL_PROJECTION_MATRIX , (double*)proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, (double*)model );
      {
        for (int j=0;j<mol->showatoms.size();j++){
          if (mol->showatoms.at(j).hidden) continue;
          if ((mol->nopm1)&&(mol->showatoms.at(j).symmGroup)&&(mol->showatoms.at(j).part<-0)) continue;
	  if (imFokus==j) qglColor(QColor("#FFAA00")); else {
        if ((mol->highlightEquivalents)&&(mol->showatoms.at(j).symmGroup)) {
	      if ((backGroundColor.saturation()>128)&&(abs(backGroundColor.hue()-240)<30)) qglColor(Qt::lightGray);
	      else {
		if (backGroundColor.value()<128) qglColor(QColor(Qt::blue).lighter(150));
		else qglColor(Qt::darkBlue);
	      }
	    }
	    else qglColor(labelColor);
	  }
	  double pla1=0,pla2=0;
	  //{1.0,0.0,-1.0,-190},
	  GLdouble in[4], out[4];

	  in[0] = mol->showatoms.at(j).pos.x;
	  in[1] = mol->showatoms.at(j).pos.y;
	  in[2] = mol->showatoms.at(j).pos.z;
	  in[3] = 1.0;
	  transform_point(out, model, in);
	  pla1=(out[0]*clip_plane1[0]+out[1]*clip_plane1[1]+out[2]*clip_plane1[2]+out[3]*clip_plane1[3]);
	  pla2=(out[0]*clip_plane2[0]+out[1]*clip_plane2[1]+out[2]*clip_plane2[2]+out[3]*clip_plane2[3]);
          posTo2D(mol->showatoms.at(j).pos,model,proj,viewport, &mol->showatoms[j].screenX, &mol->showatoms[j].screenY);
          int xminus = fmtr.boundingRect(mol->showatoms.at(j).Label).width();
          int x2minus = fmtr2.boundingRect(mol->showatoms.at(j).Label).width();
          int yminus = fmtr.boundingRect(mol->showatoms.at(j).Label).height();
          int y2minus = fmtr2.boundingRect(mol->showatoms.at(j).Label).height();
          if ((xray_eye)&&(pla1<0)&&(pla2<0))continue;
          int displace=10;
          int displace2=displace/2;
          /*if (j==0) {
            printf("XY %d %d %g %g \n",xminus,yminus,
                (mol->showatoms[j].screenX<ww/2)?
                mol->showatoms[j].screenX-(xminus+displace):
                mol->showatoms[j].screenX+displace,
                (mol->showatoms[j].screenY<wh/2)?
                mol->showatoms[j].screenY-displace2:
                mol->showatoms[j].screenY+yminus);
          }*/
          if (exporting){
            double mult= (double)ww/width();
            displace*=mult;
            displace2*=mult;
            labz ll;
            ll.x=(mol->showatoms[j].screenX<ww/2)?
              mol->showatoms[j].screenX-(xminus+displace):
              mol->showatoms[j].screenX+displace;
            ll.y=(mol->showatoms[j].screenY<wh/2)?
              mol->showatoms[j].screenY-displace2:
              mol->showatoms[j].screenY+yminus;
            ll.l=mol->showatoms.at(j).Label;
            ll.an=mol->showatoms.at(j).an;
            LabelZ.append(ll);
          }
          else{
          if (imFokus==j) {
            renderText( 
                (mol->showatoms[j].screenX<ww/2)?
                mol->showatoms[j].screenX-(xminus+displace):
                mol->showatoms[j].screenX+displace,
                (mol->showatoms[j].screenY<wh/2)?
                mol->showatoms[j].screenY-displace2:
                mol->showatoms[j].screenY+yminus,
                mol->showatoms.at(j).Label,
                //nonAtomFont);
              myFont);
            continue;

          }else {
            if ((mol->showatoms.at(j).an<0)) renderText(//(mol->selectedatoms.isEmpty())&&
                (mol->showatoms[j].screenX<ww/2)?
                mol->showatoms[j].screenX-(x2minus+displace):
                mol->showatoms[j].screenX+displace,
                (mol->showatoms[j].screenY<wh/2)?
                mol->showatoms[j].screenY-displace2:
                mol->showatoms[j].screenY+y2minus,
                mol->showatoms.at(j).Label,
                nonAtomFont);
            else
              if ((!(mol->AtomStyle[mol->showatoms.at(j).an]&ATOM_STYLE_NOLABEL)))//(mol->selectedatoms.isEmpty())&&
                renderText(
                    (mol->showatoms[j].screenX<ww/2)?
                    mol->showatoms[j].screenX-(((mol->selectedatoms.isEmpty())?xminus:x2minus)+displace):
                    mol->showatoms[j].screenX+displace,
                    (mol->showatoms[j].screenY<wh/2)?
                    mol->showatoms[j].screenY-displace2:
                    mol->showatoms[j].screenY+((mol->selectedatoms.isEmpty())?yminus:y2minus),
                    mol->showatoms.at(j).Label,
                    (mol->selectedatoms.isEmpty())?myFont:nonAtomFont);
          }
          }
        }
      }
      if (!exporting)for (int j=0;j<mol->selectedatoms.size();j++){
          int xminus = fmtr.boundingRect(mol->selectedatoms.at(j).Label).width();
          int yminus = fmtr.boundingRect(mol->selectedatoms.at(j).Label).height();
          int displace=10;
          int displace2=displace/2;
	  GLdouble in[4], out[4];
	  in[0] = mol->selectedatoms.at(j).pos.x;
	  in[1] = mol->selectedatoms.at(j).pos.y;
	  in[2] = mol->selectedatoms.at(j).pos.z;
	  in[3] = 1.0;
	  transform_point(out, model, in);
          posTo2D(mol->selectedatoms.at(j).pos,model,proj,viewport, &mol->selectedatoms[j].screenX, &mol->selectedatoms[j].screenY);
//          int x2minus = fmtr2.boundingRect(mol->selectedatoms.at(j).Label).width();
//          int y2minus = fmtr2.boundingRect(mol->selectedatoms.at(j).Label).height();
          qglColor(Qt::yellow);

          renderText(
                   (mol->selectedatoms[j].screenX<ww/2)?
                    mol->selectedatoms[j].screenX-((xminus)+displace):
                    mol->selectedatoms[j].screenX+displace,
                   (mol->selectedatoms[j].screenY<wh/2)?
                    mol->selectedatoms[j].screenY-displace2:
                    mol->selectedatoms[j].screenY+(yminus),
                    mol->selectedatoms.at(j).Label,
                   myFont);
/*              (mol->selectedatoms[j].screenX<ww/2)?mol->selectedatoms[j].screenX-(xminus+15):mol->selectedatoms[j].screenX+15,
              (mol->showatoms[j].screenY<wh/2)?
              mol->showatoms[j].screenY-7:
              mol->showatoms[j].screenY+yminus,
              mol->selectedatoms.at(j).Label,
              myFont);*/
      }
   /* renderText(100,100,"null",nonAtomFont);
    renderText(width()/2,100,"oben",nonAtomFont);
    renderText(width()/2,height()/2,"mitte",nonAtomFont);// */
    }glPopMatrix();
  }  
  {
    glPushMatrix();
    glScaled(L,L,L);{

      GLdouble model[16];
      GLdouble proj[16];
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      glGetDoublev( GL_PROJECTION_MATRIX , (double*)proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, (double*)model );
      for (int j=0;j<mol->showatoms.size();j++){
	{mol->showatoms[j].screenX=-200; mol->showatoms[j].screenY=-200;}
	if (mol->showatoms.at(j).hidden) continue;
	double pla1=0,pla2=0;
	//{1.0,0.0,-1.0,-190},
	GLdouble in[4], out[4];

	in[0] = mol->showatoms.at(j).pos.x;
	in[1] = mol->showatoms.at(j).pos.y;
	in[2] = mol->showatoms.at(j).pos.z;
	in[3] = 1.0;
	transform_point(out, model, in);
	pla1=(out[0]*clip_plane1[0]+out[1]*clip_plane1[1]+out[2]*clip_plane1[2]+out[3]*clip_plane1[3]);
	pla2=(out[0]*clip_plane2[0]+out[1]*clip_plane2[1]+out[2]*clip_plane2[2]+out[3]*clip_plane2[3]);
	if ((xray_eye)&&(pla1<0)&&(pla2<0))continue;

	if ((mol->nopm1)&&(mol->showatoms.at(j).symmGroup)&&(mol->showatoms.at(j).part<-0)) continue;
	if (!posTo2D(mol->showatoms.at(j).pos,model,proj,viewport, &mol->showatoms[j].screenX, &mol->showatoms[j].screenY))
	{mol->showatoms[j].screenX=-200; mol->showatoms[j].screenY=-200;}
      }
    }
    glPopMatrix();
  }

  glPopMatrix();  
  afok=imFokus;
}
