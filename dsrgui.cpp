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
 - fix pixmap problem when pic is not there
 - find solution for "which dsr"
 -
 - add RESI, DFIX, CF3, CF6, CF9, OCC, PART
 - REPLACE
*/


DSRGui::DSRGui(QWidget *parent):
    QWidget(parent)
{
    dsrpath = "D:/GitHub/DSR/dsr.bat";
    //dsrpath = "/Applications/DSR/dsr";
    QString picpath = "D:/Programme/DSR/fragments";
    this->ResFileName = QString("D:/Programme/DSR/example/p21c.res");
    mainVLayout = new QVBoxLayout(this);
    editLayout = new QGridLayout;
    chooserLayout = new QHBoxLayout;
    SourceAtomsLayout = new QHBoxLayout;
    optionsLayout1 = new QHBoxLayout;
    optionsLayout2 = new QHBoxLayout;
    optionsLayout3 = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;

    // layout for the interactions
    mainVLayout->addLayout(chooserLayout);
    mainVLayout->addLayout(editLayout);
    // layout for all the options
    mainVLayout->addLayout(optionsLayout1);
    mainVLayout->addLayout(optionsLayout2);
    mainVLayout->addLayout(optionsLayout3);
    // layout for the buttons
    mainVLayout->addLayout(buttonLayout);

    //QPushButton* FileOpenButton = new QPushButton(tr("SHELXL .res file"));

    QCheckBox* runExtButton = new QCheckBox(tr("External Restraints"));
    QCheckBox* invertFragBox = new QCheckBox(tr("Invert Coordinates"));
    QCheckBox* refineBox = new QCheckBox(tr("Do not Refine"));

    QLabel* sourceLabel = new QLabel(tr("Source Atoms:"));
    QLabel* searchLabel = new QLabel(tr("Search Fragment:"));

    //QPushButton* listFragsButton = new QPushButton(tr("List Fragments"));

    resEdit = new QLineEdit;
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
    "##################################################################################"));
    //fragNameInp = new QLineEdit;
    SearchInp = new QLineEdit;

    chooserLayout->addWidget(fragmentTableView);
    QPixmap Imagefile = QString("D:/GitHub/shelxle-code/fragments/9-bromphen.png");
    //QPixmap Imagefile = QString("/Users/daniel/Downloads/Daniel_Kratzert.tiff");
    imageLabel = new QLabel();
    chooserLayout->addWidget(imageLabel);
    if (Imagefile.size() != QSize(0,0))
    {
        Imagefile = Imagefile.scaledToWidth(400, Qt::SmoothTransformation);
        imageLabel->setMinimumSize(400, 400);
        imageLabel->setPixmap(Imagefile);
    } else {
        imageLabel->setMinimumSize(400, 400);
    }

    // fill editLayout with widgets:
    editLayout->addWidget(outtext, 1, 0);

    //fragNameInp->setMaximumWidth(200);

    S1 = new QLineEdit;
    S2 = new QLineEdit;
    S3 = new QLineEdit;
    SourceAtomsLayout->addWidget(sourceLabel);
    sourceLabel->setAlignment(Qt::AlignRight);
    SourceAtomsLayout->addWidget(S1);
    SourceAtomsLayout->addWidget(S2);
    SourceAtomsLayout->addWidget(S3);
    SourceAtomsLayout->addStretch();

    S1->setMaximumWidth(40);
    S2->setMaximumWidth(40);
    S3->setMaximumWidth(40);

    QPushButton* runDSRButton = new QPushButton(tr("Run!"));
    QPushButton* exportFragButton = new QPushButton(tr("Export Fragment"));

    optionsLayout1->addLayout(SourceAtomsLayout);
    optionsLayout1->addStretch();

    optionsLayout2->addWidget(runExtButton);
    optionsLayout2->addWidget(invertFragBox);
    optionsLayout2->addWidget(refineBox);
    optionsLayout2->addStretch();

    optionsLayout3->addWidget(searchLabel);
    optionsLayout3->addWidget(SearchInp);
    optionsLayout3->addStretch();

    buttonLayout->addStretch();
    buttonLayout->addWidget(exportFragButton);
    buttonLayout->addWidget(runDSRButton);
    buttonLayout->addSpacing(10);

    DSRListFragments();

    // Signal slot connections:
    connect(runDSRButton, SIGNAL (clicked(bool)),
            this, SLOT (DSRFit()));
    connect(runExtButton, SIGNAL (clicked(bool)),
            this, SLOT (DSRFitExtern(bool)));
    connect(invertFragBox, SIGNAL (clicked(bool)),
            this, SLOT (InvertFrag(bool)));
    connect(refineBox, SIGNAL (clicked(bool)),
            this, SLOT (RefineOrNot(bool)));
    connect(SearchInp, SIGNAL(textChanged(QString)),
            this, SLOT(searchFragment(QString)));
    connect(exportFragButton, SIGNAL(clicked(bool)),
            this, SLOT(ExportFrag(void)));
    connect(fragmentTableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(setFragName(QModelIndex)));
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
{
    QPixmap Imagefile = QString("D:/GitHub/shelxle-code/fragments/"+fragname+".png");
    Imagefile = Imagefile.scaledToWidth(400, Qt::SmoothTransformation);
    imageLabel->setPixmap(Imagefile);
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
    //qDebug() << runext << invert;
    //qDebug() << ResFileName;
    if (ResFileName.isEmpty())
    {
        outtext->append("No res file chosen. Doing nothing!");
        return false;
    }
    if (!runext and !invert) // the standard run without extra options
    {
        outtext->append("running normal");
        if (norefine)       // with norefine option
        {
            outtext->append("nor refine");
            dsr.start(dsrpath, QStringList() << QString("-r -n") << ResFileName);
        } else {           // ordinary run
            outtext->append("with refine");
            dsr.start(dsrpath, QStringList() << QString("-r") << ResFileName);
        }
    }
    else if (runext and !invert) // with external restraints
    {
        outtext->append("running external");
        if (norefine) // with norefine option enabled
        {
            dsr.start(dsrpath, QStringList() << QString("-re -n") << ResFileName);
        } else {
            dsr.start(dsrpath, QStringList() << QString("-re") << ResFileName);
        }
    }
    else if (!runext and invert)
    {
        outtext->append("running normal + invert");
        if (norefine)  // norefine enabled and inverted
        {
            dsr.start(dsrpath, QStringList() << QString("-r -t -n") << ResFileName);
        } else {       // regular but inverted
            dsr.start(dsrpath, QStringList() << QString("-r -t") << ResFileName);
        }
    }
    else if (runext and invert) // external restraints and inverted
    {
        outtext->append("running external + inverted");
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

