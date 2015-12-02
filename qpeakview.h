
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
#ifndef QPEAKVIEW_H
#define QPEAKVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE
/*! \brief QPeakView is a widget featuring a color gradient and the corresponding electron density values (q-peak height)
 *  it is used to highlight or hide q-peaks in the openGL repesentation of the structure.
 *
 *  
 *
 *
 *
 *
 */

class QPeakView : public QWidget
{
    Q_OBJECT
public:
    explicit QPeakView(QWidget *parent = 0);
    QSize sizeHint() const {
      return QSize(35,0);
    }
    double cutoff;//!< Q-Peaks with lower electron desitie values than this cutoff will be hidden. 
    void setMax(double m){
        max=qMax(m,min+0.001);
    }//!< sets the maximum electron desity value of list of Q-Peaks.
    void setMin(double m){
        min=qMin(m,max-0.001);
    }//!< sets the minimum electron desity value of list of Q-Peaks.
signals:
    void cutOffChanged(double coff);//!< This signal is emitted when the user clicks on the widget or scrolls on it with the mouse wheel
    void findQP(double co);//!< This signal is emitted when the user hovers the mouse over the witget

public slots:
    void qpeaksInFocus(double height);//!< when the user hovers over a Q-Peak in the OpenGL representation of the structure the corresponding area in the widget gets highlighted.  
    void unhide();//!< when the widget is visible again the cutoff value is reset to zero. 

protected:
  void paintEvent(QPaintEvent *event);
  void	mousePressEvent ( QMouseEvent * event );
  void	mouseMoveEvent ( QMouseEvent * event );
  void	wheelEvent ( QWheelEvent * event );
private:
  void cutOffChange(double coff);
  QColor farbverlauf(double wrt);
  double min,max,oco;
  double focusHeight;
  int fohi;//font height
};

#endif // QPEAKVIEW_H
