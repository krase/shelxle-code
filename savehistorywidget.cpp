#include "savehistorywidget.h"

SaveHistoryWidget::SaveHistoryWidget(QString filename, QWidget *parent  , Qt::WindowFlags f ):QDialog(parent,f){
  // DragDropVersions *dv = new DragDropVersions(parent);
  // dv->show();
  pvindex=-1;
  fileName=filename;
  dda = new DragDropVersions(filename,parent);
  QScrollArea *qsa = new QScrollArea();
  qsa->setWidget(dda);
  QScrollArea *preview = new QScrollArea();
  QVBoxLayout *pvla = new QVBoxLayout();
  pvheader = new QLabel("<h3>current file version</h3>"); 
  pvheader->setStyleSheet (QString(
			  "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
			  "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
			  "stop: 0 #ffffff, stop: 1 %1);border: 2px solid #eeeeee;}").arg(QColor("#aaeeaa").name()));
  edi = new CodeEditor(); 
  QString inspatternfile=":ins_res_style.ini";
  edi->highlighter->loadHighlightPatterns(inspatternfile);
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(9);
  edi->setFont(font);
  edi->setLineWrapMode(QPlainTextEdit::NoWrap);
  QFile p(filename);
  p.open(QIODevice::ReadOnly|QIODevice::Text);
 edi->setPlainText(orginal=p.readAll());
 p.close();
  pvla->addWidget(pvheader);
  pvla->addWidget(edi);
  edi->setReadOnly ( true);
  preview->setLayout(pvla);
 QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
  ok = buttonBox->button(QDialogButtonBox::Open);

  ok->setDisabled(true);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
/*  QLabel *usageHint = new QLabel();
  usageHint->setText("<h1>Inherit labels assigning dialog</h1> <ul><li>If you click on OK here, then atoms on the<br>"
		  " right collumn will inherit their Labels from<br> the atoms given in the left collumn.</li>"
		  "<li> You can change the ordering of the right<br> collumn by drag and drop of its items.</li>"
		  "<li> If you click on the items on the left column<br> the atoms in the structure will<br> be <em>highlighted</em>.</li>"
		  "<li> You <em>MUST</em> assingn a new <em>ResidueNr</em> and an<br> appropriate <em>Residue Class</em> here!</ul>");
  usageHint->setStyleSheet (QString(
			  "QLabel{font: bold; background-color:#ffffaa;}"));// */
  QHBoxLayout *la = new QHBoxLayout();
  QVBoxLayout *lo = new QVBoxLayout();
  QSplitter *splitter = new QSplitter(parent);

  splitter->addWidget(qsa);
  splitter->addWidget(preview);
  splitter->setChildrenCollapsible (false);
  splitter->setStretchFactor(1, 1);
  la->addWidget(splitter);
  lo->addLayout(la);
  //  lo->addLayout(rhla);
  lo->addWidget(buttonBox);
  setLayout(lo);
  connect(dda,SIGNAL(indexChanged(int)),this,SLOT(updatePreview(int)));
  connect(dda->tick,SIGNAL(timeout()), this , SLOT(tock()));
  connect(this,SIGNAL(finished(int)), dda->tick,SLOT(stop()));
  setWindowTitle(tr("Save History"));
}

void SaveHistoryWidget::updatePreview(int index){
  if ((index>=0)&&(index<dda->zeiten.size())) {
    DragVersion *childle = static_cast<DragVersion *>(sender());
    if (childle)
    childle->setStyleSheet (QString(
			  "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
			  "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
			  "stop: 0 #ffffff, stop: 1 %1);border: 0px solid #eeeeee;;}").arg(QColor("#aaeeaa").name()));
    pvheader->setText(QString("<h3>%1</h3>").arg(dda->seit(dda->now.secsTo(dda->zeiten.at(index)))));
    pvindex=index;
    edi->setReadOnly(false);
    QString ext=dda->entries.at(index);
    ext.remove(QRegExp("^\n"));
    edi->setPlainText(ext);
    edi->diffTo(orginal);
    edi->setReadOnly(true);
  }
  else if (index==-1) {
    pvindex=index;
    pvheader->setText("<h3>current file version</h3>");
    edi->setReadOnly(false);
    QFile p(fileName);
    p.open(QIODevice::ReadOnly|QIODevice::Text);  
    edi->setPlainText(p.readAll());    
    edi->diffTo(orginal);
    p.close(); 
    edi->setReadOnly(true);
  }
}

void SaveHistoryWidget::tock(){
if ((pvindex!=-1)&&(pvindex<dda->zeiten.size())){
    pvheader->setText(QString("<h3>%1</h3>").arg(dda->seit(dda->now.secsTo(dda->zeiten.at(pvindex)))));
    ok->setDisabled(false);
}else ok->setDisabled(true);
}

void SaveHistoryWidget::accept(){
  QFile f(fileName);

  f.open(QIODevice::ReadOnly|QIODevice::Text);
  QString alterScheis=f.readAll();
  QFileInfo fi(f);
  f.close();

  bool success = f.open(QIODevice::WriteOnly|QIODevice::Text);
  if (success){
    f.write(edi->toPlainText().toLatin1());
    f.close();
    QString dn=fileName.section('/',0,-2),
	    argument=fileName.section('/',-1);
    QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
    if (!work.exists ( QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME))) work.mkdir (QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
    QString saveHistName=QString("%1/%2saves/SAVEHIST").arg(dn).arg(PROGRAM_NAME);
    QFile sahi(saveHistName);
    sahi.open(QIODevice::Append|QIODevice::Text);
    sahi.write(QString("Entry|@|%1|@|%2|@|\n").arg(fi.lastModified().toString(Qt::ISODate)).arg(argument).toLatin1());
    sahi.write(alterScheis.toLatin1());
//    editor->document()->setModified(false);
//    loadFile(dirName);
  }
  done(QDialog::Accepted);
}

QString DragDropVersions::seit(int wann){
  /*! turns seconds into a human readable QString Example: 1 days 12 hours 24 minutes 36 seconds ago
      \param wann seconds elapsed. Negative values lie in the furure. 
      \returns Human readable time elapsed (long version).
  */ 
  QString dann;
  int i = abs(wann);
  int d = i / 86400;
  dann = (d)?((d==1)?"1 day ":QString("%1 days ").arg(d)):" ";
  int h = (i - d*86400)/3600;
  dann += (h)?((h==1)?"1 hour ":QString("%1 hours ").arg(h)):" ";
  int m = ((i - d*86400)-(h*3600))/60;
  dann += (m)?((m==1)?"1 minute ":QString("%1 minutes ").arg(m)):" ";
  int s = ((i - d*86400)-(h*3600)-(m*60));
  dann += (s)?((s==1)?"1 second ":QString("%1 seconds ").arg(s)):" ";
 // printf("%d d%d h%d m%d s%d \n ", i,d,h,m,s);

  if (wann <0 ) dann+=" ago";
  else if (wann == 0 ) dann+="now";
  else dann.prepend("in ");
  return dann.simplified();
}

QString DragDropVersions::seitkurzem(int wann){
  /*! turns seconds into a human readable QString Example: 1d 12h 24m 36s ago
   \param wann seconds elapsed. Negative values lie in the furure. 
   \returns Human readable time elapsed (short version). 
  */
  QString dann;
  int i = abs(wann);
  int d = i / 86400;
  dann = (i>=86400)?(QString("%1d ").arg(d)):" ";
  int h = (i - d*86400)/3600;
  dann += (i>=3600)?(QString("%1h ").arg(h,2,10,QChar('0'))):" ";
  int m = ((i - d*86400)-(h*3600))/60;
  dann += (i>=60)?(QString("%1m ").arg(m,2,10,QChar('0'))):" ";
  int s = ((i - d*86400)-(h*3600)-(m*60));
  dann += QString("%1s ").arg(s,2,10,QChar('0'));
 // printf("%d d%d h%d m%d s%d \n ", i,d,h,m,s);

  if (wann <0 ) dann+=" ago";
  else if (wann == 0 ) dann+="now";
  else dann.prepend("in ");
  return dann.simplified();
}

DragDropVersions::DragDropVersions(QString filename, QWidget *parent) :QWidget(parent){
  /*! 
   *  Gets the directory of filename and 
   *  loads in the subdirectory "PROGRAM_NAME"saves (usualy shelXlesaves/) the file SAVEHIST.
   *  In that file each entry is seperated by something like "Entry|@|2011-03-08T13:32:19|@|b11.res|@|"
   *  @param filename the file path of the res or ins file
   *  @param parent parent widget
   */
  QString dn=filename.section('/',0,-2),
	  argument=filename.section('/',-1);

      QString insorres=argument;
      if (insorres.endsWith("res",Qt::CaseInsensitive)) insorres.replace(QRegExp("res$",Qt::CaseInsensitive),"ins");
      else if (insorres.endsWith("ins",Qt::CaseInsensitive)) insorres.replace(QRegExp("ins$",Qt::CaseInsensitive),"res");

  current=-1;
  QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
  QString saveHistName=QString("%1/%2saves/SAVEHIST").arg(dn).arg(PROGRAM_NAME);
  QFile sahi(saveHistName);
  sahi.open(QIODevice::ReadOnly|QIODevice::Text);
  QString Alles=sahi.readAll();//Entry|@|2011-03-08T13:32:19|@|b11.res|@|
  sahi.close();
  QRegExp re = QRegExp("Entry\\|@\\|\\w+-\\w+-\\w+:\\w+:\\w+\\|@\\|[^@]+@\\|");
  entries = Alles.split(re,QString::SkipEmptyParts);
  int pos = re.indexIn(Alles);
  QStringList filenames;
  while (pos>-1) {
    zeiten.append(QDateTime::fromString(re.cap(0).section('|',2,2),Qt::ISODate));
    filenames.append(re.cap(0).section('|',4,4));
    re.indexIn(Alles,pos+30);
    pos=re.pos(0);
  }
  now = QDateTime::currentDateTime();
  for (int j=0; j<filenames.size(); j++)
  for (int i=0; i<filenames.size(); i++){
    if ((filenames.at(i)!=argument)&&(filenames.at(i)!=insorres)){
	entries.removeAt(i);
	zeiten.removeAt(i);
	filenames.removeAt(i);          
    }
  }
  int x = 5;
  int y = 5;
  int row=0,lawi=0,ymax=0;
  DragVersion * l;

  l =new DragVersion(-1,"current file version",666,-1,this);
  l->move(5, 5);
  l->show();
  breite=lawi = itemwidth=l->width()+25;
  x=lawi+5;
  for (int i=0; i<zeiten.size(); i++){
    DragVersion *wordLabel = new DragVersion(i,seitkurzem(now.secsTo(zeiten.at(i))),zeiten.at(i).toTime_t(),-1 , this);
    wordLabel->move(x, y);
    wordLabel->show();
    wordLabel->setAttribute(Qt::WA_DeleteOnClose);
    itemheight=wordLabel->height();
    itemwidth = qMax(wordLabel->width()+25,itemwidth);
    y += wordLabel->height() + 2;
    ymax=qMax(ymax,y+itemheight);
    if (y / height()) {
      lawi = itemwidth;
      row++; 
    }
    x = (y / height())? lawi+x-20 : x;
    y = (y / height())? 5 : y;
  }// */

  QPalette newPalette = palette();
  newPalette.setColor(QPalette::Window, Qt::white);
  setPalette(newPalette);

  setFixedSize(itemwidth+x+5, qMax(200, ymax));
  tick = new QTimer(this);
  connect(tick,SIGNAL(timeout()), this , SLOT(tack()));
  tick->start(250);
  setAcceptDrops(true);
}

void DragDropVersions::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.fillRect(event->rect(), QColor("#eeeeee"));
    p.fillRect(0,0,breite,height(),QColor("#aacccc"));


}

void DragDropVersions::tack(){
  QObjectList kinder =children () ;
  now = QDateTime::currentDateTime(); 
  for (int i=0; i<kinder.size(); i++){
    DragVersion *child = static_cast<DragVersion*>(kinder.at(i));
    if (!child||!child->inherits("QLabel")) continue;
    //printf("?? %i %p %d\n",i,child,child->inherits("QLabel"));
    int idx=child->index();
    if ((idx==-1)&&(child->secs()==666)) {
      child->setStyleSheet (QString(
			      "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
			      "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
			      "stop: 0 #ffffff, stop: 1 %1);border: 2px solid #eeeeee;}").arg(QColor((idx==current)?"#aaeeaa":"#ffeecc").name()));
    }
    if ((idx<0)||(idx>=zeiten.size())||(child->secs()==666)) continue;
    //printf("[%d] %d uti %d %s [%s](%s)\n",
//		    kinder.size(),
//		    idx,
//		    child->secs(),
//		    seitkurzem(now.secsTo(QDateTime::fromTime_t (child->secs()))).toStdString().c_str(),
//		    QDateTime::fromTime_t(child->secs()).toString( Qt::ISODate).toStdString().c_str(),
//		    child->text().toStdString().c_str()
//		    );
    child->setText(seitkurzem(now.secsTo(QDateTime::fromTime_t (child->secs()))));
    child->setStyleSheet (QString(
			  "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
			  "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
			  "stop: 0 #ffffff, stop: 1 %1);border: 2px solid #eeeeee;}").arg(QColor((idx==current)?"#aaeeaa":"#ffeecc").name()));
    child->adjustSize ();
    //printf("!\n");
  }// */

}

void DragDropVersions::mousePressEvent(QMouseEvent *event){
  DragVersion *child = static_cast<DragVersion*>(childAt(event->pos()));
  if (!child)
  return;
//  printf ("INDEX %d\n",child->index());
  current = child->index();
  emit indexChanged(current);
}

////////////////////////////////////////////////////////////////////////////////
DragVersion::DragVersion(const int index,const QString &text, uint secs,const int cur, QWidget *parent): QLabel(parent){
  m_labelText = text;
  m_index = index;
  m_secs = secs;
  setText(m_labelText);
  setStyleSheet (QString(
			  "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
			  "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
			  "stop: 0 #ffffff, stop: 1 %1);border: 2px solid #eeeeee;}").arg(QColor((index==cur)?"#aaeeaa":"#ffeecc").name()));

 /* QColor c("#eeeeaa");
  QFontMetrics metric(font());
  QSize size = metric.size(Qt::TextSingleLine, m_labelText);

  QImage image(size.width() + 10, size.height() + 10,
		  QImage::Format_ARGB32_Premultiplied);
  image.fill(qRgba(0, 0, 0, 0));

  QFont font;
  font.setStyleStrategy(QFont::ForceOutline);
  font.setWeight(QFont::Black);

  QLinearGradient gradient(0, 0, 0, image.height()-1);
  gradient.setColorAt(0.0, Qt::white);
  gradient.setColorAt(0.2, c);
  gradient.setColorAt(0.8, c);
  gradient.setColorAt(1.0, QColor(127, 127, 200));

  QPainter painter;
  painter.begin(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(gradient);
  painter.drawRoundedRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),
		  5, 5, Qt::RelativeSize);

  painter.setFont(font);
  painter.setBrush(Qt::black);
  painter.drawText(QRect(QPoint(4, 4), size + QSize(5,5)), Qt::AlignLeft, m_labelText);
  painter.end();
  setPixmap(QPixmap::fromImage(image));*/
}

QString DragVersion::labelText() const{
  return m_labelText;
}

int DragVersion::index() const{
  return m_index;
}

uint DragVersion::secs() const{
  return m_secs;
}

