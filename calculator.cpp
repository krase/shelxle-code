/****************************************************************************
**
** Copyright (C) 2015 Pavlo V. Solntsev
** All rights reserved.
** Contact: pavlo.solntsev@gmail.com
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
#include "calculator.h"

Calculator::Calculator(double wavel, QWidget *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::Window);
// Set up wavelength box
//    double lambda;
//    QList<QPair<QString,double> > wavelist;
    wavelist = new QList<XraySource>();
    wllist = new QStringList();

    wavelist->push_back(XraySource(XraySource::INPUT));
    wavelist->push_back(XraySource(XraySource::Mo));
    wavelist->push_back(XraySource(XraySource::Cu));
    wavelist->push_back(XraySource(XraySource::Ag));
    wavelist->push_back(XraySource(XraySource::Cr));

    for(QList<XraySource>::iterator it = wavelist->begin();it!=wavelist->end();it++ )
    {
        if(it->name() == "INPUT")
            wllist->push_back("From file | "+ QString::number(wavel,10,4));
        else
            wllist->push_back(it->name()+" | "+ QString::number(it->wavelength(),10,4));
    }
//    QStringList listfordialog;
    wavebox = new QComboBox(this);
    wavebox->addItems(*wllist);
    wavebox->setCurrentIndex(0);

    setWindowTitle(tr("Calculator"));
    mainvlayout = new QVBoxLayout(this);
    setLayout(mainvlayout);
// Create dialog bottom layout
    CloseButton = new QPushButton(tr("&Close"),this);
//    buttonBox->addButton(QDialogButtonBox::Close);
    tabwidget = new QTabWidget(this);
// Put tab widget and bottons into the dialog
    mainvlayout->addWidget(tabwidget);
    mainvlayout->addWidget(CloseButton,0,Qt::AlignRight);
// Add resolution page to the tab widget
    QWidget *reswidget = new QWidget();
    resgrid = new QGridLayout(); // a grid for resolution
    reswidget->setLayout(resgrid);
// Add page for 1,3 distances
    QWidget *dangwidget = new QWidget();
    danglayout = new QVBoxLayout(); // a grid for 1-3 distances
    dangwidget->setLayout(danglayout);
    danggrid = new QGridLayout();
// Prepare resolution dialog
    sourcelabel = new QLabel(QString("Wavelength, %1").arg(QChar(0xC5,0x00)),this);
    dres = new QLabel(QString("d, %1").arg(QChar(0xC5,0x00)), this); // d obtained from Bregs law: 2dsin(Theta) = 1 labda
    sinthoverlm = new QLabel(QString("sin(%1)/%2, %3<sup>-1</sup>").arg(QChar(0xB8,0x03)).arg(QChar(0xBB,0x03)).arg(QChar(0xC5,0x00)), this); // Sin(Theta)/lambda is the same as 1/(2d) obtained from Bregs law: 2dsin(Theta) = 1 labda
    theta = new QLabel(QString("%1,%2").arg(QChar(0xB8,0x03)).arg(QChar(0xB0,0x00)),this); // Theta angle
    twotheta = new QLabel(QString("2%1,%2").arg(QChar(0xB8,0x03)).arg(QChar(0xB0,0x00)),this); // 2Theta
    twotheta->setTextFormat(Qt::RichText);

    dresline = new QLineEdit(this);
    sinthoverlmline  = new QLineEdit(this);
    thetaline = new QLineEdit(this);
    twothetaline = new QLineEdit(this);

// Put all widgets to the resolution grid
    int i(0);
    resgrid->addWidget(sourcelabel,i,0);
    resgrid->addWidget(wavebox,i++,1);
    resgrid->addWidget(dres,i,0);
    resgrid->addWidget(dresline,i++,1);
    resgrid->addWidget(sinthoverlm,i,0);
    resgrid->addWidget(sinthoverlmline,i++,1);
    resgrid->addWidget(theta,i,0);
    resgrid->addWidget(thetaline,i++,1);
    resgrid->addWidget(twotheta,i,0);
    resgrid->addWidget(twothetaline,i++,1);
// put resolution and 1-3 distance widgets to the tab
    tabwidget->insertTab(0,reswidget,tr("Resolution"));
    tabwidget->insertTab(1,dangwidget,tr("1-3 Distances"));

    danglayout->insertLayout(0,danggrid);
    dangpicturelabel = new QLabel(this);
    dangpicturelabel->setPixmap(QPixmap(":icons/dang.png"));
    dangpicturelabel->show();
    danglayout->insertWidget(1,dangpicturelabel);
// Initiate all widgets for 1-3 distances tab
    d1label = new QLabel();
    d2label = new QLabel();
    angle = new QLabel();
    dlabel = new QLabel();

    d1line = new QLineEdit();
    d2line = new QLineEdit();
    angleline = new QLineEdit();
    dline = new QLineEdit();

// Set values for all labels for 1-3 tab widget
    d1label->setText(QString("d1, %1").arg(QChar(0xC5,0x00)));
    d2label->setText(QString("d2, %1").arg(QChar(0xC5,0x00)));
    angle->setText(QString("%1, %2").arg(QChar(0xB1,0x03)).arg(QChar(0xB0,0x00)) );
    dlabel->setText(QString("d, %1").arg(QChar(0xC5,0x00))  );

    i=0;
    danggrid->addWidget(d1label,i,0);
    danggrid->addWidget(d1line,i++,1);
    danggrid->addWidget(d2label,i,0);
    danggrid->addWidget(d2line,i++,1);
    danggrid->addWidget(angle,i,0);
    danggrid->addWidget(angleline,i++,1);
    danggrid->addWidget(dlabel,i,0);
    danggrid->addWidget(dline,i++,1);


// Set defult values for resolution dialog
/*
2dsinA = L => sinA/L = 1/(2d)
Default sinA/L = 0.6
*/
    _sintl = 0.6;
    lambda = wavel;
    sinthoverlmline->setText(QString::number(_sintl));
    sinthoverlmline_changed();

// Show all widgets
    tabwidget->show();
// connect signals for resolution tab
    connect(dresline,SIGNAL(editingFinished()),this,SLOT(dresline_changed()));
    connect(sinthoverlmline,SIGNAL(editingFinished()),this,SLOT(sinthoverlmline_changed()));
    connect(thetaline,SIGNAL(editingFinished()),this,SLOT(thetaline_changed()));
    connect(twothetaline,SIGNAL(editingFinished()),this,SLOT(twothetaline_changed()));
    connect(wavebox,SIGNAL(currentIndexChanged(int)),this,SLOT(wl_changed(int)));
// connect signals for dang tab
    connect(d1line,SIGNAL(editingFinished()),this,SLOT(d1line_changed()));
    connect(d2line,SIGNAL(editingFinished()),this,SLOT(d2line_changed()));
    connect(angleline,SIGNAL(editingFinished()),this,SLOT(angleline_changed()));
    connect(dline,SIGNAL(editingFinished()),this,SLOT(dline_changed()));

    connect(CloseButton, SIGNAL(clicked()), this, SLOT(close()));
//    connect(CancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setFixedSize(sizeHint());
    show();
}

Calculator::~Calculator()
{
    delete CloseButton;
    delete wllist;
    delete wavelist;
    delete dangpicturelabel;
}
/*! \brief Recalculate all values in the line using dresline as a reference
*/
void Calculator::dresline_changed()
{
/* 2dsinA = L
   sinA/L = 1/(2d)
   (sinA/L)^2 = 1/(2d)^2
   theta = arcsin(L/2d)
   2theta = 2arcsin(L/2d)
*/
    _dres = dresline->text().toDouble();
    _sintl = 1/(2*_dres);
    _theta = RAD2DEG(asin(lambda/(2*_dres)));
    _twotheta = 2*_theta;

    sinthoverlmline->setText(QString::number(_sintl,10,3));
    thetaline->setText(QString::number(_theta,10,2));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::sinthoverlmline_changed()
{
/*    sinA/L = 1/(2d) = s
           d =  1/(2s)
  (sinA/L)^2 = s^2
       theta = arcsin(s*L)
      2theta = 2*theta
*/
    _sintl = sinthoverlmline->text().toDouble();
    _dres = 1/(2*_sintl);
    _theta = RAD2DEG(asin(lambda/(2*_dres)));
    _twotheta = 2*_theta;

    dresline->setText(QString::number(_dres,10,3));
    thetaline->setText(QString::number(_theta,10,2));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::thetaline_changed()
{
/*           theta = s
             d = L/(2sin(s))
             sin(s)/L
             (sin(s)/L)^2
             2theta = 2*s
*/
    _theta = thetaline->text().toDouble();
    _dres = lambda/(2*sin(DEG2RAD(_theta)));
    _sintl = (sin(DEG2RAD(_theta)))/lambda;
    _twotheta = 2*_theta;

    dresline->setText(QString::number(_dres,10,3));
    sinthoverlmline->setText(QString::number(_sintl,10,3));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::twothetaline_changed()
{
    /*           2theta = s
                 d = L/(2 sin(s/2))
                 sin(s)/L
                 (sin(s)/L)^2
                 theta = sqrt(s)
    */
    _twotheta = twothetaline->text().toDouble();
    _theta = _twotheta/2;
    _dres = lambda/(2*sin(DEG2RAD(_theta)));
    _sintl = (sin(DEG2RAD(_theta)))/lambda;

    dresline->setText(QString::number(_dres,10,3));
    sinthoverlmline->setText(QString::number(_sintl,10,3));
    thetaline->setText(QString::number(_theta,10,2));
}

void Calculator::wl_changed(int a)
{
/*
    2dsinA = L
    sinA/L = 1/(2d) = s
    d = L/(2sinA)
    sinA = L/(2d) => A = arcsin(L/(2d))

*/
    lambda = wavelist->at(a).wavelength();
    _theta = RAD2DEG(asin(lambda/(2*_dres)));
    _twotheta = 2*_theta;
    thetaline->setText(QString::number(_theta,10,2));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::d1line_changed()
{
    _d1 = d1line->text().toDouble();
//    _d2 = d2line->text().toDouble();
//    _angle = angleline->text().toDouble();
    _d =sqrt(_d1*_d1+_d2*_d2-2*_d1*_d2*cos(DEG2RAD(_angle)));
    dline->setText(QString::number(_d,10,3));
}

void Calculator::d2line_changed()
{
//    _d1 = d1line->text().toDouble();
    _d2 = d2line->text().toDouble();
//    _angle = angleline->text().toDouble();
    _d =sqrt(_d1*_d1+_d2*_d2-2*_d1*_d2*cos(DEG2RAD(_angle)));
    dline->setText(QString::number(_d,10,3));

}

void Calculator::angleline_changed()
{
//    _d1 = d1line->text().toDouble();
//    _d2 = d2line->text().toDouble();
    _angle = angleline->text().toDouble();
    _d =sqrt(_d1*_d1+_d2*_d2-2*_d1*_d2*cos(DEG2RAD(_angle)));
    dline->setText(QString::number(_d,10,3));
}

void Calculator::dline_changed()
{
//    _d1 = d1line->text().toDouble();
//    _d2 = d2line->text().toDouble();
    _d = dline->text().toDouble();
    _angle  = RAD2DEG(acos(((_d*_d)-(_d1*_d1)-(_d2*_d2))/(-2*_d1*_d2)));
    angleline->setText(QString::number(_angle,10,3));
}


XraySource::XraySource(XraySource::Sources source)
{
    switch(source)
    {
    case XraySource::INPUT:
        _name = "INPUT";
        _wavelength = 0;
        _Kalpha1 = 0;
        _Kalpha2 = 0;
        _Kbeta1 = 0;
        _Kbeta2 = 0;
        break;
    case XraySource::Mo:
        _name = "Mo";
        _wavelength = 0.7107;
        _Kalpha1 = 0.70931715;
        _Kalpha2 = 0.713607;
        _Kbeta1 = 0.632303;
        _Kbeta2 = 0.632887;
        break;
    case XraySource::Cu:
        _name = "Cu";
        _wavelength = 1.5418;
        break;
    case XraySource::Ag:
        _name = "Ag";
        _wavelength = 0.5608;
        break;
    case XraySource::Cr:
        _name = "Cr";
        _wavelength = 2.2909;
    default:
        break;
    }
}
