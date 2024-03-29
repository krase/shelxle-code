
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
#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QMap>
#include "itsme.h"
/*! \brief HistoryWidget is a widget which shows previous versions as bars with a height and color corresoponding to the R-value 
 *  of the previous refinement. 
 *
 *  R-Values are read from the file and the remark there after HKLF statement. If this remarks are missing no bar shows up.
 *  Clicking on a bar loads that previous file version. Hovering a bar gives some mor details about the refinement. 
 *  A short preview of the file version can be displayed.  
 *
 */
class HistoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit HistoryWidget(QWidget *parent = 0);
    QList<double> RWerte;//!< List of R1 values
    QMap<int,double> FreeWerte;//!< List of R1Free values
    QList<double> RallWerte;//!< List of Rall values
    QList<int> gooddata;//!< Number of reflections above a sigma cutoff
    QList<int> alldata;//!< Totol number of reflections
    QList<int> parameters;//!< number of refined parameters
    QStringList saveFilesList;//!< File name list of previous versions 
    QList<QDateTime> dateTimesList;//!< List of file dates
    void setPath(QString dirName, int width);//!< sets the path to the resfile and the history sub directory.
signals:
    void message(const QString &);//!< emitts a text to the status bar
    void lofi(const QString &);//!< load a file version 
public slots:
void preview();//!< previewv of the first 30 lines
void preview2();//!< preview the next 30 lines
void prune();//!< delete older file versions and keep only the newest ten
void deleteOne();//!< delete a individual file version from the list
protected:
  void paintEvent(QPaintEvent *event);
  bool event(QEvent *event);
  void mousePressEvent ( QMouseEvent * event );
  void mouseMoveEvent ( QMouseEvent * event );
//  void wheelEvent ( QWheelEvent * event );
private:
  size_t gesamtGr;
  QColor farbverlauf(double wrt);
  int maxwid;
  QString dirname;
};

#endif // HISTORYWIDGET_H
