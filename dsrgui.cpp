/*
 ----------------------------------------------------------------------------
 "THE BEER-WARE LICENSE" (Revision 42):
 <daniel.kratzert@ac.uni-freiburg.de> wrote this file. As long as you retain
 this notice you can do whatever you want with this stuff. If we meet some day,
 and you think this stuff is worth it, you can buy me a beer in return.
 Daniel Kratzert
 ----------------------------------------------------------------------------
*/

#include "dsrgui.h"
#include <QtGui>

/*
 - get atom names of selected atoms in ShelXle for target atoms
 - add dsr line to cursor position of cursor in ShelXle
 - fill in dsr line
 - run DSR with command line options
 - show results

 - use insertDFIX like method to insert dsrline into file
 - find solution for "which dsr"

 - add groupBoxes for options
 - make outtext as toggle field. (show/hide output button?) blink button on activity?
 -
 - export und run in dritte spalte
 - statusleiste mit "rem dsr put ..."
 - I need a checkbox to simply activate RESI without other options:
   * use residue checkbox creates number and class field

*/


DSRGui::DSRGui(QWidget *parent):
    QWidget(parent)
{
    setWindowTitle(QString("DSR GUI"));
    //dsrpath = "D:/GitHub/DSR/dsr.bat";
    dsrpath = "dsr.bat";
    picpath = "C:/Users/daniel/Documents/GitHub/shelxle-code/fragments/";
    ResFileName = "C:/Program Files/DSR/example/p21c.res";
    QPixmap Imagefile = QString("C:/Users/daniel/Documents/GitHub/shelxle-code/fragments/9-bromphen.png");
    mainVLayout = new QVBoxLayout(this);
    editLayout = new QGridLayout;
    chooserLayout = new QHBoxLayout;
    searchLayout1 = new QHBoxLayout;
    partLayout = new QHBoxLayout;
    occLayout = new QHBoxLayout;
    resnumLayout = new QHBoxLayout;
    resclassLayout = new QHBoxLayout;
    optionsLayout1 = new QVBoxLayout;
    optionsLayout2 = new QVBoxLayout;
    optionsLayout3 = new QVBoxLayout;
    optionsLayout4 = new QVBoxLayout;
    groupBox1 = new QGroupBox(tr("Chose Options"));
    groupBox2 = new QGroupBox(tr("Chose Options"));
    groupBox3 = new QGroupBox(tr("Use a Residue"));
    buttonLayout = new QVBoxLayout;
    optionboxes = new QHBoxLayout;

    // layout for the interactions
    mainVLayout->addLayout(chooserLayout);
    mainVLayout->addLayout(editLayout);
    optionboxes->addWidget(groupBox1);
    optionboxes->addWidget(groupBox2);
    optionboxes->addWidget(groupBox3);
    optionboxes->addLayout(buttonLayout);
    optionboxes->addStretch();
    mainVLayout->addLayout(optionboxes);

    runDSRButton = new QPushButton(tr("Fit Fragment!"));
    exportFragButton = new QPushButton(tr("Export Fragment"));

    runExtBox = new QCheckBox(tr("External Restraints"));
    invertFragBox = new QCheckBox(tr("Invert Coordinates"));
    refineBox = new QCheckBox(tr("Do not Refine"));
    dfixBox = new QCheckBox(tr("Calc. DFIX"));
    replaceMode = new QCheckBox(tr("Replace Target"));

    sourceLabel = new QLabel(tr("Source Atoms:"));
    searchLabel = new QLabel(tr("Search Fragment:"));
    SearchInp = new QLineEdit;
    partLabel = new QLabel(tr("PART:"));
    occLabel = new QLabel(tr("FVAR+Occupancy:"));
    resiLabel = new QLabel(tr("Residue Number:"));
    classLabel = new QLabel(tr("Residue Class:"));

    outtext = new QTextEdit;
    fragmentList = new QStringListModel;
    fragmentListView = new QListView;
    fragmentTableView = new QTableView;
    outtext->setReadOnly(false);
    QFont font("Monospace", 11);
    font.setStyleHint(QFont::TypeWriter);
    outtext->setFont(font);
    // this is a strange hack to define the outtext width:
    outtext->setMinimumWidth(QFontMetrics(outtext->font()).width(
    "###################################################################################"));
    //fragNameInp = new QLineEdit;

    chooserLayout->addWidget(fragmentTableView);
    imageLabel = new QLabel();
    chooserLayout->addWidget(imageLabel);
    if (Imagefile.size() != QSize(0,0))
    {
        Imagefile = Imagefile.scaledToWidth(350, Qt::SmoothTransformation);
        imageLabel->setMinimumSize(380, 380);
        imageLabel->setPixmap(Imagefile);
    } else {
        imageLabel->setMinimumSize(380, 380);
    }

    // fill editLayout with widgets:
    editLayout->addWidget(outtext, 1, 0);

    occEdit = new QLineEdit;
    partspinner = new QSpinBox;
    resinumEdit = new QLineEdit;
    resiclassEdit = new QLineEdit;

    occEdit->setMaximumWidth(45);
    resinumEdit->setMaximumWidth(40);
    resiclassEdit->setMaximumWidth(50);

    searchLayout1->addStretch();
    searchLayout1->addWidget(searchLabel);
    searchLayout1->addWidget(SearchInp);

    partLayout->addStretch();
    partLayout->addWidget(partLabel);
    partLayout->addWidget(partspinner);

    occLayout->addWidget(occLabel);
    occLayout->addWidget(occEdit);
    occLabel->setAlignment(Qt::AlignRight);

    optionsLayout1->addLayout(partLayout);
    optionsLayout1->addLayout(occLayout);
    optionsLayout1->addLayout(searchLayout1);
    optionsLayout1->addStretch();
    groupBox1->setLayout(optionsLayout1);

    optionsLayout2->addWidget(runExtBox);
    optionsLayout2->addWidget(invertFragBox);
    optionsLayout2->addWidget(refineBox);
    optionsLayout2->addWidget(dfixBox);
    optionsLayout2->addStretch();
    groupBox2->setLayout(optionsLayout2);

    resnumLayout->addWidget(resiLabel);
    resnumLayout->addWidget(resinumEdit);
    resclassLayout->addWidget(classLabel);
    resclassLayout->addWidget(resiclassEdit);

    optionsLayout3->addLayout(resnumLayout);
    optionsLayout3->addLayout(resclassLayout);
    optionsLayout3->addStretch();
    partspinner->setRange(-99, 99);
    partspinner->setValue(1);
    classLabel->setAlignment(Qt::AlignRight);
    resiLabel->setAlignment(Qt::AlignRight);
    groupBox3->setLayout(optionsLayout3);  // warum geht das nicht?
    groupBox3->setCheckable(true);

    buttonLayout->addWidget(runDSRButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(exportFragButton);
    buttonLayout->addStretch();


    DSRListFragments();

    // Signal slot connections:
    connect(runDSRButton, SIGNAL (clicked(bool)),
            this, SLOT (DSRFit()));
    connect(runExtBox, SIGNAL (clicked(bool)),
            this, SLOT (DSRFitExtern(bool)));
    connect(dfixBox, SIGNAL (clicked(bool)),
            this, SLOT (DFIX(bool)));
    connect(invertFragBox, SIGNAL (clicked(bool)),
            this, SLOT (InvertFrag(bool)));
    connect(refineBox, SIGNAL (clicked(bool)),
            this, SLOT (RefineOrNot(bool)));
    connect(SearchInp, SIGNAL(textChanged(QString)),
            this, SLOT(searchFragment(QString)));
    connect(occEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setFvarOcc(QString)));
    connect(resinumEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setResiNum(QString)));
    connect(resiclassEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setResiClass(QString)));
    connect(partspinner, SIGNAL(valueChanged(int)),
            this, SLOT(setPART(int)));
    connect(exportFragButton, SIGNAL(clicked(bool)),
            this, SLOT(ExportFrag(void)));
    connect(fragmentTableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(setFragName(QModelIndex)));
    connect(groupBox3, SIGNAL(toggled(bool)),
            this, SLOT(enableResi(bool)));
}

void DSRGui::enableResi(bool enable)
//
{  outtext->clear();
    if (enable)
    {
        resistr = QString("RESI ")+resinum+" "+resiclass;
    } else {
        resistr = QString("");
    }
  outtext->append(resistr);
}

void DSRGui::setFragName(QModelIndex name)
// set the fragment name variable
{
    fragname = name.sibling(name.row(), 0).data().toString();
    outtext->clear();
    outtext->append(fragname);
    this->changePicture(fragname);
}

void DSRGui::changePicture(QString fragname)
// changes the picture of the fragment to fragname
{
    QPixmap Imagefile = QString(picpath+fragname+".png");
    Imagefile = Imagefile.scaledToWidth(400, Qt::SmoothTransformation);
    imageLabel->setPixmap(Imagefile);
}

void DSRGui::DFIX(bool checked)
// toggles the dfix option
{
    if (checked)
    {
        this->dfix = QString("DFIX");
    } else
    {
        this->dfix = QString("");
    }
    outtext->clear();
    outtext->append(QString(dfix));
}

void DSRGui::setResiClass(QString rclass)
// defines the PART option
{
  outtext->clear();
    if (rclass[0].isLetter())
    {
        if (rclass.length() > 4) {
            resiclass = QString(rclass.mid(0, 4));
        } else {
            resiclass = QString(rclass);
        }
    } else {
        resiclass = QString("");
    }
    this->enableResi(true);
  //outtext->append(resiclass);
}

void DSRGui::setResiNum(QString resnum)
// defines the residue number
{
  outtext->clear();
    resinum = QString("");
    bool ok;
    resnum.toInt(&ok);
    if (ok)
    {
        resinum = QString::number(resnum.toInt());
    } else if (!ok and resnum.length() >= 1) {
        outtext->append(QString("Please provide an integer number as residue number."));
        resinum = QString("");
    } else if (ok and resnum.toInt() == 0) {
        resinum = QString("");
    } else if (!ok and resnum.length() == 0) {
        resinum = QString("");
    }
    this->enableResi(true);
  //outtext->append(resinum);
}

void DSRGui::setFvarOcc(QString focc)
// defines the PART option
{
  outtext->clear();
    bool ok;
    focc.toFloat(&ok);
    if (ok)
    {
        fvarocc = QString("OCC ")+QString::number(focc.toFloat());
    } else if (!ok and focc.length() >= 1) {
        outtext->append(QString("Please provide a real number for FVAR/OCC"));
    } else if (!ok and focc.length() == 0) {
        fvarocc = QString("");
    }
  outtext->append(fvarocc);
}

void DSRGui::setPART(int partnum)
// defines the PART option
{
    if (partnum == 0)
    {
        part = QString("");
    }
    else
    {
        QString s = QString::number(partnum);
        part = QString("PART ")+s ;
        outtext->clear();
        outtext->append(part);
    }
}

void DSRGui::DSRFitExtern(bool checked)
// enable write restraints to external file
{
    if (checked)
    {
        this->runext = true;
    } else
    {
        this->runext = false;
    }
}

void DSRGui::InvertFrag(bool checked)
// invert the fragment coordinates
{
    if (checked)
    {
        this->invert = true;
    } else
    {
        this->invert = false;
    }
}

bool DSRGui::ExportFrag()
// export the current fragment
// TODO: file chooser to chose the target directory
{
    outtext->clear();
    if (fragname.isEmpty())
    {
        outtext->append("No fragment chosen. Doing nothing!");
        return false;
    }
    QProcess dsr;
    outtext->append("beginning export:");
    outtext->append(fragname);
    dsr.start(dsrpath, QStringList() << QString("-e") << fragname);
    dsr.setProcessChannelMode(QProcess::MergedChannels);
    dsr.closeWriteChannel();
    outtext->clear();
    if (!dsr.waitForFinished())
    {
        outtext->append("Unable to start DSR.");
        outtext->append(dsr.readAll());
        return false;
    } else {
        outtext->append(dsr.readAll());
    }
    return true;
}

void DSRGui::RefineOrNot(bool checked)
// enable or disable refinement after transfer
{
    if (checked)
    {
        this->norefine = true;
    } else
    {
        this->norefine = false;
    }
}


bool DSRGui::DSRFit()
{
    QProcess dsr;
    dsr.setProcessChannelMode(QProcess::MergedChannels);
    dsr.closeWriteChannel();
    outtext->clear();
    if (ResFileName.isEmpty())
    {
        outtext->append("No res file chosen. Doing nothing!");
        return false;
    }
    if (!runext and !invert) // the standard run without extra options
    {
        if (norefine)       // with norefine option
        {
            dsr.start(dsrpath, QStringList() << QString("-r -n") << ResFileName);
        } else {           // ordinary run
            dsr.start(dsrpath, QStringList() << QString("-r") << ResFileName);
        }
    }
    else if (runext and !invert) // with external restraints
    {
        if (norefine) // with norefine option enabled
        {
            dsr.start(dsrpath, QStringList() << QString("-re -n") << ResFileName);
        } else {
            dsr.start(dsrpath, QStringList() << QString("-re") << ResFileName);
        }
    }
    else if (!runext and invert)
    {
        if (norefine)  // norefine enabled and inverted
        {
            dsr.start(dsrpath, QStringList() << QString("-r -t -n") << ResFileName);
        } else {       // regular but inverted
            dsr.start(dsrpath, QStringList() << QString("-r -t") << ResFileName);
        }
    }
    else if (runext and invert) // external restraints and inverted
    {
        if (norefine)
        {
            dsr.start(dsrpath, QStringList() << QString("-re -t -n") << ResFileName);
        } else {
            dsr.start(dsrpath, QStringList() << QString("-re -t") << ResFileName);
        }
    } else {
        qDebug() << "Unhandeled option case in DSRGui occoured!!";
    }
    if (!dsr.waitForFinished())
    {
        outtext->append("Unable to start DSR.");
        outtext->append(dsr.readAll());
    } else {
        outtext->append(dsr.readAll());
    }
    return true;
}


void DSRGui::searchFragment(QString name)
// search for fragments in the db
{
    if (name.length() < 3)
    {
        return;
    }
    QProcess dsr;
    dsr.start(dsrpath, QStringList() << "-s" << name);
    dsr.setProcessChannelMode(QProcess::MergedChannels);
    dsr.closeWriteChannel();
    outtext->clear();
    if (!dsr.waitForFinished())
    {
        outtext->append("Unable to start DSR.");
        outtext->append(dsr.readAll());
    } else {
        outtext->append(dsr.readAll());
    }
}

bool DSRGui::DSRListFragments()
// list fragments in DSR database
{
    outlist = new QString;
    QProcess dsr;
    dsr.start(dsrpath, QStringList() << "-lc");
    dsr.setProcessChannelMode(QProcess::MergedChannels);
    dsr.closeWriteChannel();
    outtext->clear();
    outlist->clear();
    if (!dsr.waitForFinished())
    {
        outtext->append("Unable to start DSR.");
        outtext->append(dsr.readAll());
    } else {
        outlist->append(dsr.readAll());
    }
    QStringList list = outlist->split(QRegExp("\n|\r\n|\r"));
    //split(";");
    fragmentListView->setModel(fragmentList);
    //fragmentList->setStringList(list2);
    //qDebug() << list.size() << "lines";
    QStandardItemModel *FragListmodel = new QStandardItemModel(list.size()-1, 2, this); //x Rows and 2 Columns
    FragListmodel->setHorizontalHeaderItem(0, new QStandardItem(QString("tag")));
    FragListmodel->setHorizontalHeaderItem(1, new QStandardItem(QString("Fragment Name")));
    for (int i = 0; i < list.size()-1; ++i)
    {
        QString line1 = list[i].split(";;")[0];
        QString line2 = list[i].split(";;")[2];
        FragListmodel->setVerticalHeaderItem(i, new QStandardItem(QString("")));
        FragListmodel->setItem(i, 0, new QStandardItem(QString(line1)));
        FragListmodel->setItem(i, 1, new QStandardItem(QString(line2)));
    }
    fragmentTableView->setModel(FragListmodel);
    fragmentTableView->resizeColumnsToContents();
    fragmentTableView->hideColumn(0);
    //fragmentTableView->setFrameStyle(0);
    fragmentTableView->setGridStyle(Qt::PenStyle(Qt::NoPen));
    fragmentTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    return true;
}

/*
bool Window::insertDSRLine(){
// Insert the DSR command line at cursor position:
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        cursor.insertText("REM This is a test!\n");
        return fileSave();
}
*/

/*
bool DSRGui::ResFileOpenDialog()
// File dialog to define the res file processed by DSR
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open SHELXL .res file"), "", tr("SHELXL .res file (*.res *.RES)"));
    if (fileName.isEmpty())
        return false;
    this->resEdit->setText(fileName);
    //QFile file(fileName);
    //qDebug() << fileName;
    this->ResFileName = fileName;
    return true;
}
*/
