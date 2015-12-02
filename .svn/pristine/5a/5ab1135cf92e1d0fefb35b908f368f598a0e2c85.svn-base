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
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QList>
#include <QPair>
#include <QStringList>
#include <QComboBox>
#include <QString>
#include <cmath>
// some compilers do not have M_PI defined
#ifndef M_PI
#define	M_PI		3.14159265358979323846
#endif

#define DEG2RAD(x) M_PI/180*x
#define RAD2DEG(x) 180/M_PI*x

#define NM2EV(x) 12398.41858/x
#define NM2KEV(x) 12.39841858/x
#define EV2NM(x) 12398.41858/x
#define KEV2NM(x) 12.39841858/x

//! XraySource class holds information about source of radiation
/*!
 The class is designed to store information about source. For now,
 it supports only one wavelength, but in the future it may and should
 be redisigned to hold all components of K edge radiation.
 */
class XraySource
{
public:
    //! A source of the radiation
    /*! Predefined sources that can be supported by the class */
    enum Sources{
        INPUT = 0, /*! Information from the input file */
        Mg,
        Al,
        Si,
        Cr,  /*! Cr radiation */
        Mn,
        Fe,
        Co,
        Ni,
        Cu, /*! Cu radiation */
        Ga, /*! Ga radiation (for metal jet) */
        Zr,
        Mo, /*! Mo radiation */
        Ag, /*! Ag radiation */
        In,
        CUSTOM
                };
    // Do not change this order (!)
    enum XrayType {
        WAVELENGTH= 0,
        KALPHA1,
        KALPHA2,
        KBETA1,
        KBETA3
    };
    //! Contstructor to create the object.
    XraySource(XraySource::Sources source = Mo);
    ~XraySource(){}
    inline QString name()const{return _name;}
    double wavelength(XraySource::XrayType type = WAVELENGTH)const;

private:
    QString _name; //!< Name of the X-ray source. Available values: INPUT, Mo, Cu, Ag, Cr
    double _Kalpha1;
    double _Kalpha2;
    double _Kbeta1;
    double _Kbeta3;
};

//! Calculator class desinged to support a dialog for Crystallographic Calculator
/*!
    A main class for Crystallographic Calculator.
*/
class Calculator : public QWidget
{
    Q_OBJECT
public:
    //! Constructor to initiate the class.
    /*!
        Wavelength from the SHELX file should be provided.
    */
    explicit Calculator(double wavel, QWidget *parent = 0);
    //! Wavelength can be changed using this method
    /*!
        \param wavelength is a wavelength of the X-ray source.
    */
    inline void setwavelength(double &wavelength){lambda = wavelength;}
    //! Wavelength can be changed using this method
    /*!
        \param wavelength is a wavelength of the X-ray source.
    */
//    inline void setwavelength(double wavelength){lambda = wavelength;}
    //! Wavelength can be changed using this method
    /*!
        \param wavelength is a wavelength of the X-ray source.
    */
    inline void setwavelength(const double &wavelength){lambda = wavelength;}
    //! Wavelength can be changed using this method
    /*!
        \param wavelength is a wavelength of the X-ray source.
    */
    inline void setwavelength(const double wavelength){lambda = wavelength;}
    virtual ~Calculator();
signals:
    
public slots:
    void dresline_changed(); //!< Update values if dresline has been changed
    void sinthoverlmline_changed(); //!< Update values if sin(theta)/lambda has been changed
    void thetaline_changed();  //!< Update values if Theta has been changed
    void twothetaline_changed();  //!< Update values if 2Theta has been changed
    void wl_changed(int a);  //!< Update values if X-ray source has been changed
    void wl_type_changed(int a); //!< Update values for current wavelength
    void wl_custom_changed(); //!< manual wavelength setup

    void d1line_changed(); //!< Update values if d1 has been changed
    void d2line_changed();  //!< Update values if d2 has been changed
    void angleline_changed();  //!< Update values if angle has been changed
    void dline_changed(); //!< Update angle if d distance was changed. d1 and d2 remain the same

private:
    QVBoxLayout *mainvlayout; //!< Main container to keep buttons and tab widget
    QDialogButtonBox *buttonBox; //!< Container for dialog button
    QPushButton *CloseButton; //!< Close button
    QTabWidget *tabwidget; //!< A tab widget
    QGridLayout *resgrid; //!< Layout for resolution page of the tab widget

    QLabel *dres; //!< d obtained from Bregs law: 2dsin(Theta) = labda
    QLabel *sinthoverlm; //!< Sin(Theta)/labda is the same as 1/(2d) obtained from Bregs law: 2dsin(Theta) = 1 labda
    QLabel *theta; //!< Theta angle
    QLabel *twotheta; //!< 2Theta angle
    QLabel *sourcelabel; //!< Wavelength selector

    QLineEdit *dresline; //!< input line for dres.
    QLineEdit *sinthoverlmline; //!< input line for sin(Theta)/lambda
    QLineEdit *thetaline; //!< input line for theta angle
    QLineEdit *twothetaline; //!< input line for 2theta angle
    QStringList *wllist; //!< String list to store wavelength sources for the dialog
    QStringList *xraytype; //!< Type of X-ray wave: Kalpha1,Kalpha2, etc
    QList<XraySource> *wavelist; //!< Interal container to store wavelengths
    QComboBox *wavebox; //!< Widget to select an appropriate source
    QComboBox *xraytypebox; //!< Widget to select an appropriate type of radiation
    QLineEdit *waveline; //!< line to output wavelength number
    QLabel *dangpicturelabel; //!< widget to hold a picture for dang

// private members for resolution tab
    double lambda; //!< wavelength
    double _dres; //!< Resolution
    double _sintl; //!< sin(theta)/lambda
    double _theta; //!< theta angle
    double _twotheta; //!< 2theta angle
    double _shelxlambda; //!< Wavelength from the SHELX file
// 1-3 Distances tab setup
    QVBoxLayout *danglayout; //!< Main layout for DANG tab page
    QGridLayout *danggrid; //!< 1-3 distances page (a la DANG)
    QLabel *d1label; //!< A label for d1 distance
    QLabel *d2label; //!< A label for d2 distance
    QLabel *angle; //!< Angle to be restrained
    QLabel *dlabel; //!< A label for d distance. This is a DANG distance

    QLineEdit *d1line; //!< Input line for d1 distance
    QLineEdit *d2line;//!< Input line for d2 distance
    QLineEdit *angleline; //!< Angel to be restrained
    QLineEdit *dline; //!< Input line for d distance. DANG distance
// Private members for DANG tab
    double _d1; //!< d1 distance
    double _d2; //!< d2 distance
    double _d; //!< d1,3 distance
    double _angle; //!< Angle between d1 and d2

};

#endif // CALCULATOR_H
