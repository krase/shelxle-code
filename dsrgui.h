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
    QLineEdit* refineEdit; //
    QLineEdit* resEdit; //
    //QLineEdit* fragNameInp; // fragment name of fitting fragment
    QLineEdit* SearchInp; // search input field
    QLineEdit* S1; // source atom 1
    QLineEdit* S2; // source atom 2
    QLineEdit* S3; // source atom 3
    QLineEdit* T1; // target atom 1
    QLineEdit* T2; // target atom 2
    QLineEdit* T3; // target atom 3

    QVBoxLayout* mainVLayout;
    QGridLayout* editLayout;
    QHBoxLayout *optionsLayout1,
                *optionsLayout2,
                *optionsLayout3,
                *SourceAtomsLayout,
                *TargetAtomsLayout,
                *buttonLayout,
                *chooserLayout;
    QLabel *imageLabel;
    QPushButton* FileOpenButton;
    QStringListModel* fragmentList;  // list of fragments from DSR
    QListView* fragmentListView;  // list of fragments from DSR
    QTableView* fragmentTableView;  // list of fragments from DSR

    QString ResFileName; // Name of the currently processed res file
    QCheckBox* runExtBox;  // Enable external restraints button
    QCheckBox* InvertFragBox;  // invert the fragment coords button
    QCheckBox* refineBox; // refine or not after transfer

    bool runext;
    bool invert;
    bool norefine;

    QString fragname;
    QString dsrpath;
public slots:


private slots:
    bool ResFileOpenDialog();
    void changePicture(QString fragname);
    bool DSRFit();
    void DSRFitExtern(bool checked);
    void InvertFrag(bool checked);
    void RefineOrNot(bool checked);
    bool DSRListFragments(); // list fragments in the DB
    void setFragName(QModelIndex name);
    void searchFragment(QString name);
    bool ExportFrag();
    //bool insertDSRLine();

};




#endif // DSRGUI

