
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
#ifndef CHGL_H
#define CHGL_H

#include <QGLWidget>

#include "molecule.h"
#ifndef MAXSELECT
#define MAXSELECT 300001
#endif

#define HIDE_REASON_SELECT          1
#define HIDE_REASON_THIS_FRAGMENT   2
#define HIDE_REASON_OTHER_FRAGMENT  4
#define HIDE_REASON_HYDROGEN        8
#define HIDE_REASON_QPEAK          16 

/*! \brief ChGL is the QGLWidget of ShelXle
 */
class ChGL : public QGLWidget{
Q_OBJECT
public:
    explicit ChGL(QWidget * parent = 0);
    void setMainWindow(QWidget * parent){
    chparent=parent;
    }
    int ww,//!<Window with
	wh,//!<Window height
	minus;//!<for streo mode swiches right and left eye view
    int stereo_mode;//!< intager value of the current stereo mode
    int objCnt;//!< number of visible atoms + q-peaks
    double xrdist;//!< distance for x-ray view cut planes
    int ImeanThisAtom;//!< atom index for context menu. 
    double viewAngle,//!< viewing angele for the perspective view 0 is othogonal 
	   envirange;//!< range in Angstrom around which environmental atoms should be consedered.
    QColor labelColor,//!< Color of atom labels.
        backGroundColor;//!< Base color of the back groud.
    bool pause,//!< if true no rendering is made
	 inRenameMode,//!< if true wee are in rename mode
	 noWaitLabel,//!< if true wire mode after list initialization is not used to generate srceen shots.
         exporting;//!< for exporting bitmaps
    struct labz{ 
    int x,y;
    QString l;
    int an;
    };//!< struct labels for exporting bitmaps 
    QList<labz> LabelZ;//!< labels for exporting bitmaps 
    bool hiddenThings;//!< We know that there are hidden objects. 
    bool neutrons;//!< true if we have neuton scattering factors (negative Fo map is shown if this is true)
    int dratpause;//!< when the molecule is rotated for a time of dratpause milliseconds the molecule is drawn as a wire frame draft
    QFont myFont,//!< there is a Font for Labels of atoms 
	  nonAtomFont;//!< there is a Font for Labels of Q-Peaks
    QTimer *moving;//!< timer runs dratpause miliseconds after a rotation or scale
    GLdouble MM[16];//!< a GL matrix.
    QAction *addBond,//!< QAction BIND 
	    *killBond,//!< QAction FREE
	    *clearSelection,//!< QAction deselect
	    *centerSelection,//!< QAction center selected atoms
	    *hideNotSelection,//!< QAction hide non selected atoms
	    *unhide,//!< QAction show all previous hidden objects
	    *grow,//!< QAction grow
	    *fuse,//!< QAction fuse
	    *shpm1,//!< QAction show part -N ghost
	    *hideh, //!< QAction hide Hydrogen atoms
	    *qPeakBonds,//!< QAction Q-Peak binds
	    *toggleDockWidgets,//!< QAction hide text windows
	    *delSelAt,//!< QAction delete selected atoms
	    *highlightParts, //!< QAction highlight atoms in parts != 0
	    *centroid, //!< QAction create a centroid dummy
	    *enviNoQ,//!< QAction ENVI only for real atoms
	    *enviCova,//!< QAction ENVI only for covalent contacts
	    *wireButt,//!< QAction show atoms while rotation in wire mode
	    *invSelection;//!< QAction invert selection
    QToolButton *enviButt;//!< Button to clear the envi -ronment.
    QList<V3>enviPositions;//!<List of atom postions of an envi-ronment.
    QList<int>enviKat;//!< List of categorys of contacts in an envi-ronment.
    V3 enviP0;//!< coordinate used for envi.
    QMenu *sfacMenu;//!< a sub menu to change the element of a specified atom.
    QStringList labs;//!< Labels for an envi-ronment list.
    Molecule *mol;//!< pointer to the Molecule object.
    int bas,//!< GL List base for atoms bonds unit cell.
	murx,//!< non zero values lead to a rerendering of the display lists. 
	foubas[37],//!< GL List base for the fourier maps.
	wirbas;//!< GL List base for wireframe bonds and atoms.
    double L;//!< a scale factor.
    QAction *foact, //!< QAction toggling Fobs maps.
	    *fofcact;//!< QAction toggling Fobs-Fcalc maps.
    int rotze;//!< if not -1 the index of an atom in the rotation center.
    QToolButton    *invertMouseZoom;//!< inverts the mouse direction that zoom on right click.
	QCheckBox      *lighting,//!< enable lighting on the electron density. 
	      *niceTrans,//!< draw 6 perspectives for each surface to improve transparent visualization. 
	      *fillMap;//!< draw maps in filled style.
    QToolButton *rCenter;//!< resets the rotation center
    V3 altemitte;//!<rotation center position
    void initLists();
    void zoom(double speed);
    bool isFO(){return drawFO;}//!< if Fo map is visible true is returned.
    bool isDF(){return drawDF;}//!< if Fo-Fc map is visible true is returned.
    //void warFaul(){warfaul=true;}
    void gZoom(double speed);
    void rotZ(double speed);
    void rotY(double speed);
    void rotX(double speed);
    void moveX(double speed);
    void moveY(double speed);
    void setupTexture();    
    void updateBondActions();
signals:
    void newContext();//!< this seems to be a (dirty) solution for visibility problem of the GL content
    void no_hw_st();//!< we have no hard ware stereo sorry!
    void message(const QString &);//!< small text messages to the status bar.
    void bigmessage(const QString &);//!< passes HTML strings to the info window.
    void jumpit(int index);//!<An atom has ben left clicked: cursor jump on it.
    void diffscroll(int numsteps,int diff);//!< to change the iso value of the Fo- or Fo-Fc maps
    void neuemitte(V3 mitte);//!< the rotation center has changet to mitte.
    void inimibas();//!< tells FourXle to initialize the GL display lists.
    void qpfoci(double height);//!< when a mouse hovers a Q-Peak the height is emitted to the Q-Peak Legend.
    void insertDFIX(double value, double esd, QList<MyAtom> selected, QString resiSpec); //!< Insert DFIX restrain to the ins file
    void insertDANG(double value, double esd, QList<MyAtom> selected, QString resiSpec); //!< Insert DANG restrain to the ins file
    void insertFLAT(double esd, QList<MyAtom> selected, QString resiSpec); //!< Insert FLAT restrain to the ins file
    void insertEXYZ(QList<MyAtom> selected); //!< Insert EXYZ restrain to the ins file
    void insertEADP(QList<MyAtom> selected); //!< Insert EADP restrain to the ins file
    void insertDELU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec); //!< Insert DELU restrain to the ins file
    void insertSIMU(double esd1, double st, double dmax, QList<MyAtom> selected, QString resiSpec); //!< Insert SIMU restrain to the ins file
    void insertISOR(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec); //!< Insert ISOR restrain to the ins file
    void insertRIGU(double esd1, double esd2, QList<MyAtom> selected, QString resiSpec); //!< Insert DELU restrain to the ins file
    void insertCHIV(double Vol, double esd1, QList<MyAtom> selected, QString resiSpec); //!< Insert CHIV restrain to the ins file
    void insertANIS(QList<MyAtom> selected); //!< Insert ANIS instruction for selected atoms

public slots:
    void along001(); 
    void anaglyphRedCyan();
    void changeBColor();
    void changeEnviRange();
    void changeTColor();
    void clearEnvi();
    void connectSelection();
    void crosseye();
    void decFontSize();
    void disConnectSelection(int index);
    void disSelection();
    void envi();
    void expand();
    void hardwareStereo();
    void hideHydrogens();
    void hideNonSelected();
    void hideOtherFragments();
    void hidePartMinusOne(bool off);
    void hideQPeaksBelow(double cutoff);
    void hideSelected();
    void hideThisFragment();
    void highliteQPeak(double co);
    void homeXY();
    void incFontSize();
    void invertSelection();
    void Listen();
    void loadOrientation();
    void nostereo();
    void parallel();
    void rehide();
    void rotCenter();
    void saveOrientation();
    void selectPair(const QString &s);
    void selectResiByNr(int nr);
    void selectThisFragment();
    void setADP(bool b);
    void setAtom(bool b);
    void setBGGradient(bool b);
    void setBond(bool b);
    void setDepthCueing(bool b);
    void setHBond(bool b);
    void setLabel(bool b);
    void setMatrix();
    void setMolecule(Molecule *m);
    void setReNaMo(bool b);
    void setRotationCenter();
    void setRotationCenter(int rz);
    void setTube(bool b){
      //!sets tube mode to b
      tubes=b;
      updateGL();
    }
    void setUnit(bool b);
    void setViewAngle(double ang);
    void setXRayView(bool b);
    void showHidden();
    void showMatrix();
    void zoomOut();
    void unsetTube(bool b){
      //!sets tube mode to not b
      tubes=!b;
      updateGL();
    }
    void zalman();
// Restrain slots
    void addDFIX(); 
    void addDANG(); 
    void addFLAT(); 
    void addEXYZ(); 
    void addEADP(); 
    void addDELU(); 
    void addSIMU(); 
    void addISOR();
    void addRIGU();
    void addCHIV();
    void addANIS();

protected:
   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
#if (QT_VERSION >= 0x040600) && defined (Q_WS_MAC)
   bool event(QEvent *event);
#endif
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void contextMenuEvent(QContextMenuEvent *event);
   void wheelEvent(QWheelEvent *event);
private:
  QWidget *chparent; 
  GLuint ppp,pp,p;     
#if (QT_VERSION >= 0x040600) && defined (Q_WS_MAC)
        bool gestureEvent(QGestureEvent *event);
        void pinchTriggered(QPinchGesture*);
        void swipeTriggered(QSwipeGesture*);
#endif
        bool mouseOverInteraction,atomsClickable,tubes,bggradient;
	bool drawAx,drawUc,drawAt,drawADP,drawBo,drawLa,drawHb,drawFO,drawDF,xray_vision,depthcueing,
	    // warfaul,
	     warLabel;
        int imFokus,afok;
	void draw();
        //void highFocus();
	int xray_eye,fogrange;
	GLuint selectBuf[MAXSELECT];
	GLfloat feedBuf[3000];
        GLint vp[4];
	int hideReason; 
        double qcutoff,pickradius;
	int frid;
        QPoint lastPos;
    QStringList resis();
};

#endif // CHGL_H
