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
    QString respath;
    QFileInfo fileinfo(QString respath);


private:
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
    QHBoxLayout *optionsLayout2;
    QVBoxLayout *optionsLayout3;
    QGridLayout *optionsLayout4;

    //QHBoxLayout *SourceAtomsLayout;
    //QHBoxLayout *TargetAtomsLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *chooserLayout;
    QPushButton *runDSRButton;
    QPushButton *exportFragButton;
    QGroupBox* groupBox1;
    QGroupBox* groupBox2;
    QHBoxLayout* optionboxes;
    QLabel *imageLabel,
           *sourceLabel,
           *searchLabel,
           *partLabel,
           *occLabel,
           *resiLabel,
           *classLabel;
           //*enableResiLabel;
   // QPushButton* runExtButton;
    QStringListModel* fragmentList;  // list of fragments from DSR
    QListView* fragmentListView;     // list of fragments from DSR
    QTableView* fragmentTableView;   // list of fragments from DSR

    QString ResFileName;       // Name of the currently processed res file
    QCheckBox *runExtBox,      // Enable external restraints button
              *InvertFragBox,  // invert the fragment coords button
              *refineBox,      // refine or not after transfer
              *invertFragBox,  // enable/disable inverted fragment coordinates
              *dfixBox,           // enable/disable rigid group instead of restraints
              *rigidGroup;     // enable/disable keep fragment as rigid group

    bool runext;
    bool invert;
    bool norefine;
    bool dfix;
    int part;
    float fvarocc;
    int resinum;
    QString resiclass;

    QString fragname;
    QString dsrpath;
    QString picpath;
public slots:


private slots:
    //bool ResFileOpenDialog();
    void changePicture(QString fragname);
    bool DSRFit();
    void DSRFitExtern(bool checked);
    void InvertFrag(bool checked);
    void RefineOrNot(bool checked);
    bool DSRListFragments(); // list fragments in the DB
    void setFragName(QModelIndex name);
    void searchFragment(QString name);
    bool ExportFrag();
    void DFIX(bool checked);
    void PART(int part);

};




#endif // DSRGUI

