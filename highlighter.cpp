 /****************************************************************************
 **
 ** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://trolltech.com/products/qt/licenses/licensing/opensource/
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
 ** or contact the sales department at sales@trolltech.com.
 **
 ** In addition, as a special exception, Trolltech gives you certain
 ** additional rights. These rights are described in the Trolltech GPL
 ** Exception version 1.0, which can be found at
 ** http://www.trolltech.com/products/qt/gplexception/ and in the file
 ** GPL_EXCEPTION.txt in this package.
 **
 ** In addition, as a special exception, Trolltech, as the sole copyright
 ** holder for Qt Designer, grants users of the Qt/Eclipse Integration
 ** plug-in the right for the Qt/Eclipse Integration to link to
 ** functionality provided by Qt Designer and its related libraries.
 **
 ** Trolltech reserves all rights not expressly granted herein.
 **
 ** Trolltech ASA (c) 2007
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#include <QtGui>

#include "highlighter.h"

 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent){
 }

 Highlighter::Highlighter(QString FileName, QTextDocument *parent)
     : QSyntaxHighlighter(parent){
   //    setEditStyle();
       loadHighlightPatterns(FileName);
 }
/*
 void Highlighter::setEditStyle(){
  outputstyle=false;
  HighlightingRule rule;
  
  keywordFormat.setForeground(Qt::darkRed);
  keywordFormat.setFontWeight(QFont::Bold);
  keywordFormat.setBackground(QColor("#aaffaa"));
/ *  QStringList keywordPatterns;
  keywordPatterns <<
   "ACTA"<<"AFIX"<<"AGS4"<<"ANIS"<<"BASF"<<"BIND"<<"BLOC"<<"BOND"<<
   "BUMP"<<"CELL"<<"CGLS"<<"CHIV"<<"CONF"<<"CONN"<<"DAMP"<<"DANG"<<
   "DEFS"<<"DELU"<<"DFIX"<<"DISP"<<"EADP"<<"EGEN"<<"END" <<"EQIV"<<
   "ESEL"<<"EXTI"<<"EXYZ"<<"FEND"<<"FLAT"<<"FMAP"<<"FRAG"<<"FREE"<<
   "FVAR"<<"GRID"<<"HFIX"<<"HKLF"<<"HOPE"<<"HTAB"<<"INIT"<<"ISOR"<<
   "LAST"<<"LATT"<<"LAUE"<<"LIST"<<"MERG"<<"MOLE"<<"MORE"<<"MPLA"<<
   "MOVE"<<"NCSY"<<"OMIT"<<"PART"<<"PATT"<<"PHAN"<<"PHAS"<<"PLAN"<<
   "PSEE"<<"REM"<< "RESI"<<"RTAB"<<"SADI"<<"SAME"<<"SFAC"<<"SHEL"<<
   "SIMU"<<"SIZE"<<"SPEC"<<"SPIN"<<"STIR"<<"SUMP"<<"SWAT"<<"SYMM"<<
   "TEMP"<<"TEXP"<<"TIME"<<"TITL"<<"TREF"<<"TWIN"<<"UNIT"<<"VECT"<<
   "WPDB"<<"WGHT"<<"ZERR"<<"XNPD"<<"REST"<<"CHAN"<<"RIGU"<<"FLAP"<<
   "RNUM"<<"SOCC"<<"PRIG"<<"WIGL"<<"RANG"<<"TANG"<<
   "ADDA"<<
   "STAG"<<
   "ATOM"<<//PDB dummy commands ...
   "HETA"<<//PDB dummy commands ...
   "SCAL"<<//PDB dummy commands ...
   "ABIN"<<
   "ANSC"<<
   "ANSR"<<
   "NOTR"<<
   "NEUT"<<
   "TWST";//<<
   
   ;
  foreach (QString pattern, keywordPatterns) {
    pattern.prepend("^");
    pattern.append("\\b");
    rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
    pattern.remove("\\b");
    pattern.append("_\\*\\B");
    rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
    pattern.remove("_\\*\\B");
    pattern.append("_\\S*\\b");
    rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
    rule.format = keywordFormat;
    highlightingRules.append(rule);

  }// * /
  QString pattern = 
    "(^)(ACTA)|(^)(AFIX)|(^)(MPLA)|(^)(ANIS)|(^)(BASF)|(^)(BIND)|(^)(BLOC)|"
    "(^)(BOND)|(^)(BUMP)|(^)(CELL)|(^)(CGLS)|(^)(CHIV)|(^)(CONF)|(^)(CONN)|"
    "(^)(DAMP)|(^)(DANG)|(^)(DEFS)|(^)(DELU)|(^)(DFIX)|(^)(DISP)|(^)(EADP)|"
     "(^)(END)|(^)(EQIV)|(^)(EXTI)|(^)(EXYZ)|"
    "(^)(FLAT)|(^)(FMAP)|(^)(FRAG)|(^)(FREE)|(^)(FVAR)|(^)(GRID)|(^)(HFIX)|"
    "(^)(HKLF)|(^)(HOPE)|(^)(HTAB)|(^)(ISOR)|(^)(LAST)|(^)(LATT)|"
    "(^)(LAUE)|(^)(LIST)|(^)(MERG)|(^)(MOLE)|(^)(MORE)|(^)(MOVE)|(^)(L\\.S\\.)|"
    "(^)(NCSY)|(^)(OMIT)|(^)(PART)|(^)(PLAN)|"
    "(^)(RESI)|(^)(RTAB)|(^)(SADI)|(^)(SAME)|(^)(SFAC)|"
    "(^)(SHEL)|(^)(SIMU)|(^)(SIZE)|(^)(SPEC)|(^)(STIR)|(^)(SUMP)|"
    "(^)(SWAT)|(^)(SYMM)|(^)(TEMP)|(^)(TIME)|"
    "(^)(TWIN)|(^)(UNIT)|(^)(WPDB)|(^)(WGHT)|(^)(ZERR)";
  rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
  rule.format = keywordFormat;
  highlightingRules.append(rule);

  pattern= "(^)(EGEN)|(^)(PHAN)|(^)(PHAS)|(^)(SPIN)|(^)(TEXP)|"
      "(^)(VECT)|(^)(PSEE)|(^)(TREF)|(^)(PATT)|(^)(ESEL)|(^)(INIT)";
      
  rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
  rule.format = keywordFormat;
  highlightingRules.append(rule);
  pattern="(^)(XNPD)|"
      "(^)(REST)|(^)(CHAN)|(^)(RIGU)|(^)(FLAP)|(^)(RNUM)|(^)(SOCC)|(^)(PRIG)|"
      "(^)(WIGL)|(^)(RANG)|(^)(TANG)|(^)(ADDA)|(^)(STAG)|(^)(ATOM)|(^)(HETA)|"
      "(^)(SCAL)|(^)(ABIN)|(^)(ANSC)|(^)(ANSR)|(^)(NOTR)|(^)(NEUT)|(^)(TWST)"; 

  rule.pattern = QRegExp(pattern,Qt::CaseInsensitive);
  rule.format = keywordFormat;
  highlightingRules.append(rule);
/ *
  keybitsFormat.setFontWeight(QFont::Black);
  keybitsFormat.setForeground(Qt::white);
  keybitsFormat.setBackground(Qt::darkGray);
  rule.pattern =  QRegExp("\\b[01]{3,3} [01]{6,6} [01]{10,10} [01]{15,15} [01]{2,2} [01]{3,3} [01]{5,5} [01]{7,7} [01]{9,9}");
  rule.format = keybitsFormat;
  highlightingRules.append(rule);
  * /
  / *
  classFormat.setBackground(QColor("#aaffee"));
  rule.pattern = QRegExp(" =$");
  rule.format = classFormat;
  highlightingRules.append(rule);
  rule.pattern = QRegExp("^    ");
  rule.format = classFormat;
  highlightingRules.append(rule);
  * /
/ *
  QTextCharFormat xyzFormat;
  xyzFormat.setFontWeight(QFont::Bold);
  xyzFormat.setForeground(QColor(128,128,255));
  xyzFormat.setBackground(Qt::darkBlue);
  rule.pattern = QRegExp("\\bX\\b");
  rule.format = xyzFormat;
  highlightingRules.append(rule);
  xyzFormat.setForeground(QColor(255,128,128));
  xyzFormat.setBackground(Qt::darkRed);
  rule.pattern = QRegExp("\\bY\\b");
  rule.format = xyzFormat;
  highlightingRules.append(rule);
  xyzFormat.setForeground(QColor(128,255,128));
  xyzFormat.setBackground(Qt::darkGreen);
  rule.pattern = QRegExp("\\bZ\\b");
  rule.format = xyzFormat;
  highlightingRules.append(rule);
* /


  numberFormat.setFontWeight(QFont::Bold);
  numberFormat.setForeground(Qt::darkCyan);
  numberFormat.setFontOverline(true);
  numberFormat.setFontUnderline(true);
  numberFormat.setUnderlineColor(Qt::darkCyan);
  numberFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
  rule.pattern =  QRegExp("\\b[0-9]{2,}\\.[0-9]+\\b");
  rule.format = numberFormat;
  highlightingRules.append(rule);

  includeFormat.setForeground(Qt::yellow);
  includeFormat.setBackground(Qt::darkGreen);
  includeFormat.setFontWeight(QFont::Black);
  //includeFormat;
  rule.pattern =  QRegExp("^\\+.+\\b");
  rule.format = includeFormat;
  highlightingRules.append(rule);

 // das hier ist um die free variables von oben bei  Cell nicht gelten zu lassen
  cellFormat.setFontItalic(false);
  cellFormat.setFontOverline(false);
  cellFormat.setFontUnderline(false);
  cellFormat.setBackground(Qt::white);
  cellFormat.setForeground(Qt::black);  
  cellFormat.setFontWeight(QFont::Bold);
  rule.pattern = QRegExp("^CELL\\s+\\d+.\\d+\\s+\\d+.\\d+\\s+\\d+.\\d+\\s+\\d+.\\d+\\s+\\d+.\\d+\\s+\\d+.\\d+\\s+\\d+.\\d+",Qt::CaseInsensitive);
  rule.format = cellFormat;
  highlightingRules.append(rule);

  cellFormat.setForeground(Qt::darkRed);
  cellFormat.setBackground(QColor("#aaffaa"));
  rule.pattern = QRegExp("^CELL\\b",Qt::CaseInsensitive);
  rule.format = cellFormat;
  highlightingRules.append(rule);

  cellFormat.setFontItalic(false);
  cellFormat.setFontOverline(false);
  cellFormat.setFontUnderline(false);
  cellFormat.setBackground(Qt::white);

  //cellFormat.setBackground(Qt::black);
  cellFormat.setForeground(Qt::black);
  rule.pattern = QRegExp("^(UNIT|TEMP|ZERR).+$");
  rule.format = cellFormat;
  highlightingRules.append(rule);

  cellFormat.setForeground(Qt::darkRed);
  cellFormat.setBackground(QColor("#aaffaa"));
  rule.pattern = QRegExp("^(UNIT|TEMP|ZERR)\\b",Qt::CaseInsensitive);
  rule.format = cellFormat;
  highlightingRules.append(rule);



  singleLineCommentFormat.setForeground(Qt::blue);
  rule.pattern = QRegExp("^TITL [^\n]*",Qt::CaseInsensitive);
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);
  rule.pattern = QRegExp("^REM [^\n]*",Qt::CaseInsensitive);
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);
  rule.pattern = QRegExp("![^\n]*");
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);
  
  quotationFormat.setForeground(Qt::darkBlue);
  quotationFormat.setBackground(QColor("#eeeeee"));
  quotationFormat.setFontUnderline(true);
  rule.pattern = QRegExp("^\\s+\\S+[^\n\r]*");
  rule.format = quotationFormat;
  highlightingRules.append(rule);
  
  toolongFormat.setFontWeight(QFont::Black);
  toolongFormat.setForeground(Qt::yellow);
  toolongFormat.setFontUnderline(true);
  toolongFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  toolongFormat.setUnderlineColor(Qt::red);
  toolongFormat.setBackground(QColor(Qt::red).lighter(100));
  rule.pattern =  QRegExp("^[^\r\n]{81,}");
  rule.format = toolongFormat;
  highlightingRules.append(rule);

  multiLineCommentFormat.setBackground(QColor("#eefffe"));
  multiLineCommentFormat.setForeground(Qt::black);
  multiLineCommentFormat.setFontUnderline(false);
  commentStartExpression = QRegExp("=\\s*$");
  //commentEndExpression = QRegExp("^ [^=\r\n]*");
  commentEndExpression = QRegExp("^ [^=]*$");
}

void Highlighter::setOutputStyle(){
  QColor white=QColor(255,255,230);
  QTextCharFormat realFormat;
  realFormat.setForeground(Qt::red);
  realFormat.setBackground(white);

  QTextCharFormat labelFormat;
  labelFormat.setForeground(Qt::black);
  labelFormat.setBackground(Qt::lightGray);

  QTextCharFormat plusFormat;
  plusFormat.setForeground(white);
  plusFormat.setBackground(Qt::darkBlue);
  //plusFormat.setFontPointSize(7);

  QTextCharFormat r1Format;
  r1Format.setFontUnderline(true);
  r1Format.setUnderlineColor(Qt::darkBlue);
  r1Format.setForeground(Qt::blue);
  r1Format.setBackground(Qt::yellow);

  QTextCharFormat intFormat;
  intFormat.setForeground(Qt::blue);
  intFormat.setBackground(white);
  HighlightingRule rule;
  highlightingRules.clear();
  rule.pattern =  QRegExp("\\b[0-9]+\\b");
  rule.format = intFormat;
  highlightingRules.append(rule);
  rule.pattern =  QRegExp("\\b[0-9]+\\.[0-9]+\\b");
  rule.format = realFormat;
  highlightingRules.append(rule);
  rule.pattern =  QRegExp("\\b[A-Z]+[0-9]+[_0-9A-Z]*\\b",Qt::CaseInsensitive);
  rule.format = labelFormat;
  highlightingRules.append(rule);
  rule.pattern = QRegExp("[+]{2,}");
  rule.format = plusFormat;
  highlightingRules.append(rule);
  rule.pattern = QRegExp(" R1 =\\s+\\b[0-9]+\\.[0-9]+\\b",Qt::CaseInsensitive);
  rule.format = r1Format;
  highlightingRules.append(rule);
  
  staredFormat.setFontUnderline (true);
  staredFormat.setFontWeight(QFont::Black);
  staredFormat.setForeground(Qt::yellow);
  staredFormat.setBackground(Qt::red);
  rule.pattern =  QRegExp("\\*\\* [^*]*\\*\\*");
  rule.format = staredFormat;
  highlightingRules.append(rule);

  outputstyle=true; 
}
*/
void Highlighter::highlightSearch(const QString &find){
  HighlightingRule rule;
  rule.format.setBackground(Qt::yellow);
  rule.format.setFontWeight(QFont::Black);//
  findRule.clear();
  QString f=find;
  f.replace(".*",".+");
  if ((f=="^")||(f=="$"))return;
  if (!f.isEmpty()){
  rule.pattern = QRegExp(f,Qt::CaseInsensitive);

  findRule.append( rule);
  }
//  printf("suchSuch\n");
  rehighlight();
}

void Highlighter::highlightError(const QStringList fnd){
  findRule.clear();
  HighlightingRule rule;
  rule.format.setFontWeight(QFont::Black);
  rule.format.setForeground(Qt::yellow);
  rule.format.setFontUnderline(true);
  rule.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  rule.format.setUnderlineColor(Qt::red);
  rule.format.setBackground(QColor(Qt::red).lighter(100));
  QString f="";
  for (int i=0;i<fnd.size();i++){
      f=fnd.at(i);
      if (!f.isEmpty()){
	rule.pattern = QRegExp(f,Qt::CaseInsensitive,QRegExp::FixedString);

	findRule.append( rule);
      }
  }
//  printf("suchSuch\n");
  rehighlight();
}

void Highlighter::highlightDupl(const CEnvironment &dupls){
    findRule.clear();
    if (dupls.size()==0)return;
  HighlightingRule rule;
  rule.format.setBackground(QColor("orange"));
  rule.format.setForeground(Qt::blue);
  rule.format.setFontWeight(QFont::Black);//
  findRule.clear();
  QString f="";
  for (int i=0;i<dupls.size();i++){
      f=dupls.at(i).orginalLine;
  if (!f.isEmpty()){
  rule.pattern = QRegExp(f,Qt::CaseInsensitive);

  findRule.append( rule);
  }
  }
//  printf("suchSuch\n");
  rehighlight();
}

void Highlighter::highlightBlock(const QString &text) {
  foreach (HighlightingRule rule, highlightingRules) {
    if (rule.Comment) continue;
    QRegExp expression(rule.pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(expression, index + length);
    }
  }
  setCurrentBlockState(0);

  if (!outputstyle) {
  int startIndex = 0;
  if (previousBlockState() != 1)
    startIndex = text.indexOf(commentStartExpression);

  QTextCharFormat fmt = format(startIndex);
  if (fmt.foreground()==Qt::blue) startIndex=-1;
  while (startIndex >= 0) {
    int endIndex = text.indexOf(commentEndExpression, startIndex);
    int commentLength;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat);
    startIndex = text.indexOf(commentStartExpression,
			      startIndex + commentLength);
    fmt = format(startIndex);
    if (fmt.foreground()==Qt::blue) startIndex=-1;
    if (fmt.underlineStyle()) startIndex=-1;
  }
  foreach (HighlightingRule rule, highlightingRules) {
    if (!rule.Comment) continue;
    QRegExp expression(rule.pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(expression, index + length);
     // printf("in editor height = %d\n",QFontMetrics(rule.format.font()).height());
    }
  }
  }
/*
  rule.pattern =  QRegExp("\\b[0-9]{2,}\\.[0-9]+\\b");
  rule.format = numberFormat;

  expression = QRegExp(rule.pattern);
  index = text.indexOf(expression);
  while (index >= 0) {
    int length = expression.matchedLength();
    setFormat(index, length, rule.format);
    index = text.indexOf(expression, index + length);
  } */
  foreach (HighlightingRule rule, findRule) {
    QRegExp expression(rule.pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(expression, index + length);
    }
  }
//  printf("hi blo\n");
}

void Highlighter::loadHighlightPatterns(QString FileName){
  //int group=1;  
  highlightingRules.clear();
  outputstyle=true;
  //  fprintf(stderr,"loadHighlightPatterns %s\n",FileName.toStdString().c_str());
  QSettings *pat = new QSettings(FileName,QSettings::IniFormat);
  /*foreach(HighlightingRule rule, highlightingRules){  
    pat->beginGroup(QString("%1").arg(group));
    pat->setValue("Pattern"     ,rule.pattern.pattern());
    pat->setValue("Color"           ,rule.format.foreground().color().name());
    pat->setValue("Background"      ,rule.format.background().color().name());
    pat->setValue("Weight"          ,rule.format.fontWeight());
    pat->setValue("Underlined"      ,rule.format.fontUnderline());
    pat->setValue("UnderlineStyle"  ,rule.format.underlineStyle());
    pat->setValue("UnderlineColor"  ,rule.format.underlineColor().name());
    pat->setValue("Overlined"       ,rule.format.fontOverline());
    pat->setValue("CaseSensitive"   ,rule.pattern.caseSensitivity()); 
    pat->endGroup();
    group++;
    }*/
  HighlightingRule rule;
  QStringList groups = pat->childGroups();
  foreach (QString grp,groups){
    if (!grp.contains(QRegExp("\\D+"))){
      pat->beginGroup(grp);
      rule.pattern.setPattern(pat->value("Pattern","").toString());
      rule.format.setForeground(QColor(pat->value("Color"           ,"#000000").toString()));
      rule.format.setBackground(QColor(pat->value("Background"      ,"#ffffff").toString()));
      rule.format.setFontWeight(pat->value("Weight"          ,50).toInt());
      rule.format.setFontUnderline(pat->value("Underlined"      ,"false").toBool());
      rule.format.setUnderlineStyle((QTextCharFormat::UnderlineStyle) pat->value("UnderlineStyle"  ,"1").toInt());
      rule.format.setUnderlineColor(QColor(pat->value("UnderlineColor"  ,rule.format.foreground().color().name()).toString()));
      rule.format.setFontOverline(pat->value("Overlined"       ,"false").toBool());
      rule.Comment=pat->value("Comment","false").toBool();
      rule.pattern.setCaseSensitivity((Qt::CaseSensitivity)pat->value("CaseSensitive"   ,0).toInt());
      pat->endGroup();
      highlightingRules.append(rule);
    }else{
      pat->beginGroup(grp);
      multiLineCommentFormat.setForeground(QColor(pat->value("MultilineColor" ,"#000000").toString()));
      multiLineCommentFormat.setBackground(QColor(pat->value("MultilineBackground" ,"#eefffe").toString()));
      multiLineCommentFormat.setFontUnderline(pat->value("MultilineUnderlined"      ,"false").toBool());
      commentStartExpression.setPattern(pat->value("MultilineStart","=\\s*$").toString());
      commentEndExpression.setPattern(pat->value("MultilineiEnd","^ [^=]*$").toString());
      outputstyle=false;
    }
  }
  //fprintf(stderr,"finished loadHighlightPatterns %s outputstyle=%s\n",FileName.toStdString().c_str(),(outputstyle)?"true":"false");
}
