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
//äöüß needed to keep this file UTF8 QString::fromUtf8("»")
int Revision=999;
#include "window.h"
//QString datum=QString(__TIME__ " " __DATE__);
QString latestRev="no network available, or you disabled checking";
QString lstpatternfile=":lst_and_output_style.ini";
QString inspatternfile=":ins_res_style.ini";

void Window::dragEnterEvent(QDragEnterEvent *event) {
     if (event->mimeData()->hasFormat("text/uri-list"))
         event->acceptProposedAction();
}


void Window::dropEvent(QDropEvent *event) {
     QList<QUrl> drops = event->mimeData()->urls();
     QString path=drops.first().path();
#if defined Q_WS_WIN
     path.remove(QRegExp("^/"));
#endif
     if ((path.endsWith(".res",Qt::CaseInsensitive))||(path.endsWith(".ins",Qt::CaseInsensitive))){
         loadFile(path);
         event->acceptProposedAction();
     }
}
void Window::qinitres() {
	  Q_INIT_RESOURCE(icons);
  }
Window::Window(){
  /*! \brief the constructor of the main window rather lengthy but most widgets are created and placed here. 
   *
   */
    setAcceptDrops(true);
    falschAtom=-1;
    calcmenu=NULL;//if not needed it will be not created.
  doNotAnnoyMe=false;
  hhh  =  qApp->desktop()->availableGeometry(-1).height();
  shxPath=shxOptions=NULL;
  machPlatz=false;
  char *ENUS=setlocale(LC_ALL,"C");
  if (!ENUS)ENUS=setlocale(LC_ALL,"en_US.UTF-8"); 
  //printf("%s\n",ENUS);
  map_radius=5.0;
  maptrunc = 1;
  FFTErrorString="";
  range=3.0;
  mode=0;
  weight=1.0;
  //scroller=false;
  setDockNestingEnabled(true);
  otherOpenFiles=false;
  /*  try no help
  helpDock = new QDockWidget("ShelXle help",this);
  helpDock->setObjectName("Help");

  helpDock->setAllowedAreas(Qt::AllDockWidgetAreas);
  hilfe = new QWebView(helpDock);
#if defined(linux)
  //hilfe->load(QUrl("/usr/share/shelxle/help/index.html"));
  {
    QFile fle(QString("/usr/share/shelxle/index.html"));

    if (!fle.open(QIODevice::ReadOnly)){
      QMessageBox::information(this, tr("Unable to open file"),
		      fle.errorString());
    }

    QTextStream out(&fle);
    QString output = out.readAll();

    // display contents
    hilfe->setHtml(output,QUrl::fromLocalFile("/usr/share/shelxle/index.html"));
  }
#elif defined Q_WS_WIN
  {
   QFile fle(QString("%1/help/index.html").arg(qApp->applicationDirPath()));

     if (fle.open(QIODevice::ReadOnly)){
     QTextStream out(&fle);
     QString output = out.readAll();

            // display contents
     hilfe->setHtml(output,QUrl::fromLocalFile(QString("%1/help/index.html").arg(qApp->applicationDirPath())));
   }
     }
#else
  //QUrl(QUrl::fromLocalFile(QString("%1/help/index.html").arg(qApp->applicationDirPath())));
 {
  //qDebug()<<QString("/%1/help/index.html").arg(qApp->applicationDirPath());
  QFile fle(QString("/%1/help/index.html").arg(qApp->applicationDirPath()));

    if (!fle.open(QIODevice::ReadOnly)){
    //    QMessageBox::information(this, tr("Unable to open file"),
    //    fle.errorString());
    //    return;
    }
    else{
	    QTextStream out(&fle);
	    QString output = out.readAll();

	    // display contents
	    hilfe->setHtml(output,QUrl::fromLocalFile(QString("%1/help/index.html").arg(qApp->applicationDirPath())));
    }
  }
#endif
  helpDock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
  helpDock->setWidget(hilfe);
  helpDock->hide();
  addDockWidget(Qt::RightDockWidgetArea, helpDock);
  // */
  {//initialize Edidor
    navibox = new QGroupBox(this);

    nlt = new QHBoxLayout(navibox);  
    nlt->setMargin(0);
    resiFinder = new QComboBox(navibox);
    selectPart = new QCheckBox("Select Part",navibox);
    selectPart->setChecked(false);
    spinSelectPart = new QSpinBox(navibox);
    spinSelectPart->setMinimum(1);
    spinSelectPart->setMaximum(2);
    spinSelectPart->setVisible(false);
    inculdePartNull = new QCheckBox("incl. Pt. 0",navibox);
    inculdePartNull->setToolTip("Include part 0 in selection");
    inculdePartNull->setChecked(true);
    inculdePartNull->setVisible(false);
    QToolButton *nothing=new QToolButton(navibox);
    nothing->setIcon(QIcon(":/icons/nospell.png"));
    nothing->setText("no errors");
    nothing->setIconSize(QSize(22,22));
    nothing->setToolButtonStyle(Qt::ToolButtonIconOnly);

    spellCheckact = new QToolButton(navibox);
    spellCheckact->setIcon(QIcon(":/icons/spell.png"));
    spellCheckact->setText("Jump to error");
    spellCheckact->setIconSize(QSize(22,22));
    spellCheckact->setToolTip("Jump to first error in file.");
    spellCheckact->setToolButtonStyle(Qt::ToolButtonIconOnly);
    connect(spellCheckact,SIGNAL(clicked()),this,SLOT(jumpToError()));
    lineColumn = new QLabel("",navibox);
    nlt->addWidget(selectPart);
    nlt->addWidget(spinSelectPart);
    nlt->addWidget(inculdePartNull);
    nlt->addWidget(resiFinder);
    nlt->addWidget(spellCheckact);
    nlt->addWidget(nothing);
    connect(selectPart,SIGNAL(toggled(bool)),spinSelectPart,SLOT(setVisible(bool)));
    connect(selectPart,SIGNAL(toggled(bool)),inculdePartNull,SLOT(setVisible(bool)));
    connect(selectPart,SIGNAL(stateChanged(int)),this,SLOT(selectParts()));
    connect(inculdePartNull,SIGNAL(toggled(bool)),this,SLOT(selectParts()));
    connect(spinSelectPart,SIGNAL(valueChanged(int)),this,SLOT(selectParts()));
    nlt->addStretch(10);
    nlt->addWidget(lineColumn);
    navibox->setLayout(nlt);
    editor = new CodeEditor(this);
    editor->highlighter->loadHighlightPatterns(inspatternfile);
    /*QUndoStack *ust = new QUndoStack(editor);
    ust->setUndoLimit(5);
    qDebug()<<ust->undoLimit();*/
    connect(resiFinder,SIGNAL(activated(QString)),this,SLOT(findResi(QString)));
    connect(resiFinder,SIGNAL(destroyed(QObject*)),this,SLOT(resiFinderDestroyed()));
    connect(editor,SIGNAL(textChanged ()),this,SLOT(spellCheck())); 
    connect(editor,SIGNAL(cursorPositionChanged ()),this,SLOT(updateLC()));
    connect(editor,SIGNAL(findInStructure(const QString&)),this,SLOT(findInStructure(const QString &)));
    connect(editor,SIGNAL(electInStructure(const QString&)),this,SLOT(selectInStructure(const QString &)));
    connect(editor,SIGNAL(textChanged()),this,SLOT(updateSelectResi()));
    connect(editor,SIGNAL(cursorPositionChanged()),this,SLOT(resiSelectorCheck()));
    completer = new QCompleter(this);
    {
      QStringList keywords;
      keywords << "ACTA"<<"AFIX"<<"ANIS"<<"BASF"<<"BIND"<<"BLOC"<<"BOND"<<
	      "BUMP"<<"CELL"<<"CGLS"<<"CHIV"<<"CONF"<<"CONN"<<"DAMP"<<"DANG"<<
	      "DEFS"<<"DELU"<<"DFIX"<<"DISP"<<"EADP"<<"END "<<"EQIV"<<
	      "EXTI"<<"EXYZ"<<"FEND"<<"FLAT"<<"FMAP"<<"FRAG"<<"FREE"<<
	      "FVAR"<<"GRID"<<"HFIX"<<"HKLF"<<"HOPE"<<"HTAB"<<"ISOR"<<
	      "LATT"<<"LAUE"<<"LIST"<<"L.S."<<"MERG"<<"MOLE"<<
	      "MORE"<<"MOVE"<<"MPLA"<<"NCSY"<<"OMIT"<<"PART"<<"PLAN"<<
	      "REM "<<"RESI"<<"RTAB"<<"SADI"<<"SAME"<<"SFAC"<<"SHEL"<<
	      "SIMU"<<"SIZE"<<"SPEC"<<"STIR"<<"SUMP"<<"SWAT"<<"SYMM"<<
	      "TEMP"<<"TIME"<<"TITL"<<"TWIN"<<"UNIT"<<"VECT"<<
              "WPDB"<<"WGHT"<<"ZERR"<<
      // New Shelxl commands!
                  "XNPD"<<//83
                  "REST"<<//84
                  "CHAN"<<//85
                  "RIGU"<<//86
		  "FLAP"<<//87
		  "RNUM"<<//88
		  "SOCC"<<//89
		  "PRIG"<<//90
		  "WIGL"<<//91
		  "RANG"<<//92
		  "TANG"<<//93
		  "ADDA"<<//94
		  "STAG"<<//;//95
      "ABIN"<<//96
      "ANSC"<<//97
      "ANSR"<<//98
      "NOTR"<<//99
      "NEUT"<<//100
      "TWST";//<<101
      // */
	  /*      "ATOM"<<
		  "HETATM"<<
		  "ANISOU"<<
		  "SCALE1"<<
		  "SCALE2"<<
		  "SCALE3"<<*/
/*		  "WATR"<<
		  ""<<
		  ""<<
		  ""<<
		  ""<<
		  ""<<
		  
		  ; //86*/
     keywords.sort();

      completer->setModel(new QStringListModel(keywords, completer));
    }
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    editor->setCompleter(completer);

    suchbox =  new QGroupBox("Search and Replace",this);
    search = new QLineEdit(suchbox);
    search->setMinimumSize(QSize(150, 0));
    suchbox->setFlat(true);
    slt=new QGridLayout(suchbox);
    hidesearch = new QToolButton(suchbox);
    hidesearch->setIcon(QIcon(":/icons/cancel.png"));
    hidesearch->setAutoRaise(true);
    prev = new QToolButton(suchbox);
    prev->setIcon(QIcon(":/icons/moveleft.png"));
    prev->setText("Previous");
    prev->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    prev->setAutoRaise(true);
    next = new QToolButton(suchbox);
    next->setIcon(QIcon(":/icons/moveright.png"));
    next->setText("Next");
    next->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    next->setAutoRaise(true);
    wholeLine = new QCheckBox("Select whole lines",suchbox);
    wholeLine->setChecked(false);

    slt->setSpacing(5);
    slt->setMargin(0);
    slt->addWidget(hidesearch,0,0);
    slt->addWidget(search,0,1);
    slt->addWidget(prev,0,2);
    slt->addWidget(next,0,3);
    slt->setVerticalSpacing (0);
    replacementInfo = new QLabel("",suchbox);
    slt->addWidget(replacementInfo,2,1,1,5);
    slt->addWidget(wholeLine,0,4);
    slt->setColumnStretch(6,10);
    replace = new QLineEdit(suchbox);
    replace->setMinimumSize(QSize(150, 0));

    replaceButton = new QToolButton(suchbox);
    replaceButton->setIcon(QIcon(":/icons/moveright.png"));
    replaceButton->setText("Replace");
    replaceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    replaceButton->setAutoRaise(true);

    replacePButton = new QToolButton(suchbox);
    replacePButton->setIcon(QIcon(":/icons/moveleft.png"));
    replacePButton->setText("Replace");
    replacePButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    replacePButton->setAutoRaise(true);
    replaceAButton= new QToolButton(suchbox);
    ediVis=oupVis=false;
    replaceAButton->setIcon(QIcon(":/icons/all.png"));
    replaceAButton->setText("Replace All!");
    replaceAButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    replaceAButton->setAutoRaise(true);
    slt->addWidget(replace,1,1);
    slt->addWidget(replacePButton,1,2);
    slt->addWidget(replaceButton,1,3);
    slt->addWidget(replaceAButton,1,4);
    suchbox->setLayout(slt);
    suchbox->hide();
    connect(search, SIGNAL(textChanged(const QString&)),this,SLOT(findText()));
    connect(search, SIGNAL(textChanged(const QString&)),editor,SIGNAL(search(const QString&)));
    connect(next, SIGNAL(clicked() ), this, SLOT(findNext()));
    connect(prev, SIGNAL(clicked() ), this, SLOT(findPrev()));
    connect(replaceButton, SIGNAL(clicked() ), this, SLOT(replaceNext()));
    connect(replacePButton, SIGNAL(clicked() ), this, SLOT(replacePrev()));
    connect(replaceAButton, SIGNAL(clicked() ), this, SLOT(replaceAll()));
    connect(hidesearch, SIGNAL(clicked() ), this, SLOT(hideSearch()));

    shxEditorDock = new QDockWidget("SHELXL Editor",this,Qt::Widget);//Qt::Window//Qt::Tool
    shxEditorDock->setObjectName("Editor");
    shxEditorDock->setFeatures(
		    QDockWidget::DockWidgetMovable|
		    QDockWidget::DockWidgetFloatable|
		    QDockWidget::DockWidgetClosable);
    connect(shxEditorDock,SIGNAL(visibilityChanged(bool)),this,SLOT(editorIsVisible(bool)));
    shx_group = new QWidget(this);
    shx_group->setContentsMargins(0,0,0,0);
    sss = new QVBoxLayout(shx_group);
    sss->addWidget(navibox);
    sss->addWidget(editor);
    sss->addWidget(suchbox);
    shx_group->setLayout(sss);
    shxEditorDock->setWidget(shx_group);
    connect(shxEditorDock,SIGNAL(visibilityChanged(bool)),this,SLOT(shxActUpdate(bool)));
    QFont font;
    font.setFamily("Courier");
    font.setPointSize(9);
#if (QT_VERSION >= 0x040700)
    font.setStyleStrategy(QFont::ForceIntegerMetrics);
#endif
    editor->setFont(font);
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);

    addDockWidget(Qt::RightDockWidgetArea, shxEditorDock );
  } 

  {//init shelx output viewer
    xlPutor = new CodeEditor();
    xlPutor->setReadOnly(true);
    xlPutor->highlighter->loadHighlightPatterns(lstpatternfile);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(9);
    xlPutor->setFont(font);
    xlPutor->setLineWrapMode(QPlainTextEdit::NoWrap);
    shxOutputDock = new QDockWidget("SHELXL output",this,Qt::Widget);
    shxOutputDock->setObjectName("Output");
    shxOutputDock->setFeatures(
		    QDockWidget::DockWidgetMovable|
		    QDockWidget::DockWidgetFloatable|
		    QDockWidget::DockWidgetClosable);
    shxOutputDock->hide();

    connect(shxOutputDock,SIGNAL(visibilityChanged(bool)),this,SLOT(outputIsVisible(bool)));
    buttomLt = new QHBoxLayout();
    loadRes = new QPushButton(QIcon(":/icons/open.png"),"Load refinement results",this);
    withdraw = new QPushButton(QIcon(":/icons/cancel.png"),"Discard results",this);
    showLst = new QPushButton("Show output listing file",this);
    connect(loadRes,SIGNAL(pressed()),this,SLOT(loadrefres()));
    connect(withdraw,SIGNAL(pressed()),this,SLOT(withdrawres()));
    connect(showLst,SIGNAL(pressed()),this,SLOT(loadLst()));
    buttomLt->addWidget(loadRes);
    buttomLt->addWidget(withdraw);
    buttomLt->addStretch(10);    
    buttomLt->addWidget(showLst);
    stoppRefinement = new QPushButton(QIcon(":/icons/cancel.png"),"STOP refinement!",this);
    refineButtonBox = new  QGroupBox();
    refineButtonBox->setLayout(buttomLt);
    stoppRefinement->hide();
    search2 = new QLineEdit;
    search2->setMinimumSize(QSize(150, 0));
    suchbox2 =  new QGroupBox("Search");
    suchbox2->setFlat(true);
    slt2        = new QHBoxLayout();
    hidesearch2 = new QToolButton(this);
    hidesearch2->setIcon(QIcon(":/icons/cancel.png"));
    hidesearch2->setAutoRaise(true);
    prev2 = new QToolButton(this);
    prev2->setIcon(QIcon(":/icons/moveleft.png"));
    prev2->setText("Previous");
    prev2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    prev2->setAutoRaise(true);
    next2 = new QToolButton(this);
    next2->setIcon(QIcon(":/icons/moveright.png"));
    next2->setText("Next");
    next2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    next2->setAutoRaise(true);
    slt2->setSpacing(6);
    slt2->setMargin(0);
    slt2->addWidget(hidesearch2);
    slt2->addWidget(search2);
    slt2->addWidget(prev2);
    slt2->addWidget(next2);
    slt2->addStretch(10);
    suchbox2->setLayout(slt2);
    suchbox2->hide();
    connect(search2, SIGNAL(textChanged(const QString&)),this,SLOT(findText2()));
    connect(search2, SIGNAL(textChanged(const QString&)),xlPutor,SIGNAL(search(const QString&)));
    connect(next2, SIGNAL(clicked() ), this, SLOT(findNext2()));
    connect(prev2, SIGNAL(clicked() ), this, SLOT(findPrev2()));
    connect(hidesearch2, SIGNAL(clicked() ), this, SLOT(hideSearch2()));





    vlt = new QVBoxLayout();
    vlt->addWidget(xlPutor);
    vlt->addWidget(suchbox2);
    vlt->addWidget(refineButtonBox);
    vlt->addWidget(stoppRefinement);
    refineButtonBox->hide();
    gp = new  QGroupBox("");
    gp->setFlat(true);
    gp->setLayout(vlt);
    shxOutputDock->setWidget(gp);
    addDockWidget(Qt::RightDockWidgetArea, shxOutputDock);
    tabifyDockWidget (shxEditorDock,shxOutputDock);

    connect(shxOutputDock,SIGNAL(visibilityChanged(bool)),this,SLOT(shxOPTUpdate(bool)));
  }

  dock = new QDockWidget("Information Window",this);
  dock->setObjectName("Info");
  dock->setAllowedAreas(Qt::AllDockWidgetAreas);
  infoKanal = new QTextBrowser(dock);
  infoKanal->setLineWrapMode(QTextEdit::NoWrap);
  dock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
  dock->setWidget(infoKanal);
  dock->hide();
  addDockWidget(Qt::RightDockWidgetArea, dock);
  resize(qApp->desktop()->screenGeometry().width(),qApp->desktop()->screenGeometry().height());

  //rename dock
  renamDock = new QDockWidget("Rename Mode");
  renamDock->setObjectName("Rename");
  enterRenameMode = renamDock->toggleViewAction ();
  enterRenameMode->setShortcut(Qt::ALT+Qt::Key_R); 
  connect(enterRenameMode,SIGNAL(triggered(bool)),this,SLOT(renamUpdate(bool)));
  renamDock->setFeatures(
		  QDockWidget::DockWidgetMovable|
		  QDockWidget::DockWidgetFloatable|
		  QDockWidget::DockWidgetClosable);

  renameBox = new QGroupBox();
  rnla    = new QVBoxLayout();
  nliL         = new QLabel("You are in 'rename mode' now.");
  nliL->setFont(QFont("Helvetica",18,QFont::Bold));
  spellCheckact2 = new QToolButton(renameBox); 
  spellCheckact2->setIcon(QIcon(":/icons/spell.png"));
  spellCheckact2->setText("Oops! I made a mistake! Sorry!  Please check!");
  spellCheckact2->setIconSize(QSize(22,22));
  spellCheckact2->setToolTip("Jump to first error in file.");
  spellCheckact2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  connect(spellCheckact2,SIGNAL(clicked()),shxEditorDock,SLOT(raise()));
  connect(spellCheckact2,SIGNAL(clicked()),renamDock,SLOT(hide()));
  connect(spellCheckact2,SIGNAL(clicked()),this,SLOT(jumpToError()));
  rnla->addWidget(spellCheckact2);
  rnla->addWidget(nliL);
  hintL = new QLabel("<div style=color:#0000aa; style=white-space:normal; style=background-color:"
"#f8f5cd; style=font-size:large;><b><u>Hint:</u></b><a name=\"test\"><img width=\"23\" src=\""
":/icons/filesave.png\"></a> It is wise to save often. Especially after renaming a couple of Q-Peaks. "
"<br>Saving synchronizes the visualized structure and the editor content.<br> <b>Before</b> you start renaming and <b>after</b> "
"you are finished renaming you should <b>SAVE!</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"#test\" style=text-align:right;>Click here to hide this hint for ever.</a></div>");
  hintL->setWordWrap(true);
  rnla->addWidget(hintL);
  connect(hintL,SIGNAL(linkActivated ( const QString)), this ,SLOT(hintLhide()));
  rufModeBox  = new QCheckBox("Automatically jump to first unused label");
  rufModeBox->setChecked(false);
  connect(rufModeBox,SIGNAL(stateChanged(int)),this,SLOT(rufmode())); 
  partSpinL = new QLabel("Part:");
  partSpin = new QSpinBox();
  partSpin->setMinimum(-99);
  partSpin->setMaximum(99);  
  partSpin->setValue(0);
  connect(partSpin,SIGNAL(valueChanged(int)),this,SLOT(updateLabel()));
  resiNrSpinL = new QLabel("Residue Nr:");
  resiNrSpin = new QSpinBox();
  resiNrSpin->setMinimum(0);
  resiNrSpin->setMaximum(9999);
  resiNrSpin->setValue(0);
  connect(resiNrSpin,SIGNAL(valueChanged(int)),this,SLOT(renameRNchanged(int)));
  resiResiClassL = new QLabel("Residue Class:");
  resiResiClass = new QLineEdit("");
  resiResiClass->setMaxLength(4);
  resiResiClass->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z]\\w{0,3}"),resiResiClass)); 
  resiResiClass->setMinimumWidth(100);
  connect(resiResiClass,SIGNAL(textEdited(const QString&)),this,SLOT(grossRC(const QString&)));
  resiResiClass->setEnabled(false);
  rhla = new QHBoxLayout();
  rhla->addWidget(partSpinL);
  rhla->addWidget(partSpin);
  rhla->addWidget(resiNrSpinL);
  rhla->addWidget(resiNrSpin);
  rhla->addWidget(resiResiClassL);
  rhla->addWidget(resiResiClass);
  rnla->addLayout(rhla);
  rhla2 = new QHBoxLayout();
  tieOccL = new QLabel("Tie occupation to: ");
  tieOccL->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  tieType = new QComboBox();
  tieType->addItem("its fixed value",0);
  tieType->addItem("fv(n)",1);
  tieType->addItem("1-fv(n)",-1);
  tieFVNrL = new QLabel("n = ");
  tieFVNrL->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  tieFVNr = new QSpinBox();
  tieFVNr->setMinimum(2);
  tieFVNr->setMaximum(9999);
  tieFVNr->setValue(2);
  tieUsedL = new QLabel("fv(2) is 0x used");
  connect(tieType,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTieFreeVar()));
  connect(tieFVNr,SIGNAL(valueChanged(int)),this,SLOT(updateTieFreeVar()));  
  connect(partSpin,SIGNAL(valueChanged(int)),this,SLOT(updateTieFreeVar()));
  rhla2->addWidget(tieOccL);
  rhla2->addWidget(tieType);
  rhla2->addWidget(tieFVNrL);
  rhla2->addWidget(tieFVNr);
  rhla2->addWidget(tieUsedL);
  rhla2->setEnabled ( false );
  rnla->addLayout(rhla2);
  indexla = new QHBoxLayout();
  indexIncr = new QCheckBox("Increment Suffix "); 
  indexIncr->setChecked(false);
  indexL = new QLabel("Number");
  indexSpin = new QSpinBox();
  indexSpin->setMinimum(-1);
  indexSpin->setMaximum(999);
  indexSpin->setValue(labelIndex);
  connect(indexSpin,SIGNAL(valueChanged(int )),this,SLOT(changeIndexofLabel(int )));
  sufixBox = new QComboBox();
  sufixL =new QLabel("Suffix");
  alpha<<"'"<<""<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"
      <<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z"
      <<"A'"<<"B'"<<"C'"<<"D'"<<"E'"<<"F'"<<"G'"<<"H'"<<"I'"<<"J'"<<"K'"<<"L'"<<"M'"
      <<"N'"<<"O'"<<"P'"<<"Q'"<<"R'"<<"S'"<<"T'"<<"U'"<<"V'"<<"W'"<<"X'"<<"Y'"<<"Z'";
  sufixBox->addItems(alpha);
  sufixBox->setCurrentIndex(1);
  sufixBox->setEditable(true);
  connect(sufixBox,SIGNAL( currentIndexChanged ( const QString &)),this,SLOT(changeSuffixofLabel( const QString & )));
  indexla->addWidget(indexL);
  indexla->addWidget(indexSpin);
  indexla->addWidget(sufixL);
  indexla->addWidget(sufixBox);
  rnla->addLayout(indexla);
  labelSuffix="";
  nextLabel="C1";
  labelIndex=1;
  labelPSE="C";
  sfacBox = new QGroupBox("Scattering factors");
  sfacBox->setFixedSize(110*52,70);
  sfacla = new QHBoxLayout();
  dsm= new QSignalMapper(this);
  sfacla->addWidget(pserbt[0] = new QRadioButton("any"));
  dsm->setMapping(pserbt[0],"any");
  connect(pserbt[0], SIGNAL(clicked()), dsm, SLOT(map()));
  for (int i=1; i<109; i++){
    sfacla->addWidget(pserbt[i] = new QRadioButton(mole.pse(i-1)));
    dsm->setMapping(pserbt[i],mole.pse(i-1));
    connect(pserbt[i], SIGNAL(clicked()), dsm, SLOT(map()));
    pserbt[i]->hide();
  }
  connect(dsm, SIGNAL(mapped(const QString &)), this, SLOT(changeElemetofLabel(const QString &)));

  sfacBox->setLayout(sfacla);
  qsa =new QScrollArea();
  qsa->setWidget(sfacBox);
  qsa->setMaximumHeight(100);
  QHBoxLayout *rufla= new QHBoxLayout();
  rufla->addWidget(rufModeBox);
  rufla->addWidget(indexIncr);
  rnla->addLayout(rufla);
  nextNameL = new QLabel("Next Label is:<br> <font color=\"red\">C1</font>");
  nextNameL->setFont(QFont("Helvetica",16,QFont::Bold));
  /*QPalette p = nextNameL->palette();
  p.setBrush(QPalette::Active,QPalette::Background,QColor(255,255,230));
  nextNameL->setPalette (p );*/
  rnla->addWidget(nextNameL);
  rnla->addWidget(qsa);
  psew = new PSEWidget(this);
  psew->selection=0;
  psew->setVisible(false);
  rnla->addWidget(psew);
  willpse = new QPushButton("more scattering factors");

  willpse->setStyleSheet(
		  QString(
			  "QPushButton {"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			 ).arg("#dedede").arg("#eeeeee").arg("#aeaeae"));

  connect(willpse,SIGNAL(clicked()),psew,SLOT(show()));
  hidwillpse=new QTimer();
  hidwillpse->setSingleShot(true);
  hidwillpse->setInterval(100);

  connect(willpse,SIGNAL(clicked()),hidwillpse,SLOT(start()));
  connect(hidwillpse,SIGNAL(timeout()),willpse,SLOT(hide()));
  connect(psew,SIGNAL(pseSelected(int)),psew,SLOT(hide()));
  connect(psew,SIGNAL(pseSelected(int)),willpse,SLOT(show()));
  connect(psew,SIGNAL(pseSelected(int)),this,SLOT(addNewScatteringFactor(int)));

  rnla->addWidget(willpse);
  renameBox->setLayout(rnla);
 
  renamDock->setWidget(renameBox);
  addDockWidget(Qt::RightDockWidgetArea, renamDock);
  renamDock->hide();
  tabifyDockWidget (shxEditorDock,renamDock);
  //rename dock
  //
  qPeakDock = new QDockWidget("Q-Peak-Legend");
  qPeakDock->setObjectName("Peak");
  qPeakDock->hide();
  qPeakDock->setFixedWidth(50);
  qPeakDock->setToolTip("Click or scroll here to reduce the number of Q-peaks shown.");
  qPeakDock->setFeatures(QDockWidget::DockWidgetClosable);
  showQPeakLegend = qPeakDock->toggleViewAction ();
  connect(showQPeakLegend, SIGNAL(toggled(bool )),this,SLOT(setMonoQ(bool )));
  showQPeakLegend->setIcon(QIcon(":/icons/qlegend.png"));
  qpv = new QPeakView();
  qPeakDock->setWidget(qpv);
  addDockWidget(Qt::LeftDockWidgetArea,qPeakDock);

  //HISTORY
  histDock = new QDockWidget("Refinement History");
  histDock->setObjectName("History");
  hw = new HistoryWidget();
  qsa2 =new QScrollArea();
  qsa2->setWidget(hw);
  qsa2->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
  histDock->hide();
  histDock->setFeatures(QDockWidget::DockWidgetMovable|
		  QDockWidget::DockWidgetFloatable|
		  QDockWidget::DockWidgetClosable);
  histDock->setWidget(qsa2);
  histDock->setMinimumSize(100,150);
  addDockWidget(Qt::RightDockWidgetArea,histDock);
  connect(hw,SIGNAL(message(const QString&)),this,SLOT(updateStatusBar(const QString&)));
  connect(hw,SIGNAL(lofi(const QString&)),this,SLOT(loadFile(const QString&)));
  //HISTORY


  scalePic=2.0;
  if ((Revision%666)==0)
      setWindowTitle(QString("%1 - A clumsy GUI for sHELLxl (Rev: %2)(easteregg) ")
              .arg(PROGRAM_NAME)
              .arg(Revision));
  else
  setWindowTitle(QString("%1 - A Qt GUI for SHELXL (Rev: %2)").arg(PROGRAM_NAME).arg(Revision));
  balken = new QProgressBar(this);
  balken->hide();


  chgl =new ChGL(this);

   chgl->setMolecule(&mole);
  editor->setgl(chgl);
  xlPutor->setgl(chgl);
  connect(editor,SIGNAL(deleteSelected()),this,SLOT(deleteSelectedAtoms()));
  connect(qpv,SIGNAL(cutOffChanged(double)),chgl,SLOT(hideQPeaksBelow(double)));
  connect(qpv,SIGNAL(findQP(double)),chgl,SLOT(highliteQPeak(double)));
  connect(chgl,SIGNAL(qpfoci(double)),qpv,SLOT(qpeaksInFocus(double)));
  connect(renamDock,SIGNAL(visibilityChanged(bool)),chgl,SLOT(setReNaMo(bool))); 
  connect(renamDock,SIGNAL(visibilityChanged(bool)),this,SLOT(updateLabel()));

  connect(renamDock,SIGNAL(visibilityChanged(bool)),this,SLOT(renamUpdate(bool)));
  connect(chgl,SIGNAL(message(const QString&)),this,SLOT(updateStatusBar(const QString&)));
  connect(chgl,SIGNAL(bigmessage(const QString&)),this,SLOT(infoKanalNews(const QString&)));
  connect(chgl,SIGNAL(jumpit(int)),this ,SLOT(jumpToAtom(int)));
  connect(chgl,SIGNAL(insertDFIX(double,double,QList<MyAtom>,QString)),this ,SLOT(insertDFIX(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertDANG(double,double,QList<MyAtom>,QString)),this ,SLOT(insertDANG(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertFLAT(double,QList<MyAtom>,QString)),this ,SLOT(insertFLAT(double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertEXYZ(QList<MyAtom>)),this ,SLOT(insertEXYZ(QList<MyAtom>)));
  connect(chgl,SIGNAL(insertEADP(QList<MyAtom>)),this ,SLOT(insertEADP(QList<MyAtom>)));
  connect(chgl,SIGNAL(insertDELU(double, double, QList<MyAtom>,QString)),this ,SLOT(insertDELU(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertSIMU(double,double,double,QList<MyAtom>,QString)),this ,SLOT(insertSIMU(double,double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertISOR(double,double,QList<MyAtom>,QString)),this,SLOT(insertISOR(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertRIGU(double, double, QList<MyAtom>,QString)),this ,SLOT(insertRIGU(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertCHIV(double, double, QList<MyAtom>,QString)),this ,SLOT(insertCHIV(double,double,QList<MyAtom>,QString)));
  connect(chgl,SIGNAL(insertANIS(QList<MyAtom>)),this ,SLOT(insertANIS(QList<MyAtom>)));
  connect(chgl,SIGNAL(newContext()),this,SLOT(trick()));
  QAction *sma =new QAction("show mat",this);
  sma->setShortcut(tr("Alt+F1"));
  connect(sma,SIGNAL(triggered()),chgl,SLOT(showMatrix()));
  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileActs[i] = new QAction(this);
    recentFileActs[i]->setVisible(false);
    connect(recentFileActs[i], SIGNAL(triggered()),
		    this, SLOT(openRecentFile()));
  }
  fileMenu   = new QMenu(tr("File"), this);
  chgl->sfacMenu   = new QMenu(tr("Change element to..."),chgl);
  editMenu   = new QMenu(tr("Edit"),this);
  dialogMenu = new QMenu(tr("Settings"), this);
  viewMenu   = new QMenu(tr("View"), this);
  runMenu    =  new QMenu(tr("SHELX"),this);
  runMenu->setObjectName("SHELX");
  packMenu   = new QMenu(tr("Pack"),this);
  packMenu->setEnabled(false);
  extraMenu  = new QMenu(tr("Extra"),this);
  {
      QAction *a;
      a = refineAct = runMenu->addAction(QIcon(":/icons/refine.png"),"copy current file to instruction file and run shelxl",this,SLOT(runShelXL()));
      a->setShortcut(QKeySequence("F2"));
      a->setData(false);
      refineAct->setVisible(false);
      refineAct->setEnabled(false);

      a->setObjectName("refineAct");

      a = refineActAnis = runMenu->addAction(QIcon(":/icons/refineanis.png"),"Insert ANIS and run shelxl",this,SLOT(runShelXL()));
      a->setShortcut(QKeySequence("Alt+F2"));
      a->setData(true);
      refineActAnis->setVisible(false);
      refineActAnis->setEnabled(false);
      a->setObjectName("refineActAnis");
      runMenu->addAction("Try to refine until WGHT converges (max 10 runs)",this,SLOT(convergeWght()));

//      runMenu->addAction("load list file",this,SLOT(loadLst()));
      a=actionHFix=runMenu->addAction(QIcon(":/icons/autohfix.png"),
                                      tr("automatic H-Fi&x"),this,SLOT(autoHFix()));
      a->setObjectName("actionHFix");
      a=runMenu->addAction("DISPersion from WAVElength",this,SLOT(dispFromWave()));
      a=actionUniq=runMenu->addAction(QIcon(":/icons/putincell.png"),tr("move centers of gravity into unit cell"),this,SLOT(uniq()));
      a->setObjectName("actionUniq");
      runMenu->addAction("update UNIT instruction",this,SLOT(updateUnit()));
      runMenu->addAction("weed unused sfac numbers ",this,SLOT(weedEmptySfacs()));
      wghtAct=runMenu->addAction("update WGHT instruction",this,SLOT(updateWght()));
      wghtAct->setEnabled(false);
      sinaAct = runMenu->addAction(QIcon(":/icons/sina.png"),"make (selected or all) atoms isotropic (ISOT)",this,SLOT(sina()));
      sinaAct->setObjectName("sinaAct");
      sinaAct->setData(-1);
      soact=runMenu->addAction("sort atoms in file.",this,SLOT(sortAtoms()));
      sortierWeise=1;
      runMenu->addAction("create Q-Peaks from Fo-Fc map",this, SLOT(addMoreQPeaks()));
      runMenu->addAction("invert structure", this, SLOT(invert()));
      runMenu->addAction("Fix some parameters", this, SLOT(fixIt()));
      runMenu->addAction("UNIQue ", this, SLOT(unique()));
      runMenu->addSeparator();
//      runMenu->addAction("set path to SHELXL executable ...",this,SLOT(setShelxlPath()));//obsolete
      runMenu->addAction("specify SHELXL runtime options",this , SLOT(setShelxlOptions()));
  }
  {
    QToolBar *tb =fito= new QToolBar(this);
    tb->setWindowTitle(tr("File Tools"));
    tb->setObjectName("FileTB");
    addToolBar(tb);
    QAction *a;
    a=fileMenu->addAction(QIcon(":/icons/open.png"),
		    tr("&Open..."), this, SLOT(openFile() ),
		    QKeySequence(tr("Ctrl+O", "File|Open")));
    tb->addAction(a);
    a=fileMenu->addAction(QIcon(":/icons/diskette.png"),
		    tr("&ScreenShot"),this,SLOT(saveScene()),
                    QKeySequence(tr("Alt+S","File|ScreenShot")));

    tb->addAction(a);
    actionSave = new QAction(QIcon(":icons/filesave.png"),tr("&Save"), this);
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    actionSave->setEnabled(false);
    tb->addAction(actionSave);
    fileMenu->addAction(actionSave);
    actionSaveAs = new QAction(QIcon(":icons/filesave.png"),tr("&Save as ..."), this);


#if (QT_VERSION >= 0x040500)
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
#else
    actionSaveAs->setShortcut(tr("Shift+Ctrl+S"));
#endif
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    actionSaveAs->setEnabled(true);
    fileMenu->addAction(actionSaveAs);
    fileMenu->addAction("Export visible atoms ...",this,SLOT(exportCoordinates()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("open save history"),this,SLOT(openSaveHistory()));
    fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
      fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    a=fileMenu->addAction(QIcon(":/icons/cancel.png"), tr("&Quit"),
		    this, SLOT(close()),
		    QKeySequence(tr("Ctrl+Q", "File|Quit")) );

    tb->addAction(a);
  }
  backColor       = new QAction(tr("Change background color"), this);
  labelColor      = new QAction(tr("Change label color"), this);
  backColor->setShortcut (tr("Shift+Alt+B"));
  labelColor->setShortcut(tr("Shift+Alt+T"));
  connect(backColor, SIGNAL (triggered ()), chgl, SLOT(changeBColor()));
  connect(labelColor, SIGNAL (triggered ()), chgl, SLOT(changeTColor()));
  dialogMenu->addAction("Change tool bar icon size",this,SLOT(changeIconSize()));
  dialogMenu->addAction("Change screen shot resolution",this,SLOT(changeScalefactor()));
  separateLabelFile= dialogMenu->addAction("Render labels in a separate file");
  separateLabelFile->setCheckable(true);
  separateLabelFile->setChecked(false);
  dialogMenu->addAction(backColor);
  dialogMenu->addAction(labelColor);
  dialogMenu->addAction(tr("Change label font"), this, SLOT(changeLabelFont()));
  dialogMenu->addAction(tr("Change editor font"), this, SLOT(changeEditorFont()));

  mapcontrol=dialogMenu->addAction(QIcon(":/icons/mapcontrol.png"),tr("Open map control"),this,SLOT(openMapControl()));

  astdlg = dialogMenu->addAction(QIcon(":/icons/eac.png"),"Atom Styles",
                  this,SLOT(styleDialog()),QKeySequence("Alt+Shift+A"));
  dialogMenu->addAction("Save current orientation matrix",chgl,SLOT(saveOrientation()));
  dialogMenu->addAction("Load orientation matrix",chgl,SLOT(loadOrientation()));
  {
    QToolBar *tb = editToolBar = new QToolBar(this);
    tb->setObjectName("EditTB");
    tb->setWindowTitle(tr("Editor Tools"));
    addToolBar(tb);
    editMenu->addAction(enterRenameMode);
    QAction *a;
    a = actionUndo = new QAction(QIcon( ":icons/editundo.png"),tr("&Undo"), this);
    a->setShortcut(QKeySequence::Undo);
    a->setEnabled(false);
    tb->addAction(a);
    editMenu->addAction(a);
    a = actionRedo = new QAction(QIcon( ":icons/editredo.png"),tr("&Redo"), this);
    a->setShortcut(QKeySequence::Redo);
    a->setEnabled(false);
    tb->addAction(a);
    editMenu->addAction(a);
    editMenu->addSeparator();
    a = actionCut = new QAction(QIcon(":icons/editcut.png"),tr("Cu&t"), this);
    a->setShortcut(QKeySequence::Cut);
    a->setEnabled(false);
    tb->addAction(a);
    editMenu->addAction(a);
    a = actionCopy = new QAction(QIcon(":icons/editcopy.png"),tr("&Copy"), this);
    a->setShortcut(QKeySequence::Copy);
    a->setEnabled(false);
    tb->addAction(a);
    editMenu->addAction(a);
    a = actionPaste = new QAction(QIcon(":icons/editpaste.png"),tr("&Paste"), this);
    a->setShortcut(QKeySequence::Paste);
    a->setEnabled(false);
    tb->addAction(a);
    editMenu->addAction(a);
#ifndef QT_NO_CLIPBOARD
    actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
#endif

    a=actionSearch=editMenu->addAction(QIcon(":/icons/search.png"),
		    tr("&Find in Text"),this,SLOT(showSearch()),
		    QKeySequence(tr("Ctrl+F", "Edit|Find")));

    tb->addAction(a);
    a->setEnabled(false);
    a=actionNext=editMenu->addAction(QIcon(":/icons/moveright.png"),
		    tr("Find &Next"),this, SLOT(findNext()),
		    QKeySequence(tr("F3", "Edit|Next")));

    a->setEnabled(false);
    a=actionPrev=editMenu->addAction(QIcon(":/icons/moveleft.png"),
		    tr("Find &Previous"),this, SLOT(findPrev()),
		    QKeySequence(tr("Shift+F3", "Edit|Previous")));

    a->setEnabled(false);
    a=killQs=editMenu->addAction(QIcon(":/icons/killqs.png"),
		    tr("Delete all Q-peaks"),this,SLOT(deleteAllQ()),
                    QKeySequence(tr("Meta+Shift+Q")));
    tb->addAction(a);
    a=killHs=editMenu->addAction(QIcon(":/icons/killhs.png"),
		    tr("Delete all Hydrogen atoms"),this,SLOT(deleteAllH()),
		    QKeySequence(tr("Meta+Shift+H")));
    tb->addAction(a);

    tb->addAction(refineAct);

    tb->addAction(refineActAnis);
    editToolBar->hide();
  }
  {
    QToolBar *tb = vito = new QToolBar(this);
    tb->setWindowTitle(tr("View Tools"));
    tb->setObjectName("ViewTB");
    addToolBar(tb);
    QAction *a;
    stereoMenu = new  QMenu("&3D Stereo Options");
    zm_stereo = stereoMenu->addAction("&Zalman Monitor 3D");
    pe_stereo = stereoMenu->addAction("&Parallel Eye Side by Side Stereo");
    xe_stereo = stereoMenu->addAction("&Crossed Eye Side by Side Stereo");
    an_stereo = stereoMenu->addAction("&Anaglyph Stereo red left blue right(testing)");
    hw_stereo = stereoMenu->addAction("&Hardware Stereo (testing)");
    hw_stereo->setEnabled(chgl->context()->format().stereo());
//                                context()->format().stereo()
    no_stereo = stereoMenu->addAction("Stereo &Mode off");
    connect(zm_stereo,SIGNAL(triggered()),chgl,SLOT(zalman()));
    connect(pe_stereo,SIGNAL(triggered()),chgl,SLOT(parallel()));
    connect(xe_stereo,SIGNAL(triggered()),chgl,SLOT(crosseye()));
    connect(an_stereo,SIGNAL(triggered()),chgl,SLOT(anaglyphRedCyan()));
    connect(hw_stereo,SIGNAL(triggered()),chgl,SLOT(hardwareStereo()));
    connect(no_stereo,SIGNAL(triggered()),chgl,SLOT(nostereo()));
    stereoGroup = new QActionGroup(this);
    stereoGroup->addAction(zm_stereo);
    stereoGroup->addAction(pe_stereo);
    stereoGroup->addAction(xe_stereo);
    stereoGroup->addAction(an_stereo);
    stereoGroup->addAction(hw_stereo);
    stereoGroup->addAction(no_stereo);
    zm_stereo->setCheckable(true);
    pe_stereo->setCheckable(true);
    xe_stereo->setCheckable(true);
    an_stereo->setCheckable(true);
    hw_stereo->setCheckable(true);
    no_stereo->setCheckable(true);
    no_stereo->setChecked(true);
    viewMenu->addMenu(stereoMenu);
    viewMenu->addSeparator();
    moveMenu = new QMenu("Move Menu");
    a=moveMenu->addAction(tr("Zoom in"),          this,SLOT(zoomin ()),QKeySequence(Qt::ALT + Qt::Key_PageUp));
    a=moveMenu->addAction(tr("Zoom out"),         this,SLOT(zoomout()),QKeySequence(Qt::ALT + Qt::Key_PageDown));
    a=moveMenu->addAction(tr("Rotate left"),      this,SLOT(rotl   ()),QKeySequence(Qt::ALT + Qt::Key_Left));
    a=moveMenu->addAction(tr("Rotate right"),     this,SLOT(rotr   ()),QKeySequence(Qt::ALT + Qt::Key_Right));
    a=moveMenu->addAction(tr("Rotate upwards"),   this,SLOT(rotu   ()),QKeySequence(Qt::ALT + Qt::Key_Up));
    a=moveMenu->addAction(tr("Rotate downwards"), this,SLOT(rotd   ()),QKeySequence(Qt::ALT + Qt::Key_Down));
    moveMenu->addAction(sma);
    a=moveMenu->addAction("Zoom out",chgl,SLOT(zoomOut()),QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_Z));
    a=moveMenu->addAction("view along z direction",chgl,SLOT(along001()));
    viewMenu->addMenu(moveMenu);
    actionEP=a=viewMenu->addAction(QIcon(":/icons/efplus.png"),
		    tr("Increase editor fontsize"),this,SLOT(increaseEdtiorFont()),
		    QKeySequence(QKeySequence::ZoomIn));

    tb->addAction(a);
    actionEM=a=viewMenu->addAction(QIcon(":/icons/efminus.png"),
		    tr("Decrease editor fontsize"),this,SLOT(decreaseEdtiorFont()),
		    QKeySequence(QKeySequence::ZoomOut));

    tb->addAction(a);
    tb->addAction(astdlg);
    tb->addAction(mapcontrol);
    mapcontrol->setVisible(false);
    a=togAtom =viewMenu->addAction(QIcon(":/icons/atoms.png"),
		    tr("toggle atoms"),chgl,SLOT(setAtom(bool)),
		    QKeySequence("Alt+A"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    tubeStyle = tb->addAction(QIcon(":/icons/tubes.png"),"tubes",chgl,SLOT(setTube(bool)));
    ballStyle = tb->addAction(QIcon(":/icons/ballstick.png"),"ball-stick",chgl,SLOT(unsetTube(bool)));
    tubeGroup = new QActionGroup(this);
    tubeGroup->addAction(tubeStyle);
    tubeGroup->addAction(ballStyle);
    ballStyle->setCheckable(true);
    ballStyle->setChecked(true);
    tubeStyle->setCheckable(true);
    tubeStyle->setChecked(false);
    chgl->setTube(false);

    a=togElli =viewMenu->addAction(QIcon(":/icons/adps.png"),
		    tr("toggle ADPs"),chgl,SLOT(setADP(bool)),
		    QKeySequence("Alt+E"));

    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    a=togBond =viewMenu->addAction(QIcon(":/icons/bonds.png"),
		    tr("toggle bonds"),chgl,SLOT(setBond(bool)),
		    QKeySequence("Alt+B"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    a=togLabel=viewMenu->addAction(QIcon(":/icons/labels.png"),
		    tr("toggle labels"),chgl,SLOT(setLabel(bool)),
		    QKeySequence("Alt+L"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    a=viewMenu->addAction(QIcon(":/icons/laplus.png"),
		    tr("increase label fontsize"),chgl,SLOT(incFontSize()),
		    QKeySequence("Alt++"));
//    tb->addAction(a);

    a=viewMenu->addAction(QIcon(":/icons/laminus.png"),
		    tr("decrease label fontsize"),chgl,SLOT(decFontSize()),
		    QKeySequence("Alt+-"));
//    tb->addAction(a);
    a=togUnit =viewMenu->addAction(QIcon(":/icons/unitcell.png"),
		    tr("toggle unit cell"),chgl,SLOT(setUnit(bool)),
		    QKeySequence("Alt+C"));
    a->setCheckable(true);
    a->setChecked(false);
    tb->addAction(a);
    a=togHBond=viewMenu->addAction(QIcon(":/icons/hbrs.png"),
		    tr("toggle H-bonds"),chgl,SLOT(setHBond(bool)),
		    QKeySequence("Alt+H"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    fxle= new FourXle(&mole, chgl,tb);
    fxle->balken=balken;
    chgl->lighting = new QCheckBox("Lighting");
    chgl->lighting->setChecked(true);
    chgl->niceTrans= new QCheckBox("Undistorted transparency");
    chgl->niceTrans->setChecked(false);
    chgl->fillMap= new QCheckBox("Filled map surface");
    chgl->fillMap->setChecked(false);
   /* 
    a=togFOMap=viewMenu->addAction(QIcon(":/icons/fomap.png"),
		    tr("toggle Fobs-map"),chgl,SLOT(setFOM(bool)),
		    QKeySequence("Alt+O"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    connect(a,SIGNAL(triggered()),this,SLOT(zeigSXFFThinweis()));
    a=togDfMap=viewMenu->addAction(QIcon(":/icons/diffmap.png"),
		    tr("toggle Difference-map"),chgl,SLOT(setDFM(bool)),
		    QKeySequence("Alt+D"));
    a->setCheckable(true);
    a->setChecked(true);
    tb->addAction(a);
    connect(a,SIGNAL(triggered()),this,SLOT(zeigSXFFThinweis()));// */
    a=togDepthCueing=viewMenu->addAction(tr("toggle DepthCueing"),chgl,SLOT(setDepthCueing(bool)),QKeySequence("F4"));
    a->setCheckable(true);
    a->setChecked(false);
    a=togXRayView=viewMenu->addAction(tr("toggle X-Ray View"),chgl,SLOT(setXRayView(bool)),QKeySequence("Alt+F5"));
    a->setCheckable(true);
    a->setChecked(false);
    a=equivHighlighting=viewMenu->addAction("highlight symmetry equivalents",this,SLOT(toggleEQHighlighting(bool)));

    equivHighlighting->setCheckable(true);
    equivHighlighting->setChecked(true);
    a=togBackGr=viewMenu->addAction(tr("toggle background gradient"),chgl,SLOT(setBGGradient(bool)),QKeySequence("F6"));
    a->setCheckable(true);
    a->setChecked(true);

    viewMenu->addAction(dock->toggleViewAction());
    viewMenu->addAction(shxEditorDock->toggleViewAction());
    viewMenu->addAction(chgl->wireButt);
    viewMenu->addSeparator();
    viewMenu->addAction("Redraw structure",this,SLOT(effuenf()),QKeySequence("F5"));
    viewMenu->addAction("Reload structure from current file",this,SLOT(loadAFile()),QKeySequence("Ctrl+F5"));
  }
  {
    QAction *a;
    QToolBar *tb=bottomToolBar= new QToolBar("Atom Selection Tools",this);
    tb->setObjectName("AtomTB");
    tb->setMovable(false);
    tb->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QMenu *m= new QMenu("Configure selection toolbar");
    a=m->addAction("Text only",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::ToolButtonTextOnly);

    a=m->addAction("Icon only",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::ToolButtonIconOnly);


    a=m->addAction("Text beside icon",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::ToolButtonTextBesideIcon);

    a=m->addAction("Text under icon",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::ToolButtonTextUnderIcon);

    m->addSeparator();

    a=m->addAction("Place on top",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::TopToolBarArea+32);

    a=m->addAction("Place on bottom",this,SLOT(configureBottomToolbar()));
    a->setData(Qt::BottomToolBarArea+32);

    a=dialogMenu->addAction(QIcon(":/icons/configure-toolbars.png"),"Configure selection toolbar");
    a->setToolTip("Configure selection toolbar icon/text style");
    a->setMenu(m);

    chgl->toggleDockWidgets= a = tb->addAction(QIcon(":/icons/hidedockwdg.png"),
                                               "Hide Text Windows",this,SLOT(toggDocWid(bool)));
    a->setCheckable(true);
    a->setChecked(false);
    a->setVisible(false);
    toggleOrthoView = a = tb->addAction(QIcon(":/icons/ortho.png"),
		    "Ortho-projection",this,SLOT(toggleOrtho(bool)));
    a->setCheckable(true);
    a->setChecked(false);    
    a->setToolTip("toggle Orthographic projection / central perspective projection");
    chgl->grow=a=tb->addAction(QIcon(":/icons/grow.png"),"GROW ",this,SLOT(grow()));
    packMenu->addAction(a);
    a->setToolTip("Complete the molecule(s)");
    a->setVisible(false);
    chgl->fuse=a=tb->addAction(QIcon(":/icons/fuse.png"),"FUSE ",this,SLOT(fuse()));
    packMenu->addAction(a);
    a->setVisible(false);
    a->setToolTip("Show the asymmetric unit only");
    a=packMenu->addAction(QIcon(":/icons/grow.png"),"recalculate distance matrices",this,SLOT(sdm()));
    packMenu->addAction("Symmetry manager",this,SLOT(symmetryManager()));

    tb->addAction(enterRenameMode);
    enterRenameMode->setIcon(QIcon(":/icons/rename.png"));
    enterRenameMode->setVisible(false);
    tb->addAction(showQPeakLegend);
    showQPeakLegend->setVisible(false);
    a=tb->addAction(QIcon(":/icons/qbond.png"),"Q-Peak-Bonds",chgl,SLOT(updateGL()));
    a->setCheckable(true);
    a->setChecked(true);
    a->setVisible(false);
    chgl->qPeakBonds=a;
    //tb->addAction(sinaAct);
    sinaAct->setVisible(false);
    chgl->addBond=a=tb->addAction(QIcon(":/icons/bind.png"),"Bind selected atoms",this,SLOT(insertBind()));
    a->setVisible(false);
    chgl->killBond=a=tb->addAction(QIcon(":/icons/free.png"),"Free selected atoms",this,SLOT(insertFree()));
    a->setVisible(false);
    chgl->clearSelection=a=tb->addAction(QIcon(":/icons/deselect.png"),"Deselect",chgl,SLOT(disSelection()));
    a->setVisible(false);
    chgl->centroid=a=tb->addAction(QIcon(":/icons/centroid.png"), "Create centroid", this , SLOT(createCentroid()));
    a->setVisible(false);
    chgl->centerSelection=a=tb->addAction(QIcon(":/icons/center.png"),"Center selected atoms",chgl,SLOT(homeXY()));
    a->setCheckable(true);
    a->setChecked(false);
    a->setVisible(false);
    chgl->invSelection=a=tb->addAction(QIcon(":/icons/invsel.png"),"Invert selection",chgl,SLOT(invertSelection()));
    a->setVisible(false);
    chgl->hideNotSelection=a=tb->addAction(QIcon(":/icons/hide.png"),"Hide other atoms",chgl,SLOT(hideNonSelected()));
    a->setVisible(false);
    a=chgl->hideh=tb->addAction(QIcon(":/icons/hideh.png"),"Hide Hydrogen atoms",chgl,SLOT(hideHydrogens()));
    a->setVisible(false);
    chgl->unhide=a=tb->addAction(QIcon(":/icons/show.png"),"Show all hidden",chgl,SLOT(showHidden()));
    connect(a,SIGNAL(triggered()),qpv,SLOT(unhide()));
    a->setVisible(false);
    chgl->shpm1=a=tb->addAction(QIcon(":/icons/part-1.png"),"Hide PART -N ghost",chgl,SLOT(hidePartMinusOne(bool )));
    a->setCheckable(true);
    a->setChecked(false);
    a->setVisible(false);
    a->setToolTip("Hide PART -N symmetry equivalent as ghost atoms");
    chgl->highlightParts=a=tb->addAction(QIcon(":/icons/parts.png"),"Highlight PARTs",chgl,SLOT(updateGL()));
    a->setCheckable(true);
    a->setChecked(false);
    a->setVisible(false);
    a=chgl->delSelAt = tb->addAction(QIcon(":/icons/killselected.png"),"delete selected atoms",this,SLOT(deleteSelectedAtoms()));
    a->setData(-1);
  //  tb->setIconSize(QSize(23,23));
   
    a->setShortcut(Qt::CTRL+Qt::Key_Delete);
    a->setVisible(false);
    addToolBarBreak();
    addToolBar(Qt::TopToolBarArea,tb);
  }
  {
    //QAction *a;
    //a=
    packMenu->addAction("Pack cell",this,SLOT(fillCell()));
    //a=
    packMenu->addAction("Expand all contacts",this,SLOT(expandAll()));
  }
  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(editMenu);
  menuBar()->addMenu(dialogMenu);
  menuBar()->addMenu(viewMenu);
  menuBar()->addMenu(packMenu);
  menuBar()->addMenu(runMenu);
  menuBar()->addMenu(extraMenu);
  QMenu *fcoMenu = new QMenu("Data");
  fcvsfoAct=fcoMenu->addAction("Fc vs Fo",this,SLOT(foversusfc()));
  fcvsfoAct->setDisabled(true);
  calcAct=fcoMenu->addAction("Crystallographic calculator",this,SLOT(calculator()));
  calcAct->setEnabled(true);
  calcAct=fcoMenu->addAction("DSRle",this,SLOT(dsr_gui()));
  calcAct->setEnabled(true);
  menuBar()->addMenu(fcoMenu);

  {
    helpMenu = new QMenu(tr("Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction("Help ", this, SLOT(linkHelp()));
//    helpMenu->addAction(helpDock->toggleViewAction());
    helpMenu->addSeparator();
    helpMenu->addAction(tr("About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
    helpMenu->addAction("Check for new versions now",this,SLOT(checkForUpdates()));
  }

  chgl->rCenter = new QToolButton();
  chgl->rCenter->setText("Reset rotation center");
  connect(chgl->rCenter,SIGNAL(clicked()), chgl,SLOT( rotCenter()));
  chgl->rCenter->hide();

  chgl->invertMouseZoom = new QToolButton(this);
  chgl->invertMouseZoom->setIcon(QIcon(":/icons/invzoom.png"));  
  chgl->invertMouseZoom->setCheckable(true);  
  chgl->invertMouseZoom->setChecked(false);
  chgl->invertMouseZoom->setText("invert mouse zoom");
  chgl->invertMouseZoom->setToolTip("invert mouse zoom (right mouse click and scroll up or down");
  chgl->invertMouseZoom->setIconSize(QSize(32,32));
   

  QToolButton *homeMe = new QToolButton(this);
  homeMe->setIcon(QIcon(":icons/home.png"));
  homeMe->setText("home");
  homeMe->setToolTip("move molecules home");
  homeMe->setIconSize(QSize(32,32));
  //homeMe->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  connect(homeMe,SIGNAL(clicked()),chgl,SLOT(homeXY()));
  qGrowPeak = new QCheckBox("grow Q-peaks");
  connect(qGrowPeak,SIGNAL(toggled(bool)),this,SLOT(growQPeaks(bool)));
  search4duplicates = new QCheckBox("search for duplicates");
  search4duplicates->setChecked(true);
  sLabel = new QLabel;
  sLabel->setFrameStyle(QFrame::Panel  );
  sLabel->setLineWidth(2);
  sLabel->setText(QString("%1").arg(PROGRAM_NAME));
  zoomIn = new QToolButton (this);
  zoomOut = new QToolButton(this);
  zoomIn-> setIcon(QIcon(":/icons/zoom-in.png"));
  zoomOut->setIcon(QIcon(":/icons/zoom-out.png"));
  zoomIn->setText("Zoom +");
  zoomIn->setToolTip("Zoom +");
  zoomIn->setIconSize(QSize(32,32));
  zoomOut->setText("Zoom -");
  zoomOut->setToolTip("Zoom -");
  zoomOut->setIconSize(QSize(32,32));
  //zoomOut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  //zoomIn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  zoomIn->  setAutoRepeat (true);
  zoomOut-> setAutoRepeat (true);
  connect(zoomIn,SIGNAL(pressed ()),this,SLOT(zoomin()));
  connect(zoomOut,SIGNAL(pressed ()),this,SLOT(zoomout()));
  connect(chgl,SIGNAL(no_hw_st()),no_stereo,SLOT(toggle()));

  statusBar();
  statusBar()->addPermanentWidget(balken);
  statusBar()->addPermanentWidget(chgl->rCenter);
  statusBar()->addPermanentWidget(chgl->enviButt);
  statusBar()->addPermanentWidget(qGrowPeak);
  statusBar()->addPermanentWidget(search4duplicates);
  statusBar()->addPermanentWidget(zoomIn);
  statusBar()->addPermanentWidget(zoomOut);
  statusBar()->addPermanentWidget(homeMe);
  statusBar()->addPermanentWidget(chgl->invertMouseZoom);
  shxEditorDock->hide();
  ////////////////////////////// SETTINGS /////////////////////////////////////////////////////////////////////////
  mole.einstellung = new QSettings( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME );

  mole.loadSettings();
  mole.einstellung->beginGroup("Window");
  chgl->invertMouseZoom->setChecked( mole.einstellung->value("InvertMouseZoom").toBool());
  bool bbb=true;
  if (mole.einstellung->contains("Fo-Map"))
      chgl->foact->setChecked(bbb=mole.einstellung->value("Fo-Map").toBool());
  if (mole.einstellung->contains("Fc-Fo-Map"))
      chgl->fofcact->setChecked(bbb=mole.einstellung->value("Fc-Fo-Map").toBool());
  if (mole.einstellung->contains("Map-truncation-type"))
    fxle->maptrunc=mole.einstellung->value("Map-truncation-type").toInt();
  if (mole.einstellung->contains("Map-Line-Transparency"))
    fxle->lintrans = mole.einstellung->value("Map-Line-Transparency").toDouble();
  if (mole.einstellung->contains("Map-Line-Width"))
    fxle->linwidth = mole.einstellung->value("Map-Line-Width").toDouble();
  if (mole.einstellung->contains("Map-Resolution"))
    fxle->rr = mole.einstellung->value("Map-Resolution").toDouble();
  if (mole.einstellung->contains("Map-Weight"))
    fxle->rw = mole.einstellung->value("Map-Weight").toDouble();
  if (mole.einstellung->contains("Map-Lighting"))
    chgl->lighting->setChecked(mole.einstellung->value("Map-Lighting").toBool());
  if (mole.einstellung->contains("Nice-Transparency"))
    chgl->niceTrans->setChecked(mole.einstellung->value("Nice-Transparency").toBool());
  if (mole.einstellung->contains("Solid-Map"))
    chgl->fillMap->setChecked(mole.einstellung->value("Solid-Map").toBool());
  //  connect(equivHighlighting,SIGNAL(toggled(bool)),this,SLOT(toggleEQHighlighting(bool)));

  if (mole.einstellung->contains("HighlightEquivalents")){
      bool bb;
     equivHighlighting->setChecked(bb=mole.einstellung->value("HighlightEquivalents").toBool());
     mole.highlightEquivalents=bb;
  }
  if (mole.einstellung->contains("Calculate-Maps"))
    fxle->doMaps->setChecked(mole.einstellung->value("Calculate-Maps").toBool());
  if (mole.einstellung->contains("ADPs"))
      togElli->setChecked(bbb=mole.einstellung->value("ADPs").toBool());
  chgl->setADP(togElli->isChecked());
  if (mole.einstellung->contains("Tubes"))
    tubeStyle->setChecked(bbb=mole.einstellung->value("Tubes").toBool());
  chgl->setTube(tubeStyle->isChecked());
  if (mole.einstellung->contains("ShowLabels"))
   togLabel->setChecked(bbb=mole.einstellung->value("ShowLabels").toBool());
  chgl->setLabel(togLabel->isChecked());
/*
 * 
    mole.einstellung->setValue("BackGroundGradient",togBackGr->isChecked());
    mole.einstellung->setValue("DepthCueing",togDepthCueing->isChecked());
    mole.einstellung->setValue("Xray-view",togXRayView->isChecked());
*/
  if (mole.einstellung->contains("BackGroundGradient"))
    togBackGr->setChecked(bbb=mole.einstellung->value("BackGroundGradient").toBool());
  chgl->setBGGradient(togBackGr->isChecked());
  if (mole.einstellung->contains("DepthCueing"))
    togDepthCueing->setChecked(bbb=mole.einstellung->value("DepthCueing").toBool());
  chgl->setDepthCueing(togDepthCueing->isChecked());
  if (mole.einstellung->contains("Xray-view"))
    togXRayView->setChecked(bbb=mole.einstellung->value("Xray-view").toBool());
  if (mole.einstellung->contains("WireModelOnRotate"))
    chgl->wireButt->setChecked(mole.einstellung->value("WireModelOnRotate").toBool());
  chgl->setXRayView(togXRayView->isChecked());
  shelxlPath="shelxl";
  if (mole.einstellung->contains("SHELXL-PATH"))
      shelxlPath=mole.einstellung->value("SHELXL-PATH").toString();
  shelxlArgs="";
  if (mole.einstellung->contains("SHELXL-OPTION"))
      shelxlArgs=mole.einstellung->value("SHELXL-Option").toString();
  mole.einstellung->endGroup();
  mole.einstellung->beginGroup("Files");
  dirName = mole.einstellung->value("lastFile").toString();

  QStringList files = mole.einstellung->value("recentFileList").toStringList();
  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    Window *mainWin = qobject_cast<Window *>(widget);
    if (mainWin)
      mainWin->updateRecentFileActions();
  }

  mole.einstellung->endGroup();
  mole.einstellung->beginGroup("Colors");
  fxle->fopc=mole.einstellung->value("FourFoPlusColor",   QColor("#0000FF")).value<QColor>();
  fxle->fomc=mole.einstellung->value("FourFoMinusColor",  QColor("#FFA600")).value<QColor>();
  fxle->dipc=mole.einstellung->value("FourDiffPlusColor", QColor("#00B200")).value<QColor>();
  fxle->dimc=mole.einstellung->value("FourDiffMinusColor",QColor("#CC0000")).value<QColor>();
  chgl->backGroundColor=mole.einstellung->value("BackGroundColor",QColor("lightskyblue")).value<QColor>();
  chgl->labelColor=mole.einstellung->value("LabelColor").value<QColor>();
  mole.einstellung->endGroup();
  int ignore=QMessageBox::Yes;
  mole.einstellung->beginGroup("Updates");
  if (mole.einstellung->contains("IgnoreUpdates"))
     ignore=mole.einstellung->value("IgnoreUpdates").toInt();
  mole.einstellung->endGroup();
  net = new QNetworkAccessManager(this);
  if (ignore!=QMessageBox::No){
  connect(net, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
  reply = net->get(QNetworkRequest(QUrl("http://ewald.ac.chemie.uni-goettingen.de/shelx/revision.php")));
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  extraProgramNames.clear();
  extraProgramPaths.clear();
  extraArguments.clear();
  extraExtensions.clear();
  mole.einstellung->beginGroup("Extra");
  int siz=0;
  siz=mole.einstellung->beginReadArray("ProgramNames");
  for (int i = 0; i < siz; i++) {
    mole.einstellung->setArrayIndex(i);
    extraProgramNames.append(mole.einstellung->value("Name").toString());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();
 mole.einstellung->beginGroup("Extra");

  siz=mole.einstellung->beginReadArray("ProgramPaths");
  for (int i = 0; i < siz; i++) {
    mole.einstellung->setArrayIndex(i);
    extraProgramPaths.append(mole.einstellung->value("Path").toString());
  }
  mole.einstellung->endArray();

  mole.einstellung->endGroup();
  mole.einstellung->beginGroup("Extra");

  siz=mole.einstellung->beginReadArray("ProgramArgs");
  for (int i = 0; i < siz; i++) {
    mole.einstellung->setArrayIndex(i);
    extraArguments.append((Qt::CheckState) mole.einstellung->value("Arg").toInt());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();
  mole.einstellung->beginGroup("Extra");
  siz=mole.einstellung->beginReadArray("ProgramExtensions");
  for (int i = 0; i < siz; i++) {
    mole.einstellung->setArrayIndex(i);
    extraExtensions.append((Qt::CheckState)mole.einstellung->value("Ext").toInt());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("Extra");
  siz=mole.einstellung->beginReadArray("ProgramDetached");
  for (int i = 0; i < siz; i++) {
    mole.einstellung->setArrayIndex(i);
    extraDetach.append((Qt::CheckState)mole.einstellung->value("Detach").toInt());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("Extra");
  mole.einstellung->beginReadArray("AlternativeExtensions");
  for (int i = 0; i < siz; i++) {
      mole.einstellung->setArrayIndex(i);
      extraExt2.append(mole.einstellung->value("ext","").toString());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("Extra");
  mole.einstellung->beginReadArray("CommandLineOptions");
  for (int i = 0; i < siz; i++) {
      mole.einstellung->setArrayIndex(i);
      extraOptions.append(mole.einstellung->value("opt","").toString());
  }
  mole.einstellung->endArray();
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("SaveHint");
  if (mole.einstellung->contains("ShowRenameHint"))
    hintL->setVisible(mole.einstellung->value("ShowRenameHint").toBool());
  mole.einstellung->endGroup();
  {
  extraMenu->addAction("add external program",this,SLOT(extraSettings()));
  }
  if (extraProgramNames.size()) extraMenu->addSeparator();
  QFileIconProvider *epic = new QFileIconProvider();
  extraTB= new QToolBar("External Program Tool Bar",this);
  extraTB->setObjectName("ExtraTB");
  extraTB->setToolButtonStyle(Qt::ToolButtonIconOnly);
  for (int i=0; i<extraProgramNames.size(); i++){
    QAction *a;
    QString icopa=extraProgramPaths.at(i);
    extraIcon=epic->icon(QFileInfo(icopa));

    a=extraMenu->addAction(extraIcon,QString("run %1 !").arg(extraProgramNames.at(i)),this,SLOT(runExtra()));
    a->setData(i);
    extraTB->addAction(a);
  }
  delete epic;
  //extraTB->setIconSize(QSize(23,23));
  addToolBar(extraTB);
  extraOutput = new QDialog(this);
  lt = new QVBoxLayout();
  extraOutPut = new QTextEdit();
  QFont mospfo("Monospace");
  extraOutPut->setFont(mospfo);
  extraOutput->setMinimumWidth(
              QFontMetrics(extraOutPut->font())
              .width("###########################################################################################"));
  extraOutPut->setLineWrapMode(QTextEdit::NoWrap);
  lt->addWidget(extraOutPut);
  extraOutput->setLayout(lt);

  //chgl->pause=true;

  chgl->pause=false;
  setCentralWidget(chgl);
  chgl->updateGL();
  mole.einstellung->beginGroup("Window");

  QByteArray ba,ba_state=mole.einstellung->value("state").toByteArray();
  if (!ba_state.isEmpty()) restoreState(ba_state);
  ba=mole.einstellung->value("geometry").toByteArray();

  if (!ba.isEmpty()) restoreGeometry(ba);
      else {
      QSize size ;
      size=QSize(640,480);
      ediSize=QSize(640,480);
      ediPos=QPoint(0,0);
      if ((size.height())<45||(size.width()<108)) size=QSize(640,480);
      QPoint pos;
      pos=QPoint(1,1);
      move(pos);
      if(size.isNull())
        resize(640, 480);
      else
        resize(size);
  }

  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("Fonts");
  if (mole.einstellung->contains("LabelFont"))
    chgl->myFont = mole.einstellung->value("LabelFont").value<QFont>();
  if (mole.einstellung->contains("EditorFont")){
    QFont fnt;
    fnt = mole.einstellung->value("EditorFont").value<QFont>();

#if (QT_VERSION >= 0x040700)
    fnt.setStyleStrategy(QFont::ForceIntegerMetrics);
#endif
    editor->setFont(fnt);
    xlPutor->setFont(fnt);
  }
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("ToolBar");
  if (mole.einstellung->contains("SelectionToolBarStyle"))
      bottomToolBar->setToolButtonStyle((Qt::ToolButtonStyle) mole.einstellung->value("SelectionToolBarStyle").toInt());
  mole.einstellung->endGroup();

  mole.einstellung->beginGroup("DoNotAskFileSave");
  if (mole.einstellung->contains("DoNotAnnoyMe"))
      doNotAnnoyMe=mole.einstellung->value("DoNotAnnoyMe",false).toBool();
  mole.einstellung->endGroup();
  mole.einstellung->beginGroup("Sorting");
  if (mole.einstellung->contains("SortingOption")){
    sortierWeise = mole.einstellung->value("SortingOption").toInt();
  
  }
  mole.einstellung->endGroup();
  {
      sortGroup = new QActionGroup(this);

      sortMenu = new QMenu("change sort order");
      QAction *a = sortMenu->addAction("label number > atomic number > label suffix");
      sortGroup->addAction(a);
      a->setCheckable(true);
      if (sortierWeise==1)a->setChecked(true);

      a->setData(1);
      a = sortMenu->addAction("atomic number > label number > label suffix");
      sortGroup->addAction(a);
      a->setCheckable(true);
      if (sortierWeise==2)a->setChecked(true);
      a->setData(2);
      a = sortMenu->addAction("SFAC number > label number > label suffix");
      sortGroup->addAction(a);
      a->setCheckable(true);
      if (sortierWeise==3)a->setChecked(true);
      a->setData(3);
      a = sortMenu->addAction("label suffix > atomic number > label number");
      sortGroup->addAction(a);
      a->setCheckable(true);
      if (sortierWeise==4)a->setChecked(true);
      a->setData(4);
      a = sortMenu->addAction("fragment> atomic number > label number > label suffix");
      sortGroup->addAction(a);
      a->setCheckable(true);
      if (sortierWeise==5)a->setChecked(true);
      a->setData(5);
      connect(sortGroup,SIGNAL(triggered(QAction*)),this,SLOT(changeSortierWeise(QAction*)));
      runMenu->insertMenu(soact,sortMenu);
      //delete a;
  }

  mole.einstellung->beginGroup("Labeling");
  if (mole.einstellung->contains("AutoUnused")){
    rufModeBox->setChecked(mole.einstellung->value("AutoUnused").toBool());
  }
  mole.einstellung->endGroup();

  tbiconSize=32;
  mole.einstellung->beginGroup("ToolBarIcons");
  if (mole.einstellung->contains("ToolBarIconSize")){
    tbiconSize=mole.einstellung->value("ToolBarIconSize").toInt();
  }
  mole.einstellung->endGroup();
  tbiconSize=qMin(tbiconSize,32);
  tbiconSize=qMax(tbiconSize,16);
  fito->setIconSize(QSize(tbiconSize,tbiconSize));
  editToolBar->setIconSize(QSize(tbiconSize,tbiconSize));
  vito->setIconSize(QSize(tbiconSize,tbiconSize));
  extraTB->setIconSize(QSize(tbiconSize,tbiconSize));
  bottomToolBar->setIconSize(QSize(tbiconSize,tbiconSize));
  ///
  {
  scalDlg  = new QDialog(this);
  scalDlg->setWindowTitle("Change screen-shot scalefactor");
  scalla = new QGridLayout();
  scalwla = new QLabel("Width: "); 
  scalhla = new QLabel("Height: "); 
  scalsla = new QLabel("Scalefactor: ");
  widthBox        = new QSpinBox();
  heightBox       = new QSpinBox();
  scalefBox = new QDoubleSpinBox();
  widthBox->setMinimum(32);
  widthBox->setMaximum(10000);
  widthBox->setSuffix(" pixels");
  widthBox->setObjectName("widthBox");
  heightBox->setMinimum(32);
  heightBox->setMaximum(10000);
  heightBox->setSuffix(" pixels");
  heightBox->setObjectName("heightBox");
  scalefBox->setMinimum(1);
  scalefBox->setMaximum(400.0);
  scalefBox->setSuffix(" %");
  scalefBox->setValue(scalePic*100.0);
  scalefBox->setObjectName("scalefBox");
//	  scalefBox
  header = new QLabel("<h3>Change screen-shot scalefactor</h3>");
  scalla->addWidget(header   ,0,0,1,3);
  scalla->addWidget(scalwla      ,1,0);
  scalla->addWidget(widthBox ,1,1);
  scalla->addWidget(scalhla      ,2,0);
  scalla->addWidget(heightBox,2,1);
  scalla->addWidget(scalsla      ,3,0);
  scalla->addWidget(scalefBox,3,1);
  scalDlg->setLayout(scalla);
   scalDlg->hide();
  connect(widthBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(heightBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(scalefBox,SIGNAL(valueChanged(double)),this,SLOT(updateScaleDlg()));
  }
  /////////////
  {
  buttonBoxMC = new QDialogButtonBox( QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
  applyMC          = buttonBoxMC->button(QDialogButtonBox::Apply);
  applyDF          = buttonBoxMC->addButton("Apply defaults",QDialogButtonBox::ActionRole);
  wl= new QLabel("Factor to downweight weak data");
  pl= new QLabel("Map precision: ");
  dl= new QLabel("Fo-Fc map: ");
  ol= new QLabel("F-observed map: ");
  rl= new QLabel("Map radius: ");
  sl= new QLabel("Map truncation type: ");
  lw= new QLabel("Line width: ");
  ltz= new QLabel("Line transparency: ");
  md = new QDialog(this);
  md->setWindowTitle("Map Control");
  connect(applyMC , SIGNAL(clicked()), this , SLOT(controlMap()));
  connect(applyMC , SIGNAL(clicked()), this , SLOT(openMapControl()));
  connect(applyDF , SIGNAL(clicked()), this , SLOT(mapDefault()));
  connect(buttonBoxMC, SIGNAL(rejected()), md, SLOT(hide()));

  mapprec = new QDoubleSpinBox();
  mapprec->setMinimum(1.1);
  mapprec->setMaximum(8.0);
  mapprec->setValue(fxle->rr);
  mapprec->setSingleStep(0.1);

  lineTrans = new QDoubleSpinBox();
  lineTrans->setMinimum(0.1);
  lineTrans->setMaximum(1.0);
  lineTrans->setValue(fxle->lintrans);
  lineTrans->setSingleStep(0.1);

  lineWidth = new QDoubleSpinBox();
  lineWidth->setMinimum(0.15);
  lineWidth->setMaximum(4.0);
  lineWidth->setValue(fxle->linwidth);
  lineWidth->setSingleStep(0.1);

  weak = new QDoubleSpinBox();
  weak->setMinimum(0.1);
  weak->setMaximum(8.0);
  weak->setValue(fxle->rw);
  weak->setSingleStep(0.1);

  difmaps = new QDoubleSpinBox();
  difmaps->setMinimum(0);
  difmaps->setMaximum(9999);
  difmaps->setValue(fabs(fxle->iso[1]));
  difmaps->setSingleStep(fabs(fxle->iso[1])/10.0); 
  difmaps->setSuffix("e/A^3");

  fomaps = new QDoubleSpinBox();
  fomaps->setMinimum(0);
  fomaps->setMaximum(9999);
  fomaps->setValue(fxle->iso[0]);
  fomaps->setSingleStep(fxle->iso[0]/10.0);
  fomaps->setSuffix("e/A^3");
  
  maprad = new QDoubleSpinBox();
  maprad->setValue(fxle->map_radius);
  maprad->setMinimum(0);
  maprad->setMaximum(25.0);
  maprad->setSuffix("A");
  maprad->setSingleStep(0.3);
  mapSchnitt=  new QComboBox();
  mapSchnitt->addItem("one complete unit cell",0);
  mapSchnitt->addItem("sphere around rotation center",1);
  mapSchnitt->addItem("1.41A around visible atoms or peaks",2);
  mapSchnitt->setCurrentIndex(fxle->maptrunc);
  schnittart(fxle->maptrunc);
  connect(mapSchnitt,SIGNAL(currentIndexChanged(int)),this,SLOT(schnittart(int)));
  mdl = new QGridLayout();
  mdl->addWidget(mapprec,0,1);
  mdl->addWidget(weak,1,1);
  mdl->addWidget(difmaps,2,1);
  mdl->addWidget(fomaps,3,1);
  mdl->addWidget(maprad,5,1);
  mdl->addWidget(mapSchnitt,6,1);
  mdl->addWidget(lineTrans,7,1);
  mdl->addWidget(lineWidth,8,1);
  mdl->addWidget(chgl->lighting,9,1);
  mdl->addWidget(chgl->niceTrans,9,0);
  mdl->addWidget(chgl->fillMap,9,2);
  connect(chgl->fillMap,SIGNAL(stateChanged(int)),chgl,SLOT(updateGL()));
  connect(chgl->niceTrans,SIGNAL(stateChanged(int)),chgl,SLOT(updateGL()));
  connect(chgl->lighting,SIGNAL(stateChanged(int)),chgl,SLOT(updateGL()));
  mdl->addWidget(pl,0,0);
  mdl->addWidget(wl,1,0);
  mdl->addWidget(dl,2,0);
  mdl->addWidget(ol,3,0);
  mdl->addWidget(rl,5,0);
  mdl->addWidget(sl,6,0);
  mdl->addWidget(ltz,7,0);
  mdl->addWidget(lw,8,0);

  QTabWidget *tabWidget = new QTabWidget(this);
  QWidget *gwmd,*admd;
  gwmd = new QWidget();

  gwmd->setLayout(mdl);
  admd = new QWidget();
  QVBoxLayout *mdmain= new QVBoxLayout;
  tabWidget->addTab(gwmd,"General");
  tabWidget->addTab(admd,"Advanced");


  foPlusButton =new QPushButton("Fo positive color");
  foPlusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->fopc.name())
		  .arg( fxle->fopc.darker(200).name())
		  .arg( fxle->fopc.lighter(100).name())
		  .arg("#ffffff"));
  connect(foPlusButton,SIGNAL(pressed()),this,SLOT(colorDLGFOP()));
  defColorButton =new QPushButton("Default color scheme");
  connect(defColorButton,SIGNAL(pressed()),this,SLOT(defaultColors()));
  foMinusButton =new QPushButton("Fo negative color");
  foMinusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->fomc.name())
		  .arg( fxle->fomc.darker(200).name())
		  .arg( fxle->fomc.lighter(100).name())
		  .arg("#ffffff"));
		  //.arg((fxle->fomc.value()<90)?"#ffffff":"#000000"));
  connect(foMinusButton,SIGNAL(pressed()),this,SLOT(colorDLGFOM()));

  diffMinusButton =new QPushButton("Fo-Fc negative color");
  diffMinusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->dimc.name())
		  .arg( fxle->dimc.darker(200).name())
		  .arg( fxle->dimc.lighter(100).name())
		  .arg("#ffffff"));
		  //.arg((fxle->dimc.value()<90)?"#ffffff":"#000000"));
  connect(diffMinusButton,SIGNAL(pressed()),this,SLOT(colorDLGDIM()));

  diffPlusButton =new QPushButton("Fo-Fc positive color");
  diffPlusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->dipc.name())
		  .arg( fxle->dipc.darker(200).name())
		  .arg( fxle->dipc.lighter(100).name())
		  .arg("#ffffff"));
		  //.arg((fxle->dipc.value()<90)?"#ffffff":"#000000"));
  connect(diffPlusButton,SIGNAL(pressed()),this,SLOT(colorDLGDIP()));

  amdl = new QVBoxLayout();
  amdl->addWidget(foPlusButton);
  amdl->addWidget(foMinusButton);
  amdl->addWidget(diffPlusButton);
  amdl->addWidget(diffMinusButton);
  amdl->addWidget(defColorButton);
  QPushButton *jnkButton=new QPushButton("fractal dimension analysis");
  connect(jnkButton,SIGNAL(pressed()),this,SLOT(jnk()));
  amdl->addWidget(jnkButton);
  admd->setLayout(amdl);
  mdmain->addWidget(tabWidget);
  mdmain->addWidget(buttonBoxMC);

  md->setLayout(mdmain);
  md->setModal (false);
  statusBar()->addPermanentWidget(fxle->doMaps);
  QCheckBox *hideToolBars = new QCheckBox("Hide Tool Bars");
  hideToolBars->setChecked(false);
  connect(hideToolBars,SIGNAL(toggled(bool)),this,SLOT(hideToolz(bool)));
  statusBar()->addPermanentWidget(hideToolBars);
  statusBar()->addPermanentWidget(sLabel);
  connect(fxle->doMaps,SIGNAL(toggled(bool)),this , SLOT(doMapsNow(bool)));
  connect(fxle,SIGNAL(bigmessage(const QString&)),this,SLOT(infoKanalNews(const QString&)));
//  fxle->doMaps->hide();
  md->hide();
  }
  toggDocWid(true);
  raise();
  double vang=0;

#if (QT_VERSION >= 0x040600)
  dirName = (QProcessEnvironment::systemEnvironment ().value("PWD",dirName));
#else
  {
  QStringList env = QProcess::systemEnvironment();
  int da = env.indexOf(QRegExp("^PWD=.*$"));
  if (da>-1) {
    dirName = QString(env.at(da)).remove(0,4);  
  }
  }
#endif
  QString ffna="";
  startFused=false;
  int argc=QCoreApplication::arguments().size();
  if (argc>1){
    for (int i=1;i<argc;i++){
      if ((QCoreApplication::arguments().at(i).contains(".ins",Qt::CaseInsensitive)) ||
                      (QCoreApplication::arguments().at(i).contains(".res",Qt::CaseInsensitive)))
         ffna=QCoreApplication::arguments().at(i);
      if ((QCoreApplication::arguments().at(i)=="-fused")) startFused=true;
      if ((QCoreApplication::arguments().at(i)=="-HKLMAX")) fxle->HKLMX = QCoreApplication::arguments().at(i+1).toInt();
      if ((QCoreApplication::arguments().at(i)=="-viewangle")) vang = QCoreApplication::arguments().at(i+1).toDouble();
      if ((QCoreApplication::arguments().at(i)=="-xrd")) chgl->xrdist = QCoreApplication::arguments().at(i+1).toDouble();
      if ((QCoreApplication::arguments().at(i)=="-wt")) {
	chgl->dratpause = QCoreApplication::arguments().at(i+1).toInt();
    printf("Wire pause %f seconds\nThis is the time the wired model is shown after rotation, zoom etc.",chgl->dratpause*0.001);
      }
    }
  }
    chgl->pause=false;
    chgl->updateGL();
    if (vang>0.0) chgl->setViewAngle(vang);

    mole.einstellung->beginGroup("ViewSettings");
    if (mole.einstellung->contains("OrthographicProjection")) {
    bool b=mole.einstellung->value("OrthographicProjection").toBool();
    toggleOrthoView->setChecked(b);
    toggleOrtho(b);
  }
  mole.einstellung->endGroup();
  qPeakDock->setVisible(true);
  if (!ffna.isEmpty()) {
      dirName=ffna;
      QTimer::singleShot(200,this,SLOT(loadAFile()));
  }else {
      QTimer::singleShot(200,qPeakDock,SLOT(hide()));
  }
}

Window::~Window() {

  if (calcmenu!=NULL) delete calcmenu;//only delete it if it has bee created. (seg fault)
  delete sortMenu;
  delete hidwillpse;
  delete fxle;
}

void Window::setMonoQ(bool vis){
//  qDebug()<<"MonoQrom"<<vis;
  mole.monoQrom=!vis;
  chgl->murx=1;
  chgl->updateGL();
}

void Window::trick(){
    bool vis=qPeakDock->isVisible();
    qPeakDock->setVisible(!vis);
    if (vis)
        QTimer::singleShot(200,qPeakDock,SLOT(show()));
    else
        QTimer::singleShot(200,qPeakDock,SLOT(hide()));
}

void Window::omitsome(const QString &s){
//qDebug()<<"omi->t"<<s.size();
        QList<Omit> schonWeg;        
	Omit o;
        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
	QStringList sli;
	while (!cursor.isNull()){
	  cursor = document->find(QRegExp("OMIT\\s+\\d+\\s+\\d+\\s+\\d+"),cursor);
	  if (!cursor.isNull()){
	    sli=cursor.selectedText().split(" ",QString::SkipEmptyParts);
	    o.h=sli.at(1).toInt();
	    o.k=sli.at(2).toInt();
	    o.l=sli.at(3).toInt();
	    schonWeg.append(o);
	  }
	}
	QString sm=s;
	for (int i=0; i<schonWeg.size(); i++){
	  sm.remove(QString("OMIT %1 %2 %3\n").arg(schonWeg.at(i).h).arg(schonWeg.at(i).k).arg(schonWeg.at(i).l)); 
	}
	cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
	if (!sm.isEmpty()){
	  cursor = document->find("UNIT",cursor);
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
	  cursor.insertText(sm);
	  fileSave();
	}
}

void Window::foversusfc(){

  QString fcfname = dirName;
  fcfname.replace(QRegExp(".res$|.ins$", Qt::CaseInsensitive),".fcf");
  FcVsFo *d = new FcVsFo(fcfname,mole.cell.a, mole.cell.b, mole.cell.c, mole.cell.al, mole.cell.be, mole.cell.ga, mole.cell.wave);

  connect(d,SIGNAL(weg(const QString &)),this,SLOT(omitsome(const QString &)));
  d->exec();
// qDebug()<<"gut";
  delete d;
//  qDebug()<<"sehr gut";


}

void Window::colorDLGFOP(){
  QColor color;
  color=QColorDialog::getColor(fxle->fopc, this);
  if (color.isValid()) {
    fxle->fopc=color;
    foPlusButton->setStyleSheet(QString(
			    "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			    "border: 1px solid #000000;"
			    "border-radius: 9px;"
			    "color: %4;"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			    "}"
			    "QPushButton:hover {"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			    "}"
			    "QPushButton:flat {"
			    "    border: none; /* no border for a flat push button */"
			    "}"
			    )
		    .arg(color.name())
		    .arg(color.darker(200).name())
		    .arg(color.lighter(100).name())
		    .arg("#ffffff"));
  }
  mole.einstellung->beginGroup("Colors");
  mole.einstellung->setValue("FourFoPlusColor",   fxle->fopc);
  mole.einstellung->setValue("FourFoMinusColor",  fxle->fomc);
  mole.einstellung->setValue("FourDiffPlusColor", fxle->dipc);
  mole.einstellung->setValue("FourDiffMinusColor",fxle->dimc);
  mole.einstellung->endGroup();
  md->update();

}

void Window::colorDLGFOM(){
  QColor color;
  color=QColorDialog::getColor(fxle->fopc, this);
  if (color.isValid()) {
    fxle->fomc=color;

    foMinusButton->setStyleSheet(QString(
			    "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			    "border: 1px solid #000000;"
			    "border-radius: 9px;"
			    "color: %4;"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			    "}"
			    "QPushButton:hover {"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			    "}"
			    "QPushButton:flat {"
			    "    border: none; /* no border for a flat push button */"
			    "}"
			    )
		    .arg(color.name())
		    .arg(color.darker(200).name())
		    .arg(color.lighter(100).name())
		    .arg("#ffffff"));
  }
  mole.einstellung->beginGroup("Colors");
  mole.einstellung->setValue("FourFoPlusColor",   fxle->fopc);
  mole.einstellung->setValue("FourFoMinusColor",  fxle->fomc);
  mole.einstellung->setValue("FourDiffPlusColor", fxle->dipc);
  mole.einstellung->setValue("FourDiffMinusColor",fxle->dimc);
  mole.einstellung->endGroup();
  md->update();

}

void Window::colorDLGDIP(){
  QColor color;
  color=QColorDialog::getColor(fxle->fopc, this);
  if (color.isValid()) {
    fxle->dipc=color;

    diffPlusButton->setStyleSheet(QString(
			    "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			    "border: 1px solid #000000;"
			    "border-radius: 9px;"
			    "color: %4;"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			    "}"
			    "QPushButton:hover {"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			    "}"
			    "QPushButton:flat {"
			    "    border: none; /* no border for a flat push button */"
			    "}"
			    )
		    .arg(color.name())
		    .arg(color.darker(200).name())
		    .arg(color.lighter(100).name())
		    .arg("#ffffff"));
  }
  mole.einstellung->beginGroup("Colors");
  mole.einstellung->setValue("FourFoPlusColor",   fxle->fopc);
  mole.einstellung->setValue("FourFoMinusColor",  fxle->fomc);
  mole.einstellung->setValue("FourDiffPlusColor", fxle->dipc);
  mole.einstellung->setValue("FourDiffMinusColor",fxle->dimc);
  mole.einstellung->endGroup();
  md->update();

}

void Window::colorDLGDIM(){
  QColor color;
  color=QColorDialog::getColor(fxle->fopc, this);
  if (color.isValid()) {
    fxle->dimc=color;

    diffMinusButton->setStyleSheet(QString(
			    "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			    "border: 1px solid #000000;"
			    "border-radius: 9px;"
			    "color: %4;"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			    "}"
			    "QPushButton:hover {"
			    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 0.5 %1, stop: 1 %2);"
			    "}"
			    "QPushButton:flat {"
			    "    border: none; /* no border for a flat push button */"
			    "}"
			    )
		    .arg(color.name())
		    .arg(color.darker(200).name())
		    .arg(color.lighter(100).name())
		    .arg("#ffffff"));
  }
  mole.einstellung->beginGroup("Colors");
  mole.einstellung->setValue("FourFoPlusColor",   fxle->fopc);
  mole.einstellung->setValue("FourFoMinusColor",  fxle->fomc);
  mole.einstellung->setValue("FourDiffPlusColor", fxle->dipc);
  mole.einstellung->setValue("FourDiffMinusColor",fxle->dimc);
  mole.einstellung->endGroup();
  md->update();

}

void Window::defaultColors(){
  fxle->fopc = QColor("#0000FF");
  fxle->fomc = QColor("#FFA600");
  fxle->dipc = QColor("#00B200");
  fxle->dimc = QColor("#CC0000");
  foPlusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->fopc.name())
		  .arg( fxle->fopc.darker(200).name())
		  .arg( fxle->fopc.lighter(100).name())
		  .arg("#ffffff"));

  foMinusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->fomc.name())
		  .arg( fxle->fomc.darker(200).name())
		  .arg( fxle->fomc.lighter(100).name())
		  .arg("#ffffff"));

  diffMinusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->dimc.name())
		  .arg( fxle->dimc.darker(200).name())
		  .arg( fxle->dimc.lighter(100).name())
		  .arg("#ffffff"));

  diffPlusButton->setStyleSheet(QString(
			  "QPushButton {"
			  "font-size:16px;"
			  "font-weight:bold;"
			  "border: 1px solid #000000;"
			  "border-radius: 9px;"
			  "color: %4;"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %3, stop: 1 %2);"
			  "}"
			  "QPushButton:hover {"
			  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 0.5 %2, stop: 1 %1);"
			  "border: 0px"
			  "}"
			  "QPushButton:flat {"
			  "    border: none; /* no border for a flat push button */"
			  "}"
			  )
		  .arg( fxle->dipc.name())
		  .arg( fxle->dipc.darker(200).name())
		  .arg( fxle->dipc.lighter(100).name())
		  .arg("#ffffff"));
  mole.einstellung->beginGroup("Colors");
  mole.einstellung->setValue("FourFoPlusColor",   fxle->fopc);
  mole.einstellung->setValue("FourFoMinusColor",  fxle->fomc);
  mole.einstellung->setValue("FourDiffPlusColor", fxle->dipc);
  mole.einstellung->setValue("FourDiffMinusColor",fxle->dimc);
  mole.einstellung->endGroup();
  md->update();

} 

void Window::loadAFile(){
loadFile(dirName);
}

void Window::openSaveHistory(){
  if ((dirName.isEmpty())||(!QFile::exists(dirName))) return;
  SaveHistoryWidget *shw = new SaveHistoryWidget(dirName);
  if (QDialog::Accepted==shw->exec()) {
    loadFile(dirName);
  }
  delete shw;
}

void Window::toggleEQHighlighting(bool b){
    mole.highlightEquivalents = b;
    chgl->initLists();

    chgl->updateGL();
}

void Window::toggleOrtho(bool ov){
  bool werdi=chgl->pause;
  chgl->pause=false;
  chgl->updateGL();
  chgl->setViewAngle((ov)?0.1:29.0);
  chgl->pause=werdi;
  mole.einstellung->beginGroup("ViewSettings");
  mole.einstellung->setValue("OrthographicProjection",ov);
  mole.einstellung->endGroup();
}

void Window::growQPeaks(bool qpg){
    mole.growQPeak=qpg;
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();
    chgl->updateGL();
}

void Window::openMapControl(){
  weak->setValue(fxle->rw);
  mapprec->setValue(fxle->rr);
  difmaps->setValue(fabs(fxle->iso[1]));
  difmaps->setSingleStep(fabs(fxle->iso[1])/10.0);
  lineTrans->setValue(fxle->lintrans);
  lineWidth->setValue(fxle->linwidth);
  fomaps->setValue(fxle->iso[0]);
  fomaps->setSingleStep(fxle->iso[0]/10.0);
  maprad->setValue(fxle->map_radius);
  mapSchnitt->setCurrentIndex(fxle->maptrunc);
  md->show();
}

void Window::mapDefault(){
  fxle->map_radius=map_radius= 5.0;
  fxle->maptrunc=maptrunc = 2;
  fxle->rr=range= 2.0;
  fxle->rw=weight= 1.0;
  fxle->lintrans= 0.8;
  fxle->linwidth= 0.5;
  fxle->iso[1]= fxle->sigma[0] * 2.7;
  fxle->iso[0]= fxle->sigma[1] * 1.2;
  chgl->niceTrans->setChecked(false);
  chgl->lighting->setChecked(true);
  chgl->fillMap->setChecked(false);
  weak->setValue(fxle->rw);
  mapprec->setValue(fxle->rr);
  difmaps->setValue(fabs(fxle->iso[1]));
  difmaps->setSingleStep(fabs(fxle->iso[1])/10.0);
  lineTrans->setValue(fxle->lintrans);
  lineWidth->setValue(fxle->linwidth);
  fomaps->setValue(fxle->iso[0]);
  fomaps->setSingleStep(fxle->iso[0]/10.0);
  maprad->setValue(fxle->map_radius);
  mapSchnitt->setCurrentIndex(fxle->maptrunc);
  controlMap();
}

void Window::schnittart(int trunc){
  if (trunc==1) maprad->setEnabled(true);
  else maprad->setEnabled(false);
}

void Window::spellCheck(){
  QString t=editor->toPlainText();
  if (t==altText) return;
  //  return;
//  printf("spell on me!\n");
  altText=t;
  QRegExp rx=QRegExp("\\s|_");
  QRegExp nch=QRegExp("[^\"\\-~#%&$'`Â§()+\\*\\./@\\[\\]^\\\\{}\\|<>A-Za-z0-9]+");
  QString s,s0;
  QString meld;
  editor->errorInLine.clear();
  bool cont=false;
  QStringList sl;
  for (int i= 0;i< editor->blocks.size(); i++){  
    sl = editor->blocks.at(i).split(" ",QString::SkipEmptyParts);
    if (sl.isEmpty())continue;
    if ((cont)&&(editor->blocks.at(i).contains(QRegExp("^\\S+")))){
      editor->errorInLine.append(i-1);
      editor->errorInLine.append(i);
      meld+=QString("ERROR in line %1: Bad continuation line!<br>").arg(i);
    }
    cont=false;
    if (sl.isEmpty())continue;
    cont= (!(sl.at(0).contains(QRegExp("(^REM)|(^TITL)",Qt::CaseInsensitive)))&&(editor->blocks.at(i).contains(QRegExp("=\\s*$"))));
    if(editor->blocks.at(i).size()>80){
      editor->errorInLine.append(i);
      meld+=QString("ERROR in line %1: Line is to long for SHELXL!<br>").arg(i+1);
    }
    if (editor->blocks.at(i).startsWith(" ")) continue;
    if (isacommand(sl.at(0))==-1){
      s0=sl.at(0).section(rx,0,0);
      if (s0.size()>4) {
	meld+=QString("ERROR in line %1: Atom label too long %2!<br>").arg(i+1).arg(sl.at(0));
      editor->errorInLine.append(i);
//	es.append(s0);
      }
      if (sl.at(0).contains(nch)) {
	meld+=QString("ERROR in line %1: Not allowed characters in atom label %2!<br>").arg(i+1).arg(sl.at(0));
      editor->errorInLine.append(i);
  //      es.append(sl.at(0));
      }
      bool bb=true;
      if (sl.size()>1) {
	s=sl.at(1);
	int ii = s.toInt(&bb);
	if ((sl.size()>3)&&((ii<1)||(!bb))) {
//	  es.append(QString(" %1 ").arg(s));
	  meld+=QString("ERROR in line %1: Scatering-factor number is missing or out of range!<br>").arg(i+1);
      editor->errorInLine.append(i);
	}
      }
    }
  }
  FehlerTeufel= !meld.isEmpty();
  if (FehlerTeufel) infoKanalNews("<font color=red size=+1>"+meld+"</font>");
  else infoKanalNews("");
  spellCheckact->setVisible(FehlerTeufel);
  spellCheckact2->setVisible(FehlerTeufel);
  editor->update();
//  printf("spell me on!\n");
}

void Window::exportCoordinates(){
//exporting frac coordinates in a RES file
  QString exportName = dirName;
  exportName.replace(QRegExp("(.ins)|(.res)",Qt::CaseInsensitive),"_EXPORTED.res");
  QString fn=QFileDialog::getSaveFileName(this, tr("Export all visible atoms"), exportName, tr("SHELX-files (*.res *.ins)"));
  if (fn.isEmpty()) return;
  QFile f(fn);
  QString body;
  body.append(QString("TITL %1\nCELL  %2 %3 %4 %5 %6 %7 %8\n%9\n")
		  .arg(title) 
		  .arg(mole.cell.wave  )
		  .arg(mole.cell.a  )
		  .arg(mole.cell.b  )
		  .arg(mole.cell.c  )
		  .arg(mole.cell.al )
		  .arg(mole.cell.be )
		  .arg(mole.cell.ga )
		  .arg(mole.cell.Z )
	     );
  for (int i=1; i<mole.cell.symmops.size();i++){
  body.append(QString("SYMM %1\n").arg(mole.symmcode2human(i)));
  }
  body.append(QString("SFAC "));
  for (int i=0; i<sfac.size(); i++){
  body.append(QString("%1 ").arg(mole.pse(sfac.at(i))));  
  }
  body.append("\n");
  body.append(unitNeu);
  body.append(
		  "\n\nREM ######################################################\n"
		  "REM This file exported by ShelXle is for information or \n"
		  "REM visualiztion purposes only. You may run into trouble \n"
		  "REM if you try to refine it against data.\n"
		  "REM DISABLE_REFINE\n"
		  "REM ######################################################\n\n");
  body.append("FVAR 1.00 ");
  for (int i=0; i<fvar.size();i++){
    body.append(QString("%1 ").arg(fvar.at(i)));
  }
  body.append("\n\n");
  int part=0,resi=0;
  chgl->rehide();
  for (int i=0; i< mole.showatoms.size(); i++){
    if (mole.showatoms.at(i).hidden) continue;
    if ((mole.showatoms.at(i).resiNr!=resi))
        body.append(QString("RESI %1 %2\n").arg(mole.showatoms.at(i).resiNr).arg(mole.showatoms.at(i).ResiClass));
    if ((mole.showatoms.at(i).part!=part))
        body.append(QString("PART %1\n").arg(mole.showatoms.at(i).part));
    if (mole.showatoms.at(i).an==-1) 
      body.append(QString("%1 %2 %3 %4 %5 %6 %7 %8\n")
		      .arg(mole.showatoms.at(i).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0),-5)
		      .arg(qMax(1,sfac.indexOf(mole.showatoms.at(i).an)+1))
		      .arg(mole.showatoms.at(i).frac.x,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.y,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.z,10,'f',6)
                      .arg(mole.showatoms.at(i).sof_org,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m33,9,'f',5)
		      .arg(mole.showatoms.at(i).peakHeight,9,'f',2)
		 );
    else {if (mole.showatoms.at(i).uf.m22==-666.0){
      body.append(QString("%1 %2 %3 %4 %5 %6 %7\n")
		      .arg(mole.showatoms.at(i).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0),-5)
		      .arg(qMax(1,sfac.indexOf(mole.showatoms.at(i).an)+1))
		      .arg(mole.showatoms.at(i).frac.x,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.y,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.z,10,'f',6)
                      .arg(mole.showatoms.at(i).sof_org,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m33,9,'f',5)
		 );
    }else{
      body.append(QString("%1 %2 %3 %4 %5 %6 %7 %8 =\n    %9 %10 %11 %12\n")
		      .arg(mole.showatoms.at(i).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0),-5)
		      .arg(qMax(1,sfac.indexOf(mole.showatoms.at(i).an)+1))
		      .arg(mole.showatoms.at(i).frac.x,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.y,10,'f',6)
		      .arg(mole.showatoms.at(i).frac.z,10,'f',6)
                      .arg(mole.showatoms.at(i).sof_org,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m11,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m22,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m33,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m23,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m13,9,'f',5)
		      .arg(mole.showatoms.at(i).uf.m12,9,'f',5)
		 );
    }}
    part=mole.showatoms.at(i).part;
    resi=mole.showatoms.at(i).resiNr;
  }

//brauche 
  body.append("\n\n"); 
  QString eqi=mole.symmcode2human(mole.usedSymmetry);
  eqi.replace("&raquo;","EQIV $");
  eqi.replace(":","");
  eqi.replace("<b>","");
  eqi.replace("</b>","");
  eqi.replace("<br>","\n");
  body.append(eqi);
  body.append("\n\n");
  for (int i =0; i<mole.showbonds.size();i++){
    if (mole.showbonds.at(i).ato1->hidden) continue; 
    if (mole.showbonds.at(i).ato2->hidden) continue;
    QString s1="",s2="",s3="";
    s1=mole.showbonds.at(i).ato1->Label;
    s2=mole.showbonds.at(i).ato2->Label;
    s1=s1.section(QString::fromUtf8("»"),0,0);
    s2=s2.section(QString::fromUtf8("»"),0,0);
    if ((mole.showbonds.at(i).ato2->symmGroup)||(mole.showbonds.at(i).ato1->symmGroup))
        s3.append(QString("_$%1").arg(qMax(mole.showbonds.at(i).ato1->symmGroup,mole.showbonds.at(i).ato2->symmGroup)));
    body.append(QString("BIND%3 %1 %2\n")
		    .arg(s1)
                    .arg(s2)
                .arg(s3));
  }
  body.append("\nHKLF 0 !don't refine this\nEND\n");
  bool success = f.open(QIODevice::WriteOnly|QIODevice::Text);
  if (success){
    f.write(body.toLatin1());
    f.close();
  }
}

void Window::jumpToError(){
  QTextCursor c = editor->textCursor();
  if (!editor->errorInLine.isEmpty()){
  c.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
  c.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,editor->errorInLine.first());
  editor->setTextCursor(c);
  editor->ensureCursorVisible ();
  }
}

void Window::changeIconSize(){
  bool ok;
  int tbs=tbiconSize;

#if (QT_VERSION >= 0x040500)
  tbs=QInputDialog::getInt(this,"Change tool bar icon size...","Tool bar icon size:",tbiconSize,16,32,1,&ok); 
#else
  tbs=QInputDialog::getInteger(this,"Change tool bar icon size...","Tool bar icon size:",tbiconSize,16,32,1,&ok);
#endif  
  if (ok) tbiconSize=tbs;
  mole.einstellung->beginGroup("ToolBarIcons");
  mole.einstellung->setValue("ToolBarIconSize",tbiconSize);
  mole.einstellung->endGroup();
  fito->setIconSize(QSize(tbiconSize,tbiconSize));
  editToolBar->setIconSize(QSize(tbiconSize,tbiconSize));
  vito->setIconSize(QSize(tbiconSize,tbiconSize));
  extraTB->setIconSize(QSize(tbiconSize,tbiconSize));
  bottomToolBar->setIconSize(QSize(tbiconSize,tbiconSize));
  update();
}

void Window::doMapsNow(bool b){
  if (!b) {
    fxle->killmaps(); 
    fxle->n5=0;
    mapcontrol->setVisible(false);
    chgl->updateGL();
    return;
  } else {
    QString fouName;
    fouName=dirName;
    fouName.chop(3);
    fouName.append("fcf");
    bool hamrsho=((fxle->iso[0]==fxle->iso[1])&&(fxle->iso[0]==0));
    if (!fxle->loadFouAndPerform(fouName.toLocal8Bit(),hamrsho)){
      //infoKanalNews(QString("<font color=red>Could not load %1!</font><br>").arg(fouName));
      fxle->deleteLists();
      mapcontrol->setVisible(false);
    } else {        
        mapcontrol->setVisible(true);
        if (!hamrsho) fxle->inimap();
    }
    chgl->updateGL();
  }
} 

void Window::controlMap(){
  fxle->maptrunc=mapSchnitt->currentIndex (); 
  if ((fxle->rw!=weak->value())||(fxle->rr!=mapprec->value())){
    fxle->rr =  mapprec->value();  
    fxle->rw = weak->value();

    QString fouName;
    fouName=dirName;
    fouName.chop(3);
    fouName.append("fcf");
    if (!fxle->loadFouAndPerform(fouName.toLocal8Bit(),false)){
      //infoKanalNews(QString("<font color=red>Could not load %1!</font><br>").arg(fouName));
      fxle->deleteLists(); 
    }
  }    

  fxle->lintrans = lineTrans->value();
  fxle->linwidth = lineWidth->value();
  fxle->map_radius = maprad->value();
  fxle->iso[0] = fomaps->value();
  fxle->iso[1] = -difmaps->value();
  fxle->inimap();
  chgl->updateGL();
  }

void Window::jnk(){
  fxle->maptrunc=mapSchnitt->currentIndex (); 
  if ((fxle->rw!=weak->value())||(fxle->rr!=mapprec->value())){
    fxle->rr =  mapprec->value();  
    fxle->rw = weak->value();

    QString fouName;
    fouName=dirName;
    fouName.chop(3);
    fouName.append("fcf");
    if (!fxle->loadFouAndPerform(fouName.toLocal8Bit(),false)){
      //infoKanalNews(QString("<font color=red>Could not load %1!</font><br>").arg(fouName));
      fxle->deleteLists(); 
    }
  }    

  fxle->lintrans = lineTrans->value();
  fxle->linwidth = lineWidth->value();
  fxle->map_radius = maprad->value();
  fxle->iso[0] = fomaps->value();
  fxle->iso[1] = -difmaps->value();
  fxle->jnk();
 // fxle->inimap();
 // chgl->updateGL();
  }
void Window::deleteAllQ(){

    if (sfac.isEmpty()) return;
    QTextCursor cursor = editor->textCursor();
    wholeLine->setChecked(false);
    replace->setText("");
    search->setText("^Q\\S+\\s+1\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s*");
    for (int i = mole.asymm.size()-1; i>=0; i-- ){
      if (mole.asymm.at(i).an==-1){
	QString s =
		QString("<font color=magenta>Deleted atom: %1 part:%2 residue:%3 %4 </font> Please save to update structure view.")
                .arg(mole.asymm.at(i).Label)
                .arg(mole.asymm.at(i).part)
                .arg(mole.asymm.at(i).resiNr)
                .arg(mole.asymm.at(i).ResiClass);
	infoKanalNews(s);
        deletedAtom.append(mole.asymm.at(i));
        mole.asymm.removeAt(i);
      }
    }
    replaceAll();
    search->setText("");
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);
    mole.packer(mole.sdmcompleter());
    mole.showbonds =
            mole.connecting(mole.showatoms);
    //if (mol!=chgl->mol) chgl->setMolecule(mole);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();
    chgl->pause=false;
    chgl->updateBondActions();
    chgl->updateGL();
}

void Window::sdm(){
if (!(chgl->fuse->isVisible()||chgl->grow->isVisible ())){
    chgl->fuse->setVisible(true);
    chgl->grow->setVisible(false);
}
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);    
    mole.packer(mole.sdmcompleter());    
    mole.showbonds =
            mole.connecting(mole.showatoms);    
    //if (mol!=chgl->mol) chgl->setMolecule(mole);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();    
    if (growYes) chgl->mol->grow();
    chgl->pause=false;    
    chgl->updateBondActions();
    chgl->updateGL();
}

void Window::deleteAllH(){
    if (sfac.isEmpty()) return;
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    wholeLine->setChecked(false);
    replace->setText("");
    search->setText(QString("^[HhDd]\\S{0,3}\\s+%1\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s*").arg(sfac.indexOf(0)+1));
    replaceAll();
    search->setText("");
    cursor.joinPreviousEditBlock();//remove empty afix blocks
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    for (int i=0; i<document->blockCount()-1;i++){
      if ((cursor.block().text().contains(QRegExp("^afix ",Qt::CaseInsensitive)))
		      &&(cursor.block().next().text().contains(QRegExp("^afix ",Qt::CaseInsensitive)))){
	cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	cursor.deleteChar();
	cursor.deleteChar();
      }
      if ((cursor.block().text().contains(QRegExp("^afix ",Qt::CaseInsensitive)))
                      &&(cursor.block().next().text().contains(QRegExp("^hklf ",Qt::CaseInsensitive)))){
        cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
        cursor.deleteChar();
        cursor.deleteChar();
      }
      cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    }

    editor->updateAfix();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find("hklf",cursor);
    if (!cursor.isNull()){
        cursor.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        int dais=cursor.blockNumber();
        while (editor->parenthesis.at(dais)){
            if (cursor.block().text().contains(QRegExp("^afix ",Qt::CaseInsensitive))){
                cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
                cursor.deleteChar();
            }
            cursor.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
            dais=cursor.blockNumber();
        }
    }else cursor = editor->textCursor();
    for (int i = mole.asymm.size()-1; i>=0; i-- ){
      if (mole.asymm.at(i).an==0){
	QString s =
		QString("<font color=magenta>Deleted atom: %1 part:%2 residue:%3 %4 </font> Please save to update structure view.")
                .arg(mole.asymm.at(i).Label)
                .arg(mole.asymm.at(i).part)
                .arg(mole.asymm.at(i).resiNr)
                .arg(mole.asymm.at(i).ResiClass);
	infoKanalNews(s);
        deletedAtom.append(mole.asymm.at(i));
        mole.asymm.removeAt(i);
      }
    }
    cursor.endEditBlock();
    mole.showatoms.clear();
    mole.showbonds.clear();
    mole.selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);
    mole.packer(mole.sdmcompleter());
    mole.showbonds =
	    mole.connecting(mole.showatoms);
     chgl->setMolecule(&mole);
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    chgl->murx=1;
    if (growYes) chgl->mol->grow();

    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    }
    chgl->pause=false;
    chgl->updateBondActions();
    chgl->updateGL();
}

void Window::deleteSelectedAtoms(){
  if (chgl->mol->selectedatoms.isEmpty()){
      QAction *action = qobject_cast<QAction *>(sender());
      int index=-1;
      if (action)
          index=action->data().toInt();
        else index=-1;
      if (index>mole.asymm.size()) index=-1;
      if (index!=-1) {
            chgl->mol->selectedatoms.clear();
            chgl->mol->selectedatoms.append(chgl->mol->showatoms[index]);
            chgl->mol->selectedatoms.last().style=index;
      }
  }
  if (chgl->mol->selectedatoms.isEmpty()) return;
    chgl->pause=true;
    bool donotsymm=false;
    QTextCursor cursor = editor->textCursor();
    QTextDocument *document = editor->document();
    cursor.beginEditBlock();
    int ende=chgl->mol->selectedatoms.size();
    for (int i = 0; i < ende; i++ ){
      if (chgl->mol->selectedatoms.at(i).symmGroup) continue;
      if (chgl->mol->selectedatoms.at(i).Label.contains(QString::fromUtf8("»")))continue;
      if ((chgl->mol->selectedatoms.at(i).afix>0)&&(chgl->mol->selectedatoms.at(i).an>0)) continue;
      int wg=chgl->mol->selectedatoms.at(i).style;
      QString  label = mole.asymm.at(wg).orginalLine.left(80).trimmed();
      cursor.movePosition(QTextCursor::Start);
      cursor = document->find(label,cursor);
      if (cursor.isNull()){
	cursor = editor->textCursor();
	cursor.movePosition(QTextCursor::Start);
	label.chop(10);
	cursor = document->find(label,cursor);
	if (cursor.isNull()) {
	  cursor = editor->textCursor();
//          qDebug()<<"Could not find that atom in file, maybe the 3D structure is out of sync with the res file. Press Save, uncheck grow Q-peaks and try again. ";
	  printf("hab was nicht %d %s\n",i,label.toStdString().c_str());
	  continue;
	}
      }

      if (editor->parenthesis.at(cursor.blockNumber())){
	//chgl->mol->selectedatoms.removeAt(i);
	do {
	  if (!cursor.block().text().contains("afix",Qt::CaseInsensitive)) wg--;
	  cursor.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor);
	}while(editor->parenthesis.at(cursor.blockNumber()));
	cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      }

      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      while (cursor.selectedText().endsWith("=")) {
	cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      }
      int haa=1;
      while (editor->parenthesis.at(cursor.blockNumber()+1)){
	cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	if (!cursor.block().text().contains("afix",Qt::CaseInsensitive)){
	  chgl->mol->selectedatoms.append(chgl->mol->showatoms.at(wg+haa));
	  chgl->mol->selectedatoms.last().style=wg+haa;
	  haa++;
	}

      }
      cursor.deleteChar();
      cursor.deleteChar();

      editor->updateAfix();
    }

    for (int i = chgl->mol->selectedatoms.size()-1; i>=0; i-- ){
      if (chgl->mol->selectedatoms.at(i).Label.contains(QString::fromUtf8("»"))){donotsymm=true;continue;}
      if (chgl->mol->selectedatoms.at(i).symmGroup){donotsymm=true;continue;}
      QString s =
	      QString("<font color=magenta>Deleted atom: %1 part:%2 residue:%3 %4 </font> Please save to update structure view.")
	      .arg(chgl->mol->selectedatoms.at(i).Label)
	      .arg(chgl->mol->selectedatoms.at(i).part)
	      .arg(chgl->mol->selectedatoms.at(i).resiNr)
	      .arg(chgl->mol->selectedatoms.at(i).ResiClass);
      infoKanalNews(s);
      int wg=mole.asymm.indexOf(chgl->mol->selectedatoms.at(i));
      if (wg<0) continue;
      deletedAtom.append(mole.asymm.at(wg));
      mole.asymm.removeAt(wg);
    }

    cursor.endEditBlock();

    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);
    mole.packer(mole.sdmcompleter());
    mole.showbonds =
            mole.connecting(mole.showatoms);

     chgl->setMolecule(&mole);

    chgl->mol->selectedatoms.clear();
    chgl->murx=1;

    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();

    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    }
    if (donotsymm) QMessageBox::information(0,"information","Only unique atoms can be deleted!");
    chgl->pause=false;
    chgl->updateBondActions();
    chgl->updateGL();
    /*
  }
  else {chgl->disSelection();}*/
}

void Window::createCentroid(){
  if (chgl->mol->selectedatoms.isEmpty()) return;
    chgl->pause=true;
    QTextCursor cursor = editor->textCursor();
    QTextDocument *document = editor->document();
    cursor.beginEditBlock();
    int ctrcnt=1;
    for (int i=0; i<mole.asymm.size();i++){
      if ((mole.asymm.at(i).an==-66)&&(mole.asymm.at(i).Label.startsWith("Cnt",Qt::CaseInsensitive)))
	ctrcnt++;
    }
    int ende=chgl->mol->selectedatoms.size();
    V3 posc = V3(0,0,0), posf = V3(0,0,0);
    QString creatorS;
    for (int i = 0; i < ende; i++ ){
      posc+=chgl->mol->selectedatoms.at(i).pos;
      creatorS.append(chgl->mol->selectedatoms.at(i).Label+", ");
      if (creatorS.section('\n',-1).size()>50) creatorS.append("\nREM  ");
    }
    posc*=1.0/ende;
    mole.kart2frac(posc,posf);
    MyAtom ctr ;
    ctr.an=-66;
    ctr.part=0;
    ctr.peakHeight=9999.99;
    ctr.resiNr=0;
    ctr.hidden=0;
    ctr.symmGroup=0;
    ctr.uc.m12=ctr.uc.m23=ctr.uc.m13=0.0;
    ctr.uc.m21=ctr.uc.m32=ctr.uc.m31=0.0;
    ctr.uf.m12=ctr.uf.m23=ctr.uf.m13=0.0;
    ctr.uf.m21=ctr.uf.m32=ctr.uf.m31=0.0;
    ctr.pos=posc;
    ctr.frac=posf;
    if (!sfac.contains(-66)){
      sfac.append(-66);
      unitNeu.append(" 0");
      //    if (unitNeu.split('\n').last().size()>76) unitNeu.append("=\n    ");
      cursor.movePosition(QTextCursor::Start);
      cursor = document->find(unitAlt,cursor);
      if (cursor.isNull()) {
	cursor = editor->textCursor();
	cursor.endEditBlock ();
	printf("can't find unit\n");
	update();
	return;
      }
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      cursor.deleteChar();
      cursor.insertText(unitNeu);

      cursor.movePosition(QTextCursor::Start);
      cursor = document->find(unitNeu,cursor);
      cursor.clearSelection ();
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.insertText("SFAC cnt 0 0 0 0 0 0 0 0 0 0 0 0 0 0.1 ! centroid dummy\n");
    }
    cursor.movePosition(QTextCursor::Start);
    cursor = document->find("HKLF",cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.insertText(ctr.orginalLine=QString("Cnt%5 %1 %2 %3 %4 10.000 0.001 ! centroid\n")
		    .arg(sfac.indexOf(-66)+1)
		    .arg(posf.x,10,'f',6)
		    .arg(posf.y,10,'f',6)
		    .arg(posf.z,10,'f',6)
		    .arg(ctrcnt,0,36));
    cursor.insertText(QString("REM Cnt%1 was created from: %2\n").arg(ctrcnt,0,36).arg(creatorS)); 
    
    for (int i = 0; i < ende; i++ ){
      cursor.insertText(QString("FREE Cnt%1 %2 ! this might be needed for AFIX\n").arg(ctrcnt,0,36).arg(chgl->mol->selectedatoms.at(i).Label.section('_',0,0).section(QString::fromUtf8("»"),0,0)));
    }
    ctr.orginalLine= QString("Cnt%5 %1 %2 %3 %4 10.000 0.001\n")
		    .arg(sfac.indexOf(-66)+1)
		    .arg(posf.x,10,'f',6)
		    .arg(posf.y,10,'f',6)
		    .arg(posf.z,10,'f',6)
		    .arg(ctrcnt,0,36);
    ctr.Label=QString("Cnt%1").arg(ctrcnt,0,36);
    cursor.endEditBlock ();
    editor->setTextCursor(cursor);
    editor->centerCursor () ;
    cursor.clearSelection ();
    editor->setTextCursor(cursor);
    editor->setFocus () ;
    mole.asymm.append(ctr);
    chgl->pause=true;
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++){
      mole.showatoms.append(mole.asymm[o]);
    }
    mole.sdmcompleter();
    mole.showbonds =
	    mole.connecting(mole.showatoms);
     chgl->setMolecule(&mole);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();
    chgl->pause=false;
    chgl->updateGL();
    update();
}

void Window::moveSymmMateSel(){

  balken->setMinimum(0);
  balken->setMaximum(mole.selectedatoms.size());
  balken->show();
  QString kep,nextli;
  QTextCursor cursor = editor->textCursor();
  QTextDocument *document = editor->document();
  cursor.beginEditBlock();
  for (int si=0; si<mole.selectedatoms.size();si++){
    balken->setValue(si);
    QString source,target;
    source=mole.selectedatoms.at(si).Label;
    target=source.section(QString::fromUtf8("»"),0,0);
    MyAtom ato=mole.selectedatoms.at(si);
    ato.Label=target;
    ato.symmGroup=0;
    int that=mole.asymm.indexOf(ato);
    if ((that>=mole.asymm.size())||(that<0)) return;
    QString susu=mole.asymm.at(that).orginalLine;
    cursor.movePosition(QTextCursor::Start);
    cursor = document->find(susu,cursor);
    if (mole.asymm.at(that).uf.m22==-666.0){
      kep=QString("%1 %2 %3 %4 %5 %6 %7")
		      .arg(ato.Label.section('_',0,0),-5)
		      .arg(qMax(1,sfac.indexOf(ato.an)+1))
		      .arg(ato.frac.x,10,'f',6)
		      .arg(ato.frac.y,10,'f',6)
		      .arg(ato.frac.z,10,'f',6)
		      .arg(mole.asymm.at(that).sof_org,9,'f',5)
		      .arg(ato.uf.m33=mole.asymm.at(that).uf.m33,9,'f',5)
		      ;
      cursor.deleteChar();
      cursor.insertText(kep);
      cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
      ato.sof_org=mole.asymm.at(that).sof_org;
//      printf("debug:iso sof==%g atom#%d mole.asymm.at(that).afixParent %d \n",mole.asymm.at(that).sof_org,that,mole.asymm.at(that).afixParent);
      ato.uf.m11=ato.uc.m11=ato.uc.m22=ato.uc.m33= mole.asymm.at(that).uf.m11;
      ato.uf.m22=-666.0;
      ato.uc.m12=ato.uc.m13=ato.uc.m23=ato.uc.m21=ato.uc.m31=ato.uc.m32=0;
      wholeLine->setChecked(false);
      replaceNext();
    }else{
      kep=QString("%1 %2 %3 %4 %5 %6 %7 %8 ")
	      .arg(ato.Label.section('_',0,0),-5)
	      .arg(qMax(1,sfac.indexOf(ato.an)+1))
	      .arg(ato.frac.x,10,'f',6)
	      .arg(ato.frac.y,10,'f',6)
	      .arg(ato.frac.z,10,'f',6)
	      .arg(mole.asymm.at(that).sof_org,9,'f',5)
	      .arg(ato.uf.m11,9,'f',5)
	      .arg(ato.uf.m22,9,'f',5);
  //    printf("debug:anis sof==%g atom#%d\n",mole.asymm.at(that).sof_org,that);
      ato.sof_org=mole.asymm.at(that).sof_org;
      cursor.deleteChar();
      cursor.insertText(kep);
      cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::Down);
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      nextli=cursor.selectedText();
      cursor = document->find(nextli,cursor);

      cursor.deleteChar();
      cursor.insertText(QString("     %1 %2 %3 %4")
		      .arg(ato.uf.m33,9,'f',5)
		      .arg(ato.uf.m23,9,'f',5)
		      .arg(ato.uf.m13,9,'f',5)
		      .arg(ato.uf.m12,9,'f',5));
      cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
    }
    ato.orginalLine=kep.section("=",0,0);
    mole.asymm[that]=ato;
  }

  cursor = editor->textCursor();
  cursor.endEditBlock();
  chgl->pause=true;
  mole.showatoms.clear();
  mole.showbonds.clear();
  chgl->mol->showatoms.clear();
  chgl->mol->showbonds.clear();
  chgl->mol->selectedatoms.clear();
  for (int o=0; o<mole.asymm.size();o++)
    mole.showatoms.append(mole.asymm[o]);
  mole.sdmcompleter();
  mole.showbonds =
	  mole.connecting(mole.showatoms);
   chgl->setMolecule(&mole);
  chgl->mol->selectedatoms.clear();
  chgl->murx=1;
  bool growYes=chgl->fuse->isVisible ();
  chgl->mol->fuse();
  if (growYes) chgl->mol->grow();
  search->setText("");
  hideSearch();
  chgl->pause=false;
  chgl->updateGL();
  update();
  balken->hide();
}

void Window::moveSymmMate(){
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  QString source,target;
  source=chgl->mol->showatoms.at(index).Label;
  target=source.section(QString::fromUtf8("»"),0,0);
  MyAtom ato=chgl->mol->showatoms.at(index);
  ato.Label=target;
  ato.symmGroup=0;
  int that=mole.asymm.indexOf(ato);
  if ((that>=mole.asymm.size())||(that<0)) return;
  QString susu=mole.asymm.at(that).orginalLine;
  search->setText(susu);
  if (mole.asymm.at(that).uf.m22==-666.0){
  replace->setText(QString("%1 %2 %3 %4 %5 %6 %7")
		  .arg(ato.Label.section('_',0,0),-5)
		  .arg(qMax(1, sfac.indexOf(ato.an)+1))
		  .arg(ato.frac.x,10,'f',6)
		  .arg(ato.frac.y,10,'f',6)
		  .arg(ato.frac.z,10,'f',6)
                  .arg(mole.asymm.at(that).sof_org,9,'f',5)
                  .arg(ato.uf.m33=mole.asymm.at(that).uf.m33,9,'f',5)
		  );
  ato.uf.m11=ato.uc.m11=ato.uc.m22=ato.uc.m33= mole.asymm.at(that).uf.m11;
  ato.uf.m22=-666.0;
  ato.uc.m12=ato.uc.m13=ato.uc.m23=ato.uc.m21=ato.uc.m31=ato.uc.m32=0;
  wholeLine->setChecked(false);
  replaceNext();
  }else{
    QString kep=QString("%1 %2 %3 %4 %5 %6 %7 %8 ")
		  .arg(ato.Label.section('_',0,0),-5)
		  .arg(qMax(1,sfac.indexOf(ato.an)+1))
		  .arg(ato.frac.x,10,'f',6)
		  .arg(ato.frac.y,10,'f',6)
		  .arg(ato.frac.z,10,'f',6)
                  .arg(mole.asymm.at(that).sof_org,9,'f',5)
		  .arg(ato.uf.m11,9,'f',5)
		  .arg(ato.uf.m22,9,'f',5);

    replace->setText(kep);
    wholeLine->setChecked(false);
    replaceNext();
    search->setText(replace->text());
    replace->setText("");
    findNext();  
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Down);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    search->setText(cursor.selectedText());
    replace->setText(QString("     %1 %2 %3 %4")
		    .arg(ato.uf.m33,9,'f',5)
		    .arg(ato.uf.m23,9,'f',5)
		    .arg(ato.uf.m13,9,'f',5)
		    .arg(ato.uf.m12,9,'f',5)
		    );
  
    replaceNext();
    replace->setText(kep);
  }
  search->setText(replace->text());
  replace->setText("");
  findNext();  
  QTextCursor cursor = editor->textCursor();
  ato.orginalLine=cursor.selectedText().section("=",0,0);
  mole.asymm[that]=ato;
    
  chgl->pause=true;
  mole.showatoms.clear();
  mole.showbonds.clear();
  chgl->mol->showatoms.clear();
  chgl->mol->showbonds.clear();
  chgl->mol->selectedatoms.clear();
  for (int o=0; o<mole.asymm.size();o++)
    mole.showatoms.append(mole.asymm[o]);
  mole.sdmcompleter();
  mole.showbonds =
	  mole.connecting(mole.showatoms);
   chgl->setMolecule(&mole);
  chgl->mol->selectedatoms.clear();
  chgl->murx=1;
  bool growYes=chgl->fuse->isVisible ();
  chgl->mol->fuse();
  if (growYes) chgl->mol->grow();
  search->setText("");
  hideSearch();
  chgl->pause=false;
  chgl->updateGL();
  update();
}

void Window::copySymmMate(){
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  QString source,target;
  source=chgl->mol->showatoms.at(index).Label;
  target=source.section(QString::fromUtf8("»"),0,0);
  MyAtom ato=chgl->mol->showatoms.at(index);
  ato.Label=target;
  ato.symmGroup=0;
  int that=mole.asymm.indexOf(ato);
  if ((that>=mole.asymm.size())||(that<0)) return;
  QString susu=mole.asymm.at(that).orginalLine;
  QTextCursor cursor = editor->textCursor();
  cursor.movePosition(QTextCursor::Start);
  QTextDocument *document = editor->document();
  if (document->find(susu, cursor).isNull()) {
      QMessageBox::information(0,"information","The Flie is out of sync with the model. Please save and try again!");
      return;
  }
  cursor.beginEditBlock();
  //AFIX 0 am ende einfuegen
    cursor.movePosition(QTextCursor::Start);
    cursor = document->find("HKLF",cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
    if (editor->parenthesis.at(cursor.blockNumber())){
      cursor.select(QTextCursor::WordUnderCursor);
      if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
        cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
      cursor.select(QTextCursor::WordUnderCursor);
      if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
        cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      cursor.clearSelection();
      cursor.insertText("AFIX 0 \n");
    }
    /////////////////////////////
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(susu, cursor); 
  cursor.movePosition(QTextCursor::Down);
  cursor.movePosition(QTextCursor::StartOfLine);

  if (mole.asymm.at(that).uf.m22==-666.0){
    cursor.insertText(QString("%1 %2 %3 %4 %5 %6 %7 !copy\n")
		    .arg(ato.Label.section('_',0,0),-5)//1
		    .arg(qMax(1,sfac.indexOf(ato.an)+1))//2
		    .arg(ato.frac.x,10,'f',6)//3
		    .arg(ato.frac.y,10,'f',6)//4
		    .arg(ato.frac.z,10,'f',6)//5
		    .arg(mole.asymm.at(that).sof_org,9,'f',5)//6
		    .arg(ato.uf.m33=mole.asymm.at(that).uf.m33,9,'f',5)//7
		    );

  }else{
    cursor.movePosition(QTextCursor::Down);
    cursor.movePosition(QTextCursor::StartOfLine);
    while(editor->parenthesis.at(cursor.blockNumber()+1)){
    cursor.movePosition(QTextCursor::Down);
    cursor.movePosition(QTextCursor::StartOfLine);
    }
    cursor.insertText(QString("%1 %2 %3 %4 %5 %6 %7 %8 =\n %9 %10 %11 %12 !copy\n")
		    .arg(ato.Label.section('_',0,0),-5)//1
		    .arg(qMax(1,sfac.indexOf(ato.an)+1))//2
		    .arg(ato.frac.x,10,'f',6)//3
		    .arg(ato.frac.y,10,'f',6)//4
		    .arg(ato.frac.z,10,'f',6)//5
		    .arg(mole.asymm.at(that).sof_org,9,'f',5)//6
		    .arg(ato.uf.m11,9,'f',5)//7
		    .arg(ato.uf.m22,9,'f',5)//8
		    .arg(ato.uf.m33,9,'f',5)//9
		    .arg(ato.uf.m23,9,'f',5)//10
		    .arg(ato.uf.m13,9,'f',5)//11
		    .arg(ato.uf.m12,9,'f',5)//12
		    );

  }
  mole.asymm.append(ato);  
  cursor.endEditBlock();
  chgl->pause=true;
  mole.showatoms.clear();
  mole.showbonds.clear();
  chgl->mol->showatoms.clear();
  chgl->mol->showbonds.clear();
  chgl->mol->selectedatoms.clear();
  for (int o=0; o<mole.asymm.size();o++)
    mole.showatoms.append(mole.asymm[o]);
  mole.sdmcompleter();
  mole.showbonds =
	  mole.connecting(mole.showatoms);
   chgl->setMolecule(&mole);
  chgl->mol->selectedatoms.clear();
  chgl->murx=1;
  bool growYes=chgl->fuse->isVisible ();
  chgl->mol->fuse();
  if (growYes) chgl->mol->grow();
  chgl->pause=false;
  chgl->updateGL();
  update();
}

void Window::updateTieFreeVar(){
  int part = partSpin->value();
  if (!part) {
    tieOccL->hide();
    tieType->hide();
    tieType->setCurrentIndex(0);
    tieFVNrL->hide();
    tieFVNr->hide();
    tieUsedL->hide();
    update();
    return;
  }
  tieOccL->show();
  tieType->show();

  if (tieType->itemData(tieType->currentIndex()).toInt()){
    tieFVNrL->show();
    tieFVNr->show();
    tieUsedL->setText(QString("fv(%1)=%3 [%2x used]")
		    .arg(tieFVNr->value())
		    .arg(fvarCntr.value(tieFVNr->value()))
		    .arg((fvar.size()>(tieFVNr->value()-1))?fvar.at(tieFVNr->value()-1):0.666) );
    tieFVNr->setMaximum(fvar.size()+1);
    tieUsedL->show();
  }else{
    tieFVNrL->hide();
    tieFVNr->hide();
    tieUsedL->hide();
  }
  update();
}

void Window::clipboardDataChanged(){
#ifndef QT_NO_CLIPBOARD
  actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
}

void Window::shxActUpdate(bool vis){
  bool v = shxOutputDock->isVisible();
  editToolBar->setVisible(vis|v);
  actionSearch->setVisible(vis|v);
  actionNext->setVisible(vis|v);
  actionPrev->setVisible(vis|v);
  actionUndo->setVisible(vis);
  actionRedo->setVisible(vis);
  actionCut->setVisible(vis);
  actionCopy->setVisible(vis);
  actionPaste->setVisible(vis);
  actionEP->setVisible(vis|v);
  actionEM->setVisible(vis|v);
}

void Window::shxOPTUpdate(bool vis){

  bool v = shxEditorDock->isVisible();

  hideSearch2();
  editToolBar->setVisible(vis|v);
  actionSearch->setVisible(vis|v);
  actionNext->setVisible(vis|v);
  actionPrev->setVisible(vis|v);
  actionEP->setVisible(vis|v);
  actionEM->setVisible(vis|v);
  update();
}

void Window::renamUpdate(bool vis){
  enterRenameMode->setChecked(vis);
  enterRenameMode->setText((vis)?"Exit Rename Mode":"Rename Mode");
  updateTieFreeVar();

  psew->hide();
  willpse->show();
  if (vis) {
    renamDock->show();
    renamDock->raise();
    editToolBar->setVisible(vis);
    actionSearch->setVisible(vis);
    actionNext->setVisible(vis);
    actionPrev->setVisible(vis);
    actionUndo->setVisible(vis);
    actionRedo->setVisible(vis);
    actionCut->setVisible(vis);
    actionCopy->setVisible(vis);
    actionPaste->setVisible(vis);
    actionEP->setVisible(vis);
    actionEM->setVisible(vis);

    if (dock->isVisible()) dock->hide();
    if (histDock->isVisible()) histDock->hide();
    machPlatz=true;
    renamDock->show();
    renamDock->raise();
  }
  else if (enterRenameMode->text()=="Rename Mode"){
      if ((machPlatz)&&(!dock->isVisible())&&(!histDock->isVisible())) {
          dock->show();
          histDock->show();
          shxEditorDock->show();
          shxEditorDock->raise();
          machPlatz=false;

      }
  }
  if ((vis)&&(height()>hhh)) {
#if defined(linux)
      showMaximized ();
      showNormal ();
#endif
      showMaximized ();
  }
}

void Window::addNewScatteringFactor(int oz){
  changeElemetofLabel(mole.pse(oz));
  if (sfac.contains(oz)) {
    pserbt[oz+1]->setChecked(true);
    return;
  }
  sfac.append(oz);
  pserbt[sfac.last()+1]->show();
  pserbt[sfac.last()+1]->setChecked(true);
  sfacBox->setFixedSize((sfac.size()+1)*52,70);

  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock();
  cursor.movePosition(QTextCursor::End);
  cursor = document->find("sfac", cursor, QTextDocument::FindBackward);
  if (!cursor.isNull()){
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    QString sf = cursor.selectedText().trimmed();
    if (sf.size()<77) sf.append(QString(" %1").arg(mole.pse(oz)));
    cursor.deleteChar();
    cursor.insertText(sf);
  }
  cursor.movePosition(QTextCursor::End);
  cursor = document->find("unit", cursor, QTextDocument::FindBackward);
  if (!cursor.isNull()){
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    QString sf = cursor.selectedText().trimmed();
    if (sf.size()<77) sf.append(" 1");
    cursor.deleteChar();
    cursor.insertText(sf);
  }
  cursor.endEditBlock();
}

void Window::openFile(){
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open shelX file "), dirName,
		  "SHELX-Files (*.res *.ins);;");
  if (!fileName.isEmpty()) {

      loadFile(fileName);
  }
  update();
}

void Window::setShelxlOptions(){
  QDialog *shxOptDLG =  new QDialog(this);
  shxOptDLG->setWindowTitle("SHELXL runtime options");
  QGridLayout *lt = new QGridLayout(shxOptDLG);
  QLabel *PathL = new QLabel("Full path to executable",shxOptDLG);
  shxPath = new QLineEdit(shxOptDLG);
  QLabel *argumentsL =  new QLabel("Commandline options",shxOptDLG);
  shxOptions = new QLineEdit(shxOptDLG);
  shxPath->setText(shelxlPath); 
  shxOptions->setText(shelxlArgs);
  QPushButton *browsePath = new QPushButton("Browse",shxOptDLG);
  connect(browsePath,SIGNAL(pressed ()),this, SLOT(setShelxlPath()));
  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal,shxOptDLG);
  connect(buttonBox, SIGNAL(accepted()), shxOptDLG, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), shxOptDLG, SLOT(reject()));
  lt->addWidget(PathL,1,0);
  lt->addWidget(shxPath,1,1);
  lt->addWidget(argumentsL,2,0);
  lt->addWidget(shxOptions,2,1);
  lt->addWidget(browsePath,1,2);
  lt->addWidget(buttonBox,8,1,1,3);
  shxOptDLG->setLayout(lt);
  if (shxOptDLG->exec()==QDialog::Accepted) {
    shelxlArgs=shxOptions->text();
    shelxlPath=shxPath->text();
    mole.einstellung->beginGroup("Window");
    mole.einstellung->setValue("SHELXL-PATH",shelxlPath);
    mole.einstellung->setValue("SHELXL-OPTION",shelxlArgs);
    mole.einstellung->endGroup();
  }
  
  shxOptDLG->deleteLater ();
  delete shxPath;
  delete shxOptions;
	
  shxPath=shxOptions=NULL;
}

void Window::extraSettings(){

   /* printf("extraProgramNames.size %d %d %d %d %p\n",
               extraProgramNames.size(),
               extraProgramPaths.size(),
               extraArguments.size(),
               extraExtensions.size(),extraTB);*/
  QDialog *extDLG = new QDialog(this);
  extDLG->setWindowTitle("External programs"); 
  QGridLayout *lt = new QGridLayout(extDLG);
  QComboBox *pgms = new QComboBox(extDLG);
  pgms->addItems(extraProgramNames);
  connect(pgms,SIGNAL(currentIndexChanged(int)), this, SLOT(updateExtraSettings(int)));
  QLabel *extraNameL = new QLabel("Name of external program for menu entry:",extDLG);
  extraName = new QLineEdit(extDLG);
  QLabel *extraPathL = new QLabel("Full path to executable",extDLG);
  extraPath = new QLineEdit(extDLG);
  QLabel *argumentsL =  new QLabel("Commandline options",extDLG);
  extraArgumentsEdit = new QLineEdit(extDLG);
  QLabel *altExtL =  new QLabel("Alternative file Extension",extDLG);
  extraAltExtEdit = new QLineEdit(extDLG);
  QPushButton *browsePath = new QPushButton("Browse",extDLG);
  connect(browsePath,SIGNAL(pressed ()),this, SLOT(setExtraPath()));
  useArgument = new QCheckBox("Give file name as argument",extDLG);
  omitExtension = new QCheckBox("Omit '*.res' extension in argument",extDLG);
  detach = new QCheckBox("start detached",extDLG);
  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::No| QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal,extDLG);
  QPushButton *ok = buttonBox->button(QDialogButtonBox::Ok);
  ok->setText("Add program");
  QPushButton *no =buttonBox->button(QDialogButtonBox::No);
  no->setText("Remove program");
  connect(buttonBox, SIGNAL(clicked(QAbstractButton*)),this,SLOT(removeExtra(QAbstractButton*)));
  connect(buttonBox, SIGNAL(accepted()), extDLG, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), extDLG, SLOT(reject()));
  lt->addWidget(pgms,0,0);
  lt->addWidget(extraNameL,1,0);
  lt->addWidget(extraName,1,1);
  lt->addWidget(extraPathL,2,0);
  lt->addWidget(extraPath,2,1);
  lt->addWidget(browsePath,2,2);
  lt->addWidget(argumentsL,4,0);
  lt->addWidget(extraArgumentsEdit,4,1);
  lt->addWidget(altExtL,5,0);
  lt->addWidget(extraAltExtEdit,5,1);
  lt->addWidget(useArgument,13,0);
  lt->addWidget(omitExtension,13,1);
  lt->addWidget(detach,13,2);
  lt->addWidget(buttonBox,14,0,1,4);
  extDLG->setLayout(lt);
  updateExtraSettings(0);

  while (extraOptions.size()>extraProgramNames.size()){extraOptions.removeLast();}
  while (extraExt2.size()>extraProgramNames.size()){extraExt2.removeLast();}

  if (extDLG->exec()==QDialog::Accepted) {
    int wegmuss=extraProgramNames.indexOf(extraName->text());
    if (wegmuss!=-1) {
      extraProgramNames.removeAt(wegmuss);
      extraProgramPaths.removeAt(wegmuss);
      extraArguments.removeAt(wegmuss);
      extraExtensions.removeAt(wegmuss);
      extraDetach.removeAt(wegmuss);
      extraOptions.removeAt(wegmuss);
      extraExt2.removeAt(wegmuss);
    }
    extraProgramNames.append(extraName->text().trimmed());
    extraProgramPaths.append(extraPath->text().trimmed());
    extraArguments.append(useArgument->checkState());
    extraOptions.append(extraArgumentsEdit->text().trimmed());
    extraExt2.append(extraAltExtEdit->text().trimmed());
    extraExtensions.append(omitExtension->checkState());
    extraDetach.append(detach->checkState());
    mole.einstellung->beginGroup("Extra");
 /*   printf("extraProgramNames.size %d %d %d %d %p\n",
           extraProgramNames.size(),
           extraProgramPaths.size(),
           extraArguments.size(),
           extraExtensions.size(),extraTB);*/
    mole.einstellung->beginWriteArray("ProgramNames",extraProgramNames.size());
    for (int i = 0; i < extraProgramNames.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Name", extraProgramNames.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramPaths",extraProgramPaths.size());
    for (int i = 0; i < extraProgramPaths.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Path", extraProgramPaths.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("AlternativeExtensions",extraExt2.size());
    for (int i = 0; i < extraExt2.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("ext", extraExt2.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("CommandLineOptions",extraOptions.size());
    for (int i = 0; i < extraOptions.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("opt", extraOptions.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();



    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramArgs",extraArguments.size());
    for (int i = 0; i < extraArguments.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Arg", extraArguments.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramExtensions",extraExtensions.size());
    for (int i = 0; i < extraExtensions.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Ext", extraExtensions.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramDetached",extraDetach.size());
    for (int i = 0; i < extraDetach.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Detach", extraDetach.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();
    extraMenu->clear();
      extraTB->clear();
    extraMenu->addAction("add/remove external program",this,SLOT(extraSettings()));
    QFileIconProvider *p = new QFileIconProvider();
    if (extraProgramNames.size()) extraMenu->addSeparator();
    for (int i=0; i<extraProgramNames.size(); i++){
      QAction *a;
      QString icopa=extraProgramPaths.at(i);
      extraIcon=p->icon(QFileInfo(icopa));
      a=extraMenu->addAction(extraIcon,QString("run %1 !").arg(extraProgramNames.at(i)),this,SLOT(runExtra()));
      a->setData(i);  
      extraTB->addAction(a);
    }
    delete p;

  }
  extDLG->deleteLater ();
}

void Window::removeExtra(QAbstractButton *button){
  if (button->text()=="Remove program") {
    int wegmuss=extraProgramNames.indexOf(extraName->text());
    if (wegmuss!=-1) {
      extraProgramNames.removeAt(wegmuss);
      extraProgramPaths.removeAt(wegmuss);
      extraArguments.removeAt(wegmuss);
      extraExtensions.removeAt(wegmuss);
      extraOptions.removeAt(wegmuss);
      extraExt2.removeAt(wegmuss);
      extraDetach.removeAt(wegmuss);
    }
    while (extraOptions.size()>extraProgramNames.size()){extraOptions.removeLast();}
    while (extraExt2.size()>extraProgramNames.size()){extraExt2.removeLast();}
    mole.einstellung->beginGroup("Extra");
//    printf("extraProgramNames.size %d %d %d %d\n",extraProgramNames.size(),extraProgramPaths.size(),extraArguments.size(),extraExtensions.size());
    mole.einstellung->beginWriteArray("ProgramNames",extraProgramNames.size());
    for (int i = 0; i < extraProgramNames.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Name", extraProgramNames.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramPaths",extraProgramPaths.size());
    for (int i = 0; i < extraProgramPaths.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Path", extraProgramPaths.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();


    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramArgs",extraArguments.size());
    for (int i = 0; i < extraArguments.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Arg", extraArguments.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramExtensions",extraExtensions.size());
    for (int i = 0; i < extraExtensions.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Ext", extraExtensions.at(i));
    }
    mole.einstellung->endArray();    
    mole.einstellung->endGroup();

    mole.einstellung->beginGroup("Extra");
    mole.einstellung->beginWriteArray("ProgramDetached",extraDetach.size());
    for (int i = 0; i < extraDetach.size(); i++) {
        mole.einstellung->setArrayIndex(i);
        mole.einstellung->setValue("Detach", extraDetach.at(i));
    }
    mole.einstellung->endArray();
    mole.einstellung->endGroup();
    extraMenu->clear();
  
    extraMenu->addAction("add external program",this,SLOT(extraSettings()));
    extraTB->clear();
    QFileIconProvider *epic = new QFileIconProvider();
    if (extraProgramNames.size()) extraMenu->addSeparator();
    for (int i=0; i<extraProgramNames.size(); i++){
      QAction *a;
      QString icopa=extraProgramPaths.at(i);
      extraIcon=epic->icon(QFileInfo(icopa));
      a=extraMenu->addAction(extraIcon,QString("run %1 !").arg(extraProgramNames.at(i)),this,SLOT(runExtra()));
      a->setData(i);
      extraTB->addAction(a);
    }
   delete epic;
  
  }

}

void Window::updateExtraSettings(int index){
  if ((index<0)||(index>=extraProgramNames.size())) return;
  //qDebug()<<extraProgramNames<<extraProgramPaths<<extraOptions<<extraExt2<<extraArguments<<extraExtensions;
  extraName->setText(extraProgramNames.at(index));
  extraPath->setText(extraProgramPaths.at(index));
  extraArgumentsEdit->setText(extraOptions.at(index));
  extraAltExtEdit->setText(extraExt2.at(index));
  useArgument->setCheckState(extraArguments.at(index));
  omitExtension->setCheckState(extraExtensions.at(index));
  detach->setCheckState(extraDetach.at(index));
  update();
}

void Window::setExtraPath(){
  QString s =
	  QFileDialog::getOpenFileName(this, "Path to external program executable", extraPath->text());
  if (!s.isEmpty()){
    extraPath->setText(s);
    /*
    mole.einstellung->beginGroup("Window");
    mole.einstellung->setValue("SHELXL-PATH",shelxlPath);
    mole.einstellung->endGroup();
    */
  }
}

void Window::runExtra(){
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if ((index<0)||(index>=extraProgramNames.size())) return;
  
  extraOutput->hide();
  delete extraOutput;
  extraOutput = new QDialog(this);
  lt = new QVBoxLayout();
  extraOutPut = new QTextEdit();
  QFont mospfo("Monospace");
  extraOutPut->setFont(mospfo);
  extraOutput->setMinimumWidth(
              QFontMetrics(extraOutPut->font())
              .width("#########################################################################################################################################"));
  extraOutPut->setLineWrapMode(QTextEdit::NoWrap);
  lt->addWidget(extraOutPut);
  extraOutput->setLayout(lt);
  extraOutput->setWindowTitle(QString("Running %1 ...").arg(extraProgramNames.at(index)));
  extraOutPut->clear();
  QString path,name;
  path=extraProgramPaths.at(index);
#if defined (Q_WS_MAC)
  if ((QFileInfo(path).isDir())&&(path.endsWith(".app",Qt::CaseInsensitive))){
      path.append("/Contents/MacOS/");
      path.append(QDir(path).entryList(QDir::Executable|QDir::Files).first());
   }
#endif
  QString dn=dirName.section('/',0,-2),
	  argument=dirName.section('/',-1);
  if (extraExtensions.at(index)==Qt::Checked) argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
  if ((extraExt2.size()>index)&&(!extraExt2.at(index).isEmpty())){
      argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
      argument.append(extraExt2.at(index));
  }
  extraProc = new QProcess(this);
  QStringList al;
  if ((extraOptions.size()>index)&&(!extraOptions.at(index).isEmpty())) al.append(extraOptions.at(index));
  if (extraArguments.at(index)==Qt::Checked) al.append(argument);
  extraProc->setWorkingDirectory(dn);                           // updateXLput()
  connect(extraProc,SIGNAL(readyReadStandardOutput ()),this,SLOT(updateExtraOutput()));
  connect(extraProc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(updateExtraOutput()));
  connect(extraProc,SIGNAL(error(QProcess::ProcessError)),this,SLOT(updateExtraOutput())); 
#if (QT_VERSION >= 0x040600)
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  extraProc->setProcessEnvironment(env);//Thanks to Dr. Pavlo Solntsev!! 
#else
  QStringList env = QProcess::systemEnvironment();
  extraProc->setEnvironment(env);
#endif
 QString dummyargument=al.join(" ");
 al=dummyargument.split(" "); 
 if (extraDetach.at(index)) extraProc->startDetached(path,al,dn);
 else {
  extraProc->start(path,al);
  extraOutPut->setText(QString("---Running %1 %2 in %3---\n").arg(path).arg(al.join(" ")).arg(dn));
  extraOutput->show();
  }
}

void Window::updateExtraOutput(){
  if (extraProc->pid()) {
    extraOutPut->setReadOnly(false);
    QString s=extraOutPut->toPlainText();
    s.append(extraProc->readAllStandardOutput());
    s.append(extraProc->readAllStandardError());
    extraOutPut->setPlainText(s);
    extraOutPut->moveCursor(QTextCursor::End); 
    extraOutPut->ensureCursorVisible ();
    extraOutPut->setReadOnly(true);
    extraProc->write("\n"); // this is sent to fake a 'pause' waiting for any key so that the script continues
  }else{
    extraOutPut->setReadOnly(false);
    QString s=extraOutPut->toPlainText();
    s.append(extraProc->readAllStandardOutput());
    s.append(extraProc->readAllStandardError());
    if (s.isEmpty()) s.append("---Program is not running!---\n");
    else{
      s.append("\n---Program is finished.---\n");
      QString dn=dirName.section('/',0,-2), argument=dirName.section('/',-1);
      argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
      QDir work=QDir(QString("%1/").arg(dn));
      QStringList filter;
      filter << QString("%1*.res").arg(argument);
      QStringList resfs;
      resfs=work.entryList(filter,QDir::Files,QDir::Time|QDir::Reversed);
      QHBoxLayout *hbl=new QHBoxLayout();
      QToolButton *tbb0=new QToolButton();
      QAction *ac0=new QAction(QString("cancel"),this);
      connect(ac0,SIGNAL(triggered()),extraOutput,SLOT(reject()));
      tbb0->setDefaultAction(ac0);
      for (int i=0; i<resfs.size(); i++){
      QAction *ac=new QAction(QString("load %1").arg(resfs.at(i)),this);
      ac->setData(QString("%1/%2").arg(dn).arg(resfs.at(i)));
      connect(ac,SIGNAL(triggered()),this,SLOT(loadThisFile())); 
      QToolButton *tbb=new QToolButton();
      tbb->setDefaultAction(ac);
      hbl->addWidget(tbb);
      }
      hbl->addWidget(tbb0);
      lt->addLayout(hbl);
    }
    extraOutPut->setPlainText(s);
    extraOutPut->moveCursor(QTextCursor::End); 
    extraOutPut->ensureCursorVisible ();
    extraOutPut->setReadOnly(true);
  }
}

void Window::loadThisFile(){
  extraOutput->hide();
  QAction *action = qobject_cast<QAction *>(sender());
  QString name="";
  if (action)
    name=action->data().toString();
  else return;
  if (name.isEmpty()) return;

  
  loadFile(name);
}

void Window::configureBottomToolbar(){
  QAction *action = qobject_cast<QAction *>(sender());
  int index=0;
  if (action)
    index=action->data().toInt();
  else return;
  if ((index>=0)&&(index<5)) bottomToolBar->setToolButtonStyle((Qt::ToolButtonStyle) index);
  if (index>32) {
    index-=32;
    addToolBarBreak();
    addToolBar((Qt::ToolBarArea) index,bottomToolBar);
    }
    mole.einstellung->beginGroup("ToolBar");
    mole.einstellung->setValue("SelectionToolBarStyle",bottomToolBar->toolButtonStyle());
    mole.einstellung->endGroup();

}

void Window::grossRC(const QString &text){
  QString s=text.toUpper ();
  resiResiClass->setText(s);
  update();
}

void Window::insertList6(){
    QFile f(dirName);
    f.open(QIODevice::ReadOnly|QIODevice::Text);
    QString alterScheis=f.readAll();
    f.close();
    if (!alterScheis.contains(QRegExp("\nLIST",Qt::CaseInsensitive))){

        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        cursor.insertText("LIST 6 ! automatically inserted. Change 6 to 4 for CHECKCIF!!\n");
        fileSave();
    }
}

bool Window::insertAnis(){
  //  QFile f(dirName);
  //  f.open(QIODevice::ReadOnly|QIODevice::Text);

        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        cursor.insertText("ANIS ! automatically inserted\n");
        return fileSave();
}

void Window::runShelXL(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        if (action->data().toBool()) if (!insertAnis()) return;;
  if (maybeSave2()){
    if (mole.qbeforehkl) deleteAllQ();
//    printf("runXL line %d",__LINE__);
    insertList6();
    xlPutor->clear();

    xlPutor->ListFile=false;
    xlPutor->highlighter->loadHighlightPatterns(lstpatternfile);
    shxOutputDock->show();
    stoppRefinement->show();
    QObject::disconnect(stoppRefinement, 0, this, 0);
    refineButtonBox->hide();
    loadRes->show();
    shxEditorDock->hide();
    dock->hide();
    histDock->hide();
    renamDock->hide();

    QString dn=dirName.section('/',0,-2),
	    argument=dirName.section('/',-1);
    argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
    if (dirName.contains(QRegExp(".res$",Qt::CaseInsensitive))){
      QString insname =dirName;
      insname.replace(QRegExp(".res$",Qt::CaseInsensitive),".ins");

      QString fcfname = dirName;
      fcfname.replace(QRegExp(".res$",Qt::CaseInsensitive),".fcf");
      QString fcfname2 = dirName;
      fcfname2.replace(QRegExp(".res$",Qt::CaseInsensitive),".2fcf");
//      qDebug()<<QFileInfo(insname).lastModified().toString(Qt::TextDate)<<QFileInfo(dirName).lastModified().toString(Qt::TextDate)<<(QFileInfo(dirName).lastModified()>=QFileInfo(insname).lastModified());
      if (//(0)&&  
          (QFileInfo(dirName).lastModified()>=QFileInfo(insname).lastModified())){
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
	QFile::copy(insname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(insname);
	QFile::copy(dirName,insname);

	QFile::copy(fcfname2,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(fcfname2);
	QFile::copy(fcfname,fcfname2);
      }
      else {
	int but=QMessageBox::information(0,"information",
QString("<h3>The <b>ins</b>truction file is newer than the <b>res</b>ult file you are working on!</h3> %1 %2 <br> %3 <br> %4<br>Maybe you edited it outside of this program.\
  <br> Now the your computer clock says: %5<br>%6 %7 (negative numbers here probably indicate asynchronous clocks on network file systems.)<br><hr><b> Click on 'Ignore' to copy your current .res to .ins file and refine.</b>")
		.arg(insname)
		.arg(dirName)
		.arg(QFileInfo(insname).lastModified().toString(Qt::TextDate))
		.arg(QFileInfo(dirName).lastModified().toString(Qt::TextDate))
                .arg(QDateTime::currentDateTime().toString(Qt::TextDate))
#if (QT_VERSION >= 0x040700)
                .arg(QFileInfo(insname).lastModified().msecsTo(QDateTime::currentDateTime()))
                .arg(QFileInfo(dirName).lastModified().msecsTo(QDateTime::currentDateTime()))
#else 
                .arg(QFileInfo(insname).lastModified().secsTo(QDateTime::currentDateTime()))
                .arg(QFileInfo(dirName).lastModified().secsTo(QDateTime::currentDateTime()))

#endif
                ,QMessageBox::Ignore|QMessageBox::Ok);
       if (but==QMessageBox::Ignore){
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
	QFile::copy(insname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(insname);
	QFile::copy(dirName,insname);

	QFile::copy(fcfname2,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(fcfname2);
	QFile::copy(fcfname,fcfname2);
      }else{
	shxOutputDock->hide();
	refineButtonBox->hide();
	stoppRefinement->hide();
	shxEditorDock->show();
	dock->show();
	histDock->show();
	return;
       }
      }
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
      QFile::copy(dirName,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));

    }

    shxOutputDock->setWindowTitle(QString("SHELXL OUTPUT: %1").arg(argument));
    shelxl = new QProcess(this);

    QStringList al;
    al.append(argument);
    al+=shelxlArgs.split(' ',QString::SkipEmptyParts);
    shelxl->setWorkingDirectory(dn);                           // updateXLput()
    connect(shelxl,SIGNAL(readyReadStandardOutput ()),this,SLOT(updateXLput()));
    connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),refineButtonBox ,SLOT(show()));
    connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),loadRes ,SLOT(setFocus()));
    connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),stoppRefinement ,SLOT(hide()));
    connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(updateXLput()));

    connect(shelxl,SIGNAL(error(QProcess::ProcessError)),this,SLOT(updateXLput())); 
#if defined(Q_WS_WIN)
    connect(stoppRefinement,SIGNAL(clicked()),shelxl,SLOT(kill()));
#else
    connect(stoppRefinement,SIGNAL(clicked()),shelxl,SLOT(terminate()));
#endif
    connect(stoppRefinement,SIGNAL(clicked()),loadRes,SLOT(hide()));
    shelxl->start(shelxlPath,al);
  }
}

void Window::updateXLput(){
  if (shelxl->pid()) {
    xlPutor->setReadOnly(false);
    QString s=xlPutor->toPlainText();
    s.append(shelxl->readAllStandardOutput());
    s.append(shelxl->readAllStandardError());
    if (shelxl->state()==QProcess::NotRunning) {
      s.append("** NO running Process maybe the 'shelxl' is not in PATH or is spelled differently **\n\n** Please give a correct path to an existing executable of shelxl.**\n");
      refineButtonBox->show();
      stoppRefinement->hide();
    }
    xlPutor->setPlainText(s);
    falschAtom=-1;
    xlPutor->setReadOnly(true);
    loadRes->setEnabled((s.contains("for all data")));
    if (!s.contains("for all data")) withdraw->setFocus();
    if (s.contains(" ** ")) {
      QString Fehl=s.section("**",1,1).simplified();
      Fehl=Fehl.remove("UNSUITABLE AFIX HYDROGENS FOLLOW ");
      Fehl=Fehl.remove("BAD Ueq DEPENDENCE FOR ");
      Fehl=Fehl.remove("BAD AFIX FOR ");
      Fehl=Fehl.remove("BAD AFIX CONNECTIVITY: ");
      Fehl=Fehl.remove("NO PIVOT ATOM FOR RIGID GROUP CONTAINING ");
      /*Fehl=Fehl.remove("");
      Fehl=Fehl.remove("");
      Fehl=Fehl.remove("");
      Fehl=Fehl.remove("");
      Fehl=Fehl.remove("");
      Fehl=Fehl.remove("");*/
      Fehl=Fehl.simplified();
      Fehl=Fehl.section(' ',0,0);
      //qDebug()<<Fehl;
      if ((Fehl.size()<5)||Fehl.contains('_')){
	chgl->mol->selectedatoms.clear();
	for (int i=0; i<chgl->mol->showatoms.size(); i++){
	  if (!Fehl.compare(chgl->mol->showatoms.at(i).Label,Qt::CaseInsensitive)){
	    chgl->mol->selectedatoms.append(chgl->mol->showatoms[i]);
	    chgl->mol->selectedatoms.last().style=i;
	  //  qDebug()<<chgl->mol->showatoms.at(i).Label;
	  }
	}
	if (!chgl->mol->selectedatoms.isEmpty()) {
	  chgl->centerSelection->setChecked(true);
	  chgl->updateBondActions();
	  chgl->updateGL();
	  falschAtom=chgl->mol->selectedatoms.last().style;
	  jumpToAtom(chgl->mol->selectedatoms.last().style);
	  
	}
      }
    }
  }else{
    xlPutor->setReadOnly(false);
    QString s=xlPutor->toPlainText();
    s.append(shelxl->readAllStandardOutput());
    s.append(shelxl->readAllStandardError());
    if (s.isEmpty()){
    s.append("** NO running Process maybe the 'shelxl' is not in PATH or is spelled differently **\n\n** Please give a correct path to an existing executable of shelxl. **\n");
    }
    xlPutor->setPlainText(s);
    xlPutor->setReadOnly(true);
    if (!s.contains("WGHT")){
    loadRes->setEnabled((s.contains("for all data")));
  
    refineButtonBox->show();
    stoppRefinement->hide();
    }
  }// */

  QTextCursor tc = xlPutor->textCursor();
  tc.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  xlPutor->setTextCursor(tc);
  xlPutor->ensureCursorVisible();
  update();
}

void Window::loadrefres(){
  dirName.replace(QRegExp(".ins$",Qt::CaseInsensitive),".res");
  shxOutputDock->hide();
  refineButtonBox->hide();
  shxEditorDock->show();
  shxEditorDock->raise();

  loadFile(dirName);
}

void Window::replyFinished(QNetworkReply* antwort){
QSettings *einstellung =new QSettings( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME );
int ignore=0;
einstellung->beginGroup("Updates");
if (einstellung->contains("IgnoreUpdates"))
  ignore=einstellung->value("IgnoreUpdates").toInt();
einstellung->endGroup();
QString a=antwort->readAll();
latestRev=a;
bool ok;
int lat=a.toInt(&ok,10);
if (ok&&lat&&(0==(ignore&QMessageBox::No))){
    if ((lat-7)>Revision) {
    int later=
        QMessageBox::information(
        0,"information",QString(
                "<h3>This is revision: <font color=#aa66cc>%1"
                "</font><br> The latest available version is revision: <font color=#aa66cc> %2</font> "
                "New versions can be downloaded here: "
                "<a href=\"http://ewald.ac.chemie.uni-goettingen.de/shelx/index.php\">"
                "http://ewald.ac.chemie.uni-goettingen.de/shelx/index.php</a></h3>"
                "<h4>Please contact the author"
                " <a href=\"mailto:chuebsch@moliso.de\">chuebsch@moliso.de</a> if you find any bugs.<br> Thank you!</h4>"
                "<h3>Do you want to be reminded later?</h3>").arg(Revision).arg(lat)
                ,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

    einstellung->beginGroup("Updates");
    einstellung->setValue("IgnoreUpdates",later);
    einstellung->endGroup();
    }
}else{
    latestRev="<font color=red> network unavailable, check failed.</font>";
}
antwort->close();
antwort->deleteLater();
  disconnect(net,SIGNAL(finished(QNetworkReply*)),0,0);
  delete einstellung;
}

void Window::checkForUpdates(){
  //disconnect(net,SIGNAL(finished(QNetworkReply*)),0,0);
  connect(net, SIGNAL(finished(QNetworkReply*)), this, SLOT(isUpToDate(QNetworkReply*)));
  reply = net->get(QNetworkRequest(QUrl("http://ewald.ac.chemie.uni-goettingen.de/shelx/revision.php")));
}

void Window::isUpToDate(QNetworkReply* antwort){
  QSettings *einstellung =new QSettings( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME );
  //int ignore=0;
  einstellung->beginGroup("Updates");
  if (einstellung->contains("IgnoreUpdates"))
//    ignore=
	    einstellung->value("IgnoreUpdates").toInt();
  einstellung->endGroup();
  QString a=antwort->readAll();
  latestRev=a;
  bool ok;
  int lat=a.toInt(&ok,10);
  if (ok&&lat){
      int later=
	      QMessageBox::information(
			      0,"information",QString(
				      "<h3>This is revision: <font color=#aa66cc>%1"
				      "</font><br> The latest available version is revision: <font color=#aa66cc> %2</font> "
				      "<h3>%3</h3>"
				      "New versions can be downloaded here: "
				      "<a href=\"http://ewald.ac.chemie.uni-goettingen.de/shelx/index.php\">"
				      "http://ewald.ac.chemie.uni-goettingen.de/shelx/index.php</a></h3>"
				      "<h4>Please contact the author"
				      " <a href=\"mailto:chuebsch@moliso.de\">chuebsch@moliso.de</a> if you find any bugs.<br> Thank you!</h4>"
                                      "<h3>Do you want to be reminded for new versions at startup?</h3>").arg(Revision).arg(lat).arg(((lat-7)>Revision)?"You should probably update ShelXle.":"Your ShelXle is rather up to date.")
			      ,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

    einstellung->beginGroup("Updates");
    einstellung->setValue("IgnoreUpdates",later);
    einstellung->endGroup();
  }else{
    latestRev="<font color=red> network unavailable, check failed.</font>";
    QMessageBox::information( 0,"information",latestRev);
  }
  antwort->close();
  antwort->deleteLater();

  disconnect(net,SIGNAL(finished(QNetworkReply*)),0,0);

}

void Window::changeSortierWeise(QAction* action){
   int index = action->data().toInt();
   sortierWeise = index;
    mole.einstellung->beginGroup("Sorting");
    mole.einstellung->setValue("SortingOption",sortierWeise);
    mole.einstellung->endGroup();
}

void Window::sortAtoms(){
  if (mole.asymm.isEmpty()) return;

    const QString sicher = editor->toPlainText();
    //find atomranges
    if (sicher.contains(QRegExp("[<>]"))){
        QStringList ranges;
        QStringList lines = sicher.split('\n');
        for (int i=0; (i<lines.size()); i++){
            if (lines.at(i).startsWith(' ')) continue;
            if (lines.at(i).startsWith("REM",Qt::CaseInsensitive)) continue;
            if (lines.at(i).startsWith("HKLF",Qt::CaseInsensitive)) break;
            if (lines.at(i).contains(QRegExp("[<>]"))) ranges.append(QString("LINE %1: %2")
                .arg(i+1)
                .arg(lines.at(i)));
        }
        if (!ranges.isEmpty()){
            QMessageBox::information(0,"information",QString("<h3>Atom ranges detected! %1 "
                    " can not sort here without destroying functionality of the file.</h3 <pre>%2 </pre>").arg(PROGRAM_NAME).arg(ranges.join("\n")));
            return;
        }
    }
    QMap<long double,MyAtom> atomSortMap;

    int amax = mole.asymm.size();
    int t1=0,t2=0;
    for (int i=0; i < mole.asymm.size(); i++){
        if (mole.asymm.at(i).an<0) continue;
	t1++;
        QString s1 = mole.asymm.at(i).Label;
        s1.remove(0,mole.pse(mole.asymm.at(i).an).size());
        int n1 = s1.section(QRegExp("\\D"),0,0).toInt();
        int r1 = 0 ;
        char cc[10];
        strncpy(cc,s1.section(QRegExp("\\d+"),1,-1).toStdString().c_str(),4);
        for (size_t k=0; k<strlen(cc);k++) {r1*=256;r1+=(size_t)cc[k];}
        switch (sortierWeise){
        case 1:
                atomSortMap.insertMulti(
                mole.asymm.at(i).resiNr*99*amax+
                mole.asymm.at(i).part*amax+
                n1+
                ((110-mole.asymm.at(i).an)/220.0)+
                (r1 / 50000.0)//46656
                ,mole.asymm.at(i));
                break;
        case 2:
                atomSortMap.insertMulti(
                mole.asymm.at(i).resiNr*99*amax+
                mole.asymm.at(i).part*amax+
                n1/999.0+
                ((110-mole.asymm.at(i).an))+
                (r1 / 50000.0)//46656
                ,mole.asymm.at(i));
                break;
        case 3:
                atomSortMap.insertMulti(
                mole.asymm.at(i).resiNr*99*amax+
                mole.asymm.at(i).part*amax+
                n1/999.0+
                sfac.indexOf(mole.asymm.at(i).an)*10+
                (r1 / 50000.0)//46656
                ,mole.asymm.at(i));
                break;
        case 4:
                atomSortMap.insertMulti(
                mole.asymm.at(i).resiNr*99*amax+
                mole.asymm.at(i).part*amax+
                n1/50000.0+
                ((110-mole.asymm.at(i).an)/220)+
                (r1 )//46656
                ,mole.asymm.at(i));
                t2++;
                break;
        case 5:
                atomSortMap.insertMulti(
                mole.asymm.at(i).molindex*9999*amax+
                mole.asymm.at(i).resiNr*99*amax+
                mole.asymm.at(i).part*amax+
                n1/999.0+
                ((110-mole.asymm.at(i).an))+
                (r1 / 50000.0)//46656
                ,mole.asymm.at(i));
		printf("%s %g\n", mole.asymm.at(i).Label.toStdString().c_str(),mole.asymm.at(i).molindex*9999*amax+
				mole.asymm.at(i).resiNr*99*amax+
				mole.asymm.at(i).part*amax+
				n1/999.0+
				((110-mole.asymm.at(i).an))+
				(r1 / 50000.0));
                break;
        default:
                atomSortMap.insert(i
                ,mole.asymm.at(i));
                break;
    }
    }
    CEnvironment asymm2=
            atomSortMap.values();

    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    wholeLine->setChecked(false);
    replace->setText("REM PART ");
    search->setText("^part ");
    replaceAll();
    replace->setText("REM RESI ");
    search->setText("^resi ");
    replaceAll();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::Start);
    cursor = document->find("HKLF",cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
    if (editor->parenthesis.at(cursor.blockNumber())){
      cursor.select(QTextCursor::WordUnderCursor);
      if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
        cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
      cursor.select(QTextCursor::WordUnderCursor);
      if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
        cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      cursor.clearSelection();
      cursor.insertText("AFIX 0 \n");
    }

    QProgressDialog progress("Sorting atoms in Editor", QString(), 0, asymm2.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    int ares=0, apart=0;
    for (int i=0; i < asymm2.size(); i++){
        progress.setValue(i);
        QString  label = asymm2.at(i).orginalLine.left(80).trimmed();
        cursor.movePosition(QTextCursor::Start);
        cursor = document->find(label,cursor);
        if (cursor.isNull()){
            cursor = editor->textCursor();
            cursor.movePosition(QTextCursor::Start);
            label.chop(10);
            cursor = document->find(label,cursor);
            if (cursor.isNull()) {
                cursor = editor->textCursor();
                printf("hab was nicht %d %s\n",i,label.toStdString().c_str());
                continue;
            }
        }

        if (editor->parenthesis.at(cursor.blockNumber()))continue;//Afix not sorted here
        QString vorherig,neues;
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
        while (cursor.selectedText().endsWith("=")) {
            cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
            cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
        }
        while (editor->parenthesis.at(cursor.blockNumber()+1)){
            cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
            cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
        }
        neues = vorherig= cursor.selectedText();
        cursor.deleteChar();
        cursor.deleteChar();
        cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor = document->find("HKLF",cursor);
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::StartOfLine);
        if ((asymm2.at(i).resiNr!=ares))
            cursor.insertText(QString("RESI %1 %2\n")
                              .arg(asymm2.at(i).resiNr)
                              .arg(asymm2.at(i).ResiClass));
        if ((asymm2.at(i).part!=apart))
            cursor.insertText(QString("PART %1\n")
                              .arg(asymm2.at(i).part));
        cursor.insertText(neues);
        cursor.insertText("\n");
        ares = asymm2.at(i).resiNr;
        apart = asymm2.at(i).part;
        editor->updateAfix();
    }
    cursor = document->find("HKLF",cursor);
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::StartOfLine);
    if ((0!=apart)) cursor.insertText(QString("PART 0\n"));
    if ((0!=ares)) cursor.insertText(QString("RESI 0\n"));
    cursor.endEditBlock ();
    updateLabel();

    chgl->disSelection();
    if (chgl->fuse->isVisible())chgl->mol->grow();
    else  chgl->mol->fuse();
    chgl->murx=1;
    editor->setUndoRedoEnabled ( true );
    chgl->updateGL();
    search->setText("");
    replace->setText("");
    replacementInfo->setText("");
    wholeLine->setChecked(false);
    editor->setTextCursor(cursor);

    progress.setValue(asymm2.size());
}

void Window::changeLabelFont(){
  bool ok;
  QFont font = QFontDialog::getFont(&ok, chgl->myFont, this,"Change Label Font");
  if (ok) {
    font.setStyleStrategy(QFont::OpenGLCompatible);
    chgl->myFont=font;
    mole.einstellung->beginGroup("Fonts");
    mole.einstellung->setValue("LabelFont",chgl->myFont);
    mole.einstellung->endGroup();
    chgl->updateGL();
  }
}

void Window::changeEditorFont(){
  bool ok;
  QFont font = QFontDialog::getFont(&ok, editor->font(), this,"Change Editor Font");
  if (ok) {
    font.setFixedPitch(true);
    mole.einstellung->beginGroup("Fonts");
    mole.einstellung->setValue("EditorFont",font);
    mole.einstellung->endGroup();
    editor->setFont(font);
    xlPutor->setFont(font);
  }
}

void Window::withdrawres(){
    if (dirName.contains(QRegExp(".res$",Qt::CaseInsensitive))){
      QString insname =dirName;
      insname.replace(QRegExp(".res$",Qt::CaseInsensitive),".ins");
      QString fcfname = dirName;
      fcfname.replace(QRegExp(".res$",Qt::CaseInsensitive),".fcf");
      QString fcfname2 = dirName;
      fcfname2.replace(QRegExp(".res$",Qt::CaseInsensitive),".2fcf");
      QFile::remove(dirName);
      QFile::copy(insname,dirName);
      QFile::remove(fcfname);
      QFile::copy(fcfname2,fcfname);
      editor->document()->setModified(false);
      loadFile(dirName);
    }
  shxOutputDock->hide();
  refineButtonBox->hide();
  shxEditorDock->show();
  if (falschAtom!=-1){
//    qDebug()<<"tata"<<falschAtom;
    chgl->mol->selectedatoms.clear();
    chgl->mol->selectedatoms.append(chgl->mol->showatoms[falschAtom]);
    chgl->mol->selectedatoms.last().style=falschAtom;
    chgl->centerSelection->setChecked(true);
    chgl->updateBondActions();
    chgl->updateGL();
    jumpToAtom(falschAtom);
	  
  }
}

void Window::loadLst(){
  xlPutor->clear();

  QString listName=dirName;
  listName.replace(QRegExp(".ins$",Qt::CaseInsensitive),".lst");
  listName.replace(QRegExp(".res$",Qt::CaseInsensitive),".lst");
  QFile lstf(listName);
  int wl=0;
  if (lstf.open(QIODevice::ReadOnly|QIODevice::Text)){
  xlPutor->setReadOnly(false);
  xlPutor->ListFile=true;
//  xlPutor->highlighter->setOutputStyle();
  shxOutputDock->setWindowTitle(QString("LIST FILE: %1").arg(listName.section('/', -1)));
  QString testS=QString(lstf.readAll().replace('\0','~'));
  xlPutor->setPlainText(testS);
  wl=testS.count("\n");
  lstf.close();

  QPalette p = xlPutor->palette(); 
  p.setBrush(QPalette::Active,QPalette::Base,QColor(255,255,230));
  xlPutor->setPalette (p );
  }

  shxOutputDock->show();
  mole.einstellung->beginGroup("Cursor");

  if (mole.einstellung->contains("ListFilePos")){
      xlPutor->setReadOnly(false);
      int mipo= qMin(wl,mole.einstellung->value("ListFilePos",0).toInt());       
     // printf("mipo %d\n",mipo);
      QTextCursor c=xlPutor->textCursor();
     // qDebug()<<c.isNull();
      c.movePosition(QTextCursor::Start);
      c.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,mipo);
      if (!c.isNull()) xlPutor->setTextCursor(c);
      xlPutor->centerCursor();
      xlPutor->setReadOnly(true);
  }
  mole.einstellung->endGroup();
  xlPutor->setReadOnly(true);
//  shxOutputDock->raise();
  update();
}

void Window::loadFile(QString fileName){

  QDir directory(fileName);
  QString dn=directory.canonicalPath();
  if ((editor->toPlainText().length()>80)){//(dn!=dirName)&&
      QTextCursor c=editor->textCursor();
  if (!c.isNull()){
      myHash[dirName]=c.position();
      mole.einstellung->beginGroup("Cursor");
      mole.einstellung->setValue("lastCursorHash", myHash );
      mole.einstellung->setValue("ListFilePos",xlPutor->midCursorpos);
      mole.einstellung->endGroup();
   }
  }
  dirName=dn;
  mole.einstellung->beginGroup("Files");
  mole.einstellung->setValue("lastFile", dirName );
  mole.einstellung->endGroup();
  
  fileName=dirName;
  if (fileName.isEmpty()) return;
  bool gut=false;
  QFile test;
  QString alltest;
  test.setFileName(fileName);
  if (test.exists()&&test.size()){
    test.open(QIODevice::ReadOnly|QIODevice::Text);
    alltest=test.readAll();
    gut= ((alltest.contains("UNIT",Qt::CaseInsensitive))&&(alltest.contains("HKLF",Qt::CaseInsensitive)||alltest.contains("END",Qt::CaseInsensitive)));
    test.close();
  } 
  if (!gut){
    fileName.replace(QRegExp("res$",Qt::CaseInsensitive),"ins");
    test.setFileName(fileName);
    if (test.exists()&&test.size()){
    test.open(QIODevice::ReadOnly|QIODevice::Text);
    alltest=test.readAll();
    gut= ((alltest.contains("UNIT",Qt::CaseInsensitive))&&(alltest.contains("HKLF",Qt::CaseInsensitive)||alltest.contains("END",Qt::CaseInsensitive)));
    test.close();
    if (gut ) dirName=fileName;
    } 
  }
  if (!gut) {

    test.open(QIODevice::ReadOnly|QIODevice::Text);
    alltest=test.readAll();
    test.close();
    return;  
  }

  packMenu->setEnabled(true);
  if ((Revision%666)==0)
  setWindowTitle(QString("%1 - A Qt GUI for sHELLxl (Rev: %2): %3[*] ")
              .arg(PROGRAM_NAME)
              .arg(Revision)
              .arg(fileName.section("/",-1)));
  else
  setWindowTitle(QString("%1 - A Qt GUI for SHELXL (Rev: %2): %3[*] ")
		  .arg(PROGRAM_NAME)
		  .arg(Revision)
		  .arg(fileName.section("/",-1)));
  loadLst();
  shx_editor(fileName,load_sheldrick(fileName));
  mole.einstellung->beginGroup("Files");
  mole.einstellung->setValue("lastFile", fileName );
  QStringList files = mole.einstellung->value("recentFileList").toStringList();
  files.removeAll(fileName);//alle identischen "fileName" aus der Liste entfernen
  files.prepend(fileName);// und dann vorne anfuegen...
  while (files.size() > MaxRecentFiles)
    files.removeLast();//hinten abschneiden was laenger als MaxRecentFiles ist
  mole.einstellung->setValue("recentFileList", files);
  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    Window *mainWin = qobject_cast<Window *>(widget);
    if (mainWin)
      mainWin->updateRecentFileActions();
  }
  mole.einstellung->endGroup();
  hw->setPath(fileName, dock->width());
  if (hw->gooddata.size())
    histDock->show();
}

void Window::openRecentFile() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
    loadFile(action->data().toString());
}

void Window::closeEvent(QCloseEvent *event)  {
  if (maybeSave()){
    mole.einstellung->beginGroup("Window");
    mole.einstellung->setValue("geometry", saveGeometry());
    mole.einstellung->setValue("state", saveState());
    mole.einstellung->setValue("InvertMouseZoom",(chgl->invertMouseZoom->isChecked()));
    mole.einstellung->setValue("Calculate-Maps",fxle->doMaps->isChecked());
    mole.einstellung->setValue("Fo-Map",chgl->foact->isChecked());
    mole.einstellung->setValue("Fc-Fo-Map",chgl->fofcact->isChecked());
    mole.einstellung->setValue("Map-truncation-type",mapSchnitt->currentIndex());
    mole.einstellung->setValue("Map-Line-Transparency",fxle->lintrans);
    mole.einstellung->setValue("Map-Line-Width", fxle->linwidth);
    mole.einstellung->setValue("Map-Resolution",fxle->rr);
    mole.einstellung->setValue("Map-Weight",fxle->rw);
    mole.einstellung->setValue("Map-Lighting",chgl->lighting->isChecked());
    mole.einstellung->setValue("Nice-Transparency",chgl->niceTrans->isChecked());
    mole.einstellung->setValue("Solid-Map",chgl->fillMap->isChecked());
    mole.einstellung->setValue("HighlightEquivalents",mole.highlightEquivalents);
    mole.einstellung->setValue("ADPs",togElli->isChecked());
    mole.einstellung->setValue("Tubes",tubeStyle->isChecked());
    mole.einstellung->setValue("ShowLabels",togLabel->isChecked());
    mole.einstellung->setValue("BackGroundGradient",togBackGr->isChecked());
    mole.einstellung->setValue("DepthCueing",togDepthCueing->isChecked());
    mole.einstellung->setValue("Xray-view",togXRayView->isChecked());
    mole.einstellung->setValue("SHELXL-PATH",shelxlPath);
    mole.einstellung->setValue("LevelOfDetail",mole.LOD);
    mole.einstellung->setValue("WireModelOnRotate",chgl->wireButt->isChecked());
    mole.einstellung->endGroup();
    mole.einstellung->beginGroup("Colors");
    mole.einstellung->setValue("LabelColor",chgl->labelColor);
    mole.einstellung->setValue("BackGroundColor",chgl->backGroundColor);
    mole.einstellung->endGroup();
    mole.einstellung->beginGroup("Files");
    mole.einstellung->setValue("lastFile", dirName );
    mole.einstellung->endGroup();
    mole.einstellung->beginGroup("Fonts");
    mole.einstellung->setValue("EditorFont",editor->font());
    mole.einstellung->setValue("LabelFont",chgl->myFont);
    mole.einstellung->endGroup();
    mole.einstellung->beginGroup("ToolBarIcons");
    mole.einstellung->setValue("ToolBarIconSize",tbiconSize);
    mole.einstellung->endGroup();
    QTextCursor c=editor->textCursor();
    if (!c.isNull()){
        myHash[dirName]=c.position();
        mole.einstellung->beginGroup("Cursor");
        mole.einstellung->setValue("lastCursorHash", myHash );
        mole.einstellung->endGroup();
    }
    delete  mole.einstellung;
    //    printf("deleting\n");
    mole.asymm.clear();
    mole.sdm.clear();
    mole.showatoms.clear();
    mole.showbonds.clear();
    mole.cell.symmops.clear();
    mole.cell.trans.clear();
    mole.selectedatoms.clear();
    sfac.clear();
    fvar.clear();
    fvarCntr.clear();
    mole.transEQIV.clear();
    mole.symmopsEQIV.clear();
    mole.labelEQIV.clear();
    mole.freeatoms.clear();
    mole.bindatoms.clear();
    disconnect(this);
    disconnect(editor);
    disconnect(chgl);
    editor->clear();
    /*
    delete hilfe;
    printf("Line %d OK.\n",__LINE__);
    delete helpDock;
    printf("Line %d OK.\n",__LINE__);

    delete resiFinder;
    printf("Line %d OK.\n",__LINE__);
    delete selectPart;
    printf("Line %d OK.\n",__LINE__);
    delete spinSelectPart;
    printf("Line %d OK.\n",__LINE__);
    delete inculdePartNull;
    printf("Line %d OK.\n",__LINE__);
    delete spellCheckact;
    printf("Line %d OK.\n",__LINE__);
    delete lineColumn;
    printf("Line %d OK.\n",__LINE__);
    delete nlt;
    printf("Line %d OK.\n",__LINE__);
    delete navibox;
    printf("Line %d OK.\n",__LINE__);

    delete completer;
    printf("Line %d OK.\n",__LINE__);
    delete search;
    printf("Line %d OK.\n",__LINE__);
    delete prev;
    printf("Line %d OK.\n",__LINE__);
    delete next;
    printf("Line %d OK.\n",__LINE__);
    delete replaceButton;
    printf("Line %d OK.\n",__LINE__);
    delete replacePButton;
    printf("Line %d OK.\n",__LINE__);
    delete replaceAButton; 
    printf("Line %d OK.\n",__LINE__);

    delete wholeLine;
    printf("Line %d OK.\n",__LINE__);
    delete replacementInfo;
    printf("Line %d OK.\n",__LINE__);
    delete replace;
    printf("Line %d OK.\n",__LINE__);
    delete slt;
    printf("Line %d OK.\n",__LINE__);
    delete suchbox;
    printf("Line %d OK.\n",__LINE__);


    delete prev2;
    printf("Line %d OK.\n",__LINE__);
    delete next2;
    printf("Line %d OK.\n",__LINE__);
    delete search2;
    printf("Line %d OK.\n",__LINE__);
    delete slt2;
    printf("Line %d OK.\n",__LINE__);
    delete suchbox2;
    printf("Line %d OK.\n",__LINE__);

    delete xlPutor;
    printf("Line %d OK.\n",__LINE__);
    editor->document()->disconnect();
    editor->setgl(NULL);

    editor->document()->disconnect();
    delete editor;
    printf("Line %d OK.\n",__LINE__);
    delete sss;
    printf("Line %d OK.\n",__LINE__);
    delete shx_group; 
    printf("Line %d OK.\n",__LINE__);
    delete shxEditorDock;
    printf("Line %d OK.\n",__LINE__);
    loadRes->clearFocus();
    loadRes->hide();
    disconnect(loadRes);
    delete loadRes ;
    printf("Line %d OK.\n",__LINE__);
    delete withdraw;
    printf("Line %d OK.\n",__LINE__);
    delete showLst;
    printf("Line %d OK.\n",__LINE__);
    delete buttomLt;
    printf("Line %d OK.\n",__LINE__);
    delete refineButtonBox;
    printf("Line %d OK.\n",__LINE__);
    infoKanal->clear();
    delete infoKanal;
    printf("Line %d OK.\n",__LINE__);
    delete dock;
    printf("Line %d OK.\n",__LINE__);
    delete nextNameL;
    printf("Line %d OK.\n",__LINE__);
    delete hintL;
    printf("Line %d OK.\n",__LINE__);
    delete rufModeBox;
    printf("Line %d OK.\n",__LINE__);
    delete partSpinL;
    printf("Line %d OK.\n",__LINE__);
    delete partSpin;
    printf("Line %d OK.\n",__LINE__);
    delete resiNrSpinL;
    printf("Line %d OK.\n",__LINE__);
    delete resiNrSpin;
    printf("Line %d OK.\n",__LINE__);
    delete resiResiClassL;
    printf("Line %d OK.\n",__LINE__);
    delete resiResiClass;
    printf("Line %d OK.\n",__LINE__);
    QObject::disconnect(stoppRefinement, 0, this, 0);
    delete stoppRefinement;// 
    printf("Line %d OK.\n",__LINE__);
    delete tieOccL; 
    printf("Line %d OK.\n",__LINE__);
    delete tieType; 
    printf("Line %d OK.\n",__LINE__);
    delete tieFVNrL; 
    printf("Line %d OK.\n",__LINE__);
    delete tieFVNr ;
    printf("Line %d OK.\n",__LINE__);
    delete tieUsedL ;
    disconnect(dsm);
    for (int i=0; i<109; i++) if (pserbt[i]) {
      disconnect(pserbt[i]);
      delete pserbt[i];
    }
    delete dsm;
    printf("Line %d OK.\n",__LINE__);
    delete sfacla;;
    printf("Line %d OK.\n",__LINE__);
    delete sfacBox ;
    printf("Line %d OK.\n",__LINE__);
    delete rhla;
    printf("Line %d OK.\n",__LINE__);
    delete rhla2;
    printf("Line %d OK.\n",__LINE__);
    delete psew;
    printf("Line %d OK.\n",__LINE__);
    delete willpse;
    printf("Line %d OK.\n",__LINE__);
    delete indexL;
    printf("Line %d OK.\n",__LINE__);
    delete indexSpin;
    printf("Line %d OK.\n",__LINE__);
    delete indexla;
    printf("Line %d OK.\n",__LINE__);
    delete sufixBox;
    printf("Line %d OK.\n",__LINE__);
    delete nliL;
    printf("Line %d OK.\n",__LINE__);
    delete rnla;
    printf("Line %d OK.\n",__LINE__);
    delete renameBox;
    printf("Line %d OK.\n",__LINE__);
    delete vlt;
    printf("Line %d OK.\n",__LINE__);
    delete gp;
    printf("Line %d OK.\n",__LINE__);
    delete renamDock;
    printf("Line %d OK.\n",__LINE__);
    delete shxOutputDock;
    printf("Line %d OK.\n",__LINE__);
    delete net;
    printf("Line %d OK.\n",__LINE__);
    delete qpv;
    printf("Line %d OK.\n",__LINE__);
    delete qPeakDock;
    printf("Line %d OK.\n",__LINE__);
    delete hw;
    printf("Line %d OK.\n",__LINE__);
    delete histDock;
    printf("Line %d OK.\n",__LINE__);
    delete balken;
    printf("Line %d OK.\n",__LINE__);
    for (int i = 0; i < MaxRecentFiles; ++i) {
      delete recentFileActs[i];
    printf("Line %d OK.\n",__LINE__);
    }
    if (fxle->n5){
      fxle->deleteLists();
      fxle->killmaps();
      delete fxle;    
    printf("Line %d OK.\n",__LINE__);
    }
    delete fileMenu;
    printf("Line %d OK.\n",__LINE__);
    delete editMenu;
    printf("Line %d OK.\n",__LINE__);
    delete dialogMenu;
    printf("Line %d OK.\n",__LINE__);
    delete viewMenu;
    printf("Line %d OK.\n",__LINE__);
    delete runMenu;
    printf("Line %d OK.\n",__LINE__);
    delete packMenu;
    printf("Line %d OK.\n",__LINE__);
    delete extraMenu;
    printf("Line %d OK.\n",__LINE__);
    delete fito;
    printf("Line %d OK.\n",__LINE__);
    delete actionSave;
    printf("Line %d OK.\n",__LINE__);
    delete actionSaveAs;
    printf("Line %d OK.\n",__LINE__);
    delete backColor;
    printf("Line %d OK.\n",__LINE__);
    delete labelColor;
    printf("Line %d OK.\n",__LINE__);
    delete editToolBar;
    printf("Line %d OK.\n",__LINE__);
    delete actionUndo;
    printf("Line %d OK.\n",__LINE__);
    delete actionRedo;
    printf("Line %d OK.\n",__LINE__);
    delete actionCut;
    printf("Line %d OK.\n",__LINE__);
    delete actionCopy;
    printf("Line %d OK.\n",__LINE__);
    delete actionPaste;
    printf("Line %d OK.\n",__LINE__);
    delete vito;
    printf("Line %d OK.\n",__LINE__);
    delete stereoMenu;
    printf("Line %d OK.\n",__LINE__);
    delete stereoGroup;
    printf("Line %d OK.\n",__LINE__);
    delete moveMenu;
    printf("Line %d OK.\n",__LINE__);
    delete tubeGroup;
    printf("Line %d OK.\n",__LINE__);
    delete chgl->lighting;
    printf("Line %d OK.\n",__LINE__);
    delete chgl->niceTrans;
    printf("Line %d OK.\n",__LINE__);
    delete chgl->fillMap;
    printf("Line %d OK.\n",__LINE__);
    //QMenu *m;
    delete helpMenu; 
    printf("Line %d OK.\n",__LINE__);
    delete chgl->rCenter; 
    printf("Line %d OK.\n",__LINE__);
    delete chgl->invertMouseZoom; 
    printf("Line %d OK.\n",__LINE__);
    delete qGrowPeak; 
    printf("Line %d OK.\n",__LINE__);
    delete sLabel; 
    printf("Line %d OK.\n",__LINE__);
    delete zoomIn ;
    printf("Line %d OK.\n",__LINE__);
    delete zoomOut; 
    printf("Line %d OK.\n",__LINE__);
    delete extraTB;
    printf("Line %d OK.\n",__LINE__);
    delete lt ;
    printf("Line %d OK.\n",__LINE__);
    delete extraOutPut ;
    printf("Line %d OK.\n",__LINE__);
    delete extraOutput ;
    printf("Line %d OK.\n",__LINE__);
    delete sortGroup ;
    printf("Line %d OK.\n",__LINE__);
    delete sortMenu ;
    printf("Line %d OK.\n",__LINE__);

    delete scalwla ;
    printf("Line %d OK.\n",__LINE__);
    delete scalhla ;
    printf("Line %d OK.\n",__LINE__);
    delete scalsla ;
    printf("Line %d OK.\n",__LINE__);
    delete scalla ;
    printf("Line %d OK.\n",__LINE__);

    delete widthBox     ;   
    printf("Line %d OK.\n",__LINE__);
    delete heightBox     ;  
    printf("Line %d OK.\n",__LINE__);
    delete scalefBox ;
    printf("Line %d OK.\n",__LINE__);

    delete header ;
    printf("Line %d OK.\n",__LINE__);
    delete buttonBoxMC ;
    printf("Line %d OK.\n",__LINE__);
    delete wl;
    printf("Line %d OK.\n",__LINE__);
    delete pl;
    printf("Line %d OK.\n",__LINE__);
    delete dl;
    printf("Line %d OK.\n",__LINE__);
    delete ol;
    printf("Line %d OK.\n",__LINE__);
    delete rl;
    printf("Line %d OK.\n",__LINE__);
    delete sl;
    printf("Line %d OK.\n",__LINE__);
    delete lw;
    printf("Line %d OK.\n",__LINE__);
    delete ltz;
    printf("Line %d OK.\n",__LINE__);
    delete scalDlg  ;
    printf("Line %d OK.\n",__LINE__);
    delete mapprec ;
    printf("Line %d OK.\n",__LINE__);
    delete lineTrans; 
    printf("Line %d OK.\n",__LINE__);
    delete lineWidth ;
    printf("Line %d OK.\n",__LINE__);
    delete weak ;
    printf("Line %d OK.\n",__LINE__);
    delete difmaps ;
    printf("Line %d OK.\n",__LINE__);
    delete fomaps ;
    printf("Line %d OK.\n",__LINE__);
    delete maprad ;
    printf("Line %d OK.\n",__LINE__);
    delete mapSchnitt;
    printf("Line %d OK.\n",__LINE__);
    delete mdl ;
    printf("Line %d OK.\n",__LINE__);
    delete md;
    printf("Line %d OK.\n",__LINE__);
    delete chgl;
    printf("Line %d OK.\n",__LINE__);
    */
    mole.asymm.clear();
    mole.sdm.clear();
    mole.showbonds.clear();
    mole.showatoms.clear();
    mole.selectedatoms.clear();

    mole.envi_sdm.clear();
    mole.contact.clear();
    mole.symmopsEQIV.clear();
    mole.labelEQIV.clear();
    mole.transEQIV.clear();
    mole.freeatoms.clear();
    mole.bindatoms.clear();
    for (int i=0; i<mole.knoepfe.size();i++) mole.knoepfe[i].neighbors.clear();
    mole.knoepfe.clear();
    mole.cell.symmops.clear();
    mole.cell.trans.clear();
    event->accept();
  //  printf("line %d\n",__LINE__);
  }
  else
    event->ignore();
}

void Window::changeScalefactor(){
  widthBox->disconnect();
  heightBox->disconnect();
  scalefBox->disconnect();
  widthBox->setValue(int(chgl->width()*scalePic+0.5));
  heightBox->setValue(int(chgl->height()*scalePic+0.5));
  scalefBox->setValue(scalePic*100.0);
  connect(widthBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(heightBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(scalefBox,SIGNAL(valueChanged(double)),this,SLOT(updateScaleDlg()));
  scalDlg->exec();  
}

void Window::updateScaleDlg(){
  QObject *o = qobject_cast<QObject *>(sender());
  if (o) {
  QString name = o->objectName();
  widthBox->disconnect();
  heightBox->disconnect();
  scalefBox->disconnect();
  if (name=="scalefBox"){
    scalePic=scalefBox->value()*0.01;
    widthBox->setValue(int(chgl->width()*scalePic+0.5));
    heightBox->setValue(int(chgl->height()*scalePic+0.5));
  } else if (name=="widthBox"){
    scalePic= (double (widthBox->value() -0.5) / chgl->width()) ;
    heightBox->setValue(int(chgl->height()*scalePic+0.5));
    scalefBox->setValue(scalePic*100.0);
  } else if (name=="heightBox"){
    scalePic= (double (heightBox->value() -0.5) / chgl->height()) ;
    widthBox->setValue(int(chgl->width()*scalePic+0.5));
    scalefBox->setValue(scalePic*100.0);
  }
  scalDlg->update();
  
  connect(widthBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(heightBox,SIGNAL(valueChanged(int)),this,SLOT(updateScaleDlg()));
  connect(scalefBox,SIGNAL(valueChanged(double)),this,SLOT(updateScaleDlg()));
  }
}

void Window::updateSelectResi(){
  QStringList l = editor->document()->toPlainText().split('\n');
  if (resiFinder == NULL) return;
  resiFinder->clear();
  resiFinder->addItem("<Select residues>",-1);
  for (int i=0; i<l.size();i++){
    if ((l.at(i).startsWith("RESI",Qt::CaseInsensitive))&&(!l.at(i).contains(QRegExp(" 0\\b"))))
      resiFinder->addItem(l.at(i).trimmed(),i);
//    if (l.at(i).startsWith("MOLE",Qt::CaseInsensitive))
  //    resiFinder->addItem(l.at(i).trimmed(),i);
  }
  int mi=1,ma=-1;
  for (int i=0;i<editor->parenthesis2.size();i++){
    mi=qMin(editor->parenthesis2.at(i),mi);
    ma=qMax(editor->parenthesis2.at(i),ma);
  }
  spinSelectPart->setMinimum(mi);
  spinSelectPart->setMaximum(ma);
}

void Window::selectParts(){
  int pt=spinSelectPart->value();
  bool b=inculdePartNull->isChecked();
  chgl->mol->selectedatoms.clear();
  int hidden=0;
  if (selectPart->isChecked()){
    for (int i=0; i<chgl->mol->showatoms.size();i++){
      hidden+=chgl->mol->showatoms.at(i).hidden;
      if (chgl->mol->showatoms.at(i).part==pt){
	chgl->mol->selectedatoms.append(chgl->mol->showatoms[i]);
	chgl->mol->selectedatoms.last().style=i;
      }
      if ((pt!=0)&&(chgl->mol->showatoms.at(i).part==0)&&b){
	chgl->mol->selectedatoms.append(chgl->mol->showatoms[i]);
	chgl->mol->selectedatoms.last().style=i;
      }
    }
  }
  if (hidden) chgl->hideNonSelected();
  chgl->updateBondActions();
  chgl->updateGL();
}

void Window::resiSelectorCheck(){
  QTextCursor tc = editor->textCursor();
  int l,c=0,rnr=-1;
  l=tc.blockNumber();
  while ((c<resiFinder->count())&&(l>=(rnr=resiFinder->itemData(c).toInt()))) {c++;}
  if (c) resiFinder->setCurrentIndex(c-1);
  update();
}

void Window::fuse(){
  chgl->disSelection();
  chgl->mol->fuse();
  infoKanalNews(chgl->mol->HumanSymmetry);
  chgl->murx=1;
   chgl->setMolecule(&mole);
  chgl->fuse->setVisible(false);
  chgl->grow->setVisible(true);
  chgl->updateGL();
}

void Window::uniq(){
  if (mole.asymm.isEmpty()) return;
  chgl->disSelection();
  chgl->mol->uniqueInCell();
  infoKanalNews(chgl->mol->HumanSymmetry);
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();

  for (int i=0; i<mole.asymm.size();i++){
    cursor.movePosition(QTextCursor::Start);
    QString label=mole.asymm.at(i).orginalLine.left(80).trimmed();
    cursor = document->find(label ,cursor);  

    if (cursor.isNull()){
      cursor = editor->textCursor();    
      cursor.movePosition(QTextCursor::Start);
      label.chop(10);
      cursor = document->find(label,cursor);
      if (cursor.isNull()) {
	cursor = editor->textCursor();
      }
    }
    QString vorherig,neues;
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    while (cursor.selectedText().endsWith("=")) {
      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
    }
    neues = vorherig= cursor.selectedText();
    QStringList sl = vorherig.split(QRegExp("\\s+"));
    sl.removeOne("=");
    if (sl.size()>=7)
      neues=QString("%1  %2  %3 %4 %5 %6 %7 ")
	      .arg(mole.asymm.at(i).Label.section('_',0,0),-4)
	      .arg(qMax(1,sfac.indexOf(mole.asymm.at(i).an)+1))
              .arg(mole.asymm.at(i).frac.x,10,'f',6)
              .arg(mole.asymm.at(i).frac.y,10,'f',6)
              .arg(mole.asymm.at(i).frac.z,10,'f',6)
              .arg(mole.asymm.at(i).sof_org,9,'f',5)
              .arg((mole.asymm.at(i).uf.m22==-666.0)?mole.asymm.at(i).uf.m33:mole.asymm.at(i).uf.m11,9,'f',5);
    mole.asymm[i].orginalLine=neues;
    if (mole.asymm.at(i).an<0) {
        neues.append(QString("%1 ")
                        .arg(mole.asymm.at(i).peakHeight,9,'f',2));
    } else
    if (mole.asymm.at(i).uf.m22!=-666.0){
     neues.append(QString("%1 =\n    %2 %3 %4 %5")
                     .arg(mole.asymm.at(i).uf.m22,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m33,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m23,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m13,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m12,9,'f',5));
    } 
    cursor.deleteChar();
    cursor.insertText(neues);

  }//i
  cursor.endEditBlock ();

  grow();
}

void Window::complete(){

  chgl->murx=1;
  chgl->updateGL();
}

void Window::grow(){
  chgl->disSelection();
  chgl->mol->grow();
  infoKanalNews(chgl->mol->HumanSymmetry);
  chgl->murx=1;
   chgl->setMolecule(&mole);
  chgl->fuse->setVisible(true);
  chgl->grow->setVisible(false);
  chgl->updateGL();
}

void Window::expandAll(){
  chgl->disSelection();
  chgl->mol->expandAll();
  infoKanalNews(chgl->mol->HumanSymmetry);
  chgl->murx=1;
     chgl->setMolecule(&mole);
  chgl->fuse->setVisible(true);
  chgl->grow->setVisible(false);
  chgl->updateGL();
}

void Window::fillCell(){
  chgl->disSelection();
  chgl->mol->fillCell();
  infoKanalNews(chgl->mol->HumanSymmetry);
  chgl->murx=1;
     chgl->setMolecule(&mole);
  chgl->fuse->setVisible(true);
  chgl->grow->setVisible(false);
  chgl->updateGL();
}

void Window::inheritLabels(){
  QList <int> imapperA,imapperB, istfalsch,istsuper;
  {
  QAction *action = qobject_cast<QAction *>(sender());
  QVariantList index;
  if (action)
    index=action->data().toList();
  else return;
  if (index.size()<2) return;
  //qDebug()<<index;
  QList<double> spur;
  QList<double> srtspur,srtspur2;

  for (int i =0; i < mole.asymm.size(); i++){
    spur.append(0);
    srtspur.append(0);
    srtspur2.append(0);
  }
  int am=0;
  QList<SdmItem> asdm;
  SdmItem bsdm;
  bsdm.floorD=V3(0,0,0);
  bsdm.covalent=false;
  bsdm.sn=0;
  for (int i =0; i < mole.asymm.size(); i++){
    for (int j =0; j < mole.asymm.size(); j++){
      bsdm.a1=i;
      bsdm.a2=j;
      bsdm.d=sqrt(Distance(mole.asymm.at(j).pos,mole.asymm.at(i).pos));
      asdm.append(bsdm);
    }
  }
  QList<int> aii,bii;
  double m;
  //qDebug()<<"1"<<index;
  int aa[2]={0,0},minan=110;
  for (int i =0; i < index.size(); i++){
    int inta = index.at(i).toInt();

    if ((inta< mole.asymm.size())||(inta>=0)){
        int molA=mole.asymm.at(inta).molindex;

        for( int j = 0; j < asdm.size(); j++){
	if ((asdm.at(j).a1!=asdm.at(j).a2)
			&&(mole.asymm.at(asdm.at(j).a1).molindex==molA)
			&&(mole.asymm.at(asdm.at(j).a2).molindex==molA)){
	  spur[asdm.at(j).a1]+=m=
		  (asdm.at(j).d<1.7)?pow(asdm.at(j).d,2):(asdm.at(j).d<2.7)?pow(asdm.at(j).d,-1):0;
	  if (i==0)srtspur[asdm.at(j).a1]+=m;
	  else srtspur2[asdm.at(j).a1]+=m;
	  aa[i]++;
	  am++;
	  minan=qMin(minan,mole.asymm.at(asdm.at(j).a1).an);
    }
      }
    }
  }
  if (aa[0]!=aa[1]) {
    QMessageBox::information(0,"information",QString("The Number of atoms in the two molecules is not the same. This program can not help here, sorry! %1 %2").arg(aa[0]).arg(aa[1]));
    return;
  }
  if (minan<1){
    QMessageBox::information(0,"information","The molecules contain Hydrogen atoms or Q-Peaks. This program can not help here, sorry!");
    return;
  }
  am/=2;
  double madi[2]={-10000,-10000};
  int imadi[2]={0,0};
  qSort(srtspur.begin(),srtspur.end());
  qSort(srtspur2.begin(),srtspur2.end());
  double maxxx=0,xx;
  for (int i=1; i<srtspur.size()-1;i++){
    xx=qMin(fabs(srtspur.at(i)-srtspur.at(i-1)),fabs(srtspur.at(i)-srtspur.at(i+1)));
    if (xx>maxxx){
      madi[0]=srtspur.at(i);
      maxxx=xx;
    }
  }
  imadi[0]=spur.indexOf(madi[0]);
  maxxx=0;

  for (int i=1; i<srtspur2.size()-1;i++){
    xx=qMin(fabs(srtspur2.at(i)-srtspur2.at(i-1)),fabs(srtspur2.at(i)-srtspur2.at(i+1)));
    if ((fabs(srtspur2.at(i)-madi[0])<0.4)&&(xx>maxxx)){
      madi[1]=srtspur2.at(i);
      maxxx=xx;
    }
  }
  double mm=0,ma=0,mz=2;

  for( int j = 0; j < srtspur.size(); j++){
    imapperA.append(0);
    imapperB.append(0);

    if( srtspur.at(j)){
      imapperA[j]=spur.indexOf(srtspur.at(j));
      imapperB[j]=spur.indexOf(srtspur2.at(j));
      ma=Distance(mole.asymm.at(imapperA.at(j)).pos,mole.asymm.at(imadi[0]).pos);
      mm=fabs(Distance(mole.asymm.at(imapperA.at(j)).pos,mole.asymm.at(imadi[0]).pos)-
		      Distance(mole.asymm.at(imapperB.at(j)).pos,mole.asymm.at(imadi[1]).pos));
      if (mm/ma>0.04) istfalsch.append(j);
      if ((mm)&&(mm/ma<0.002)&&((mole.asymm.at(imapperA.at(j)).an*mole.asymm.at(imapperB.at(j)).an))>0)
	istsuper.append(j);
    }
  }
  for (int i=0; i<istfalsch.size(); i++){
    for (int j=i+1; j<istfalsch.size(); j++){
      if (istfalsch.at(i)==istfalsch.at(j)) continue;
      mz=fabs(srtspur.at(istfalsch.at(i))-srtspur2.at(istfalsch.at(j)));
      mm=fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(i))).pos,mole.asymm.at(imadi[0]).pos)-
		      Distance(mole.asymm.at(imapperB.at(istfalsch.at(i))).pos,mole.asymm.at(imadi[1]).pos))
	      +fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(j))).pos,mole.asymm.at(imadi[0]).pos)-
			      Distance(mole.asymm.at(imapperB.at(istfalsch.at(j))).pos,mole.asymm.at(imadi[1]).pos));
      ma=fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(i))).pos,mole.asymm.at(imadi[0]).pos)-
		      Distance(mole.asymm.at(imapperB.at(istfalsch.at(j))).pos,mole.asymm.at(imadi[1]).pos))
	      +fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(j))).pos,mole.asymm.at(imadi[0]).pos)-
			      Distance(mole.asymm.at(imapperB.at(istfalsch.at(i))).pos,mole.asymm.at(imadi[1]).pos));
      if ((mz<0.5)&&(ma/mm<0.25)&&(ma<mm)) {
	imapperB.swap(istfalsch.at(i),istfalsch.at(j)); 
      }
    }
  }
  istfalsch.clear();
  if (istsuper.isEmpty()) {
      istsuper.append(0);
      imapperA[istsuper.at(0)]=imadi[0];
      imapperB[istsuper.at(0)]=imadi[1];
  }
  for( int j = 0; j < srtspur.size(); j++){
    if( srtspur.at(j)){
      ma=Distance(mole.asymm.at(imapperA.at(j)).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos);
      mm=fabs(Distance(mole.asymm.at(imapperA.at(j)).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos)-
		      Distance(mole.asymm.at(imapperB.at(j)).pos,mole.asymm.at(imapperB.at(istsuper.at(0))).pos));
      if (mm/ma>0.04) istfalsch.append(j);
    }
  }

  for (int i=0; i<istfalsch.size(); i++){
    for (int j=i+1; j<istfalsch.size(); j++){
      if (istfalsch.at(i)==istfalsch.at(j)) continue;
      mz=fabs(srtspur.at(istfalsch.at(i))-srtspur2.at(istfalsch.at(j)));
      mm=fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(i))).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos)-
		      Distance(mole.asymm.at(imapperB.at(istfalsch.at(i))).pos,mole.asymm.at(imapperB.at(istsuper.at(0))).pos))
	      +fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(j))).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos)-
			      Distance(mole.asymm.at(imapperB.at(istfalsch.at(j))).pos,mole.asymm.at(imapperB.at(istsuper.at(0))).pos));
      ma=fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(i))).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos)-
		      Distance(mole.asymm.at(imapperB.at(istfalsch.at(j))).pos,mole.asymm.at(imapperB.at(istsuper.at(0))).pos))
	      +fabs(Distance(mole.asymm.at(imapperA.at(istfalsch.at(j))).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos)-
			      Distance(mole.asymm.at(imapperB.at(istfalsch.at(i))).pos,mole.asymm.at(imapperB.at(istsuper.at(0))).pos));

      if ((mz<0.5)&&(ma<mm)&&(ma/mm<0.25)){
        imapperB.swap(istfalsch.at(i),istfalsch.at(j));
      }
    }
  }
  istfalsch.clear();
  for( int j = 0; j < srtspur.size(); j++){
    if( srtspur.at(j)){
      ma=Distance(mole.asymm.at(imapperA.at(j)).pos,mole.asymm.at(imapperA.at(istsuper.at(0))).pos);
      if (mm/ma>0.05) istfalsch.append(j);
    }
  }
  int oje=srtspur.size();
  for( int j = 0; j < srtspur.size(); ){
    if( srtspur.at(j)==0){
      imapperA.removeAt(j);
      imapperB.removeAt(j);
      srtspur.removeAt(j);
    }else{j++;}

  }
  oje-=srtspur.size();
  chgl->disSelection();
  chgl->updateGL();
  }

  DragDropAtomsDialog  *ddad = new DragDropAtomsDialog(mole, imapperA, &imapperB,this);
  connect(ddad,SIGNAL(pairs(const QString&)),chgl,SLOT(selectPair(const QString&)));
  ddad->resiFinder=resiFinder;

  if (ddad->exec()==QDialog::Accepted){

    QProgressDialog progress("Inherit labels", QString(), 0, imapperB.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    resiNrSpin->setValue(ddad->resiNrSpin->value());
    resiResiClass->setText(ddad->resiResiClass->text());

    for (int i=0;i<imapperB.size();i++){
      progress.setValue(i);
      update();
      QRegExp num=QRegExp("\\d+");
      if (mole.asymm.at(imapperA.at(i)).Label.contains(num)) {
	int p=mole.asymm.at(imapperA.at(i)).an;
	labelPSE=mole.pse(p);

	labelIndex=mole.asymm.at(imapperA.at(i)).Label.section(QRegExp("\\D+"),0,0,QString::SectionSkipEmpty).toInt();
	labelSuffix=mole.asymm.at(imapperA.at(i)).Label.section(num,1,1);
	labelSuffix.remove(QRegExp("[^A-Za-z']+"));
      }
      else{
	int p=mole.asymm.at(imapperA.at(i)).an;
	labelPSE=mole.pse(p);
	labelIndex=-1;
	labelSuffix=mole.asymm.at(imapperA.at(i)).Label;
	labelSuffix.remove(0,labelPSE.size());
	labelSuffix.remove(QRegExp("[^A-Za-z']+"));
      }
      updateLabel();
      renameThisAtom(imapperB.at(i));
    }
    progress.setValue(imapperB.size());
  }
  delete ddad;
  }

void Window::sina(){//anis rueckrwaerts
  if (mole.asymm.isEmpty()) return;
  if (chgl->mol->selectedatoms.isEmpty()) {
   QAction *action = qobject_cast<QAction *>(sender());
   int index=-1;
   if (action)
       index=action->data().toInt();
     else index=-1;
   if (index>mole.asymm.size()) index=-1;
   if (index!=-1) {
         chgl->mol->selectedatoms.clear();
         chgl->mol->selectedatoms.append(chgl->mol->showatoms[index]);
         chgl->mol->selectedatoms.last().style=index;
   }
  }
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  if (chgl->mol->selectedatoms.isEmpty()){
    double uEQ;
    cursor.beginEditBlock();
    for (int i=0; i<mole.asymm.size(); i++){    
      if (mole.asymm.at(i).uf.m22!=-666.0){
	uEQ=0.05;//ueq(mole.asymm.at(i).uf);
	mole.asymm[i].uf.m11=
	mole.asymm[i].uc.m11=
	mole.asymm[i].uc.m22=
	mole.asymm[i].uc.m33=uEQ;
	mole.asymm[i].uf.m22=-666.0;
	mole.asymm[i].uf.m33=uEQ;
	mole.asymm[i].uf.m12=
	mole.asymm[i].uf.m21=
	mole.asymm[i].uf.m13=
	mole.asymm[i].uf.m31=
	mole.asymm[i].uf.m23=
	mole.asymm[i].uf.m32=
	mole.asymm[i].uc.m21=
	mole.asymm[i].uc.m12=
	mole.asymm[i].uc.m13=
	mole.asymm[i].uc.m31=
	mole.asymm[i].uc.m32=
	mole.asymm[i].uc.m23=0.0;
	QString  label = mole.asymm.at(i).orginalLine.left(80).trimmed();
	cursor.movePosition(QTextCursor::Start);
	cursor = document->find(label,cursor);
	if (cursor.isNull()){
	  cursor = editor->textCursor();
	  cursor.movePosition(QTextCursor::Start);
	  label.chop(10);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()) {
	    cursor = editor->textCursor();
	    printf("hab was nicht %d %s\n",i,label.toStdString().c_str());
	    continue;
	  }
	}
	cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	while (cursor.selectedText().endsWith("=")) {
	  cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	}
	cursor.deleteChar();
	mole.asymm[i].orginalLine=
		QString("%1  %2  %3 %4 %5 %6 %7")
		.arg(mole.asymm.at(i).Label.section('_',0,0),-4)
                .arg(qMax(1,sfac.indexOf(mole.asymm.at(i).an)+1))
		.arg(mole.asymm.at(i).frac.x,9,'f',5)
		.arg(mole.asymm.at(i).frac.y,9,'f',5)
		.arg(mole.asymm.at(i).frac.z,9,'f',5)
		.arg(mole.asymm.at(i).sof_org,9,'f',5)
		.arg(mole.asymm.at(i).uf.m11,9,'f',5);
	cursor.insertText(mole.asymm[i].orginalLine);
	QString s =
		QString("<font color=blue>Isotropyfied atom: %1 part:%2 residue:%3 %4 </font> Please save to update structure view.")
		.arg(mole.asymm.at(i).Label)
		.arg(mole.asymm.at(i).part)
		.arg(mole.asymm.at(i).resiNr)
		.arg(mole.asymm.at(i).ResiClass);
	infoKanalNews(s);
      }
    }
    cursor.endEditBlock();
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);
    mole.showbonds =
	    mole.connecting(mole.showatoms);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();

    chgl->pause=false;
    chgl->updateBondActions();
    chgl->updateGL();
  }
  else
  {
    double uEQ;
    cursor.beginEditBlock();
    for (int j=0; j<mole.selectedatoms.size(); j++){    
      int i=mole.selectedatoms.at(j).style;
      if ((i>=mole.asymm.size())||(i<0)) continue;
      if (mole.asymm.at(i).uf.m22!=-666.0){
	uEQ=0.05;//ueq(mole.asymm.at(i).uf);
	mole.asymm[i].uf.m11=
	mole.asymm[i].uc.m11=
	mole.asymm[i].uc.m22=
	mole.asymm[i].uc.m33=uEQ;
	mole.asymm[i].uf.m22=-666.0;
	mole.asymm[i].uf.m33=uEQ;
	mole.asymm[i].uf.m12=
	mole.asymm[i].uf.m21=
	mole.asymm[i].uf.m13=
	mole.asymm[i].uf.m31=
	mole.asymm[i].uf.m23=
	mole.asymm[i].uf.m32=
	mole.asymm[i].uc.m21=
	mole.asymm[i].uc.m12=
	mole.asymm[i].uc.m13=
	mole.asymm[i].uc.m31=
	mole.asymm[i].uc.m32=
	mole.asymm[i].uc.m23=0.0;
	QString  label = mole.asymm.at(i).orginalLine.left(80).trimmed();
	cursor.movePosition(QTextCursor::Start);
	cursor = document->find(label,cursor);
	if (cursor.isNull()){
	  cursor = editor->textCursor();
	  cursor.movePosition(QTextCursor::Start);
	  label.chop(10);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()) {
	    cursor = editor->textCursor();
	    printf("hab was nicht %d %s\n",i,label.toStdString().c_str());
	    continue;
	  }
	}
	cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	while (cursor.selectedText().endsWith("=")) {
	  cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	}
	cursor.deleteChar();
	mole.asymm[i].orginalLine=
		QString("%1  %2  %3 %4 %5 %6 %7")
		.arg(mole.asymm.at(i).Label.section('_',0,0),-4)
                .arg(qMax(1,sfac.indexOf(mole.asymm.at(i).an)+1))
		.arg(mole.asymm.at(i).frac.x,9,'f',5)
		.arg(mole.asymm.at(i).frac.y,9,'f',5)
		.arg(mole.asymm.at(i).frac.z,9,'f',5)
		.arg(mole.asymm.at(i).sof_org,9,'f',5)
		.arg(mole.asymm.at(i).uf.m11,9,'f',5);
	cursor.insertText(mole.asymm[i].orginalLine);
	QString s =
		QString("<font color=blue>Isotropyfied atom: %1 part:%2 residue:%3 %4 </font> Please save to update structure view.")
		.arg(mole.asymm.at(i).Label)
		.arg(mole.asymm.at(i).part)
		.arg(mole.asymm.at(i).resiNr)
		.arg(mole.asymm.at(i).ResiClass);
	infoKanalNews(s);
      }
    }
    cursor.endEditBlock();
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++)
      mole.showatoms.append(mole.asymm[o]);
    mole.sdmcompleter();
    mole.showbonds =
	    mole.connecting(mole.showatoms);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    bool growYes=chgl->fuse->isVisible ();
    chgl->mol->fuse();
    if (growYes) chgl->mol->grow();

    chgl->pause=false;
    chgl->updateBondActions();
    chgl->updateGL();
  
  }
}

void Window::autoHFix(){
  if (mole.asymm.isEmpty()) return;
  int warmal=mole.asymm.size();
  int warmalAtome=0;
  for (int i=0; i<mole.asymm.size();i++) if (mole.asymm.at(i).an>=0)warmalAtome++;
  QStringList alab;
  for (int i =0; i<mole.asymm.size(); i++){
      alab.append(mole.asymm.at(i).Label);
  }
  QString nn;
  chgl->disSelection();
  chgl->mol->grow();

  MyAtom htest,qtest;

  double xh[8]={
    0.98,//1
    0.97,//2
    0.96,//3
    0.93,//4
    1.10,//15 B
    0.82,//8 O
    0.93,//9
    0.93// 16
  };

  if (sfac.indexOf(0)==-1)addNewScatteringFactor(0);
  htest.an=0;
  htest.part=0;
  htest.resiNr=0;
  htest.hidden=0;
  htest.symmGroup=0;
  htest.uc.m12=htest.uc.m23=htest.uc.m13=0.0;
  htest.uc.m21=htest.uc.m32=htest.uc.m31=0.0;
  htest.uf.m12=htest.uf.m23=htest.uf.m13=0.0;
  htest.uf.m21=htest.uf.m32=htest.uf.m31=0.0;
  qtest=htest;
  qtest.an=-1;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();
  QMap<int,int> keinDonor;
  QMap<int,int> istDonor;
  for (int j=0;j<mole.contact.size();j++){
    double ch1,ch2;
    if (mole.knoepfe.at(mole.contact.at(j).a1).neighbors.size()==0) keinDonor[mole.contact.at(j).a1]++;
    if (mole.knoepfe.at(mole.contact.at(j).a2).neighbors.size()==0) keinDonor[mole.contact.at(j).a2]++;
    if ((mole.asymm.at(mole.contact.at(j).a1).an==7)&&(mole.knoepfe.at(mole.contact.at(j).a1).neighbors.size()>1)) keinDonor[mole.contact.at(j).a1]++;
    if ((mole.asymm.at(mole.contact.at(j).a2).an==7)&&(mole.knoepfe.at(mole.contact.at(j).a2).neighbors.size()>1)) keinDonor[mole.contact.at(j).a2]++;
    if ((mole.contact.at(j).covalent)){//
      ch1=ch2=0;

      for (int k=0; k<mole.knoepfe.at(mole.contact.at(j).a1).neighbors.size(); k++){
	ch1 += sqrt(Distance(mole.asymm.at(mole.contact.at(j).a1).pos,
				mole.showatoms.at(mole.knoepfe.at(mole.contact.at(j).a1).neighbors.at(k)).pos))-
		((mole.Kovalenz_Radien[mole.asymm.at(mole.contact.at(j).a1).an]+
		  mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(mole.contact.at(j).a1).neighbors.at(k)).an])*0.01);

      }
      if (mole.knoepfe.at(mole.contact.at(j).a1).neighbors.size()) 
	ch1*=1.0/mole.knoepfe.at(mole.contact.at(j).a1).neighbors.size();

      for (int k=0; k<mole.knoepfe.at(mole.contact.at(j).a2).neighbors.size(); k++){
	ch2 += sqrt(Distance(mole.asymm.at(mole.contact.at(j).a2).pos,
				mole.showatoms.at(mole.knoepfe.at(mole.contact.at(j).a2).neighbors.at(k)).pos))-
		((mole.Kovalenz_Radien[mole.asymm.at(mole.contact.at(j).a2).an]+
		  mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(mole.contact.at(j).a2).neighbors.at(k)).an])*0.01);
      }
      if (mole.knoepfe.at(mole.contact.at(j).a2).neighbors.size()) 
	ch2*=1.0/mole.knoepfe.at(mole.contact.at(j).a2).neighbors.size();
      if (ch1<-0.2 ) keinDonor[mole.contact.at(j).a1]++;
      if (ch2<-0.2 ) keinDonor[mole.contact.at(j).a2]++;
      if ((ch1-0.01)>ch2) istDonor[mole.contact.at(j).a1]++;
      if ((ch2-0.01)>ch1) istDonor[mole.contact.at(j).a2]++;
    }
  }
  for (int i =0; i<mole.asymm.size(); i++){
    if ((mole.asymm.at(i).an==5)||(mole.asymm.at(i).an==6)){
      double nh= (mole.asymm.at(i).an==6) ? 0.07 : 0;


      double plan=1.0;
      if (mole.knoepfe.at(i).neighbors.size()==1) {
	if ((mole.asymm.at(i).an==6)&&(!istDonor.contains(i))) continue; 
	V3 v=V3(0,0,0),w=V3(0,0,0),u=V3(0,0,0) ,a=V3(0,0,0);//,b=V3(0,0,0),c=V3(0,0,0);
	a = mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos - mole.asymm.at(i).pos;
	//printf("A = %s-%s %g %g %g\n",mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).Label.toStdString().c_str(),mole.asymm.at(i).Label.toStdString().c_str(),a.x,a.y,a.z);
	double chi = sqrt(Norm(a))-((mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).an]+mole.Kovalenz_Radien[mole.asymm.at(i).an])/100.0);
	if (chi>-0.09) {//afix 137 127
	  int j,k,l;
	  a=Normalize(a);
	  double jx,kx,lx,mx,my,mz;
	  double val[24],merg[8],wink=0,t=0,mt=0;
	  for (int z=0; z<8;z++) merg[z]=0;
	  if (fxle->n5){
	    for (int z=0; z<24; z++){
	      if ((a==V3(0,0,-1))||(a==V3(0,0,1))) w=Normalize(V3(1,1,1)%a); 
	      else w=Normalize(V3(0,0,1)%a);
              //printf("w= %g %g %g\n",w.x,w.y,w.z);
	      u=Normalize(w%a);
	      v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	      qtest.pos=v;
	      mole.kart2frac(v,v);
	      int zplu=0;
	      do {
		jx=((zplu+v.x)*fxle->n1+0.5);
		zplu++;
	      }while (jx<0);
	      zplu=0;
	      do {
		kx=((zplu+v.y)*fxle->n2+0.5);
		zplu++;
	      }while (kx<0);
	      zplu=0;
              do {
		lx=((zplu+v.z)*fxle->n3+0.5);
		zplu++;
	      }while (lx<0);
	      mx=jx-floor(jx);
	      my=kx-floor(kx);
	      mz=lx-floor(lx);
	      j=((int)jx)%fxle->n1;//
	      k=((int)kx)%fxle->n2;//
	      l=((int)lx)%fxle->n3;//
	      merg[z%8]+=val[z]=(
			      (1-mx)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      mx*fxle->datfo_fc[((j+1)+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      (1-my)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      my*fxle->datfo_fc[(j+fxle->n1*((k+1)+l*fxle->n2))%fxle->n5]+
			      (1-mz)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      mz*fxle->datfo_fc[(j+fxle->n1*(k+(l+1)*fxle->n2))%fxle->n5])/3.0;
	      qtest.peakHeight=val[z];
	      qtest.frac=v;
	      qtest.Label=QString("Q%1").arg(z,-3);
	      mole.pmin=qMin(mole.pmin, qtest.peakHeight);
	      mole.pmax=qMax(mole.pmax, qtest.peakHeight);
          qpv->cutoff=0;//mole.pmin;
	      qpv->setMin(mole.pmin);
	      qpv->setMax(mole.pmax);
	      mole.asymm.append(qtest);
	      wink+=0.261799387799;
	    }
	    for (int z=0; z<8;z++) {
	      t =merg[(z+8)%8] ;
	      double x = t - merg[(z+9)%8];
	      double u = t - merg[(z+7)%8];
	      if (fmin(x,u)<0) continue;
	      double p=0.5*(u-x)/(u+x);
	      t=t+0.25*(p*(u-x));
	      mt=(mt>t)?mt:t;
	      if (t<mt)continue;
	      wink=((double) z + p )*0.261799387799; 
	    }
	  }else{
	    int zzz=0;
	    int z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
	    while ((z==i)&&(zzz<mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.size()))
	      z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
	    if ((a==V3(0,0,-1))||(a==V3(0,0,1))) w=Normalize(V3(1,1,1)%a); 
	    else w=Normalize(V3(0,0,1)%a);
	    u=Normalize(w%a);
	    v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	    wink=mole.dieder(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos-mole.asymm.at(z).pos,
			    mole.asymm.at(i).pos-mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos,
			    v-mole.asymm.at(i).pos)/180.0*M_PI;
        }
	  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	  htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	  htest.part=mole.asymm.at(i).part;
	  htest.resiNr=mole.asymm.at(i).resiNr;
	  htest.Label.replace(0,1,'H');
	  int hindex=1;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	  htest.uf.m11=mole.asymm.at(i).uf.m11*1.5;
	  htest.uf.m22=-666.0;
	  htest.uf.m33=-1.5;
	  htest.pos=v;
	  mole.kart2frac(htest.pos,htest.frac);	
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.5");
	  mole.asymm.append(htest);
	  wink+=2.09439510239;
	  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	  htest.Label.chop(1);
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.pos=v;
	  mole.kart2frac(htest.pos,htest.frac);	
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.5");
	  mole.asymm.append(htest);
	  wink+=2.09439510239;
	  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	  htest.Label.chop(1);
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.pos=v;
	  mole.kart2frac(htest.pos,htest.frac);	
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.5");
	  mole.asymm.append(htest);
	  QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	  int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	  cursor.movePosition(QTextCursor::Start);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()){
	    cursor = editor->textCursor();
	    cursor.movePosition(QTextCursor::Start);
	    label.chop(10);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()) {
	      cursor = editor->textCursor();
	    }
	  }
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  while (cursor.selectedText().endsWith("=")) {
	    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  }
	  cursor.clearSelection();
	  cursor.movePosition(QTextCursor::Down);
	  cursor.insertText(QString("AFIX 137\n%1\n%2\n%3\nAFIX %4\n")
			  .arg(mole.asymm.at(mole.asymm.size()-3).orginalLine)
			  .arg(mole.asymm.at(mole.asymm.size()-2).orginalLine)
			  .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			  .arg(afixfuenf)
			  );
	}else if (chi> -0.27){//afix 93
	  int j,k,l;
	  a=Normalize(a);
	  double jx,kx,lx,mx,my,mz;
	  double val[24],merg[12],wink=0,t=0,mt=0;
	  if (fxle->n5){
	    for (int z=0; z<12;z++) merg[z]=0;
	    for (int z=0; z<24; z++){
	      wink=z*0.261799387799;
	      w=Normalize(V3(0,0,1)%a);
	      u=Normalize(w%a);
	      v=(-(xh[6]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[6]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	      qtest.pos=v;
	      mole.kart2frac(v,v);
	      jx=(v.x>0)?(v.x*fxle->n1+0.5):((1.0+v.x)*fxle->n1+0.5);    
	      kx=(v.y>0)?(v.y*fxle->n2+0.5):((1.0+v.y)*fxle->n2+0.5);   
	      lx=(v.z>0)?(v.z*fxle->n3+0.5):((1.0+v.z)*fxle->n3+0.5);   
	      mx=jx-floor(jx);
	      my=kx-floor(kx);
	      mz=lx-floor(lx);
	      j=((int)jx)%fxle->n1;//
	      k=((int)kx)%fxle->n2;//
	      l=((int)lx)%fxle->n3;//
	      merg[z%12]+=val[z]=(
			      (1-mx)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      mx*fxle->datfo_fc[((j+1)+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      (1-my)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      my*fxle->datfo_fc[(j+fxle->n1*((k+1)+l*fxle->n2))%fxle->n5]+
			      (1-mz)*fxle->datfo_fc[(j+fxle->n1*(k+l*fxle->n2))%fxle->n5]+
			      mz*fxle->datfo_fc[(j+fxle->n1*(k+(l+1)*fxle->n2))%fxle->n5])/3.0;
	      qtest.peakHeight=val[z];
	      qtest.frac=v;
	      qtest.Label=QString("Q%1").arg(z+100);
	      mole.pmin=qMin(mole.pmin, qtest.peakHeight);
	      mole.pmax=qMax(mole.pmax, qtest.peakHeight);
          qpv->cutoff=0;//mole.pmin;
	      qpv->setMin(mole.pmin);
	      qpv->setMax(mole.pmax);
	      mole.asymm.append(qtest);
	    }
	    for (int z=0; z<12;z++) {
	      t =merg[(z+12)%12] ;
	      double x = t - merg[(z+13)%12];
	      double u = t - merg[(z+11)%12];
	      if (fmin(x,u)<0) continue;
	      double p=0.5*(u-x)/(u+x);
	      t=t+0.25*(p*(u-x));
	      mt=(mt>t)?mt:t;
	      if (t<mt)continue;
	      wink=((double) z + p )*0.261799387799; 

	      /////////////////
	    }
	  }else{
	    int zzz=0;
	    int z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
	    while ((z==i)&&(zzz<mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.size()))
	      z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
	    w=Normalize(V3(0,0,1)%a);
	    u=Normalize(w%a);
	    v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
	    wink=mole.dieder(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos-mole.asymm.at(z).pos,
			    mole.asymm.at(i).pos-mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos,
			    v-mole.asymm.at(i).pos)/180.0*M_PI;
	  }

	  v=(-(xh[6]-nh)*0.5*a)+ mole.asymm.at(i).pos+(xh[6]-nh)*0.866025403784*(sin(wink)*u+cos(wink)*w);
	  htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	  htest.part=mole.asymm.at(i).part;
	  htest.resiNr=mole.asymm.at(i).resiNr;
	  htest.Label.replace(0,1,'H');
	  int hindex=1;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	  htest.uf.m11=mole.asymm.at(i).uf.m11*1.2;
	  htest.uf.m22=-666.0;
	  htest.uf.m33=-1.2;
	  htest.pos=v;
	  mole.kart2frac(htest.pos,htest.frac);	
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.2");
	  mole.asymm.append(htest);
	  wink+=3.14159265359;
	  v=(-(xh[6]-nh)*0.5*a)+ mole.asymm.at(i).pos+(xh[6]-nh)*0.866025403784*(sin(wink)*u+cos(wink)*w);
	  htest.Label.chop(1);    
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.pos=v;
	  mole.kart2frac(htest.pos,htest.frac);	
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.2");
	  mole.asymm.append(htest);

	  QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	  int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	  cursor.movePosition(QTextCursor::Start);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()){
	    cursor = editor->textCursor();
	    cursor.movePosition(QTextCursor::Start);
	    label.chop(10);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()) {
	      cursor = editor->textCursor();
	    }
	  }
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  while (cursor.selectedText().endsWith("=")) {
	    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  }
	  cursor.clearSelection();
	  cursor.movePosition(QTextCursor::Down);
	  cursor.insertText(QString("AFIX 93\n%1\n%2\nAFIX %3\n")
			  .arg(mole.asymm.at(mole.asymm.size()-2).orginalLine)
			  .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			  .arg(afixfuenf)
			  );
	}else{//afix 163
	  if (mole.asymm.at(i).an==5){
	    a = Normalize(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos - mole.asymm.at(i).pos);
	    v=(-(xh[7]-nh)*a)+ mole.asymm.at(i).pos;
	    htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	    htest.part=mole.asymm.at(i).part;
	    htest.resiNr=mole.asymm.at(i).resiNr;
	    htest.Label.replace(0,1,'H');
	  int hindex=0;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;

	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	    htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	    htest.uf.m11=mole.asymm.at(i).uf.m11*1.2;
	    htest.uf.m22=-666.0;
	    htest.uf.m33=-1.2;
	    htest.pos=v;
	    mole.kart2frac(htest.pos,htest.frac);
	    htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		    .arg(htest.Label)
		    .arg(qMax(1,sfac.indexOf(0)+1))
		    .arg(htest.frac.x,9,'f',5)
		    .arg(htest.frac.y,9,'f',5)
		    .arg(htest.frac.z,9,'f',5)
		    .arg(mole.asymm.at(i).sof_org)
		    .arg("-1.2");
	    mole.asymm.append(htest);

	    QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	    int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	    cursor.movePosition(QTextCursor::Start);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()){
	      cursor = editor->textCursor();
	      cursor.movePosition(QTextCursor::Start);
	      label.chop(10);
	      cursor = document->find(label,cursor);
	      if (cursor.isNull()) {
		cursor = editor->textCursor();
	      }
	    }
	    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    while (cursor.selectedText().endsWith("=")) {
	      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    }
	    cursor.clearSelection();
	    cursor.movePosition(QTextCursor::Down);
	    cursor.insertText(QString("AFIX 163\n%1\nAFIX %2\n")
			    .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			    .arg(afixfuenf)
			    );
	  }
	}
      }
      if (mole.knoepfe.at(i).neighbors.size()==2){//afix 43 oder 23
	if ((mole.asymm.at(i).an==6)&&(!istDonor.contains(i))) continue; 
	double angle=0;
	V3 v=V3(0,0,0),w=V3(0,0,0),u=V3(0,0,0) ,a=V3(0,0,0),b=V3(0,0,0),c=V3(0,0,0);
	a = mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)      ).pos - mole.asymm.at(i).pos;
	b = mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(1)      ).pos - mole.asymm.at(i).pos;
	double chi1 = sqrt(Norm(a))-
		((mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).an]+
		  mole.Kovalenz_Radien[mole.asymm.at(i).an])/100.0);
	double chi2 = sqrt(Norm(b))-
		((mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(1)).an]+
		  mole.Kovalenz_Radien[mole.asymm.at(i).an])/100.0);
	a = c = Normalize(a) ;
	c = c+ (b= Normalize(b));
	angle=mole.winkel(a,b);
	c = Normalize(c);
	double andi=(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(1)).an+mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).an)/2.0-5.0;
	andi= (andi)?0.8:1.0;
        if (angle<160) {
	  if ((andi*(chi1+chi2))<-0.13) {//43
	    c = (-(xh[3]-nh) * c)+mole.asymm.at(i).pos;    
	    htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	    htest.part=mole.asymm.at(i).part;
	    htest.resiNr=mole.asymm.at(i).resiNr;
	    htest.Label.replace(0,1,'H');
	  int hindex=0;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;

	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	    htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	    htest.uf.m11=mole.asymm.at(i).uf.m11*1.2;
	    htest.uf.m22=-666.0;
	    htest.uf.m33=-2.2;
	    htest.pos=c;
	    mole.kart2frac(htest.pos,htest.frac);	
	    htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		    .arg(htest.Label)
		    .arg(qMax(1,sfac.indexOf(0)+1))
		    .arg(htest.frac.x,9,'f',5)
		    .arg(htest.frac.y,9,'f',5)
		    .arg(htest.frac.z,9,'f',5)
		    .arg(mole.asymm.at(i).sof_org)
		    .arg("-1.2");
	    mole.asymm.append(htest);
	    QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	    int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	    cursor.movePosition(QTextCursor::Start);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()){
	      cursor = editor->textCursor();
	      cursor.movePosition(QTextCursor::Start);
	      label.chop(10);
	      cursor = document->find(label,cursor);
	      if (cursor.isNull()) {
		cursor = editor->textCursor();
	      }
	    }
	    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    while (cursor.selectedText().endsWith("=")) {
	      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    }
	    cursor.clearSelection();
	    cursor.movePosition(QTextCursor::Down);
	    cursor.insertText(QString("AFIX 43\n%1\nAFIX %2\n")
			    .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			    .arg(afixfuenf));
	  }
	  else{//23
	    w = Normalize(a);
	    u = Normalize(b);
	    double si=1.0376-0.0346*Norm(w-u);    //=1.91063323625-((acos(u*w)-1.91063323625)/5.0);//-1.91063323625 = 109.47 grad ; 3.7 empirischer Wert
	    c = (-cos(si)*(xh[1]-nh) * c);
	    v = sin(si)  *(xh[1]-nh) * Normalize(c%a);
	    a = mole.asymm.at(i).pos + c + v;
	    b = mole.asymm.at(i).pos + c - v;
	    c = c + mole.asymm.at(i).pos;

	    htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	    htest.part=mole.asymm.at(i).part;
	    htest.resiNr=mole.asymm.at(i).resiNr;
	    htest.Label.replace(0,1,'H');
	    int hindex=1;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	    htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	    htest.uf.m11=mole.asymm.at(i).uf.m11*1.2;
	    htest.uf.m22=-666.0;
	    htest.uf.m33=-1.2;
	    htest.pos=a;
	    mole.kart2frac(htest.pos,htest.frac);	
	    htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		    .arg(htest.Label)
		    .arg(qMax(1,sfac.indexOf(0)+1))
		    .arg(htest.frac.x,9,'f',5)
		    .arg(htest.frac.y,9,'f',5)
		    .arg(htest.frac.z,9,'f',5)
		    .arg(mole.asymm.at(i).sof_org)
		    .arg("-1.2");
	    mole.asymm.append(htest);
	    htest.Label.chop(1);    
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;

	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	    htest.pos=b;
	    mole.kart2frac(htest.pos,htest.frac);	
	    htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		    .arg(htest.Label)
		    .arg(qMax(1,sfac.indexOf(0)+1))
		    .arg(htest.frac.x,9,'f',5)
		    .arg(htest.frac.y,9,'f',5)
		    .arg(htest.frac.z,9,'f',5)
		    .arg(mole.asymm.at(i).sof_org)
		    .arg("-1.2");
	    mole.asymm.append(htest);
	    QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	    int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	    cursor.movePosition(QTextCursor::Start);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()){
	      cursor = editor->textCursor();
	      cursor.movePosition(QTextCursor::Start);
	      label.chop(10);
	      cursor = document->find(label,cursor);
	      if (cursor.isNull()) {
		cursor = editor->textCursor();
	      }
	    }
	    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    while (cursor.selectedText().endsWith("=")) {
	      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	    }
	    cursor.clearSelection();
	    cursor.movePosition(QTextCursor::Down);
	    cursor.insertText(QString("AFIX 23\n%1\n%2\nAFIX %3\n")
			    .arg(mole.asymm.at(mole.asymm.size()-2).orginalLine)
			    .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			    .arg(afixfuenf));

	  }
        }
      }

      if (mole.knoepfe.at(i).neighbors.size()==3) {
	if ((mole.asymm.at(i).an==6)&&(!istDonor.contains(i))) continue; 
	V3 v,w=V3(0,0,0),u ,a,b,c=V3(0,0,0);
	double ch1=0;
	for (int k=0; k<3; k++){
	  a = Normalize(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(k)      ).pos - mole.asymm.at(i).pos);
	  b = Normalize(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at((k+1)%3)).pos - mole.asymm.at(i).pos);
	  c = c + a;
	  v = Normalize(a%b);
	  if (k) {plan*=fabs(w*v);
	  }
	  w = v;
	  ch1 += sqrt(Distance(mole.asymm.at(i).pos,
				  mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(k)).pos))-
		  ((mole.Kovalenz_Radien[mole.asymm.at(i).an]+
		    mole.Kovalenz_Radien[mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(k)).an])*0.01);

	}
	c = Normalize(c);
	ch1*=1.0/3;
	if ((plan<0.8)&&(ch1>-0.09)){
	  c *=-(xh[0]-nh);
	  htest.Label=mole.asymm.at(i).Label.section('_',0,0);
	  htest.part=mole.asymm.at(i).part;
	  htest.resiNr=mole.asymm.at(i).resiNr;
	  htest.Label.replace(0,1,'H');
	  int hindex=0;
	  do {
	  if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
	   nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
	  hindex++;
	  if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive)))htest.Label.chop(1);
	  }while(alab.contains(nn,Qt::CaseInsensitive));
	  alab.append(nn);
	  htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
	  htest.uf.m11=mole.asymm.at(i).uf.m11*1.2;
	  htest.uf.m22=-666.0;
	  htest.uf.m33=-1.2;
	  htest.pos = mole.asymm.at(i).pos + c;
	  mole.kart2frac(htest.pos,htest.frac);
	  QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
	  int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
	  cursor.movePosition(QTextCursor::Start);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()){
	    cursor = editor->textCursor();
	    cursor.movePosition(QTextCursor::Start);
	    label.chop(10);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()) {
	      cursor = editor->textCursor();
	    }
	  }
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  while (cursor.selectedText().endsWith("=")) {
	    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  }
	  cursor.clearSelection();
	  cursor.movePosition(QTextCursor::Down);
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(i).sof_org)
		  .arg("-1.2");
	  cursor.insertText(QString("AFIX 13\n%1\nAFIX %2\n").arg(htest.orginalLine).arg(afixfuenf));
	  mole.asymm.append(htest);
	}
      }
    }
  }
  for (int j=0;j<mole.contact.size();j++){
    if ((istDonor.contains(mole.contact.at(j).a1)||(!keinDonor.contains(mole.contact.at(j).a1)))
		    &&(mole.asymm.at(mole.contact.at(j).a1).an==7)) {
      if (keinDonor.contains(mole.contact.at(j).a1)) continue;
      if (fxle->n5) {
	htest = findOH(mole.cell.symmops.at(mole.contact.at(j).sn) * mole.asymm.at(mole.contact.at(j).a1).frac
			+ mole.cell.trans.at(mole.contact.at(j).sn) - mole.contact.at(j).floorD,
			mole.asymm.at(mole.contact.at(j).a2).frac,
			mole.contact.at(j).a1,alab);

	if (Distance(htest.pos,V3(0,0,0))<0.001){
	  htest = findOH(mole.asymm.at(mole.contact.at(j).a1).frac,
			  mole.cell.symmops.at(mole.contact.at(j).sn) * mole.asymm.at(mole.contact.at(j).a2).frac
			  + mole.cell.trans.at(mole.contact.at(j).sn) ,
			  mole.contact.at(j).a1,alab);
	  if (Distance(htest.pos,V3(0,0,0))<0.001){
	    keinDonor[mole.contact.at(j).a2]++;
	  }
	} 
	else {
	  keinDonor[mole.contact.at(j).a1]++;
	}
	if (Distance(htest.pos,V3(0,0,0))){
	  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
		  .arg(htest.Label)
		  .arg(qMax(1,sfac.indexOf(0)+1))
		  .arg(htest.frac.x,9,'f',5)
		  .arg(htest.frac.y,9,'f',5)
		  .arg(htest.frac.z,9,'f',5)
		  .arg(mole.asymm.at(mole.contact.at(j).a1).sof_org)
		  .arg("-1.5");
	  mole.asymm.append(htest);
	  QString label = mole.showatoms.at(mole.contact.at(j).a1).orginalLine.left(80).trimmed();
	  int afixfuenf=(mole.showatoms.at(mole.contact.at(j).a1).afix/10)?(mole.showatoms.at(mole.contact.at(j).a1).afix/10)*10+5:0;
	  cursor.movePosition(QTextCursor::Start);
	  cursor = document->find(label,cursor);
	  if (cursor.isNull()){
	    cursor = editor->textCursor();
	    cursor.movePosition(QTextCursor::Start);
	    label.chop(10);
	    cursor = document->find(label,cursor);
	    if (cursor.isNull()) {
	      cursor = editor->textCursor();
	    }
	  }
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  while (cursor.selectedText().endsWith("=")) {
	    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
	    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	  }
	  cursor.clearSelection();
	  cursor.movePosition(QTextCursor::Down);
	  cursor.insertText(QString("AFIX %2\n%1\nAFIX %3\n")
			  .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
			  .arg((fxle->n5)?148:88)
			  .arg(afixfuenf)
			  );
	  keinDonor[mole.contact.at(j).a1]++;
	}
      }
    }

  }
  cursor.endEditBlock();
  if (gehtMichAn.size()) gehtMichAn.takeLast();
  if (mole.asymm.size()!=warmal){
    gehtMichAn.append(mole.asymm.size()-warmal+2);
  }

  int jetztAtome=0;
  for (int i=0; i<mole.asymm.size();i++) if (mole.asymm.at(i).an>=0)jetztAtome++;
  actionHFix->setDisabled(true);
  fuse();
  QString info=QString("<font color=\"green\"><b>%1</b> Hydrogen atoms added. <h2>Please SAVE NOW!</h2></font>").arg(jetztAtome-warmalAtome);
  infoKanalNews(info); 

  switch (duplicates()){
  case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
  case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  }
}

MyAtom Window::findOH(V3 donor, V3 acceptor,int dindex,QStringList alab){
  CEnvironment peaks;
  QString nn;
  peaks.clear();
  double minGoofy=100000.0,g=0;
  int minG=0;
  V3 mdz=(0.5*fxle->dx)+(0.5*fxle->dy)+(0.5*fxle->dz);
  V3 df=donor;
  V3 af=acceptor;
  if (mole.fl(af.x-df.x, af.y-df.y,af.z-df.z)>3.3){
    V3 floors;
    floors = af-df+ V3(0.5,0.5,0.5);
    floors = V3(floor(floors.x),floor(floors.y),floor(floors.z));
    af+=-1.0*floors;
  }
  double ad=mole.fl(af.x-df.x, af.y-df.y,af.z-df.z);
  MyAtom htest;
  htest.an=0;
  htest.part=0;
  htest.resiNr=0;
  htest.hidden=0;
  htest.Label=mole.asymm.at(dindex).Label.section('_',0,0);
  htest.part=mole.asymm.at(dindex).part;
  htest.resiNr=mole.asymm.at(dindex).resiNr;
  htest.Label.replace(0,1,'H');
  int hindex=0;
  do {
    if (hindex) htest.Label=htest.Label.left(3).append(QString::number(hindex+9,36));
	  if (htest.Label.size()>4) htest.Label=htest.Label.left(2).append(QString::number(hindex+9,36));
    nn=QString("%1_%3%4")
	    .arg(htest.Label)
	    .arg((htest.resiNr)?QString::number(htest.resiNr):"")
	    .arg((htest.part)?QString::number((htest.part<0)?
				    36+htest.part:
				    htest.part+9,36):"");
    hindex++;
   if ((hindex>1)&&(alab.contains(nn,Qt::CaseInsensitive))) htest.Label.chop(1);
  }while(alab.contains(nn,Qt::CaseInsensitive));
  alab.append(nn);
  htest.symmGroup=0;
  htest.frac=V3(0,0,0);
  htest.pos=V3(0,0,0);
  htest.uc.m12=htest.uc.m23=htest.uc.m13=0.0;
  htest.uc.m21=htest.uc.m32=htest.uc.m31=0.0;
  htest.uf.m12=htest.uf.m23=htest.uf.m13=0.0;
  htest.uf.m21=htest.uf.m32=htest.uf.m31=0.0;
  htest.uf.m11=0.05;
  htest.uf.m22=-666.0;
  htest.uf.m33=-1.5;
  htest.uc.m11=htest.uc.m22=htest.uc.m33=htest.uf.m11;
  if (fxle->n5==0) return htest;
  double x,y,z,u,v,w,p,q,r,t;
  for (int k=0;k<fxle->n3;k++){
    for (int j=0;j<fxle->n2;j++){
      for (int i=0;i<fxle->n1;i++){
	V3 o= (i*fxle->dx+j*fxle->dy+k*fxle->dz) + mdz;
	mole.kart2frac(o,o);
	V3 floors;
	floors = o-df+ V3(0.5,0.5,0.5);
	floors = V3(floor(floors.x),floor(floors.y),floor(floors.z));
	o+=-1.0*floors;
	double ha,hd;
	hd=mole.fl(df.x-o.x, df.y-o.y,df.z-o.z);
	ha=mole.fl(af.x-o.x, af.y-o.y,af.z-o.z);
	if (1.2<hd) continue;
	if (2.6<ha) continue;
	if (hd<0.6) continue;
	if (((ha*ha+hd*hd-ad*ad)/(2*hd*ha))>-0.707106781187)continue; //winkel groesser 135 sind ok!
	t = fxle->datfo_fc[i+ fxle->n1*(j+k*fxle->n2)];
	x = t -  fxle->datfo_fc[((i+1)%fxle->n1) + fxle->n1*(j+k*fxle->n2)];
	y = t -  fxle->datfo_fc[i+ fxle->n1*(((j+1)%fxle->n2)+k*fxle->n2)];
	z = t -  fxle->datfo_fc[i+ fxle->n1*(j+((k+1)%fxle->n3)*fxle->n2)];
	u = t -  fxle->datfo_fc[((i+fxle->n1-1)%fxle->n1)+ fxle->n1*(j+k*fxle->n2)];
	v = t -  fxle->datfo_fc[i+ fxle->n1*(((j+fxle->n2-1)%fxle->n2)+k*fxle->n2)];
	w = t -  fxle->datfo_fc[i+ fxle->n1*(j+((k+fxle->n2-1)%fxle->n3)*fxle->n2)];
	if (fmin(x,fmin(y,fmin(z,fmin(u,fmin(v,w)))))<0) continue;
	p=0.5*(u-x)/(u+x);
	q=0.5*(v-y)/(v+y);
	r=0.5*(w-z)/(w+z);
	t=t+0.25*(p*(u-x)+q*(v-y)+r*(w-z));
	if (t<fabs(fxle->iso[1])*1.2) continue;
	htest.peakHeight=t;  
	htest.frac.x = ((double) i + p +0.5)/fxle->n1; 
	htest.frac.y = ((double) j + q +0.5)/fxle->n2;
	htest.frac.z = ((double) k + r +0.5)/fxle->n3;
	hd=mole.fl(df.x-htest.frac.x, df.y-htest.frac.y,df.z-htest.frac.z);
	ha=mole.fl(af.x-htest.frac.x, af.y-htest.frac.y,af.z-htest.frac.z);
	if (hd<0.6) continue;
	g=(ha+hd)/htest.peakHeight;
	minG=(g<minGoofy)?peaks.size():minG;
	minGoofy=(g<minGoofy)?g:minGoofy;
	mole.frac2kart( htest.frac, htest.pos);
	peaks.append(htest); 
      }
    }
  }
  if (peaks.isEmpty()) {
    htest.frac=htest.pos=V3(0,0,0);
    return htest;
  }
  V3 D,floorD,prime,nf,dp;
  double min=1000,dk=0;
  for (int nn=0;nn<mole.cell.symmops.size();  nn++){
    prime=mole.cell.symmops.at(nn) * peaks.at(minG).frac + mole.cell.trans.at(nn);
    D=prime - mole.asymm.at(dindex).frac + V3(0.5,0.5,0.5) ;
    floorD=V3(floor(D.x),floor(D.y),floor(D.z));
    dp=D - floorD - V3(0.5,0.5,0.5);
    dk=mole.fl(dp.x,dp.y,dp.z);
    if (dk<min){
      min=dk;
      nf=prime-floorD;
    }
  }
  peaks[minG].frac=nf;
  mole.frac2kart(peaks[minG].frac,peaks[minG].pos);
  return peaks[minG];
}

void Window::invert(){
  if (mole.asymm.isEmpty()) return;
  //inverting structure
   QList<V3> moves;
   moves.append(V3(1,1,1));//most of the space groups
   moves.append(V3(1,0.5,1));//I41, I41md, I41cd  
   moves.append(V3(1,0.5,0.25));//I4122, I-42d
   moves.append(V3(0.25,0.25,1));//Fdd2
   moves.append(V3(0.25,0.25,0.25));//F4132
   int enantio=0;
   QString kep,nextli;
   QTextDocument *document = editor->document();
   QTextCursor cursor = editor->textCursor();
   cursor.beginEditBlock ();
   if ((!mole.cell.centered)&&(!mole.cell.centeric)){
      for (int i=0; i<mole.cell.trans.size(); i++){
	V3 t = V3(1,1,1) - mole.cell.trans.at(i);
	t.x=fmod(t.x,1);
	t.y=fmod(t.y,1);
	t.z=fmod(t.z,1);
        if (!(t==mole.cell.trans.at(i))) {
          enantio++;
          mole.cell.trans[i]=t;
	}

      }   
   }
   if (enantio){

     replace->setText("REM SYMM ");
     search->setText("^SYMM ");
     Window::replaceAll();
     cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
     cursor = document->find("REM SYMM",cursor);
     cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
     for (int i=1; i<mole.cell.symmops.size(); i++){
       cursor.insertText(QString("SYMM %1\n").arg(mole.symmcode2human(i)));
     }

     cursor.insertText("REM enantiomorph space group. SYMM cards changed!\n");  
     search->setText("");
     replace->setText("");
   }
   //qDebug("test %d\n",enantio);
   V3 testP=V3(0.3234,0.2234,0.1234); 
   QList<V3> tps,ips;
   Matrix inv(-1.0,0.0,0.0, 0.0,-1.0,0.0, 0.0,0.0,-1.0);  
   for (int n=0;n<mole.cell.symmops.size();  n++){
   V3 pt=mole.cell.symmops.at(n) * testP + mole.cell.trans.at(n);
   ips.append(pt);
   }
   int kkk=0,jjj=0,mo=0;
   if (mole.cell.centered) do{
     kkk=jjj=0;
     tps.clear();
     for (int z=0; z<ips.size();z++){
       V3 pt=inv*ips.at(z)+moves.at(mo);
       tps.append(pt);
     }
   double dk;
  V3 prime,dp,D,floorD;
   for (int i=0; i<tps.size();i++){
   for (int j=0; j<tps.size();j++){
      for (int n=0;n<mole.cell.symmops.size();  n++){
	prime=mole.cell.symmops.at(n) * tps.at(i) + mole.cell.trans.at(n);
	D=prime - tps.at(j)+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
	dk=mole.fl(dp.x,dp.y,dp.z);
	if (dk<0.1) kkk++;
      }
    }
  }
   for (int i=0; i<ips.size();i++){
   for (int j=0; j<ips.size();j++){
      for (int n=0;n<mole.cell.symmops.size();  n++){
	prime=mole.cell.symmops.at(n) * ips.at(i) + mole.cell.trans.at(n);
	D=prime - ips.at(j)+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
	dk=mole.fl(dp.x,dp.y,dp.z);
	if (dk<0.1) jjj++;
      }
    }
  }
   mo++;
   }while (kkk-jjj);
   if (mo) mo--;
   //printf("Move : %g %g %g\n",moves.at(mo).x,moves.at(mo).y,moves.at(mo).z);
   for (int i = 0; i < mole.asymm.size(); i++){
       V3 f = inv * mole.asymm.at(i).frac + moves.at(mo);
       mole.asymm[i].frac=f;
       mole.frac2kart(mole.asymm.at(i).frac,mole.asymm[i].pos);

       if (mole.asymm.at(i).an==-1) continue;
       QString source,target;
       source=mole.asymm.at(i).Label;
       target=source.section(QString::fromUtf8("»"),0,0);
       MyAtom ato=mole.asymm.at(i);
       ato.Label=target;
       ato.symmGroup=0;
       int that=mole.asymm.indexOf(ato);
       if ((that>=mole.asymm.size())||(that<0)) return;
       QString susu=mole.asymm.at(that).orginalLine;
       cursor.movePosition(QTextCursor::Start);
       cursor = document->find(susu,cursor);
       if (mole.asymm.at(that).uf.m22==-666.0){
         kep=QString("%1 %2 %3 %4 %5 %6 %7")
                         .arg(ato.Label.section('_',0,0),-5)
                         .arg(qMax(1,sfac.indexOf(ato.an)+1))
                         .arg(ato.frac.x,10,'f',6)
                         .arg(ato.frac.y,10,'f',6)
                         .arg(ato.frac.z,10,'f',6)
                         .arg(mole.asymm.at(that).sof_org,9,'f',5)
                         .arg(ato.uf.m33=mole.asymm.at(that).uf.m33,9,'f',5)
                         ;
         cursor.deleteChar();
         cursor.insertText(kep);
         cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
         ato.sof_org=mole.asymm.at(that).sof_org;
   //      printf("debug:iso sof==%g atom#%d mole.asymm.at(that).afixParent %d \n",mole.asymm.at(that).sof_org,that,mole.asymm.at(that).afixParent);
         ato.uf.m11=ato.uc.m11=ato.uc.m22=ato.uc.m33= mole.asymm.at(that).uf.m11;
         ato.uf.m22=-666.0;
         ato.uc.m12=ato.uc.m13=ato.uc.m23=ato.uc.m21=ato.uc.m31=ato.uc.m32=0;
         wholeLine->setChecked(false);
         replaceNext();
       }else{
         kep=QString("%1 %2 %3 %4 %5 %6 %7 %8 ")
                 .arg(ato.Label.section('_',0,0),-5)
                 .arg(qMax(1,sfac.indexOf(ato.an)+1))
                 .arg(ato.frac.x,10,'f',6)
                 .arg(ato.frac.y,10,'f',6)
                 .arg(ato.frac.z,10,'f',6)
                 .arg(mole.asymm.at(that).sof_org,9,'f',5)
                 .arg(ato.uf.m11,9,'f',5)
                 .arg(ato.uf.m22,9,'f',5);
     //    printf("debug:anis sof==%g atom#%d\n",mole.asymm.at(that).sof_org,that);
         ato.sof_org=mole.asymm.at(that).sof_org;
         cursor.deleteChar();
         cursor.insertText(kep);
         cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
         cursor.movePosition(QTextCursor::Down);
         cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
         cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
         nextli=cursor.selectedText();
         cursor = document->find(nextli,cursor);

         cursor.deleteChar();
         cursor.insertText(QString("     %1 %2 %3 %4")
                         .arg(ato.uf.m33,9,'f',5)
                         .arg(ato.uf.m23,9,'f',5)
                         .arg(ato.uf.m13,9,'f',5)
                         .arg(ato.uf.m12,9,'f',5));
         cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
       }
       ato.orginalLine=kep.section("=",0,0);
       mole.asymm[that]=ato;
     }

     cursor = editor->textCursor();
     cursor.endEditBlock();
     chgl->pause=true;
     mole.showatoms.clear();
     mole.showbonds.clear();
     chgl->mol->showatoms.clear();
     chgl->mol->showbonds.clear();
     chgl->mol->selectedatoms.clear();
     for (int o=0; o<mole.asymm.size();o++)
       mole.showatoms.append(mole.asymm[o]);
     mole.sdmcompleter();
     mole.showbonds =
             mole.connecting(mole.showatoms);
      chgl->setMolecule(&mole);
     chgl->mol->selectedatoms.clear();
     chgl->murx=1;
     bool growYes=chgl->fuse->isVisible ();
     chgl->mol->fuse();
     if (growYes) chgl->mol->grow();
     search->setText("");
     hideSearch();
     chgl->pause=false;
     chgl->updateGL();
     update();

}

void Window::addDissorderedMethyl(){
  QAction *action = qobject_cast<QAction *>(sender());
  int i=0;
  if (action)
    i=action->data().toInt();
  else return;
  if (i>mole.asymm.size()) return;
  int warmalAtome=0;
  for (int i=0; i<mole.asymm.size();i++) if (mole.asymm.at(i).an>=0)warmalAtome++;

  chgl->disSelection();
  MyAtom htest,qtest;
  if (sfac.indexOf(0)==-1)addNewScatteringFactor(0);
  htest.an=0;
  htest.part=0;
  htest.resiNr=0;
  htest.hidden=0;
  htest.symmGroup=0;
  htest.uc.m12=htest.uc.m23=htest.uc.m13=0.0;
  htest.uc.m21=htest.uc.m32=htest.uc.m31=0.0;
  htest.uf.m12=htest.uf.m23=htest.uf.m13=0.0;
  htest.uf.m21=htest.uf.m32=htest.uf.m31=0.0;
  qtest=htest;
  qtest.an=-1;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();
  //  !!!Neue Freie Variable anlegen!!!
  cursor.movePosition(QTextCursor::End);	
  cursor = document->find("FVAR",cursor,QTextDocument::FindBackward);
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  while (cursor.selectedText().endsWith("=")) {
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
  }
  QString fvstr = cursor.selectedText().trimmed();
  fvstr.append(" 0.666 ");
  fvar.append(0.667);
  cursor.deleteChar();
  cursor.insertText(fvstr);
  updateTieFreeVar();
  int frei=fvar.size();
  // !!!!
  double xh[8]={
    0.98,//1
    0.97,//2
    0.96,//3
    0.93,//4
    1.10,//15 B
    0.82,//8 O
    0.93,//9
    0.93// 16
  };
  V3 v=V3(0,0,0),w=V3(0,0,0),u=V3(0,0,0) ,a=V3(0,0,0);//,b=V3(0,0,0),c=V3(0,0,0);
  a = mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos - mole.asymm.at(i).pos;
  int ji,ki,li;
  a=Normalize(a);
  double jx,kx,lx,mx,my,mz;
  double val[24],merg[4],wink=0,t=0,mt=0;
  for (int z=0; z<4;z++) merg[z]=0;
  if (fxle->n5){
    for (int z=0; z<24; z++){
      w=Normalize(V3(0,0,1)%a);
      u=Normalize(w%a);
      v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
      qtest.pos=v;
      mole.kart2frac(v,v);
      int zplu=0;
      do {
	jx=((zplu+v.x)*fxle->n1+0.5);
	zplu++;
      }while (jx<0);
      zplu=0;
      do {
	kx=((zplu+v.y)*fxle->n2+0.5);
	zplu++;
      }while (kx<0);
      zplu=0;
      do {
	lx=((zplu+v.z)*fxle->n3+0.5);
	zplu++;
      }while (lx<0);
      mx=jx-floor(jx);
      my=kx-floor(kx);
      mz=lx-floor(lx);
      ji=((int)jx)%fxle->n1;//
      ki=((int)kx)%fxle->n2;//
      li=((int)lx)%fxle->n3;//
      merg[z%4]+=val[z]=(
		      (1-mx)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      mx*fxle->datfo_fc[((ji+1)+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      (1-my)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      my*fxle->datfo_fc[(ji+fxle->n1*((ki+1)+li*fxle->n2))%fxle->n5]+
		      (1-mz)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      mz*fxle->datfo_fc[(ji+fxle->n1*(ki+(li+1)*fxle->n2))%fxle->n5])/3.0;
      qtest.peakHeight=val[z];
      qtest.frac=v;
      qtest.Label=QString("Q%1").arg(z,-3);
      mole.pmin=qMin(mole.pmin, qtest.peakHeight);
      mole.pmax=qMax(mole.pmax, qtest.peakHeight);
      qpv->cutoff=0;//mole.pmin;
      qpv->setMin(mole.pmin);
      qpv->setMax(mole.pmax);
      mole.asymm.append(qtest);
      wink+=0.261799387799;
    }
    for (int z=0; z<4;z++) {
      t =merg[(z+4)%4] ;
      double x = t - merg[(z+5)%4];
      double u = t - merg[(z+3)%4];
      if (fmin(x,u)<0) continue;
      double p=0.5*(u-x)/(u+x);
      t=t+0.25*(p*(u-x));
      mt=(mt>t)?mt:t;
      if (t<mt)continue;
      wink=((double) z + p )*0.261799387799; 
    }
  }else{
    int zzz=0;
    int z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
    while ((z==i)&&(zzz<mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.size()))
      z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);

    w=Normalize(V3(0,0,1)%a);
    u=Normalize(w%a);
    v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
    wink=mole.dieder(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos-mole.asymm.at(z).pos,
		    mole.asymm.at(i).pos-mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos,
		    v-mole.asymm.at(i).pos)/180.0*M_PI;
  }
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label=mole.asymm.at(i).Label.section('_',0,0);
  htest.Label.replace(0,1,'H');
  htest.Label=htest.Label.left(3).append('A');
  htest.part=mole.asymm.at(i).part;
  htest.resiNr=mole.asymm.at(i).resiNr;
  htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
  htest.uf.m11=mole.asymm.at(i).uf.m11*1.5;
  htest.uf.m22=-666.0;
  htest.uf.m33=-1.5;
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof+10*frei)
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=1.04719755119;
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('D');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(-1*(mole.asymm.at(i).sof+10*frei))
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=1.04719755119;
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('B');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof+10*frei)
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=1.04719755119;
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('E');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(-1*(mole.asymm.at(i).sof+10*frei))
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=1.04719755119;
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('C');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof+10*frei)
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=1.04719755119;
  v=(-(xh[2])*0.33333*a)+ mole.asymm.at(i).pos+(xh[2])*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('F');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(-1*(mole.asymm.at(i).sof+10*frei))
	  .arg("-1.5");
  mole.asymm.append(htest);
  QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
  int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(label,cursor);
  if (cursor.isNull()){
    cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    label.chop(10);
    cursor = document->find(label,cursor);
    if (cursor.isNull()) {
      cursor = editor->textCursor();
    }
  }
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  while (cursor.selectedText().endsWith("=")) {
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  }
  cursor.clearSelection();
  cursor.movePosition(QTextCursor::Down);
  cursor.insertText(QString("AFIX 127\n%1\n%2\n%3\n%5\n%6\n%7\nAFIX %4\n")
		  .arg(mole.asymm.at(mole.asymm.size()-6).orginalLine)
		  .arg(mole.asymm.at(mole.asymm.size()-4).orginalLine)
		  .arg(mole.asymm.at(mole.asymm.size()-2).orginalLine)
		  .arg(afixfuenf)
		  .arg(mole.asymm.at(mole.asymm.size()-5).orginalLine)//-
		  .arg(mole.asymm.at(mole.asymm.size()-3).orginalLine)//-
		  .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)//-
		  );

  cursor.endEditBlock();
  int jetztAtome=0;
  for (int i=0; i<mole.asymm.size();i++) if (mole.asymm.at(i).an>=0)jetztAtome++;
  actionHFix->setDisabled(true);
  fuse();
  QString info=QString("<font color=\"green\"><b>%1</b> Hydrogen atoms added. <b>Please SAVE before GROW!</font>").arg(jetztAtome-warmalAtome);
  infoKanalNews(info); 
}

void Window::addMethyl(){
  QAction *action = qobject_cast<QAction *>(sender());
  int i=0;
  if (action)
    i=action->data().toInt();
  else return;
  if (i>mole.asymm.size()) return;
  int warmalAtome=0;
  for (int j=0; j<mole.asymm.size();j++) if (mole.asymm.at(j).an>=0)warmalAtome++;
  double nh= (mole.asymm.at(i).an==6) ? 0.07 : 0;

  chgl->disSelection();
  MyAtom htest,qtest;
  if (sfac.indexOf(0)==-1)addNewScatteringFactor(0);
  htest.an=0;
  htest.part=0;
  htest.resiNr=0;
  htest.hidden=0;
  htest.symmGroup=0;
  htest.uc.m12=htest.uc.m23=htest.uc.m13=0.0;
  htest.uc.m21=htest.uc.m32=htest.uc.m31=0.0;
  htest.uf.m12=htest.uf.m23=htest.uf.m13=0.0;
  htest.uf.m21=htest.uf.m32=htest.uf.m31=0.0;
  qtest=htest;
  qtest.an=-1;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();
  double xh[8]={
    0.98,//1
    0.97,//2
    0.96,//3
    0.93,//4
    1.10,//15 B
    0.82,//8 O
    0.93,//9
    0.93// 16
  };
  V3 v=V3(0,0,0),w=V3(0,0,0),u=V3(0,0,0) ,a=V3(0,0,0);//,b=V3(0,0,0),c=V3(0,0,0);
  a = mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos - mole.asymm.at(i).pos;
  int ji,ki,li;
  a=Normalize(a);
  double jx,kx,lx,mx,my,mz;
  double val[24],merg[8],wink=0,t=0,mt=0;
  for (int z=0; z<8;z++) merg[z]=0;
  if (fxle->n5){
    for (int z=0; z<24; z++){
      w=Normalize(V3(0,0,1)%a);
      u=Normalize(w%a);
      v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
      qtest.pos=v;
      mole.kart2frac(v,v);
      int zplu=0;
      do {
	jx=((zplu+v.x)*fxle->n1+0.5);
	zplu++;
      }while (jx<0);
      zplu=0;
      do {
	kx=((zplu+v.y)*fxle->n2+0.5);
	zplu++;
      }while (kx<0);
      zplu=0;
      do {
	lx=((zplu+v.z)*fxle->n3+0.5);
	zplu++;
      }while (lx<0);
      mx=jx-floor(jx);
      my=kx-floor(kx);
      mz=lx-floor(lx);
      ji=((int)jx)%fxle->n1;//
      ki=((int)kx)%fxle->n2;//
      li=((int)lx)%fxle->n3;//
      merg[z%8]+=val[z]=(
		      (1-mx)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      mx*fxle->datfo_fc[((ji+1)+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      (1-my)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      my*fxle->datfo_fc[(ji+fxle->n1*((ki+1)+li*fxle->n2))%fxle->n5]+
		      (1-mz)*fxle->datfo_fc[(ji+fxle->n1*(ki+li*fxle->n2))%fxle->n5]+
		      mz*fxle->datfo_fc[(ji+fxle->n1*(ki+(li+1)*fxle->n2))%fxle->n5])/3.0;
      qtest.peakHeight=val[z];
      qtest.frac=v;
      qtest.Label=QString("Q%1").arg(z,-3);
      mole.pmin=qMin(mole.pmin, qtest.peakHeight);
      mole.pmax=qMax(mole.pmax, qtest.peakHeight);
      qpv->cutoff=0;//mole.pmin;
      qpv->setMin(mole.pmin);
      qpv->setMax(mole.pmax);
      mole.asymm.append(qtest);
      wink+=0.261799387799;
    }
    for (int z=0; z<8;z++) {
      t =merg[(z+8)%8] ;
      double x = t - merg[(z+9)%8];
      double u = t - merg[(z+7)%8];
      if (fmin(x,u)<0) continue;
      double p=0.5*(u-x)/(u+x);
      t=t+0.25*(p*(u-x));
      mt=(mt>t)?mt:t;
      if (t<mt)continue;
      wink=((double) z + p )*0.261799387799; 
    }
  }else{
    int zzz=0;
    int z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);
    while ((z==i)&&(zzz<mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.size()))
      z=mole.knoepfe.at(mole.knoepfe.at(i).neighbors.at(0)).neighbors.at(zzz++);

    w=Normalize(V3(0,0,1)%a);
    u=Normalize(w%a);
    v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
    wink=mole.dieder(mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos-mole.asymm.at(z).pos,
		    mole.asymm.at(i).pos-mole.showatoms.at(mole.knoepfe.at(i).neighbors.at(0)).pos,
		    v-mole.asymm.at(i).pos)/180.0*M_PI;
  }
  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label=mole.asymm.at(i).Label.section('_',0,0);
  htest.part=mole.asymm.at(i).part;
  htest.resiNr=mole.asymm.at(i).resiNr;
  htest.Label.replace(0,1,'H');
  htest.Label=htest.Label.left(3).append('A');
  htest.uc.m11=htest.uc.m22=htest.uc.m33=mole.asymm.at(i).uc.m11*1.2;
  htest.uf.m11=mole.asymm.at(i).uf.m11*1.5;
  htest.uf.m22=-666.0;
  htest.uf.m33=-1.5;
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof_org)
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=2.09439510239;
  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('B');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof_org)
	  .arg("-1.5");
  mole.asymm.append(htest);
  wink+=2.09439510239;
  v=(-(xh[2]-nh)*0.33333*a)+ mole.asymm.at(i).pos+(xh[2]-nh)*0.942809041582*(sin(wink)*u+cos(wink)*w);
  htest.Label.chop(1);
  htest.Label=htest.Label.left(3).append('C');
  htest.pos=v;
  mole.kart2frac(htest.pos,htest.frac);	
  htest.orginalLine=QString("%1  %2  %3 %4 %5 %6 %7")
	  .arg(htest.Label)
	  .arg(qMax(1,sfac.indexOf(0)+1))
	  .arg(htest.frac.x,9,'f',5)
	  .arg(htest.frac.y,9,'f',5)
	  .arg(htest.frac.z,9,'f',5)
	  .arg(mole.asymm.at(i).sof_org)
	  .arg("-1.5");
  mole.asymm.append(htest);
  QString label = mole.showatoms.at(i).orginalLine.left(80).trimmed();
  int afixfuenf=(mole.showatoms.at(i).afix/10)?(mole.showatoms.at(i).afix/10)*10+5:0;
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(label,cursor);
  if (cursor.isNull()){
    cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    label.chop(10);
    cursor = document->find(label,cursor);
    if (cursor.isNull()) {
      cursor = editor->textCursor();
    }
  }
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  while (cursor.selectedText().endsWith("=")) {
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  }
  cursor.clearSelection();
  cursor.movePosition(QTextCursor::Down);
  cursor.insertText(QString("AFIX 137\n%1\n%2\n%3\nAFIX %4\n")
		  .arg(mole.asymm.at(mole.asymm.size()-3).orginalLine)
		  .arg(mole.asymm.at(mole.asymm.size()-2).orginalLine)
		  .arg(mole.asymm.at(mole.asymm.size()-1).orginalLine)
		  .arg(afixfuenf)
		  );

  cursor.endEditBlock();
  int jetztAtome=0;
  for (int i=0; i<mole.asymm.size();i++) if (mole.asymm.at(i).an>=0)jetztAtome++;
  actionHFix->setDisabled(true);
  fuse();
  QString info=QString("<font color=\"green\"><b>%1</b> Hydrogen atoms added. <b>Please SAVE before GROW!</font>").arg(jetztAtome-warmalAtome);
  infoKanalNews(info); 
}

bool peakcomp(MyAtom &a1,MyAtom &a2){
return (a1.peakHeight>a2.peakHeight);
}

void Window::addMoreQPeaks(){
  if (mole.asymm.isEmpty()) return;
  if (fxle->n5==0) return;

  for (int i=mole.asymm.size()-1; i>=0; i--){
    if (mole.asymm.at(i).an<0) mole.asymm.removeAt(i); 
  }
  MyAtom htest;
  htest.an=-1;
  htest.part=0;
  htest.resiNr=0;
  htest.hidden=0;
  htest.symmGroup=0;
  htest.frac=V3(0,0,0);
  htest.pos=V3(0,0,0);
  htest.resiNr=0;
  htest.part=0;
  htest.symmGroup=0;
  htest.uc.m12=htest.uc.m23=htest.uc.m13=0.0;
  htest.uc.m21=htest.uc.m32=htest.uc.m31=0.0;
  htest.uf.m12=htest.uf.m23=htest.uf.m13=0.0;
  htest.uf.m21=htest.uf.m32=htest.uf.m31=0.0;
  htest.uf.m11=0.05;
  htest.uf.m22=-666.0;
  htest.uf.m33=-1.5;
  htest.uc.m11=htest.uc.m22=htest.uc.m33=htest.uf.m11;
  float x,y,z,u,v,w,p,q,r,t;
  int ipeak=0;
  mole.pmin=10000;
  mole.pmax=-10000;
  QList<MyAtom> peaks;
  for (int k=0;k<fxle->n3;k++){
    for (int j=0;j<fxle->n2;j++){
      for (int i=0;i<fxle->n1;i++){
	t = fxle->datfo_fc[i+ fxle->n1*(j+k*fxle->n2)];
	x = t -  fxle->datfo_fc[((i+1)%fxle->n1) + fxle->n1*(j+k*fxle->n2)];
	y = t -  fxle->datfo_fc[i+ fxle->n1*(((j+1)%fxle->n2)+k*fxle->n2)];
	z = t -  fxle->datfo_fc[i+ fxle->n1*(j+((k+1)%fxle->n3)*fxle->n2)];
	u = t -  fxle->datfo_fc[((i+fxle->n1-1)%fxle->n1)+ fxle->n1*(j+k*fxle->n2)];
	v = t -  fxle->datfo_fc[i+ fxle->n1*(((j+fxle->n2-1)%fxle->n2)+k*fxle->n2)];
	w = t -  fxle->datfo_fc[i+ fxle->n1*(j+((k+fxle->n2-1)%fxle->n3)*fxle->n2)];
	if (fmin(x,fmin(y,fmin(z,fmin(u,fmin(v,w)))))<0) continue;
	p=0.5*(u-x)/(u+x);
	q=0.5*(v-y)/(v+y);
	r=0.5*(w-z)/(w+z);
	t=t+0.25*(p*(u-x)+q*(v-y)+r*(w-z));
	if (t<fabs(fxle->iso[1])*1.02) continue;
	htest.peakHeight=t;  
	htest.frac.x = ((double) i + p +0.5)/fxle->n1; 
	htest.frac.y = ((double) j + q +0.5)/fxle->n2;
	htest.frac.z = ((double) k + r +0.5)/fxle->n3;
	mole.frac2kart( htest.frac, htest.pos);
	if (!finite(htest.frac.x)||!finite(htest.frac.y)||!finite(htest.frac.z))continue;
	ipeak++;
	mole.pmin=qMin(mole.pmin, htest.peakHeight);
	mole.pmax=qMax(mole.pmax, htest.peakHeight);
    qpv->cutoff=0;//mole.pmin;
	qpv->setMin(mole.pmin);
	qpv->setMax(mole.pmax);
	htest.Label=QString("Q%1").arg(ipeak,3,36,QChar('0'));
	peaks.append(htest); 
      }
    }
  }

  qSort(peaks.begin(),peaks.end(),peakcomp );
  mole.asymm+=peaks;  /*
  ////
  // George Sheldrick Seminar ideas
  double dk,dddd;
  V3 prime,dp,D,floorD;
  mole.sdm.clear();
  SdmItem sdmItem;
  sdmItem.a1=0;
  sdmItem.a2=1;
  sdmItem.sn=0;
  SdmItem sdmItem2;
  sdmItem2.a1=0;
  sdmItem2.a2=1;
  sdmItem2.sn=0;

  for (int i=0; i<mole.asymm.size(); i++){ 
    if (mole.asymm.at(i).an<0) mole.asymm[i].style=0;
  }
  for (int i=0; i<mole.asymm.size(); i++){ 
    for (int j=0; j<mole.asymm.size(); j++ ){
      double min=1000000;
      bool hma=false;
      for (int n=0;n<mole.cell.symmops.size();  n++){
	prime=mole.cell.symmops.at(n) * mole.asymm.at(i).frac + mole.cell.trans.at(n);
	D=prime - mole.asymm.at(j).frac+ V3(0.5,0.5,0.5) ;
	floorD=V3(floor(D.x),floor(D.y),floor(D.z));
	dp=D - floorD - V3(0.5,0.5,0.5);
	dk=mole.fl(dp.x,dp.y,dp.z);
	if ((mole.asymm.at(i).an<0)&&(i==j)&&(dk<0.1)) mole.asymm[i].style++;
	if ((dk>0.01)&&((min+0.05)>=dk)){
	  min=dk;
	  sdmItem.d=min;
	  sdmItem.floorD=floorD;
	  sdmItem.a1=i;
	  sdmItem.a2=j;
	  sdmItem.sn=n;
	  hma=true;
	}
      }
      if ((mole.asymm[sdmItem.a1].an>-1)&&(mole.asymm[sdmItem.a2].an>-1)&&
		      ((mole.asymm[sdmItem.a1].part*mole.asymm[sdmItem.a2].part==0)||
		       (mole.asymm[sdmItem.a1].part==mole.asymm[sdmItem.a2].part)))
	dddd=(mole.Kovalenz_Radien[mole.asymm[sdmItem.a1].an]+ mole.Kovalenz_Radien[mole.asymm[sdmItem.a2].an])*0.012;
      else dddd=0;
      if (sdmItem.d<dddd){
	sdmItem.covalent=true;
      }else sdmItem.covalent=false;
      if (hma) mole.sdm.append(sdmItem);
    }
  }

  qSort(mole.sdm.begin(),mole.sdm.end());
  QList<int> flags;
  for (int i=0; i<mole.asymm.size(); i++) flags.append((mole.asymm.at(i).an<0)?-1:1);
  for (int i=0; i<mole.sdm.size(); i++)
    if ((flags.at(mole.sdm.at(i).a1)*flags.at(mole.sdm.at(i).a2))==-1) {
      if (((flags.at(mole.sdm.at(i).a1)==-1)&&((mole.asymm.at(mole.sdm.at(i).a2).an>-1)))){
	if ((mole.sdm.at(i).sn==0)&&(mole.sdm.at(i).floorD==V3(0,0,0))) {flags[mole.sdm.at(i).a1]=1;continue;}
	if (mole.sdm.at(i).d>2.4) continue;
	mole.asymm[mole.sdm.at(i).a1].frac = mole.cell.symmops.at(mole.sdm.at(i).sn) * mole.asymm[mole.sdm.at(i).a1].frac + mole.cell.trans.at(mole.sdm.at(i).sn) - mole.sdm.at(i).floorD;
	flags[mole.sdm.at(i).a1]=1;
	mole.frac2kart(mole.asymm[mole.sdm.at(i).a1].frac,mole.asymm[mole.sdm.at(i).a1].pos); 
      }
    }*/

  mole.sdmcompleter();
  QList<MyAtom> duplicate;
  for (int i=0; i<mole.asymm.size(); i++){
    for (int j=i+1; j<mole.asymm.size(); j++){
      if ((mole.asymm.at(i).an>-1)||(mole.asymm.at(j).an>-1)) continue;
      if (Distance(mole.asymm.at(i).pos,mole.asymm.at(j).pos)<0.1) duplicate.append(mole.asymm.at(j));
    }
  }
  //printf("Duplicate size %d\n",duplicate.size());
  for (int i=0; i<duplicate.size();i++) {
    //  printf("teste i= %d dup %d \n",i,duplicate.at(i).an);
    int rr=mole.asymm.indexOf(duplicate.at(i));
    if (rr<0) continue;
    //printf("loesche %d aus %d\n",rr,mole.asymm.size());
    mole.asymm.removeAt(rr);
  }
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock();
  replace->setText("");
  search->setText("^Q\\S+\\s+1\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s*");
  Window::replaceAll();
  search->setText("");
  cursor.movePosition(QTextCursor::End);
  cursor.movePosition(QTextCursor::StartOfLine);
  ipeak=1;
  for (int i=0; i<mole.asymm.size(); i++){
    if (mole.asymm.at(i).an>-1) continue;
    mole.asymm[i].Label=QString("Q%1").arg(ipeak,3,36,QChar('0')).toUpper();
    ipeak++;
    mole.asymm[i].orginalLine=
	    QString("%1 1 %2 %3 %4 %5 0.05 %6")
	    .arg(mole.asymm.at(i).Label,-4)
	    .arg(mole.asymm.at(i).frac.x,10,'f',6)
	    .arg(mole.asymm.at(i).frac.y,10,'f',6)
	    .arg(mole.asymm.at(i).frac.z,10,'f',6)
	    .arg(mole.asymm[i].sof_org=(10+(1.0/mole.asymm.at(i).style)),9,'f',5)
	    .arg(mole.asymm.at(i).peakHeight,9,'f',3)
	    ;
    mole.asymm[i].sof=mole.asymm[i].sof_org-10;

    cursor.insertText(mole.asymm[i].orginalLine);
    cursor.insertText("\n");

  }
  cursor.endEditBlock();

  mole.showatoms.clear();
  for (int i=0; i<mole.asymm.size(); i++){
    mole.showatoms.append(mole.asymm.at(i));
  }
  ////
  bool growYes=chgl->fuse->isVisible ();
  bool fuseYes=chgl->grow->isVisible ();
  chgl->fuse->setVisible(true);
  chgl->grow->setVisible(false);
  mole.sdmcompleter();
  //    mole.loadSettings();
  mole.showbonds = mole.connecting(mole.showatoms);
  //    if (mol!=chgl->mol) chgl->setMolecule(mole);

  chgl->mol->fuse();
  if (((!fuseYes)&&(growYes))||((!fuseYes)&&(!growYes))) chgl->mol->grow();
  else {
    chgl->fuse->setVisible(false);
    chgl->grow->setVisible(true);
  }
  infoKanalNews(mole.HumanSymmetry);
  chgl->murx=1;
  chgl->updateGL();
}

void Window::setShelxlPath(){
  QString s =
          QFileDialog::getOpenFileName(this, "Path to SHELXL executable", shelxlPath,QString(),0,QFileDialog::DontUseNativeDialog);
  if (!s.isEmpty()){
    shelxlPath=s;
    mole.einstellung->beginGroup("Window");
    mole.einstellung->setValue("SHELXL-PATH",shelxlPath);
    mole.einstellung->endGroup();
    if (shxPath!=NULL) shxPath->setText(shelxlPath);
  }
}

void Window::findResi(QString index){
  if (!index.isEmpty()) {
    QTextCursor c=editor->document()->find(index);
    if (!c.isNull()) {
      c.movePosition(QTextCursor::StartOfLine);
      editor->setTextCursor(c);
      editor->centerCursor();
      c.clearSelection ();
      //QTextBlock b =c.block();
      QStringList tt=index.split(QRegExp("\\s+"),QString::SkipEmptyParts);

      int nr=0;
      if ((tt.size()>1)&&(tt.at(1).indexOf(QRegExp("^[0-9]+"))!=-1))
      nr=tt.at(1).toInt();
      else if (tt.size()>2) nr=tt.at(2).toInt();
      //QString tt = index.section(QRegExp("\\s+"),1,1).toInt();
      chgl->selectResiByNr(nr);
      editor->setTextCursor(c);
      editor->setFocus () ;
    }
  }
}

void Window::resiFinderDestroyed(){
  resiFinder = NULL;
}

void Window::saveScene(){
  //qDebug()<<"Window::saveScene";
  glGetDoublev(GL_MODELVIEW_MATRIX,chgl->MM);

  QList<QByteArray> supo = QImageWriter::supportedImageFormats ();
  QString formats;
  saveName = dirName;
  saveName.replace(dirName.size()-4,4,".jpg");
  QString selectedFilter="jpg-file (*.jpg)";
  for (int i=0; i<supo.size(); i++){
    if ((!QString(supo.at(i)).contains("ps",Qt::CaseInsensitive)) &&
		    (!QString(supo.at(i)).contains(QRegExp("[A-Z]"))))
      formats+=QString("%1-file (*.%1)%2").arg(QString(supo.at(i))).arg(((i+1)<supo.size())?";;":"");
  }
  int
	  a = int(chgl->width()*scalePic+0.5),
	    b = int(chgl->height()*scalePic+0.5),
	    c = int(chgl->myFont.pointSize ()*scalePic+0.5),
	    d = chgl->width(),
	    e = chgl->height(),
	    f = chgl->myFont.pointSize ();
  QString fileName = QFileDialog::getSaveFileName(this,QString("Save a screen shot with %1 x %2 pixels").arg(a).arg(b), saveName,formats,&selectedFilter);
  if (!fileName.isEmpty()){
    QString endung=selectedFilter.section('*',1,1).section(')',0,0);
    if (!fileName.endsWith(endung)) fileName.append(endung);
    chgl->noWaitLabel=true;
    chgl->exporting=true;
    effuenf();
    chgl->myFont.setPointSize(c);
   //fprintf(stderr,"bild\n");
   QPixmap   map = chgl->renderPixmap(a,b);
   if (map.isNull ()){
   //printf("map 0\n");
   if (QGLFramebufferObject::hasOpenGLFramebufferObjects()) {
     QImage map=chgl->grabFrameBuffer(true); 
   //printf("hasOpenGLFramebufferObjects\n");
     map.save(fileName);
   }else{
     QPixmap pixmap = QPixmap::grabWindow( chgl->winId() );
     pixmap.save(fileName);
   //printf("not hasOpenGLFramebufferObjects\n");
   }
   }else map.save(fileName);
   //fprintf(stderr,"bild\n");
   // fprintf(stderr,"bild\n");
   if(!separateLabelFile->isChecked()) {
    QPixmap fmap = QPixmap(fileName);
    QPainter *paint = new QPainter(&fmap);
    paint->setPen(QPen(chgl->labelColor));
    paint->setFont(chgl->myFont);
    for (int n=0; n<chgl->LabelZ.size(); n++){
        if ((!(mole.AtomStyle[chgl->LabelZ.at(n).an]&ATOM_STYLE_NOLABEL)))
        paint->drawText(chgl->LabelZ.at(n).x,chgl->LabelZ.at(n).y,chgl->LabelZ.at(n).l);
    }
    paint->end();
    fmap.save(fileName);
  }
  else{
    QPixmap fmap = QPixmap(a,b);
    fmap.fill(QColor(128,128,128,0));
    QPainter *paint = new QPainter(&fmap);
    paint->setPen(QPen(chgl->labelColor));
    paint->setFont(chgl->myFont);
    for (int n=0; n<chgl->LabelZ.size(); n++){
        if ((!(mole.AtomStyle[chgl->LabelZ.at(n).an]&ATOM_STYLE_NOLABEL)))
        paint->drawText(chgl->LabelZ.at(n).x,chgl->LabelZ.at(n).y,chgl->LabelZ.at(n).l);
    }
    paint->end();
    QString fn2=fileName;
    fn2.insert(fileName.lastIndexOf("."),"_labels");
    fmap.save(fn2);
  }
    saveName=fileName;
    chgl->exporting=false;
  }
  chgl->myFont.setPointSize(f);
  chgl->ww=d;
  chgl->wh=e;
  effuenf();  
  chgl->noWaitLabel=false;
}

void Window::increaseEdtiorFont(){
    QFont f =editor->font();
    f.setPointSize(qMin(f.pointSize()+1,24));
    editor->setFont(f);
    xlPutor->setFont(f);
    update();
  }

void Window::decreaseEdtiorFont(){
  QFont f =editor->font();
  f.setPointSize(qMax(f.pointSize()-1,6));
  editor->setFont(f);
  xlPutor->setFont(f);  update();
}

void Window::infoKanalNews(const QString& s){
  QString alt = infoKanal->toHtml();
//  printf("\n\n\n%s\n\n\n",(const char *)alt.toLocal8Bit());
  alt.remove(QRegExp("ERROR [^!]*!"));
  alt.remove(QRegExp("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:large; color:#ff0000;\"><br /></span></p>",
			  Qt::CaseInsensitive,QRegExp::FixedString));
  alt.remove(QRegExp("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:large; color:#ff0000;\"></span></p>"
			  ,Qt::CaseInsensitive,QRegExp::FixedString));
  alt.append(s);
  infoKanal->setHtml(alt);
  QTextCursor c = infoKanal->textCursor();
  c.movePosition(QTextCursor::End);
  infoKanal->setTextCursor(c);
  infoKanal->ensureCursorVisible();
  if (chgl->toggleDockWidgets->isChecked()) return;
  if ( enterRenameMode->isChecked()) return;
  dock->show();
  dock->raise();
}

void Window::shx_editor(QString filename,QString title){
  if (otherOpenFiles){
    editor->clear();
    editor->document()->disconnect();
    actionUndo->disconnect();
    actionRedo->disconnect();
    actionCut->disconnect();
    actionCopy->disconnect();
    actionPaste->disconnect();
  }
  altursize=0;
  QFile shxf(filename);
  if (shxf.open(QIODevice::ReadOnly|QIODevice::Text)){
    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);
    QString s=shxf.readAll().replace('\0','~');
    s=s.replace(QRegExp("[ \t]+\n")," \n");// printf("remove non used spaces\n");
    editor->setPlainText(s);
    actionSave->setEnabled(false);
    actionHFix->setDisabled(false);
    setWindowModified(editor->document()->isModified());
    editor->highlighter->setDocument(editor->document());
    connect(editor->document(), SIGNAL(modificationChanged(bool)),
		    actionSave, SLOT(setEnabled(bool)));
    connect(editor->document(), SIGNAL(modificationChanged(bool)),
		    actionHFix, SLOT(setDisabled(bool)));
    connect(editor->document(), SIGNAL(modificationChanged(bool)),
		    this, SLOT(setWindowModified(bool)));
    connect(editor->document(), SIGNAL(undoAvailable(bool)),
		    actionUndo, SLOT(setEnabled(bool)));
    connect(editor->document(), SIGNAL(undoCommandAdded ()),
		    this, SLOT(wasRename()));
    connect(editor->document(), SIGNAL(undoCommandAdded()),this,SLOT(wasDelete()));
    connect(editor->document(), SIGNAL(redoAvailable(bool)),
		    actionRedo, SLOT(setEnabled(bool)));
    connect(actionUndo, SIGNAL(triggered()), editor, SLOT(undo()));
    connect(actionUndo,SIGNAL(triggered()),this,SLOT(undoRename()));
    connect(actionRedo, SIGNAL(triggered()), editor, SLOT(redo()));
    connect(actionRedo, SIGNAL(triggered()),this, SLOT(redoRename()));
    connect(actionCut, SIGNAL(triggered()), editor, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), editor, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), editor, SLOT(paste()));
    connect(editor, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));
    shxf.close();    
    shxEditorDock->setWindowTitle(title);
    shxEditorDock->show();
    shxEditorDock->raise();
    actionSearch->setEnabled(true);
    actionNext->setEnabled(true);
    actionPrev->setEnabled(true);
    otherOpenFiles=true;
    editor->fv=fvar;
    QTextCursor c1=editor->textCursor();
    c1.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
    int maxPos=c1.position();
    c1.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    int pos1=0;
    mole.einstellung->beginGroup("Cursor");
    if (mole.einstellung->contains("lastCursorHash"))
        myHash=mole.einstellung->value("lastCursorHash").toHash();
    //qDebug()<<myHash;
    pos1=myHash.value(dirName).toInt();
    mole.einstellung->endGroup();
    if ((myHash.contains(dirName))&&(pos1<=maxPos)&&(pos1>=0))
        c1.setPosition(pos1,QTextCursor::MoveAnchor);
    if (!c1.isNull()) editor->setTextCursor(c1);
    editor->centerCursor();
    editor->setFocus(Qt::OtherFocusReason);
    if (!title.contains("!!Empty!!"))infoKanalNews(fvarinfo);
  }
  balken->hide();
  chgl->pause=false;
  if (title.contains("!!Empty!!")) infoKanalNews("<font size=+2 color=red>No atoms in file!</font>");
}

bool Window::fileSave(){
  spellCheck();
  if (FehlerTeufel) {
    QMessageBox::warning(0,"Syntax-errors", "Syntax-errors were found in the file please correct them before saving! (Saving more frequently might reduce errors caused by this GUI)",QMessageBox::Ok,QMessageBox::Ok);
    return false;
  }
  QFile f(dirName);

  f.open(QIODevice::ReadOnly|QIODevice::Text);
  QString alterScheis=f.readAll();
  QFileInfo fi(f);
  f.close();
  QTextCursor c=editor->textCursor();
  if (!c.isNull()){
      myHash[dirName]=c.position();
      mole.einstellung->beginGroup("Cursor");
      mole.einstellung->setValue("lastCursorHash", myHash );
      mole.einstellung->endGroup();
  }
  bool success = f.open(QIODevice::WriteOnly|QIODevice::Text);
  if (success){
    f.write(editor->toPlainText().toLatin1());
    f.close();
    QString dn=dirName.section('/',0,-2),
	    argument=dirName.section('/',-1);
    QDir work=QDir(QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
    if (!work.exists ( QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME))) work.mkdir (QString("%1/%2saves/").arg(dn).arg(PROGRAM_NAME));
    QString saveHistName=QString("%1/%2saves/SAVEHIST").arg(dn).arg(PROGRAM_NAME);
    QFile sahi(saveHistName);
    sahi.open(QIODevice::Append|QIODevice::Text);
    sahi.write(QString("Entry|@|%1|@|%2|@|\n").arg(fi.lastModified().toString(Qt::ISODate)).arg(argument).toLatin1());
    sahi.write(alterScheis.toLatin1());
    editor->document()->setModified(false);
    loadFile(dirName);
  }
  else
    infoKanalNews(QString("<font color=\"red\">%1 could not be opened for writing!</font>").arg(f.fileName()));
  return success;
}

bool Window::fileSaveAs(){    
  spellCheck();
    if (FehlerTeufel) {
        QMessageBox::warning(0,"Syntax-errors", "Syntax-errors were found in the file please correct them before saving! (Saving more frequently might reduce errors caused by this GUI)",QMessageBox::Ok,QMessageBox::Ok);
        return false;
    }
  QString fn=QFileDialog::getSaveFileName(this, tr("Save As ..."), dirName, tr("SHELX-files (*.res *.ins)"));
  if (fn.isEmpty()) return false;
  QFile f(fn);
  bool success = f.open(QIODevice::WriteOnly|QIODevice::Text);
  if (success){
    f.write(editor->toPlainText().toLatin1());
    f.close();
    editor->document()->setModified(false);
    loadFile(dirName=fn);
  }
  return success;
}

void Window::unique(){
  if (mole.asymm.isEmpty()) return;
  mole.enviSDM(2.5);

  QList<int> flags;
  for (int i=0; i<mole.asymm.size(); i++) flags.append((i!=0)?-1:1);
  int finished=1;
  do{
    finished=1;
  for (int i=0; i<mole.envi_sdm.size(); i++){

   if (flags.at(mole.envi_sdm.at(i).a1)*flags.at(mole.envi_sdm.at(i).a2)==-1)  {
      if ((mole.asymm[mole.envi_sdm.at(i).a1].an<0)||(mole.asymm[mole.envi_sdm.at(i).a2].an<0))continue;
      if (flags.at(mole.envi_sdm.at(i).a1)==1)continue;
	if ((mole.envi_sdm.at(i).sn==0)&&(mole.envi_sdm.at(i).floorD==V3(0,0,0))) {
          flags[mole.envi_sdm.at(i).a1]=1;
          flags[mole.envi_sdm.at(i).a2]=1;
          finished=0;
          break;
        }
        mole.asymm[mole.envi_sdm.at(i).a1].frac = mole.cell.symmops.at(mole.envi_sdm.at(i).sn) * mole.asymm[mole.envi_sdm.at(i).a1].frac + mole.cell.trans.at(mole.envi_sdm.at(i).sn) - mole.envi_sdm.at(i).floorD;
	flags[mole.envi_sdm.at(i).a1]=1;
	flags[mole.envi_sdm.at(i).a2]=1;
        mole.frac2kart(mole.asymm[mole.envi_sdm.at(i).a1].frac,mole.asymm[mole.envi_sdm.at(i).a1].pos);
        finished=0;

        mole.enviSDM(2.5);
        break;
      }
    }
  }while (!finished);
    mole.showatoms.clear();
    mole.showbonds.clear();
    chgl->mol->showatoms.clear();
    chgl->mol->showbonds.clear();
    chgl->mol->selectedatoms.clear();
    for (int o=0; o<mole.asymm.size();o++){
      mole.showatoms.append(mole.asymm[o]);
    }
    mole.sdmcompleter();
    mole.showbonds =
	    mole.connecting(mole.showatoms);
     chgl->setMolecule(&mole);
    chgl->mol->selectedatoms.clear();
    chgl->murx=1;
    
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();

  for (int i=0; i<mole.asymm.size();i++){
    cursor.movePosition(QTextCursor::Start);
    QString label=mole.asymm.at(i).orginalLine.left(80).trimmed();
    cursor = document->find(label ,cursor);  

    if (cursor.isNull()){
      cursor = editor->textCursor();    
      cursor.movePosition(QTextCursor::Start);
      label.chop(10);
      cursor = document->find(label,cursor);
      if (cursor.isNull()) {
	cursor = editor->textCursor();
      }
    }
    QString vorherig,neues;
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    while (cursor.selectedText().endsWith("=")) {
      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
    }
    neues = vorherig= cursor.selectedText();
    QStringList sl = vorherig.split(QRegExp("\\s+"));
    sl.removeOne("=");
    if (sl.size()>=7)
      neues=QString("%1  %2  %3 %4 %5 %6 %7 ")
	      .arg(mole.asymm.at(i).Label.section('_',0,0),-4)
	      .arg(qMax(1,sfac.indexOf(mole.asymm.at(i).an)+1))
              .arg(mole.asymm.at(i).frac.x,10,'f',6)
              .arg(mole.asymm.at(i).frac.y,10,'f',6)
              .arg(mole.asymm.at(i).frac.z,10,'f',6)
              .arg(mole.asymm.at(i).sof_org,9,'f',5)
              .arg((mole.asymm.at(i).uf.m22==-666.0)?mole.asymm.at(i).uf.m33:mole.asymm.at(i).uf.m11,9,'f',5);
    mole.asymm[i].orginalLine=neues;
    if (mole.asymm.at(i).an<0) {
        neues.append(QString("%1 ")
                        .arg(mole.asymm.at(i).peakHeight,9,'f',2));
    } else
    if (mole.asymm.at(i).uf.m22!=-666.0){
     neues.append(QString("%1 =\n    %2 %3 %4 %5")
                     .arg(mole.asymm.at(i).uf.m22,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m33,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m23,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m13,9,'f',5)
                     .arg(mole.asymm.at(i).uf.m12,9,'f',5));
    } 
    cursor.deleteChar();
    cursor.insertText(neues);

  }//i
  cursor.endEditBlock ();

  bool growYes=chgl->fuse->isVisible ();
  chgl->mol->fuse();
  if (growYes) chgl->mol->grow();
  chgl->pause=false;

}

void Window::hideSearch(){
  replacementInfo->clear();
  suchbox->hide();
}

void Window::showSearch(){
  replacementInfo->clear();
  if (shxEditorDock->isVisible()&&ediVis) {
    suchbox->show();
    shxEditorDock->activateWindow();
    search->setFocus();
//    QTimer::singleShot(200,search,SLOT(setFocus()));
  }
  if (shxOutputDock->isVisible()&&oupVis) {
    suchbox2->show();
    shxEditorDock->activateWindow();
    search2->setFocus(Qt::OtherFocusReason);
  }
}

void Window::editorIsVisible(bool vis){ediVis=vis;}//isVisible is also true if widget is stacked behind an other
void Window::outputIsVisible(bool vis){
    oupVis=vis;
    if (vis){
        //xlPutor->setReadOnly(false);

    }


}

void Window::findNext(){
  replacementInfo->clear();
  suchbox->show();
  searchchanged(false,false);
}

void Window::findPrev(){
  replacementInfo->clear();
  suchbox->show();
  searchchanged(false,true);
}

void Window::findText(){
  replacementInfo->clear();
  searchchanged(true,false);
}

void Window::hideSearch2(){
  suchbox2->hide();
}

void Window::hideToolz(bool b){

editToolBar->setVisible(!b);
bottomToolBar->setVisible(!b);
fito->setVisible(!b);
vito->setVisible(!b);
update();
}

void Window::showSearch2(){
  if (shxOutputDock->isVisible()) {
    suchbox2->show();
    search2->setFocus(Qt::OtherFocusReason);
    QTimer::singleShot(200,search2,SLOT(setFocus()));
  }
}

void Window::findNext2(){
  suchbox2->show();
  searchchanged2(false,false);
}

void Window::findPrev2(){
  suchbox2->show();
  searchchanged2(false,true);
}

void Window::findText2(){
  searchchanged2(true,false);
}

void Window::updateLC(){
  QTextCursor tc = editor->textCursor();
  int l,c;
  l=tc.blockNumber()+1;
  c=tc.columnNumber()+1; 
  lineColumn->setText(QString("Line: <b>%1</b> | Col: <b>%2</b>").arg(l,5).arg(c,5));
  editor->update();
  lineColumn->update();
}

void Window::paintEvent(QPaintEvent *){
  static int ap=0;
  if (chgl->stereo_mode==1){ 
    int p=geometry().y()+chgl->y();
//    chgl->minus=(p%2)?-1:1;
    chgl->minus=(p%2)?1:-1;
    if (p!=ap) chgl->updateGL();
    ap=p;
  }
}

void Window::moveEvent(QMoveEvent *) {
    static int ap=0;
    if (chgl->stereo_mode==1){ 
      int p=geometry().y()+chgl->y();
      chgl->minus=(p%2)?-1:1;
      if (p!=ap) chgl->updateGL();
      ap=p;
    }
  }

void Window::searchchanged(bool current,bool back){
  QRegExp searchString = QRegExp(search->text(),Qt::CaseInsensitive);
  QPalette p = search->palette();
  p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
  QTextDocument *document;
  QTextCursor cursor;

  document = editor->document();
  cursor = editor->textCursor();

  if (!searchString.isEmpty()){
    if (cursor.hasSelection())
      cursor.setPosition((current|back)?cursor.anchor():cursor.position(), QTextCursor::MoveAnchor);
    QTextDocument::FindFlags options;
    if (back) options|=QTextDocument::FindBackward;
    cursor=document->find(searchString,cursor,options);
    if (cursor.isNull()){
      QTextCursor ac(document);
      ac.movePosition(options & QTextDocument::FindBackward
		      ? QTextCursor::End : QTextCursor::Start);
      cursor = document->find(searchString, ac,options);
      if (cursor.isNull()){
	p.setBrush(QPalette::Active,QPalette::Base,QColor(205,100,100));
	cursor = editor->textCursor();
	cursor.movePosition(QTextCursor::Start);
      }
    }
    if ((!cursor.isNull())&&(wholeLine->isChecked())){
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    }
  }
  else cursor.movePosition(QTextCursor::Start);

  editor->setTextCursor(cursor);
  search->setPalette(p);
}

void Window::replaceNext(){
  QRegExp searchString = QRegExp(search->text(),Qt::CaseInsensitive);
  QPalette p = search->palette();
  p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
  QTextDocument *document;
  QTextCursor cursor;

  document = editor->document();
  cursor = editor->textCursor();

  if (!searchString.isEmpty()){
    if (cursor.hasSelection()){
      cursor.beginEditBlock();
      if (wholeLine->isChecked()){
	cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      }
      cursor.deleteChar();
      if (replace->text().isEmpty())cursor.deleteChar();
      else cursor.insertText(replace->text());
      cursor.endEditBlock();
      cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
    }
    cursor=document->find(searchString,cursor);
    if (cursor.isNull()){
      QTextCursor ac(document);
      ac.movePosition(QTextCursor::Start);
      cursor = document->find(searchString, ac);
      if (cursor.isNull()){
	p.setBrush(QPalette::Active,QPalette::Base,QColor(205,100,100));
      }
    }
    if ((!cursor.isNull())&&(wholeLine->isChecked())){
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    }
  }
  else cursor.movePosition(QTextCursor::Start);
  editor->setTextCursor(cursor);
  search->setPalette(p);
  editor->centerCursor();
  return;
}

void Window::replaceAll(){
  QRegExp searchString = QRegExp(search->text(),Qt::CaseInsensitive);
  QPalette p = search->palette();
  p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
  QTextDocument *document;
  QTextCursor cursor;

  document = editor->document();
  cursor = editor->textCursor();
  int occurrence=0;
  cursor.beginEditBlock();
  cursor.movePosition(QTextCursor::Start);
  if (!searchString.isEmpty()){
    do {
      cursor=document->find(searchString,cursor);
      if (cursor.hasSelection()){
	cursor.beginEditBlock();
	if (wholeLine->isChecked()){
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
	}
	cursor.deleteChar();
	if (replace->text().isEmpty())cursor.deleteChar();
	else cursor.insertText(replace->text());
	cursor.endEditBlock();
	cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
	occurrence++;
      }
    }
    while (!cursor.isNull());
  }

  cursor = editor->textCursor();
  cursor.endEditBlock();
  editor->setTextCursor(cursor);
  search->setPalette(p);
  editor->centerCursor();
  if (occurrence)
    replacementInfo->setText(QString("Replaced %1 occurrencies of <b>'%2'</b> by <b>'%3'</b>.").arg(occurrence).arg(searchString.pattern()).arg(replace->text()));
  return;
}

void Window::replacePrev(){
  QRegExp searchString = QRegExp(search->text(),Qt::CaseInsensitive);

  QPalette p = search->palette();
  p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
  QTextDocument *document;
  QTextCursor cursor;

  document = editor->document();
  cursor = editor->textCursor();

  if (!searchString.isEmpty()){
    if (cursor.hasSelection()){
      cursor.beginEditBlock();
      if (wholeLine->isChecked()){
	cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      }
      cursor.deleteChar();
      if (replace->text().isEmpty())cursor.deleteChar();
      else cursor.insertText(replace->text());
      cursor.endEditBlock();
      cursor.setPosition(cursor.anchor(), QTextCursor::MoveAnchor);
    }
    cursor=document->find(searchString,cursor,QTextDocument::FindBackward);
    if (cursor.isNull()){
      QTextCursor ac(document);
      ac.movePosition(QTextCursor::End);
      cursor = document->find(searchString, ac,QTextDocument::FindBackward);
      if (cursor.isNull()){
	p.setBrush(QPalette::Active,QPalette::Base,QColor(205,100,100));
      }
    }
  }
  else cursor.movePosition(QTextCursor::Start);
  editor->setTextCursor(cursor);
  search->setPalette(p);
  editor->centerCursor();
  searchchanged(false,true);
  return;
}

void Window::searchchanged2(bool current,bool back){
  QRegExp searchString = QRegExp(search2->text(),Qt::CaseInsensitive);

  QPalette p = search2->palette();
  p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
  QTextDocument *document;
  QTextCursor cursor;
  document = xlPutor->document();
  cursor = xlPutor->textCursor();

  if (!searchString.isEmpty()){
    if (cursor.hasSelection())
      cursor.setPosition((current|back)?cursor.anchor():cursor.position(), QTextCursor::MoveAnchor);
    QTextDocument::FindFlags options;
    if (back) options|=QTextDocument::FindBackward;
    cursor=document->find(searchString,cursor,options);
    if (cursor.isNull()){
      QTextCursor ac(document);
      ac.movePosition(options & QTextDocument::FindBackward
		      ? QTextCursor::End : QTextCursor::Start);
      cursor = document->find(searchString, ac,options);
      if (cursor.isNull()){
	p.setBrush(QPalette::Active,QPalette::Base,QColor(205,100,100));
      }
    }
  }
  else cursor.movePosition(QTextCursor::Start);
  xlPutor->setTextCursor(cursor);
  search2->setPalette(p);
}

void Window::styleDialog(){
  EacDlg *eac = new EacDlg(chgl->mol);
  eac->exec();
  mole.showbonds = mole.connecting(mole.showatoms);//das ist notwendig weil es in eac ebenfalls aufgerufen wird und dort lbonds ueberschreibt.
  chgl->murx=1;
  chgl->setFocus(Qt::OtherFocusReason);
  chgl->show();
  chgl->raise();
  chgl->updateGL();
  delete eac;
}

void Window::effuenf(){
  chgl->murx=1;
  chgl->setFocus(Qt::OtherFocusReason);
  chgl->show();
  chgl->raise();
  chgl->updateGL();
}

void Window::selectInStructure(const QString &info){
  chgl->mol->selectedatoms.clear();
  for (int i=0; i<chgl->mol->showatoms.size(); i++){
    if (chgl->mol->showatoms.at(i).orginalLine.size()<30) continue;
    if (info.contains(chgl->mol->showatoms.at(i).orginalLine,Qt::CaseInsensitive)){
      chgl->mol->selectedatoms.append(chgl->mol->showatoms[i]);
      chgl->mol->selectedatoms.last().style=i;
    }
  }
  chgl->updateBondActions();
  chgl->updateGL();
}

void Window::findInStructure(const QString &info){
  int index=-1;
  for (int i=0; i<chgl->mol->showatoms.size(); i++){
    if (chgl->mol->showatoms.at(i).orginalLine.contains(info,Qt::CaseInsensitive)) {index=i;break;}
  }
  if (index!=-1){
    chgl->rotze=index;
    chgl->rCenter->show();
    chgl->mol->selectedatoms.clear();
    chgl->mol->selectedatoms.append(chgl->mol->showatoms[index]);
    chgl->mol->selectedatoms.last().style=index;
    chgl->updateBondActions();
    chgl->updateGL();
  }
}

void Window::jumpToAtom(int index){
  if ((chgl->inRenameMode)&&(enterRenameMode->isChecked())) {
    renameThisAtom(index);
    return;
  }
  if (chgl->inRenameMode) renamUpdate(false);
  QString label = mole.showatoms.at(index).orginalLine.left(80).trimmed();
//  qDebug()<<index<<mole.showatoms.at(index).Label<<label;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(label,cursor);
  if (cursor.isNull()){
    cursor = editor->textCursor();    
    cursor.movePosition(QTextCursor::Start);
    label.chop(10);
    cursor = document->find(label,cursor);
    if (cursor.isNull()) cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    label = mole.showatoms.at(index).orginalLine.left(80).simplified();
    cursor = document->find(label,cursor);
    if (cursor.isNull()) cursor = editor->textCursor();
  }
  cursor.movePosition(QTextCursor::StartOfLine);
  editor->setTextCursor(cursor);
  editor->centerCursor();
  cursor.clearSelection();
  editor->setTextCursor(cursor);
  editor->setFocus();
  if (chgl->toggleDockWidgets->isChecked()) return;
  shxEditorDock->show();
}

void Window::rufmode(){

  mole.einstellung->beginGroup("Labeling");
  mole.einstellung->setValue("AutoUnused",rufModeBox->isChecked());
  mole.einstellung->endGroup();
  bool inuse=isLabelInUse();
  if ((inuse)&&(rufModeBox->isChecked())&&((labelIndex==-1)||(indexIncr->isChecked()))){
    if (sufixBox->currentIndex()<0) sufixBox->setCurrentIndex(0);
    while (isLabelInUse()){
    sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
    updateLabel();
    }
    inuse=isLabelInUse();
  }else
  if ((rufModeBox->isChecked())){
    labelIndex=1;
    while (isLabelInUse()) {
       labelIndex++;
       if (labelIndex>999) {
         labelIndex=00;
         sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
         updateLabel();
       }
    }
    inuse=isLabelInUse(); 
    indexSpin->setValue(labelIndex); 
  }
    updateLabel();
}

void Window::changeElemetofLabel(const QString &pse){
  labelPSE=pse;
  bool inuse=isLabelInUse();
  if ((inuse)&&(rufModeBox->isChecked())&&((labelIndex==-1)||(indexIncr->isChecked()))){
    if (sufixBox->currentIndex()<0) sufixBox->setCurrentIndex(0);
    while (isLabelInUse()){
    sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
    updateLabel();
    }
    inuse=isLabelInUse();
  }else
  if ((rufModeBox->isChecked())){
    if (!nextLabel.contains("any"))labelIndex=1;
    while (isLabelInUse()) {
       labelIndex++;
       if (labelIndex>999) {
         labelIndex=00;
         sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
         updateLabel();
       }
    }
    inuse=isLabelInUse(); 
    indexSpin->setValue(labelIndex); 
  }
  if ((nextLabel.contains("any"))){
      QString altLabel="";
      for (int i=1; i<109; i++){
          if (pserbt[i]->isVisible()) {
              if (labelIndex<0) altLabel+=QString("%1%2").arg(pserbt[i]->text()).arg(labelSuffix);
              else altLabel+=QString("%1%2%3").arg(pserbt[i]->text()).arg(labelIndex).arg(labelSuffix);
              altLabel.append(", ");
          }
      }
      altLabel.remove(QRegExp(", $"));
      int lakoma = altLabel.lastIndexOf(", ");
      if (lakoma>1) altLabel.replace(lakoma,2," or ");
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>")
                         .arg(altLabel)
                         .arg("blue"));
  } else {
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>%3")
                         .arg(nextLabel)
                         .arg((inuse)?"red":"green")
                         .arg((inuse)?"<font size=\"-19\"><br>Label is already in use. You may change ResidueNr or Part.</font>":""));
}
  update();
}

void Window::changeIndexofLabel(int i){
  labelIndex=i;
  bool inuse=isLabelInUse();
  /*if ((inuse)&&(rufModeBox->isChecked())){
    labelIndex=1;
    while (isLabelInUse()) {
       labelIndex++;
    }
    inuse=isLabelInUse(); 
    indexSpin->setValue(labelIndex); 
  }*/
//  qDebug()<<inuse<<rufModeBox->isChecked()<<labelIndex<<indexIncr->isChecked();
  if ((inuse)&&(rufModeBox->isChecked())&&((labelIndex==-1)||(indexIncr->isChecked()))){
    if (sufixBox->currentIndex()<0) sufixBox->setCurrentIndex(0);
    while (isLabelInUse()){
    sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
    updateLabel();
    }
    inuse=isLabelInUse();
  }else
  if ((rufModeBox->isChecked())){
    if (!nextLabel.contains("any"))labelIndex=1;
    while (isLabelInUse()) {
       labelIndex++;
       if (labelIndex>999) {
         labelIndex=00;
         sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
         updateLabel();
       }
    }
    inuse=isLabelInUse(); 
    indexSpin->setValue(labelIndex); 
  }


  if ((nextLabel.contains("any"))){
      QString altLabel="";
      for (int i=1; i<109; i++){
          if (pserbt[i]->isVisible()) {
              if (labelIndex<0) altLabel+=QString("%1%2").arg(pserbt[i]->text()).arg(labelSuffix);
              else altLabel+=QString("%1%2%3").arg(pserbt[i]->text()).arg(labelIndex).arg(labelSuffix);
              altLabel.append(", ");
          }
      }
      altLabel.remove(QRegExp(", $"));
      int lakoma = altLabel.lastIndexOf(", ");
      if (lakoma>1) altLabel.replace(lakoma,2," or ");
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>")
                         .arg(altLabel)
                         .arg("blue"));
  } else {
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>%3")
                     .arg(nextLabel)
                     .arg((inuse)?"red":"green")
                     .arg((inuse)?"<font size=\"-19\"><br>Label is already in use. You may change ResidueNr or Part.</font>":""));
  }
  update();
}

void Window::changeSuffixofLabel(const QString &fix){
  labelSuffix=fix;
  bool inuse=isLabelInUse();
  if ((inuse)&&(rufModeBox->isChecked())&&((labelIndex==-1)||(indexIncr->isChecked()))){
    if (sufixBox->currentIndex()<0) sufixBox->setCurrentIndex(0);
    while (isLabelInUse()){
    sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
    updateLabel();
    }
    inuse=isLabelInUse();
  }else if((inuse)&&(rufModeBox->isChecked())&&(labelIndex!=-1)){
    updateLabel();
    inuse=isLabelInUse();
  }
  if ((nextLabel.contains("any"))){
      QString altLabel="";
      for (int i=1; i<109; i++){
          if (pserbt[i]->isVisible()) {
              if (labelIndex<0) altLabel+=QString("%1%2").arg(pserbt[i]->text()).arg(labelSuffix);
              else altLabel+=QString("%1%2%3").arg(pserbt[i]->text()).arg(labelIndex).arg(labelSuffix);
              altLabel.append(", ");
          }
      }
      altLabel.remove(QRegExp(", $"));
      int lakoma = altLabel.lastIndexOf(", ");
      if (lakoma>1) altLabel.replace(lakoma,2," or ");
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>")
                         .arg(altLabel)
                         .arg("blue"));
  } else {
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>%3")
		  .arg(nextLabel)
		  .arg((inuse)?"red":"green")
		  .arg((inuse)?"<font size=\"-19\"><br>Label is already in use. You may change ResidueNr or Part.</font>":""));
  }
  update();
}

void Window::updateLabel(){
  indexSpin->setValue(labelIndex);
//  if ((rufModeBox->isChecked())&&(!nextLabel.contains("any"))) labelIndex=1;
  bool inuse=isLabelInUse();
  if ((inuse)&&(rufModeBox->isChecked())){
    while (isLabelInUse()) {
       labelIndex++;
       if (labelIndex>999) {
         labelIndex=00;
         sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
         updateLabel();
       }
    }
    inuse=isLabelInUse(); 
    indexSpin->setValue(labelIndex); 
  }
  if ((nextLabel.contains("any"))){
      QString altLabel="";
      for (int i=1; i<109; i++){
          if (pserbt[i]->isVisible()) {
              if (labelIndex<0) altLabel+=QString("%1%2").arg(pserbt[i]->text()).arg(labelSuffix);
              else altLabel+=QString("%1%2%3").arg(pserbt[i]->text()).arg(labelIndex).arg(labelSuffix);
              altLabel.append(", ");
          }
      }
      altLabel.remove(QRegExp(", $"));
      int lakoma = altLabel.lastIndexOf(", ");
      if (lakoma>1) altLabel.replace(lakoma,2," or ");
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>")
                         .arg(altLabel)
                         .arg("blue"));
  } else {
      nextNameL->setText(QString("Next Label is:<br> <font size=\"+1\" color=\"%2\">%1</font>%3")
		  .arg(nextLabel)
		  .arg((inuse)?"red":"green")
		  .arg((inuse)?"<font size=\"-19\"><br>Label is already in use. You may change ResidueNr or Part.</font>":""));

  }
  update();
}

int Window::duplicates(){
    int was=0;
  if (search4duplicates->isChecked()){
    mole.duplicateAtoms.clear();
    for (int i =0; i<mole.asymm.size(); i++){
      if (mole.asymm.at(i).an<0) continue;//Q peaks don't have to be uniq!
        for (int j =i+1; j<mole.asymm.size(); j++){
	  if (mole.asymm.at(j).an<0) continue;//Q peaks don't have to be uniq!
            if ((mole.asymm.at(i).part==mole.asymm.at(j).part)&&
                    (mole.asymm.at(i).resiNr==mole.asymm.at(j).resiNr)&&
                    (mole.asymm.at(i).Label==mole.asymm.at(j).Label)){
                mole.duplicateAtoms.append(mole.asymm.at(i));
                mole.duplicateAtoms.last().part=666;
                mole.duplicateAtoms.append(mole.asymm.at(j));
                mole.duplicateAtoms.last().part=666;
                was|=(mole.asymm.at(i).an==0)?1:2;
            }
        }
    }
    editor->highlighter->highlightDupl(mole.duplicateAtoms);
  }
return was;
}

bool Window::isLabelInUse(){
  QStringList alab;
  for (int i =0; i<mole.asymm.size(); i++){
      alab.append(mole.asymm.at(i).Label);
  }
  QString nn;
  //wenn rufMode dann labelIndex auf 1 setzen und hochzaehlen bis (alab.contains(nn,Qt::CaseInsensitive)) false
  // sollte (labelIndex<0) sein dann suffix auf A und hochzaehlen
  if (labelIndex<0) nextLabel=QString("%1%2").arg(labelPSE).arg(labelSuffix);
  else nextLabel=QString("%1%2%3").arg(labelPSE).arg(labelIndex).arg(labelSuffix);
  if (!nextLabel.startsWith("any")) nextLabel=nextLabel.left(4);
  if ((partSpin->value()!=0)||(resiNrSpin->value()!=0)){
    nn=QString("%1_%3%4")
	    .arg(nextLabel)
	    .arg((resiNrSpin->value())?QString::number(resiNrSpin->value()):"")
	    .arg((partSpin->value())?QString::number((partSpin->value()<0)?
				    36+partSpin->value():
				    partSpin->value()+9,36):"");

  }else
  nn=nextLabel;
  return (alab.contains(nn,Qt::CaseInsensitive));
}

void Window::renameThisAtom(){
    QAction *action = qobject_cast<QAction *>(sender());
    int index=0;
    if (action)
      index=action->data().toInt();
    else return;
    if (index>mole.asymm.size()) return;
    QRegExp num=QRegExp("\\d+");
    if (mole.asymm.at(index).Label.contains(num)) {
      int p=mole.asymm.at(index).an;
      labelPSE=mole.pse(p);

      labelIndex=mole.asymm.at(index).Label.section(QRegExp("\\D+"),0,0,QString::SectionSkipEmpty).toInt();
      labelSuffix=mole.asymm.at(index).Label.section(num,1,1);
      labelSuffix.remove(QRegExp("[^A-Za-z']+"));
    }
    else{
      int p=mole.asymm.at(index).an;
      labelPSE=mole.pse(p);
      labelIndex=-1;
      labelSuffix=mole.asymm.at(index).Label;
      labelSuffix.remove(0,labelPSE.size());
      labelSuffix.remove(QRegExp("[^A-Za-z']+"));
    }

    partSpin->setValue(mole.asymm.at(index).part);
    resiNrSpin->setValue(mole.asymm.at(index).resiNr);
    resiResiClass->setText(mole.asymm.at(index).ResiClass);
    pserbt[mole.asymm.at(index).an+1]->setChecked(true);

    renamUpdate(true);
    updateLabel();

  }

void Window::renameRNchanged(int ii){
  QMap<int,QString> resNrClass;
  for (int i=0; i<resiFinder->count(); i++){
    QString resil=resiFinder->itemText(i);
    resil.remove(QRegExp("^RESI"));//remove resi at the beginning resi could be also a (STUPID) resi class
    QString resinr=resil;
    resil.remove(QRegExp("\\b\\d+\\b"));//remove the number which sepeated (e.g. not like CCF3)
    resinr.remove(resil.trimmed());//the rest should be the number
    int rn=resinr.toInt();
    resNrClass[rn]=resil.trimmed();
  }

  if ((rufModeBox->isChecked())&&(!nextLabel.contains("any"))) labelIndex=1;
  if (!ii) {
    resiResiClass->setEnabled(false);
    resiResiClass->setText("");
  }
  else {
    resiResiClass->setDisabled(resNrClass.contains(ii));  
    resiResiClass->setText(resNrClass[ii]);
  }
  updateLabel();

}

void Window::redoRename(){
  if (gehtMichAn2.size()&&(0!=gehtMichAn2.takeLast())){//redoDeleteAtoms
    deletedAtom.clear();
    for (int i=0; i<restoredAtom.size();i++)deletedAtom.append(restoredAtom.at(i));
    for (int i=0; i<restoredAtom.size();i++){
      mole.asymm.removeAt(mole.asymm.indexOf(restoredAtom.at(i)));
    }
    restoredAtom.clear();
    chgl->disSelection();
    if (chgl->fuse->isVisible())chgl->mol->grow();
    else  chgl->mol->fuse();

    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    }
    chgl->murx=1;
    chgl->updateGL();
    gehtMichAn.takeLast();
    return;
  }
  if (gehtMichAn.size()&&(0==gehtMichAn.takeLast()))return;
  MyAtom renamed;
  int renamindex;
  if (redoAtom.isEmpty()) return;
  renamindex = redoAtomIndex.takeLast();
  if (renamindex==-1){
    do {
      renamed = redoAtom.takeLast();
      undoAtom.append(renamed);
      mole.asymm.append(renamed);
      undoAtomIndex.append(renamindex);
    } while ((redoAtomIndex.size())&&(-1==(renamindex = redoAtomIndex.takeLast())));
  }else {

    renamed = redoAtom.takeLast();
    undoAtom.append(mole.asymm[renamindex]);
    undoAtomIndex.append(renamindex);

    mole.asymm[renamindex]=renamed;
  }
  updateLabel();
  chgl->disSelection();
  if (chgl->fuse->isVisible())chgl->mol->grow();
  else  chgl->mol->fuse();

  switch (duplicates()){
  case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
  case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  }chgl->murx=1;
  chgl->updateGL();
}

void Window::wasDelete(){
  static int changeDel=0;
  if (changeDel!=(deletedAtom.size()+restoredAtom.size())) gehtMichAn2.append(1);
  else gehtMichAn2.append(0);
  changeDel=deletedAtom.size()+restoredAtom.size();
}

void Window::wasRename(){
  if (altursize!=(redoAtom.size()+undoAtom.size()))gehtMichAn.append(1);
  else gehtMichAn.append(0);
  altursize=(redoAtom.size()+undoAtom.size());
}

void Window::undoRename(){
  int angang=0;
  if (gehtMichAn2.size()&&(0!=(angang=gehtMichAn2.takeLast()))){//undoDeleteAtoms
    restoredAtom.clear();
    for (int i=0; i<deletedAtom.size();i++)restoredAtom.append(deletedAtom.at(i));
    for (int i=0; i<deletedAtom.size();i++){
      mole.asymm.append(deletedAtom.at(i));
    }
    deletedAtom.clear();
    chgl->disSelection();
    if (chgl->fuse->isVisible())chgl->mol->grow();
    else  chgl->mol->fuse();

    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    } chgl->murx=1;
    chgl->updateGL();
    gehtMichAn.takeLast();
    gehtMichAn2.append(1);
    return;
  }
  angang=0;
  if (gehtMichAn.size()&&(0==(angang=gehtMichAn.takeLast()))){
    return;
  }
  if (angang>1){

    for (int i = 2; i<angang; i++ ){
      redoAtom.append(mole.asymm.takeLast());
      redoAtomIndex.append(-1);

    }

    updateLabel();
    chgl->disSelection();
    if (chgl->fuse->isVisible())chgl->mol->grow();
    else  chgl->mol->fuse();

    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    }chgl->murx=1;
    chgl->updateGL();

    return;
  }
  MyAtom renamed;
  int renamindex;
  if (undoAtom.isEmpty()) return;
  renamed = undoAtom.takeLast();
  renamindex = undoAtomIndex.takeLast();
  if (renamindex<0)redoAtom.append(renamed);
  else redoAtom.append(mole.asymm[renamindex]);
  redoAtomIndex.append(renamindex);

  mole.asymm[renamindex]=renamed;
  updateLabel();
  chgl->disSelection();
  if (chgl->fuse->isVisible())chgl->mol->grow();
  else  chgl->mol->fuse();

  switch (duplicates()){
  case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
  case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  }chgl->murx=1;
  chgl->updateGL();
}

void Window::renameThisAtom(int index){
//  QTime rzeit;
//  rzeit.start();
  if ((index<0)||index>=mole.asymm.size()){
    if (mole.showatoms.at(index).Label.startsWith('Q')){

      MyAtom ato=mole.showatoms.at(index);
      ato.Label=ato.Label.remove("'");
      ato.symmGroup=0;
      int nindex=mole.asymm.indexOf(ato);
      if (nindex!=-1 ){
/*     printf("%d %d %d %s %d %d\n %g %g %g <=> %g %g %g\n",index,nindex,mole.asymm.size()
		      ,ato.Label.toStdString().c_str(),ato.part,ato.resiNr
		      ,ato.frac.x,ato.frac.y,ato.frac.z,
		      mole.asymm.at(nindex).frac.x,mole.asymm.at(nindex).frac.y,mole.asymm.at(nindex).frac.z
		      ); //  */
    mole.asymm[nindex]=ato;
	index=nindex;
      }else return;
    }
    else{
      QMessageBox::information(0,"information","The Atom you have clicked is not present in the file (yet) and can't be renamed by this program!<br>You can try 'move ... here' in the context menu of this atom. ");
      return;
    }
  }
//  printf("1 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  bool warnicht=false;
  int dahin=-1,dohin=-1;

  QStringList alleLabels;
  QMap<QString,int> indexe;
  QMap<QString,int> werte;
  QString insertSuchString;
  bool b;
  for (int i=0; i<mole.asymm.size();i++){
    if (mole.asymm.at(i).resiNr!=resiNrSpin->value()) continue;
    if (mole.asymm.at(i).part!=partSpin->value()) continue;
    alleLabels.append(mole.asymm.at(i).Label);
    QString s=mole.asymm.at(i).Label;
    s.remove(mole.pse(mole.asymm.at(i).an),Qt::CaseInsensitive);
    s.remove(QRegExp("_.+"));
    s.remove(QRegExp("\\W+"));
    werte[mole.asymm.at(i).Label]=s.toInt(&b,36);
    indexe[mole.asymm.at(i).Label]=i;
  }
  QString nlab = nextLabel ;
  b= nlab.contains("any");
  QString nsn = nlab.replace("any",mole.pse(mole.asymm.at(index).an));
  if (b) nsn.remove(mole.pse(mole.asymm.at(index).an),Qt::CaseInsensitive);
  else nsn.remove(labelPSE,Qt::CaseInsensitive);
  if ((b)&&(mole.asymm.at(index).an)) nsn.remove(QRegExp("\\W+"));
  else {
    if (b) nsn.remove(QRegExp("\\D+"));
    else {
      if (mole.getOZ(labelPSE)) nsn.remove(QRegExp("\\W+"));
      else nsn.remove(QRegExp("\\D+"));
    }
  }
  int v=nsn.toInt(&b,36) ;
  int mindi=1000000;
  int maxdi=-1000000;
  for (int i=0; i<alleLabels.size();i++){
    int di=werte[alleLabels.at(i)]-v;
    if (di>=0)  {
      dahin=(di<mindi)? i  : dahin;
      mindi=(di<mindi)? di : mindi; 
    }
    if (di<0)  {
      dohin=(di>maxdi)? i  : dohin;
      maxdi=(di>maxdi)? di : maxdi; 
    }
  }
  if (dahin==-1)insertSuchString="HKLF";
  else insertSuchString=mole.asymm.at(indexe.value(alleLabels.at(dahin))).orginalLine.left(80).trimmed();
  if ((dahin==-1)&&(dohin!=-1))insertSuchString=mole.asymm.at(indexe.value(alleLabels.at(dohin))).orginalLine.left(80).trimmed();
  if (insertSuchString.startsWith('Q')) insertSuchString="HKLF";
  //qDebug()<<"suche nach: "<<insertSuchString;
  QString label = mole.showatoms.at(index).orginalLine.left(80).trimmed();
//  printf("2 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();

  //insert missing afix 0 at end
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find("HKLF",cursor);
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
  if (editor->parenthesis.at(cursor.blockNumber())){
    cursor.select(QTextCursor::WordUnderCursor);
    if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
      cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
    cursor.select(QTextCursor::WordUnderCursor);
    if ((cursor.selectedText().contains("RESI",Qt::CaseInsensitive))||(cursor.selectedText().contains("PART",Qt::CaseInsensitive)))
      cursor.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    cursor.clearSelection();
    cursor.insertText("AFIX 0 ! chuebsch want to keep this here!!\nREM #####\n");
    editor->updateAfix();
  }
  //insert missing afix 0 at End!

  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(label,cursor);
  if (cursor.isNull()){
    cursor = editor->textCursor();    
    cursor.movePosition(QTextCursor::Start);
    label.chop(10);
    cursor = document->find(label,cursor);
    if (cursor.isNull()) {
      cursor = editor->textCursor();
      warnicht=true;
    }
  }
  if (editor->parenthesis.at(cursor.blockNumber())){
    QMessageBox::information(0,"information",QString("ATOM %1 is in a AFIX construct and can't be renamed by this program!").arg(label.section(' ',0,0)));
    cursor.endEditBlock ();
    editor->setTextCursor(cursor);
    return;
  }
  if (warnicht) {
    QMessageBox::information(0,"information","The Atom you have clicked is not present in the file (yet) and can't be renamed by this program!");
    cursor.endEditBlock ();
    editor->setTextCursor(cursor);
    return;//hier muss eingefuegt werden: verhalten bei objecten die noch nicht in der datei stehen
  }
  QString vorherig,neues;
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  QString otxt= cursor.selectedText();
  while (cursor.selectedText().endsWith("=")) {
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
  }
  while (editor->parenthesis.at(cursor.blockNumber()+1)){
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
  }
  neues = vorherig= cursor.selectedText();
  //qDebug()<<otxt<<neues;
  if (tieType->itemData(tieType->currentIndex()).toInt()){
    QString asof=neues.section(QRegExp("\\s+"),5,5,QString::SectionSkipEmpty);
    asof.prepend(" ");
    QString nsof=QString(" %1").arg(tieType->itemData(tieType->currentIndex()).toInt()*(fabs(fmod(mole.asymm.at(index).sof_org,10))+(10*tieFVNr->value())),7,'f',5);
//    qDebug()<<asof<<nsof;
    neues.replace(asof,nsof);
    otxt.replace(asof,nsof);
    mole.asymm[index].sof_org=nsof.toDouble();
  }
  QString oldSfac=neues.section(QRegExp("\\s+"),1,1,QString::SectionSkipEmpty);
  int ii=sfac.indexOf(mole.getOZ(labelPSE));
  QString nl=QString("%1     ").arg(nextLabel,-4,' ');
  nl.replace("any",mole.pse(sfac.at(oldSfac.toInt()-1))).left(4);
  if (ii<0) {
    nl.replace("any",mole.pse(sfac.at(oldSfac.toInt()-1))).left(4);
  }
  else{
    neues.replace(QRegExp(QString("\\s%1\\s").arg(oldSfac)),QString(" %1 ").arg((ii+1)));
    otxt.replace(QRegExp(QString("\\s%1\\s").arg(oldSfac)),QString(" %1 ").arg((ii+1)));
  }
  nl=nl.left(4);
  neues.replace(0,4,nl);
  otxt.replace(0,4,nl);
//  printf("3 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  undoAtom.append(mole.asymm[index]);
  undoAtomIndex.append(index);
//  printf("3' => %d ms\n", rzeit.elapsed());  rzeit.restart();
  QString ol=mole.asymm[index].Label.section('_',0,0);
  QString oldH=ol;
  oldH.replace(QRegExp(QString("^%1").arg(mole.pse(mole.asymm[index].an)),Qt::CaseInsensitive),"H");
  mole.asymm[index].Label=nextLabel;
  mole.asymm[index].Label.replace("any",mole.pse(sfac.at(oldSfac.toInt()-1)));
  mole.asymm[index].Label=mole.asymm[index].Label.left(4);
  if (mole.asymm[index].an<0) {
      otxt=neues=
	    QString("%1 %2 %3 %4 %5 %6 0.05 \n")
	    .arg(mole.asymm[index].Label,-5)
	    .arg(ii+1)
	    .arg(mole.asymm.at(index).frac.x,9,'f',5)
	    .arg(mole.asymm.at(index).frac.y,9,'f',5)
	    .arg(mole.asymm.at(index).frac.z,9,'f',5)
	    .arg(mole.asymm.at(index).sof_org,9,'f',5);
  }
  mole.asymm[index].an=mole.getOZ(labelPSE);
  if (mole.asymm[index].an<0) {
    mole.asymm[index].an=sfac.at(oldSfac.toInt()-1);
  }
  QString newH=mole.asymm[index].Label;
  newH.replace(QRegExp(QString("^%1").arg(mole.pse(mole.asymm.at(index).an)),Qt::CaseInsensitive),"H");
  newH=newH.left(3);
  neues.replace(oldH,newH);

  //printf("altes H='%s' neues H='%s' %s %s\n",oldH.toStdString().c_str(),newH.toStdString().c_str(),mole.asymm[index].Label.toStdString().c_str(),mole.pse(mole.asymm.at(index).an).toStdString().c_str());
  mole.asymm[index].orginalLine=otxt.left(80).section('=',0);
//  printf(":%s:\n",mole.asymm[index].orginalLine.toStdString().c_str());
//  qDebug()<<"aa"<<mole.asymm[index].orginalLine;
  mole.asymm[index].ResiClass=resiResiClass->text();
  mole.asymm[index].resiNr=resiNrSpin->value();
  mole.asymm[index].part=partSpin->value();
  cursor.deleteChar();
  //cursor.deleteChar();//...this seems to cause errors but those emty lines are so ugly!
  //cursor.insertText(QString(" %1 has been renamed to %2. SHELXL ignores this line.").arg(ol).arg(mole.asymm[index].Label));
  QTextCursor wc = cursor; 
  if ((partSpin->value()!=0)||(resiNrSpin->value()!=0)){
  mole.asymm[index].Label=QString("%1_%3%4")
          .arg(nextLabel)
          .arg((resiNrSpin->value())?QString::number(resiNrSpin->value()):"")
          .arg((partSpin->value())?QString::number((partSpin->value()<0)?
                      36+partSpin->value():
                      partSpin->value()+9,36):"");
  mole.asymm[index].Label.replace("any",mole.pse(sfac.at(oldSfac.toInt()-1)));

  }
  cursor = editor->textCursor();    
  cursor.movePosition(QTextCursor::Start);
  editor->updateAfix();
  cursor = document->find(insertSuchString,cursor);
  if (cursor.isNull()){
    cursor = wc; //alte schreibposition setzen (wir haben atom nicht gefunden weil wir es gerade geloescht haben....   
    cursor.movePosition(QTextCursor::Down);
  }
  else if(insertSuchString!="HKLF"){
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    while (cursor.selectedText().endsWith("=")) {
      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
    }
    while (editor->parenthesis.at(cursor.blockNumber()+1)){
      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
    }
    cursor.movePosition(QTextCursor::Down); 
  }
  cursor.clearSelection ();
  cursor.movePosition(QTextCursor::StartOfLine);
  if ((insertSuchString=="HKLF")&&(mole.asymm[index].resiNr)) cursor.insertText(QString("RESI %1 %2\n")
		  .arg(mole.asymm[index].resiNr)
		  .arg(mole.asymm[index].ResiClass));
  if ((insertSuchString=="HKLF")&&(mole.asymm[index].part)) cursor.insertText(QString("PART %1 \n")
		  .arg(mole.asymm[index].part)
//		  .arg((tieType->itemData(tieType->currentIndex()).toInt())?
//			  QString::number((tieType->itemData(tieType->currentIndex()).toInt()*(10*tieFVNr->value()+1))):"")
		  );
  cursor.insertText(neues);
  if (!neues.endsWith("\n")) cursor.insertText("\n");
  if ((insertSuchString=="HKLF")&&(mole.asymm[index].part)) cursor.insertText("PART 0\n");
  if ((insertSuchString=="HKLF")&&(mole.asymm[index].resiNr)) cursor.insertText("RESI 0\n");
  if ((tieType->itemData(tieType->currentIndex()).toInt())&&(mole.asymm[index].part)&&(fvar.size()<tieFVNr->value())){
    //  !!!Neue Freie Variable anlegen!!!
    cursor.movePosition(QTextCursor::End);	
    cursor = document->find("FVAR",cursor,QTextDocument::FindBackward);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    while (cursor.selectedText().endsWith("=")) {
      cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);  
    }
    QString fvstr = cursor.selectedText().trimmed();
    fvstr.append(" 0.666 ");
    fvar.append(0.667);
    cursor.deleteChar();
    cursor.insertText(fvstr);
    updateTieFreeVar();
    // !!!!
  }
//qDebug()<<"redi";
 // printf("3! => %d ms\n", rzeit.elapsed());  rzeit.restart();
  chgl->murx=1;


  cursor.movePosition(QTextCursor::Start);
  int apo,po=cursor.position();
  apo=po;
  do {
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      if (cursor.selectedText().isEmpty()) {
	cursor.deleteChar();
      }
     //cursor.deleteChar();
      cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      apo=po;
      po=cursor.position();
  }
  while (!cursor.isNull()&&(po!=apo));

//  printf("3!a => %d ms\n", rzeit.elapsed());  rzeit.restart();
  cursor.endEditBlock ();
//  printf("3!b => %d ms\n", rzeit.elapsed());  rzeit.restart();
  if ((!indexIncr->isChecked())&&(labelIndex!=-1)) labelIndex++;
  else {
    if (sufixBox->currentIndex()<0) sufixBox->setCurrentIndex(0);
    sufixBox->setCurrentIndex((sufixBox->count()>sufixBox->currentIndex()+1)?sufixBox->currentIndex()+1:sufixBox->currentIndex());
  }
  updateLabel();

 // printf("3!0 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  chgl->disSelection();
//  printf("3!1 => %d ms\n", rzeit.elapsed());  rzeit.restart();
  if (chgl->fuse->isVisible())chgl->mol->grow();
  else  chgl->mol->fuse();

//  printf("3!2 => %d ms\n", rzeit.elapsed());  rzeit.restart();
    switch (duplicates()){
    case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
    case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
    } chgl->murx=1;
    editor->setTextCursor(cursor);
//    chgl->setRotationCenter(index);
 // printf("4# => %d ms\n", rzeit.elapsed());  rzeit.restart();
    chgl->updateGL();
 // printf("5 => %d ms\n", rzeit.elapsed());  rzeit.restart();
}

void Window::changeElement(){
    QAction *action = qobject_cast<QAction *>(sender());
    int index=0;
    if (action)
      index=action->data().toInt();
    else return;
    if (index>sfac.size()) return;
    if ((chgl->ImeanThisAtom>mole.asymm.size())||(chgl->ImeanThisAtom<0)) return;
    int das=chgl->ImeanThisAtom;
 //   bool alterMichi=rufModeBox->isChecked(); //DK suggested me to comment this out
 //   rufModeBox->setChecked(false);
 //   printf("new Element of %s is %s\n",mole.asymm.at(das).Label.toStdString().c_str(),mole.pse(sfac.at(index)).toStdString().c_str());
    QRegExp num=QRegExp("\\d+");
    if (mole.asymm.at(das).Label.contains(num)) {
      int p=sfac.at(index);
      labelPSE=mole.pse(p);

      labelIndex=mole.asymm.at(das).Label.section(QRegExp("\\D+"),0,0,QString::SectionSkipEmpty).toInt();
      labelSuffix=mole.asymm.at(das).Label.section(num,1,1);
      labelSuffix.remove(QRegExp("[^A-Za-z']+"));
    }
    else{
      int p=sfac.at(index);
      labelPSE=mole.pse(p);
      labelIndex=-1;
      labelSuffix=mole.asymm.at(das).Label;
      labelSuffix.remove(0,labelPSE.size());
      labelSuffix.remove(QRegExp("[^A-Za-z']+"));
    }

    partSpin->setValue(mole.asymm.at(das).part);
    resiNrSpin->setValue(mole.asymm.at(das).resiNr);
    resiResiClass->setText(mole.asymm.at(das).ResiClass);
    pserbt[sfac.at(index)+1]->setChecked(true);

    renamUpdate(false);
    updateLabel();
    renameThisAtom(das);
//    rufModeBox->setChecked(alterMichi);
}

void Window::zoomin(){
  chgl->zoom(0.1);
  chgl->updateGL();
} 

void Window::zoomout(){
  chgl->zoom(-0.1);
  chgl->updateGL();
}

void Window::calculator()
{
    calcmenu = new Calculator(mole.cell.wave,this);
}

void Window::dsr_gui()
{
    dsr = new DSRGui();
    dsr->setAttribute(Qt::WA_QuitOnClose);
    dsr->setMinimumSize(600, 500);
    dsr->show();
}

/*
void Window::zoomSlided(int z){
  double v=(z-50.0)/100.0;
  chgl->zoom(v);
  if (!sliderTimer->isActive()) sliderTimer->start(200);
  chgl->updateGL();
}

void Window::springSlider(){

  zoomer->setValue(50);
}
*/

void Window::rotl(){
  chgl->rotY(0.1);
  chgl->updateGL();
}   

void Window::rotr(){
  chgl->rotY(-0.1);
  chgl->updateGL();
}   

void Window::rotu(){
  chgl->rotX(0.1);
  chgl->updateGL();
}   

void Window::rotd(){
  chgl->rotX(-0.1);
  chgl->updateGL();
} 

void Window::insertFree(){
  if (mole.selectedatoms.size()!=2) return;
  
  QAction *action = qobject_cast<QAction *>(sender());
  if (action){
  int index=  action->data().toInt();
  QString frei=QString("FREE %1 %2 !experimental does not work with EQIV AND PART\n")
	  .arg(mole.selectedatoms.at(0).Label.section(QString::fromUtf8("»"),0,0))
	  .arg(mole.selectedatoms.at(1).Label.section(QString::fromUtf8("»"),0,0));

  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock();
  cursor = document->find("HKLF",cursor);
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.insertText(frei);
  cursor.endEditBlock();
  chgl->disConnectSelection(index);
  }
}

void Window::insertBind(){
  if (mole.selectedatoms.size()!=2) return;
  QString binde=QString("BIND %1 %2 !experimental does not work with EQIV AND PART\n")
	  .arg(mole.selectedatoms.at(0).Label.section(QString::fromUtf8("»"),0,0))
	  .arg(mole.selectedatoms.at(1).Label.section(QString::fromUtf8("»"),0,0));
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock();
  cursor = document->find("HKLF",cursor);
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.insertText(binde);
  cursor.endEditBlock();
  chgl->connectSelection();
}

void Window::updateStatusBar(const QString& s) {
  statusBar()->showMessage(s);
  update();

}

double Window::ueq(const Matrix m){
  double erg=0;
  erg+=m.m11*mole.cell.as*mole.cell.a*mole.cell.a*mole.cell.as;
  erg+=m.m12*mole.cell.as*mole.cell.a*mole.cell.b*mole.cell.bs;
  erg+=m.m13*mole.cell.as*mole.cell.a*mole.cell.c*mole.cell.cs;
  erg+=m.m21*mole.cell.bs*mole.cell.b*mole.cell.a*mole.cell.as;
  erg+=m.m22*mole.cell.bs*mole.cell.b*mole.cell.b*mole.cell.bs;
  erg+=m.m23*mole.cell.bs*mole.cell.b*mole.cell.c*mole.cell.cs;
  erg+=m.m31*mole.cell.cs*mole.cell.c*mole.cell.a*mole.cell.as;
  erg+=m.m32*mole.cell.cs*mole.cell.c*mole.cell.b*mole.cell.bs;
  erg+=m.m33*mole.cell.cs*mole.cell.c*mole.cell.c*mole.cell.cs;
  erg*=1/3.0;
  return erg;
}

/*
   double Window::getNumber(double v,const QList<double> fv,int afix, double uiso){
   if ((v<1.0)&&(afix))  {return (-v*uiso);}
   if (v<10.0)   {return (v);}
   if (v>=110.0) {return (v-110.0)*fv[10];}
   if (v>=100.0) {return (v-100.0)*fv[9];}
   if (v>=90.0)  {return (v-90.0)*fv[8];}
   if (v>=80.0)  {return (v-80.0)*fv[7];}
   if (v>=70.0)  {return (v-70.0)*fv[6];}
   if (v>=60.0)  {return (v-60.0)*fv[5];}
   if (v>=50.0)  {return (v-50.0)*fv[4];}
   if (v>=40.0)  {return (v-40.0)*fv[3];}
   if (v>=30.0)  {return (v-30.0)*fv[2];}
   if (v>=20.0)  {return (v-20.0)*fv[1];}
   if (v>=10.0)  {return (v-10.0);}  
   return 0.0;
   }*/

double Window::getNumber(double v, const QList<double> fv,int idx,int &fixFlag){
  double av=qAbs(v),res=0.0,var=1.0;
  int m=0;
  while ((-10*m+av) > 5){m++;}
  if (m>1) m=qMin(m,fv.size());
  if (m>1) var=fv.at(m-1);
  if (!m) res=v;
  else if (v>0) res=(av-(10*m))*var;
  else res=(av-(10*m))*(1.0-var);
  fvarCntr[m]++;
  if (m>0) fixFlag|=1<<idx; else if (fixFlag&1<<idx) fixFlag-=1<<idx;
  //printf("idx: %d shift %d  flag %d  m%d\n", idx,1<<idx,fixFlag,m);
  return res;
}

double Window::getNumber(double v, const QList<double> fv, double uiso){
  double av=qAbs(v),res=0.0,var=1.0;
  if ((v<-0.5)&& (v>-5.0)) return res = av * uiso ;
  int m=0;
  while ((-10*m+av) > 5){m++;}
  if (m>1) m=qMin(m,fv.size());
  if (m>1) var=fv.at(m-1);
  if (!m) return v;
  else if (v>=0) res=(av-(10*m))*var;
  else res=(av-(10*m))*(1.0-var);
  fvarCntr[m]++;
  return res;
}

int Window::isacommand(QString command){
  QStringList keywords;
  keywords << 
	  "ACTA"<<//0
	  "AFIX"<<//1
	  "MPLA"<<//2
	  "ANIS"<<//3
	  "BASF"<<//4
	  "BIND"<<//5
	  "BLOC"<<//6
	  "BOND"<<//7 AGS4 to MPLA
	  "BUMP"<<//8
	  "CELL"<<//9 
	  "CGLS"<<//10
	  "CHIV"<<//11
	  "CONF"<<//12
	  "CONN"<<//13
	  "DAMP"<<//14
	  "DANG"<<//15
	  "DEFS"<<//16
	  "DELU"<<//17
	  "DFIX"<<//18
	  "DISP"<<//19
	  "EADP"<<//20
	  "EGEN"<<//21
	  "END" <<//22
	  "EQIV"<<//23
	  "ESEL"<<//24
	  "EXTI"<<//25
	  "EXYZ"<<//26
	  "FEND"<<//27
	  "FLAT"<<//28
	  "FMAP"<<//29
	  "FRAG"<<//30
	  "FREE"<<//31
	  "FVAR"<<//32
	  "GRID"<<//33
	  "HFIX"<<//34
	  "HKLF"<<//35
	  "HOPE"<<//36
	  "HTAB"<<//37
	  "INIT"<<//38
	  "ISOR"<<//39
	  "LAST"<<//40
	  "LATT"<<//41
	  "LAUE"<<//42
	  "LIST"<<//43
	  "L.S."<<//44
	  "MERG"<<//45
	  "MOLE"<<//46
	  "MORE"<<//47
	  "MOVE"<<//48
	  "NCSY"<<//49
	  "OMIT"<<//50
	  "PART"<<//51
	  "PATT"<<//52
	  "PHAN"<<//53
	  "PHAS"<<//54
	  "PLAN"<<//55
	  "PSEE"<<//56
	  "REM"<< //57
	  "RESI"<<//58
	  "RTAB"<<//59
	  "SADI"<<//60
	  "SAME"<<//61
	  "SFAC"<<//62
	  "SHEL"<<//63
	  "SIMU"<<//64
	  "SIZE"<<//65
	  "SPEC"<<//66
	  "SPIN"<<//67
	  "STIR"<<//68
	  "SUMP"<<//69
	  "SWAT"<<//70
	  "SYMM"<<//71
	  "TEMP"<<//72
	  "TEXP"<<//73
	  "TIME"<<//74
	  "TITL"<<//75
	  "TREF"<<//76
	  "TWIN"<<//77
	  "UNIT"<<//78
	  "VECT"<<//79
	  "WPDB"<<//80
	  "WGHT"<<//81
	  "ZERR"<<//82
	  "XNPD"<<//83
	  "REST"<<//84
          "CHAN"<<//85
	  "RIGU"<<//86
	  "FLAP"<<//87
	  "RNUM"<<//88
	  "SOCC"<<//89
	  "PRIG"<<//90
	  "WIGL"<<//91
	  "RANG"<<//92
	  "TANG"<<//93
	  "ADDA"<<//94
	  "STAG"<<//95
	  "ATOM"<<//96PDB dummy commands ...
	  "HETA"<<//97
	  "SCAL"<<//98
	  "ABIN"<<//99
	  "ANSC"<<//100
	  "ANSR"<<//101
	  "NOTR"<<//102
	  "NEUT"<<//103
	  "TWST";//104<<
	  ;//
  QString c=command;
  c=c.toUpper();
  c.remove(QRegExp("_\\S*"));
  if (c.startsWith("+")) return 666;//file inclusion
  return keywords.indexOf(c);
}

QString Window::load_sheldrick(QString fileName){
  chgl->showHidden();
  machPlatz=false;
  chgl->neutrons=false;
  //chgl->neutrons=true;
  static QString maidenName="";
  infoKanal->clear();
  shxEditorDock->show();
  QFile shxf(fileName);
  wghtAct->setEnabled(false);
  QString problem;
  if (!shxf.open(QIODevice::ReadOnly|QIODevice::Text)) {QMessageBox::information(0,"information",QString("Can't open %1").arg(fileName)); return "ERROR!";}
  QString inhalt=shxf.readAll().replace('\0','~');;
  shxf.close();
  qPeakDock->hide();

  chgl->pause=true;
  int minp=1,maxp=0;
  bool qbeforehkl=false,hklf=false;
  enterRenameMode->setVisible(true);
  chgl->qPeakBonds->setVisible(false);
  chgl->shpm1->setVisible(false);
  mole.cell.symmops.clear();
  mole.cell.trans.clear();
  mole.asymm.clear();
  mole.sdm.clear();
  mole.showbonds.clear();
  mole.showatoms.clear();
  mole.selectedatoms.clear();

  mole.envi_sdm.clear();
  mole.contact.clear();
  mole.envi_sdm.clear();
  mole.contact.clear();
  mole.symmopsEQIV.clear();
  mole.labelEQIV.clear();
  mole.transEQIV.clear();
  mole.freeatoms.clear();
  mole.bindatoms.clear();
  for (int i=0; i<mole.knoepfe.size();i++) mole.knoepfe[i].neighbors.clear();
  mole.knoepfe.clear();
  mole.showbonds.clear();
  mole.selectedatoms.clear();
  //chgl->rotCenter();
  chgl->updateBondActions();
  mole.showatoms.clear();
  V3 nl(0,0,0);
  mole.cell.trans.append(nl);
  mole.cell.symmops.append(Matrix(1,0,0, 0,1,0, 0,0,1));
  mole.pmin=10000;
  mole.pmax=-10000;
  MyAtom newAtom;
  newAtom.part=0;
  newAtom.resiNr=0;
  newAtom.hidden=0;
  newAtom.symmGroup=0;
  //newAtom.ownStyle=false;
  //newAtom.style=3;
  //newAtom.rad=0.2;
  newAtom.afix=0;
  //newAtom.color=QColor("seagreen");
  bool virg=true;
  if (maidenName!=fileName) {
    labelIndex=1;
    nextLabel="C1";
    labelPSE="C";
    pserbt[6]->setChecked(true);
    renamDock->hide();
    updateLabel();
  }else{
    virg=false;
  }
  maidenName=fileName;
  int part=0,afix=0;
  double uiso=0.05;
  int lattice=1;
  int isoat=0,adpat=0,qpeaks=0;
  int afixparent=0;
  sfac.clear();
  fvar.clear();
  chgl->sfacMenu->clear();

  fvarCntr.clear();
  mole.transEQIV.clear();
  mole.symmopsEQIV.clear();
  mole.labelEQIV.clear();
  mole.freeatoms.clear();
  mole.bindatoms.clear();
  mole.envi_sdm.clear();
  chgl->labs.clear();
  chgl->enviPositions.clear();

  for (int i=1; i<109; i++) pserbt[i]->hide();
  bool notRefine = inhalt.contains("DISABLE_REFINE");
  runMenu->setDisabled(notRefine);
  packMenu->setDisabled(notRefine);
  if (notRefine) lattice=-1;
  QRegExp sep=QRegExp("\\s+");
  inhalt.replace(QRegExp("=\\s*[\\r\\n]+\\s{1,}"),"=");
  inhalt.replace(QRegExp("REM[^\\n]*\n"),"\n");
  inhalt.replace(QRegExp("![^\\n]*\n"),"\n");
  while (inhalt.contains(QRegExp("[\\n^]\\+[^\\n]*\n"))){
      QString incl=inhalt.section(QRegExp("[\\n^]\\+"),1,1);
      incl=incl.section('\n',0,0);
      QString pre=dirName.section('/',0,-2);
      //qDebug()<<pre+"/"+incl<<QFileInfo(pre+"/"+incl).exists()<<dirName;
      if (QFileInfo(pre+"/"+incl).exists()){
          QFile include(pre+"/"+incl);
          QString inst;
          if (include.open(QIODevice::ReadOnly|QIODevice::Text)) inst=include.readAll();
          inhalt.replace("+"+incl,inst);
      }
      else inhalt.remove("+"+incl);
  }
  QStringList lines = inhalt.split(QRegExp("[\\r\\n]"),QString::SkipEmptyParts);
  bool fragIgnore=false;
  for (int i=0; i<lines.size(); i++){
      if (lines.at(i).startsWith(" ")) continue;
    if (!lines.at(i).isEmpty()){
      newAtom.orginalLine=lines.at(i).section("=",0,0);
      lines[i].remove("=");
      int cmd=isacommand(lines.at(i).section(sep,0,0));
      //qDebug()<<cmd<<lines.at(i);
      QString resiSpez=lines.at(i).section(sep,0,0).section('_',1,1);
      if (cmd==30) fragIgnore=true;
      if (cmd==27) fragIgnore=false;
      if (!fragIgnore)
	switch (cmd) {
		case 1: 
			afix=lines.at(i).section(sep,1,1).toInt(); 
			newAtom.afix=afix;           
			break;
		case 5://bind
			{
			  if (notRefine) break;
			  MyBind aa;
                          aa.Lab1=lines.at(i).section(sep,1,1).toUpper();
                          aa.Lab2=lines.at(i).section(sep,2,2).toUpper();
			  mole.bindatoms.append(aa);
			}
			break;
		case 9:
			mole.cell.wave = lines.at(i).section(sep,1,1).toDouble();
			mole.cell.a = lines.at(i).section(sep,2,2).toDouble();
			mole.cell.b = lines.at(i).section(sep,3,3).toDouble();
			mole.cell.c = lines.at(i).section(sep,4,4).toDouble();
			mole.cell.al= lines.at(i).section(sep,5,5).toDouble();
			mole.cell.be = lines.at(i).section(sep,6,6).toDouble();
			mole.cell.ga = lines.at(i).section(sep,7,7).toDouble();
			mole.cellSetup();
			break;
		case 23:{
			  mole.decodeSymmCardEQIV(lines.at(i));
			}
			break;
		case 31:{//free
			  MyBind aa;
                          aa.Lab1=lines.at(i).section(sep,1,1).toUpper();
                          aa.Lab2=lines.at(i).section(sep,2,2).toUpper();
			  mole.freeatoms.append(aa);
			}
			break;
		case 32:{
			  editor->fvMinLine=i;
			  QStringList tok = lines.at(i).split(sep,QString::SkipEmptyParts);
			  for (int ifv=1; ifv<tok.size();ifv++){
			    fvar.append(tok.at(ifv).toDouble());
			  }
			}
			break;
		case 35: hklf=true;break;
		case 41:
			lattice=lines.at(i).section(sep,1,1).toInt();
			break;
                case 51:
			part = lines.at(i).section(sep,1,1).toInt();
			minp=(minp<part)?minp:part;
			maxp=(maxp>part)?maxp:part;

			break;
		case 58:
                        if (lines.at(i).section(sep,1,1).indexOf(QRegExp("^[0-9]+"))!=-1){
			newAtom.resiNr=lines.at(i).section(sep,1,1).toInt();
                        newAtom.ResiClass=lines.at(i).section(sep,2,2);
                    }else{
                        newAtom.resiNr=lines.at(i).section(sep,2,2).toInt();
                        newAtom.ResiClass=lines.at(i).section(sep,1,1);
                    }
                    break;
		case 62:{
			  QStringList tok = lines.at(i).split(sep,QString::SkipEmptyParts);
			  if ((tok.size()>4)&&(!tok.at(2).contains(QRegExp("[A-Za-z]")))) {
			    if ((tok.at(2).toDouble()==0)&&((tok.at(3).toDouble()==1))) {chgl->neutrons=true;}
			    sfac.append(mole.getOZ(tok.at(1)));
			    if (sfac.last()>-2) pserbt[sfac.last()+1]->show();
			    if ((virg)&&(sfac.last()==5)) pserbt[sfac.last()+1]->setChecked(true);
			    sfacBox->setFixedSize((sfac.size()+1)*52,70);
			  }
			  else {
			    for (int isf=1; isf<tok.size();isf++){
			      sfac.append(mole.getOZ(tok.at(isf)));
			      pserbt[sfac.last()+1]->show();
			      if ((virg)&&(sfac.last()==5)) pserbt[sfac.last()+1]->setChecked(true);
			      sfacBox->setFixedSize((sfac.size()+1)*52,70);
			    }
			  }
			}
			break;
		case 69:{
			  QStringList forml= lines.at(i).split(sep);
			  int fvix=4;
			  while (forml.size()>fvix){
			    int m=forml.at(fvix).toInt();			  
			    fvarCntr[m]++;
			    fvix+=2;
			  }
			}
			break;
		case 71:
			mole.decodeSymmCard(lines.at(i).section(" ",1,130));
			break;
		case 75:
			title = QString ("'%1'@ %2")
				.arg(lines.at(i).section(" ",1,30).simplified())
				.arg(fileName.section('/', -1));
			break;
		case 78:
			unitAlt = lines.at(i) ;	
			mole.applyLatticeCentro(lattice);
			break;
		case 81:
			wghtAct->setEnabled(true);
			break;
		case 82:
			mole.cell.Z= lines.at(i);
			break;
		case 103:
		//	qDebug()<<"Neutrons!";

			chgl->neutrons=true;
			break; 
		case -1:{//an atom or an error!
        if ((newAtom.afix>170)&&((newAtom.afix<180))) break;
			  QStringList tok = lines.at(i).split(sep,QString::SkipEmptyParts);
			  newAtom.part=part;
                          if (newAtom.resiNr<0){
                              newAtom.resiNr=0;
                              newAtom.ResiClass="";
                          }
			  if (tok.size()==7){
			    if ((newAtom.part!=0)||(newAtom.resiNr!=0)){
			      newAtom.Label=QString("%1_%3%4")
				      .arg(tok.at(0))
				      .arg((newAtom.resiNr)?QString::number(newAtom.resiNr):"")
				      .arg((newAtom.part)?QString::number((newAtom.part<0)?
							      36+newAtom.part:
							      newAtom.part+9,36):"");
			    }else newAtom.Label=tok.at(0);
                            int fac=tok.at(1).toInt()-1;
                            newAtom.fixFlag=0;
                            newAtom.an=((fac<0)||(fac>=sfac.size()))?-2:sfac.at(fac);
			    newAtom.frac.x = getNumber(tok.at(2).toDouble(),fvar,0,newAtom.fixFlag);
			    newAtom.frac.y = getNumber(tok.at(3).toDouble(),fvar,1,newAtom.fixFlag);
			    newAtom.frac.z = getNumber(tok.at(4).toDouble(),fvar,2,newAtom.fixFlag);
			    newAtom.sof_org=tok.at(5).toDouble();
			    newAtom.sof= getNumber(tok.at(5).toDouble(),fvar,3,newAtom.fixFlag);
			    newAtom.uf.m11= getNumber(tok.at(6).toDouble(),fvar,uiso);
			    newAtom.uf.m22=-666.0;
			    newAtom.uf.m33=tok.at(6).toDouble();
			    newAtom.peakHeight=-666;
                            if (newAtom.an==-66) newAtom.peakHeight=9999.99;
			    newAtom.afixParent=-1;
			    if (newAtom.an>0) afixparent=mole.asymm.size();
			    else newAtom.afixParent=afixparent;
			    mole.asymm.append(newAtom);
			    isoat++;
			  }
			  if (tok.size()==8) {
			    newAtom.Label=tok.at(0);
                            if (newAtom.Label.startsWith('Q',Qt::CaseInsensitive)) {
			      newAtom.an=-1;
			      newAtom.resiNr=-1;
			      newAtom.ResiClass="Q-Peak";
			      qpeaks++;
			    }
			    else {
                                int fac=tok.at(1).toInt()-1;
                              newAtom.an=((fac<0)||(fac>=sfac.size()))?-2:sfac.at(fac);

			      isoat++;
			    }
			    newAtom.fixFlag=0;
                            newAtom.frac.x = tok.at(2).toDouble();
			    newAtom.frac.y = tok.at(3).toDouble();
			    newAtom.frac.z = tok.at(4).toDouble();
			    newAtom.sof_org=tok.at(5).toDouble();
			    newAtom.sof= getNumber(tok.at(5).toDouble(),fvar,3,newAtom.fixFlag);
			    newAtom.uf.m11= getNumber(tok.at(6).toDouble(),fvar,uiso);			    
			    newAtom.uf.m22=-666.0;
			    newAtom.uf.m33= tok.at(6).toDouble();
			    newAtom.peakHeight=tok.at(7).toDouble();
                           if (!hklf) qbeforehkl=true;
                            if(newAtom.an==-1){
                                mole.pmin=qMin(mole.pmin, newAtom.peakHeight);
                                mole.pmax=qMax(mole.pmax, newAtom.peakHeight);
                            }
			    chgl->qPeakBonds->setVisible(true);

			    showQPeakLegend->setVisible(true);
			    newAtom.afixParent=-1;
			    mole.asymm.append(newAtom);

			  }
			  if (tok.size()==12){
			    if ((newAtom.part!=0)||(newAtom.resiNr!=0)){
			      newAtom.Label=QString("%1_%3%4")
				      .arg(tok.at(0))
				      .arg((newAtom.resiNr)?QString::number(newAtom.resiNr):"")
				      .arg((newAtom.part)?QString::number((newAtom.part<0)?
							      36+newAtom.part:
							      newAtom.part+9,36):"");
			    }else newAtom.Label=tok.at(0);
                            int fac=tok.at(1).toInt()-1;
                            newAtom.an=((fac<0)||(fac>=sfac.size()))?-2:sfac.at(fac);
			    newAtom.frac.x = getNumber(tok.at(2).toDouble(),fvar,0,newAtom.fixFlag);
			    newAtom.frac.y = getNumber(tok.at(3).toDouble(),fvar,1,newAtom.fixFlag);
			    newAtom.frac.z = getNumber(tok.at(4).toDouble(),fvar,2,newAtom.fixFlag);
			    newAtom.sof_org=tok.at(5).toDouble();
			    newAtom.sof= getNumber(tok.at(5).toDouble(),fvar,3,newAtom.fixFlag);
			    newAtom.uf.m11 = getNumber(tok.at(6).toDouble(),fvar,4,newAtom.fixFlag);
			    newAtom.uf.m22 = getNumber(tok.at(7).toDouble(),fvar,5,newAtom.fixFlag);
			    newAtom.uf.m33 = getNumber(tok.at(8).toDouble(),fvar,6,newAtom.fixFlag);
			    newAtom.uf.m32 = newAtom.uf.m23 = getNumber(tok.at(9).toDouble(),fvar,7,newAtom.fixFlag);
			    newAtom.uf.m31 = newAtom.uf.m13 = getNumber(tok.at(10).toDouble(),fvar,8,newAtom.fixFlag);
			    newAtom.uf.m21 = newAtom.uf.m12 = getNumber(tok.at(11).toDouble(),fvar,9,newAtom.fixFlag);
			    newAtom.peakHeight=-666;
			    uiso=ueq(newAtom.uf);
			    if (newAtom.an>0) afixparent=mole.asymm.size();
			    newAtom.afixParent=-1;
			    mole.asymm.append(newAtom);
			    adpat++;
			  }
			}
			break;
	}


    }

  }
  mole.qboMax=1.67;
  /*for (int i=0; i<sfac.size();i++){
    if (sfac.at(i)<0) continue;
    double r=0.017*mole.Kovalenz_Radien[sfac.at(i)];
    mole.qboMax=qMax(mole.qboMax,r);
    //mole.qboMin=qMin(mole.qboMin,r);
  }
  printf("%g\n",mole.qboMax);*/
  //qDebug()<<mole.qboMax;
  QString info;
  for (int ifv=0; ifv<fvar.size();ifv++){
    if (ifv==0) {
      info.append("OSF: ");
      info.append(QString("<font color=red>%1</font>").arg(fvar.at(ifv)));
    }
    else {
      info.append(QString("FVAR<font color=blue>%1</font>: ").arg(ifv+1));
      if (fvarCntr.value(ifv+1)<1)info.append(QString("<font color=red>%1</font>(Never Used!)").arg(fvar.at(ifv)));
      else info.append(QString("<font color=red>%1</font>(used %2 time%3.)").arg(fvar.at(ifv))
		      .arg(fvarCntr.value(ifv+1)).arg((fvarCntr.value(ifv+1)>1)?"s":""));
    }
    if ((ifv+1)<fvar.size()) info.append(", ");
    info.append("<br>");
  }
  fvarinfo=QString("<b>Free variables:</b> %1").arg(info);
  if (minp<0) {
    chgl->shpm1->setVisible(true);
    chgl->highlightParts->setVisible(true);
  }
  if (maxp>0) chgl->highlightParts->setVisible(true);
  mole.qbeforehkl=qbeforehkl;
  actionUniq->setDisabled(mole.qbeforehkl); 
  for (int i=0;i<mole.asymm.size();i++){ 

    if ((mole.asymm.at(i).uf.m22==-666.0) ){
      mole.asymm[i].uc.m11=mole.asymm[i].uc.m22=mole.asymm[i].uc.m33=mole.asymm[i].uf.m11;
      mole.asymm[i].uc.m12=mole.asymm[i].uc.m13=mole.asymm[i].uc.m23=mole.asymm[i].uc.m21=mole.asymm[i].uc.m31=mole.asymm[i].uc.m32=0.0;

    }
    else mole.Uf2Uo(mole.asymm.at(i).uf,mole.asymm[i].uc);
    mole.frac2kart(mole.asymm.at(i).frac,mole.asymm[i].pos);
    mole.showatoms.append(mole.asymm.at(i));
  }
  if (mole.asymm.size()){
      updateStatusBar("analysing structure...");

//  QTime zeit; zeit.start();
    if ((!notRefine)&&(mole.asymm.size()<350)) mole.packer(mole.sdmcompleter());
 // printf(">>%d\n",zeit.elapsed());zeit.restart();
    infoKanalNews(mole.Fragments);
    bool growYes=chgl->fuse->isVisible ();
    bool fuseYes=chgl->grow->isVisible ();
    chgl->fuse->setVisible(true);
    chgl->grow->setVisible(false);
    mole.loadSettings();
    updateStatusBar("finding bonds...");

  //printf(">>%d\n",zeit.elapsed());zeit.restart();
    mole.showbonds = mole.connecting(mole.showatoms);
  //printf(">>%d\n",zeit.elapsed());zeit.restart();
     chgl->setMolecule(&mole);
    chgl->mol->fuse();
    if ((!notRefine)&&(mole.asymm.size()<350)) {
        if (((!fuseYes)&&(growYes))||(((!fuseYes)&&(!growYes))&&(!startFused))) chgl->mol->grow();
    else {
      chgl->fuse->setVisible(false);
      chgl->grow->setVisible(true);
    }
    }
    else {
      if ((mole.asymm.size()>350)&&(chgl->dratpause==80))
	chgl->dratpause=300;
//      printf("chgl->dratpause=%d \n",chgl->dratpause);
      chgl->fuse->setVisible(false);
      chgl->grow->setVisible(false);   

    }
    infoKanalNews(mole.HumanSymmetry);
    //if (chgl->bas) glDeleteLists(chgl->bas,10);
    updateStatusBar("drawing display lists...");

  chgl->rehide();
    chgl->initLists();
    updateStatusBar("");
  }
  chgl->pause=false;
  chgl->updateGL();


  urs=V3(0,0,0);int gt=0;
  for (int i=0; i<mole.showatoms.size();i++)
    if (mole.showatoms.at(i).an>-1) {urs+=mole.showatoms.at(i).frac;gt++;}
  urs*=1.0/gt;
  urs=V3(1,1,1)-1.0*urs;
  mole.frac2kart(urs,urs);
  mode=0;
  qpv->cutoff=0;//qMin(0.0,mole.pmin);
  qpv->setMin(mole.pmin);
  qpv->setMax(mole.pmax);
  if (qpeaks) qPeakDock->show();
  QString fouName=fileName;
  fouName.chop(4);
  fouName.append(".fcf");
  
  if (!fxle->loadFouAndPerform(fouName.toLocal8Bit())) {
    //if (fxle->doMaps->isChecked()) infoKanalNews(QString("<font color=red>Could not load %1!</font><br>").arg(fouName));
    fxle->deleteLists();
    fxle->n1=fxle->n2=fxle->n3=fxle->n5=0;
}
//printf("fxle N5 %d\n",fxle->n5);
  if (fxle->doMaps->isChecked()){
      mapcontrol->setVisible(true);
}
  fxle->doMaps->show();
  QMap<int,double> unit;
  for (int i=0; i<mole.asymm.size(); i++){
    if (mole.asymm.at(i).an>=0) unit[mole.asymm.at(i).an]+= mole.asymm.at(i).sof * mole.cell.symmops.size();
  }
  unitNeu="UNIT";
  for (int i = 0; i < sfac.size() ; i++){
    if (unit.value(sfac.at(i))>9999)
    unitNeu.append(QString(" %1").arg((int)unit.value(sfac.at(i)))); else
    unitNeu.append(QString(" %1").arg(unit.value(sfac.at(i)),0,'g',4));
    if (unitNeu.split('\n').last().size()>76) unitNeu.append("=\n    ");
  }
  chgl->toggleDockWidgets->setVisible(true);
  astdlg->setDisabled(false);
//  chgl->hideh->setVisible( (sfac.indexOf(0)!=-1) );
  toggDocWid(false);
  sinaAct->setVisible(adpat!=0);
  if (!mole.asymm.size()) title.append("!!Empty!!");

  refineAct->setVisible(!notRefine);
  refineAct->setEnabled(!notRefine);

  refineActAnis->setVisible(!notRefine);
  refineActAnis->setEnabled(!notRefine);
  for (int k=0; k<sfac.size();k++){
    if (sfac.at(k)<0) continue;
    QAction *a=chgl->sfacMenu->addAction(mole.pse(sfac.at(k)),this,SLOT(changeElement()));
    a->setData(k);
  }
  int td=0;
  if (!notRefine) switch (td=duplicates()){
  case 1:infoKanalNews("<font color=orange >Some hydrogen atoms have identical names please check.</font>");break;
  case 2:infoKanalNews("<font color=red >Some non-hydrogen atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  case 3:infoKanalNews("<font color=red >Some atoms have identical names. YOU HAVE TO CHECK BEFORE REFINING!</font>");break;
  }
/*
  QCursor cc=QCursor(QPixmap(":icons/test4.png"),1,1);
  QApplication::setOverrideCursor(cc);
  */
 //Flack x parameter = 
  QString listName=fileName;
  listName.replace(QRegExp(".ins$"),".lst");
  listName.replace(QRegExp(".res$"),".lst");
  QFile lstf(listName);
  if (lstf.open(QIODevice::ReadOnly|QIODevice::Text)){
    QString liste=QString(lstf.readAll());
    while (liste.contains(" REM")) liste=liste.replace(QRegExp("\n\\s+REM[^\n]*\n",Qt::CaseInsensitive),"\n");
  //  liste=liste.replace(QRegExp("\n REM[^\n]*\n",Qt::CaseInsensitive),"\n");//das muss man 2 mal machen da die newline am anfang schon weggelesen ist.
    if (liste.contains("Flack x parameter =")){
      QString flackS=liste.section("Flack x parameter =",1).section('\n',0,0);
      double flackX=flackS.split(QRegExp("\\s+"),QString::SkipEmptyParts).first().toDouble();
      double flackXesd=flackS.split(QRegExp("\\s+"),QString::SkipEmptyParts).last().toDouble();
      infoKanalNews(QString("SHELXL-list file reports <b>Flack</b> x parameter:<b> %1 &plusmn; %2</b>").arg(flackX).arg(flackXesd)); 
    }
    //Flack x =

    if (liste.contains("Flack x =")){
      QString flackS=liste.section("\n Flack x = ",1).section('\n',0,1);
      QString flackX=flackS.split(QRegExp("\\s+"),QString::SkipEmptyParts).first();

      QString parsonsZ=flackS.section('\n',1,1);
      if (parsonsZ.isEmpty()){
	infoKanalNews(QString("SHELXL-list file reports <b>Flack</b> x parameter:<b> %1 </b>").arg(flackX)); 
      
      }
      else{
	parsonsZ=parsonsZ.split(QRegExp("\\s+"),QString::SkipEmptyParts).first();
    infoKanalNews(QString("SHELXL-list file reports <br><b>Flack</b> x parameter:<b> %1 </b><br><b>Parsons</b> parameter:<b>  %2</b>")
			.arg(flackX).arg(parsonsZ));
      }
    }
    lstf.close();
  }
  QString fcfname = fileName;
  fcfname.replace(QRegExp(".res$|.ins$",Qt::CaseInsensitive),".fcf");

  fcvsfoAct->setEnabled(QFile::exists(fcfname));
  chgl->showHidden();
  return title;
}//shelx

void Window::toggDocWid(bool weg){
  if (weg) {
    shxEditorDock->hide();
    shxOutputDock->hide();
    dock->hide();
    renamDock->hide();
    histDock->hide();
    chgl->toggleDockWidgets->setText("Show Text Windows");
  }
  else{
    shxEditorDock->show();
    histDock->show();
    dock->show();
    chgl->toggleDockWidgets->setText("Hide Text Windows");
  }
  update();
  chgl->updateGL();
}

void Window::convergeWght(){
/*! \brief Update WHGT scheme and refine in a loop until scheme has converged or a maximum of runs has exceeded.*/
  if (mole.asymm.isEmpty()) return;
  dirName.replace(QRegExp(".ins$",Qt::CaseInsensitive),".res");
  fileSave();
  cycls=0;
  xlPutor->clear();
  //xlPutor->highlighter->setOutputStyle();
  shxOutputDock->show();
  shxOutputDock->raise();
  stoppRefinement->show();
  loadRes->show();
  QObject::disconnect(stoppRefinement, 0, this, 0);
  refineButtonBox->hide();
  shxEditorDock->hide();
  dock->hide();
  histDock->hide();
  renamDock->hide();
  update();
  runXLWGHT();
 }

  void Window::runXLWGHT() {
  update();
  QString dn=dirName.section('/',0,-2),
	  argument=dirName.section('/',-1);
  argument.remove(QRegExp(".res$|.ins$",Qt::CaseInsensitive));
  QStringList al;
  al.append(argument);
  al+=shelxlArgs.split(' ',QString::SkipEmptyParts);
  shelxl = new QProcess(this);
  shelxl->setWorkingDirectory(dn);                           // updateXLput()
  stoppRefinement->show();
  QObject::disconnect(stoppRefinement, 0, this, 0);
  refineButtonBox->hide();
  connect(shelxl,SIGNAL(readyReadStandardOutput ()),this,SLOT(updateXLput()));
  connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(updateXLput()));
  connect(shelxl,SIGNAL(finished(int,QProcess::ExitStatus)),this ,SLOT(runXLWGHT()));
  connect(shelxl,SIGNAL(error(QProcess::ProcessError)),this,SLOT(updateXLput())); 
  shxOutputDock->show();
  shxOutputDock->raise();
  QFile test;
  QStringList alltest;
  QString alls;
  test.setFileName(dirName);
  QStringList wghts;
  if (test.exists()&&test.size()){
    test.open(QIODevice::ReadOnly|QIODevice::Text);
   alls=test.readAll();
   alltest=alls.split('\n');
   wghts=alltest.filter(QRegExp("^WGHT.*$",Qt::CaseInsensitive));
   if (wghts.size()>1) alls.replace(wghts.at(0),wghts.at(1));
   else if (wghts.size()) alls.replace(wghts.at(0),"WGHT 0.2!default");
    for (int i=0; i<wghts.size();i++) {
      wghts[i].remove(QRegExp("\\b0\\.[0]+\\b"));
      wghts[i].remove(QRegExp("\\B[0]+\\b"));
      wghts[i] = wghts.at(i).simplified();}
    test.close();
    test.open(QIODevice::WriteOnly|QIODevice::Text);
    test.write(alls.toLatin1());    
    test.close();
  } 
  xlPutor->ListFile=false;
  QString s=xlPutor->toPlainText();
  s.append(QString("############################\n%1\n%2\nConverged: %3\nafter %4 runs.\n############################\n")
		  .arg((wghts.size()>0)?wghts.at(0):"WGHT 0.2!default")
		  .arg((wghts.size()>1)?wghts.at(1):"WGHT 0.2!default")
		  .arg(((wghts.size()>1)&&(wghts.at(0)==wghts.at(1)))?"Yes":"No")
		  .arg(cycls)); 
  xlPutor->setPlainText(s);

  if ((wghts.size()>1)&&(wghts.at(0)==wghts.at(1))) {
    stoppRefinement->hide(); 
    refineButtonBox->show();
    loadRes->setEnabled((s.contains("for all data"))||(s.contains("Converged: Yes")));
    QTextCursor tc = xlPutor->textCursor();
    tc.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    xlPutor->setTextCursor(tc);
    xlPutor->ensureCursorVisible();
    update();
    return;
  }
  if (cycls>9) {
    stoppRefinement->hide(); 
    refineButtonBox->show(); 
    loadRes->setEnabled((s.contains("for all data")));
    QTextCursor tc = xlPutor->textCursor();
    tc.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    xlPutor->setTextCursor(tc);
    xlPutor->ensureCursorVisible();
    update();
    return;
  }
    if (dirName.contains(QRegExp(".res$",Qt::CaseInsensitive))){
      QString insname =dirName;
      insname.replace(QRegExp(".res$",Qt::CaseInsensitive),".ins");

      QString fcfname = dirName;
      fcfname.replace(QRegExp(".res$",Qt::CaseInsensitive),".fcf");
      QString fcfname2 = dirName;
      fcfname2.replace(QRegExp(".res$",Qt::CaseInsensitive),".2fcf");
      if (QFileInfo(dirName).lastModified()>=QFileInfo(insname).lastModified()){
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
	QFile::copy(insname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(insname);
	QFile::copy(dirName,insname);
	QFile::copy(fcfname2,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(fcfname2);
	QFile::copy(fcfname,fcfname2);
      }
      else {
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
	QFile::copy(insname,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(insname);
	QFile::copy(dirName,insname);

	QFile::copy(fcfname2,QString("%1/%4saves/%2_%3_.fcf").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));
	QFile::remove(fcfname2);
	QFile::copy(fcfname,fcfname2);
      }
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
      QFile::copy(dirName,QString("%1/%4saves/%2_%3_.ins").arg(dn).arg(argument).arg(vers,2,36,QLatin1Char('0')).arg(PROGRAM_NAME));

    }

    shxOutputDock->setWindowTitle(QString("SHELXL OUTPUT: %1").arg(argument));
    shelxl->start(shelxlPath,al);
    cycls++;
  }

void Window::updateWght(){
  if (mole.asymm.isEmpty()) return;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();
  cursor.movePosition(QTextCursor::End);
  cursor = document->find("WGHT",cursor,QTextDocument::FindBackward);
 if (cursor.isNull()) {
   cursor = editor->textCursor();
   cursor.endEditBlock ();
   return;
 } 
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  QString newWeight= cursor.selectedText();
  cursor.clearSelection();

  cursor.movePosition(QTextCursor::Up);

  cursor = document->find("WGHT",cursor,QTextDocument::FindBackward);

  cursor.movePosition(QTextCursor::StartOfLine);
  cursor.clearSelection();
  cursor.insertText(" ");
  cursor.movePosition(QTextCursor::Down);
  cursor.insertText(newWeight);
  cursor.insertText("\n");

  cursor.endEditBlock ();
  editor->setTextCursor(cursor);
  editor->centerCursor () ;
  cursor.clearSelection ();
  editor->setFocus () ;
  update();
}

void Window::fixIt(){
  if (mole.asymm.isEmpty()) return;
  QDialog *fixDlg = new QDialog(this);
  QGroupBox *gpx =new QGroupBox("Checked parameters will not be refined.",fixDlg);
  QVBoxLayout *vbl = new QVBoxLayout();
  QList<QCheckBox *> alleboxen;
  QSignalMapper *map = new QSignalMapper(this);
  int allIx=0;
  for (int i=0; i<mole.asymm.size(); i++){
     allIx=10*i;
     if (mole.asymm.at(i).an<0) continue;
     QGroupBox *gpx2 = new QGroupBox(mole.asymm.at(i).Label,fixDlg);
     QList<QCheckBox *> boxen;
     gpx2->setAlignment(Qt::AlignRight);
     QVBoxLayout *v = new QVBoxLayout(gpx2);
     QHBoxLayout *hbl1 = new QHBoxLayout();
     QCheckBox *Xlab = new QCheckBox(QString::number(mole.asymm.at(i).frac.x,'f',6).prepend("X: "),fixDlg);
     QCheckBox *Ylab = new QCheckBox(QString::number(mole.asymm.at(i).frac.y,'f',6).prepend("Y: "),fixDlg);
     QCheckBox *Zlab = new QCheckBox(QString::number(mole.asymm.at(i).frac.z,'f',6).prepend("Z: "),fixDlg);
     QCheckBox *Olab = new QCheckBox(QString::number(mole.asymm.at(i).sof,'f',6).prepend("site occupancy: "),fixDlg);
     hbl1->addWidget(Xlab);
     hbl1->addWidget(Ylab);
     hbl1->addWidget(Zlab);
     hbl1->addWidget(Olab);
     boxen.append(Xlab);
     boxen.append(Ylab);
     boxen.append(Zlab);
     boxen.append(Olab);
     v->addLayout(hbl1);
     if (mole.asymm.at(i).uf.m22!=-666.0) {
       QHBoxLayout *hbl2 = new QHBoxLayout();
       QCheckBox *M11lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m11,'f',6).prepend("U11: "),fixDlg);
       QCheckBox *M22lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m22,'f',6).prepend("U22: "),fixDlg);
       QCheckBox *M33lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m33,'f',6).prepend("U33: "),fixDlg);
       QCheckBox *M23lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m23,'f',6).prepend("U23: "),fixDlg);
       QCheckBox *M13lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m13,'f',6).prepend("U13: "),fixDlg);
       QCheckBox *M12lab = new QCheckBox(QString::number(mole.asymm.at(i).uf.m12,'f',6).prepend("U12: "),fixDlg);
       hbl2->addWidget(M11lab);
       hbl2->addWidget(M22lab);
       hbl2->addWidget(M33lab);
       hbl2->addWidget(M23lab);
       hbl2->addWidget(M13lab);
       hbl2->addWidget(M12lab);
       boxen.append(M11lab);
       boxen.append(M22lab);
       boxen.append(M33lab);
       boxen.append(M23lab);
       boxen.append(M13lab);
       boxen.append(M12lab);
       v->addLayout(hbl2);
     }
     vbl->addWidget(gpx2);
     alleboxen.append(boxen);
     for (int j=0;j<boxen.size();j++){
       int idx=1<<j;
       boxen[j]->setChecked(mole.asymm.at(i).fixFlag&idx);
       map->setMapping(boxen[j],allIx++);
       connect(boxen[j], SIGNAL(stateChanged(int)), map, SLOT(map()));
     } 


  }
  connect(map, SIGNAL(mapped(int)), this, SLOT(changeFixes(int)));
  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,fixDlg);
  connect(bb, SIGNAL(accepted()), fixDlg, SLOT(accept()));
  connect(bb, SIGNAL(rejected()), fixDlg, SLOT(reject()));
  gpx->setLayout(vbl);
  QVBoxLayout *la = new QVBoxLayout();
  QScrollArea *qsa = new QScrollArea(fixDlg);
  qsa->setWidget(gpx);
  la->addWidget(qsa);
  la->addWidget(bb);
  fixDlg->setLayout(la);
  fixDlg->exec();
  fixDlg->deleteLater();
}

void Window::changeFixes(int i){
  int da=i/10;
  int blub=i%10;
  int vor=-1;
  if (da<mole.asymm.size()){
    vor=mole.asymm[da].fixFlag;
    if (vor&(1<<blub))mole.asymm[da].fixFlag-=1<<blub;
    else mole.asymm[da].fixFlag|=1<<blub;
    jumpToAtom(da);
    QTextCursor c = editor->textCursor();
    c.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    if (c.selectedText().contains("=")) {
      c.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
      c.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    }
    //qDebug()<<mole.asymm.at(da).orginalLine;
    //qDebug()<< c.selectedText();
    int end= (mole.asymm.at(da).uf.m22!=-666.0)?10:4;
    QString sn=c.selectedText();
    sn.remove(QRegExp("^\\S+\\s+\\d+\\s+"));
    sn.remove(QRegExp("[=\n\r]+"));
    QStringList snl=sn.split(QRegExp("\\s+"));
    int egal=0;  
    double w[10],o[10];
    for (int i=0; i<snl.size();i++){w[i]=getNumber(snl.at(i).toDouble(),fvar,i,egal);}
    for (int i=0; i<end; i++){
      int tst=1<<i;
      int m=(tst&mole.asymm.at(da).fixFlag)?1:0;
      /* free variables come here ... m*fvarNr*fvarType
       *
       * */
      w[i]+=m*10;
    }
    QString neu=(end==10)?QString("%1  %2 %3 %4 %5  %6 %7 %8 =\n      %9 %10 %11 %12")
      .arg(mole.asymm.at(da).Label.section('_',0,0),-4)
      .arg(qMax(1,sfac.indexOf(mole.asymm.at(da).an)+1))
      .arg(w[0],11,'f',6)
      .arg(w[1],11,'f',6)
      .arg(w[2],11,'f',6)
      .arg(w[3],10,'f',5)
      .arg(w[4],10,'f',5)
      .arg(w[5],10,'f',5)
      .arg(w[6],10,'f',5)
      .arg(w[7],10,'f',5)
      .arg(w[8],10,'f',5)
      .arg(w[9],10,'f',5)
      :QString("%1  %2 %3 %4  %5 %6 %7")
      .arg(mole.asymm.at(da).Label.section('_',0,0),-4) 
      .arg(qMax(1,sfac.indexOf(mole.asymm.at(da).an)+1))
      .arg(w[0],11,'f',6)
      .arg(w[1],11,'f',6)
      .arg(w[2],11,'f',6)
      .arg(w[3],10,'f',5)
      .arg(o[4],10,'f',5);
    c.insertText(neu);
    mole.showatoms[da].orginalLine=mole.asymm[da].orginalLine=neu;
    mole.asymm[da].orginalLine.truncate(77);
    //qDebug()<<da<<mole.asymm.at(da).orginalLine;
    update();

  }


}

void Window::dispFromWave(){
  if (mole.asymm.isEmpty()) return;
  QFile kis(":kisselwave");
  kis.open(QIODevice::ReadOnly|QIODevice::Text);
  QByteArray ba=kis.readAll();
  QStringList all= QString(ba).split('\n');
  kis.close();
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock();
  replace->setText("REM DISP ");
  search->setText("^DISP ");
  replaceAll();
  replace->setText("");
  search->setText("");
  cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
  cursor = document->find("UNIT",cursor);
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  QStringList nn,an;
  QString disp;
  for (int i=0; i<sfac.size();i++){
    double aw=0,w;
    w=aw;

    int x=all.indexOf(QString("#S %1 %2").arg(sfac.at(i)+1,2,10,QChar('0')).arg(mole.pse(sfac.at(i))));
    for (int j=x+1; j<4000+x; j++){
      an=nn;
      nn=all.at(j).split(" ",QString::SkipEmptyParts);
      aw=w;
      w=nn.at(0).toDouble();
      if ((w!=aw)&&(mole.cell.wave>w)) {
	double f= (mole.cell.wave-aw)/(w-aw); 
//	printf("DISP $%s %f %f\n",mole.pse(sfac.at(i)).toStdString().c_str(),f*nn.at(1).toDouble()+(1-f)*an.at(1).toDouble(),f*nn.at(2).toDouble()+(1-f)*an.at(2).toDouble());
        disp=QString("DISP $%1 %2 %3 %4!source kissel\n")//REM %5\nREM %6\n")
		.arg(mole.pse(sfac.at(i)))
		.arg(f*nn.at(1).toDouble()+(1-f)*an.at(1).toDouble(),8,'f',5)
                .arg(f*nn.at(2).toDouble()+(1-f)*an.at(2).toDouble(),8,'f',5)
                .arg(f*nn.at(3).toDouble()+(1-f)*an.at(3).toDouble(),8,'f',5)
                //.arg(muval.value(sfac.at(i)))
                //.arg(all.at(j-1))
                //.arg(all.at(j))
                ;
	cursor.insertText(disp);
	j=5000+x;
      }
    }
//    printf("bei zeile %d %s %d [%s]\n",x,QString("#S %1 %2").arg(sfac.at(i)+1,2,10,QChar('0')).arg(mole.pse(sfac.at(i))).toStdString().c_str(), all.size(),all.at(0).toStdString().c_str());

  }

  cursor.endEditBlock();
}

void Window::updateUnit(){
  if (mole.asymm.isEmpty()) return;
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  cursor.beginEditBlock ();
  cursor.movePosition(QTextCursor::Start);
  cursor = document->find(unitAlt,cursor);
  if (cursor.isNull()) {
    cursor = editor->textCursor();
    cursor.endEditBlock ();
    printf("can't find unit\n");
    update();
    return;
  }
  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
  cursor.deleteChar();
  cursor.insertText(unitNeu);
  cursor.endEditBlock ();
  editor->setTextCursor(cursor);
  editor->centerCursor () ;
  cursor.clearSelection ();
  editor->setTextCursor(cursor);
  editor->setFocus () ;
  update();
}

void Window::weedEmptySfacs(){
  if (mole.asymm.isEmpty()) return;
  QMap<int,double> unit;
  for (int i=0; i<mole.asymm.size(); i++){
    if (mole.asymm.at(i).an>=0) unit[mole.asymm.at(i).an]+= mole.asymm.at(i).sof * mole.cell.symmops.size();
  }
  unitNeu="UNIT";
  for (int i = 0; i < sfac.size() ; i++){
    if (unit.value(sfac.at(i))) {
      if (unit.value(sfac.at(i))>9999)
      unitNeu.append(QString(" %1").arg((int)unit.value(sfac.at(i)))); else
      unitNeu.append(QString(" %1").arg(unit.value(sfac.at(i)),0,'g',4));
    }
    if (unitNeu.split('\n').last().size()>76) unitNeu.append("=\n    ");
  }
  updateUnit();
  QTextDocument *document = editor->document();
  QTextCursor cursor = editor->textCursor();
  QTextCursor c=cursor;
  cursor.movePosition(QTextCursor::Start);
  int apos=0,pos=0;
  QList<int> mussweg,newSfac=sfac;
  for (int i = 0; i < sfac.size() ; i++){
    if (unit.value(sfac.at(i))==0) {
      mussweg.append(i);
      newSfac.removeOne(sfac.at(i));
    }
  }
  if (mussweg.isEmpty()) return;
  cursor = document->find(QRegExp("^SFAC"),cursor);
  if (cursor.isNull()){editor->setTextCursor(c);return;}
  cursor.movePosition(QTextCursor::Start);
  while ((!cursor.isNull())&&(pos<sfac.size())){
    cursor = document->find(QRegExp("^SFAC"),cursor);
    cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
    if (cursor.selectedText().contains(QRegExp("[.0-9]+"))){
      bool hatgleich=cursor.selectedText().contains(QRegExp("=\\s*$"));
      cursor.clearSelection();
      cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
      if (mussweg.contains(pos)) {
	cursor.insertText("REM ");
	if (hatgleich) {
	  cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
	  cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
	  cursor.insertText("REM ");
	}
      }else if (hatgleich) cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
      pos++;
    }
    else {
      pos+=cursor.selectedText().split(" ",QString::SkipEmptyParts).size()-1;
      cursor.deleteChar();
      QString ac;
      for (int i =apos; i<newSfac.size() && i<pos+apos;i++) ac.append(QString("%1 ").arg(mole.pse(newSfac.at(i))));
      apos=pos;
      cursor.insertText(QString("SFAC %1\n").arg(ac));
    }
  }
  for (int i=0; i<mole.asymm.size();i++){
    if ((sfac.indexOf(mole.asymm.at(i).an)>-1)&&
		    (newSfac.indexOf(mole.asymm.at(i).an)>-1)&&
		    (sfac.indexOf(mole.asymm.at(i).an)!=newSfac.indexOf(mole.asymm.at(i).an))){
      jumpToAtom(i);
      cursor = editor->textCursor();
  //    qDebug()<<cursor.blockNumber();
      cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
      QString repla=cursor.selectedText();
      cursor.deleteChar();
      repla.replace(QString(" %1 ").arg(1+sfac.indexOf(mole.asymm.at(i).an)), QString(" %1 ").arg(1+newSfac.indexOf(mole.asymm.at(i).an)));
      cursor.insertText(repla);
      mole.asymm[i].orginalLine=repla;
//      printf("%s %d => %d\n",mole.asymm.at(i).Label.toStdString().c_str(),sfac.indexOf(mole.asymm.at(i).an)+1,newSfac.indexOf(mole.asymm.at(i).an)+1);

    }
  }    
  fileSave();
  loadrefres();
  document = editor->document();
  if(document->toPlainText().contains("DISP"))
    dispFromWave(); 
}

bool Window::maybeSave() {
  if (!editor->document()->isModified()) return true;
  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(this, tr("Application"),
		  tr("The document has been modified.\n"
			  "Do you want to save your changes?"),
		  QMessageBox::Save | QMessageBox::Discard
		  | QMessageBox::Cancel);
  if (ret == QMessageBox::Save){
    return fileSave();
  }
  else {if (ret == QMessageBox::Cancel)
    return false;
  }
  return true;
}

bool Window::maybeSave2() {
  if (!editor->document()->isModified()) return true;
  if (doNotAnnoyMe) return fileSave();
  QMessageBox *box=new QMessageBox(QMessageBox::Question,tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
  QPushButton *ischkansonetabeida = box->addButton("Always!",QMessageBox::ActionRole);
  box->setDefaultButton(QMessageBox::Save);
  int ret;
  ret = box->exec();

  if (box->clickedButton()==ischkansonetabeida){
      doNotAnnoyMe=true;
      mole.einstellung->beginGroup("DoNotAskFileSave");
      mole.einstellung->setValue("DoNotAnnoyMe",doNotAnnoyMe);
      mole.einstellung->endGroup();
      return fileSave();
  }
  if (ret == QMessageBox::Save){
    return fileSave();
  }
  else {if (ret == QMessageBox::Cancel)
    return false;
  }
  return true;
}

QString Window::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

void Window::updateRecentFileActions() {
  QSettings *einstellung =new QSettings( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME );
  einstellung->beginGroup("Files");
  QStringList files = einstellung->value("recentFileList").toStringList();
  int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

  for (int i = 0; i < numRecentFiles; ++i) {
    QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
    recentFileActs[i]->setText(text);
    recentFileActs[i]->setData(files[i]);
    recentFileActs[i]->setStatusTip(files[i]);
    recentFileActs[i]->setVisible(true);
  }
  for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
    recentFileActs[j]->setVisible(false);
  einstellung->endGroup();
  delete einstellung;
}

void Window::linkHelp(){
  QMessageBox::about(this, tr("%1-Help").arg(PROGRAM_NAME),
      tr("<h1>%1-Help</h1><br> The offline help has ben removed. <a href=\"http://ewald.ac.chemie.uni-goettingen.de/shelx/help/\">Click here for Online Help.</a>\n"
        ).arg(PROGRAM_NAME));
}

void Window::about(){
    QString openGLVersion=QString("OpenGL Version %1").arg((char *)glGetString(GL_VERSION));
    QMessageBox::about(this, tr("About %1").arg(PROGRAM_NAME),
                    tr("<p><b>%1</b>&nbsp;is&nbsp;a&nbsp;tool&nbsp;for&nbsp;<b>SHELX</b>&nbsp;small&nbsp;"\
                            "molecule&nbsp;refinement.&nbsp;It&nbsp;combines&nbsp;an&nbsp;editor with simple syntax highlighting "\
			    "with an OpenGL representation of the structure.</p> "\
			    "<p><img src=\":/icons/cbh.png\" alt=\"Christian B. H&uuml;bschle\">"\
			    "<img src=\":/icons/gms.png\" alt=\"George M. Sheldrick\"></p> "\
			    "<p><b>%1</b> is written by <b>Christian B. H&uuml;bschle</b> "\
			    " at the University of G&ouml;ttingen. "\
                            "Some parts of the program are taken from Qt examples, others are from the "\
			    "<b>SHELX</b> author <b>George M. Sheldrick</b> and/or his group Members</p> "\
                       "<font color=darkgreen> Please cite: <br>C. B. H&uuml;bschle, G. M. Sheldrick and B. Dittrich<br>"
                       "<em>'ShelXle:&nbsp;a&nbsp;Qt&nbsp;graphical&nbsp;user&nbsp;interface&nbsp;for&nbsp;SHELXL'</em><br>"
                       "<em>J. Appl. Cryst.</em> (2011). <b>44</b>, 1281-1284 .</font><br>"
                       "<a href=\"http://dx.doi.org/10.1107/S0021889811043202\">[&nbsp;doi:10.1107/S0021889811043202&nbsp;]</a>"
                            "<p>Please visit the following web sites:<br>"
"<a href=\"http://ewald.ac.chemie.uni-goettingen.de/\">"
                            "Dr. Birger Dittrich's Work Group</a> and <br>"
                            "<a href=\"http://shelx.uni-ac.gwdg.de/SHELX/\"><b>SHELX</b> site</a><p>"
                            "<p><a href=\"http://ewald.ac.chemie.uni-goettingen.de/shelx/index.php\">"
                            "Dowload new versions from here</a></p>"
			    "<p>This is Revision <b>%2</b>"
                            //"last build was: <b>%3</b></p> "
                            "<p>The latest Version available is '%6'"
			    "<p>The Version of Qt used is %4. </p> "\
			    "<p>%5<br>%8<br>%9 </p><p>The settings of this program are stored in: <b>%7</b>")
		    .arg(PROGRAM_NAME)
		    .arg(Revision)
//		    .arg(datum)
		    .arg(qVersion ())
		    .arg(openGLVersion)
                    .arg(latestRev)
		    .arg(mole.einstellung->fileName ())
		    .arg(QString("Graphics card: %1").arg((char *)glGetString(GL_RENDERER)))
		    .arg("")
		    );
  }

void Window::hintLhide(){
  hintL->hide();
  mole.einstellung->beginGroup("SaveHint");
  mole.einstellung->setValue("ShowRenameHint",hintL->isVisible());
  mole.einstellung->endGroup();


}

void Window::symmetryManager(){
    prosaTimer.setSingleShot(true);
    QDialog *sman = new QDialog(this);
    sman->setWindowTitle("Symmetry Manager");
    QGridLayout *lt = new QGridLayout(sman);
    sman->setMinimumSize(400,300);

    OrtepCodesbrowser=new QTextBrowser(sman);
    OrtepCodesbrowser->setOpenLinks(false);
    OrtepCodesbrowser->setLineWrapMode(QTextEdit::NoWrap);
    symmIntBx = new QSpinBox(sman);
    symmIntBx->setMinimum(1);
    symmIntBx->setMaximum(mole.cell.symmops.size());

    symmXBx = new QSpinBox(sman);
    symmXBx->setMinimum(1);
    symmXBx->setMaximum(9);
    symmXBx->setValue(5);

    symmYBx = new QSpinBox(sman);
    symmYBx->setMinimum(1);
    symmYBx->setMaximum(9);
    symmYBx->setValue(5);

    symmZBx = new QSpinBox(sman);
    symmZBx->setMinimum(1);
    symmZBx->setMaximum(9);
    symmZBx->setValue(5);


    symmFrgBx = new QSpinBox(sman);
    symmFrgBx->setMinimum(1);
    symmFrgBx->setMaximum(mole.maxmols());
    symmFrgBx->setValue(1);
    prosaSymm = new QLineEdit();
    prosaSymm->setText("x,y,z");
    connect(prosaSymm,SIGNAL(textEdited(QString)),this,SLOT(updateSM4(QString)));
    connect(symmIntBx,SIGNAL(valueChanged(int)),this,SLOT(updateSM1()));
    connect(symmXBx,SIGNAL(valueChanged(int)),this,SLOT(updateSM1()));
    connect(symmYBx,SIGNAL(valueChanged(int)),this,SLOT(updateSM1()));
    connect(symmZBx,SIGNAL(valueChanged(int)),this,SLOT(updateSM1()));
    connect(symmFrgBx,SIGNAL(valueChanged(int)),this,SLOT(updateSM1()));
    QLabel *unter = new QLabel("_");
    QLabel *doppl = new QLabel(":");
    shortestDistanceLabel = new QLabel("<p>Shortest distace to asymmetric unit: 0.00 &Aring;</p>");
    QPushButton *addSymmOp=new QPushButton("add symmetry to list");
    connect(addSymmOp,SIGNAL(clicked()),this,SLOT(updateSM2()));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), sman, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), sman, SLOT(reject()));
    connect(OrtepCodesbrowser,SIGNAL(anchorClicked( const QUrl &)), this ,SLOT(updateSM3(const QUrl &)));
    lt->addWidget(new QLabel("List of symmetry in use:"),0,0,1,12,Qt::AlignHCenter);
   // lt->addWidget(SymmetryCodesbrowser,1,0,1,5);
    lt->addWidget(OrtepCodesbrowser,1,0,1,12);
    lt->addWidget(new QLabel("symmetry operation"),2,0,1,5);
    lt->addWidget(new QLabel("sym No"),2,5,1,1);
    lt->addWidget(new QLabel("translation"),2,7,1,3,Qt::AlignHCenter);
    lt->addWidget(new QLabel("fragment"),2,11,1,1);
    lt->addWidget(prosaSymm,3,0,1,5);
    lt->addWidget(symmIntBx,3,5,1,1);
    lt->addWidget(unter,3,6,1,1);
    lt->addWidget(symmXBx,3,7,1,1);
    lt->addWidget(symmYBx,3,8,1,1);
    lt->addWidget(symmZBx,3,9,1,1);
    lt->addWidget(doppl,3,10,1,1);
    lt->addWidget(symmFrgBx,3,11,1,1);
    lt->addWidget(shortestDistanceLabel,4,0,1,9);
    lt->addWidget(addSymmOp,4,9,1,3);
    lt->addWidget(buttonBox,10,0,1,12);
    sman->setLayout(lt);
    QStringList oldsymm=mole.usedSymmetry;
    QString text="<table><tr><th>ID</th><th>symmetry op. </th><th style=\"background:#d4d4d4\">internal code</th><th></th><th></th><th>distance to asymmetric unit</th></tr>";;
    for (int i=0; i<mole.usedSymmetry.size(); i++)
        text.append(QString("<tr><th>$%6</th><td> %5 </td><th style=\"background:#d4d4d4\"> %1</th> <td><a href=\"%2\">remove</a> </td><td> <a href=\"%4\">edit</a></td><td> %3 &Aring;</td></tr>")
                     .arg(mole.usedSymmetry.at(i))
                     .arg(i)
                     .arg(mole.shortestDistance(mole.usedSymmetry.at(i)),0,'f',2)
                     .arg(-1-i)
                     .arg(mole.symmcode2human(mole.usedSymmetry.at(i)))
		     .arg(i+1));
    text.append("</table>");
    OrtepCodesbrowser->setHtml(text);

    if (sman->exec()==QDialog::Accepted){

        mole.showatoms.clear();
        mole.showbonds.clear();
        mole.showatoms+=mole.asymm;
        mole.packer(mole.usedSymmetry);
        mole.showbonds =
               mole.connecting(mole.showatoms);
        chgl->mol->selectedatoms.clear();
        chgl->murx=1;
     chgl->updateBondActions();
    }else{
        mole.usedSymmetry.clear();
        mole.usedSymmetry=oldsymm;
        oldsymm.clear();

        mole.showatoms.clear();

        mole.showatoms+=mole.asymm;
        mole.showbonds.clear();
        mole.packer(mole.usedSymmetry);
        mole.showbonds =
                mole.connecting(mole.showatoms);
        chgl->mol->selectedatoms.clear();
        chgl->murx=1;
        chgl->updateBondActions();
    }
        ;
    delete buttonBox ;

    text.clear();
    OrtepCodesbrowser->clear();
    delete OrtepCodesbrowser;
    delete symmIntBx;
    delete symmXBx;
    delete symmYBx;
    delete symmZBx;
    delete doppl;
    delete unter;
    delete symmFrgBx;
    delete shortestDistanceLabel;
    delete lt;
    delete sman;

    chgl->updateGL();
}

void Window::updateSM1(){
    if (prosaTimer.isActive()) return;
    QString t=QString("%1_%2%3%4:%5")
            .arg(symmIntBx->value())
            .arg(symmXBx->value())
            .arg(symmYBx->value())
            .arg(symmZBx->value())
            .arg(symmFrgBx->value());
    shortestDistanceLabel->setText(QString("<p>Shortest distace to asymmetric unit: %1 &Aring;</p>").arg(
                mole.shortestDistance(t),0,'f',2));
    prosaSymm->setText(mole.symmcode2human(t));
    QPalette p = prosaSymm->palette();
    p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
    prosaSymm->setPalette(p);
}

void Window::updateSM2(){
    QString t=QString("%1_%2%3%4:%5,")
            .arg(symmIntBx->value())
            .arg(symmXBx->value())
            .arg(symmYBx->value())
            .arg(symmZBx->value())
            .arg(symmFrgBx->value());
    if (t.startsWith("1_555:1"))return;
    OrtepCodesbrowser->clear();
    if (!mole.usedSymmetry.contains(t)) mole.usedSymmetry.append(t);
    QString text="<table><tr><th>symmetry op. </th><th style=\"background:#d4d4d4\">internal code</th><th></th><th></th><th>distance to asymmetric unit</th></tr>";;
    for (int i=0; i<mole.usedSymmetry.size(); i++)
        text.append(QString("<tr><td>%5 </td><th style=\"background:#d4d4d4\"> %1</th> <td><a href=\"%2\">remove</a> </td><td> <a href=\"%4\">edit</a></td><td> %3 &Aring;</td></tr>")
                     .arg(mole.usedSymmetry.at(i))
                     .arg(i)
                     .arg(mole.shortestDistance(mole.usedSymmetry.at(i)),0,'f',2)
                     .arg(-1-i)
                     .arg(mole.symmcode2human(mole.usedSymmetry.at(i))));
    text.append("</table>");
    OrtepCodesbrowser->setHtml(text);

}

void Window::updateSM3(const QUrl link){
   int index = link.toString().toInt();
   if (index<0) {
       index=-index-1;
       QString t = mole.usedSymmetry.at(index);
       int s,h,k,l,symmgroup;
       sscanf(t.toLatin1(),"%d_%1d%1d%1d:%d",&s,&h,&k,&l,&symmgroup);
       shortestDistanceLabel->setText(QString("<p>Shortest distace to asymmetric unit: %1 &Aring;</p>").arg(
                   mole.shortestDistance(t),0,'f',2));
       prosaTimer.stop();
       symmIntBx->setValue(s);
       symmXBx->setValue(h);
       symmYBx->setValue(k);
       symmZBx->setValue(l);
       symmFrgBx->setValue(symmgroup);
       return;
   }
   if (mole.usedSymmetry.size()<=index) return;
   mole.usedSymmetry.removeAt(index);
   QString text="<table><tr><th>symmetry op. </th><th style=\"background:#d4d4d4\">internal code</th><th></th><th></th><th>distance to asymmetric unit</th></tr>";
   for (int i=0; i<mole.usedSymmetry.size(); i++)
       text.append(QString("<tr><td>%5 </td><th style=\"background:#d4d4d4\"> %1</th> <td><a href=\"%2\">remove</a> </td><td> <a href=\"%4\">edit</a></td><td> %3 &Aring;</td></tr>")
                    .arg(mole.usedSymmetry.at(i))
                    .arg(i)
                    .arg(mole.shortestDistance(mole.usedSymmetry.at(i)),0,'f',2)
                    .arg(-1-i)
                    .arg(mole.symmcode2human(mole.usedSymmetry.at(i)))
                    );
   text.append("</table>");
   OrtepCodesbrowser->setHtml(text);

}

void Window::updateSM4(QString symtxt){
   QString t = mole.symmCard2Code(symtxt);
   QPalette p = prosaSymm->palette();

   p.setBrush(QPalette::Active,QPalette::Base,QColor(205,100,100));
   if (t.isEmpty()) {prosaSymm->setPalette(p);return;}

   p.setBrush(QPalette::Active,QPalette::Base,Qt::white);
   int s,h,k,l,symmgroup;
   shortestDistanceLabel->setText(QString("<p>Shortest distace to asymmetric unit: %1 &Aring;</p>").arg(
               mole.shortestDistance(t),0,'f',2));

   sscanf(t.toLatin1(),"%d_%1d%1d%1d:%d",&s,&h,&k,&l,&symmgroup);
   prosaTimer.start(80);
   symmIntBx->setValue(s);
   symmXBx->setValue(h);
   symmYBx->setValue(k);
   symmZBx->setValue(l);
   symmFrgBx->setValue(symmgroup);
   prosaSymm->setPalette(p);
   update();
}

void Window::insertDFIX(double value, double esd, QList<MyAtom> selected, QString resiSpec){

        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        QString buffer =QString("DFIX%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(value).arg(esd);
        for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
            buffer += (it->Label.section("_",0,0) + " ");
        buffer += "\n";
        cursor.insertText(buffer);
}

void Window::insertDANG(double value, double esd, QList<MyAtom> selected, QString resiSpec){

        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        QString buffer =QString("DANG%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(value).arg(esd);
        for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
            buffer += (it->Label.section("_",0,0) + " ");
        buffer += "\n";
        cursor.insertText(buffer);
}

void Window::insertFLAT(double esd, QList<MyAtom> selected, QString resiSpec){
        QTextDocument *document = editor->document();
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
        cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        QString buffer =QString("FLAT%1 %2 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(esd);
        for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
            buffer += (it->Label.section("_",0,0) + " ");
        buffer += "\n";
        cursor.insertText(buffer);
}

void Window::insertEXYZ(QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("EXYZ%1 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec);

    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertEADP(QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("EADP%1 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec);

    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertDELU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("DELU%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(esd1).arg(esd2);
    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertSIMU(double esd1, double esd2, double dmax, QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("SIMU%1 %2 %3 %4 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(esd1).arg(esd2).arg(dmax);
    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertISOR(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("ISOR%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(esd1).arg(esd2);
    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertRIGU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("RIGU%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(esd1).arg(esd2);
    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertCHIV(double vol, double esd1, QList<MyAtom> selected, QString resiSpec){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
    QString buffer =QString("CHIV%1 %2 %3 ").arg((resiSpec.isEmpty())?"":"_"+resiSpec).arg(vol).arg(esd1);
    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        buffer += (it->Label.section("_",0,0) + " ");
    buffer += "\n";
    cursor.insertText(buffer);
}

void Window::insertANIS(QList<MyAtom> selected){
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    cursor = document->find(QRegExp("^UNIT",Qt::CaseInsensitive),cursor);
    cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);

/* We need to exclude hydrogen atoms from ANIS instruction */
    QList<MyAtom> selectNoHydrogen;

    for (QList<MyAtom>::iterator it = selected.begin(); it != selected.end(); it++)
        if(it->an > 1)
            selectNoHydrogen.push_back(*it);

// Add ANIS instruction for each RESI individually
    int resicount(0);
    int workingresi(0);

    while (selectNoHydrogen.size() > resicount)
    {
        QList<MyAtom> sameresi;
        for (QList<MyAtom>::iterator it = selectNoHydrogen.begin(); it != selectNoHydrogen.end(); it++)
            if(it->resiNr == workingresi)
                sameresi.push_back(*it);

        resicount += sameresi.size();

        if(sameresi.size() != 0)
        {
            QString buffer("ANIS");
            QString buffer2;

            if(sameresi.at(0).resiNr != 0)
            {
                buffer += "_";
                buffer += QString::number(sameresi.at(0).resiNr) + " ";
            }
            else
                buffer += " ";

            for (QList<MyAtom>::iterator it = sameresi.begin(); it != sameresi.end(); it++)
            {
                buffer2 += (it->Label.section("_",0,0) + " ");

                if((buffer.size() + buffer2.size()) > 75) // max length was selected to be 75. Just in  case.
                {
                    buffer2 += "\n";
                    cursor.insertText(buffer+buffer2);
                    buffer2.clear();
                }
            }

            buffer2 += "\n";
            cursor.insertText(buffer+buffer2);
//            sameresi.erase(sameresi.begin(),sameresi.end());
        }

        workingresi++;
    }
}


#ifdef vc7hack1
int finite(double arg){
    return _finite(arg);
}
#endif
