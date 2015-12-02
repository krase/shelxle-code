
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
#include "qpeakview.h"
#include <QtGui>

QPeakView::QPeakView(QWidget *parent) :
    QWidget(parent){
    //Dieses Widget soll einen Farbverlauf mit Peak-Hoehen darstellen
    min=0;
    focusHeight=-1.0;
    max=80.0;
    oco=cutoff=0.0;
    fohi = fontMetrics().height();
    setMouseTracking(true);
    //setCursor(QCursor(Qt::SplitVCursor));
}

void QPeakView::unhide(){
  printf("unhide\n");
    oco=cutoff=0;//qMin(0.0,min);
    update();
}

void QPeakView::qpeaksInFocus(double height){
    focusHeight=-1.0;
    if (height<min) {update();return;}
    if (height>max) {update();return;}
    focusHeight=height;
    update();
}
void QPeakView::cutOffChange(double coff){
    if (oco!=cutoff) emit cutOffChanged(coff);
    oco=cutoff;
}

void QPeakView::paintEvent(QPaintEvent *event){

    QPainter p(this);
    p.fillRect(event->rect(), QColor("#eeeeee"));
    p.fillRect(0,0,20,height(),Qt::black);
    int h = height();
    int fh = p.fontMetrics().height();
    //fohi=fh;
    fh=qMax(0,fh);
    //printf("%d\n",fh);
    double v =  h / fh;
    double w = v-1,u;

    QBrush b;
    for (int i = 0; i < v; i++){
        u=(1.0-i/w)*max+(i/w)*min;
        if (QString::number(focusHeight,'f',2)==QString::number((1.0-i/w)*max+(i/w)*min,'f',2)){
            p.setPen(Qt::yellow);
            b.setColor(Qt::darkGray);
            p.fillRect(20,i*fh,50,fh,b);
        }
        else {
            p.setPen(Qt::black);
            if (u<0)p.setPen(Qt::red);
        }
        //printf("%f %f %f %f %f %d %d\n",v,w,u,min,max,h,fh);
        if (cutoff>0) {
            if (((u)<cutoff)&&(u>0)) b.setStyle(Qt::Dense3Pattern);
            else b.setStyle(Qt::SolidPattern);
        }
        else {
            if (((u)>cutoff)&&(u<0)) b.setStyle(Qt::Dense3Pattern);
            else b.setStyle(Qt::SolidPattern);
        }
        b.setColor(farbverlauf(u));
        if (cutoff>0)
            p.fillRect(0,i*fh,((u<cutoff)&&(u>0))?50:20,fh,b);
        else p.fillRect(0,i*fh,((u>cutoff)&&(u<0))?50:20,fh,b);

        if (cutoff>0) {if ((u>=cutoff)||(u<0)) p.drawText((u<0)?12:20,i*fh,40,fh,Qt::AlignLeft,QString::number(u,'f',2));}
        else {if ((u<cutoff)||(u>0)) p.drawText((u<0)?12:20,i*fh,40,fh,Qt::AlignLeft,QString::number(u,'f',2));}

    }
}

void QPeakView::mousePressEvent ( QMouseEvent * event ){

    if (event->buttons()==Qt::LeftButton){
        int h = height() - 2 * fohi;
        double v =  height() / fohi;
        double w = v-1,u;
        u = ((1.0 - (w-1) / w) * max + ( (w-1) / w ) * min)-min;
        cutoff=((double)(h - event->pos().y()) / h) * max +
               ((double)(event->pos().y()) / h ) * min + u;
        //cutoff=qMax(min,cutoff);
        //cutoff=qMin(max,cutoff);
        //printf("coff %f mx %f mn %f u %f \n",cutoff, max, min, u);
        cutOffChange(cutoff);
        update();
    }
}
void QPeakView::wheelEvent ( QWheelEvent * event ){

    double numSteps = event->delta() / 12000.0;
    if (event->modifiers()==Qt::NoModifier){
      double d = cutoff;
      cutoff=(d+numSteps>=(min-0.05))?d+numSteps:d;
      cutoff=qMin(cutoff,max+0.005);
      cutOffChange(cutoff);
      update();
    }
}

void QPeakView::mouseMoveEvent ( QMouseEvent * event ){
    //event->CursorChange();

    if (event->buttons()==Qt::NoButton){
        int h = height();
        double v =  height() / fohi;
        double w = v-1,u;
        u = ((1.0 - (w-1) / w) * max + ( (w-1) / w ) * min)-min;
        double co=((double)(h - event->pos().y()) / h) * max +
               ((double)(event->pos().y()) / h ) * min + u;
        emit findQP(co);

    }
    if (event->buttons()==Qt::LeftButton){
        int h = height() - 2 * fohi;
        double v =  height() / fohi;
        double w = v-1,u;
        u = ((1.0 - (w-1) / w) * max + ( (w-1) / w ) * min)-min;
        cutoff=((double)(h - event->pos().y()) / h) * max +
               ((double)(event->pos().y()) / h ) * min + u;
        //cutoff=qMax(min,cutoff);
        //cutoff=qMin(max,cutoff);
        //printf("coff %f mx %f mn %f u %f \n",cutoff, max, min, u);

        cutOffChange(cutoff);
        update();
    }
}

QColor QPeakView::farbverlauf(double wrt){
    if (min>=max) max=min+0.001;
    int rot,gruen,blau,alpha;
    int lauf=0;
    const float farbe[6][4]={{1.0,0.0,0.0,1.0},
                          {1.0,1.0,0.0,1.0},
                          {0.0,1.0,0.0,1.0},
                          {0.0,1.0,1.0,1.0},
                          {0.0,0.0,1.0,1.0},
                          {1.0,0.0,1.0,1.0}};
    float nwrt=(wrt-min)/(max-min);
    nwrt=(nwrt>=1.0)?0.99999:nwrt;
    nwrt=(nwrt<=0.0)?0.00001:nwrt;
    lauf=(int (nwrt/0.2));
    nwrt-=(0.2*lauf);
    nwrt/=(0.2);

    rot=(int)(255*((1.0-nwrt)*farbe[lauf][0]+farbe[lauf+1][0]*nwrt));
    gruen=(int)(255*((1.0-nwrt)*farbe[lauf][1]+farbe[lauf+1][1]*nwrt));
    blau=(int)(255*((1.0-nwrt)*farbe[lauf][2]+farbe[lauf+1][2]*nwrt));
    alpha=255;
    return QColor(rot,gruen,blau,alpha);
}
