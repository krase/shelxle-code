
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
#ifndef FCVSFO_H
#define FCVSFO_H 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <QtGui>
#ifndef isnormal
int isnormal(double arg);
#define vc7hack 1
#endif

//! Omit is a struct to specify a refection to be omitted by the OMIT instruction
typedef struct {
int h,k,l;
} Omit;

//! MCell: see Cell in molecule.h for details ....
struct MCell {
  double a,b,c,al,be,ga;
  double phi,V,as,bs,cs,tau,cosga,cosal,cosbe,singa,tanga;
  double wave;
};

//! Refls is a struct type for reflections in the fcf file
typedef struct {
  int h,k,l;
  double F;
  double phase;
  double sigma;
  double stl;
  double w;
} Refls;

//! GraphSceene is a QGraphicsScene class to display reflections graphically in 2D. Used in FcVsFo. \inherit QGraphicsScene
class GraphSceene:public QGraphicsScene{
Q_OBJECT
public:
GraphSceene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
  QGraphicsRectItem *selEckt;
  QList<int> selected;
  QPointF selStart,selEnd;
//  GraphSceene();
signals:
  void meinstatus(int da);
  void listen();
protected:
  void mouseMoveEvent ( QGraphicsSceneMouseEvent  * mouseEvent );
  void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent ); 
private:
};

//! FcVsFo is a QDialog to analyse reflection data and to omit outliers by hand.
class FcVsFo: public QDialog {
Q_OBJECT
public:
FcVsFo(QString filename, double a,double b, double c, double al, double be, double ga, double lambda);
~FcVsFo();
public slots:
	void delayMe();
	void readFCF();
        void omitref();
	void st(int da);
	void zoomin();
	void zoomout();
	void zoomoff();
	void zoomfit();
	void selektiert();
	void graphChange(int type);
signals:
	void weg(const QString &);
private:
	QString fileName;
	int listType;
	QGraphicsView *view;
	GraphSceene *scene;
	QList<Refls> Fc,Fo;
	Omit omits;
	double foMax,foMin,fcMax,fcMin;
	QLabel *state,*state2;
	QTextBrowser *brw;
	QTimer *timer;
	MCell cell;	
	QGraphicsItem *diagonale;
	QGraphicsTextItem *txt,*txt2;
};
#endif
