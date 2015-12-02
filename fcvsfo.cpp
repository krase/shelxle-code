#include "fcvsfo.h"

FcVsFo::FcVsFo(QString filename,double a,double b, double c, double al, double be, double ga, double lambda){
  const double g2r=180.0/M_PI;
  cell.wave=lambda;
  cell.a=a;
  cell.b=b;
  cell.c=c;
  cell.al=al;
  cell.be=be;
  cell.ga=ga;
  cell.cosal = (cos(cell.be / g2r)*cos(cell.ga / g2r)-cos(cell.al / g2r))/(sin(cell.be / g2r)*sin(cell.ga / g2r));
  cell.cosbe = (cos(cell.al / g2r)*cos(cell.ga / g2r)-cos(cell.be / g2r))/(sin(cell.al / g2r)*sin(cell.ga / g2r));
  cell.cosga = (cos(cell.al / g2r)*cos(cell.be / g2r)-cos(cell.ga / g2r))/(sin(cell.be / g2r)*sin(cell.be / g2r));
  cell.singa = sin(cell.ga / g2r);
  cell.tanga = tan(cell.ga / g2r);
  cell.phi =  sqrt(1-(cell.cosal * cell.cosal)-
		  (cell.cosbe * cell.cosbe)-(cell.cosga * cell.cosga)
                  +2*cell.cosal * cell.cosbe * cell.cosga);
  cell.tau = cell.c *((cell.cosal- cell.cosbe * cell.cosga) / cell.singa);
  cell.V =  cell.a * cell.b * cell.c*cell.phi;
  cell.as = cell.c * cell.b * sin(cell.al / g2r) / cell.V;
  cell.bs = cell.c * cell.a * sin(cell.be / g2r) / cell.V;
  cell.cs = cell.a * cell.b * cell.singa / cell.V;
  if (filename.endsWith(".fcf", Qt::CaseInsensitive)){
    fileName=filename;
  }else{
    fileName=filename;
    fileName.chop(3);
    fileName.append("fcf");
  }
  foMax=foMin=fcMax=fcMin=0;
  readFCF();
  timer = new QTimer(this);
  timer->setSingleShot(true); 
  connect(timer, SIGNAL(timeout()), this, SLOT(selektiert()));
  QHBoxLayout *lh = new QHBoxLayout(this);
  QVBoxLayout *lt = new QVBoxLayout();
  QVBoxLayout *lr = new QVBoxLayout();
  brw = new QTextBrowser(this);
  brw->setLineWrapMode(QTextEdit::NoWrap);
  brw->setHtml("<h1>Selected Reflections</h1>");

  scene= new GraphSceene(-30,-10,550,530);
  scene->setBackgroundBrush(QBrush(QColor("#e9f7d6")));
  scene->clear ();

 // qDebug()<<"Fo"<<foMin<< foMax<<"Fc "<<fcMin <<fcMax ;
  QGraphicsItem *itm;
  for (int i=0; i<51;i++){
    itm=scene->addLine(i*10,0,i*10,500,QPen(QColor("#cbdbbb"),0));
    itm->setData(0,-1);
    itm=scene->addLine(0,i*10,500,i*10,QPen(QColor("#cbdbbb"),0));
    itm->setData(0,-1);
  }
//  scene->addEllipse(50,450,2,2,QPen(Qt::NoPen),QBrush(QColor("#e90706")));
  //double maxx2=-1000,minn2=10000;
  //minn2=qMin(foMin*foMin,fcMin*fcMin);
  //maxx2=qMax(foMax*foMax,fcMax*fcMax);
  double maxx=-1000,minn=10000;
  minn=qMin(foMin,fcMin);
  maxx=qMax(foMax,fcMax);
  //qDebug()<<minn<<maxx<< minn2<<maxx2;
  for (int i=0; i<Fo.size(); i++){
//    printf("%g %g\n",(Fo.at(i).F-foMin)/(foMax-foMin)*500,500-(Fc.at(i).F-fcMin)/(fcMax-fcMin)*500);
 
  //itm=scene->addEllipse(((Fo.at(i).F*Fo.at(i).F)-minn2)/(maxx2-minn2)*500,500-((Fc.at(i).F*Fc.at(i).F)-minn2)/(maxx2-minn2)*500,2,2,QPen(Qt::NoPen),QBrush(QColor("#e90706")));
  //itm->setData(0,i);
  itm=scene->addEllipse((Fo.at(i).F-minn)/(maxx-minn)*500,500-(Fc.at(i).F-minn)/(maxx-minn)*500,1.5,1.5,QPen(Qt::NoPen),QBrush(QColor("#0907e6")));
  itm->setData(0,i);
  }
  diagonale=scene->addLine(0,500,500,0,QColor("#d99999"));
  itm->setData(0,-2);
  itm=scene->addLine(0,0,3,5,QColor("#000000"));
  itm->setData(0,-1);
  itm=scene->addLine(0,0,-3,5,QColor("#000000"));
  itm->setData(0,-1);
  itm=scene->addLine(0,0,0,500,QPen(QColor("#000000"),1.1));
  itm->setData(0,-1);
  itm=scene->addLine(500,500,495,503,QColor("#000000"));
  itm->setData(0,-1);
  itm=scene->addLine(500,500,495,497,QColor("#000000"));
  itm->setData(0,-1);
  itm=scene->addLine(0,500,500,500,QPen(QColor("#000000"),1.0));
  itm->setData(0,-1);
  txt = scene->addText("Fc");
  txt->moveBy(-20,10); 
  txt->setData(0,-1);
  txt2 = scene->addText("Fo");
  txt2->moveBy(480,503); 
  txt2->setData(0,-1);
  QToolBar *tb=new QToolBar(this);
  tb-> addAction(QIcon(":/icons/zoom-in.png"),"Zoom +",this,SLOT(zoomin())); 
  tb-> addAction(QIcon(":/icons/zoom-out.png"),"Zoom -",this,SLOT(zoomout()));
  tb-> addAction(QIcon(":/icons/zoom-fit.png"),"Zoom selected",this,SLOT(zoomfit()));
  tb-> addAction(QIcon(":/icons/zoom-off.png"),"Zoom out",this,SLOT(zoomoff()));
  QComboBox *cbx=new QComboBox(this);
  cbx->addItem("Fo vs. Fc",1);
  cbx->addItem("Delta( F^2)/sigma vs. sin(theta/lambda)");
  cbx->addItem("Fo / Fc vs. sin(theta/lambda)");
  connect(cbx,SIGNAL(currentIndexChanged(int)),this,SLOT(graphChange(int)));
  tb->addWidget(cbx);
  view = new QGraphicsView(scene,this);
  state = new QLabel("", this);
  connect(scene, 
		  SIGNAL(meinstatus(int)),
		  this,
		  SLOT(st(int)));
  connect(scene,
		 SIGNAL(listen()),
		this,
	       SLOT(delayMe()));	
  QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Close);
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  setMouseTracking(true);
  //view.show();
  scene->selEckt=scene->addRect(0,0,0,0,Qt::DashLine,QBrush(QColor("#d9e7e6")));
  scene->selEckt-> setOpacity(0.5);
  lt->addWidget(view);
  lt->addWidget(tb);
  lt->addWidget(state);
  lt->addWidget(buttonBox);
  QPushButton *omit =new QPushButton("Omit reflections",this);
  connect(omit, SIGNAL(clicked()), this, SLOT(omitref()));
  lr->addWidget(brw);
  lr->addWidget(omit);
  lh->addLayout(lt);
  lh->addLayout(lr);
  //setLayout(lh);
//  int da= scene->itemAt(50, 50)->data(0).toInt();
//  qDebug()<<Fo.at(da).h<<Fo.at(da).k<<Fo.at(da).l<<Fo.at(da).F<<Fc.at(da).F;
}

FcVsFo::~FcVsFo(){
// qDebug()<<"a"; 
 Fo.clear();
// qDebug()<<"b";
 Fc.clear();
// qDebug()<<"c";
// qDebug()<<"d";
 scene->selected.clear();
// qDebug()<<"e";
 scene->clear();
// qDebug()<<"f";
}

void FcVsFo::delayMe(){
  if (!timer->isActive()) timer->start(80);
}

void FcVsFo::zoomin(){
  view->scale(1.05,1.05);
}

void FcVsFo::zoomout(){
  view->scale(0.95,0.95);

}

void FcVsFo::zoomfit(){
  view->fitInView(QRectF(scene->selStart,scene->selEnd),Qt::KeepAspectRatio);//Qt::KeepAspectRatio); 
}

void FcVsFo::zoomoff(){
  view->fitInView(QRectF(-30,-10,550,530),Qt::KeepAspectRatio);
}

void FcVsFo::readFCF(){
  QFile f(fileName);
  listType=0;
  f.open(QIODevice::ReadOnly|QIODevice::Text);
  // figure out what list type it is...
  QString test=QString(f.readLine(120));
  QStringList lines;
  if (test.contains("index")) listType=1;
  //qDebug()<<test<<test.contains(QRegExp("\\s*\\d+\\s*\\d+\\s*\\d+\\s*\\d+\\.\\d+\\s*\\d+\\.\\d+\\s*\\d+"));
  if (test.contains(QRegExp("\\s*\\d+\\s*\\d+\\s*\\d+\\s*\\d+\\.\\d+\\s*\\d+\\.\\d+\\s*\\d+"))) listType=2;
  if (test.contains(QRegExp("\\s*\\d+\\s*\\d+\\s*\\d+\\s*\\d+\\.\\d+\\s*\\d+\\.\\d+\\s*\\d+\\.\\d+"))) listType=5;
  if ((test.startsWith("#"))||(test.startsWith("data"))){
    while (!test.isEmpty()){
      test=QString(f.readLine(120));
      if (test.startsWith("_shelx_refln_list_code")){
	test.remove(0,22);
	test=test.trimmed();
	listType=test.toInt();
	break;
      }
    }  
  }
  //printf("list type %d \n",listType);
  QStringList tok;
  Refls obs,cal;
  double a,b;
  cal.sigma=obs.sigma=0;
  switch (listType){
	  case 0:
		  ;
		  break;
	  case 1:
		  f.reset();
		  lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);
		  for (int i=0; i<lines.size(); i++){
		    tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		    if (!tok.isEmpty()){
		      obs.h=cal.h=tok.at(1).toInt();
		      obs.k=cal.k=tok.at(2).toInt();
		      obs.l=cal.l=tok.at(3).toInt();
		      obs.F=tok.at(5).toDouble();
		      cal.F=tok.at(7).toDouble();
		      obs.stl=cal.stl=
			      sqrt(
					      obs.h*obs.h*cell.as*cell.as+
					      obs.k*obs.k*cell.bs*cell.bs+
					      obs.l*obs.l*cell.cs*cell.cs+
					      2*obs.h*obs.l*cell.as*cell.cs+cell.cosbe+
					      2*obs.k*obs.l*cell.bs*cell.cs+cell.cosal+
					      2*obs.h*obs.k*cell.as*cell.bs+cell.cosga)/2.0;



		      obs.phase=cal.phase=tok.at(8).toDouble();
		      Fc.append(cal);
		      Fo.append(obs);
		    }
		  }
		  break;
	  case 3:{
		   f.reset();
		   lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);

		   int i= 0;
		   for (int j=0; !j; i++){
		     if (lines.at(i).contains("_refln_B_calc")) j++;
		   }
		   for (; i<lines.size(); i++){
		     tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		     if (!tok.isEmpty()&&(tok.size()==7)){
		       obs.h= cal.h=tok.at(0).toInt();
		       obs.k= cal.k=tok.at(1).toInt();
		       obs.l= cal.l=tok.at(2).toInt();
		       obs.F= tok.at(3).toDouble();
		       obs.sigma= tok.at(4).toDouble();
		       a= tok.at(5).toDouble();
		       b= tok.at(6).toDouble();
		       cal.F= sqrt(a*a+b*b);
		       double FF= (b<0)?-1:1;
		       obs.phase= cal.phase= (cal.F!=0)?FF*acosf(a/cal.F):0;
		       obs.phase= cal.phase=fmod(4*M_PI+cal.phase,2*M_PI);
		       obs.stl=cal.stl=
			       sqrt(
					       obs.h*obs.h*cell.as*cell.as+
					       obs.k*obs.k*cell.bs*cell.bs+
					       obs.l*obs.l*cell.cs*cell.cs+
					       2*obs.h*obs.l*cell.as*cell.cs+cell.cosbe+
					       2*obs.k*obs.l*cell.bs*cell.cs+cell.cosal+
					       2*obs.h*obs.k*cell.as*cell.bs+cell.cosga)/2.0;
		       Fc.append(cal);
		       Fo.append(obs);
		     }
		   }
		 }
		 break;
	  case 4:{
		   f.reset();
		   lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);

		   int i= 0;
		   for (int j=0; !j; i++){
		     if (lines.at(i).contains("_refln_observed_status")) j++;
		   }
		   for (; i<lines.size(); i++){
		     tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		     if (!tok.isEmpty()&&(tok.size()==7)){
		       obs.h= cal.h=tok.at(0).toInt();
		       obs.k= cal.k=tok.at(1).toInt();
		       obs.l= cal.l=tok.at(2).toInt();
		       obs.F= sqrt(fabs(tok.at(4).toDouble()));
		       if (tok.at(4).toDouble()<0) obs.F*=-1;
		       cal.F= sqrt(fabs(tok.at(3).toDouble()));
		       if (tok.at(3).toDouble()<0) cal.F*=-1;
		       cal.sigma=obs.sigma=tok.at(5).toDouble();
		       obs.stl=cal.stl=
			       sqrt(
					       obs.h*obs.h*cell.as*cell.as+
					       obs.k*obs.k*cell.bs*cell.bs+
					       obs.l*obs.l*cell.cs*cell.cs+
					       2*obs.h*obs.l*cell.as*cell.cs+cell.cosbe+
					       2*obs.k*obs.l*cell.bs*cell.cs+cell.cosal+
					       2*obs.h*obs.k*cell.as*cell.bs+cell.cosga)/2.0;
		       Fc.append(cal);
		       Fo.append(obs);
		     }
		   }

		 }
		 //		 qDebug()<<"List4 OK";
		 break;
	  case 5:f.reset();
		 lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);
		 for (int i=0; i<lines.size(); i++){
		   tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		   if (!tok.isEmpty()&&(tok.size()>5)){
		     obs.h=cal.h=tok.at(0).toInt();
		     obs.k=cal.k=tok.at(1).toInt();
		     obs.l=cal.l=tok.at(2).toInt();
		     obs.F=tok.at(3).toDouble();
		     cal.F=tok.at(4).toDouble();
		     obs.phase=cal.phase=tok.at(5).toDouble();
		     obs.stl=cal.stl=
			     sqrt(
					     obs.h*obs.h*cell.as*cell.as+
					     obs.k*obs.k*cell.bs*cell.bs+
					     obs.l*obs.l*cell.cs*cell.cs+
					     2*obs.h*obs.l*cell.as*cell.cs+cell.cosbe+
					     2*obs.k*obs.l*cell.bs*cell.cs+cell.cosal+
					     2*obs.h*obs.k*cell.as*cell.bs+cell.cosga)/2.0;
		     Fc.append(cal);
		     Fo.append(obs);
		   }
		 }

		 break;
	  case 6:{
		   f.reset();
		   lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);

		   int i= 0;
		   for (int j=0; (!j)&&(i<lines.size()); i++){
		     if (lines.at(i).contains("_refln_phase_calc")) j++;
		   }
		   // qDebug()<<i<<lines.at(i);
		   for (; i<lines.size(); i++){
		     tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		     if (!tok.isEmpty()&&(tok.size()==7)){
		       obs.h= cal.h=tok.at(0).toInt();
		       obs.k= cal.k=tok.at(1).toInt();
		       obs.l= cal.l=tok.at(2).toInt();
		       obs.F= sqrt(fabs(tok.at(3).toDouble()));
		       cal.sigma=obs.sigma=tok.at(4).toDouble();
		       cal.F= tok.at(5).toDouble();
		       obs.phase=cal.phase=tok.at(6).toDouble();
		       obs.stl=cal.stl=
			       sqrt(
					       obs.h*obs.h*cell.as*cell.as+
					       obs.k*obs.k*cell.bs*cell.bs+
					       obs.l*obs.l*cell.cs*cell.cs+
					       2*obs.h*obs.l*cell.as*cell.cs+cell.cosbe+
					       2*obs.k*obs.l*cell.bs*cell.cs+cell.cosal+
					       2*obs.h*obs.k*cell.as*cell.bs+cell.cosga)/2.0;
		       Fc.append(cal);
		       Fo.append(obs);
//		       printf("%d %d %d %f %f\n",obs.h,obs.k,obs.l,obs.F,cal.F);
		     }
		   }

		 }
		 //		 qDebug()<<"List6 OK";

		 break;
	  case 7:{

		 }
		 break;
	  case 8:{

		   f.reset();
		   lines=QString(f.readAll()).split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);

		   int i= 0;
		   for (int j=0; (!j)&&(i<lines.size()); i++){
		     if (lines.at(i).contains("_shelx_refinement_sigma")) j++;
		   }
//		    qDebug()<<i<<lines.at(i-1);
		   for (; i<lines.size(); i++){
		     tok=lines.at(i).split(' ',QString::SkipEmptyParts);
		     if (!tok.isEmpty()&&(tok.size()==9)){
		       obs.h= cal.h=tok.at(0).toInt();
		       obs.k= cal.k=tok.at(1).toInt();
		       obs.l= cal.l=tok.at(2).toInt();
		       obs.F= sqrt(fabs(tok.at(3).toDouble()));
		       cal.sigma=obs.sigma=tok.at(4).toDouble();
		       cal.F= sqrt(fabs(tok.at(5).toDouble()));
		       obs.phase=cal.phase=tok.at(6).toDouble();
		       obs.stl=cal.stl=1.0/(2.0*tok.at(7).toDouble());
		       obs.w=tok.at(8).toDouble();
		       Fc.append(cal);
		       Fo.append(obs);
		     }
		   }
		 }
		 break;
	  default:
		 break;

  }
  // qDebug() << Fo.size() << Fc.size() << Fo.last().F<<Fc.last().F;
  f.close();
  foMin=fcMin=1000000;
  for (int i=0; i<Fo.size(); i++){
    foMax=qMax(foMax,Fo.at(i).F);
    foMin=qMin(foMin,Fo.at(i).F);
    fcMax=qMax(fcMax,Fc.at(i).F);
    fcMin=qMin(fcMin,Fc.at(i).F);
  }
  //  Fo.clear();
  //  Fc.clear();
  // qDebug()<<"Fo"<<foMin<< foMax<<"Fc "<<fcMin <<fcMax ;
  lines.clear();
  //  qDebug()<<"Fo"<<foMin<< foMax<<"Fc "<<fcMin <<fcMax ;
}

void FcVsFo::omitref(){
  QString omitstr;
  for (int i=0; i<scene->selected.size(); i++){
  omitstr.append(QString("OMIT %1 %2 %3\n")
		    .arg(Fo.at(scene->selected.at(i)).h)
		    .arg(Fo.at(scene->selected.at(i)).k)
		    .arg(Fo.at(scene->selected.at(i)).l));
  }
  emit weg(omitstr); 
}

void FcVsFo::selektiert(){
  
  QString s="<h1>Selected Reflections</h1> <table><tr><th width=\"30\">h</th><th width=\"30\">k</th><th width=\"30\">l</th><th width=\"50\">Fo<sup>2</sup></th><th width=\"50\">Fc<sup>2</sup></th><th width=\"50\">&Delta;(F<sup>2</sup>)/sigma</th><th width=\"50\">sigma</th><th width=\"50\">sin(theta)/lambda</th></tr>\n";
//double wghta=0,wghtb=0;
  for (int i=0; i<scene->selected.size(); i++){
   int sel=scene->selected.at(i); 

   //  Weight = 1 / [ sigma^2(Fo^2) + ( 0.0693 * P )^2 +   0.00 * P ]   where  P = ( Max ( Fo^2, 0 ) + 2 * Fc^2 ) / 3
  // double P= (qMax(0.0,Fo.at(sel).F*Fo.at(sel).F) + 2.0 * Fc.at(sel).F * Fc.at(sel).F )/3.0;
  // double wght=1.0/(Fo.at(sel).sigma + ( wghta * P)*( wghta * P) +wghtb*P);
    s.append(QString(
"<tr>"
"<th style=\"text-align: right;\" >%1</th>"
"<th style=\"text-align: right;\" >%2</th>"
"<th style=\"text-align: right;\" >%3</th>"
"<th style=\"text-align: right;\" >%4</th>"
"<th style=\"text-align: right;\" >%5</th>"
"<th style=\"text-align: right;\" >%6</th>"
"<th style=\"text-align: right;\" >%7</th>"
"<th style=\"text-align: right;\" >%8</th></tr>\n")
		    .arg(Fo.at(sel).h)
		    .arg(Fo.at(sel).k)
		    .arg(Fo.at(sel).l)
		    .arg(Fo.at(sel).F*
			 Fo.at(sel).F)
		    .arg(Fc.at(sel).F*
			 Fc.at(sel).F)
		    .arg(fabs(Fo.at(sel).F*Fo.at(sel).F-				    
				    Fc.at(sel).F*Fc.at(sel).F)/ (Fc.at(sel).sigma))
		    .arg(Fc.at(sel).sigma)
		    .arg(Fc.at(sel).stl));
    //Delta(F^2)/esd  Fc/Fc(max)  Resolution(A)
    //
//    qDebug()<<Fo.at(scene->selected.at(i)).h<<Fo.at(scene->selected.at(i)).k<<Fo.at(scene->selected.at(i)).l<<Fo.at(scene->selected.at(i)).F<<Fc.at(scene->selected.at(i)).F;
  } 
   s.append("</table>\n");
 // qDebug()<<s;
  brw->setHtml(s);
}

void  FcVsFo::graphChange(int type){
  QList<QGraphicsItem *> drin= scene->items (QRectF(-30,-10,550,530),Qt::ContainsItemShape);
  view->hide();
  double maxx=-1000,minn=10000;
  minn=qMin(foMin,fcMin);
  maxx=qMax(foMax,fcMax);
  double maxsintl=0;
  switch (type){
	  case 1:
		  {
		    if (txt2->toPlainText()!="sin(theta)/lambda"){
		      txt2->setPlainText("sin(theta)/lambda");
		      txt2->moveBy(-100,0);
		    }
		    txt->setPlainText("Delta( F^2)/sigma(F^2)");
		    maxx=-1000;
		    minn=10000;
		    for (int i=0; i<Fo.size(); i++){
		      double xx=fabs(Fo.at(i).F*Fo.at(i).F-Fc.at(i).F*Fc.at(i).F)/Fo.at(i).sigma;
		      minn=qMin(minn,xx);
		      maxx=qMax(maxx,xx);
		      //    qDebug()<<Fo.at(i).stl<<maxsintl;
		      maxsintl=qMax(maxsintl,Fo.at(i).stl);
		    }
		  }
		  break;
	  case 0:
		  {
		    txt->setPlainText("Fc");
		    txt2->setPlainText("Fo");
		    txt2->moveBy(100,0);
		  }
		  break;
	  case 2:
		  {
		    if (txt2->toPlainText()!="sin(theta) / lambda"){
		      txt2->setPlainText("sin(theta) / lambda");
		      txt2->moveBy(-100,0);
		    }
		    txt->setPlainText("Fo / Fc");
		    maxx=-1000;
		    minn=10000;
		    for (int i=0; i<Fo.size(); i++){
		      double xx=Fo.at(i).F/Fc.at(i).F;
		      if ((isnormal(xx))&&(xx>0.001)){
		      minn=qMin(minn,xx);
		      maxx=qMax(maxx,xx);
//		      qDebug()<<Fo.at(i).stl<<maxsintl<<xx;
		      maxsintl=qMax(maxsintl,Fo.at(i).stl);
		      }
		    }

		  }
		  break;
  }
  QRectF ect;
  if (type!=0) diagonale->hide();
  else diagonale->show();
  for (int i=0; i<drin.size();i++){
	bool ok=false;
	int ix;
    ix=drin.at(i)->data(0).toInt(&ok);
    ect=drin.at(i)->sceneBoundingRect();
	if ((ix>-1)&&(ok)&&(ix<Fo.size())&&(ix<Fc.size())) {
	drin[i]->moveBy(-ect.x(),-ect.y());
	  switch (type){
		  case 0:
			  drin[i]->moveBy((Fo.at(ix).F-minn)/(maxx-minn)*500,500-(Fc.at(ix).F-minn)/(maxx-minn)*500);
			  break;
		  case 1:{
			  double xx=fabs(Fo.at(ix).F*Fo.at(ix).F-Fc.at(ix).F*Fc.at(ix).F)/Fo.at(ix).sigma;
			  if ((isnormal(Fc.at(ix).stl))&&(isnormal(xx))){
//			    printf("%g %g\n",Fc.at(ix).stl/maxsintl*500,500-(xx-minn)/(maxx-minn)*500);
			  drin[i]->moveBy(Fc.at(ix).stl/maxsintl*500,500-(xx-minn)/(maxx-minn)*500);
			  }
			  else drin[i]->moveBy(0,500);
			 }
			 break;
		  case 2:{
			  double xx=Fo.at(ix).F / Fc.at(ix).F;
			  if ((isnormal(Fc.at(ix).stl))&&(isnormal(xx))){
			    drin[i]->moveBy(Fc.at(ix).stl/maxsintl*500,500-(xx-minn)/(maxx-minn)*500);
			  }
			  else drin[i]->moveBy(0,500);
			 }
			 break;
	    
	  }

	}
  }
  view->show();
}


GraphSceene::GraphSceene( qreal x, qreal y, qreal width, qreal height, QObject * parent ) :QGraphicsScene (  x,  y, width,  height, parent){
//constructor 
} 

void GraphSceene::mouseMoveEvent ( QGraphicsSceneMouseEvent  * mouseEvent ){
  qreal  x= mouseEvent->scenePos().x(),
	 y= mouseEvent->scenePos().y();
//  qDebug()<<mouseEvent->buttons()<< mouseEvent->buttonDownScreenPos (Qt::LeftButton);
  if (mouseEvent->buttons()&Qt::LeftButton){
    QPointF oli,ure;
    oli.setX(qMin(selStart.x(),selEnd.x()));
    oli.setY(qMin(selStart.y(),selEnd.y()));
    ure.setX(qMax(selStart.x(),selEnd.x()));
    ure.setY(qMax(selStart.y(),selEnd.y()));
    QList<QGraphicsItem *> drin= items ( QRectF(oli,ure),Qt::ContainsItemShape);
//    qDebug()<<drin.size();
    selected.clear();
    if (drin.size()){
      for (int i=0; i<drin.size();i++){
	bool ok=false;
	int ix;
	ix=drin.at(i)->data(0).toInt(&ok);
	if ((ix>-1)&&(selected.size()<100)&&(ok)) selected.append(ix);      
      }
      emit listen();    
    }
    selStart = mouseEvent->buttonDownScenePos (Qt::LeftButton);
    selEnd =   mouseEvent->scenePos();
    if (selStart!=QPointF(0,0)) selEckt->setRect(QRectF(selStart,selEnd));
    drin.clear();
   // qDebug()<<"End"<<selEnd;
  }else{	
    // qDebug()<<x<<y;
    if (!itemAt(x,y)) emit meinstatus(-1);
    else{
      int da = itemAt(x,y)->data(0).toInt(); 
      emit meinstatus(da);  
    }
  }
}

void GraphSceene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent ){
   if (mouseEvent->button()==Qt::LeftButton){
     selEckt->setRect(0,0,0,0);
     selected.clear();
     emit listen();
    // qDebug()<<"start"<<selStart;
   }
}



void FcVsFo::st(int da){
if (da<0) state->setText("");
else{
  QString s;
  s=QString("%1 %2 %3 Fo=%4 Fc%5 delta(F^2)/sigma(F^2)= %6")
	  .arg(Fo.at(da).h).arg(Fo.at(da).k).arg(Fo.at(da).l).arg(Fo.at(da).F).arg(Fc.at(da).F)
	  .arg(fabs(Fo.at(da).F*Fo.at(da).F-Fc.at(da).F*Fc.at(da).F)/Fo.at(da).sigma)
	  ;
  state->setText(s);
}
}
#ifdef vc7hack
#include <float.h>
int isnormal(double arg){
    return _finite(arg)||(arg==0);
}
#endif
