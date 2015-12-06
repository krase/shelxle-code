/*
 ----------------------------------------------------------------------------
 "THE BEER-WARE LICENSE" (Revision 42):
 <daniel.kratzert@ac.uni-freiburg.de> wrote this file. As long as you retain
 this notice you can do whatever you want with this stuff. If we meet some day,
 and you think this stuff is worth it, you can buy me a beer in return.
 Daniel Kratzert
 ----------------------------------------------------------------------------
*/

#ifndef DSRGUI_H
#define DSRGUI_H

#include <QDialog>
#include <QModelIndex>

class QLineEdit;
class QTabWidget;
class QVBoxLayout;
class QGridLayout;
class QHBoxLayout;
class QTextEdit;
class QFileInfo;
class QStatusBar;
class QRadioButton;
class QCheckBox;
class QStringListModel;
class QListView;
class QTableView;
class QLabel;
class QGroupBox;
class QString;
class QPushButton;
class QSpinBox;

class DSRGui : public QWidget
{
    Q_OBJECT

public:
    explicit DSRGui(QWidget *parent = 0);

private:
    void ResFileOpenDialog();
    QTextEdit* outtext;
    QString*   outlist;
    QLineEdit *SearchInp;
    QLineEdit *occEdit;
    QLineEdit *resinumEdit;
    QLineEdit *resiclassEdit;
    QSpinBox  *partspinner; // part definition spinbox

    QVBoxLayout* mainVLayout;
    QGridLayout* editLayout;
    QHBoxLayout *searchLayout1;
    QHBoxLayout *partLayout;
    QHBoxLayout *occLayout;
    QHBoxLayout *resnumLayout;
    QHBoxLayout *resclassLayout;
    QVBoxLayout *optionsLayout1;
    QVBoxLayout *optionsLayout2;
    QVBoxLayout *optionsLayout3;
    QVBoxLayout *optionsLayout4;

    QVBoxLayout *buttonLayout;
    QHBoxLayout *chooserLayout;
    QPushButton *runDSRButton;
    QPushButton *exportFragButton;
    QGroupBox* groupBox1;
    QGroupBox* groupBox2;
    QGroupBox* groupBox3;
    QHBoxLayout* optionboxes;
    QLabel *imageLabel,
           *sourceLabel,
           *searchLabel,
           *partLabel,
           *occLabel,
           *resiLabel,
           *classLabel;

    QStringListModel* fragmentList;  // list of fragments from DSR
    QListView* fragmentListView;     // list of fragments from DSR
    QTableView* fragmentTableView;   // list of fragments from DSR

    QString ResFileName;       // Name of the currently processed res file
    QCheckBox *runExtBox,      // Enable external restraints button
              *InvertFragBox,  // invert the fragment coords button
              *refineBox,      // refine or not after transfer
              *invertFragBox,  // enable/disable inverted fragment coordinates
              *dfixBox,        // enable/disable rigid group instead of restraints
              *rigidGroup,     // enable/disable keep fragment as rigid group
              *replaceMode;    // enable replace mode

    bool runext;
    bool invert;
    bool norefine;
    bool replace;
    QString dfix;
    QString part;
    QString fvarocc;
    QString resistr;
    QString resinum;
    QString resiclass;
    QString fragname;
    QString dsrpath;
    QString picpath;
    QString combiDSRline;

signals:
   void optionTextChanged(void);

public slots:


private slots:
    void changePicture(QString fragname);
    bool DSRFit();
    void DSRFitExtern(bool checked);
    void InvertFrag(bool checked);
    void RefineOrNot(bool checked);
    void ReplaceOrNot(bool checked);
    bool DSRListFragments(); // list fragments in the DB
    void setFragName(QModelIndex name);
    void searchFragment(QString name);
    bool ExportFrag();
    void DFIX(bool checked);
    void setPART(int part);
    void setFvarOcc(QString focc);
    void setResiNum(QString resnum);
    void setResiClass(QString rclass);
    void enableResi(bool enable);
    void combineOptionstext(void);
};




#endif // DSRGUI

