#include "dragdropatomsdlg.h"
DragDropAtomsDialog::DragDropAtomsDialog(Molecule &mole, const QList<int> &reference, QList<int> *target, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f){
    DragDropAtoms *dda = new DragDropAtoms(mole,reference,target,parent);
    QScrollArea *qsa = new QScrollArea();
    qsa->setWidget(dda);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    ok = buttonBox->button(QDialogButtonBox::Ok);

    ok->setDisabled(true);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QLabel *usageHint = new QLabel();
    usageHint->setText("<h1>Inherit labels assigning dialog</h1> <ul><li>If you click on OK here, then atoms on the<br>"
                       " right collumn will inherit their Labels from<br> the atoms given in the left collumn.</li>"
                       "<li> You can change the ordering of the right<br> collumn by drag and drop of its items.</li>"
                       "<li> If you click on the items on the left column<br> the atoms in the structure will<br> be <em>highlighted</em>.</li>"
                       "<li> You <em>MUST</em> assingn a new <em>ResidueNr</em> and an<br> appropriate <em>Residue Class</em> here!</ul>");
    usageHint->setStyleSheet (QString(
                "QLabel{font: bold; background-color:#ffffaa;}"));
    QHBoxLayout *la = new QHBoxLayout();
    setWindowTitle(tr("Inherit labels assigning dialog"));
    QLabel *resiNrSpinL = new QLabel("Residue Nr:");
    resiNrSpin = new QSpinBox();
    resiNrSpin->setMinimum(0);
    resiNrSpin->setMaximum(9999);
    resiNrSpin->setValue(0);
    //connect(resiNrSpin,SIGNAL(valueChanged(int)),this,SLOT(updateLabel()));
    connect(resiNrSpin,SIGNAL(valueChanged(int)),this,SLOT(renameRNchanged(int)));
    QLabel *resiResiClassL = new QLabel("Residue Class:");
    resiResiClass = new QLineEdit("");
    resiResiClass->setMaxLength(4);
    resiResiClass->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z]\\w{0,3}"),resiResiClass));
    resiResiClass->setMinimumWidth(100);
    connect(resiResiClass,SIGNAL(textEdited(const QString&)),this,SLOT(grossRC(const QString&)));
    resiResiClass->setEnabled(false);
    QHBoxLayout *rhla = new QHBoxLayout();
    rhla->addWidget(resiNrSpinL);
    rhla->addWidget(resiNrSpin);
    rhla->addWidget(resiResiClassL);
    rhla->addWidget(resiResiClass);

    QVBoxLayout *lo = new QVBoxLayout();
    la->addWidget(qsa);
    la->addWidget(usageHint);
    lo->addLayout(la);
    lo->addLayout(rhla);
    lo->addWidget(buttonBox);
    setLayout(lo);
    setMinimumWidth(190+usageHint->sizeHint().width());
    connect(dda,SIGNAL(pairs(QString)),this,SIGNAL(pairs(QString)));
}

void DragDropAtomsDialog::grossRC(const QString &text){
  QString s=text.toUpper ();
  resiResiClass->setText(s);
  update();
}

void DragDropAtomsDialog::renameRNchanged(int ii){
  QMap<int,QString> resNrClass;
  for (int i=0; i<resiFinder->count(); i++){
    QString resil=resiFinder->itemText(i);
    resil.remove("RESI");
    QString resinr=resil;
    resil.remove(QRegExp("\\b\\d+\\b"));
    resinr.remove(QRegExp("\\b\\D\\S+"));
    int rn=resinr.toInt();
    resNrClass[rn]=resil.trimmed();
  }
  if (!ii) {
    resiResiClass->setEnabled(false);
    ok->setDisabled(true);
    resiResiClass->setText("");
  }
  else {
    resiResiClass->setDisabled(resNrClass.contains(ii));
    ok->setDisabled(resNrClass.contains(ii));
    resiResiClass->setText(resNrClass[ii]);
  }

}
DragDropAtoms::DragDropAtoms(Molecule &mole, const QList<int> &reference, QList<int> *target, QWidget *parent)
    :QWidget(parent){
  m_mole = mole;
  m_reference = reference;
  m_target = target;  

//  setMouseTracking(true);
  int x = 5;
  int y = 5;
  dadas.clear();
  //printf("%d %d \n",reference.size(),target.size());
  for (int i=0; i<target->size(); i++){
    int j=target->at(i);
    int k=reference.at(i);
    dadas.append(QString("%1 %2").arg(k).arg(j));
    //printf("%d %d %s\n",i,j,mole.asymm.at(j).Label.toStdString().c_str());
    QLabel * l =new QLabel(mole.asymm.at(k).Label,this);
    l->move(x, y);
    l->show();
    l->setAcceptDrops(false);
    l->setAlignment(Qt::AlignCenter);
    l->setAttribute(Qt::WA_DeleteOnClose);
    l->setStyleSheet (QString(
            "QLabel{font: bold; border: 2px solid #2f2f31; border-radius: 1px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 %1, stop: 1 #dadbde);} "
            "QLabel:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
                        "stop: 0 #ffffff, stop: 1 %1);border: 0px;}").arg(mole.AtomColor[mole.asymm.at(k).an].name()));
    DragAtom *wordLabel = new DragAtom(j,mole.asymm.at(j).Label,mole.AtomColor[mole.asymm.at(j).an], this);
    wordLabel->move(60, y);
    l->setFixedSize(47,wordLabel->height()-2);
    wordLabel->show();
    wordLabel->setFixedWidth(56);
    wordLabel->setAttribute(Qt::WA_DeleteOnClose);
    dragAtoms.append(wordLabel);
    itemheight=wordLabel->height();
    y += wordLabel->height() + 2;
  }

  QPalette newPalette = palette();
  newPalette.setColor(QPalette::Window, Qt::white);
  setPalette(newPalette);

  setFixedSize(120, qMax(200, y));
  setAcceptDrops(true);
}

void DragDropAtoms::dragEnterEvent(QDragEnterEvent *event){

  if (event->mimeData()->hasFormat("application/x-dragdropatom")) {
    //printf("dragEnterEvent\n");
    if (children().contains(event->source())) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    } else {
      event->acceptProposedAction();
    }
  }  else {
    event->ignore();
  }
}

void DragDropAtoms::dragMoveEvent(QDragMoveEvent *event){

  if (event->mimeData()->hasFormat("application/x-dragdropatom")) {
    //printf("dragMoveEvent\n");
    int idx =  (event->pos().y()-5) / (itemheight+2);
    //printf("idx %d  current %d\n",idx,current);
    for (int i = 0; i< dragAtoms.size(); i++){
        int k=0;
        if ((idx<current)&&(i>=idx)&&(i<current)) k=1;
        if ((idx>current)&&(i<=idx)&&(i>=current)) k=-1;
        dragAtoms.at(i)->move(60, 5+(itemheight+2)*(i+k));
    }
    if (children().contains(event->source())) {
      event->setDropAction(Qt::MoveAction);

      event->accept();
    } else {
      event->acceptProposedAction();
    }
  } else {
    event->ignore();
  }
  update();
}

void DragDropAtoms::dropEvent(QDropEvent *event){

  if (event->mimeData()->hasFormat("application/x-dragdropatom")) {
    const QMimeData *mime = event->mimeData();
    QByteArray itemData = mime->data("application/x-dragdropatom");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    int idx;
    QColor cc;
    QString text;
    QPoint offset;
    dataStream >>idx>>cc>> text >> offset;
    int index =  (event->pos().y()-5) / (itemheight+2);

    m_target->removeOne(idx);
    m_target->insert(index,idx);
    //text=text.toLower();
    DragAtom *newLabel = new DragAtom(idx,text,cc, this);
    newLabel->move(event->pos() - offset);
    newLabel->show();
    newLabel->setAttribute(Qt::WA_DeleteOnClose);
    dragAtoms.removeAt(current);
    dragAtoms.insert(index,newLabel);
    //printf("dropEvent\n");
    if (event->source() == this) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    } else {
      event->acceptProposedAction();
    }
    dadas.clear();
    for (int i = 0; i< dragAtoms.size(); i++){
        dragAtoms.at(i)->move(60, 5+(itemheight+2)*(i));
        int j=m_target->at(i);

        int k=m_reference.at(i);

        dadas.append(QString("%1 %2").arg(k).arg(j));
        printf("%d %s %s\n",i,m_mole.asymm.at(k).Label.toStdString().c_str(),m_mole.asymm.at(j).Label.toStdString().c_str());
    }
  } /*else if (event->mimeData()->hasText()) {
    QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"),
		    QString::SkipEmptyParts);
    QPoint position = event->pos();

    foreach (QString piece, pieces) {
      DragAtom *newLabel = new DragAtom(0,piece,Qt::white this);
      newLabel->move(position);
      newLabel->show();
      newLabel->setAttribute(Qt::WA_DeleteOnClose);

      position += QPoint(newLabel->width(), 0);
    }

    event->acceptProposedAction();
  }*/ else {
    event->ignore();
  }
}
/*
void DragDropAtoms::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons()==Qt::NoButton){
        static int last=-1;
        int index =  (event->pos().y()-5) / (itemheight+2);
        printf("index %d x=%d y=%d  %d\n",index,event->pos().x(),event->pos().y(),itemheight);
        QString dada="";


        if ((index<0)||(index>=m_reference.size())) {
            emit pairs(dada);
            last=index;
            return;
        }
        if ((event->pos().x()>5)){
            emit pairs(dadas.at(index));

            last=index;
            return;
        }

        last=index;
       emit pairs(dada);
    }
}
*/
void DragDropAtoms::mousePressEvent(QMouseEvent *event){
    if (event->pos().x()<50) {

        int index =  (event->pos().y()) / (itemheight+2);
        emit pairs(dadas.at(index));
        event->accept();return;}
  DragAtom *child = static_cast<DragAtom*>(childAt(event->pos()));
  if (!child)
    return;

  QPoint hotSpot = event->pos() - child->pos();

  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream<<child->index()<<child->color() << child->labelText() << QPoint(hotSpot);
  current = m_target->indexOf(child->index());
  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/x-dragdropatom", itemData);
  mimeData->setText(child->labelText());

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(*child->pixmap());
  drag->setHotSpot(hotSpot);

  child->hide();

  if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
    child->close();
  else
    child->show();
}
//void DragDropAtoms::
//void DragDropAtoms::
//
//
DragAtom::DragAtom(const int index,const QString &text,QColor c, QWidget *parent): QLabel(parent){
  m_index=index;
  m_labelText=text;
  m_c=c;
  QFontMetrics metric(font());
  QSize size = metric.size(Qt::TextSingleLine, text);

  QImage image(size.width() + 12, size.height() + 12,
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
		  25, 25, Qt::RelativeSize);

  painter.setFont(font);
  painter.setBrush(Qt::black);
  painter.drawText(QRect(QPoint(6, 6), size), Qt::AlignCenter, text);
  painter.end();

  setPixmap(QPixmap::fromImage(image));
}

QString DragAtom::labelText() const {
  return m_labelText;
}

int DragAtom::index() const{
  return m_index;
}

QColor DragAtom::color() const{
return m_c;
}
