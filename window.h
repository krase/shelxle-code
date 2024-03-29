
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
#ifndef WINDOW_H
#define WINDOW_H
#include <QtGui>
#include <QtNetwork>
// #include <QtWebKit>
#include "chgl.h"
#include "highlighter.h"
#include "codeeditor.h"
#include "eacDlg.h"
#include "qpeakview.h"
#include "psewidget.h"
#include "dragdropatomsdlg.h"
#include "historywidget.h"
#include "savehistorywidget.h"
#include "fourxle.h"
#include "fcvsfo.h"
#include "itsme.h"
#ifndef fmin
#define fmin(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef round
#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#endif
#undef vc7hack1
#ifdef _MSC_VER
#include <float.h>
int finite(double arg);
#define vc7hack1 1
#endif
#include "calculator.h"
#include "dsrgui.h"

/*! \brief Window is the main window class of ShelXle most of the magic happens here ;-)
*
*/

class Window : public QMainWindow {
Q_OBJECT
public:
  Window();
  virtual ~Window();
  //bool scroller;
  ChGL *chgl;  
  QDockWidget *dock, *renamDock, *qPeakDock, *histDock;
  QDockWidget *shxEditorDock,*shxOutputDock;
  QAction *enterRenameMode,*showQPeakLegend;
  QToolBar *editToolBar,//!< Edit toolbar (redo undo copy cut and paste...) +kill Hs, kill Qs, refine XL and anis refine
           *bottomToolBar,//!< selection toolbar with context specific content.
           *fito,//!< File toolbar
           *vito,//!< View toolbar to toggle bonds atoms etc...
           *extraTB, //!< External programs can be startet from here.
           *refToolBar;//!< Refinement toolbar not used yet... 
  QMenu  *runMenu;//!< the SHELX menu where you also run XL and do convientent stuff with the res file.
  QMenu *fileMenu, //!< file menu including the save history
        *editMenu, //!< standart edit menu
        *dialogMenu, //!< settings menu
        *viewMenu, //!< View menu 
        *packMenu, //!< Packing, growing, fusing etc...
        *extraMenu;//!< add and start external programs...

  QHash<QString,QVariant> myHash;//!< This Hash connects recent file names with cursor positions. So that files are opened always at the same position.
  QAction *refineAct;
  QAction *refineActAnis;
  double map_radius; //!< radius of the map in Angstrom around the rotaion center (used only in a mode of FourXle).
  FourXle *fxle;//!<The fourier map object.
//  QList<V3> orte;
//  QList<V3> normalen,nnn;
//  QList<QColor> farben;
//  int zoff,yoff,xoff;
  QList<int> sfac;//!<List of Scattering factors.
  QList<double> fvar;//!<List of Free Variables.
  QMap<int,int> fvarCntr;//!<Free Variable counter QMap.
  bool isLabelInUse();//!< Checks if the current Label in the reaname dialog is in use. \returns true if Label is already in use.
  public slots:
	  
  void qinitres();
  void about();//!<Shows the about dialog of ShelXle.
  void addNewScatteringFactor(int oz);//!<Adds a new scattering factor to the SFAC / UNIT list.
  void addDissorderedMethyl();//!<Adds rotation dissordered hydroghen atoms to a R-C carbon atom. (methyl group) AFIX 127.
  void addMethyl();//!<Adds hydroghen atoms to a R-C carbon atom. (methyl group) AFIX 137.
  void addMoreQPeaks();//!<Creates Q-Peaks from the Fo-Fc map.
  void autoHFix();//!<Automatically adds H atoms and AFIX instructions to the file uses the Fo-Fc map. 
  void changeFixes(int i);//!< Variable that should not be refined get a 10 added.
  void changeEditorFont();//!<Changes the Editor font via an font dialog.
  void changeElemetofLabel(const QString &pse);//!<NextLabel of rename mode consists of labelPSE labelIndex labelSuffix, this changes the LabelPSE to pse. @param pse next labelPSE. 
  void changeElement();//!<Changes the element of an atom (chgl->ImeanThisAtom) to an other scattering factor (acation->data)
  void changeIconSize();//!<Changes the size of the tool bar icons.
  void changeIndexofLabel(const int i);//!<NextLabel of rename mode consists of labelPSE labelIndex labelSuffix, this changes the LabelIndex to pse. @param i next labelIndex. For i < 0 no number in label. 
  void changeLabelFont();//!<Changes the font of the on screen Labels of the atoms.
  void changeScalefactor();//!<Changes the screen shot scale factor. 
  void changeSortierWeise(QAction* action);//!<Changes the sorting option to action->data. @param action the calling QAction.
  void changeSuffixofLabel(const QString &fix);//!<NextLabel of rename mode consists of labelPSE labelIndex labelSuffix, this changes the labelSuffix @param fix next labelSuffix.
  void checkForUpdates();//!<Checks via the internet if new versions of ShelXle are availabe.
  void clipboardDataChanged();//!<Enables the paste action if something is in the clipboard.
  void closeEvent(QCloseEvent *event);//!<Tidys everything up and stores settings befor closing the ShelXle app.
  void colorDLGFOP();//!<Color Dialog for Fobs plus. Positive values of Fo map are drawn with a mesh in this color.
  void colorDLGFOM();//!<Color Dialog for Fobs minus. Negative values of Fo map are drawn with a mesh in this color.
  void colorDLGDIP();//!<Color Dialog for Fobs-Fcalc plus. Positive values of Fo-Fc map are drawn with a mesh in this color.
  void colorDLGDIM();//!<Color Dialog for Fobs-Fcalc minus. Negative values of Fo-Fc map are drawn with a mesh in this color.
  void controlMap();//!<Shows the Map-Control Dialog.
  void convergeWght();//!<Tries to refine until the wheighting scheme converges.
  void complete();//!<forces the display list to be updated. 
  void configureBottomToolbar();//!<Changes the look and feel and position of the Selection Tool Bar.
  void copySymmMate();//!<copies the fractional coordinates of a symmetry generated part -n atom in the file. 
  void createCentroid();//!<creates a centroid dummy atom which can be used for restraints and measurements.
  void decreaseEdtiorFont();//!<Decreases the editor font by one point.
  void defaultColors();//!<Restores the default colors of the electron density maps (Fo+ blue Fo- yellow Fo-Fc+ green Fo-Fc- red).
  void deleteAllH();//!<Deletes all Hydrogen atoms and associated AFIX instructions. 
  void deleteAllQ();//!<Deletes all Q-Peaks.
  void deleteSelectedAtoms();//!<Deletes all selected atoms.
  void dispFromWave();//!<calculates f', f" and mu for the given wavelength and inserts DISP lines for each SFAC entry.
  void doMapsNow(bool b);//!<destroys maps or (re)calculates them freshly.
  int duplicates();//!<searches for identical labeled Atoms and add them to a list. 
  void editorIsVisible(bool);//!is Editor really visble
  void effuenf();//!<F5 recalculates the display lists sets the focus to the openGL widget and raises it on top. 
  void expandAll();//!<Expands all contacts .
  void extraSettings();//!<A dialog to add/edit exteral programs started from ShelXle with or without arguments.
  void exportCoordinates();//!<Exports all visible atoms to an extra file. 
  bool fileSave();//!<saves the editor content to the current file.
  bool fileSaveAs();//!<Saves the editor content to a specified file.
  void fillCell();//!<Uses symmetry until the unit cell box is full.
  void findInStructure(const QString &info);//!<the atom with the matching orgiginal line gets centered and selected.@param info original line content.
  void findNext();//!<Find next (F3) functionality of search and replace in the editor. 
  void findNext2();//!<Find next (F3) functionality of search in the list view.
  void findPrev();//!<Find previous functionality of search and replace in the editor.
  void findPrev2();//!<Find previous functionality of search in the list view.
  void findResi(QString);//!<Searches a Residue via a string like "RESI 22 ALA" or "RESI ALA 22" in the editor and selects all atoms with the same residue number (eg: 22).
  void findText();//!<Find functionality of search and replace in the editor.
  void findText2();//!<Find functionality of search in the list view.  
  void fixIt();//!< Fix some parameters from being refined.
  void foversusfc();//!< Shows a duialog to analyse and omit outlier reflections.
  void fuse();//!< show only the asymetric unit.
  void grossRC(const QString &text);//!< Capitalizes the text and sets the resiResiClass text to text @param text new text for resiResiClass.
  void grow();//!< completes the molecule by symmetry.
  void growQPeaks(bool qpg);//!< applies symmetrie operations used for the main structure also for the Q-Peaks.
  void hideSearch();//!< Hides the search and replace widget for the editor.
  void hideSearch2();//!< Hides the search widget for the list view.
  void hideToolz(bool b);//!< Hides all tool bars.
  void hintLhide();//!< Hides a hint in the reaname mode for ever.
  void increaseEdtiorFont();//!< Increases the editor font by one point.
  void infoKanalNews(const QString& s);//!< passes a string to the Info widget. @param s a new text for the info widget.
  void inheritLabels();//!< opens an inherit labels dialog.
  bool insertAnis();//!< inserts anis instruction into the file.
  void insertBind();//!< inserts a BIND instruction into the file.
  void insertFree();//!< inserts a FREE instruction into the file.
  void insertList6();//!< inserts a LIST 6 instruction into the file.
  void invert();//!< inverts the structure and changes ot the right enantiomorph space group.
//  inline int Intersect( double& vm, double& vp ){ return vm*vp <= 0.0 && (vm<0.0 || vp<0.0); }//!<
  void isUpToDate(QNetworkReply* antwort);//!< Interpretes the network reply for the check for updates (manual version)
  void jnk();//!< Julian Henn and Kathrin Meindl frcatal dimension analisis
  void jumpToAtom(int index);//!< if rename mode is visible the atom at index gets renamed if not the cursor of the editor is pointed to the specified atom.
  void jumpToError();//!< the cursor points to the first syntax error in the file.
  void linkHelp();//!< Show a Link to the online help. 
  void loadAFile();//!< (re)loads the file specified in dirName.
  void loadFile(QString fileName);//!< loads the file with the specified path the listfile is also loaded. @param fileName path to the res / ins file.
  void loadLst();//!< loads the .lst file if it exists.
  void loadrefres();//!< loads the refinement results from the res file.
  void loadThisFile();//!< loads a file from an QAction with path as Action data. 
  void mapDefault();//!< restores the defaults of the Map Control dialog.
  void moveSymmMate();//!< Moves the specified symmetry generated atom to the asymmetric unit.
  void moveSymmMateSel();//!< Moves selected symmetry generated atoms to the asymmetric unit.
  void omitsome(const QString &s);//!< insetrts a 'OMIT h k l\n' string in the res file.  
  void openFile();//!< Open res file dialog
  void openMapControl();//!< shows the Map Control dialog.
  void openRecentFile();//!< opens a recently used file
  void openSaveHistory();//!< sows the save history dialog
  void outputIsVisible(bool);//!< is Output really visble
  void insertDFIX(double value, double esd, QList<MyAtom> selected, QString resiSpec=""); //!< Insert DFIX restraint into ins file
  void insertDANG(double value, double esd, QList<MyAtom> selected, QString resiSpec=""); //!< Insert DANG restraint into ins file
  void insertFLAT(double esd, QList<MyAtom> selected, QString resiSpec=""); //!< Insert FLAT restraint into ins file
  void insertEXYZ(QList<MyAtom> selected, QString resiSpec=""); //!< Insert EXYZ restraint into ins file
  void insertEADP(QList<MyAtom> selected, QString resiSpec=""); //!< Insert EADP restraint into ins file
  void insertDELU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec=""); //!< Insert DELU restraint into ins file
  void insertSIMU(double esd1, double esd2, double dmax, QList<MyAtom> selected, QString resiSpec=""); //!< Insert SIMU restraint into ins file
  void insertISOR(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec=""); //!< Insert ISOR restraint into ins file
  void insertRIGU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec=""); //!< Insert RIGU restraint into ins file
  void insertCHIV(double vol, double esd1, QList<MyAtom> selected, QString resiSpec=""); //!< Insert CHIV restraint into ins file
  void insertANIS(QList<MyAtom> selected); //!< Insert ANIS for selected atoms
//  bool peakcomp(MyAtom &a1,MyAtom &a2);
  void redoRename();//!<if redo is clicked and it was a rename a rename is redone.
  void removeExtra(QAbstractButton *button);//!<removes an external program from the menu / tool bar.
  void renameRNchanged(int ii);//!< Residue Number of rename mode is changed to ii @param ii residue number.
  void renameThisAtom();//!< The label of the specified atom is used to feed the inputs of rename mode and the latter is opened.  
  void renameThisAtom(int index);//!< specified atom is renamed as given by rename mode
  void renamUpdate(bool vis);//!<rename mode gets shown or hidden. @param vis visibitity of rename mode.
  void replaceAll();//!<replace all functionality of search and replace in the editor.
  void replaceNext();//!<replace next functionality of search and replace in the editor.
  void replacePrev();//!<replace previous functionality of search and replace in the editor.
  void replyFinished(QNetworkReply* antwort);//!<Interpretes the network reply for the check for updates (automatic version)
  void resiSelectorCheck();//!<updates the 'Residue Finder' when the text cursor moves.
  void rotd();//!< rotates down 
  void rotl();//!< rotates left   
  void rotr();//!< rotates right   
  void rotu();//!< rotates up   
  void rufmode();//!< new labels can not be duplicates if this mode is on
  void runShelXL();//!< copies res to ins file and runs shelxl 
  void runExtra();//!< runs rexternal programs
  void runXLWGHT();//!< runs shelxl for convergeWght. 
  void resiFinderDestroyed();
  void saveScene();//!< saves a srceen shot of the OpenGL widget
  void schnittart(int trunc);//!< the truncation type of the electron density maps changes to trunc @param trunc new truncation type.
  void sdm();//!< the shortest distance matrix is (re)calculated and the structure is grown
  void selectInStructure(const QString &info);//!< selects all atoms found in selected text in the structure @params info seleted text. 
  void selectParts();//!< selects atoms for 'Part Selector'
  void setShelxlOptions();//!< sets path and command line options for ShelXl executable.
  void setShelxlPath();//!< sets path for ShelXl executable
  void setExtraPath();//!<sets path for an external program
  void setMonoQ(bool vis);//!<sets monochome modus for Q-Peaks to vis @param vis mochrome mode of Q-Peaks.
  void showSearch();//!< shows the search and repleace for the editor.
  void showSearch2();//!< shows the search for the list view
  void shxActUpdate(bool vis);//!< the editor tool bar gets visible when the editor is visible.
  void shxOPTUpdate(bool vis);//!< the editor tool bar gets visible when the editor is visible.
  void sina();//!< ANIS backwards is SINA! makes specified, selected or all atoms isotrop again with uiso = 0.05
  void sortAtoms();//!< sorts atoms in file acording to the sort option.  
//  void springSlider();
  void  spellCheck();//!< a syntax check for the res file.
  QString strippedName(const QString &fullFileName);//!< the file name without the path is retrned \returns the filename without path. @param total file path. 
  void styleDialog();//!< Atom style dialog is shown.
  void symmetryManager();//!< a symmetry manager is shown 
  void toggDocWid(bool weg);//!< hides Text Windows
  void toggleEQHighlighting(bool);//!< toggles higlighting of symmetry generated atoms.
  void toggleOrtho(bool ov);//!< toggles orthographic and perspective projection.
  void trick();//!< a (dirty) trick to show the GLWidget in the right way all the time. 
  void undoRename();//!<if undo is clicked and it was a rename a rename is undone.
  void uniq();//!< moves centers of gravity of all molecules into the unit cell box. !does not heal scrambled molecules!
  void unique();//!< !does ?not? heal scrambled molecules!
  void updateLabel();//!< the rename mode is updated 
  void updateLC();//!<updates the Lines and Columns display.
  void updateRecentFileActions();//!<updates the recent file list 
  void updateScaleDlg();//!< recalculates scale factor with and heigth of the screen shot scale factor dialog
  void updateSelectResi();//!< updates the ResiFinder ComboBox for each editor line starting with RESI a new item is issued .
  void updateSM1();//!< for the symmetry manager dialog
  void updateSM2();//!< for the symmetry manager dialog
  void updateSM3(const QUrl link);//!< for the symmetry manager dialog
  void updateSM4(QString s);//!< for the symmetry manager dialog
  void updateStatusBar(const QString& s) ;  //!<changes the statusbar text to s @param s text for the status bar
  void updateTieFreeVar();//!< if the occupation factor should be tied to a free variable the rename mode 
  void updateUnit();//!< the unit instruction gets the true amount of atoms per type
  void updateWght();//!< the WGHT instruction is updated from the back of the res file
  void updateXLput();//!< the output view for the shelxl run is updated
  void updateExtraOutput();//!< the output view for the external program is updated.
  void updateExtraSettings(int index);//!< the external programs dialog gets updated.
  void wasDelete();//!< for undo redo fuctionality of atom deletion.
  void wasRename();//!< for undo redo fuctionality of atom renameing. 
  void withdrawres();//!< discard the results of the refinement. Copies back the ins file to the res file and loads that.  
  void weedEmptySfacs();//!< weeds out unused scattering factors 
  void zoomin(); //!< zoomes in 
  void zoomout();//!< zoomes out 
  void calculator(); //!< A dialog to work with resolution and 1,3 distances via the angle
  void dsr_gui(); //!< A dialog for DSR
protected:
  void paintEvent(QPaintEvent *event);
  void moveEvent(QMoveEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
private:
  int sortierWeise;
  int cycls;
  QNetworkAccessManager *net;
  QNetworkReply *reply;
  V3 urs;
  QString altText;
  PSEWidget *psew;  
  QSpinBox *symmIntBx;
  QSpinBox *symmXBx;
  QSpinBox *symmYBx;
  QSpinBox *symmZBx;
  QSpinBox *symmFrgBx;
  QLineEdit *prosaSymm;
  QTimer prosaTimer;
  QTextBrowser *OrtepCodesbrowser;
  QLabel *shortestDistanceLabel;
  QPushButton *willpse;
  bool machPlatz,FehlerTeufel;
  int hhh;
//  QSlider *zoomer;
//  QTimer *sliderTimer;
  QSignalMapper *dsm;
  QProgressBar *balken;
  HistoryWidget *hw;
  double range,weight;
  int maptrunc,tbiconSize;
  int falschAtom;
  QCompleter *completer;
  QString labelPSE,labelSuffix,nextLabel,title;
  int labelIndex;
  QLabel *nextNameL, *replacementInfo, *hintL;
  QCheckBox *rufModeBox,*indexIncr;
  QSpinBox *resiNrSpin;
  QSpinBox *indexSpin;
  QSpinBox *partSpin;
  QSpinBox *spinSelectPart;
  QCheckBox *inculdePartNull, *wholeLine;
  QCheckBox *selectPart;
  QLabel *tieOccL;
  QComboBox *tieType;
  QLabel *tieFVNrL;
  QSpinBox *tieFVNr;
  QLabel *tieUsedL;
  QString FFTErrorString;
  QComboBox *sufixBox;
  QLineEdit *resiResiClass;
  QPeakView *qpv;
  CEnvironment undoAtom,redoAtom,deletedAtom,restoredAtom;
  QList<int> undoAtomIndex,redoAtomIndex,gehtMichAn,gehtMichAn2;
  int balkenstep,altursize;
  int mode;
  QMenu *recentFilesMenu,*stereoMenu,*moveMenu,*helpMenu,*sortMenu;
  QActionGroup *stereoGroup,*tubeGroup,*sortGroup;
  enum { MaxRecentFiles = 10 };
  QAction *recentFileActs[MaxRecentFiles];
  bool otherOpenFiles,doNotAnnoyMe;
  Molecule mole;
  QComboBox *mapSchnitt;
  QDoubleSpinBox *maprad,*fomaps,*difmaps,*weak,*mapprec,*lineTrans,*lineWidth;
  QPushButton *foPlusButton,*foMinusButton,*diffPlusButton,*diffMinusButton, *defColorButton;
  QDialog *md;
  QSpinBox *widthBox;       
  QSpinBox *heightBox;      
  QDoubleSpinBox *scalefBox;
  QDialog *scalDlg;
  QTextBrowser *infoKanal;
  QRadioButton *pserbt[110];
  QGroupBox *sfacBox;
  double scalePic;
  QString saveName,dirName,fvarinfo;
  QProcess *shelxl,*extraProc;
  QDialog *extraOutput;
  QTextEdit *extraOutPut;
  CodeEditor *editor , *xlPutor;
  QLineEdit *search2, *search, *replace;
  QString unitAlt,unitNeu,shelxlPath,shelxlArgs;
  QStringList extraProgramNames,extraProgramPaths,extraOptions,extraExt2;
  QList<Qt::CheckState>  extraArguments,extraExtensions, extraDetach;
  QLineEdit *extraPath,*extraName,*extraAltExtEdit,*extraArgumentsEdit,*shxPath,*shxOptions;
  QDockWidget *helpDock;
//  QWebView *hilfe;
  //QTextBrowser *hilfe;
  QWidget *shx_group;
  QVBoxLayout *sss,*amdl;
  QHBoxLayout *nlt;
  QGroupBox *renameBox ;
  QVBoxLayout *rnla;    
  QLabel *nliL,*resiResiClassL,*resiNrSpinL,*partSpinL;         
  QHBoxLayout *sfacla;
  QScrollArea *qsa,*qsa2;
  QAction *zm_stereo, *pe_stereo, *xe_stereo, *an_stereo, *hw_stereo, *no_stereo; 
  QAction *fcvsfoAct, *calcAct;



  QAction *soact;
  QAction *backColor,*labelColor;
  QVBoxLayout *lt;
  QToolButton  *zoomIn,*zoomOut;
  QLabel *sLabel;
  QCheckBox *qGrowPeak,*search4duplicates;
  QTimer *hidwillpse;
  QLabel *indexL,*sufixL;
  QLabel *scalwla, *scalhla, *scalsla, *header;
  QDialogButtonBox *buttonBoxMC; 
  QPushButton *applyMC;          
  QPushButton *applyDF;          
  QLabel *wl, *pl, *dl, *ol, *rl, *sl, *lw, *ltz;




  QHBoxLayout *indexla;
  QStringList alpha;
  QGridLayout *slt,*scalla,*mdl;
  QHBoxLayout *slt2;       
  QVBoxLayout *vlt;
  QGroupBox *gp;QHBoxLayout *rhla,*rhla2; 
  QHBoxLayout *buttomLt;
  QPushButton *showLst;
  QToolButton *hidesearch2,*hidesearch;
  QToolButton *prev,*next,*prev2,*next2,*replaceButton,*replacePButton,*replaceAButton; 
  QIcon extraIcon;
  QCheckBox *useArgument;
  QCheckBox *omitExtension;
  QCheckBox *detach;
  QSize ediSize;
  QPoint ediPos;
  QComboBox *resiFinder;
  QLabel *lineColumn;
  QGroupBox *suchbox2,*suchbox,*navibox,*refineButtonBox;
  QPushButton *stoppRefinement;
  QPushButton *loadRes;
  QPushButton *withdraw;
  QToolButton *spellCheckact,*spellCheckact2;
  QAction
	  *tubeStyle,
	  *ballStyle,
          *separateLabelFile,
      *toggleOrthoView,
      *actionUniq,
      *actionSearch,
      *actionNext,
      *actionPrev,
      *actionSave,
      *actionSaveAs,
      *actionUndo,
      *actionRedo,
      *actionCut,
      *actionCopy,
      *actionPaste,
      *actionHFix,
      *actionEP,
      *actionEM,
      *togAtom,
      *togBond,
      *togLabel,
      *togUnit,
      *togElli,
      *togHBond,
      *togDepthCueing,
      *togXRayView,
      *togBackGr,
      *killQs,
      *killHs,
      *astdlg,
  *equivHighlighting,
      *mapcontrol,
      *wghtAct,
      *sinaAct;
  void searchchanged(bool current,bool back);
  void searchchanged2(bool current,bool back);
  void shx_editor(QString filename,QString title);
  int isacommand(QString command);
  double getNumber(double v,const QList<double> fv,double uiso);//to be changed!
  double getNumber(double v,const QList<double> fv,int idx,int &fixFlag);//to be changed!
  double ueq(Matrix m);
  MyAtom findOH(V3 donor, V3 acceptor,int dindex,QStringList alab);
  QString load_sheldrick(QString fileName);
  bool maybeSave();  
  bool maybeSave2();
  bool ediVis,oupVis,startFused;
  Calculator *calcmenu;
  DSRGui *dsr;
};
#endif

