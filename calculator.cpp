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
    setWindowTitle(tr("Calculator"));
// Set up wavelength box
//    double lambda;
//    QList<QPair<QString,double> > wavelist;
    wavelist = new QList<XraySource>();
    wllist = new QStringList();
    xraytype = new QStringList();
    waveline = new QLineEdit();

    wavelist->push_back(XraySource(XraySource::INPUT));// Should be first in this array !
    wavelist->push_back(XraySource(XraySource::Cr));
    wavelist->push_back(XraySource(XraySource::Mn));
    wavelist->push_back(XraySource(XraySource::Fe));
    wavelist->push_back(XraySource(XraySource::Co));
    wavelist->push_back(XraySource(XraySource::Ni));
    wavelist->push_back(XraySource(XraySource::Cu));
    wavelist->push_back(XraySource(XraySource::Ga));
    wavelist->push_back(XraySource(XraySource::Zr));
    wavelist->push_back(XraySource(XraySource::Mo));
    wavelist->push_back(XraySource(XraySource::Ag));
    wavelist->push_back(XraySource(XraySource::In));
    wavelist->push_back(XraySource(XraySource::CUSTOM)); // Should be last in this array !

    for(QList<XraySource>::iterator it = wavelist->begin();it!=wavelist->end();it++ )
    {
        if(it->name() == "INPUT")
            wllist->push_back("From file");
        else
            wllist->push_back(it->name());
    }
//    QStringList listfordialog;
    wavebox = new QComboBox(this);
    wavebox->addItems(*wllist);
    wavebox->setCurrentIndex(0);

    xraytype->push_back(QString("<K%1 1&2>").arg(QChar(0xB1,0x03))); // Kalpha AVERAGE
    xraytype->push_back(QString("K%1 1").arg(QChar(0xB1,0x03))); // Kalpha1
    xraytype->push_back(QString("K%1 2").arg(QChar(0xB1,0x03))); // Kalpha2
    xraytype->push_back(QString("K%1 1").arg(QChar(0xB2,0x03))); // Kbeta1
    xraytype->push_back(QString("K%1 3").arg(QChar(0xB2,0x03))); // Kbeta3

    xraytypebox = new QComboBox(this);
    xraytypebox->addItems(*xraytype);
    xraytypebox->setCurrentIndex(0);

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
    resgrid->addWidget(xraytypebox,i,0);
    resgrid->addWidget(waveline,i++,1);

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
    _shelxlambda = lambda = wavel;
    sinthoverlmline->setText(QString::number(_sintl));
    sinthoverlmline_changed();
    waveline->setText(QString::number(lambda,10,5));

    if(_shelxlambda < 0.001)
    {
        xraytypebox->setEnabled(false);
        waveline->setEnabled(false);
    }

    xraytypebox->setEnabled(false);

// Show all widgets
    tabwidget->show();
// connect signals for resolution tab
    connect(dresline,SIGNAL(editingFinished()),this,SLOT(dresline_changed()));
    connect(sinthoverlmline,SIGNAL(editingFinished()),this,SLOT(sinthoverlmline_changed()));
    connect(thetaline,SIGNAL(editingFinished()),this,SLOT(thetaline_changed()));
    connect(twothetaline,SIGNAL(editingFinished()),this,SLOT(twothetaline_changed()));
    connect(wavebox,SIGNAL(currentIndexChanged(int)),this,SLOT(wl_changed(int)));
    connect(xraytypebox,SIGNAL(currentIndexChanged(int)),this,SLOT(wl_type_changed(int)));


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
    delete xraytype;
    delete waveline;
    delete d1label;
    delete d2label;
    delete angle;
    delete d1line;
    delete d2line;
    delete angleline;
    delete dline;
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

    if(a==0)
    {
        xraytypebox->setEnabled(false);
        if(_shelxlambda  < 0.001)
            waveline->setEnabled(false);

        lambda = _shelxlambda;
//        lambda = wavelist->at(a).wavelength(static_cast<XraySource::XrayType>(xraytypebox->currentIndex()));
        waveline->setText(QString::number(lambda,10,5));
    }
    else
    {
        if(a == (wavelist->size()-1))
        {
            if (xraytypebox->isEnabled())
                xraytypebox->setEnabled(false);
            if(!waveline->isEnabled())
                waveline->setEnabled(true);

            connect(waveline,SIGNAL(editingFinished()),this,SLOT(wl_custom_changed()));
        }
        else
        {
            if(!xraytypebox->isEnabled())
                xraytypebox->setEnabled(true);
            if(!waveline->isEnabled())
                waveline->setEnabled(true);

            lambda = wavelist->at(a).wavelength(static_cast<XraySource::XrayType>(xraytypebox->currentIndex()));
            waveline->setText(QString::number(lambda,10,5));
        }
    }

    _theta = RAD2DEG(asin(lambda/(2*_dres)));
    _twotheta = 2*_theta;
    thetaline->setText(QString::number(_theta,10,2));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::wl_type_changed(int a)
{
    lambda = wavelist->at(wavebox->currentIndex()).wavelength(static_cast<XraySource::XrayType>(a));
    waveline->setText(QString::number(lambda,10,5));

    _theta = RAD2DEG(asin(lambda/(2*_dres)));
    _twotheta = 2*_theta;
    thetaline->setText(QString::number(_theta,10,2));
    twothetaline->setText(QString::number(_twotheta,10,2));
}

void Calculator::wl_custom_changed()
{
   lambda = waveline->text().toDouble();
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
/*
All wavelengths were takedn from
Internationl Union of Crystallography Vol. C (2004)
Table 4.2.2.1
*/
    switch(source)
    {
    case XraySource::INPUT:
        _name = "INPUT";
//        _wavelength = 0;
        _Kalpha1 = 0;
        _Kalpha2 = 0;
        _Kbeta1 = 0;
        _Kbeta3 = 0;
        break;
    case XraySource::Mg:
        _name = "Mg";
//        _wavelength = 0.7107;
        _Kalpha1 = 9.889555;
        _Kalpha2 = 9.89153;
        _Kbeta1 = 0;
        _Kbeta3 = 0;
        break;
    case XraySource::Al:
        _name = "Al";
        _Kalpha1 = 8.339515;
        _Kalpha2 = 8.341832;
        _Kbeta1 = 0;
        _Kbeta3 = 0;
        break;
    case XraySource::Si:
        _name = "Si";
        _Kalpha1 = 7.125589;
        _Kalpha2 = 7.12801;
        _Kbeta1 = 0;
        _Kbeta3 = 0;
        break;
    case XraySource::Cr:
        _name = "Cr";
        _Kalpha1 = 2.2897260;
        _Kalpha2 = 2.2936510;
        _Kbeta1 = 2.0848810;
        _Kbeta3 = 2.0848810 ;
        break;
    case XraySource::Mn:
        _name = "Mn";
        _Kalpha1 = 2.1018540;
        _Kalpha2 = 2.1058220;
        _Kbeta1 = 1.9102160;
        _Kbeta3 = 1.9102160 ;
        break;
    case XraySource::Fe:
        _name = "Fe";
        _Kalpha1 = 1.9360410;
        _Kalpha2 = 1.9399730;
        _Kbeta1 = 1.7566040;
        _Kbeta3 = 1.7566040;
        break;
    case XraySource::Co:
        _name = "Co";
        _Kalpha1 = 1.7889960;
        _Kalpha2 = 1.7928350;
        _Kbeta1 = 1.6208260;
        _Kbeta3 = 1.6208260 ;
        break;
    case XraySource::Ni:
        _name = "Ni";
        _Kalpha1 = 1.6579300;
        _Kalpha2 = 1.6617560;
        _Kbeta1 = 1.5001520;
        _Kbeta3 = 1.5001520 ;
        break;
    case XraySource::Cu:
        _name = "Cu";
        _Kalpha1 = 1.5405930;
        _Kalpha2 = 1.5444275;
        _Kbeta1 = 1.392235;
        _Kbeta3 = 1.392235 ;
        break;
    case XraySource::Ga:
        _name = "Ga";                
        _Kalpha1 = 1.3401270;
        _Kalpha2 = 1.3440260;
        _Kbeta1  = 1.207930;
        _Kbeta3  = 1.208390;
        break; 
    case XraySource::Zr:
        _name = "Zr";
        _Kalpha1 = 0.785958;
        _Kalpha2 = 0.790179;
        _Kbeta1 = 0.701801;
        _Kbeta3 = 0.702355 ;
        break;
    case XraySource::Mo:
        _name = "Mo";
        _Kalpha1 = 0.7093172;
        _Kalpha2 = 0.71361;
        _Kbeta1 = 0.632303;
        _Kbeta3 = 0.632887;
        break;
    case XraySource::Ag:
        _name = "Ag";
        _Kalpha1 = 0.5594218;
        _Kalpha2 = 0.563813;
        _Kbeta1 = 0.497082;
        _Kbeta3 = 0.497697;
        break;
    case XraySource::In:
        _name = "In";
        _Kalpha1 = 0.512126;
        _Kalpha2 = 0.516558;
        _Kbeta1 = 0.454562;
        _Kbeta3 = 0.455197;
        break;
    case XraySource::CUSTOM:
        _name = "Custom";
        _Kalpha1 = 0;
        _Kalpha2 = 0;
        _Kbeta1 = 0;
        _Kbeta3 = 0;
        break;
    default:
        break;
    }
}

double XraySource::wavelength(XraySource::XrayType type) const
{
    double res(0);

    switch (type) {
    case XraySource::WAVELENGTH:
        res = (_Kalpha1*2+_Kalpha2)/3;
        break;
    case XraySource::KALPHA1:
        res =  _Kalpha1;
        break;
    case XraySource::KALPHA2:
        res =  _Kalpha2;
        break;
    case XraySource::KBETA1:
        res =  _Kbeta1;
        break;
    case XraySource::KBETA3:
        res =  _Kbeta3;
        break;
    default:
        res = (_Kalpha1*2+_Kalpha2)/3;
        break;
    }
    return res;
}
