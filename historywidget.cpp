
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
#include "historywidget.h"
#include <QtGui>
//#define PROGRAM_NAME "shelXle"
HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent){
    setMinimumSize(100,100);
    setMouseTracking(true);
}

void HistoryWidget::mouseMoveEvent(QMouseEvent *event){
    if (event->pos().y()<6) {
        emit message("");
        return;
    }
    if  (RWerte.isEmpty()) return;
    int w = width() / RWerte.size();
    int index = qMin(event->pos().x() / w, saveFilesList.size()-1);
    if ((index>-1)&&(index<saveFilesList.size()))
    emit message(QString("%1 (R1=%2)").arg(saveFilesList.at(index)).arg(RWerte.at(index)));
}

void HistoryWidget::preview(){
    QAction *action = qobject_cast<QAction *>(sender());
    QString fileName = "";
    if (action)
      fileName = action->data().toString();
    else return;
    if (fileName.isEmpty()) return;
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QString all = f.readAll();
    all = all.section('\n',0,30);
    //all = all.replace("\n","<br>\n");
    f.close();
    //QWhatsThis *wtf = new QWhatsThis();
    QWhatsThis::showText(QPoint(75,0),//<style type=\"text/css\">{ font-size:0.7em; white-space:pre; }</style>
                         QString("<h2>%2</h2><font color=red>--click here to close--</font><font size=-1><pre><code>%1</code></pre></font><center></center>")
                         .arg(all)
                         .arg(fileName));
    all.clear();
    return;

}

void HistoryWidget::preview2(){
    QAction *action = qobject_cast<QAction *>(sender());
    QString fileName = "";
    if (action)
      fileName = action->data().toString();
    else return;
    if (fileName.isEmpty()) return;
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QString all = f.readAll();
    all = all.section('\n',30);
    //all = all.replace("\n","<br>\n");
    f.close();
    //QWhatsThis *wtf = new QWhatsThis();
    QWhatsThis::showText(QPoint(75,0),//<style type=\"text/css\">{ font-size:0.7em; white-space:pre; }</style>
                         QString("<h2>%2</h2><font color=red>--click here to close--</font><font size=-1><pre><code>...\n%1</code></pre></font><center></center>")
                         .arg(all)
                         .arg(fileName)			 
			 );
    all.clear();
    return;

}

void HistoryWidget::prune(){
  QString fcfname;
  for (int i=0; (i+10)<saveFilesList.size(); i++){
    QFile::remove(saveFilesList.at(i));
    fcfname=saveFilesList.at(i);
    fcfname.replace(fcfname.size()-3,3,"fcf");
    QFile::remove(fcfname);
  }
  setPath(dirname,maxwid);
}

void HistoryWidget::deleteOne(){
    QAction *action = qobject_cast<QAction *>(sender());
    QString fileName = "";
    if (action)
      fileName = action->data().toString();
    else return;
    if (fileName.isEmpty()) return;
    QString fcfname=fileName;
    fcfname.replace(fcfname.size()-3,3,"fcf");
    if (QMessageBox::Yes==QMessageBox::question ( this, "Delete hisory entry", 
			    QString("Do you really whant to delete history files '<b>%1</b>' and '<b>%2</b>'?").
			    arg(fileName).
			    arg(fcfname), 
			    QMessageBox::Yes|QMessageBox::No, QMessageBox::NoButton )){
    QFile::remove(fileName);
    QFile::remove(fcfname);
    }
    setPath(dirname,maxwid);
}


void HistoryWidget::mousePressEvent ( QMouseEvent * event ){
  if  (RWerte.isEmpty()) return;
  int w = width() / RWerte.size();
  int index = qMin(event->pos().x() / w, saveFilesList.size()-1);//event->pos().x() / w;
  //copy ins to last programnameSaves
  //copy res to ins
  //copy savefilelist.at(index) to res and open this res file
  QString dn=dirname.section('/',0,-2),
	  argument=dirname.section('/',-1);
  argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
  if (event->buttons() & Qt::RightButton){
      QMenu *menu = new QMenu("");
      QString shortname=saveFilesList.at(index).section('/',-1);
      QAction *a=menu->addAction(QString("Preview %1 (first 30 lines)").arg(shortname),this,SLOT(preview()));
      a->setData(saveFilesList.at(index));
      a=menu->addAction(QString("Preview %1 (next 30 lines)").arg(shortname),this,SLOT(preview2()));
      a->setData(saveFilesList.at(index));
      a=menu->addAction(QString("Purge history. Keep newest ten entries."),this,SLOT(prune()));
      a=menu->addAction(QString("Delete %1 !").arg(shortname),this,SLOT(deleteOne()));
      a->setData(saveFilesList.at(index));
      menu->exec(event->globalPos());
      delete menu;
  }
  if (event->buttons() & Qt::LeftButton){
    if (dirname.contains(QRegExp(".res$",Qt::CaseInsensitive))){
      QString insname =dirname;
      insname.replace(QRegExp(".res$",Qt::CaseInsensitive),".ins");
      QString fcfname = dirname;
      fcfname.replace(QRegExp(".res$",Qt::CaseInsensitive),".fcf");
      QString fcfname2 = dirname;
      fcfname2.replace(QRegExp(".res$",Qt::CaseInsensitive),".2fcf");
      QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
      QStringList filter;
      filter << QString("%1_*_.ins").arg(argument);
      QStringList resfs;
      resfs=work.entryList(filter,QDir::Files,QDir::Time|QDir::Reversed);
      int vers=0;
      if (resfs.isEmpty()) vers=1;
      else {
	bool ok=true;
	for (int i = 0; i < resfs.size(); i++)
	  vers = qMax(resfs.at(i).section("_",-2,-2).toInt(&ok,36),vers);
	vers++;
      }
      if (!work.exists ( QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME))) work.mkdir (QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
      if (saveFilesList.at(index)==insname){
	QFile::copy(dirname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(dirname);
	QFile::copy(saveFilesList.at(index),dirname);
//	QFile::remove(saveFilesList.at(index));

        QFile::copy(fcfname,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
        QFile::remove(fcfname);
        QFile::copy(fcfname2,fcfname);
        //printf("copy %s to %s\n",fcfname.toStdString().c_str()
        //		,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME).toStdString().c_str());
        //printf("copy %s to %s\n",fcfname2.toStdString().c_str(), fcfname.toStdString().c_str());

      }else{
	QFile::copy(insname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(insname);
	QFile::copy(dirname,insname);
	if (saveFilesList.at(index) != dirname) {
	  QFile::remove(dirname);
	}
        QFile::copy(fcfname2,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
        QFile::remove(fcfname2);
        QFile::copy(fcfname,fcfname2);
        QFile::remove(fcfname);
        QString sfcf=saveFilesList.at(index);
        sfcf = sfcf.replace(sfcf.size()-3,3,"fcf");
        QFile::copy(sfcf,fcfname);

	QFile::copy(saveFilesList.at(index),dirname);
	//QFile::remove(saveFilesList.at(index));
        //printf("copy %s to %s\n",fcfname.toStdString().c_str()
        //		,fcfname2.toStdString().c_str());
        //printf("copy %s to %s\n",fcfname2.toStdString().c_str()
        //		,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME).toStdString().c_str());
        //printf("copy %s to %s\n",sfcf.toStdString().c_str(), fcfname.toStdString().c_str());
      }
      //QTest::qWait(550);
/*		 .arg(insname)
		 .arg(QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME))
		 .arg(dirname)
		 .arg(saveFilesList.at(index));*/
    }else{
      QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
      QStringList filter;
      filter << QString("%1_*_.ins").arg(argument);
      QStringList resfs;
      resfs=work.entryList(filter,QDir::Files,QDir::Time|QDir::Reversed);
      int vers=0;
      if (resfs.isEmpty()) vers=1;
      else {
	bool ok=true;
	for (int i = 0; i < resfs.size(); i++)
	  vers = qMax(resfs.at(i).section("_",-2,-2).toInt(&ok,36),vers);

	vers++;
      }
      if (!work.exists ( QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME))) work.mkdir (QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
      QFile::copy(dirname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
      dirname.chop(3);
      dirname.append("res");
      QFile::copy(saveFilesList.at(index),dirname);
    }
    emit lofi(dirname);
  }
  update();
}

void HistoryWidget::setPath(QString dirName,int wid){
  maxwid = wid-5;
  dirname=dirName;
  QString dn = dirName.section('/',0,-2),
	  argument=dirName.section('/',-1);
  argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
  QString insname =dirName;
  insname.replace(QRegExp(".res$",Qt::CaseInsensitive),".ins");

  QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
  QStringList filter;
  filter << QString("%1_*_.ins").arg(argument);
  QStringList resfs;
  resfs=work.entryList(filter,QDir::Files,QDir::Time|QDir::Reversed);
  filter.clear();
  filter << QString("%1_*_.*").arg(argument);
  QFileInfoList resfil = work.entryInfoList(filter,QDir::Files,QDir::Time|QDir::Reversed);
  gesamtGr=0;
  for (int k=0; k< resfil.size(); k++){
      gesamtGr += resfil.at(k).size();
  }
  resfil.clear();
  int vers=0;
  if (resfs.isEmpty()) vers=0;
  else {
    bool ok=true;
    for (int i = 0; i < resfs.size(); i++)
      vers = qMax(resfs.at(i).section("_",-2,-2).toInt(&ok,36),vers);

    // vers++;
  }
  FreeWerte.clear();
  RWerte.clear();
  RallWerte.clear();
  gooddata.clear();
  alldata.clear();
  parameters.clear();
  saveFilesList.clear();
  dateTimesList.clear();
  for (int i=0; i<resfs.size();i++){
    QFile alt(QString("%1/%2").arg(work.absolutePath()).arg(resfs.at(i)));
    alt.open(QIODevice::ReadOnly|QIODevice::Text);
    QString inhalt=alt.readAll();
    alt.close();
    inhalt = inhalt.section("\nHKLF ",1);


    //REM R1 =  0.0150 for   6927 Fo > 4sig(Fo)  and  0.0160 for all   7220 data
    QStringList sl = inhalt.section("REM R1 =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
    if (sl.size()<11) continue;
    RWerte.append(sl.at(0).toDouble()*100.0);
    gooddata.append(sl.at(2).toInt());
    RallWerte.append(sl.at(7).toDouble()*100.0);
    alldata.append(sl.at(10).toInt());
    //R1(Free) =
    QStringList slfree = inhalt.section("REM R1(Free) =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
    if (slfree.size()>1){
        FreeWerte[RWerte.size()-1]=slfree.at(0).toDouble()*100;
    }
    sl = inhalt.section("REM R1 =",1).section("\n",1,1).trimmed().split(" ",QString::SkipEmptyParts);
    if (sl.size()<2) continue;
    parameters.append(sl.at(1).toInt());
    saveFilesList.append(alt.fileName());
    dateTimesList.append(QFileInfo(alt.fileName()).lastModified());

  }

  {
    QFile alt(insname);
    if (alt.open(QIODevice::ReadOnly)){
      QString inhalt=alt.readAll();

      alt.close();

      inhalt = inhalt.section("\nHKLF ",1);
      //REM R1 =  0.0150 for   6927 Fo > 4sig(Fo)  and  0.0160 for all   7220 data
      QStringList sl = inhalt.section("REM R1 =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
      if (sl.size()<11) return;
      RWerte.append(sl.at(0).toDouble()*100.0);
      gooddata.append(sl.at(2).toInt());
      RallWerte.append(sl.at(7).toDouble()*100.0);
      alldata.append(sl.at(10).toInt());
      //R1(Free) =
      QStringList slfree = inhalt.section("REM R1(Free) =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
      if (slfree.size()>1){
      FreeWerte[RWerte.size()-1]=slfree.at(0).toDouble()*100;
      }


      sl = inhalt.section("REM R1 =",1).section("\n",1,1).trimmed().split(" ",QString::SkipEmptyParts);
      if (sl.size()<2) return;
      parameters.append(sl.at(1).toInt());
      saveFilesList.append(alt.fileName());
      dateTimesList.append(QFileInfo(alt.fileName()).lastModified());
    }}
    {
      QFile alt(dirName);
      if (alt.open(QIODevice::ReadOnly)){
	QString inhalt=alt.readAll();
	alt.close();

        inhalt = inhalt.section("\nHKLF ",1);
	//REM R1 =  0.0150 for   6927 Fo > 4sig(Fo)  and  0.0160 for all   7220 data

    QStringList sl = inhalt.section("REM R1 =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
	if (sl.size()<11) return;
	RWerte.append(sl.at(0).toDouble()*100.0);
	gooddata.append(sl.at(2).toInt());
	RallWerte.append(sl.at(7).toDouble()*100.0);
        alldata.append(sl.at(10).toInt());
        //R1(Free) =
        QStringList slfree = inhalt.section("REM R1(Free) =",1).section("\n",0,0).trimmed().split(" ",QString::SkipEmptyParts);
        if (slfree.size()>1){
        FreeWerte[RWerte.size()-1]=slfree.at(0).toDouble()*100;
        }

        sl = inhalt.section("REM R1 =",1).section("\n",1,1).trimmed().split(" ",QString::SkipEmptyParts);
	if (sl.size()<2) return;
	parameters.append(sl.at(1).toInt());
	saveFilesList.append(alt.fileName());
	dateTimesList.append(QFileInfo(alt.fileName()).lastModified());
      }}


      update();
}

QColor HistoryWidget::farbverlauf(double wrt){
  int rot,gruen,blau,alpha;
  int lauf=0;
  const float farbe[6][4]={
    {0.0,0.6,0.0,1.0},
    {0.0,0.6,1.0,1.0},
    {0.0,0.0,1.0,1.0},
    {0.7,0.6,0.0,1.0},
    {0.7,0.0,1.0,1.0},
    {0.7,0.0,0.0,1.0}};
  float nwrt=(wrt)/(100);
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

void HistoryWidget::paintEvent(QPaintEvent *event){
  double x =0;
  for (int i=0; i<RWerte.size(); i++){
    x = qMax(RWerte.at(i),x);
    if (FreeWerte.contains(i))
        x = qMax(FreeWerte.value(i),x);
  }
  QPainter p(this);
  setMinimumWidth(qMax(p.fontMetrics().width(QString("%1").arg(x,4,'f',3))*RWerte.size(),maxwid));
  setMaximumWidth(qMax(p.fontMetrics().width(QString("%1").arg(x,4,'f',3))*RWerte.size(),maxwid));
  p.fillRect(0,0,width(),height(),QPalette().window());// );QColor("#eeeecc")
  p.setPen(Qt::black);

  if  (RWerte.isEmpty()) return;
  int h = height(), w = width() / RWerte.size(),fh=p.fontMetrics().height();
  p.drawText(0,0,width(),fh,Qt::AlignLeft,QString("Total size of history on disk: %1%2")
             .arg((gesamtGr/(1024*1024))?(double)gesamtGr/(1024*1024):(double)gesamtGr/1024,7,'f',1)
             .arg((gesamtGr/(1024*1024))?"MB":"kB"));
  for (int i = 0; i < RWerte.size(); i++){
/*    if (saveFilesList.at(i)==dirname){
    p.setBrush(QBrush(QColor("#ffddaa")));
    int W=qMax(p.fontMetrics().width("open save"),w);
    p.drawRoundedRect(i*w-((W-w+2)),0,W,2*fh+2,5.0,5.0);
    QPoint drei[3];
    drei[0]= QPoint((i*w+w/3),(2*fh+2));
    drei[1]= QPoint((i*w+2*w/3),(2*fh+2));
    drei[2]= QPoint((i*w+w/2),qMax((int)(h-(RWerte.at(i)/25*h))-1,0)-fh);
    p.drawPolygon(drei,3);
    p.setPen(Qt::black);
    p.drawText(i*w-((W-w+2)),0,W, fh,Qt::AlignCenter,QString("open save"));
    p.drawText(i*w-((W-w+2)),fh+2,W, fh,Qt::AlignCenter,QString("history"));
    }*/
      if (FreeWerte.contains(i)){
      p.setBrush(QBrush(farbverlauf(FreeWerte.value(i)*3)));
      p.drawRect(i*w+5,qMax((int)(h-(FreeWerte.value(i)/25*h))-1,0),w*0.95-5,qMin((int)(FreeWerte.value(i)/25*h),h-2));
    }
    p.setBrush(QBrush(farbverlauf(RWerte.at(i)*3)));
    p.drawRect(i*w,qMax((int)(h-(RWerte.at(i)/25*h))-1,0),w*0.9,qMin((int)(RWerte.at(i)/25*h),h-2));
      if (FreeWerte.contains(i)){
          p.drawText(i*w, qMax((int)(h-(FreeWerte.value(i)/25*h)),fh+5)-fh-2, w,fh,Qt::AlignLeft, QString("%1")
                  .arg(RWerte.at(i),4,'f',2));

          p.drawText(i*w, qMax((int)(h-((FreeWerte.value(i))/25*h))-fh,fh+5)-fh-2, w,fh,Qt::AlignLeft, QString("%1")
                  .arg(FreeWerte.value(i),4,'f',2));
      }else
    p.drawText(i*w, qMax((int)(h-(RWerte.at(i)/25*h)),fh+5)-fh-2, w,fh,Qt::AlignLeft, QString("%1")
		    .arg(RWerte.at(i),4,'f',2));
  }
  event->accept();
  //p.drawText(0,0,p.fontMetrics().width("1234567890"),p.fontMetrics().height(),Qt::AlignLeft,QString("%1 %2").arg(h).arg(w));
}

bool HistoryWidget::event(QEvent *event){
  if (!isVisible()) {event->ignore(); return false;}
  if (event->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
    QPoint da = helpEvent->pos();
    if  (RWerte.isEmpty()) return true;
    int w = width() / RWerte.size();
    int index = da.x() / w;
    QString fcfname;
    if ((index<0)||(index>=saveFilesList.size())) return true;
    else fcfname=saveFilesList.at(index);
    fcfname.replace(fcfname.size()-3,3,"fcf");
    bool fcfda=QFile::exists(fcfname);
    if (FreeWerte.contains(index)){
        QToolTip::showText(helpEvent->globalPos(),
        QString("<b>%4:</b><br> R1(Free) = <b>%9</b><br>R1 = <b>%1</b><br>R1all = <b>%2</b><br>Reflection/Parameter = <b>%3</b><br>Reflection = <b>%6</b><br>Parameter = <b>%7</b><br>FCF-File present: <b>%8</b><br>Last modified: <b>%5</b>")
                        .arg(RWerte.at(index))
                        .arg(RallWerte.at(index))
                        .arg(((index<gooddata.size())&&(index<parameters.size()))  ?(double) gooddata.at(index) / parameters.at(index):0)
                        .arg((index<saveFilesList.size())?saveFilesList.at(index):0)
                        .arg((index<dateTimesList.size())?dateTimesList.at(index).toString("d MMM yyyy - hh:mm:ss"):"??")
                        .arg((index<gooddata.size())?gooddata.at(index):0)
                        .arg((index<parameters.size())?parameters.at(index):0)
                        .arg((fcfda)?"Yes":"No")
                        .arg(FreeWerte.value(index)));

    }else
    QToolTip::showText(helpEvent->globalPos(),
    QString("<b>%4:</b><br>R1 = <b>%1</b><br>R1all = <b>%2</b><br>Reflection/Parameter = <b>%3</b><br>Reflection = <b>%6</b><br>Parameter = <b>%7</b><br>FCF-File present: <b>%8</b><br>Last modified: <b>%5</b>")
                    .arg(RWerte.at(index))
                    .arg(RallWerte.at(index))
                    .arg(((index<gooddata.size())&&(index<parameters.size()))  ?(double) gooddata.at(index) / parameters.at(index):0)
                    .arg((index<saveFilesList.size())?saveFilesList.at(index):0)
                    .arg((index<dateTimesList.size())?dateTimesList.at(index).toString("d MMM yyyy - hh:mm:ss"):"??")
                    .arg((index<gooddata.size())?gooddata.at(index):0)
                    .arg((index<parameters.size())?parameters.at(index):0)
		    .arg((fcfda)?"Yes":"No")
		    
		    );
    return true;
  }
  return QWidget::event(event);
}
