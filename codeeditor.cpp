/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "codeeditor.h"
#include "shx_helper.h"
//#include "chgl.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), c(0){
  lineNumberArea = new LineNumberArea(this);
  lineNumberArea->setObjectName("lineNumberArea");
  afixHighlightArea = new AfixHighlightArea(this);
  afixHighlightArea->setObjectName("afixHighlightArea");
  setMouseTracking(true);
  midCursorpos=0;
  //setTabChangesFocus(true);
  highlighter = new Highlighter(document());
  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
//  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  connect(this, SIGNAL(textChanged()),this,SLOT(updateAfix()));
  connect(this, SIGNAL(search(const QString &)),highlighter,SLOT(highlightSearch(const QString &)));
//  updateLineNumberAreaWidth(0);
  //highlightCurrentLine();

  updateAfix();
}

CodeEditor::~CodeEditor(){
  disconnect(this);
  delete lineNumberArea;
  delete afixHighlightArea;
  delete highlighter;

}

int CodeEditor::lineNumberAreaWidth(){
  int digits = 4;//2;
  //int max = qMax(1, blockCount());
  //while (max >= 10) {
  //  max /= 10;
  //  ++digits;
  //}

  int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;
  return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */){ 
    setViewportMargins(lineNumberAreaWidth()+15, 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy){
  Q_UNUSED ( rect );
//  printf("updateLineNumberArea\n");
    if (dy){
        lineNumberArea->scroll(0, dy);
        afixHighlightArea->scroll(0,dy);
    }
   /*
    else{
      printf("Area->update1 %d %d\n",rect.x(),rect.y());
    //lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
      printf("Area->update2\n");
    //afixHighlightArea->update(0,rect.y(),10,rect.height());
      printf("Area->update3\n");
}*/
  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
    //*/
}

void CodeEditor::resizeEvent(QResizeEvent *e){
  QPlainTextEdit::resizeEvent(e);
  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
  afixHighlightArea->setGeometry(QRect(
      cr.left()+lineNumberAreaWidth()+1,
      cr.top(),
      10,cr.height()
      ));
  e->accept();
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event){
  QPainter painter(lineNumberArea);
  int current = textCursor().blockNumber();
  painter.fillRect(event->rect(), QColor("#eeeeee"));
  QTextBlock block = firstVisibleBlock();
 // printf("edidor Font: %s italic%d bold %d point %d pixel %d height %d\n",fontInfo().family().toStdString().c_str(),fontInfo().italic (),fontInfo().bold (),fontInfo().pointSize (),fontInfo().pixelSize (),fontMetrics().height());
//   printf("lineNumberAreaPaintEvent\n");
  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  // printf ("line %d %d\n",top,bottom);
  int minbl=8888888,maxbl=0;
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      bool iscurrent=(blockNumber==current);
      if (blockNumber%2) painter.fillRect(0, top,
                                      lineNumberArea->width(),
                                      fontMetrics().height(),QColor("#dddddd"));
      if (iscurrent) painter.fillRect(0, top,
                                      lineNumberArea->width(),
                                      fontMetrics().height(),QColor("#242424"));
      // /*
      QFont f=painter.font();
      if (errorInLine.contains(blockNumber)) {
	painter.fillRect(0, top,
			lineNumberArea->width(),
			fontMetrics().height(),QColor("#aa0000"));
	iscurrent=true;
      }
      if (comment.at(blockNumber)) {
    painter.fillRect(5, top+2,
			3,//lineNumberArea->width()
            fontMetrics().height()-4,(iscurrent)?QColor("#99aaff"):QColor("#0000aa"));
      }
      
      
      //printf("edidor Font: %s %d %d %d\n",f.family().toStdString().c_str(),f.pointSize(),fontMetrics().height(),fontMetrics().lineSpacing ());
      f.setWeight((iscurrent)?QFont::Black:QFont::Light);
      painter.setFont(f);
      // */
      painter.setPen((iscurrent)?Qt::white:Qt::darkGray);
      painter.drawText(0, top,
		      lineNumberArea->width(), 
                      fontMetrics().lineSpacing () ,
                      Qt::AlignRight, number);
      minbl=qMin(minbl,blockNumber + 1);
      maxbl=qMax(maxbl,blockNumber + 1);
    }
    if (ListFile)
    midCursorpos=(minbl+maxbl)/2;
    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }


  //updateAfix();
  //afixHighlightArea->update();
  event->accept();
}

void CodeEditor::lineNumberToggled(QMouseEvent *event){
    QTextBlock block=firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bn=firstVisibleBlock().blockNumber();
    int idx=(int)  (event->y()-top)/ ((int) blockBoundingRect(block).height()) + bn + 1;
//    printf("remark->  idx-1 %d  event.y %d top %d first %d height %g\n",idx-1,event->y(),top,bn,blockBoundingRect(block).height());
remark(idx-1);
}

void CodeEditor::updateAfix(){
 // printf("updateAfix\n");
  blocks=toPlainText().split('\n');

  /*printf("test die Zeilenanzahl ist jetzt %d %d\n",  
		  blocks.size(),
		  blockCount()
		  );*/
  parenthesis.clear();
  parenthesis2.clear();
  parenthesis3.clear();
  comment.clear();
  int probe=0,aprobe=0,amn=0;
  bool gutso=false;
  bool kommentar=false;
  for (int i=0; i<blocks.size();i++){
    kommentar =  (blocks.at(i).contains(QRegExp("^REM",Qt::CaseInsensitive)));
    comment.append(kommentar);
      if (blocks.at(i).contains(QRegExp("^AFIX\\s+0",Qt::CaseInsensitive))) {
          probe=0;
          amn=0;
      }
    else {
      if (blocks.at(i).contains(QRegExp("^AFIX\\s+\\d+",Qt::CaseInsensitive))) {
	probe++;
	int m=blocks.at(i).section(QRegExp("\\s+"),1,1,QString::SectionSkipEmpty).toInt()/10;
        int n=blocks.at(i).section(QRegExp("\\s+"),1,1,QString::SectionSkipEmpty).toInt()%10;
        if (amn==(m*10+n)) {
            probe--;
            parenthesis.last()=2;
        }
	switch (m){
		case 5:
		case 6:
		case 7:
		case 10:
		case 11:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
			gutso=true;
			break;
	
        }
	if (n==5) {
//	  printf("%d be---\n",probe);
	  probe=(qMax(probe-2,0));
//	  printf("%d----\n",probe);
	}
        amn=m*10+n;
      }
    }
    if (blocks.at(i).contains(QRegExp("^HKLF",Qt::CaseInsensitive))) {probe=aprobe=0;}
    parenthesis.append(qMax(probe,aprobe));
    parenthesis3.append(gutso);
    if (probe==0) gutso=false;
    aprobe=probe;
  }
  probe=0;
  for (int i=0; i<blocks.size();i++){
    if (blocks.at(i).contains(QRegExp("^PART\\s+0",Qt::CaseInsensitive))) probe=0;
    else if (blocks.at(i).contains(QRegExp("^PART\\s+[-0-9]{1,}",Qt::CaseInsensitive)))
      probe=blocks.at(i).section(' ',1,1,QString::SectionSkipEmpty).toInt();
    if (blocks.at(i).contains(QRegExp("^HKLF",Qt::CaseInsensitive))) {probe=0;}
    parenthesis2.append(probe);
  }
  afixHighlightArea->update();
}

void CodeEditor::remark(int line){
  if (isReadOnly ())return;
  QString zeile=blocks.at(line).simplified();
  bool cont= blocks.at(qMax(0,line-1)).contains(QRegExp("=\\s*$"));
  if (zeile.contains(QRegExp("^REM",Qt::CaseInsensitive))) {
    zeile=zeile.remove(0,4);
    if (cont) zeile.prepend(' ');
  }
  else zeile.prepend("REM ");  
  QTextCursor cu=textCursor ();
  cu.movePosition(QTextCursor::Start);
  cu.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,line);
  cu.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cu.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  if (!cu.selectedText().isEmpty()) cu.deleteChar();
  cu.insertText(zeile);
  //qDebug()<<zeile;

}

void CodeEditor::toggleRemarks(){
QTextCursor cu=textCursor();
cu.beginEditBlock();
int start=0,end=0,hilf=0;
QTextBlock block = firstVisibleBlock();
while (block.isValid()){
    if (block.contains(cu.selectionStart())) start= block.blockNumber();
    if (block.contains(cu.selectionEnd())) end= block.blockNumber();		
    block = block.next();
}
hilf=start;
start=qMin(start,end);
end=qMax(hilf,end);
if (end!=start) {
cu.clearSelection();
for (int i=start; i<=end; i++) remark(i);
//qDebug()<<cu.selectedText()<<"\n"<<start<<end;
}
cu.endEditBlock();
cu.clearSelection();
if (!cu.isNull()) setTextCursor(cu);
}

void CodeEditor::afixHighlightAreaPaintEvent(QPaintEvent *event){
//   printf("\nafixHighlightAreaPaintEvent\n");
  QPainter apainter(afixHighlightArea);
  apainter.fillRect(afixHighlightArea->rect(), QColor("#fafaf0"));
  QTextBlock block = firstVisibleBlock();
  QColor color;
  QColor pcolor;
  int blockNumber = block.blockNumber();


  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  //printf ("afix %d %d %d %d \n",top,bottom,block.isVisible(),event->rect().top());
  while (block.isValid() && top <= event->rect().bottom()) {
  switch (parenthesis.at(blockNumber)){ 
  case 0: color=QColor("#fafaf0");
      //          Qt::white;
      break;
  case 1: color=Qt::darkGreen;break;
  case 2: color=QColor("blueviolet");
      //printf("%d\n",parenthesis3.at(blockNumber));
      if (parenthesis3.at(blockNumber)) break;
  default: color=Qt::red;break;
  }
  switch (parenthesis2.at(blockNumber)){ 
  case 0: pcolor=QColor("#fafaf0");
              //Qt::white;
              break;
  case 1: pcolor=QColor("lightskyblue");break;
  case 2: pcolor=QColor("darkgoldenrod");break;
  case 3: pcolor=Qt::green;break;
  case 4: pcolor=Qt::yellow;break;
  case 5: pcolor=QColor("orange");break;
  case 6: pcolor=Qt::black;break;
  case 7: pcolor=QColor("lightsalmon");break;
  case 8: pcolor=QColor("lightseagreen");break;
  case -1: pcolor=QColor("aquamarine");break;
  case -2: pcolor=QColor("olive");break;
  case -3: pcolor=QColor("bisque");break;
  default: if (parenthesis2.at(blockNumber)>0)
          pcolor=QColor::colorNames().at(qMin(parenthesis2.at(blockNumber),QColor::colorNames().size()-1));
      else pcolor=Qt::magenta;
  }

    if (block.isVisible() && bottom >= event->rect().top()) {
    //  printf("%d %d %d %d %d %d\n",0,top, 4,fontMetrics().height(),parenthesis.at(blockNumber),blockNumber);
      apainter.fillRect(5,top, 4,fontMetrics().lineSpacing (),color);
      apainter.fillRect(0,top, 4,fontMetrics().lineSpacing (),pcolor);
     }
    block = block.next();
    blockNumber = block.blockNumber();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
  } 
  event->accept();
}

bool CodeEditor::viewportEvent(QEvent *event){
//  printf("lala %d\n",event->type());
if (!isVisible()) {event->ignore(); return false;}
if (event->type() == QEvent::ToolTip) {
  QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
  QTextCursor cursor = cursorForPosition( helpEvent->pos());
  if(cursor.block().text().isEmpty()) return true;
  QStringList block=shxToolTips.filter(cursor.block().text().left(4),Qt::CaseInsensitive);
  if (block.size())
    QToolTip::showText(helpEvent->globalPos(),block.first());
  int lnr=cursor.block().blockNumber()+1;
  QRegExp re=QRegExp("[-]*\\d{2,}\\.\\d+");
  double fvar=6666.6666;
  QString fvs;
  QString msg;
  int pos=0,len=0;
  if ((block.isEmpty())&&(!highlighter->outputStyle())&&(cursor.block().blockNumber() > fvMinLine)){
    while (-1 != (pos=re.indexIn(cursor.block().text(),pos+len+1))){
      fvs=re.cap(0);
      len=fvs.size();
      fvar=fvs.toDouble();
      if (fvar!=6666.6666) {
        double av=qAbs(fvar),res=0.0,var=1.0;
        int m=0;
        while ((-10*m+av) > 5){m++;}
        if ((m>1)&&(m<=fv.size())) var = fv.at(m-1);
        if (!m) res = fvar;
        else if (fvar>0) res = (av-(10*m))*var;
        else res = (av-(10*m))*(1.0-var);
        //qDebug()<<v<<res<<m;
        // std::cout<< av <<"abs  v"<< v <<" m ="<< m << " var =" << var <<" res = "<<res<< std::endl ;
        //return res;
        if ((m)>fv.size()){
          if (m>1) msg+=QString("<font color=\"red\">Error in line %2!<br>Free Variable %1 is not defined yet!</font><hr>")
          .arg(m).arg(lnr);}
        else
          msg+=QString("Line <b>%3</b>: Free Variable <b>%1</b> gives <b> %2</b> here.<hr>")
               .arg(m).arg(res).arg(lnr);
      }
    }
    QToolTip::showText(helpEvent->globalPos(),msg);
    //  printf("shxToolTips size+%d %d %f\n", shxToolTips.size(),block.size(),fvar);
  }
  event->accept();
  return true;
}
return QAbstractScrollArea::viewportEvent ( event);
}

void  CodeEditor::contextMenuEvent(QContextMenuEvent *event) {
     //QMenu *menu = new QMenu();//createStandardContextMenu();
     QMenu *menu = createStandardContextMenu();
     QAction *a;
     QTextCursor cursor = cursorForPosition( event->pos());
     QTextBlock block=cursor.block();
     QString aname=block.text().section(" ",0,0);
     QStringList bfl=shxToolTips.filter(QRegExp(QString("^%1\\b").arg(aname)));
     if (bfl.isEmpty()){
        QString data=block.text().remove("=").trimmed();                                      // findInStructure
        a=menu->addAction(QString("locate %1 in structure").arg(aname) ,this,SLOT(searchInStructure()));
        a->setData(data);
     }//else {qDebug()<<bfl;}

     if (textCursor().hasSelection()){
         a=menu->addAction(QString("select atoms in structure found in selected text"),
                         this, SLOT(selectInStructure()));
         a->setData(textCursor().selectedText());
	 a=menu->addAction(QString("toggle remarks states of selected text"),
			 this,SLOT(toggleRemarks()));
     }
     //...
     menu->exec(event->globalPos());
     delete menu;

     event->accept();
 }

void CodeEditor::diffTo(QString org){
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    QStringList orgZ = org.split('\n');
    QColor lineColor = QColor("#ffddaa");
    setReadOnly(false);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    selection.cursor.movePosition(QTextCursor::Start);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    QTextBlock b = selection.cursor.block();
    int i=0;
    while (b.isValid()){
        if (b.text()!=orgZ.at(i)){
           /* printf(">>%s!\n<<%s!%d\n",
                   b.text().toStdString().c_str(),
                   orgZ.at(i).toStdString().c_str(),b.position()
                   );// */
            selection.cursor.clearSelection();
            selection.cursor.setPosition(b.position());
            selection.cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
            selection.cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::MoveAnchor);

            extraSelections.append(selection);
        }
        b=b.next();
        int j=orgZ.indexOf(b.text(),i);
        i=(j<0)?i+1:j;
        i=qMin(i,orgZ.size()-1);
    }
    setExtraSelections(extraSelections);
    setReadOnly(true);
    update();
}

void CodeEditor::searchInStructure(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        emit findInStructure(action->data().toString());

}

void CodeEditor::selectInStructure(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        emit electInStructure(action->data().toString());

}

void CodeEditor::setCompleter(QCompleter *completer){
    //printf("setCompleter %p %p\n",c,completer);
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    //c->setMaxVisibleItems(13);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

QCompleter *CodeEditor::completer() const{
    return c;
}

void CodeEditor::insertCompletion(const QString& completion){
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    tc.joinPreviousEditBlock ();
    //int extra = completion.length() - c->completionPrefix().length();
    //tc.movePosition(QTextCursor::Left);
//    int p=tc.selectionStart()- tc.block().position();
    tc.movePosition(QTextCursor::EndOfWord,QTextCursor::MoveAnchor);
    tc.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
    QString extra = tc.selectedText();
    //extra.remove(0,1);
    extra.remove(QRegExp("^[a-z]+"));
    if ((completion!="REM "))extra.clear();
    tc.deleteChar();
    tc.insertText(completion+" "+extra);
    tc.endEditBlock();
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    if (tc.selectedText().contains("_")) return "";
    int si=tc.selectedText().size();
    //if (tc.selectedText().startsWith('r',Qt::CaseSensitive))
    //    return tc.selectedText().section(QRegExp("[A-Z]+",Qt::CaseSensitive),0,0).toUpper();
    //    printf("%d %d\n", (tc.selectionStart ()  - tc.block().position()) ,si);
    if (((tc.selectionStart () - tc.block().position())<4)&&(si>4)) return tc.selectedText().left(si-4);
    if ((tc.position() - tc.block().position())>2) return "";
    return tc.selectedText();
}

void CodeEditor::focusInEvent(QFocusEvent *e){

//    printf("focusInEvent %p \n",c);
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
    e->accept();
    //e->ignore();
    //setFocus(Qt::MouseFocusReason);
}

void CodeEditor::keyPressEvent(QKeyEvent *e){
//    printf("kp\n");
//    if (!chgl->mol->selectedatoms.isEmpty()){
    if ((e->modifiers() & Qt::ControlModifier)&&(e->key()==Qt::Key_Delete)) {
      	e->ignore();
        emit deleteSelected();
	return;
    }
    //}
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
      // case Qt::Key_Left:
      // case Qt::Key_Right:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

   // bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
   // if (!c) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("!"); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();
    if ((hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.moveRight(lineNumberAreaWidth()+11);;
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
